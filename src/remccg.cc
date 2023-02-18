#include "proto_to_c.h"
#include "../build/src/c_proto.pb.h"
#include <ostream>
#include <sstream>
#include <algorithm>
#include <stdlib.h>
#include <fstream>
#include <sys/stat.h> //used for get file size
#include <sys/sendfile.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <error.h>
#include <algorithm>
#include <memory>
#include <queue>
#include <random>
#include <set>
#include <vector>

// TODO Before running remccg, do make sure this path is correctly set in your machine
std::string smu_gcc4_path("/home/haoxin/haoxin-data/compilers/compiler-corpus/gcc-10.1.0/build/stage1-gcc/cc1");

std::string compiled_compiler = smu_gcc4_path;

// Flags
static bool smu_gcc = true;

static bool smu_clang = false;

static int timeout = 900; // to determine the potiential performance bugs

std::string extra_option_for_gcc("-w");
std::string extra_option_for_clang("-w");

static unsigned phase1 = 10;

struct testcase{
    unsigned seed;
    std::vector<int> grammar;
    int coverage;
};

static unsigned getseed(void)
{
    unsigned seed;
    int urandom = open("/dev/urandom", O_RDONLY);

    if(urandom == -1)
        die("couldn't open /dev/urandom for reading.");

    read(urandom, &seed, sizeof(unsigned));
    close(urandom);

    return seed;
}

bool cmp(std::pair<std::vector<unsigned long long>, int> a, std::pair<std::vector<unsigned long long>, int> b){
    return a.second > b.second;
}


float CaculateAcceptancePro(std::vector<int> tempVec, std::vector<int> tempVecP, int coverage_seed, int coverage_new){
    float ret = 0;
    float eucDistance = EuclideanDistance(tempVec, tempVecP);
    //std::cout << "//Eucdistance = " << eucDistance << std::endl;
    float alpha = 0.05; // constant for grammar distance
    float gama = 0.05; // constant for size difference
    float sigma = -0.005; // constant for acceptance probability
    //int sizeOrginal = 1024; //kb size no need for now
    float programDistance = alpha * eucDistance + gama * abs(coverage_new - coverage_seed);
    float compare = exp(sigma * programDistance);
    if (compare < 1.0)
        ret = compare;
    else
        ret = 1.0;
    return ret;
}

using namespace clang_fuzzer;

pid_t waitpid_eintr(int status) {
  pid_t pid = 0;
  while ( (pid = waitpid(WAIT_ANY, &status, 0)) == -1 ) {
    if (errno == EINTR) {
      continue;
    } else {
      perror("waitpid");
      abort();
    }
  }
  return pid;
}

void save_timeout_cases(unsigned long time, std::string opt, std::string name){
	char filename[PATH_MAX];
	snprintf(filename, sizeof(filename), "case-timeout-%s-%lu.c", opt.c_str(), time);
	fprintf(stderr,"Found a Timeout Bug here!!!!\n");
	fprintf(stderr, "Writing reproducer to %s\n", filename);

    int source = open(name.c_str(), O_RDONLY, 0);
    int dest = open(filename, O_WRONLY | O_CREAT, 0644);

    // struct required, rationale: function stat() exists also
    struct stat stat_source;
    fstat(source, &stat_source);

    sendfile(dest, source, 0, stat_source.st_size);

    close(source);
}

void insert_sort(std::vector<testcase> pq){
    int size = pq.size();
    for (int j = 1; j < size; j++){
        testcase key = pq[j];
        int i = j -1;
        while (i >= 0 && key.coverage < pq[i].coverage){
            pq[i+1] = pq[i];
            i--;
        }
        pq[i+1] = key;
    }
}


int main(int argc, char* argv[]){

    bool coverage_guided = 1;

    srand((unsigned)time(NULL));

	int devnull = open("/dev/null", O_RDWR);
	if (devnull == -1)
		error(EXIT_FAILURE, errno, "/dev/null: open()");

	FILE *devnullf = fdopen(devnull, "r+");
	if (!devnullf)
		error(EXIT_FAILURE, errno, "/dev/null: fdopen()");

	struct timeval tv_start;
	if (gettimeofday(&tv_start, 0) == -1)
		error(EXIT_FAILURE, errno, "gettimeofday()");

	static char stderr_filename[PATH_MAX];
	snprintf(stderr_filename, sizeof(stderr_filename), "stderr-%lu.txt", tv_start.tv_sec);

    std::vector<testcase> pq;

	unsigned int nu_execs = 0;
    unsigned int nu_crashes = 0;
    unsigned int nu_timeouts = 0;

    // record file size
    static int file_size;
    static unsigned seed_ccg;
    static std::vector<int> seed_grammar;
    static std::vector<int> tempVecP;
    static int seed_execs = 0;
    static int num_seed = 0;
    static int s = 100;
	while (true) {

        static int seed_count = 0;
		if (pq.empty()) {
            //MainFunction func;
            std::vector<int> initVec(10);
            testcase new_testcase;
            new_testcase.seed = s;
            new_testcase.grammar = initVec;
            new_testcase.coverage = 0;
            pq.push_back(new_testcase);

            s = s + 100;
            ++num_seed;
            seed_count ++;
            if (seed_count == 100){
                printf("Testing Done\n");
                exit(0);
            }
		}

        std::string selected_mutant_file;
        //TODO prioritize the selection
        auto it_selected = pq[random_generate(0, pq.size()-1)];
        seed_ccg = it_selected.seed;
        seed_grammar = it_selected.grammar;

		struct timeval tv;
		if (gettimeofday(&tv, 0) == -1)
			error(EXIT_FAILURE, errno, "gettimeofday()");

		int pipefd[2];
		if (pipe2(pipefd, 0) == -1)
			error(EXIT_FAILURE, errno, "pipe2()");

            selected_mutant_file = "0.c";

        // pipe on tempVecP
        int fd[2];
 	    int ret = pipe(fd);
        if (ret == -1){
            error (EXIT_FAILURE, errno, "pipe()");
            return -1;
        }

        pid_t child_no = fork();
        if (child_no == -1)
			error(EXIT_FAILURE, errno, "fork()");
        if (child_no == 0){
		    //fprintf(stderr, "This is the first child -O0\n");
            //10M = 10486700
            //fprintf(stderr, "Now testing coverage_guided mode(ours)!\n");
            MainFunction func;
            std::vector<float> allVec;
            std::vector<std::vector<int>> allGrammar;
            //fprintf(stderr,"phase1 = %d\n", phase1);
            phase1 += 1;
            for (int i = 1; i < phase1; i++){
                std::string filename = std::to_string(i) + ".c";
                freopen(filename.c_str(), "w", stdout);
                unsigned currentSeed = seed_ccg;
                std::vector<int> currentGrammar = seed_grammar;
                std::vector<int> tempVec = MainFunctionToString(func, currentSeed);
                allVec.push_back(EuclideanDistance(seed_grammar, tempVec));
                allGrammar.push_back(tempVec);
                std::cout << "// Eucdistance : " << EuclideanDistance(seed_grammar, tempVec) << std::endl;
		        //fprintf(stderr, "distance is %f\n", EuclideanDistance(seed_grammar, tempVec));
                std::cout << "//CCG Seed : " << currentSeed << std::endl;
                fclose(stdout);
            }
            std::vector<float>::iterator biggest = std::max_element(std::begin(allVec), std::end(allVec));
            //std::vector<float>::iterator smallest = std::min_element(std::begin(allVec), std::end(allVec));
		    fprintf(stderr, "The max_distance is %d.c\n", std::distance(std::begin(allVec), biggest) + 1);

            // store the intesting one as 0.c
            std::string max_filename = std::to_string(std::distance(std::begin(allVec), biggest) + 1) + ".c";
            int source = open(max_filename.c_str(), O_RDONLY, 0);
            int dest = open("0.c", O_WRONLY | O_CREAT | O_TRUNC, 0644);

            // struct required, rationale: function stat() exists also
            struct stat stat_source;
            fstat(source, &stat_source);

            sendfile(dest, source, 0, stat_source.st_size);

            close(source);
            close(dest);
		    //fprintf(stderr, "allGrammar size = %d\n", allGrammar.size());
            //std::vector<int> tempVecPP = allGrammar[std::distance(std::begin(allVec), biggest) + 1];
            tempVecP = allGrammar.at(std::distance(std::begin(allVec), biggest));
            allGrammar.clear();
            allVec.clear();
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			dup2(devnull, STDOUT_FILENO);

			int stderr_fd = open(stderr_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (stderr_fd == -1)
				error(EXIT_FAILURE, errno, "open()");

			dup2(stderr_fd, STDERR_FILENO);

            //pipe for tempVecP
            close(fd[0]);
            char tmp[12] = {0x0};
            for (int i = 0; i < tempVecP.size(); i++){
                int value = tempVecP[i];
                sprintf(tmp, "%d", value);
                //fprintf(stderr, "value = %d\n", value);
                write(fd[1], tmp, sizeof(tmp));
            }

            exit(0);
        }
    	close(fd[1]);
        std::vector<int> tempVecPP(10);
        for (int i = 0; i < tempVecPP.size(); i++){
            char tmp_f[12];
            read(fd[0], tmp_f, sizeof(tmp_f));
            //fprintf(stderr, "+++ In father's process tmp_f = %s\n", tmp_f);
            tempVecPP[i] = atoi(tmp_f);
        }
	    close(fd[0]); // clear pipe file

        int status;
		while (true) {
			pid_t kid = waitpid(child_no, &status, 0);
			if (kid == -1) {
				if (errno == EINTR || errno == EAGAIN)
					continue;
				error(EXIT_FAILURE, errno, "waitpid()");
			}

			if (kid != child_no)
				error(EXIT_FAILURE, 0, "kid != child");

			if (WIFEXITED(status) || WIFSIGNALED(status))
				break;
        }


        //fprintf(stderr, "+++ In father's process SquareSum(seed_grammar) = %d\n", SquareSum(seed_grammar));
        //fprintf(stderr, "+++ In father's process SquareSum(tempVecPP) = %d\n", SquareSum(tempVecPP));

        // Set timer for each child
        // Deal with -O0
        pid_t timer1_pid = fork();
        if (timer1_pid == -1) error(EXIT_FAILURE, errno, "fork timer1 failed");
        if (timer1_pid == 0){
            sleep(timeout);
            exit(0);
        }

	    pid_t p1 = fork();
        if (p1 == 0){
            //sleep(1);
            //fprintf(stderr, "This is the first child -O0\n");
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			dup2(devnull, STDOUT_FILENO);

			int stderr_fd = open(stderr_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (stderr_fd == -1)
				error(EXIT_FAILURE, errno, "open()");

			dup2(stderr_fd, STDERR_FILENO);
            //TODO Debug for timeout bug
			// SMU Machine
            // Test GCC
            if (smu_gcc){
                if (execlp(compiled_compiler.c_str(), "cc1","-w", "-quiet", "-g", "-O0",extra_option_for_gcc.c_str(), selected_mutant_file.c_str(), NULL) == -1)
				    error(EXIT_FAILURE, errno, "execvp()");
            }
            // Test Clang
            if (smu_clang){
			    if (execlp(compiled_compiler.c_str(),"clang", "-w", "-g", "-O0",extra_option_for_clang.c_str(), selected_mutant_file.c_str(), NULL) == -1)
				    error(EXIT_FAILURE, errno, "execvp()");
            }
            return 0;
        }
        int status1 = 0;
        pid_t finished_first1 = waitpid_eintr(status1);
        if (finished_first1 == timer1_pid){
            //printf("time out on -O0\n");
		    save_timeout_cases(nu_execs, "O0", selected_mutant_file);
            kill(p1, SIGKILL);
        }else{
            //printf("good on -O0\n");
            kill(timer1_pid, SIGKILL);
        }
        waitpid_eintr(status1);

        // Deal with -O1
        pid_t timer2_pid = fork();
        if (timer2_pid == -1) error(EXIT_FAILURE, errno, "fork timer2 failed");
        if (timer2_pid == 0){
            sleep(timeout);
            exit(0);
        }

        pid_t p2 = fork();
        if (p2 == 0){
            //sleep(1);
            //fprintf(stderr, "This is the second child -O1\n");
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			dup2(devnull, STDOUT_FILENO);

			int stderr_fd = open(stderr_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (stderr_fd == -1)
				error(EXIT_FAILURE, errno, "open()");

			dup2(stderr_fd, STDERR_FILENO);
            //TODO Debug for time-out bugs
            //sleep(1);
            // Test GCC
            if (smu_gcc){
                if (execlp(compiled_compiler.c_str(), "cc1","-w", "-quiet", "-g", "-O1",extra_option_for_gcc.c_str(), selected_mutant_file.c_str(), NULL) == -1)
                //if (execlp(compiled_compiler.c_str(), "tcc","-w", "-g" "-std=c99", extra_option_for_gcc.c_str(), selected_mutant_file.c_str(), NULL) == -1)
				    error(EXIT_FAILURE, errno, "execvp()");
            }
            // Test Clang
            if (smu_clang){
			    if (execlp(compiled_compiler.c_str(),"clang", "-w", "-g", "-O1",extra_option_for_clang.c_str(), selected_mutant_file.c_str(), NULL) == -1)
				    error(EXIT_FAILURE, errno, "execvp()");
            }
            return 0;
        }
        int status2 = 0;
        pid_t finished_first2 = waitpid_eintr(status2);
        if (finished_first2 == timer2_pid){
            //printf("time out on -O1\n");
		    save_timeout_cases(nu_execs, "O1", selected_mutant_file);
            kill(p2, SIGKILL);
        }else{
            //printf("good on -O1\n");
            kill(timer2_pid, SIGKILL);
        }
        waitpid_eintr(status2);


        // Deal with -O2
        pid_t timer3_pid = fork();
        if (timer3_pid == -1) error(EXIT_FAILURE, errno, "fork timer3 failed");
        if (timer3_pid == 0){
            sleep(timeout);
            exit(0);
        }
	    pid_t p3 = fork();
        if (p3 == 0){
            //sleep(1);
            //fprintf(stderr, "This is the third child -O2\n");

			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			dup2(devnull, STDOUT_FILENO);

			int stderr_fd = open(stderr_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (stderr_fd == -1)
				error(EXIT_FAILURE, errno, "open()");

			dup2(stderr_fd, STDERR_FILENO);
            //TODO Debug for time-out bug
            //sleep(1);
            // Test GCC
            if (smu_gcc){
                if (execlp(compiled_compiler.c_str(), "cc1","-w", "-quiet", "-g", "-O2", extra_option_for_gcc.c_str(), selected_mutant_file.c_str(), NULL) == -1)
				    error(EXIT_FAILURE, errno, "execvp()");
            }
            // Test Clang
            if (smu_clang){
			    if (execlp(compiled_compiler.c_str(),"clang", "-w", "-g", "-O2", extra_option_for_clang.c_str(), selected_mutant_file.c_str(), NULL) == -1)
				    error(EXIT_FAILURE, errno, "execvp()");
            }
            return 0;
        }
        int status3 = 0;
        pid_t finished_first3 = waitpid_eintr(status3);
        if (finished_first3 == timer3_pid){
            //printf("time out on -O2\n");
		    save_timeout_cases(nu_execs, "O2", selected_mutant_file);
            kill(p3, SIGKILL);
        }else{
            //printf("good on -O2\n");
            kill(timer3_pid, SIGKILL);
        }
        waitpid_eintr(status3);

        // Deal with -O3
        pid_t timer4_pid = fork();
        if (timer4_pid == -1) error(EXIT_FAILURE, errno, "fork timer4 failed");
        if (timer4_pid == 0){
            sleep(timeout);
            exit(0);
        }

	    pid_t p4 = fork();
        if (p4 == 0){
            //sleep(1);
            //fprintf(stderr, "This is the 4th child -O3\n");
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			dup2(devnull, STDOUT_FILENO);

			int stderr_fd = open(stderr_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (stderr_fd == -1)
				error(EXIT_FAILURE, errno, "open()");

			dup2(stderr_fd, STDERR_FILENO);
            //TODO Debug for time-out bug
            // Test GCC
            if (smu_gcc){
                if (execlp(compiled_compiler.c_str(), "cc1","-w", "-quiet", "-g", "-O3", extra_option_for_gcc.c_str(), selected_mutant_file.c_str(), NULL) == -1)
				    error(EXIT_FAILURE, errno, "execvp()");
            }
            // Test Clang
            if (smu_clang){
			    if (execlp(compiled_compiler.c_str(),"clang", "-w", "-g", "-O3", extra_option_for_clang.c_str(), selected_mutant_file.c_str(), NULL) == -1)
				    error(EXIT_FAILURE, errno, "execvp()");
            }
            return 0;
        }
        int status4 = 0;
        pid_t finished_first4 = waitpid_eintr(status4);
        if (finished_first4 == timer4_pid){
            //printf("time out on -O3\n");
		    save_timeout_cases(nu_execs, "O3", selected_mutant_file);
            kill(p4, SIGKILL);
        }else{
            //printf("good on -O3\n");
            kill(timer4_pid, SIGKILL);
        }
        waitpid_eintr(status4);

        // Deal with -Os
        pid_t timer5_pid = fork();
        if (timer5_pid == -1) error(EXIT_FAILURE, errno, "fork timer5 failed");
        if (timer5_pid == 0){
            sleep(timeout);
            exit(0);
        }

	    pid_t p5 = fork();
        if (p5 == 0){
            //sleep(1);
            //fprintf(stderr, "This is the 5th child -Os\n");
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			dup2(devnull, STDOUT_FILENO);

			int stderr_fd = open(stderr_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (stderr_fd == -1)
				error(EXIT_FAILURE, errno, "open()");

			dup2(stderr_fd, STDERR_FILENO);
            // TODO Debug for time-out bugs
            // Test GCC
            if (smu_gcc){
                if (execlp(compiled_compiler.c_str(), "cc1","-w", "-quiet", "-g", "-Os", extra_option_for_gcc.c_str(), selected_mutant_file.c_str(), NULL) == -1)
				    error(EXIT_FAILURE, errno, "execvp()");
            }
            // Test Clang
            if (smu_clang){
			    if (execlp(compiled_compiler.c_str(),"clang", "-w", "-g", "-Os", extra_option_for_clang.c_str(), selected_mutant_file.c_str(), NULL) == -1)
				    error(EXIT_FAILURE, errno, "execvp()");
            }
            return 0;
        }
        int status5 = 0;
        pid_t finished_first5 = waitpid_eintr(status5);
        if (finished_first5 == timer5_pid){
            //printf("time out on -Os\n");
		    save_timeout_cases(nu_execs, "Os", selected_mutant_file);
            kill(p5, SIGKILL);
        }else{
            //printf("good on -Os\n");
            kill(timer5_pid, SIGKILL);
        }
        waitpid_eintr(status5);


		close(pipefd[0]);
		FILE *f = fdopen(pipefd[1], "w");
		if (!f)
			error(EXIT_FAILURE, errno, "fdopen()");
		fclose(f);

		++nu_execs;

		{
			FILE *f = fopen(stderr_filename, "r");
			if (!f)
				error(EXIT_FAILURE, errno, "fopen()");

			static char buffer[10 * 4096 * 1024];
			size_t len = fread(buffer, 1, sizeof(buffer), f);
			fclose(f);

			if (len > 0) {
				buffer[len - 1] = '\0';
				// Check for ICEs, but ignore a set of specific ones which we've already reported and which keep showing up.
				// For GCC
                //fprintf(stderr,"Output Buffer: %s\n", buffer);
                if (smu_gcc){
                    if ((strstr(buffer, "internal") || strstr(buffer, " fault") || strstr(buffer, "fatal")) || strstr(buffer, "internal")) {//&& !strstr(buffer, "types may not be defined in parameter types") && !strstr(buffer, "internal compiler error: in synthesize_implicit_template_parm") && !strstr(buffer, "internal compiler error: in search_anon_aggr") && !strstr(buffer, "non_type_check") && !strstr(buffer, "internal compiler error: in xref_basetypes, at") && !strstr(buffer, "internal compiler error: in build_capture_proxy") && !strstr(buffer, "internal compiler error: tree check: expected record_type or union_type or qual_union_type, have array_type in reduced_constant_expression_p") && !strstr(buffer, "internal compiler error: in cp_lexer_new_from_tokens") && !strstr(buffer, "internal compiler error: in extract_constrain_insn") && !strstr(buffer, "in lra_eliminate_reg_if_possible") && !strstr(buffer, "Max. number of generated reload insns per insn is achieved") && !strstr(buffer, "standard_conversion") && !strstr(buffer, "in pop_local_binding") && !strstr(buffer, "of kind implicit_conv_expr") && !strstr(buffer, "in cp_build_addr_expr_1") && !strstr(buffer, "in poplevel_class, at") && !strstr(buffer, "force_constant_size")) {
		                ++nu_crashes;
					    char filename[PATH_MAX];
					    snprintf(filename, sizeof(filename), "case-crash-%lu.c", nu_execs);
                        //fprintf(stderr,"filename = %s\n", filename);
					    //printf("Writing reproducer to %s\n", filename);

					    fprintf(stderr,"Find a Crash Bug here!!!!\n");
					    fprintf(stderr, "Writing reproducer to %s\n", filename);
                        fprintf(stderr,"%s\n", buffer);
                        // copy file
                        int source = open(selected_mutant_file.c_str(), O_RDONLY, 0);
                        int dest = open(filename, O_WRONLY | O_CREAT, 0644);

                        // struct required, rationale: function stat() exists also
                        struct stat stat_source;
                        fstat(source, &stat_source);

                        sendfile(dest, source, 0, stat_source.st_size);

                        close(source);
                        close(dest);

                        //clear stderr_filename
                        fclose(fopen(stderr_filename, "w"));
                    }
				}
		        if (smu_clang){
                    if ((strstr(buffer, "Assertion") || strstr(buffer, "Please"))){
		                ++nu_crashes;
                        char filename[PATH_MAX];
					    snprintf(filename, sizeof(filename), "case-crash-%lu.c", nu_execs);
                        //fprintf(stderr,"filename = %s\n", filename);
			            fprintf(stderr,"Found a Crash Bug here!!!!\n");
					    fprintf(stderr, "Writing reproducer to %s\n", filename);
                        //fprintf(stderr,"%s\n", buffer);

                        // TODO do not save file for now
                        FILE* clang_output = fopen("clang-output.txt", "a+");
                        fputs(buffer, clang_output);
                        fclose(clang_output);
                        fclose(fopen(stderr_filename, "w"));
                    }
				}
			}
		}
        fclose(fopen(stderr_filename, "w"));
		int success = WIFEXITED(status) && WEXITSTATUS(status) == 0;
		fprintf(stderr, "\e[33m Results: %u test program compiled: found %u crashes and %u timeouts \e[0m \n", nu_execs, nu_crashes,  nu_timeouts);
	}
    return 0;
}

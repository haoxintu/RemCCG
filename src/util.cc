#include "util.h"


int random_generate(int min,int max){
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<unsigned long long> dist(min,max);
    auto dice = bind(dist, mt);
    return dice();
}

long long random_char(){
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<char> dist(CHAR_MIN,CHAR_MAX);
    auto dice = bind(dist, mt);
    return dice();
}
unsigned long long random_uchar(){
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<unsigned char> dist(0,UCHAR_MAX);
    auto dice = bind(dist, mt);
    return dice();
}
long long random_wchar(){
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<wchar_t> dist(WCHAR_MIN,WCHAR_MAX);
    auto dice = bind(dist, mt);
    return dice();
}
unsigned long long random_uwchar(){
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<wchar_t> dist(0,WCHAR_MAX);
    auto dice = bind(dist, mt);
    return dice();
}
long long random_short(){
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<short> dist(SHRT_MIN,SHRT_MAX);
    auto dice = bind(dist, mt);
    return dice();
}
unsigned long long random_ushort(){
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<unsigned short> dist(0,USHRT_MAX);
    auto dice = bind(dist, mt);
    return dice();
}
long long random_int(){
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<int> dist(INT_MIN,INT_MAX);
    auto dice = bind(dist, mt);
    return dice();
}
unsigned long long random_uint(){
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<unsigned int> dist(0,UINT_MAX);
    auto dice = bind(dist, mt);
    return dice();
}
long long random_long(){
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<long> dist(LONG_MIN,LONG_MAX);
    auto dice = bind(dist, mt);
    return dice();
}
unsigned long long random_ulong(){
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<unsigned long> dist(0,ULONG_MAX);
    auto dice = bind(dist, mt);
    return dice();
}
long long random_llong(){
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<long long> dist(LLONG_MIN,LLONG_MAX);
    auto dice = bind(dist, mt);
    return dice();
}
unsigned long long random_ullong(){
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<unsigned long long> dist(0,ULLONG_MAX);
    auto dice = bind(dist, mt);
    return dice();
}

float random_float(){
    std::default_random_engine e;
    std::uniform_real_distribution<float> u(3.40282e+038, 1.17549e-038);
    return u(e);
}

double random_double(){
    std::default_random_engine e;
    std::uniform_real_distribution<double> u(1.79769e+308, 2.22507e-308);
    return u(e);
}

float EuclideanDistance(std::vector<int> seed, std::vector<int> p){
    if (seed.size() != p.size()) {
        std::cout << "//Size not meet!" << std::endl;
        //exit(1);
    }
    float sum = 0;
    for (int i = 0; i < seed.size(); i++){
        sum += pow(seed[i] - p[i], 2);
    }
    return sqrt(sum);
}

int SquareSum(std::vector<int> vec){
    long sum = 0;
    for (int i = 0; i < vec.size(); i++){
        sum += pow(vec[i], 2);
    }
    return sum;
}

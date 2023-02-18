//==-- proto_to_cxx.cpp - Protobuf-C++ conversion --------------------------==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements functions for converting between protobufs and C++.
//
//===----------------------------------------------------------------------===//

#include "proto_to_c.h"
#include "../build/src/c_proto.pb.h"
#include <ostream>
#include <sstream>
#include <search.h>

Program program;
void *rootMakeLabel = 0;
void *root = 0;
static unsigned seed_ccg_init;

#ifdef DEBUG_MEMORY
extern unsigned long long int allocated_bytes;
#endif

//static void printProgram(void)
void printProgram(void)
{
    FunctionList *f;
    VariableList *v;
    foreach(v, program.globalvars)
        printVariableDecl(v->variable);
}

static Context *cc ;//= (Context*) xmalloc(sizeof(*cc));
static bool isFunction;

int isFind(std::vector<std::string> vec, std::string name){
    for (auto i : vec){
        if (i == name) {
            //printf("//Both have %s\n", name.c_str());
            return 1;
        }
    }
    return 0;
}


static std::vector<std::string> labelNameForFunction;
static std::vector<std::string> labelNameForFunctionL;
void insertStatementForFunction(Context *cc1){
    //cc->currfunc->numlabels = 0;
   Statement *ret = makeStatementForFunction(cc1, 5, 1);
   printStatementForFunction(ret, 1);
}
static std::vector<std::string> labelName;
void insertStatement(Context *cc1){
   //Statement *ret = makeStatement(cc1, 8, 0);
   printf("//+++++++insertStatement start\n");
   Block *ret1 = makeBlockForFunction(cc1, 2);
   //Statement *ret = makeStatement(cc1, 8, 0);
   printBlockForFunction(ret1);
   printf("//------insertStatement done\n");
}

void insertStatements(){
    insertStatement(cc);
}

void insertExpression(Context* cc_expr) {

    //std::cout << "\n//Print expression Start" << std::endl;
    Expression *expr = makeExpression(cc, 15);
    printExpression(expr);
}

void insertStruct(){
    printf("ss.a = ");
    insertExpression(cc);
    printf(";\n");
    printf("ss.b = ");
    insertExpression(cc);
    printf(";\n");
    printf("ss.c = ");
    insertExpression(cc);
    printf(";\n");
    printf("ss.d = ");
    insertExpression(cc);
    printf(";\n");
    printf("ss.e = ");
    insertExpression(cc);
    printf(";\n");
}
namespace clang_fuzzer {

const int array_size = 100;

// Collect the number of calling times of each statement
static int numPimaryExpr = 0;
static int numAssignmentStmt = 0;
static int numWhileLoopStmt = 0;
static int numDowhileLoopStmt = 0;
static int numForLoopStmt = 0;
static int numIfElseStmt = 0;
static int numIfStmt = 0;
static int numSwithStmt = 0;
static int numLabeledStmt = 0;
static int numExprStmt = 0;
static int numCompStmt = 0;
static int numJumpStmt = 0;

int numlimit = 20;
static int depth = 0;
const int max_depth = 15;
//std::vector<int> retVec;
//Define fixed specifier

//storageClassSpecifier
std::vector<std::string> assignedGlobalStorageClassSpecifier{"static ", "_Thread_local ", "auto ", "register "};
std::vector<std::string> assignedLocalStorageClassSpecifier{"static ", "auto ", "register "};
std::vector<std::string> NotAssignedLocalStorageClassSpecifier{"typedef ", "extern "};

//TypeSpecifier
std::vector<std::string> simpleType{"char ", "short ", "int ", "long ", "signed ", "unsigned ", "_Bool "};//, "_Complex ", "__m128 ", "__m128d ", "__m128i "};
    //"__extension__ (__m128) ", "__extension__ (__m128d) ", "__extension__ (__m128i) "};


std::string randome_pick_one(std::vector<std::string> vec_string){
    return vec_string[random_generate(0, vec_string.size()-1)];
}
// Proto to C++.
// forward decls
std::ostream &operator<<(std::ostream &os, const AssignmentStatement &assignment_statement);
std::ostream &operator<<(std::ostream &os, const StatementSeq &statement_seq);
std::ostream &operator<<(std::ostream &os, const Statement1 &statement);
std::ostream &operator<<(std::ostream &os, const PrimaryExpression &x);
std::ostream &operator<<(std::ostream &os, const Identifier &x);
std::ostream &operator<<(std::ostream &os, const Constant &x);
std::ostream &operator<<(std::ostream &os, const StringLiteral &x);
std::ostream &operator<<(std::ostream &os, const Expression &x);
std::ostream &operator<<(std::ostream &os, const GenericSelection &x);
std::ostream &operator<<(std::ostream &os, const VariableArgument &x);
std::ostream &operator<<(std::ostream &os, const OffSetOf &x);
std::ostream &operator<<(std::ostream &os, const PostfixExpression &x);
std::ostream &operator<<(std::ostream &os, const UnaryExpression &x);
std::ostream &operator<<(std::ostream &os, const AtomicTypeSpecifier &x);
std::ostream &operator<<(std::ostream &os, const StructOrUnionSpecifier &x);
std::ostream &operator<<(std::ostream &os, const EnumSpecifier &x);

//From ccg
static std::vector<std::string> ptrName;
static std::vector<std::string> normalVariableName;


std::ostream &operator<<(std::ostream &os, const PrimaryExpression &x){
    numPimaryExpr += 1;
    //std::cout << "//Here is PrimaryExpression : size of normalVariableName = " << normalVariableName.size() << "\n";
    std::cout << x.identifier() << ";\n";
    std::cout << x.constant() << ";\n";
    std::cout << x.string_literal() << ";\n";
    std::cout << x.expression() << ";\n";
    std::cout << x.generic_selection() << ";\n";
    std::cout << x.variable_argument() << ";\n";
    std::cout << x.off_set_off() << ";\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Identifier &identifier){
    std::cout << randome_pick_one(normalVariableName) << ";";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Constant &con){
    std::cout << random_ullong() << ";";
    return os;
}

std::ostream &operator<<(std::ostream &os, const StringLiteral &x){
    std::cout << "\"randome hello world!\"";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Expression &x){
    insertExpression(cc);
    std::cout << ";";
    return os;
}

std::ostream &operator<<(std::ostream &os, const GenericSelection &x){
    return os;
}

std::ostream &operator<<(std::ostream &os, const VariableArgument &x){
   // std::cout << "va_list arg;\n";
    std::cout << "__builtin_va_arg (arg, " << randome_pick_one(simpleType) << ");\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, const OffSetOf &x){

    std::cout << "__builtin_offsetof(struct S, a);\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, const PostfixExpression &x){
    std::cout << randome_pick_one(normalVariableName) << " = " << "( __extension__) (" << randome_pick_one(simpleType) << ")"
        << "{ " << randome_pick_one(normalVariableName) << random_ullong() << "hello" << "});\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, const UnaryExpression &x){
    printf("//normalVariableName size = %lu\n", normalVariableName.size());
    std::cout << randome_pick_one(normalVariableName) <<  " = ++" << randome_pick_one(normalVariableName) << ";\n";
    std::cout << randome_pick_one(normalVariableName) <<  " = --" << randome_pick_one(normalVariableName) << ";\n";
    std::cout << randome_pick_one(normalVariableName) << " = sizeof(" << randome_pick_one(normalVariableName) << ");\n";
    std::cout << randome_pick_one(normalVariableName) << " = sizeof(" << randome_pick_one(simpleType) << ");\n";
    //TODO add this feature
    //std::cout << "&& " << randome_pick_one(labelName) << ";\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, const AtomicTypeSpecifier &x){
    return os;
}

std::ostream &operator<<(std::ostream &os, const StructOrUnionSpecifier &x){


    return os;
}

std::ostream &operator<<(std::ostream &os, const EnumSpecifier &x){

    return os;
}

std::ostream &operator<<(std::ostream &os, const Const &x) {
    insertExpression(cc);
    return os;
}

std::ostream &operator<<(std::ostream &os, const VarRef &x) {
    std::cout<< normalVariableName[random_generate(0, normalVariableName.size()-1)];
    return os;
}
std::ostream &operator<<(std::ostream &os, const Rvalue &x) {
    if (x.has_cons()){
        x.cons();
        return os;
    }
    if (x.has_binop()){
        x.binop();
        return os;
    }
    if (x.has_varref())
        std::cout<< x.varref();
    //std::cout<< "1";

    return os;
}
std::ostream &operator<<(std::ostream &os, const BinaryOp &x) {
    std::cout << "(" << x.left();
    switch (x.op()) {
        case BinaryOp::PLUS: std::cout << "+"; break;
        case BinaryOp::MINUS: std::cout << "-"; break;
        case BinaryOp::MUL: std::cout << "*"; break;
        case BinaryOp::XOR: std::cout << "^"; break;
        case BinaryOp::AND: std::cout << "&"; break;
        case BinaryOp::OR: std::cout << "|"; break;
        case BinaryOp::EQ: std::cout<< "=="; break;
        case BinaryOp::NE: std::cout << "!="; break;
        case BinaryOp::LE: std::cout << "<="; break;
        case BinaryOp::GE: std::cout << ">="; break;
        case BinaryOp::LT: std::cout << "<"; break;
        case BinaryOp::GT: std::cout << ">"; break;
  }
    std::cout << x.right() << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, const LabeledStatement &labeled_statement) {
    numLabeledStmt += 1;
    /*
    depth +=1;
    if (depth > max_depth){
        return os;
    }
    */
    printf("//Add labeled statement\n");

    unsigned int index = random_uint();
    printf("L_%u: \n", index);
    insertExpression(cc);
    printf(";\n");
    std::string label = "L_" + std::to_string(index);
    //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
    //    os << labeled_statement.statements();
        //return os;
    //else
        os << labeled_statement.assignment();
    //labelName.push_back(label);
    labelNameForFunctionL.push_back(label);
    return os;
}

std::ostream &operator<<(std::ostream &os, const CompoundStatement &compound_statement) {
    numCompStmt += 1;
    /*
    depth +=1;
    if (depth > max_depth){
        return os;
    }
    */
    printf("//Add compound statement\n");
    printf("{\n");
    //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
     //   os << compound_statement.assignment();
        //return os;
    //else
    insertStatement(cc);
        os << compound_statement.statements();
    printf("}\n");
    return os;
}

std::ostream &operator<<(std::ostream &os, const ExpressionStatement &expression_statement) {
    numExprStmt += 1;
    /*
    depth +=1;
    if (depth > max_depth){
        return os;
    }
    */
    printf("//Add expression statement\n");
    insertExpression(cc);
    printf(";\n");
    return os;
}

std::ostream &operator<<(std::ostream &os, const JumpStatement &jump_statement) {
    numJumpStmt += 1;
    /*
    depth +=1;
    if (depth > max_depth){
        return os;
    }
    */
    printf("//Add jump_statement\n");
    bool j = random_generate(0,1);
    printf("// j = %d\n", j);
    if (j == 0) {
        if (labelName.size() > 0)
            printf("goto %s;\n", labelName[random_generate(0, labelName.size()-1)].c_str());
        else
            printf("return 0;\n");
    }
    else{
        printf("return ");
        insertExpression(cc);
        printf(";\n");
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const AssignmentStatement &assignment_statement) {
    numAssignmentStmt += 1;
    /*
    depth +=1;
    if (depth > max_depth){
        return os;
    }
    */
    //std::cout<< "    array[" << random_generate(0,array_size) << "] = " << random_int() << ";\n" ;
    printf("//Add assignment statement\n");
    //printf("    array[%d] = %llu;\n", random_generate(0, array_size), random_int()) ;
    //assignment_statement.varref();
    if (normalVariableName.size() !=0)
        printf("%s ", normalVariableName[random_generate(0, normalVariableName.size()-1)].c_str());
        //printf("aaa");
    else
        printf("aaa");
    printf("  =  ");
    insertExpression(cc);
    printf(";\n");
    printf("//isFunction in AssignmentStatement = %d\n", isFunction);
    //if (isFunction == 1){
        //printf("//Now print block in proto, numlabels size = %d\n", cc->currfunc->numlabels);
        //cc->currfunc->numlabels = 0;
    //    insertStatementForFunction(cc);
    insertStatements();
    //insertStatement(cc);
    //}

    return os;
    //return std::cout<< x.varref() << "=" << x.rvalue() << ";\n";
}

std::ostream &operator<<(std::ostream &os, const IfElseStatement &if_else_stmt) {
    numIfElseStmt += 1;
    depth +=1;
    if (depth > max_depth){
        return os;
    }
    printf("//Add if_else statement depth = %d \n", depth);
    printf("  if (");
    insertExpression(cc);
    printf("){\n");
    //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
    //    os << if_else_stmt.if_body().statements().assignment();
        //return os;
    //else
    insertStatements();
        os << if_else_stmt.if_body();
    printf("}\n");
    printf("  else {\n");
    //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
    //    os << if_else_stmt.else_body().statements().assignment();
        //return os;
    //else
    insertStatements();
        os << if_else_stmt.else_body();
    //insertStatement(cc);
    printf(" }\n");
    return os;
}

std::ostream &operator<<(std::ostream &os, const IfStatement &if_stmt) {
    numIfStmt += 1;
    depth +=1;
    if (depth > max_depth){
        return os;
    }
    //if (depth > 10)
    //    return os;
    printf("//Add if statement depth = %d \n", depth);
    printf("  if (");
    insertExpression(cc);
    printf("){\n");
    //insertStatement(cc);
    printf("//inserttttt\n");
    insertStatements();
    //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
    printf("//insert assignment\n");
    os << if_stmt.if_body().statements().assignment();
        //return os;
    //else
    os << if_stmt.if_body();
    printf("}\n");
    return os;
}

std::ostream &operator<<(std::ostream &os, const SwitchStatement switch_stmt){
    numSwithStmt += 1;
    depth +=1;
    if (depth > max_depth){
        return os;
    }
    //TODO change the body
    printf("//Add switch statement depth = %d \n", depth);
    /*
    printf("     switch ( ");
    insertExpression(cc);
    printf("){\n");
    for (int i = 0; i < 20; i++){
        printf( "    case %d :{\n", i);
        if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
            os << switch_stmt.switch_statement().statements().assignment();
        else
            os << switch_stmt.switch_statement().statements();
        insertStatement(cc);
        if (random_generate(0,1)) {
            printf("break;\n");
        }
        printf("     }\n");
    }
    printf("    default:{\n");
    //insertStatements();
    insertStatement(cc);
    if (random_generate(0,1)) printf("break;\n");
    printf("    }}\n");
    */
    std::string var = randome_pick_one(normalVariableName);
    //printf("variable = %s\n", var.c_str());
    unsigned int index = random_uint();
    //printf("    int var_%u = %d;\n",index, random_generate(0,20));
    printf("    %s = %d;\n", var.c_str(), random_generate(3,10));
    printf("     switch  ");
    printf(" ( %s )", var.c_str());
    printf("{\n");
    for (int i = 0; i < 10; i++){
        printf( "    case %d :{\n", i);
        //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
        //    os << switch_stmt.switch_statement().statements().assignment();
        //else
            insertStatements();
            os << switch_stmt.switch_statement();
        if (random_generate(0,1)) {
            printf("break;\n");
        }
        printf("     }\n");
    }
    printf("    default:{\n");
    insertStatements();
            os << switch_stmt.switch_statement();
    if (random_generate(0,1)) printf("break;\n");
    printf("    }}\n");

    //add orginal statement
    printf("  switch");
    printf(" (");
    insertExpression(cc);
    printf(" )");
    printf(" {\n");
    //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
    //    os << switch_stmt.switch_statement().statements().assignment();
    //else
            insertStatements();
        os << switch_stmt.switch_statement();
    printf(" }\n");
    return os;
}

std::ostream &operator<<(std::ostream &os, const WhileLoopStatement &while_loop_statement){
    numWhileLoopStmt += 1;
    depth +=1;
    if (depth > max_depth){
        return os;
    }
    printf("//Add while-loop statement depth = %d \n", depth);
    /*
    printf("while (");
    insertExpression(cc);
    printf(")");
    printf(" {\n");
    if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
        os << while_loop_statement.asssignment_statement();
    else
        os << while_loop_statement.statement();
    insertStatement(cc);
    //os << while_loop_statement.statement();
    printf("    }");
    */
    std::string var = randome_pick_one(normalVariableName);
    int var_condition = random_generate(-10, 10);
    std::vector<std::string> up_operator{"+= ","+= "};
    std::vector<std::string> down_operator{"-= ","-= "};
    //unsigned int index = random_uint();
    printf("    %s = %d;\n", var.c_str(), var_condition);
    if (var_condition > 0){
        printf("    while");
        bool mm = random_generate(0,1);
        if (mm == 1){
            printf(" ( %s -- )", var.c_str());
            printf(" {\n");
        //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
        //    os << while_loop_statement.asssignment_statement();
        //else
            insertStatements();
            os << while_loop_statement.statement();
            printf("    }\n");
        }else {
            printf(" ( %s  > 2){\n", var.c_str());
            printf("\n    %s  %s %d ;\n ", var.c_str(),  down_operator[random_generate(0,1)].c_str(), random_generate(2,5));
            //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
            //    os << while_loop_statement.asssignment_statement();
            //else
            insertStatements();
                os << while_loop_statement.statement();
            printf("    }\n");
        }
    }else if (var_condition < 0){
        printf("    while");
        bool pp = random_generate(0, 1);
        if (pp == 1){
            printf(" (%s ++ ) {\n", var.c_str());
            //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
            //    os << while_loop_statement.asssignment_statement();
            //else
            insertStatements();
                os << while_loop_statement.statement();
            printf("    }\n");
        }else {
            printf(" ( %s < 100){\n", var.c_str());
            printf("\n    %s %s %u ;\n ", var.c_str(),  up_operator[random_generate(0,1)].c_str(), random_generate(2,5));
            //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
            //    os << while_loop_statement.asssignment_statement();
            //else
            insertStatements();
                os << while_loop_statement.statement();
            printf("    }\n");
        }
    }
    else {
        printf("     while(0){");
        //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
        //    os << while_loop_statement.asssignment_statement();
        //else
            insertStatements();
            os << while_loop_statement.statement();
        printf("    }\n");
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const DoWhileLoopStatement &do_while_loop_statement){
    numDowhileLoopStmt += 1;
    depth +=1;
    if (depth > max_depth){
        return os;
    }
    printf("//Add do-while-loop statement depth = %d\n", depth);
    /*
    printf("    do");
    printf(" {\n");
    if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
        os << do_while_loop_statement.asssignment_statement();
    else
        os << do_while_loop_statement.statement();
    insertStatement(cc);
    printf("    } while");
    printf(" ( ");
    insertExpression(cc);
    printf(" );\n ");
    */

    std::string var = randome_pick_one(normalVariableName);
    int var_condition = random_generate(-10,10);
    std::vector<std::string> up_operator{"+= ", "+= "}; // *=
    std::vector<std::string> down_operator{"-= ", "-= "}; // /=
    //unsigned int index = random_uint();
    printf("    %s = %d;\n", var.c_str(), var_condition);
    if (var_condition > 0){
        printf("    do");
        bool mm = random_generate(0,1);
        if (mm == 1){
            printf(" {\n");
        //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
        //    os << do_while_loop_statement.asssignment_statement();
        //else
            insertStatements();
            os << do_while_loop_statement.statement();
            printf("    } while");
            printf(" ( %s --);\n", var.c_str());
        }else {
            printf("{\n    %s %s %d;\n ", var.c_str(), down_operator[random_generate(0,1)].c_str(), random_generate(2,5));
            //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
            //    os << do_while_loop_statement.asssignment_statement();
            //else
            insertStatements();
                os << do_while_loop_statement.statement();
            printf("    }while");
            printf(" (%s > 2 );\n", var.c_str());
        }
    }else if (var_condition < 0){
        printf("    do");
        bool pp = random_generate(0,1);
        if (pp == 1){
            printf(" {\n");
            //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
            //    os << do_while_loop_statement.asssignment_statement();
            //else
            insertStatements();
                os << do_while_loop_statement.statement();
            printf("    }while");
            printf(" ( %s ++ );\n", var.c_str());
        }else {
            printf("{\n    %s %s %u ;\n ", var.c_str(),  up_operator[random_generate(0,1)].c_str(), random_generate(2,5));
            //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
            //    os << do_while_loop_statement.asssignment_statement();
            //else
            insertStatements();
                os << do_while_loop_statement.statement();
            printf("    }while");
            printf(" ( %s < 100 );\n", var.c_str());
        }
    }
    else {
        printf("  do{\n");
            //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
            //    os << do_while_loop_statement.asssignment_statement();
            //else
            insertStatements();
                os << do_while_loop_statement.statement();
        printf("    }while( 0 );\n");
    }
    return os;

}

std::ostream &operator<<(std::ostream &os, const ForLoopStatement &for_loop_statement){
    numForLoopStmt += 1;
    depth +=1;
    if (depth > max_depth){
        return os;
    }
    printf("//Add for-loop statement depth = %d \n", depth);
    /*
    printf("for(");
    insertExpression(cc);
    printf(";");
    insertExpression(cc);
    printf(";");
    insertExpression(cc);
    printf("){\n");
    if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
        os << for_loop_statement.asssignment_statement();
    else
        os << for_loop_statement.statement();
    insertStatement(cc);
    printf("}\n");
    */

    std::string var = randome_pick_one(normalVariableName);
    int var_condition = random_generate(-10,10);
    std::vector<std::string> up_operator{"+= ", "+= "}; // *=
    std::vector<std::string> down_operator{"-=", "-="}; // /=
    //unsigned int index = random_uint();
    //std::cout<< "    int var_" << index << " = " << var_condition << ";\n";
    printf("    %s = %d;\n", var.c_str(), var_condition);
    if (var_condition > 0){
        printf("    for");
        bool mm = random_generate(0,1);
        if (mm == 1){
            printf(" ( ; %s > 2; %s -- ){\n", var.c_str(), var.c_str()) ;
        //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
        //    os << for_loop_statement.asssignment_statement();
        //else
            insertStatements();
            os << for_loop_statement.statement();
        printf("    }\n");
        }else{
            printf(" ( ; %s > 2; %s %s %d ){\n ", var.c_str(), var.c_str(), down_operator[random_generate(0,1)].c_str(), random_generate(2,5));
            insertStatements();
            os << for_loop_statement.statement();
            printf("    }\n");
        }
    }else if (var_condition < 0){
        printf("    for");
        bool pp = random_generate(0,1);
        if (pp == 1){
            printf(" ( ; %s < 100; %s ++ ){\n", var.c_str(), var.c_str()) ;
            //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
            //    os << for_loop_statement.asssignment_statement();
            //else
            insertStatements();
                os << for_loop_statement.statement();
            printf("    }\n");
        }else {
            printf(" ( ; %s > 100; %s %s %d ){\n ", var.c_str(), var.c_str(), up_operator[random_generate(0,1)].c_str(), random_generate(2,5));
            //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
            //    os << for_loop_statement.asssignment_statement();
            //else
            insertStatements();
                os << for_loop_statement.statement();
            printf("    }\n");
        }
    }
    else {
        printf("    for (; 0; ){\n");
        //if (numIfStmt > numlimit || numSwithStmt > numlimit || numForLoopStmt > numlimit || numWhileLoopStmt > numlimit || numIfElseStmt > numlimit)
        //    os << for_loop_statement.asssignment_statement();
        //else
            insertStatements();
            os << for_loop_statement.statement();
        printf("    }\n");
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, const Statement1 &statement) {
    int var = random_generate(1,11);
    switch (var) {
        case 1 : os << statement.assignment(); break;
        case 2 : os << statement.while_loop_statement();break;
        case 3 : os << statement.do_while_loop_statement();break;
        case 4 : os << statement.for_loop_statement(); break;
        case 5 : os  << statement.if_else_statement(); break;
        case 6 : os << statement.if_statement(); break;
        case 7 : os << statement.switch_statement(); break;
        case 10 : os << statement.labeled_statement(); break;
        case 8 : os << statement.expression_statement();;break;
        case 9: os << statement.compound_statement(); break;
        case 11: os << statement.jump_statement();break;

        //default : os << statement.while_loop_statement();break;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const StatementSeq_last & statement_seq_last){
    //depth +=1;
    os << statement_seq_last.statement_seq();
    os << statement_seq_last.statement();
    return os;
}
std::ostream &operator<<(std::ostream &os, const StatementSeq &statement_seq) {
    int var = random_generate(1,3);
    //if (depth > 10){
    //    depth = 0;
    //    return os;
    //}
    //depth = 0;
    printf("//depth = %d \n", depth);
    switch (var) {
        case 1 : os << statement_seq.statements();return os;
        case 2 : os << statement_seq.statement_seq_last(); return os;
        default : os << statement_seq.statement_seq_last();return os;
    }
}


  //  std::vector<std::string> ptrName;
  //  std::vector<std::string> normalVariableName;
std::ostream &operator<<(std::ostream &os, const LoopFunction &loop_function) {
    //printf("\nint loop_func(){\n");
    //printf("    func_0();\n");
    /*
    StatementList *s;
    FunctionList *f;
    foreach (f, program.functions){
        foreach(s, f->function->body->statementlist)
           ;//// printStatement(s->statement);
    }
    */
    //For PrimaryExpression
    FunctionList *f;

    std::vector<std::string> paramlistName;
    foreach(f, program.functions){
        //printFunction(f->function);
        VariableList *v;
        foreach(v, f->function->paramlist){
            std::string var_name = v->variable->name;
            paramlistName.push_back(var_name);
            }
	}

    foreach(f, program.functions){
        //printFunction(f->function);
        VariableList *v;

        Block* block = f->function->body;
        foreach(v, block->localvars){
            std::string name = v->variable->name;
            if (v->variable->type != _integer){
                //printVariableDecl(v->variable);
                ptrName.push_back(name);
            }
        }
    }
    printf("\nint loop_func(){\n");
    //TODO
    Statement *ret_flag = makeStatement(cc, 8, 1);
    printStatementForFunction(ret_flag, 0); //change the flag
    printf("//----------paramlist start-----------\n");

    //remove dup
    sort(paramlistName.begin(), paramlistName.end());
    paramlistName.erase(unique(paramlistName.begin(), paramlistName.end()), paramlistName.end());

    sort(ptrName.begin(), ptrName.end());
    ptrName.erase(unique(ptrName.begin(), ptrName.end()), ptrName.end());

    for (auto it : paramlistName){
        //std::string storage = randome_pick_one(assignedLocalStorageClassSpecifier);
        std::string type = randome_pick_one(simpleType);
        printf("%s %s;\n", type.c_str(), it.c_str());
    }
    for (auto it : ptrName){
        printf("// ptr name %s;\n", it.c_str());
    }
    //paramlistName.clear();
    printf("//++++++++++paramlist end++++++++++++\n");
    //print function body
    foreach(f, program.functions){
        //printFunction(f->function);
        VariableList *v;

        StatementList *s;
        Block* block = f->function->body;
        foreach(v, block->localvars){
            std::string name = v->variable->name;
            //printf("// name in local : %s\n", name.c_str());
            int findParam = isFind(paramlistName, name);
            int findPtr = isFind(ptrName, name);
            //printf("//find = %d\n", find);
            if (v->variable->type == _integer){
                if (findParam == 0){
                //printf("// name in set : %s\n", name.c_str());
                    //normalVariableName.push_back(name);
                    if (! isFind(normalVariableName, name)){
                        printIntegerDecl(v->variable);
                        normalVariableName.push_back(name);
                    }
                }
            }
            else {
                if ( findPtr == 1 ) {
                    printPointerDecl(v->variable);
                    ptrName.erase(std::remove(ptrName.begin(), ptrName.end(), name), ptrName.end());
                }
            //if (findPtr == 1){
            //    printPointerDeclNoType(v->variable);
            //}
            }
        }

        foreach(s, block->statementlist){
        printStatement(s->statement);
	    }
    }

    //clear the paramlistName set
    paramlistName.clear();

    insertStatements();

    printf("\nreturn 0;\n}\n");
    normalVariableName.clear();
    labelName.clear();
    return os;
}

std::ostream &operator<<(std::ostream &os, const MainFunction &main_function) {
    //std::cout<< "#include<iostream>\n";
    //define array
    //printf("int array[%d] =  {0};\n", array_size);
    //TODO swith of ccg and this tool
    puts("\n\n");
    printf("int main(){\n");
    printf("    func_0();\n");
    printf("}\n");

    return os;
}
// ---------------------------------

std::string LoopFunctionToString(const LoopFunction &loop_function) {
    std::ostringstream os;
    os << loop_function;
    return os.str();
}

std::vector<int> MainFunctionToString(const MainFunction &main_function, unsigned seed_ccg) {
    std::ostringstream os;
    std::vector<int> retVec;
    //clear the former variables
    retVec.clear();
    numPimaryExpr = 0;
    numAssignmentStmt = 0;
    numWhileLoopStmt = 0;
    numDowhileLoopStmt = 0;
    numForLoopStmt = 0;
    numIfElseStmt = 0;
    numIfStmt = 0;
    numSwithStmt = 0;
    numLabeledStmt = 0;
    numCompStmt = 0;
    numExprStmt = 0;
    numJumpStmt = 0;
    std::vector<std::string> funcName;
    cc = (Context*) xmalloc(sizeof(*cc));
    //os << main_function;
    //TODO do not use seed_ccg
    //seed_ccg_init = random_uint();
    //replace init()
    //init();
    program.globalvars = NULL, program.functions = NULL;
    program.numfunctions = program.numglobalvars = 0;

    /* Default settings */
    cmdline.max_functions = 10; //15
    cmdline.max_localvars = 4; //4
    cmdline.max_function_parameters = 4; //4
    cmdline.max_statements_per_block = 7; //7
    cmdline.max_expression_nesting = 8; //8
    cmdline.max_block_nesting = 7; //7
    cmdline.max_pointer_depth = 2; //2

    cmdline.seed = seed_ccg;
    cmdline.nojumps = false;
    printf("/*seed : %lu*/\n", cmdline.seed);

    int argcc = 1;
    char *argvv[] = {"./ccg"};
    processCommandline(argcc, argvv);
    srand(cmdline.seed);
    makeGlobalVariables();


    puts("#include <stdint.h>");
    puts("#include <stdarg.h>");
    //puts("#include <emmintrin.h>");
    puts("#include <stdlib.h>\n\n/* Global variables */");
    //std::cout << "//Test Global variable" << std::endl;
    // what's wrong with this?
    VariableList *v;
    foreach(v, program.globalvars) {
        //os << "//----CovSmith: printVariableDecl----\n";
        //std::cout << printVariableDecl(v->variable) << std::endl;
        printVariableDecl(v->variable) ;
        std::string name = v->variable->name;
        normalVariableName.push_back(name);
        //std::cout << "\n";
        //os << "//++++CovSmith: printVariableDecl++++\n";
    }

    //printProgram();
    printf("int aaa;\n");
    //Context *cc;
    //cc = (Context*) xmalloc(sizeof(*cc));
    cc->scope = program.globalvars;
    cc->nvars = program.numglobalvars;
    cc->nintegers = program.numglobalvars;

    /*
    std::cout << "\n//Test expression" << std::endl;
    Expression *expr = makeExpression(cc, 3);
    printExpression(expr);
    printf("\n");
    Expression *expr1 = makeExpression(cc, 3);
    printExpression(expr1);
    */


    //std::cout << "\n//Test function" << std::endl;
    Function *func = makeFunction(false);
    //os << main_function;
    //printProgram();
    //for add on other functions
    cc->currfunc = func;
    //makeStatement(cc, 5, 1);

    int nesting = 8;
    Context *context = cc;
    size_t i, numlocalvars, numstatements;
    Block *ret = (Block *) xmalloc(sizeof(*ret));
    Context *bcontext = (Context *) xmalloc(sizeof(*bcontext));

    bcontext->currfunc = context->currfunc;
    bcontext->nvars = context->nvars;
    bcontext->nintegers = context->nintegers;
    bcontext->scope = NULL;

    copyVariableList(context->scope, &bcontext->scope);

    ret->localvars = NULL, ret->statementlist = NULL;

    numlocalvars = rand() % cmdline.max_localvars + !nesting;

    for(i = 0; i < numlocalvars; ++i)
    {
        Variable *tmp = makeVariable(bcontext, _randomvartype);
        bcontext->nvars++;

        if(tmp->type == _integer)
            bcontext->nintegers++;

        addVariableToList(tmp, &(ret->localvars));
        addVariableToList(tmp, &bcontext->scope);
    }

    //printf("int foo_test(){\n");
    if(nesting < cmdline.max_block_nesting)
    {
        numstatements = rand() % cmdline.max_statements_per_block;

        for(i = 0; i < numstatements; ++i)
        {
            Statement *tmp = makeStatement(bcontext, nesting, !(numstatements - 1 - i));
            //printStatement(tmp);
	    if (tmp->label){printf("/*label name: %s*/\n", tmp->label->name);}
            addStatementToList(tmp, &ret->statementlist);
        }
    }
    //printf("}\n");

    //TODO
    freeVariableList(bcontext->scope);
    free(bcontext);


    //copyVariableList(func->paramlist, &cc->scope);
    FunctionList *f;
    foreach(f, program.functions){
        //os << "//----CovSmith: printFunctionPrototype----\n";
        //std::cout << printFunctionPrototype(f->function) << ";" << std::endl;
        std::string name = f->function->name;
        //funcName.push_back(name);
        if (! isFind(funcName, name)) {
            printFunctionPrototype(f->function);
            printf(";\n");
            funcName.push_back(name);
       }
        //os << "//++++CovSmith: printFunctionPrototype++++\n";
    }
    funcName.clear();

    os << "\n";
    //os << main_function;
    foreach(f, program.functions){
        //printf("//----CovSmith: printFunction Definition----\n");
        std::string name = f->function->name;
        //printf("//name = %s\n",name.c_str());
    }
            Context *temp_cc = (Context*) xmalloc(sizeof(*temp_cc));
            temp_cc->nvars = cc->nvars;
            temp_cc->nintegers = cc->nintegers;
            temp_cc->scope = cc->scope;
            temp_cc->currfunc = cc->currfunc;
    foreach(f, program.functions){
        printf("//----CovSmith: printFunction----\n");
        std::string name = f->function->name;
        //funcName.push_back(name);
        if (! isFind(funcName, name)) {
            //printFunction(f->function);
            printFunctionPrototype(f->function);
            printf("\n");
            printf("{\n");
            printf("//Now print original block in function body start\n");
            //printBlockForFunction(f->function->body);
            //body of block
            printf("//New insert statement in function body Start\n");
            Context *lcc = (Context*) xmalloc(sizeof(*lcc));
            lcc->nvars = f->function->nvars;
            lcc->nintegers = f->function->nintegers;
            lcc->scope = f->function->scope;
            lcc->currfunc = f->function->currfunc;

            //lcc->currfunc->numlabels = 0;
            //insertStatementForFunction(lcc);
            MainFunction ma;
            isFunction = 1;
            //Context *temp_cc = (Context*) xmalloc(sizeof(*temp_cc));
            //temp_cc->nvars = cc->nvars;
            //temp_cc->nintegers = cc->nintegers;
            //temp_cc->scope = cc->scope;
            //temp_cc->currfunc = cc->currfunc;
            // FOR save label name
            //insertStatement(cc);

            cc->nvars = lcc->nvars;
            cc->nintegers = lcc->nintegers;
            cc->scope = lcc->scope;
            cc->currfunc = lcc->currfunc;


            //TODO
            //Statement *ret_flag = makeStatement(lcc, 8, 1);
            //printStatementForFunction(ret_flag, 1); //change the flag
            /*
            Statement *ret_flag1 = makeStatementForFunction(cc, 5, 1);
            printStatementForFunction(ret_flag1, 1); //change the flag
            Statement *ret_flag2 = makeStatementForFunction(cc, 5, 1);
            printStatementForFunction(ret_flag2, 1); //change the flag
            */
            //TODO doing save variable names, save global variables only

            printf("//Now print original block in function body start\n");
            //printBlockForFunction(f->function->body);
            //body of block
            VariableList *v;
            StatementList *s;
            foreach(v, f->function->body->localvars) {
                printVariableDecl(v->variable);
            }

            foreach(s, f->function->body->statementlist) {
                printStatement(s->statement);
                //int var = random_generate(2,12);
                //printf("//var = %d\n", var);
                //for (int i = 1; i < var; i++){
                //if (random_generate(0,1)){
            //}
                    printf("//inserted statements start\n");
                    //os << ma.loop_function().statement_seq().statements();
                if (random_generate(0,1)){
                    int var = random_generate(1,11);
                    printf("//random insert = 1\n");
                    switch (var){
                        printf("//inserted statements start\n");
                        case 1: os << ma.loop_function().statement_seq().statements().assignment(); break;
                        case 2: os << ma.loop_function().statement_seq().statements().while_loop_statement(); depth = 0; break;
                        case 3: os << ma.loop_function().statement_seq().statements().do_while_loop_statement(); depth = 0; break;
                        case 4: os << ma.loop_function().statement_seq().statements().for_loop_statement(); depth = 0; break;
                        case 5: os << ma.loop_function().statement_seq().statements().if_else_statement(); depth = 0; break;
                        case 6: os << ma.loop_function().statement_seq().statements().if_statement(); depth = 0; break;
                        case 7: os << ma.loop_function().statement_seq().statements().switch_statement(); depth = 0; break;
                        case 8: os << ma.loop_function().statement_seq().statements().labeled_statement(); break;
                        case 9: os << ma.loop_function().statement_seq().statements().expression_statement(); break;
                        case 10: os << ma.loop_function().statement_seq().statements().compound_statement(); break;
                        case 11: os << ma.loop_function().statement_seq().statements().jump_statement(); depth = 0; break;
                        default: os << ma.loop_function().statement_seq().statements().while_loop_statement(); depth = 0; break;

                    }
                    printf("//inserted statements done\n");

                }
            }
            printf("//Now print original block in function body end\n");

            printf("//New insert statement in function body End\n");
            //printf("//Now print original block in function body\n");
            //printBlock(f->function->body);

            printf("}\n");
            funcName.push_back(name);

            isFunction = 0;

       }
    }
            cc->nvars = temp_cc->nvars;
            cc->nintegers = temp_cc->nintegers;
            cc->scope = temp_cc->scope;
            cc->currfunc = temp_cc->currfunc;
            free(temp_cc);

    //funcName.clear();
    os << main_function;
    /*
    std::cout << "\n//Test Block" << std::endl;
    Block *blk = makeBlock(cc, 3);
    os << printBlock(blk);
    */
    /*
    std::cout << "\n\n//Test Statement" << std::endl;
    Statement *statement = makeStatement(cc, 5, 0);
    os << "//----CovSmith: printStatement----\n";
    //std::cout << printStatement(statement) << std::endl;
    os << printStatement(statement) << "\n" ;
    printf("\n");

    Statement *statement1 = makeStatement(cc, 5, 0);
    //std::cout << printStatement(statement1) << std::endl;
    os << printStatement(statement1) << "\n";

    os << "//++++CovSmith: printStatement++++\n";
    //makeFunction(false);
    //printProgram();
    */
    /*
    std::cout << "\n\n Test Block" << std::endl;
    Block *blk = makeBlock(cc, 5);
    printBlock(blk);
    */
    //std::cout << "//----CovSmith----" << std::endl;
    //std::cout << os.str() << std::endl;
    printf("//++++CovSmith++++\n");
    for (auto i : labelName) printf("/*Lable before free : %s*/", i.c_str());
    //labelName.clear();
    //TODO when to free?
    free(cc);
    printf("//Statictics in proto_to_c.cc\n");
    //printf("//numPimaryExpr = %d\n", numPimaryExpr);
    printf("//numAssignmentStmt = %d\n", numAssignmentStmt);
    printf("//numWhileLoopStmt = %d\n", numWhileLoopStmt);
    printf("//numDowhileLoopStmt = %d\n", numDowhileLoopStmt);
    printf("//numForLoopStmt = %d\n", numForLoopStmt);
    printf("//numIfElseStmt = %d\n", numIfElseStmt);
    printf("//numIfStmt = %d\n", numIfStmt);
    printf("//numSwithStmt = %d\n", numSwithStmt);
    printf("//numLabeledStmt = %d\n", numLabeledStmt);
    printf("//numCompStmt = %d\n", numCompStmt);
    printf("//numExprStmt = %d\n", numExprStmt);
    printf("//numJumpStmt = %d\n", numJumpStmt);
    //Add the above numbers to the retVec
    //retVec.push_back(numPimaryExpr);
    //retVec.push_back(numAssignmentStmt);
    retVec.push_back(numWhileLoopStmt);
    retVec.push_back(numDowhileLoopStmt);
    retVec.push_back(numForLoopStmt);
    retVec.push_back(numIfElseStmt);
    retVec.push_back(numIfStmt);
    retVec.push_back(numSwithStmt);
    retVec.push_back(numLabeledStmt);
    retVec.push_back(numCompStmt);
    retVec.push_back(numExprStmt);
    retVec.push_back(numJumpStmt);

    //clear the previous label name, to make sure the following label can be printed
    rootMakeLabel = 0;
    root = 0;

    //clear the variable name list
    printf("//Number of Global Variables : %d \n", normalVariableName.size());
    normalVariableName.clear();
    return retVec;
}

} // namespace clang_fuzzer_loop

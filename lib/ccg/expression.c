/* C Code Generator
 *
 * Copyright (C) 2016, Antoine Balestrat <antoine.balestrat@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "ccg.h"

//ignore TernaryExpression for now
//static const ExpressionType exprarray[9] = {_functioncallexpr, _functioncallexpr, _operationexpr, _operationexpr, _operationexpr, _testexpr, _testexpr, _assignmentexpr, _assignmentexpr};
static const ExpressionType exprarray[10] = {_ternaryexpr, _functioncallexpr, _functioncallexpr, _operationexpr, _operationexpr, _operationexpr, _testexpr, _testexpr, _assignmentexpr, _assignmentexpr};
//static const ExpressionType exprarray[10] = {_testexpr};

char const * const testop2str[_testopmax] = {"==", "<=", ">=", "<", ">", "!="};
char const * const arithop2str[_arithopmax] = {"+", "-", "/", "%", "*"};
char const * const bitwiseop2str[_bitwiseopmax] = {"&", "|", "^", ">>", "<<"};
char const * const logicalop2str[_logicalopmax] = {"&&", "||", "~", "~~"};
char const * const assignop2str[_assignopmax] = {"+=", "-=", "/=", "%=", "*=", "&=", "|=", "^=", "=", "++", "--"};

void buildOperand(Expression*, Context*, unsigned);
void buildTernary(Expression *expr, Context*, unsigned);
void buildOperation(Expression*, Context*, unsigned);
void buildTest(Expression*, Context*, unsigned);
void buildAssignment(Expression*, Context*, unsigned);
void buildFunctionCall(Expression*, Context*, unsigned);

static void (*buildfunctions[_expressiontypemax])(Expression*, Context*, unsigned) =
{
    [_operandexpr] = buildOperand,
    [_ternaryexpr] = buildTernary,
    [_operationexpr] = buildOperation,
    [_testexpr] = buildTest,
    [_assignmentexpr] = buildAssignment,
    [_functioncallexpr] = buildFunctionCall
};

void addExpressionToList(Expression *expression, ExpressionList **list)
{
    if(!*list)
    {
        *list = xmalloc(sizeof(**list));
        (*list)->expression = expression;
        (*list)->next = NULL;
    }

    else
    {
        ExpressionList *e;

        for(e = *list; e->next; e = e->next);
        e->next = xmalloc(sizeof(*e->next));
        e->next->expression = expression;
        e->next->next = NULL;
    }
}

#define EXPRESSION_IS_INVALID(t) (((t) == _functioncallexpr && program.numfunctions >= cmdline.max_functions))

Expression *makeExpression(Context *context, unsigned nesting)
{
    Expression *expression = xmalloc(sizeof(*expression));

    if(nesting >= cmdline.max_expression_nesting)
        expression->type = _operandexpr;
        //Haoxin
        //expression->type = exprarray[rand() % 10];
    else
    {
        do{
            //printf("//expression->type = %lu\n", expression->type);
            //expression->type = exprarray[rand() % (sizeof(exprarray) / sizeof(*exprarray))];
            expression->type = exprarray[rand() % 10];
        }
        while(EXPRESSION_IS_INVALID(expression->type));
    }

    (buildfunctions[expression->type])(expression, context, nesting + 1);
    //buildTest(expression, context, nesting + 1);
    return expression;
}

void buildOperand(Expression *expression, Context *context, unsigned nesting)
{
    expression->expr.operand = selectOperand(context);
}

void buildTest(Expression *expression, Context *context, unsigned nesting)
{
    struct TestExpression *te = xmalloc(sizeof(*te));

    te->op = rand() % _testopmax;
    te->lefthand = makeExpression(context, nesting + 1), te->righthand = makeExpression(context, nesting + 1);

    expression->expr.testexpr = te;
}

void buildTernary(Expression *expression, Context *context, unsigned nesting)
{
    struct TernaryExpression *te = xmalloc(sizeof(*te));

    te->test = makeExpression(context, nesting + 1);
    te->truepath = (rand() % 4) ? makeExpression(context, nesting + 1) : NULL, te->falsepath = makeExpression(context, nesting + 1);
    expression->expr.ternexpr = te;
}

void buildOperation(Expression *expression, Context *context, unsigned nesting)
{
    struct OperationExpression *oe = xmalloc(sizeof(*oe));

    oe->lefthand = makeExpression(context, nesting + 1), oe->righthand = makeExpression(context, nesting + 1);
    oe->type = rand() % _operationtypemax;

    if(oe->type == _arithmetic)
        oe->operator_c.arithop = rand() % _arithopmax;
    else if(oe->type == _bitwise)
        oe->operator_c.bitwiseop = rand() % _bitwiseopmax;
    else
        oe->operator_c.logicalop = rand() % _logicalopmax;

    expression->expr.opexpr = oe;
}

#define ASSIGNMENT_OP_IS_INVALID(oprtr, left, right) ((((left->type == _double || left->type == _float) || (IS_FLOATING_POINT_VARIABLE(right))) && (oprtr == _assignmod)))

void buildAssignment(Expression *expression, Context *context, unsigned nesting)
{
    struct AssignmentExpression *ae = xmalloc(sizeof(*ae));

    ae->lvalue = selectVariable(context, _randomvartype);
    ae->rvalue = makeExpression(context, nesting + 1);
    ae->op = rand() % _assignopmax;

    expression->expr.assignexpr= ae;
}

void buildFunctionCall(Expression *expression, Context *context, unsigned nesting)
{
    struct FunctionCallExpression *fce = xmalloc(sizeof(*fce));
    VariableList *v;

    fce->paramlist = NULL;
    fce->function = makeFunction(true);
    //add to functionlist
    addFunctionToList(fce->function, &program.functions);
    //printf("//------------Function call in expression.c------------------\n");
    //printFunction(fce->function);
    //printf("//++++++++++++Function call in expression.c++++++++++++++++++\n");


    foreach(v, fce->function->paramlist)
        addExpressionToList(makeExpression(context, nesting + 1), (ExpressionList**) &fce->paramlist);

    expression->expr.funccallexpr = fce;
}

void printExpression(Expression *expression);

static void printOperand(Operand *op)
{
    //char ret[1] = {};
    /*
    char *ret1;
    if (op->type == _variable)
        ret1 = USABLE_ID(op->op.variable);
    else
        ret1 = printConstant(op->op.constant);
    */
    //char *ret = (char*) calloc(1, sizeof(char));
    //char *ret;
    //ret[0] = '\0';
    if(op->type == _variable) {
        fputs(USABLE_ID(op->op.variable), stdout);
        //char *ret1 = USABLE_ID(op->op.variable);
        //ret = (char*) realloc(ret, strlen(ret1) + strlen(ret) + 1);
        //mystrcat(ret, ret1);
    }
    else {
        printConstant(op->op.constant);
        //char *ret1 = printConstant(op->op.constant);
        //ret = (char*) realloc(ret, strlen(ret1) + strlen(ret) + 1);
        //mystrcat(ret, ret1);
    }
    //printf("%s\n", ret);
    //return ret;
}

static void printTest(struct TestExpression *te)
{
    //char ret[1] = {};
    /*
    char *ret1 = printExpression(te->lefthand);
    char *ret2 = printExpression(te->righthand);
    char *ret = (char*) malloc(1 + strlen(ret1) + 1 +strlen(testop2str[te->op])+ 1+  strlen(ret2) + 1 + 1);
    ret[0] = '\0';
    */
    putchar('(');
    //mystrcat(ret, "(");
    printExpression(te->lefthand);
    //mystrcat(ret, ret1);
    //mystrcat(ret, " ");
    printf(" %s ", testop2str[te->op]);
    //mystrcat(ret, testop2str[te->op]);
    //mystrcat(ret, " ");
    printExpression(te->righthand);
    //mystrcat(ret, ret2);
    putchar(')');
    //mystrcat(ret, ")");
    //printf("%s\n", ret);
    //return ret;
}


static void printTernary(struct TernaryExpression *te)
{
    //char ret[1] = {};
    /*
    char *ret1 = printExpression(te->test);
    char *ret2 = printExpression(te->falsepath);
    char *ret = (char*) malloc(1 + strlen(ret1) + 3 + strlen(ret2) + 3 + 1 + 1);
    ret[0] = '\0';
    */
    putchar('(');
    //mystrcat(ret, "(");
    printExpression(te->test);
    //mystrcat(ret, ret1);
    fputs(" ? ", stdout);
    //mystrcat(ret, " ? ");

    if(te->truepath){
        //char *tt = printExpression(te->truepath);
        //ret = (char*) realloc(ret, strlen(ret) + strlen(ret1) + 1);
        //mystrcat(ret, tt);
        printExpression(te->truepath);
    }

    fputs(" : ", stdout);
    //mystrcat(ret, " : ");
    printExpression(te->falsepath);
    //mystrcat(ret, ret2);
    putchar(')');
    //mystrcat(ret, ")");
    //printf("%s\n", ret);
    //return ret;
}

static void printOperation(struct OperationExpression *oe)
{
    //char ret[1] = {};
    /*
    char *ret1 = printExpression(oe->lefthand);
    char *ret2 = printExpression(oe->righthand);
    char *tt = oe->type == _arithmetic ? arithop2str[oe->operator_c.arithop] : (oe->type == _bitwise ? bitwiseop2str[oe->operator_c.bitwiseop] : logicalop2str[oe->operator_c.logicalop]);
    char *ret = (char*) malloc(strlen(ret1) + strlen(ret2) + 1 +1 + strlen(tt) + 1 + 1 + 1);
    ret[0] = '\0';
    */
    putchar('(');
    printExpression(oe->lefthand);
    printf(" %s ", oe->type == _arithmetic ? arithop2str[oe->operator_c.arithop] : (oe->type == _bitwise ? bitwiseop2str[oe->operator_c.bitwiseop] : logicalop2str[oe->operator_c.logicalop]));
    printExpression(oe->righthand);
    putchar(')');

    /*
    mystrcat(ret, "(");
    mystrcat(ret, ret1);
    mystrcat(ret, " ");
    //mystrcat(ret, oe->type == _arithmetic ? arithop2str[oe->operator_c.arithop] : (oe->type == _bitwise ? bitwiseop2str[oe->operator_c.bitwiseop] : logicalop2str[oe->operator_c.logicalop]));
    mystrcat(ret, tt);
    mystrcat(ret, " ");
    mystrcat(ret, ret2);
    mystrcat(ret, ")");
    */

    //printf("%s\n", ret);
    //return ret;
}

static void printAssignment(struct AssignmentExpression *ae)
{
    //char ret[1] = {};
    //char *ret1 = printExpression(ae->rvalue);
    //char *ret = (char*) malloc(1 + strlen(USABLE_ID(ae->lvalue)) + 1 + strlen(assignop2str[ae->op]) + 1 + strlen(ret1) + 1 + 1);
    //ret[0] = '\0';

    putchar('(');
    printf("%s %s ", USABLE_ID(ae->lvalue), assignop2str[ae->op]);
    printExpression(ae->rvalue);
    putchar(')');

    /*
    mystrcat(ret, "(");
    mystrcat(ret, USABLE_ID(ae->lvalue));
    mystrcat(ret, " ");
    mystrcat(ret, assignop2str[ae->op]);
    mystrcat(ret, " ");
    mystrcat(ret, ret1);
    mystrcat(ret, ")");
    */

    //printf("%s\n", ret);
    //return ret;
}

static void printFunctionCall(struct FunctionCallExpression *fce)
{
    //char ret[1] = {};
    //char *ret = (char*) malloc(strlen(fce->function->name) + 1 + 1 + 1);
    //ret[0] = '\0';
    ExpressionList *e;

    printf("%s(", fce->function->name);
    //mystrcat(ret, fce->function->name);
    //mystrcat(ret, "(");

    foreach(e, fce->paramlist)
    {
        //char *ret1 = printExpression(e->expression);
        //ret = (char*) realloc(ret, strlen(ret) + strlen(ret1) + 1);
        //mystrcat(ret, ret1);
        printExpression(e->expression);

        if(e->next){
            fputs(", ", stdout);
            //char *tt = ", ";
            //ret = (char*) realloc(ret, strlen(ret) + strlen(tt) + 1);
            //mystrcat(ret, tt);
        }
    }

    putchar(')');
    //mystrcat(ret, ")");
    //printf("%s\n", ret);
    //return ret;
}


void printExpression(Expression *expression)
{
   // char ret[1] = {};

    //char *ret = (char*) malloc(1);
    //ret[0] = '\0';
    switch(expression->type)
    {
        /*
        case _operandexpr: { char *t1 = printOperand(expression->expr.operand); ret = (char*) realloc(ret, strlen(ret) + strlen(t1) + 1); mystrcat(ret, t1); break;}
        case _ternaryexpr: { char *t1 = printTernary(expression->expr.ternexpr); ret = (char*) realloc(ret, strlen(ret) + strlen(t1) + 1); mystrcat(ret, t1); break;}
        case _operationexpr: { char *t1 = printOperation(expression->expr.opexpr); ret = (char*) realloc(ret, strlen(ret) + strlen(t1) + 1); mystrcat(ret, t1); break;}
        case _testexpr: { char *t1 = printTest(expression->expr.testexpr); ret = (char*) realloc(ret, strlen(ret) + strlen(t1) + 1); mystrcat(ret, t1); break;}
        case _assignmentexpr: { char *t1 = printAssignment(expression->expr.assignexpr); ret = (char*) realloc(ret, strlen(ret) + strlen(t1) + 1); mystrcat(ret, t1); break;}
        case _functioncallexpr: { char *t1 = printFunctionCall(expression->expr.funccallexpr); ret = (char*) realloc(ret, strlen(ret) + strlen(t1) + 1); mystrcat(ret, t1); break;}
        default: die("are you... mad ?");
        */

        case _operandexpr: printOperand(expression->expr.operand); break;
        case _ternaryexpr: printTernary(expression->expr.ternexpr); break;
        case _operationexpr: printOperation(expression->expr.opexpr); break;
        case _testexpr: printTest(expression->expr.testexpr); break;
        case _assignmentexpr: printAssignment(expression->expr.assignexpr); break;
        case _functioncallexpr: printFunctionCall(expression->expr.funccallexpr); break;
        default: printf("0 /*default in printExpression*/");
        //default: die("are you... mad ?");

    }
    //printf("%s\n", ret);
    //ireturn "";
}


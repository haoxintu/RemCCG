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

//Context context;

void addFunctionToList(Function *function, FunctionList **list)
{
    if(!*list)
    {
        *list = xmalloc(sizeof(**list));
        (*list)->function = function;
        (*list)->next = NULL;
    }

    else
    {
        FunctionList *v;

        for(v = *list; v->next; v = v->next);
        v->next = xmalloc(sizeof(*v->next));
        v->next->function = function;
        v->next->next = NULL;
    }
}

char *makeFunctionName(void)
{
    char *name;
    char buff[8];

    sprintf(buff, "%zu", program.numfunctions);
    name = malloc(strlen(buff) + 6);
    sprintf(name, "func_%zu", program.numfunctions);

    return name;
}

void printFunctionPrototype(Function *function);
/* At this point, only global variables are available */
Function *makeFunction(bool params)
{
    Function *ret = xmalloc(sizeof(*ret));
    //if (params == 0){
    printf("//makeFunction\n");
    short i, numparams = 0;
    //Function *ret = xmalloc(sizeof(*ret));

    Context *context = xmalloc(sizeof(*context));
    context->scope = params ? NULL : program.globalvars;
    context->currfunc = ret;

    ret->paramlist = NULL;
    ret->returntype = rand() % _inttypemax;
    ret->name = makeFunctionName();
    ret->numlabels = 0;
    ret->labels = NULL;

    context->nvars = context->nintegers = program.numglobalvars;
    numparams = (params ? rand() % (cmdline.max_function_parameters + 1) : 0);

    program.numfunctions++;

    if(params)
    {
        copyVariableList(program.globalvars, &context->scope);

        for(i = 0; i < numparams; ++i)
        {
            Variable *v = makeVariable(context, _integer);
            context->nvars++;

            if(v->type == _integer)
                context->nintegers++;

            addVariableToList(v, &ret->paramlist);
            addVariableToList(v, &context->scope);
        }
    }

    ret->body = makeBlockForFunction(context, 0);

    addFunctionToList(ret, &program.functions);

    ret->nvars = ret->body->nvars;
    ret->nintegers = ret->body->nintegers;
    ret->scope = ret->body->scope;
    ret->currfunc = ret->body->currfunc;
    //printf("0;");
    //
    printf("extern ");
    printFunctionPrototype(ret);
    printf(";\n");
    //
    //if(params)
    //    freeVariableList(context->scope);

    //free(context);
    //}
/*
    if (params == 1) {
    printf("//makeFunction(true)\n");
    short i, numparams = 0;
    //Function *ret = xmalloc(sizeof(*ret));

    Context *context = xmalloc(sizeof(*context));
    context->scope = params ? NULL : program.globalvars;
    context->currfunc = ret;

    ret->paramlist = NULL;
    ret->returntype = rand() % _inttypemax;
    ret->name = makeFunctionName();
    ret->numlabels = 0;
    ret->labels = NULL;

    context->nvars = context->nintegers = program.numglobalvars;
    numparams = (params ? rand() % (cmdline.max_function_parameters + 1) : 0);

    program.numfunctions++;

    if(params)
    {
        copyVariableList(program.globalvars, &context->scope);

        for(i = 0; i < numparams; ++i)
        {
            Variable *v = makeVariable(context, _integer);
            context->nvars++;

            if(v->type == _integer)
                context->nintegers++;

            addVariableToList(v, &ret->paramlist);
            addVariableToList(v, &context->scope);
        }
    }

    ret->body = makeBlockForFunction(context, 0);

    addFunctionToList(ret, &program.functions);

    if(params)
        freeVariableList(context->scope);
    free(context);
    }
*/
    return ret;
}

void printFunctionPrototype(Function *function)
{
    //char ret[1] = {};
    //char *ret = (char*) malloc(strlen(inttype2str[function->returntype]) + 1 + strlen(function->name) + 1 + 1 + 1);
    //ret[0] = '\0';
    VariableList *v;

    printf("%s %s(", inttype2str[function->returntype], function->name);


    /*
    mystrcat(ret, inttype2str[function->returntype]);
    mystrcat(ret, " ");
    mystrcat(ret, function->name);
    mystrcat(ret, "(");
    */



    foreach(v, function->paramlist)
    {
        printf(v == function->paramlist ? "" : ", ");
        printVariableType(v->variable);
        printf(" %s", v->variable->name);

        /*
        if (v == function->paramlist){
            char *tt = "";
            ret = (char*) realloc(ret, strlen(ret) + strlen(tt) + 1);
            mystrcat(ret, tt);
        }
        else{
            char *tt = ", ";
            ret = (char*)realloc(ret, strlen(ret) + strlen(tt) + 1);
            mystrcat(ret, tt);
        }

        char *ret1 = printVariableType(v->variable);
        char *ret2 = " ";
        char *ret3 = v->variable->name;

        ret = (char*) realloc(ret, strlen(ret) + strlen(ret1) + 1);
        mystrcat(ret, ret1);
        ret = (char*) realloc(ret, strlen(ret) + strlen(ret2) + 1);
        mystrcat(ret, ret2);
        ret = (char*) realloc(ret, strlen(ret) + strlen(ret3) + 1);
        mystrcat(ret, ret3);
        */


    }

    printf(")");
    //mystrcat(ret, ")");
    //printf("%s\n", ret);
    //return ret;
}

void printFunction(Function *function)
{
    //char ret[1] = {};
    //char *ret1 = printFunctionPrototype(function);
    //char *ret2 = printBlock(function->body);

    //char *ret = (char*) calloc(strlen(ret1) + strlen(ret2) +1, sizeof(char));
    //ret[0] = '\0';
    printFunctionPrototype(function);
    //char *ret1 = printFunctionPrototype(function);
    //ret = (char*) realloc(ret, strlen(ret) + strlen(ret1) + 1);
    //mystrcat(ret, ret1);
    putchar('\n');
    //mystrcat(ret, "\n");
    printBlock(function->body);
    //char *ret2 = printBlock(function->body);
    //printf("sizeof(ret2) = %d\n", sizeof(ret2));
    //ret = (char*) realloc(ret, strlen(ret) + strlen(ret2) + 1);
    //mystrcat(ret, ret2);
    //printf("%s\n", ret);
    //return ret;
}

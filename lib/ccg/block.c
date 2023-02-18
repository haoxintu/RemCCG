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

/*
void insertIfStatement(Context *cc){
    printf("int c = ");
    Expression* expr = makeExpression(cc,3);
    printExpression(expr);
    printf(";\n");
}
*/
//New added for insert new statements inside function
Block *makeBlockForFunction(Context *context, unsigned nesting)
{
    size_t i, numlocalvars, numstatements;
    Block *ret = xmalloc(sizeof(*ret));
    Context *bcontext = xmalloc(sizeof(*bcontext));

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

    //if(nesting < cmdline.max_block_nesting)
    //{
      //  numstatements = rand() % cmdline.max_statements_per_block;

        //for(i = 0; i < numstatements; ++i)
        //{
            Statement *tmp = makeStatement(bcontext, nesting, 1);
            addStatementToList(tmp, &ret->statementlist);
        //}

    //}
    //printf("//New makeStatement\n");
    //insertIfStatement(bcontext);
    //Statement *tmp1 = makeStatement(bcontext, nesting, 0);
    //addStatementToList(tmp1, &ret->statementlist);
    ret->nvars = bcontext->nvars;
    ret->nintegers = bcontext->nintegers;
    ret->scope = bcontext->scope;
    ret->currfunc = bcontext->currfunc;
    //freeVariableList(bcontext->scope);
    //free(bcontext);

    return ret;
}


/* `nesting' is the nesting level of the block. func(){0 {1} {1 {2}}} */
Block *makeBlock(Context *context, unsigned nesting)
{
    size_t i, numlocalvars, numstatements;
    Block *ret = xmalloc(sizeof(*ret));
    Context *bcontext = xmalloc(sizeof(*bcontext));

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

    if(nesting < cmdline.max_block_nesting)
    {
        numstatements = rand() % cmdline.max_statements_per_block;

        for(i = 0; i < numstatements; ++i)
        {
            Statement *tmp = makeStatement(bcontext, nesting, !(numstatements - 1 - i));
            addStatementToList(tmp, &ret->statementlist);
        }
    }

    //freeVariableList(bcontext->scope);
    //free(bcontext);

    return ret;
}


void printBlock(Block *block)
{
    //char ret[1] = {};
    /*
    printf("///sizeof(block->localvars + block->statementlist) = %d\n", sizeof(block->localvars) + sizeof(block->statementlist));
    char *ret = (char*)malloc(1);
    ret[0] = '\0';
    */
    VariableList *v;
    StatementList *s;

    puts("{");
    //mystrcat(ret, "{");

    //char *ret1;
    foreach(v, block->localvars) {
        printVariableDecl(v->variable);
        //char *ret1 = printVariableDecl(v->variable);
        //printf("ret2 : %s", ret2);
        //ret = (char*) realloc(ret, strlen(ret) + strlen(ret1) + 1);
        //mystrcat(ret, ret1);
    }

    foreach(s, block->statementlist) {
        printStatement(s->statement);
        //char *ret2 = printStatement(s->statement);
        //printf("ret2 : %s", ret2);
        //ret = (char*)realloc(ret, strlen(ret) + strlen(ret2) + 1);
        //mystrcat(ret, ret2);
        //printf("ret2 done\n");
    }

    puts("}");
    //mystrcat(ret, "}");
    //printf("%s\n", ret);
    //return ret;
}

void printBlockForFunction(Block *block)
{
    //char ret[1] = {};
    /*
    printf("///sizeof(block->localvars + block->statementlist) = %d\n", sizeof(block->localvars) + sizeof(block->statementlist));
    char *ret = (char*)malloc(1);
    ret[0] = '\0';
    */
    VariableList *v;
    StatementList *s;

    puts("{"); //omit
    //mystrcat(ret, "{");

    //char *ret1;
    foreach(v, block->localvars) {
        printVariableDecl(v->variable);
        //char *ret1 = printVariableDecl(v->variable);
        //printf("ret2 : %s", ret2);
        //ret = (char*) realloc(ret, strlen(ret) + strlen(ret1) + 1);
        //mystrcat(ret, ret1);
    }

    foreach(s, block->statementlist) {
        printStatement(s->statement);
        //char *ret2 = printStatement(s->statement);
        //printf("ret2 : %s", ret2);
        //ret = (char*)realloc(ret, strlen(ret) + strlen(ret2) + 1);
        //mystrcat(ret, ret2);
        //printf("ret2 done\n");
        //TODO only print limited statement here
        //break;
    }

    puts("}");
    //mystrcat(ret, "}");
    //printf("%s\n", ret);
    //return ret;
}

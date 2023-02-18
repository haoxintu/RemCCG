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

void addVariableToList(Variable *variable, VariableList **list)
{
    if(!*list)
    {
        *list = xmalloc(sizeof(**list));
        (*list)->variable = variable;
        (*list)->next = NULL;
    }

    else
    {
        VariableList *v;

        for(v = *list; v->next; v = v->next);
        v->next = xmalloc(sizeof(*v->next));
        // Haoxin
        //if (v->next) {
            v->next->variable = variable;
            v->next->next = NULL;
        //}
    }
}

void freeVariableList(VariableList *list)
{
    VariableList *v, *next;

    for(v = list; v; v = next)
    {
        next = v->next;
        free(v);
    }
}

Variable *makeVariable(Context *context, VariableType type)
{
    Variable *ret = xmalloc(sizeof(Variable));

    if(type != _randomvartype)
    {
        ret->type = type;

        if(type == _integer)
            makeInteger(ret, context);
        else
            makePointer(ret, context);
    }

    else
    {
        if((ret->type = rand() % _vartypemax) == _integer)
            makeInteger(ret, context);
        else if(ret->type == _pointer)
            makePointer(ret, context);
    }

    return ret;
}

void printVariableDecl(Variable *var)
{
    //char ret[1] = {};

    /*
    char *ret1;
    if (var->type == _integer)
        ret1 = printIntegerDecl(var);
    else
        ret1 = printPointerDecl(var);

    char *ret = (char*) malloc(1 + strlen(ret1));
    ret[0] = '\0';
    */
    if(var->type == _integer){
        //char *ret1 = printIntegerDecl(var);
        //ret = (char*)realloc(ret, strlen(ret) + strlen(ret1) + 1);
        //mystrcat(ret, ret1);
        printIntegerDecl(var);
    }else{
        //printf("ret : %s\n", tt);
        //char *ret1 = printPointerDecl(var);
        //ret = (char*) realloc(ret, strlen(ret) + strlen(ret1) + 1);
        //mystrcat(ret, ret1);
        printPointerDecl(var);
    }
    //printf("address: %p\n", ret);
    //printf("stuck!\n");
    //return ret;
}

void printVariableType(Variable *var)
{
    //char ret[1] = {};
    //char *ret = (char*) malloc(strlen(inttype2str[ultimateType(var)]) + 1);
    //ret[0] = '\0';
    printf("%s", inttype2str[ultimateType(var)]);
    //mystrcat(ret, inttype2str[ultimateType(var)]);

    if(var->type == _pointer)
    {
        size_t n = pointerDepth(var);

        for(size_t i = 0; i < n; ++i){
            //char *tt = "*";
            //ret = (char*) realloc(ret, strlen(ret) + strlen(tt) + 1);
            //mystrcat(ret, tt);
            putchar('*');
        }
    }
    //printf("//Test printIntegerDecl\n");
    //printf("%s\n", ret);
    //char r[strlen(ret) + 1];
    //strcpy(r, ret);
    //free(ret);
    //return ret;
}

void copyVariableList(VariableList *src, VariableList **dest)
{
    VariableList *v;

    foreach(v, src)
        addVariableToList(v->variable, dest);
}

Variable *selectVariable(Context *context, VariableType type)
{
    VariableList *v;
    size_t n, t = 0;

    if(type == _randomvartype)
        n = rand() % context->nvars;
    else
        n = rand() % (type == _integer ? context->nintegers : (context->nvars - context->nintegers));

    foreach(v, context->scope)
    {
        if(v->variable->type == type || type == _randomvartype)
            if(t++ == n)
                return v->variable;
    }

    //printf("Stuck in selectVariable function!\n");
    die("unreachable");
    return NULL;
}

void makeGlobalVariables(void)
{
    size_t i;
    Context *c = (Context*) xmalloc(sizeof(*c));

    //c->scope = 3;
    c->scope = program.globalvars;
    c->nvars = 0;

    program.numglobalvars = rand() % 10 + 10; //changed from Haoxin

    for(i = 0; i < program.numglobalvars; ++i)
    {
        addVariableToList(makeVariable(c, _integer), &program.globalvars);
        //addVariableToList(makeVariable(c, _pointer), &program.globalvars);
        c->nvars++;
    }

    free(c);
}

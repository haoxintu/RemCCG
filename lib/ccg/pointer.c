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

Variable *pickPointableVariable(Context *context);
//inline size_t pointerDepth(Variable *var);
#define VARIABLE_IS_NOT_POINTABLE(var) (pointerDepth(var) >= cmdline.max_pointer_depth)

static char *makePointerName(Context *context)
{
    char *ret, buff[16];

    sprintf(buff, "ptr_%u", context->nvars);
    ret = xmalloc(strlen(buff) + 1);
    strcpy(ret, buff);

    return ret;
}

inline size_t pointerDepth(Variable *var)
{
    size_t n = 0;

    while(var->type == _pointer)
    {
        ++n;
        var = var->pointer.pointed;
    }

    return n;
}
//void printPointerDecl(Variable *var);
//Variable *pickPointableVariable(Context *context);
size_t pointerDepth(Variable *var);
void makePointer(Variable *var, Context *context)
{
    var->pointer.pointed = pickPointableVariable(context);
    var->name = makePointerName(context);
}

Variable *pickPointableVariable(Context *context)
{
    VariableList *v;
    Variable *ret = NULL;

    do
    {
        size_t t = rand() % context->nvars, i = 0;

        foreach(v, context->scope)
        {
            if(i++ == t)
                ret = v->variable;
        }

    } while(VARIABLE_IS_NOT_POINTABLE(ret));

    return ret;
}


/* This function goes through all the pointers to find the type of the ultimately pointed integer ! */
IntegerType ultimateType(Variable *var)
{
    while(var->type == _pointer)
        var = var->pointer.pointed;

    return var->intvar.type;
}

char *maxDerefdPointer(Variable *var)
{
    static char buffer[32];
    size_t depth = pointerDepth(var);

    memset(buffer, '*', depth);
    buffer[depth] = 0;

    strcat(buffer, var->name);

    return buffer;
}

void printPointerDecl(Variable *var)
{
    //printf("//----Start printPointerDecl\n");
    //char ret[1] = {};
    //char *ret = (char*) malloc(strlen(inttype2str[ultimateType(var)]) + 1 + pointerDepth(var) + strlen(var->name) + 4 + strlen(var->pointer.pointed->name) + 2 + 1 + 100);
    //ret[0] = '\0';
    size_t i, depth = pointerDepth(var);

    //char *ret = (char*) calloc(strlen(inttype2str[ultimateType(var)]) + 1 + depth + strlen(var->name) + 4 + strlen(var->pointer.pointed->name) + 2 + 1 , sizeof(char));
    //ret[0] = '\0';
    printf("%s ", inttype2str[ultimateType(var)]);
    //mystrcat(ret, inttype2str[ultimateType(var)]);
    //mystrcat(ret, " ");
    for(i = 0; i < depth; ++i){
        putchar('*');
        //mystrcat(ret, "*");
    }

    printf("%s = &%s;\n", var->name, var->pointer.pointed->name);


    /*
    mystrcat(ret, var->name);
    mystrcat(ret, " = &");
    mystrcat(ret, var->pointer.pointed->name);
    mystrcat(ret, ";\n");
    */
    //printf("//printPointerDecl\n");
    //printf("ret = %d\n", strlen(ret));
    //printf("//----End printPointerDecl\n");
    //char r[strlen(ret) + 1];
    //strcpy(r, ret);
    //free(ret);
    //return ret;
}

void printPointerDeclNoType(Variable *var)
{
    //printf("//----Start printPointerDecl\n");
    //char ret[1] = {};
    //char *ret = (char*) malloc(strlen(inttype2str[ultimateType(var)]) + 1 + pointerDepth(var) + strlen(var->name) + 4 + strlen(var->pointer.pointed->name) + 2 + 1 + 100);
    //ret[0] = '\0';
    size_t i, depth = pointerDepth(var);

    //char *ret = (char*) calloc(strlen(inttype2str[ultimateType(var)]) + 1 + depth + strlen(var->name) + 4 + strlen(var->pointer.pointed->name) + 2 + 1 , sizeof(char));
    //ret[0] = '\0';
    //printf("%s ", inttype2str[ultimateType(var)]);
    //mystrcat(ret, inttype2str[ultimateType(var)]);
    //mystrcat(ret, " ");
    for(i = 0; i < depth; ++i){
        putchar('*');
        //mystrcat(ret, "*");
    }

    printf("%s = &%s;\n", var->name, var->pointer.pointed->name);


    /*
    mystrcat(ret, var->name);
    mystrcat(ret, " = &");
    mystrcat(ret, var->pointer.pointed->name);
    mystrcat(ret, ";\n");
    */
    //printf("//printPointerDecl\n");
    //printf("ret = %d\n", strlen(ret));
    //printf("//----End printPointerDecl\n");
    //char r[strlen(ret) + 1];
    //strcpy(r, ret);
    //free(ret);
    //return ret;
}

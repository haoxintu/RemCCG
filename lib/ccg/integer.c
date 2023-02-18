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

static char const * const inttype2varid[_inttypemax] = {"c", "uc", "s", "us", "i", "ui", "li", "uli"};
char const * const inttype2str[_inttypemax] = {"int8_t", "uint8_t", "int16_t", "uint16_t", "int32_t", "uint32_t", "int64_t", "uint64_t",
    "signed", "unsigned", "_Bool", "_Complex", "char"};

// *Haoxin added
char const * const storage2str[_inttypemax] = {"static", "volatile", " ", "static volatile", "volatile static"};

static char *makeIntegerName(IntegerType type, Context *context)
{
    char buffer[32] = {0}, *ret;

    sprintf(buffer, "%s_%u", inttype2varid[type], context->nvars);
    ret = xmalloc(strlen(buffer) + 1);
    strcpy(ret, buffer);

    return ret;
}

void makeInteger(Variable *var, Context *context)
{
    var->intvar.type = rand() % _inttypemax;
    var->intvar.initializer = makeIntegerConstant(INTEGERTYPE_SIZE(var->intvar.type));
    var->name = makeIntegerName(var->intvar.type, context);
}

void printIntegerDecl(Variable *var)
{
    //char ret[1] = {};
    //char *ret = (char*)calloc(strlen(inttype2str[var->intvar.type]) + 1 + strlen(var->name) + 3 + strlen(var->intvar.initializer->value) + 2 + 1, sizeof(char));
    //ret[0] = '\0';
    //memset(ret, "0", 1000);


    //srand((unsigned)time(NULL));
    /*
    int randome = rand()%5;
    //printf("//randome = %d\n", randome);
    if (randome == 0)
        printf("static %s %s = %s;\n", inttype2str[var->intvar.type], var->name, var->intvar.initializer->value);
    if (randome == 1)
        printf("volatile %s %s = %s;\n", inttype2str[var->intvar.type], var->name, var->intvar.initializer->value);
    if (randome == 2)
        printf("static volatile %s %s = %s;\n", inttype2str[var->intvar.type], var->name, var->intvar.initializer->value);
    if (randome == 3)
        printf("volatile static %s %s = %s;\n", inttype2str[var->intvar.type], var->name, var->intvar.initializer->value);
    if (randome == 4)
    */
    //int tt = rand() % (_inttypemax - 3);
    //printf("//tt = %d\n", tt);
        //printf("%s %s %s = %s;\n",storage2str[tt], inttype2str[var->intvar.type], var->name, var->intvar.initializer->value);
        printf("%s %s = %s;\n", inttype2str[var->intvar.type], var->name, var->intvar.initializer->value);
    //snprintf(ret, 100, "%s %s = %s;\n", inttype2str[var->intvar.type], var->name, var->intvar.initializer->value);
    /*
    mystrcat(ret, inttype2str[var->intvar.type]);
    mystrcat(ret, " ");
    mystrcat(ret, var->name);
    mystrcat(ret, " = ");
    mystrcat(ret, var->intvar.initializer->value);
    mystrcat(ret, ";\n");
    */

    //printf("%s\n", ret);
    //char r[strlen(ret) + 1];
    //strcpy(r, ret);
    //free(ret);
    //return ret;
}

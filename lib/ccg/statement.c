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

//labelVariable labelvar;// = xmalloc(sizeof(*labelvar));
static const StatementType statarray[20] = {_if, _if, _if, _if, _for, _for, _for, _for, _assignment, _assignment, _assignment, _assignment, _ptrassignment, _ptrassignment, _ptrassignment, _functioncall, _functioncall, _functioncall, _goto, _return};

static void buildIf(Statement*, Context*, unsigned);
static void buildFor(Statement*, Context*, unsigned);
static void buildAssignment(Statement*, Context*, unsigned);
static void buildPtrAssignment(Statement*, Context*, unsigned);
static void buildFunctionCall(Statement*, Context*, unsigned);
static void buildReturn(Statement*, Context*, unsigned);
static void buildGoto(Statement*, Context*, unsigned);

static bool isFunction = 0; // For the purpose of aviod dumplicate label definition in function body

int compar(const void *l, const void *r)
{
    const strIntMap *lm = l;
    const strIntMap *lr = r;
    return strcmp(lm->key, lr->key);
}



static void (*buildfunctions[_statementtypemax])(Statement*, Context*, unsigned) =
{
    [_if] = buildIf,
    [_for] = buildFor,
    [_assignment] = buildAssignment,
    [_ptrassignment] = buildPtrAssignment,
    [_functioncall] = buildFunctionCall,
    [_return] = buildReturn,
    [_goto] = buildGoto
};

void addStatementToList(Statement *statement, StatementList **list)
{
    if(!*list)
    {
        *list = xmalloc(sizeof(**list));
        (*list)->statement = statement;
        (*list)->next = NULL;
    }

    else
    {
        StatementList *s;

        for(s = *list; s->next; s = s->next);
        s->next = xmalloc(sizeof(*s->next));
        s->next->statement = statement;
        s->next->next = NULL;
    }
}
/*
#define IS_INVALID ((type == _ptrassignment && (context->nvars == context->nintegers))\
                    || (type == _functioncall && program.numfunctions >= cmdline.max_functions)\
                    || (type == _return && (!nesting || !lastofblock))\
                    || (type == _goto && (cmdline.nojumps || (context->currfunc->numlabels == 0))))
*/
#define IS_INVALID ((type == _ptrassignment && (context->nvars == context->nintegers))\
                    || (type == _functioncall && program.numfunctions >= cmdline.max_functions)\
                    || (type == _return && (!nesting || !lastofblock))\
                    || (type == _goto && (cmdline.nojumps || (context->currfunc->numlabels == 0))))

Statement *makeStatement(Context *context, unsigned nesting, bool lastofblock)
{
    Statement *ret = xmalloc(sizeof(*ret));
    StatementType type;

    do
        type = statarray[rand() % (sizeof(statarray) / sizeof(*statarray))];
    while(IS_INVALID);
    //while(1);

    // 2% of chance to put a label
    if(!cmdline.nojumps && type != _goto && !(rand() % 50)){
        ret->label = makeLabel(context);
        printf("//label name in makeStatement : %s\n", ret->label->name);
        /*
        char *name = ret->label->name;
	    //find first
        strIntMap *find_a = malloc(sizeof(strIntMap));
        find_a->key = name;
        void *r = tfind(find_a, &rootMakeLabel, compar); // read
        //char * find = (*(strIntMap**)r)->key;
        //not find
        if (!r){
            //printf("%s:\n", ret->label->name);
            strIntMap *a = malloc(sizeof(strIntMap));
            a->key = name;
            a->value = 1;
    	    tsearch(a, &rootMakeLabel, compar); // insert
        }
        */
        //find it
        /*
        if (r){
            int ret = (*(strIntMap**)r)->value;
            if ( ret == 1){
                //print first
                printf("%s:\n", statement->label->name);
	            //then update value
                (*(strIntMap**)r)->value += 1;
            }
            //else{
            //    (*(strIntMap**)r)->value += 1;
            // }
        }*/
    }
    else
        ret->label = NULL;
    //ret->label = NULL;

    (buildfunctions[(ret->type = type)])(ret, context, nesting);

    return ret;
}

Statement *makeStatementForFunction(Context *context, unsigned nesting, bool lastofblock)
{
    Statement *ret = xmalloc(sizeof(*ret));
    StatementType type;

    do
        type = statarray[rand() % (sizeof(statarray) / sizeof(*statarray))];
    while(IS_INVALID);
    //while(1);

    // 0% of chance to put a label
    if(0){
        ret->label = makeLabel(context);
        //printf("//label name in makeStatement : %s\n", ret->label->name);
        char *name = ret->label->name;
	    //find first
        strIntMap *find_a = malloc(sizeof(strIntMap));
        find_a->key = name;
        void *r = tfind(find_a, &rootMakeLabel, compar); // read
        //char * find = (*(strIntMap**)r)->key;
        //not find
        if (!r){
            //printf("%s:\n", statement->label->name);
            strIntMap *a = malloc(sizeof(strIntMap));
            a->key = name;
            a->value = 2;
    	    tsearch(a, &rootMakeLabel, compar); // insert
        }
        //find it
        /*
        if (r){
            int ret = (*(strIntMap**)r)->value;
            if ( ret == 1){
                //print first
                printf("%s:\n", statement->label->name);
	            //then update value
                (*(strIntMap**)r)->value += 1;
            }
            //else{
            //    (*(strIntMap**)r)->value += 1;
            // }
        }*/
    }
    else
        ret->label = NULL;
    //ret->label = NULL;

    (buildfunctions[(ret->type = type)])(ret, context, nesting);

    return ret;
}
/***** Build functions *****/

static void buildIf(Statement *statement, Context *context, unsigned nesting)
{
    IfStatement *ifstatement = xmalloc(sizeof(*ifstatement));

    ifstatement->condition = makeExpression(context, 0);
    ifstatement->truepath = makeBlock(context, nesting + 1);

    /* An else branch is generated 30% of the time */
    ifstatement->falsepath = !(rand() % 3) ? makeBlock(context, nesting + 1) : NULL;

    statement->stmnt.ifstatement = ifstatement;
}

static void buildFor(Statement *statement, Context *context, unsigned nesting)
{
    ForStatement *forstatement = xmalloc(sizeof(*forstatement));

    forstatement->iterator = selectVariable(context, _randomvartype);

    /* The init var is in [-30;0[ u ]0;30] and the test val is in [30;89] */
    forstatement->init = (rand() % 30 + 1) * ((forstatement->iterator->type == _integer ? IS_UNSIGNED_INTEGERTYPE(forstatement->iterator->intvar.type) : IS_UNSIGNED_INTEGERTYPE(ultimateType(forstatement->iterator))) ? 1 : ((rand() % 2) ? -1 : 1));
    forstatement->testval = (rand() % 60 + 30);

    forstatement->testop = _lowerorequal;
    forstatement->incval = 1;
    forstatement->assignop = _assigninc;

    forstatement->body = makeBlock(context, nesting + 1);

    statement->stmnt.forstatement = forstatement;
}

static void buildFunctionCall(Statement *statement, Context *context, unsigned nesting)
{
    FunctionCallStatement *funccallstatement = xmalloc(sizeof(*funccallstatement));
    funccallstatement->paramlist = NULL;
    VariableList *v;

    funccallstatement->function = makeFunction(true);

    foreach(v, funccallstatement->function->paramlist)
        addExpressionToList(makeExpression(context, nesting + 1), (ExpressionList**) &funccallstatement->paramlist);

    statement->stmnt.funccallstatement = funccallstatement;
}

static void buildAssignment(Statement *statement, Context *context, unsigned nesting)
{
    AssignmentStatement *as = xmalloc(sizeof(*as));

    as->var = selectVariable(context, _randomvartype);
    as->op = rand() % _assignopmax /*_assign*/;
    as->expr = makeExpression(context, 0);

    statement->stmnt.assignmentstatement = as;
}

#define PTRASSIGNMENT_IS_CONSISTENT(lhs, rhs) (INTEGERTYPE_SIZE(ultimateType(lhs)) <= INTEGERTYPE_SIZE(ultimateType(rhs)))

static void buildPtrAssignment(Statement *statement, Context *context, unsigned nesting)
{
    Variable *v;
    PtrAssignmentStatement *pas = xmalloc(sizeof(*pas));

    pas->lhs = selectVariable(context, _pointer);

    do
        v = selectVariable(context, _randomvartype);
    while(!PTRASSIGNMENT_IS_CONSISTENT(pas->lhs, v));

    pas->rhs = v;

    statement->stmnt.ptrassignmentstatement = pas;
}

static void buildReturn(Statement *statement, Context *context, unsigned nesting)
{
    ReturnStatement *rs = xmalloc(sizeof(*rs));

    rs->retval = selectOperand(context);

    statement->stmnt.returnstatement = rs;
}

static void buildGoto(Statement *statement, Context *context, unsigned nesting)
{
    GotoStatement *gs = xmalloc(sizeof(*gs));

    gs->label = selectLabel(context);

    statement->stmnt.gotostatement = gs;
}


void printStatement(Statement *statement);

static void printIfStatement(Statement *statement)
{
    //char ret[1] = {};
    IfStatement *ifstatement = statement->stmnt.ifstatement;

    /*
    char *ret1 = printExpression(ifstatement->condition);
    char *ret2 = printBlock(ifstatement->truepath);
    char *ret = (char*) calloc(3 + strlen(ret1) + 1 + strlen(ret2) + 1, sizeof(char));
    */
    //ret[0] = '\0';
    fputs("if(", stdout);
    //mystrcat(ret, "if(");
    printExpression(ifstatement->condition);
    //mystrcat(ret, ret1);
    puts(")");
    //mystrcat(ret, ")");
    printBlock(ifstatement->truepath);
    //mystrcat(ret, ret2);

    if(ifstatement->falsepath)
    {
        puts("else");
        //char *t1 = "else";
        //ret = (char*) realloc(ret, strlen(ret) + strlen(t1) + 1);
        //mystrcat(ret, t1);
        printBlock(ifstatement->falsepath);
        //char * tt = printBlock(ifstatement->falsepath);
        //ret = (char*) realloc(ret, strlen(ret) + strlen(tt) + 1);
        //mystrcat(ret, tt);
    }
    //printf("%s\n", ret);
    //return ret;
}


static void printForStatement(Statement *statement)
{
    //char ret[1] = {};
    ForStatement *forstatement = statement->stmnt.forstatement;
    char *id = USABLE_ID(forstatement->iterator);

    printf("for(%s = %d; %s %s %d; %s %s %d)\n", id, forstatement->init,
           id, testop2str[forstatement->testop], forstatement->testval,
           id, assignop2str[forstatement->assignop], forstatement->incval);

    /*
    char *ret1 = printBlock(forstatement->body);

    char *ret = (char*) malloc(5 + strlen(id) + 3 + 10 + 1 + strlen(id) + 1 + strlen(testop2str[forstatement->testop])
                                + 1 + 10 + 2 + strlen(id) + 1 + strlen(assignop2str[forstatement->assignop]) + 10 + 2 + strlen(ret1) + 1);
    ret[0] = '\0';
    mystrcat(ret, "\nfor( ");
    mystrcat(ret, id);
    mystrcat(ret, " = ");
    char i1[10];
    sprintf(i1, "%d", forstatement->init);
    mystrcat(ret, i1);
    mystrcat(ret, ";");
    mystrcat(ret, " ");
    mystrcat(ret, id);
    mystrcat(ret, " ");
    mystrcat(ret, testop2str[forstatement->testop]);
    mystrcat(ret, " ");
    char i2[10];
    sprintf(i2, "%d", forstatement->testval);
    mystrcat(ret, i2);
    mystrcat(ret, "; ");
    mystrcat(ret, id);
    mystrcat(ret, " ");
    mystrcat(ret, assignop2str[forstatement->assignop]);
    mystrcat(ret, " ");
    char i3[10];
    sprintf(i3, "%d", forstatement->incval);
    mystrcat(ret, i3);
    mystrcat(ret, ")\n");

    mystrcat(ret, ret1);
    */


    printf("\n");
    printBlock(forstatement->body);
    //printf("%s\n", ret);
    //return ret;
}


static void printAssignmentStatement(Statement *statement)
{
    //char ret[1] = {};
    AssignmentStatement *assignstatement = statement->stmnt.assignmentstatement;

    printf("%s %s ", USABLE_ID(assignstatement->var), assignop2str[assignstatement->op]);
    printExpression(assignstatement->expr);
    puts(";");

    /*
    char *ret1 = printExpression(assignstatement->expr);
    char *ret = (char*) malloc(strlen(USABLE_ID(assignstatement->var)) + 1 + strlen(assignop2str[assignstatement->op]) + 1 + strlen(ret1) + 1 + 1);
    ret[0] = '\0';
    mystrcat(ret, USABLE_ID(assignstatement->var));
    mystrcat(ret, " ");
    mystrcat(ret, assignop2str[assignstatement->op]);
    mystrcat(ret, " ");
    mystrcat(ret, ret1);
    mystrcat(ret, ";");
    */


    //printf("%s\n", ret);
    //return ret;
}

static void printPtrAssignmentStatement(Statement *statement)
{
    //char ret[1] = {};
    PtrAssignmentStatement *pas = statement->stmnt.ptrassignmentstatement;
    size_t lhsdepth = pointerDepth(pas->lhs), rhsdepth = pointerDepth(pas->rhs);

    /*
    char *ret1;
    if (pas->rhs->type == _pointer)
    {
        if (lhsdepth == rhsdepth)
            ret1 = " ";
        else if(lhsdepth < rhsdepth)
            ret1 = genStars(rhsdepth - lhsdepth);
        else
            ret1 = genStars(lhsdepth - rhsdepth);
    }else{
        ret1 = genStars(lhsdepth - rhsdepth - 1);
    }
    */

    //char *ret = (char*) malloc(strlen(pas->lhs->name) + strlen(pas->rhs->name) + 3 + 2 + 1);
    //char *ret = (char*) malloc(500);
    //ret[0] = '\0';
    if(pas->rhs->type == _pointer)
    {
        if(lhsdepth == rhsdepth) {
            printf("%s = %s;\n", pas->lhs->name, pas->rhs->name);
            /*
            mystrcat(ret, pas->lhs->name);
            mystrcat(ret, " = ");
            mystrcat(ret, pas->rhs->name);
            mystrcat(ret, ";\n");
            */


        }
        else if(lhsdepth < rhsdepth){
            printf("%s = %s%s;\n", pas->lhs->name, genStars(rhsdepth - lhsdepth), pas->rhs->name);
            /*
            mystrcat(ret, pas->lhs->name);
            mystrcat(ret, " = ");
            char *ret1 = genStars(rhsdepth - lhsdepth);
            ret = (char*) realloc(ret, strlen(ret) + strlen(ret1) + 1);
            mystrcat(ret, ret1);
            mystrcat(ret, pas->rhs->name);
            mystrcat(ret, ";\n");
            */

        }
        else{
            printf("%s%s = %s;\n", genStars(lhsdepth - rhsdepth), pas->lhs->name, pas->rhs->name);
            /*
            char *ret1 = genStars(lhsdepth - rhsdepth);
            ret = (char*) realloc(ret, strlen(ret) + strlen(ret1) + 1);
            mystrcat(ret, ret1);
            mystrcat(ret, pas->lhs->name);
            mystrcat(ret, " = ");
            mystrcat(ret, pas->rhs->name);
            mystrcat(ret, ";\n");
            */


        }
    }

    else
    {
        printf("%s%s = &%s;\n", genStars(lhsdepth - rhsdepth - 1), pas->lhs->name, pas->rhs->name);
        /*
        char *ret1 = genStars(lhsdepth - rhsdepth - 1);
        ret = (char*) realloc(ret, strlen(ret) + strlen(ret1) + 1);
        mystrcat(ret, ret1);
        mystrcat(ret, pas->lhs->name);
        mystrcat(ret, " = ");
        mystrcat(ret, pas->rhs->name);
        mystrcat(ret, ";\n");
        */

    }
    //printf("%s\n", ret);
    //return ret;
}

static void printFunctionCallStatement(Statement *statement)
{
    //char ret[1] = {};
    ExpressionList *e;
    FunctionCallStatement *funccallstatement = statement->stmnt.funccallstatement;

    printf("\n%s(", funccallstatement->function->name);

    /*
    char *ret = (char*) malloc(1);
    ret[0] = '\0';
    mystrcat(ret, "\n");
    mystrcat(ret, funccallstatement->function->name);
    mystrcat(ret, "(");
    */


    foreach(e, funccallstatement->paramlist)
    {
        printExpression(e->expression);
        //char *ret1 = printExpression(e->expression);
        //ret = (char*) realloc(ret, strlen(ret) + strlen(ret1) + 1);
        //mystrcat(ret, ret1);

        if(e->next){
            fputs(", ", stdout);
            //char *tt = ", ";
            //ret = (char*) realloc(ret, strlen(ret) + strlen(ret1) + 1);
            //mystrcat(ret, ", ");
        }
    }

    puts(");");
    //mystrcat(ret, ");");
    //printf("%s\n", ret);
    //return ret;
}

static void printReturnStatement(Statement *statement)
{
    //char ret[1] = {};
    Operand *retval = statement->stmnt.returnstatement->retval;

    //char *ret = (char*) malloc(5 + strlen(retval->type == _variable ? USABLE_ID(retval->op.variable) : retval->op.constant->value) + 2 + 1);
    //ret[0] = '\0';
    printf("return %s;\n", retval->type == _variable ? USABLE_ID(retval->op.variable) : retval->op.constant->value);

    /*
    mystrcat(ret, "return ");
    mystrcat(ret, retval->type == _variable ? USABLE_ID(retval->op.variable) : retval->op.constant->value);
    mystrcat(ret, ";\n");
    */

    //printf("%s\n", ret);
    //return ret;
}
void *tempRoot = NULL;
static void printGotoStatement(Statement *statement)
{
    printf("goto %s;\n", statement->stmnt.gotostatement->label->name);
    //char ret[1] = {};
    //char *ret = (char*) malloc(5 + strlen(statement->stmnt.gotostatement->label->name) + 2 + 1);
    //ret[0] = '\0';
    //Special handling
    /*
    char *name = statement->stmnt.gotostatement->label->name;

    	//find first
        strIntMap *find_a = malloc(sizeof(strIntMap));
        find_a->key = name;
        void *r = tfind(find_a, &root, compar); // read
        //char * find = (*(strIntMap**)r)->key;
        strIntMap *find_b = malloc(sizeof(strIntMap));
        find_b->key = name;
        void *r1 = tfind(find_b, &tempRoot, compar); // read
        //not find
        if (!r && !r1){
            printf("//NOT FOUND!\n");
            //printf("goto %s;\n", statement->stmnt.gotostatement->label->name);
            //printf("%s: ;\n", name);
            strIntMap *a = malloc(sizeof(strIntMap));
            //printf("goto %s;\n", statement->stmnt.gotostatement->label->name);
            a->key = name;
            a->value = 2;
    	    //tsearch(a, &tempRoot, compar); // insert
        }// insert
        else
            ;//printf("goto %s;\n", statement->stmnt.gotostatement->label->name);
            */

    /*
    char *name = statement->stmnt.gotostatement->label->name;
	    //find first
        strIntMap *find_a = malloc(sizeof(strIntMap));
        find_a->key = name;
        void *r = tfind(find_a, &rootMakeLabel, compar); // read
        //char * find = (*(strIntMap**)r)->key;
        //not find
        if (r){
            printf("//Found it in rootMakeLabel ! %s %p:\n", name, r);
            //strIntMap *a = malloc(sizeof(strIntMap));
            //printf("goto %s;\n", statement->stmnt.gotostatement->label->name);
            //a->key = name;
            //a->value = 2;
    	    //tsearch(a, &rootMakeLabel, compar); // insert
        }
        */
    //printf("//here is a goto statement; skip for now\n");
    /*
    mystrcat(ret, "goto ");
    mystrcat(ret, statement->stmnt.gotostatement->label->name);
    mystrcat(ret, ";\n");
    */
    //printf("%s\n", ret);
    //return ret;
}

int inLabelList(labelVariable *labellist, char *name){
     labelVariable *t = labellist;
     while (!t){
	t = t->next;
	if (strcmp(t->name, name))
		return 1;

	}
	return 0;
}


void printStatement(Statement *statement)
{
    //char ret[1] = {};
    //printf("//printStatement in function body? %d\n", isFunction);
    static void (*printfunctions[_statementtypemax])(Statement*) =
    {
        [_if] = printIfStatement,
        [_for] = printForStatement,
        [_return] = printReturnStatement,
        [_assignment] = printAssignmentStatement,
        [_ptrassignment] = printPtrAssignmentStatement,
        [_functioncall] = printFunctionCallStatement,
        [_goto] = printGotoStatement
    };

    /*
    char *ret1 = (printfunctions[statement->type])(statement);
    char *ret = (char*) malloc(strlen(ret1)  + 2 + 1 );
    ret[0] = '\0';
    */
    //Special handling

    if(statement->label){
	    char *name = statement->label->name;
        printf("%s:\n", statement->label->name);
    }
    /*

    	//find first
        strIntMap *find_a = malloc(sizeof(strIntMap));
        find_a->key = name;
        void *r = tfind(find_a, &root, compar); // read
        //char * find = (*(strIntMap**)r)->key;
        //not find
        if (!r){
            //printf("//also in?\n");
            printf("%s:\n", statement->label->name);
            strIntMap *a = malloc(sizeof(strIntMap));
            a->key = name;
            a->value = 1;
    	    tsearch(a, &root, compar); // insert
        }
        //find it
        if (r){
            int ret = (*(strIntMap**)r)->value;
            if ( ret == 1){
                //print first
                //printf("//Here are in rootMakeLabel\n");
                if (!isFunction)
                    printf("%s:\n", statement->label->name);
	            //then update value
                (*(strIntMap**)r)->value += 1;
            }
            //else{
            //    (*(strIntMap**)r)->value += 1;
            // }
            tdelete(find_a, &rootMakeLabel, compar); // delete it in rootMakeLabel
        }
        //printf("%s:\n", statement->label->name);
    }

    else { // print rootMakeLabel
        if (rootMakeLabel != 0 ){
            if ((*(strIntMap**)rootMakeLabel)->value == 2){
                //strIntMap *find_a = malloc(sizeof(strIntMap));
                //void *r = tfind(find_a, &root, compar); // read
                printf("//Here are in rootMakeLabel\n");
                (*(strIntMap**)rootMakeLabel)->value = 1;
                printf("%s :\n", (*(strIntMap**)rootMakeLabel)->key);
            }else{
                printf("%s :\n", (*(strIntMap**)rootMakeLabel)->key);
                tdelete((*(strIntMap**)rootMakeLabel), &rootMakeLabel, compar); // delete it in rootMakeLabel
            }
        }
    }
    */
    (printfunctions[statement->type])(statement);
    //mystrcat(ret, ret1);
    //printf("%s\n", ret);
    //return ret;
}

static void printFakeGotoStatement(Statement *statement){
    printf(";");
}

void printStatementForFunction(Statement *statement, bool isFunc)
{
    isFunction = isFunc;
    //char ret[1] = {};
    static void (*printfunctions[_statementtypemax])(Statement*) =
    {
        [_if] = printIfStatement,
        [_for] = printForStatement,
        [_return] = printReturnStatement,
        [_assignment] = printAssignmentStatement,
        [_ptrassignment] = printPtrAssignmentStatement,
        [_functioncall] = printFunctionCallStatement,
        //[_goto] = printFakeGotoStatement
        [_goto] = printGotoStatement
    };
    if(statement->label){
	    char *name = statement->label->name;
        printf("%s:\n", statement->label->name);
    }
    (printfunctions[statement->type])(statement);

}



void printStatementNoLabel(Statement *statement)
{
    //char ret[1] = {};
    static void (*printfunctions[_statementtypemax])(Statement*) =
    {
        [_if] = printIfStatement,
        [_for] = printForStatement,
        [_return] = printReturnStatement,
        [_assignment] = printAssignmentStatement,
        [_ptrassignment] = printPtrAssignmentStatement,
        [_functioncall] = printFunctionCallStatement,
        [_goto] = printFakeGotoStatement
    };
    //if (statement->type != _for || statement->type != _if)
    	(printfunctions[statement->type])(statement);

	//printf("//Here we should handle the dup label! skip it for now\n");
	//printf("{}\n");
    //mystrcat(ret, ret1);
    //printf("%s\n", ret);
    //return ret;
}


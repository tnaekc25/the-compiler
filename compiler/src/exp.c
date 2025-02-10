
#include "comp.h"


struct exp *new_Var(hinfo *inf) {

	struct exp *np = (struct exp*)malloc(sizeof(struct exp));
	np -> typ = VAR;
	np -> exps._var.inf = inf;

	return np;
}

struct exp *new_Func(const char *name, size_t argc) {

	struct exp *np = (struct exp*)malloc(sizeof(struct exp));
	np -> typ = FUNC;
	np -> exps._func.key = 666666;
	np -> exps._func._retval = NULL;

	np -> exps._func.argc = argc;
	np -> exps._func.args = (struct exp**)malloc(
		sizeof(struct exp)*argc);

	return np;
}

struct exp *new_Int(long int val) {

	struct exp *np = (struct exp*)malloc(sizeof(struct exp));
	np -> typ = INTT;
	np -> exps._int.val = val;

	return np;
}

struct exp *new_Flt(long double val) {

	struct exp *np = (struct exp*)malloc(sizeof(struct exp));
	np -> typ = FLT;
	np -> exps._flt.val = val;

	return np;	
}

struct exp *new_Str(const char *val, int sp, int ep) {
	
	char *c;
	struct exp *np = (struct exp*)malloc(sizeof(struct exp));
	
	np -> typ = STR;

	c = (char*)malloc(sizeof(char)*(ep-sp));
	strncpy(c, val+sp+1, ep-sp-1);
	np -> exps._str.val = c;

	return np;
}

struct exp *new_Bool(bool val) {

	struct exp *np = (struct exp*)malloc(sizeof(struct exp));
	np -> typ = BOL;
	np -> exps._bool.val = val;

	return np;
}

struct exp *new_Op(char ops, bool binary) {

	struct exp *np = (struct exp*)malloc(sizeof(struct exp));
	np -> typ = binary ? bOP : uOP;
	np -> exps._op.ops = ops;

	return np;
}

void setOpExp(struct exp *ep, struct exp *e1, struct exp *e2) {
	ep -> exps._op._exp1 = e1;
	ep -> exps._op._exp2 = e2;
}

void freeExp(struct exp *e) {free(e);}










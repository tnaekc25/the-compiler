
#include "comp.h"

/*Stack*/

Stack *newStack(size_t size) {
	Stack *np = (Stack*)malloc(sizeof(Stack));
	np -> size = size;
	np -> arr = (char*)malloc(sizeof(char)*size);
	np -> top = -1;

	return np;
}

char top(Stack *st) {
	if (st -> top >= 0)
		return st -> arr[st -> top];
	return -1;
}

void push(Stack *st, char val) {
	st -> arr[++st -> top] = val;
}

char pop(Stack *st) {
	if (st -> top >= 0)
		return st -> arr[st -> top--];
	return -1;
}

void delst(Stack *st) {
	free(st -> arr);
	free(st);
}

/*!Stack*/


/*expStack*/

expStack *newexpStack(size_t size) {
	expStack *np = (expStack*)malloc(sizeof(expStack));
	np -> size = size;
	np -> arr = (struct exp**)malloc(sizeof(struct exp*)*size);
	np -> top = -1;

	return np;
}

struct exp *exptop(expStack *st) {
	if (st -> top >= 0)
		return st -> arr[st -> top];
	return NULL;
}

void exppush(expStack *st, struct exp *val) {
	st -> arr[++st -> top] = val;
}

struct exp *exppop(expStack *st) {
	if (st -> top >= 0)
		return st -> arr[st -> top--];
	return NULL;
}

void expdelst(expStack *st) {
	free(st -> arr);
	free(st);
}

/*!expStack*/

#include "comp.h"

void printExpTree(struct exp *node, int space) {

	int i;

	if (node -> typ != bOP && node -> typ != uOP) {
		printf("\n");
		for (i = 5;i < space;i++)
			printf(" ");
	}

	if (node -> typ == VAR) 
		printf("<%d>", (int)node -> exps._var.inf -> typs._var.addr);
	else if (node -> typ == FUNC) {

		int j;

		printf("%d[%d]", (int)node -> exps._func.key, (int)node -> exps._func.argc);
		
		for (j = 0;j < node -> exps._func.argc;j++) {
			printf("\n");
			for (i = 5;i < space;i++)
				printf(" ");
			printf("ARG: ");
			printExpTree(node -> exps._func.args[j], space + 5);
		}
	}

	else if (node -> typ == INTT)
		printf("%d", (int)node -> exps._int.val);
	else if (node -> typ == FLT)
		printf("%f", (float)node -> exps._flt.val);
	else if (node -> typ == STR)
		printf("%s", node -> exps._str.val);
	else if (node -> typ == BOL)
		printf("%d", node -> exps._bool.val);

	else if (node -> typ == bOP) {
		printExpTree(node -> exps._op._exp1, space+5);

		printf("\n");
		for (i = 5;i < space;i++)
			printf(" ");

		printf("%c", node -> exps._op.ops);
		printExpTree(node -> exps._op._exp2, space+5);
	}

	else if (node -> typ == uOP) {
		printExpTree(node -> exps._op._exp1, space+5);

		printf("\n");
		for (i = 5;i < space;i++)
			printf(" ");

		printf("%c", node -> exps._op.ops);
	}
}


void printStatTree(struct stat *node, int space) {

	int i;

	if (!node) return;

	if (node -> typ <= ELSE && node -> typ >= WHILE) {
		
		printStatTree(node -> stats._para._jump, space+5);

		printf("\n");
		for (i = 5;i < space;i++)
			printf(" ");

		if (node -> typ == WHILE) 
			printf("while");
		else if (node -> typ == FOR)
			printf("for");
		else if (node -> typ == IF)
			printf("if");
		else if (node -> typ == ELSE)
			printf("else");

	}

	else if (node -> typ == EXP) {
		printExpTree(node -> stats._para._cond, space+5);

		printf("\n");
		for (i = 5;i < space;i++)
			printf(" ");
		printf("EXP");
	}

	if (node -> next)
		printStatTree(node -> next, space+5);
}




int calculateTree(struct exp *root) {

	if (!root) return 0;

	if (root -> typ == bOP) {

		struct exp *e1 = root -> exps._op._exp1, *e2 = root -> exps._op._exp2;

		switch (root -> exps._op.ops) {

			case '+':
				return calculateTree(e1) + calculateTree(e2); 
			case '-':
				return calculateTree(e1) - calculateTree(e2); 
			case '*':
				return calculateTree(e1) * calculateTree(e2); 
			case '/':
				return calculateTree(e1) / calculateTree(e2);
			case '=':
				return calculateTree(e2); 
		};
	}

	else if (root -> typ == uOP) {

		struct exp *e1 = root -> exps._op._exp1;

		switch (root -> exps._op.ops) {
			case '.':
				return calculateTree(e1); 
			case '~':
				return -calculateTree(e1); 
		};
	}

	else if (root -> typ == INTT) {
		return root -> exps._int.val;
	}

	else if (root -> typ == FLT)
		return root -> exps._flt.val;

	else if (root -> typ == BOL)
		return root -> exps._bool.val ? 1 : 0;

	return 0;
}
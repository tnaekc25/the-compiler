
#include "comp.h"


struct stat *new_Stat(struct exp *cond, enum StatTyp typ) {

	struct stat *np = (struct stat*)malloc(sizeof(struct stat));
	np -> typ = typ;

	np -> stats._para._cond = cond;
	
	np -> stats._para._jump = NULL;
	np -> next = NULL;

	return np;
}

void freeStat(struct stat *s) {free(s);}



#ifndef _TYPES_H_
#define _TYPES_H_


struct varinf {
	int addr;
	enum StatTyp type;
	unsigned int block;

};

struct varexp {
	struct varinf *inf;
};

struct funcexp {
	unsigned long key;
	size_t argc;
	struct exp *_retval, **args;
};

struct intexp {
	long int val;
};

struct fltexp {
	long double val;
};

struct strexp {
	const char *val;
};

struct boolexp {
	bool val;
};

struct opexp {
	char ops;
	struct exp *_exp1, *_exp2;
};


struct exp {
	
	enum ExpTyp typ;

	union {
		struct varexp _var;
		struct funcexp _func;
		struct intexp _int;
		struct fltexp _flt;
		struct strexp _str;
		struct boolexp _bool;
		struct opexp _op;
	} exps;	
};


struct exp *new_Var(struct varinf *inf);
struct exp *new_Func(const char *name, size_t argc);
struct exp *new_Int(long int val);
struct exp *new_Flt(long double val);
struct exp *new_Str(const char *val, int sp, int ep);
struct exp *new_Bool(bool val);
struct exp *new_Op(char ops, bool binary);
void setOpExp(struct exp *ep, struct exp *e1, struct exp *e2);
void freeExp(struct exp *e);




struct parastat {
	struct exp *_cond;
	struct stat *_jump;
};

struct stat {

	enum StatTyp typ;

	union {
		struct parastat _para;
	} stats;

	struct stat *next;
};


struct stat *new_Stat(struct exp *cond, enum StatTyp typ);
void freeStat(struct stat *s);

#endif

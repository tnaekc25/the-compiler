
#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#define MAX_LOAD 0.5


typedef struct stack {
	size_t size;
	ssize_t top;
	char *arr;
} Stack;

typedef struct expstack {
	size_t size;
	ssize_t top;
	struct exp **arr;
} expStack;

Stack *newStack(size_t size);
char top(Stack *st);
void push(Stack *st, char val);
char pop(Stack *st);
void delst(Stack *st);

expStack *newexpStack(size_t size);
struct exp *exptop(expStack *st);
void exppush(expStack *st, struct exp *val);
struct exp *exppop(expStack *st);
void expdelst(expStack *st);


struct trienode {
	bool key;
	enum StatTyp v;
	struct trienode *arr[29];
};

struct trienode *buildStatTrie();
enum StatTyp getToken(const char *token, struct trienode *node, size_t *s);
void delete_Trie(struct trienode *root);


typedef struct _pair {

	char *skey;
	bool imported;
	hinfo *inf;

} pair;



typedef struct hashTable {

	size_t size;
	size_t capacity;

	pair **arr;

} HashTable;


hinfo *new_varinf(int addr, enum StatTyp type);
hinfo *new_funcinf(struct stat *jump, 
	size_t argc, hinfo **args, enum StatTyp type);

HashTable *new_HashTable(size_t capacity);
void delete_HashTable(HashTable *tp); 
void deleteTable(HashTable *tp);
bool pushTable(HashTable *tp, char *str, hinfo *inf, bool imported);
hinfo *getfTable(HashTable *tp, char *str);
void printTable(HashTable *tp);


#endif

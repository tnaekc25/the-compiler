
bool isempty(char c);
bool isnum(char c);
bool isname(char c);
bool isbinary(char c);
bool istoken(char c);
long int strtoint(const char *str, int s, int e);

struct exp *getexptree(const char *line, size_t start, size_t end, HashTable *vartable);
struct stat *getstat(char *line, HashTable *vartable, FILE *fp);
struct stat *parseblock(FILE *fp, char *holder, HashTable *prevtable, HashTable *extratable);

void printExpTree(struct exp *node, int space);
void printStatTree(struct stat *node, int space);
int calculateTree(struct exp *root);
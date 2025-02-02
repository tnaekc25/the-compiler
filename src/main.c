
#include "comp.h"


int main() {

	FILE *fp = fopen("test.the", "r");

	struct trienode *stattrie = buildStatTrie();
	HashTable *vartable = new_HashTable(TABLE_SIZE);
	
	char holder[CONT_SIZE];
	struct stat *root;

	if (!fp) return 1;

	root = parseblock(fp, holder, stattrie, vartable);
	printStatTree(root, 5);

	delete_Trie(stattrie);
	delete_HashTable(vartable);

	printf("\n\nnoseg");

	return 0;
}



/*char *line = "a && 5 >= 3 ";
struct exp *e1 = getexptree(line, 0, -1);
printExpTree(e1, 5);*/
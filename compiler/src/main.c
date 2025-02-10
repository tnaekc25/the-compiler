
#include "comp.h"


struct trienode *stattrie;

int main() {

	FILE *fp = fopen("test.the", "r");

	char holder[CONT_SIZE];
	struct stat *root;

	stattrie = buildStatTrie();
	if (!fp) return 1;

	root = parseblock(fp, holder, NULL, NULL);
	printStatTree(root, 5);

	delete_Trie(stattrie);

	printf("\n\nnoseg");

	return 0;
}



/*char *line = "a && 5 >= 3 ";
struct exp *e1 = getexptree(line, 0, -1);
printExpTree(e1, 5);*/
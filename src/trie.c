
#include "comp.h"

struct trienode *new_TrieNode(enum StatTyp v) {

	int i;
	struct trienode *np = (struct trienode*)malloc(sizeof(struct trienode));

	for (i = 0;i < 29;i++)
		np -> arr[i] = NULL;
	np -> key = false;
	np -> v = v;

	return np;
}

void insertTrie(const char *token, struct trienode *node, enum StatTyp v) {

	while(*token) {
		if (node -> arr[(*token)-97])
			node = node -> arr[(*token)-97];
		else
			node = node -> arr[(*token)-97] = new_TrieNode(v);

		token++;
	}

	node -> key = true;
}

void delete_Trie(struct trienode *root) {

	int i;
	for (i = 0;i < 29;i++)
		if (root -> arr[i])
			delete_Trie(root -> arr[i]);

	free(root);
}

enum StatTyp getToken(const char *token, struct trienode *node, size_t *s) {

	while (istoken(token[*s])) {

		if (node -> arr[token[*s]-97])
			node = node -> arr[token[*s]-97];
		else
			return NAN;

		(*s)++;
	}

	return (node && node -> key) ? node -> v : NAN;
}


struct trienode *buildStatTrie() {
	struct trienode *root = new_TrieNode(NAN);

	insertTrie("if", root, IF);
	insertTrie("else", root, ELSE);
	insertTrie("while", root, WHILE);
	insertTrie("for", root, FOR);

	insertTrie("int", root, INTDEC);
	insertTrie("float", root, FLTDEC);
	insertTrie("string", root, STRDEC);
	insertTrie("bool", root, BOLDEC);

	return root;
}





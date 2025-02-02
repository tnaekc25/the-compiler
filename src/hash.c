
#include "comp.h"

HashTable *new_HashTable(size_t capacity) {

	HashTable *np = (HashTable*)malloc(sizeof(HashTable));

	np -> capacity = capacity;
	np -> size = 0;

	np -> arr = (pair**)calloc(capacity, sizeof(pair*));
	np -> dummy = (pair*)malloc(sizeof(pair));

	return np;
}

pair *new_Pair(char *str, struct varinf *inf) {

	pair *np = (pair*)malloc(sizeof(pair));

	np -> key = str;
	np -> val = inf;

	return np;
}


size_t hash(char *str, size_t capacity) {

	unsigned long int val = 0;

	while (*str)
		val = val*37 + (*(str++));

	return val % capacity;
}

void pushTable(HashTable *tp, char *str, struct varinf *inf) {

	size_t key = hash(str, tp -> capacity), i = 1;

	if (tp -> size / (float)tp -> capacity > 0.5)
		resizeTable(tp);

	while (tp -> arr[key] && tp -> arr[key] != tp -> dummy) {
		key = (key + i*i) % (tp -> capacity);
		i++;
	}

	tp -> arr[key] = new_Pair(str, inf);

	tp -> size++;
}


struct varinf *retfTable(HashTable *tp, char *str) {

	size_t key = hash(str, tp -> capacity), i = 1;

	while (tp -> arr[key])
		if (strcmp(tp -> arr[key] -> key, str) == 0)
			return tp -> arr[key] -> val;
		else {
			key = (key + i*i) % tp -> capacity;
			i++;
		}

	return NULL;
}


int remvfTable(HashTable *tp, char *str) {

	size_t key = hash(str, tp -> capacity), i = 1;

	while (tp -> arr[key])
		if (strcmp(tp -> arr[key] -> key, str) == 0) {
			free(tp -> arr[key]);
			tp -> arr[key] = tp -> dummy;
			return 1;
		}

		else {
			key = (key + i*i) % tp -> capacity;
			i++;
		}

	return -1;
}


void resizeTable(HashTable *tp) {

	size_t oldc = tp -> capacity;
	pair **temp = tp -> arr;
	int i;

	tp -> capacity = oldc * 2 + 1;
	tp -> size = 0;
	tp -> arr = (pair**)calloc(tp -> capacity, sizeof(pair*));

	for (i = 0;i < oldc;i++)
		if (temp[i] && temp[i] != tp -> dummy) {
			pushTable(tp, temp[i] -> key, temp[i] -> val);
			free(temp[i]);
		}

	free(temp);
}


void delete_HashTable(HashTable *tp) {

	int i;

	for (i = 0;i < tp -> capacity;i++)
		if (tp -> arr[i] && tp -> arr[i] != tp -> dummy) {
			free(tp -> arr[i] -> key);
			free(tp -> arr[i] -> val);
			free(tp -> arr[i]);
		}
		
	free(tp -> arr);
	free(tp -> dummy);
}


void printTable(HashTable *tp) {

	int i = 0;

	printf("\n");

	for (;i < tp -> capacity;i++)
		if (tp -> arr[i] == tp -> dummy)
			printf("%d DUMMY\n", i);
		else if (tp -> arr[i])
			printf("%d %s -> %d\n", i, tp -> arr[i] -> key,
			 -1);
		else
			printf("%d NULL\n", i);
}








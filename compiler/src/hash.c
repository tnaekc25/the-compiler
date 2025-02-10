
#include "comp.h"




/*varinf*/

hinfo *new_varinf(int addr, enum StatTyp type) {

	hinfo *np = (hinfo*)malloc(sizeof(hinfo));
	np -> typs._var.addr = addr;
	
	np -> type = type;
	np -> used = false;
	np -> isvar = true;

	return np;
}


/*funcinf*/

hinfo *new_funcinf(struct stat *jump, size_t argc,
 hinfo **args, enum StatTyp type) {

	hinfo *np = (hinfo*)malloc(sizeof(hinfo));

	np -> typs._func.jump = jump;
	np -> typs._func.argc = argc;
	np -> typs._func.args = args;


	np -> type = type;
	np -> used = false;
	np -> isvar = false;

	return np;
}



/*Pair*/

pair *new_Pair(char *str, hinfo *inf, bool imported) {

	pair *np = (pair*)malloc(sizeof(pair));

	np -> skey = str;
	np -> inf = inf;
	np -> imported = imported;

	return np;
}

void delete_Pair(pair *p) {

	if (p -> imported == false)
		free(p -> skey);
	free(p);
}

/*!Pair*/


size_t hash(char *str, size_t capacity) {

	unsigned long int val = 0;

	while (*str)
		val = val*37 + (*(str++));

	return val % capacity;
}



/*Destructor - Constructor */

HashTable *new_HashTable(size_t capacity) {

	HashTable *np = (HashTable*)malloc(sizeof(HashTable));

	np -> capacity = capacity;
	np -> size = 0;

	np -> arr = (pair**)calloc(capacity, sizeof(pair*));

	return np;
}

void delete_HashTable(HashTable *tp) {

	int i;

	for (i = 0;i < tp -> capacity;i++)
		if (tp -> arr[i]) {
			
			if (tp -> arr[i] -> inf 
				&& tp -> arr[i] -> inf -> used == false
				&& tp -> arr[i] -> imported == false) 
					free(tp -> arr[i] -> inf);

			delete_Pair(tp -> arr[i]);
		}

	free(tp -> arr);
}

void deleteTable(HashTable *tp) {

	int i;

	for (i = 0;i < tp -> capacity;i++)
		if (tp -> arr[i])
			free(tp -> arr[i]);

	free(tp -> arr);
}

/*!Destructor - Constructor */



void resizeTable(HashTable *tp) {

	size_t oldc = tp -> capacity;
	pair **temp = tp -> arr;
	int i;

	tp -> capacity = oldc * 2 + 1;
	tp -> size = 0;
	tp -> arr = (pair**)calloc(tp -> capacity, sizeof(pair*));

	for (i = 0;i < oldc;i++)
		if (temp[i]) {
			pushTable(tp, temp[i] -> skey, temp[i] -> inf, temp[i] -> imported);
			delete_Pair(temp[i]);
		}

	free(temp);
}


bool pushTable(HashTable *tp, char *str, hinfo *inf, bool imported) {

	size_t key, i = 1;
	bool newp = true;

	if (tp -> size / (float)tp -> capacity > MAX_LOAD)
		resizeTable(tp);

	key = hash(str, tp -> capacity);

	while (tp -> arr[key]) {

		if (strcmp(tp -> arr[key] -> skey, str) == 0) {
			if (tp -> arr[key] -> imported) 
				{newp = false; break;}

			else return 1;
		}

		key = (key + i*i) % (tp -> capacity);
		i++;
	}

	if (newp)
		tp -> arr[key] = new_Pair(str, inf, imported);
	else {
		tp -> arr[key] -> skey = str;
		tp -> arr[key] -> imported = 0;
		tp -> arr[key] -> inf = inf;
	}

	tp -> size++;

	return 0;
}


hinfo *getfTable(HashTable *tp, char *str) {

	size_t key = hash(str, tp -> capacity), i = 1;

	while (tp -> arr[key])
		if (strcmp(tp -> arr[key] -> skey, str) == 0)
			return tp -> arr[key] -> inf;
		else {
			key = (key + i*i) % tp -> capacity;
			i++;
		}

	return NULL;
}



void printTable(HashTable *tp) {

	int i = 0;

	printf("\n");

	for (;i < tp -> capacity;i++)
		if (tp -> arr[i] && tp -> arr[i] -> inf -> isvar)
			printf("%d <%d> %s -> %d\n", i, tp -> arr[i] -> imported,
			 tp -> arr[i] -> skey, tp -> arr[i] -> inf -> typs._var.addr);
		else
			printf("%d NULL\n", i);
}
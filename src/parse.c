
#include "comp.h"


struct exp *getexptree(const char *line, size_t start, size_t end, HashTable *vartable) {

	int sp, ep;
	size_t i = start;
	char c;
	bool preop = true;

	Stack *st = newStack(CONT_SIZE);
	expStack *est = newexpStack(CONT_SIZE);

	struct exp *qe[CONT_SIZE], *temp;
	size_t back = 0, front  = 0;


	while ((end < 0 || i <= end) && (c = line[i])) {

		if ((c == '+' || c == '-' || (c == '!' && !(line[i+1] && line[i+1] == '='))) && preop) {
			push(st, ((c == '!') ? '!' : ((c == '-') ? '~' : '.')));
			preop = true;
		}

		else if (c == '*' || c == '/' || c == '%') {
			while ((
				top(st) == '*' || top(st) == '/' || top(st) == '%'
			 || top(st) == '~' || top(st) == '.' || top(st) == '!') && top(st) != '(') {
			 	
			 	char co = pop(st);
				qe[back++] = new_Op(co, isbinary(co));
			}

			push(st, c);
			preop = true;
		}

		else if (c == '+' || c == '-') {
			while ((
				top(st) == '+' || top(st) == '-'
			 || top(st) == '*' || top(st) == '/' || top(st) == '%'
			 || top(st) == '~' || top(st) == '.' || top(st) == '!') && top(st) != '(') {
				
				char co = pop(st);
				qe[back++] = new_Op(co, isbinary(co));
			}

			push(st, c);
			preop = true;
		}

		else if (c == '<' || c == '>') {

			while ((
				top(st) == '+' || top(st) == '-'
			 || top(st) == '*' || top(st) == '/' || top(st) == '%'
			 || top(st) == '~' || top(st) == '.' || top(st) == '!' 
			 || top(st) == '<' || top(st) == '>' 
			 || top(st) == 'b' || top(st) == 's') && top(st) != '(') {

				char co = pop(st);
				qe[back++] = new_Op(co, isbinary(co));
			}

			if (line[i+1] && line[i+1] == '=') {
				push(st, c == '>' ? 'b' : 's');
				i++;
			}

			else
				push(st, c);

			preop = true;
		}

		else if (c == '!') {
			while ((
				top(st) == '+' || top(st) == '-'
			 || top(st) == '*' || top(st) == '/' || top(st) == '%'
			 || top(st) == '~' || top(st) == '.' || top(st) == '!' 
			 || top(st) == '<' || top(st) == '>' 
			 || top(st) == 'b' || top(st) == 's'
			 || top(st) == 'e' || top(st) == 'n') && top(st) != '(') {

				char co = pop(st);
				qe[back++] = new_Op(co, isbinary(co));
			}

			push(st, 'n');
			i++;

			preop = true;
		}

		else if (c == '=') {
			if (!(line[i+1] && line[i+1] == '=')) {
				while ((
					top(st) == '+' || top(st) == '-'
				 || top(st) == '*' || top(st) == '/' || top(st) == '%'
				 || top(st) == '~' || top(st) == '.' || top(st) == '!' 
				 || top(st) == '<' || top(st) == '>') && top(st) != '(') {
					
					char co = pop(st);
					qe[back++] = new_Op(co, isbinary(co));
				}
	
				push(st, c);
				preop = true;
			}

			else {
				while ((
					top(st) == '+' || top(st) == '-'
				 || top(st) == '*' || top(st) == '/' || top(st) == '%'
				 || top(st) == '~' || top(st) == '.' || top(st) == '!' 
				 || top(st) == '<' || top(st) == '>' 
				 || top(st) == 'b' || top(st) == 's'
				 || top(st) == 'e' || top(st) == 'n') && top(st) != '(') {
	
					char co = pop(st);
					qe[back++] = new_Op(co, isbinary(co));
				}
	
				push(st, 'e');
				i++;
	
				preop = true;
			}
		}

		else if (line[i+1] && ((c == '&' && line[i+1] == '&') ||
			(c == '|' && line[i+1] == '|'))) {

			while ((
					top(st) == '+' || top(st) == '-'
				 || top(st) == '*' || top(st) == '/' || top(st) == '%'
				 || top(st) == '~' || top(st) == '.' || top(st) == '!' 
				 || top(st) == '<' || top(st) == '>' 
				 || top(st) == 'b' || top(st) == 's'
				 || top(st) == 'e' || top(st) == 'n'
				 || top(st) == '&' || top(st) == '|') && top(st) != '(') {
	
					char co = pop(st);
					qe[back++] = new_Op(co, isbinary(co));
				}
	
				push(st, c);
				i++;
	
				preop = true;
		}

		/*paranthesis handling*/

		else if (c == '(') {
			push(st, c);
			preop = true;
		}

		else if (c == ')') {
			while (top(st) >= 0 && top(st) != '(')
				qe[back++] = new_Op(pop(st), 1);
			pop(st);
			preop = false;
		}

		/*!paranthesis handling*/

		/*process string*/
		else if (c == '"') {
			sp = i;
			while ((c = line[++i]) != '"');
			ep = i;

			if (ep > sp)
				qe[back++] = new_Str(line, sp, ep);

			preop = false;
		}

		/*process int*/
		else if (isnum(c)) {
			sp  = i;
			do c = line[++i]; while (isnum(c));
			ep = --i;

			qe[back++] = new_Int(strtoint(line, sp, ep));
			
			preop = false;
		}

		/*process varname or function*/
		else if (isname(c)) {
			sp = i;
			do c = line[++i]; while (isname(c) || isnum(c));
			ep = --i;

			if (strncmp(line+sp, "true", ep-sp-1) == 0)
				qe[back++] = new_Bool(1);

			else if (strncmp(line+sp, "false", ep-sp-1) == 0)
				qe[back++] = new_Bool(0);
			
			
			else {
				/*process varname*/
				if (line[i] && line[i+1] != '(') {

					char *name;
					size_t cc = 0;
					struct varinf *inf;

					while(isname(line[i+(cc++)]));
					name = (char*)malloc(sizeof(char)*cc);
					name[--cc] = 0;
					while (cc--) name[cc] = line[i+cc];

					if (inf = retfTable(vartable, name))
						qe[back++] = new_Var(inf);
					else
						return NULL;
				}
				
				/*process function*/
				else {
					for (sp = 0, ep = 0;;sp++) 
						if (line[sp] == ',') ep++; 
						else if (line[sp] == ')') {ep++; break;}
					
					qe[back++] = new_Func("test", ep);

					sp = i+2;
					ep = 0;

					do  {
						c = line[++i];
						
						if (c == ',' || c == ')') {
							temp = getexptree(line, sp, i-1, vartable);
							qe[back-1] -> exps._func.args[ep++] = temp;
							sp = i+1;
						}

					} while (c != ')');
				}
			}
			
			preop = false;
		}

		i++;
	}

	while (top(st) >= 0) {
		char c = pop(st);
		qe[back++] = new_Op(c, isbinary(c));
	}

	delst(st);

	/*queue to tree*/
	while(front < back) {
		if (qe[front] -> typ == bOP) {
			struct exp *e1, *e2;
			temp = qe[front];

			e2 = exppop(est);
			e1 = exppop(est);

			setOpExp(temp, e1, e2);
			exppush(est, temp);
		}

		else if (qe[front] -> typ == uOP) {
			struct exp *e1;
			temp = qe[front];

			e1 = exppop(est);
			setOpExp(temp, e1, NULL);
			exppush(est, temp);
		}

		else
			exppush(est, qe[front]);

		front++;
	}
	/*!queue to tree*/

	/*get root*/
	temp = exppop(est);
	expdelst(est);

	return temp;
}



struct stat *getstat(const char *line, struct trienode *trie, HashTable *vartable,
 struct stat *prev, unsigned int blockid) {

	size_t i = 0, sp, ep;
	char c;

	while ((c = line[i])) {

		if (istoken(c)) {
			enum StatTyp typ = getToken(line, trie, &i);

			if (typ >= WHILE && typ <= IF) {

				while ((c = line[i++]) != '(');
				sp = i;
				while ((c = line[i]) != ')') i++;
				ep = i-1;

				if (prev)
					return (prev -> next = new_Stat(getexptree(line, sp, ep, vartable), typ));
				else 
					return new_Stat(getexptree(line, sp, ep, vartable), typ);
			}

			else if (typ == ELSE) {

				if (prev)
					return (prev -> next = new_Stat(NULL, ELSE));
				else 
					return (new_Stat(NULL, ELSE));
			}

			else if (typ >= INTDEC && typ <= BOLDEC) {

				char *name;	
				size_t cc = 0;
				struct varinf *inf;

				printf("VAR ");
				while (isempty((c = line[i]))) i++;
				if (line[i] == '*') {
					printf("POINTER ");
					while (isempty((c = line[++i])));
				} 
				
				while(isname(line[i+(cc++)]));
				name = (char*)malloc(sizeof(char)*cc);
				name[--cc] = 0;
				while (cc--) name[cc] = line[i+cc];

				inf = (struct varinf*)malloc(sizeof(struct varinf));
				inf -> addr = -1;
				inf -> type = typ;
				inf -> block = blockid;

				printf("<%s> ", name);
				printf("of block: %d ", blockid);

				if (retfTable(vartable, name)) {
					printf("NAME IN USE!\n");
					return NULL;
				}

				printf("\n");
				pushTable(vartable, name, inf);

				return NULL;
			}



			else if (typ == NAN) {
				
				struct exp *temp = getexptree(line, 0, -1, vartable);

				if (temp) {

					if (prev)
						return (prev -> next = new_Stat(temp, EXP));
					else 
						return new_Stat(temp, EXP);
				
				}
			}
		}

		else
			i++;
	}

	return NULL;
}



struct stat *parseblock(FILE *fp, char *holder, struct trienode *stattrie, HashTable *vartable) {

	struct stat *root = NULL;
	struct stat *prev = root;

	char c = fgetc(fp);
	int k = 0;


	while (c != EOF && c != '}') {

		if (c == ';' || c == '{') {
			holder[k] = 0;

			if (root)
				prev = getstat(holder, stattrie, vartable, prev, 0);
			else
				root = prev = getstat(holder, stattrie, vartable, prev, 0);

			k = 0;

			if (c == '{') {

				if (root) {
					if (prev -> typ >= WHILE && prev -> typ <= ELSE)
						prev -> stats._para._jump = parseblock(fp, holder, stattrie, vartable);				
					else
						prev = parseblock(fp, holder, stattrie, vartable);
				}

				else
					root = prev = parseblock(fp, holder, stattrie, vartable);
			}
		}

		else
			holder[k++] = c;

		c = fgetc(fp);
	}

	return root;
}














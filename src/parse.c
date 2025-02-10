
#include "comp.h"


extern struct trienode *stattrie;
int testc = 0;


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

			char *name;
			size_t cc = 0, ref;

			while(isname(line[i+(cc++)]));
			name = (char*)malloc(sizeof(char)*(ref=cc));
			name[--cc] = 0;
			while (cc--) name[cc] = line[i+cc];
			i+=ref-1;


			if (strcmp(name, "true") == 0)
				qe[back++] = new_Bool(1);

			else if (strcmp(name, "false") == 0)
				qe[back++] = new_Bool(0);
			
			else {
				/*process varname*/
				if (line[i] && line[i] != '(') {
					hinfo *inf;					

					if ((inf = getfTable(vartable, name)) && inf -> isvar) {
						inf -> used = true;
						qe[back++] = new_Var(inf);
					}

					else if (inf && inf -> isvar == false)
						printf("<%s> IS NOT VARNAME\n", name);

					else
						printf("VARNAME <%s> NOT FOUND\n", name);
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



struct stat *getstat(char *line, HashTable *vartable, FILE *fp) {

	size_t i = 0, sp, ep;
	char c;

	while ((c = line[i])) {

		if (istoken(c)) {
			enum StatTyp typ = getToken(line, stattrie, &i);
			struct stat *retstat;

			/*CONDITIONAL STATEMENTS*/

			if (typ >= WHILE && typ <= IF) {

				while ((c = line[i++]) != '(');
				sp = i;
				while ((c = line[i]) != ')') i++;
				ep = i-1;

				retstat = new_Stat(getexptree(line, sp, ep, vartable), typ);

				while (isempty(line[++i]));

				if (istoken(line[i]))
					retstat -> stats._para._jump = getstat(line+i, vartable, fp);
				else
					retstat -> stats._para._jump = parseblock(fp, line, vartable, NULL);

				return retstat;
			}

			else if (typ == ELSE) {

				retstat = new_Stat(NULL, ELSE);

				while (isempty(line[++i]));

				if (istoken(line[i])) retstat -> stats._para._jump = getstat(line+i, vartable, fp);
				else
					retstat -> stats._para._jump = parseblock(fp, line, vartable, NULL);

				return retstat;
			}

			/*!CONDITIONAL STATEMENTS*/

			else if (typ >= INTDEC && typ <= VOIDDEC) {

				char *name;	
				size_t cc = 0, ref;
				hinfo *inf;
				struct stat *jumpstat;

				while (isempty((c = line[i]))) i++;
				if (line[i] == '*') {
					printf("POINTER ");
					while (isempty((c = line[++i])));
				} 

				while(isname(line[i+(cc++)]));
				name = (char*)malloc(sizeof(char)*(ref=cc));
				name[--cc] = 0;
				while (cc--) name[cc] = line[i+cc];
				i+=ref-1;


				/*FUNCTION DECLARATION*/

				if (line[i] && line[i] == '(') {

					size_t argc = 0, j = i+1, cc, ref, k = 0;
					hinfo **args;
					enum StatTyp ptyp;
					char *pname;

					HashTable *paramtable = new_HashTable(TABLE_SIZE);


					while (line[j] != ')')
						if (line[++j] == ',' || line[j] == ')')
							argc++;

					args = (hinfo**)malloc(sizeof(hinfo*)*argc);

					j = i+1;

					printf("FUNC \"%s\" with %d PARAMS: ", name, (int)argc);

					while (line[j] != ')')
						if (line[j] != ',' && line[j] != ')' && !isempty(line[j])) {

							ptyp = getToken(line, stattrie, &j);
							
							if (ptyp >= INTDEC && ptyp <= BOLDEC) {
								while(isempty(line[j])) j++;

								cc = 0;

								while(isname(line[j+(cc++)]));
								pname = (char*)malloc(sizeof(char)*(ref=cc));
								pname[--cc] = 0;
								while (cc--) pname[cc] = line[j+cc];
								j+=ref-1;

								printf("[%s] ", pname);

								inf = new_varinf(-1, ptyp);
								args[k++] = inf;

								if (pushTable(paramtable, pname, inf, 0))
									printf("NAME IN USE!");
							}

							else;
						} else j++;

					printf("\n");

					i = j+1;
					jumpstat = parseblock(fp, line, vartable, paramtable);
					inf = new_funcinf(jumpstat, argc, args, typ);

					printf("->\n{{{\n\n");
					printStatTree(jumpstat, 5);
					printf("\n\n}}}");

					if (pushTable(vartable, name, inf, 0))
						printf("NAME IN USE!");

					deleteTable(paramtable);
				}

				/*!FUNCTION DECLARATION*/


				else {
					printf("VAR %s", name);

					inf = new_varinf(-1, typ);
					if (pushTable(vartable, name, inf, 0))
						printf("NAME IN USE!");
				}

				printf("\n");

				return NULL;
			}



			else if (typ == NAN) {
				
				struct exp *temp = getexptree(line, 0, -1, vartable);
				/*process expressions*/

				if (temp)
					return new_Stat(temp, EXP);

				else;


				/*!process expressions*/
			}
		}

		else
			i++;
	}

	return NULL;
}



struct stat *parseblock(FILE *fp, char *holder, HashTable *prevtable, HashTable *extra) {

	struct stat *root = NULL;
	struct stat *prev = root;

	char c = fgetc(fp);
	int k = 0;
	struct stat *retstat;

	/*create vartable*/
	HashTable *vartable = new_HashTable(TABLE_SIZE);

	/*import to vartable*/

	if (prevtable) {
		int i;

		for (i = 0;i < prevtable -> capacity;i++)
			if (prevtable -> arr[i])
				pushTable(vartable, prevtable -> arr[i] -> skey,
			 	prevtable -> arr[i] -> inf, 1);
	}

	if (extra) {
		int i;

		for (i = 0;i < extra -> capacity;i++)
			if (extra -> arr[i])
				pushTable(vartable, extra -> arr[i] -> skey,
			 	extra -> arr[i] -> inf, 0);
	}
	/*!import to vartable*/


	while (c != EOF && c != '}') {

		if (c == ';' || c == '{') { 
			holder[k] = 0;

			if ((retstat = getstat(holder, vartable, fp))) {
				if (root)
					prev -> next = retstat;
				else
					root = retstat;

				prev = retstat;
			}

			k = 0;
		}

		else
			holder[k++] = c;

		c = fgetc(fp);
	}

	delete_HashTable(vartable);

	return root;
}














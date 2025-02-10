
#include "comp.h"


bool isempty(char c) {
	return c == ' ' || c == '\n' || c == '\t';
}

bool isnum(char c) {
	return (c > 47 && c < 58);
}
bool isname(char c) {
	return ((c > 64 && c < 91) || (c > 47 && c < 58) || (c > 96 && c < 123) || c == 95);
}
bool isbinary(char c) {
	return !(c == '.' || c == '~' || c == '!');
}
bool istoken(char c) {
	return (c > 96 && c < 123);
}

long int strtoint(const char *str, int s, int e) {

	long int retval = 0;

	while (s <= e)
		retval = retval*10 + str[s++] - 48;

	return retval;
}
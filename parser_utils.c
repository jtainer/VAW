// 
// Various functions for parsing strings
// I figure these will probably be used more than once so they get their own source file
//
// 2023, Jonathan Tainer
//

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser_utils.h"

int StringIsEmpty(const char* str) {
	if (str == NULL) return 0;
	else return *str == 0;
}

int StringIsWhitespace(const char* str) {
	if (str == NULL) return 0;
	while (*str != 0) {
		str;
		if (isspace(*str)) {
			str++;
			continue;
		}
		else return 0;
	}
	return 1;
}

int StringStartsWith(const char* str, char flag) {
	if (str == NULL) return 0;
	while (*str != 0) {
		str++;
		if (isspace(*str)) {
			str++;
			continue;
		}
		else if (*str == flag) return 1;
		else return 0;
	}
	return 0;
}

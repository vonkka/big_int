#include <iostream>
#include <cstring>
#include "help_functions.h"

char* get_str_input(int* len) {
	char* str = (char*)malloc(64);
	char symb;
	while ((symb = getchar()) != '\n') {
		if (!((*len) & 63)) {
			str = (char*)realloc(str, (*len) + 64);
		}
		str[(*len)] = symb;
		++(*len);
	}
	str = (char*)realloc(str, (*len) + 1);
	str[(*len)] = '\n';
	++(*len);
	if (str) return str;
	return NULL;
}

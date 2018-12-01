#include <string.h>

#include "stringUtil.h"

char UPPER_CASE_ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char LOWER_CASE_ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";

void trimTrailingWhiteSpace(char *str) {
	// avoid reading past the beginning of the string if it is empty
	if (strlen(str) == 0) {
		return;
	}

	// start at the last index
	char *cursor = str + strlen(str) - 1;

	// replace white space with '\0'
	while (*cursor == ' ' || *cursor == '\t' || *cursor == '\n' || *cursor == '\r') {
		*(cursor--) = '\0';
	}
}

char upperCaseLetter(int i) {
	return UPPER_CASE_ALPHABET[i % 26];
}

char lowerCaseLetter(int i) {
	return LOWER_CASE_ALPHABET[i % 26];
}

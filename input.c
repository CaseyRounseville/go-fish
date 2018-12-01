#include <stdio.h>

#include "input.h"

void clearInputBuffer(void) {
	char c;
	do {
		scanf("%c", &c);
	} while (c != '\n');
}

void waitForUserToPressEnter(char prompt[]) {
	printf("%s", prompt);
	clearInputBuffer();
}

int scanYesOrNo(char prompt[]) {
	printf("%s", prompt);
	char c;
	scanf("%c", &c);
	clearInputBuffer();
	while (c != 'y' && c != 'n') {
		printf("Sorry, I didn't understand that.\n");
		printf("\n");
		printf("%s", prompt);
		scanf("%c", &c);
		clearInputBuffer();
	}
	return c == 'y';
}

#include <stdio.h>

#include "input.h"

void waitForUserToPressEnter(void) {
	char c;
	do {
		scanf("%c", &c);
	} while (c != '\n');
}

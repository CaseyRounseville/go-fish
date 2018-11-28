#include <stdio.h>
#include <stdlib.h>

#include "rendering.h"
#include "cards.h"

#define CARD_WIDTH 9
#define CARD_HEIGHT 7
#define CARDS_PER_ROW 4
#define DISPLAY_WIDTH CARD_WIDTH * CARDS_PER_ROW

char values[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'J', 'Q', 'K'};


void renderCard(char display[][DISPLAY_WIDTH], card *c, int startRow, int startCol);
void blitDisplay(char display[][DISPLAY_WIDTH], int displayHeight);

void renderCard(char display[][DISPLAY_WIDTH], card *c, int startRow, int startCol) {
	// render four corners
	display[startRow][startCol] = '.';
	display[startRow][startCol + CARD_WIDTH - 1] = '.';
	display[startRow + CARD_HEIGHT - 1][startCol] = '\'';
	display[startRow + CARD_HEIGHT - 1][startCol + CARD_WIDTH - 1] = '\'';

	// render horizontal borders
	for (int col = startCol + 1; col < startCol + CARD_WIDTH - 1; col++) {
		display[startRow][col] = '-';
		display[startRow + CARD_HEIGHT - 1][col] = '-';
	}

	// render vertical borders
	for (int row = startRow + 1; row < startRow + CARD_HEIGHT - 1; row++) {
		display[row][startCol] = '|';
		display[row][startCol + CARD_WIDTH - 1] = '|';
	}

	// render contents
	if (c->value == 10) {
		display[startRow + 1][startCol + CARD_WIDTH - 3] = '1';
		display[startRow + 1][startCol + CARD_WIDTH - 2] = '0';
		display[startRow + 2][startCol + CARD_WIDTH - 2] = c->suit[0] - ('a' - 'A');
		display[startRow + CARD_HEIGHT - 3][startCol + 1] = '1';
		display[startRow + CARD_HEIGHT - 3][startCol + 2] = '0';
		display[startRow + CARD_HEIGHT - 2][startCol + 1] = c->suit[0] - ('a' - 'A');
	} else {
		display[startRow + 1][startCol + CARD_WIDTH - 2] = values[c->value - 1];
		display[startRow + 2][startCol + CARD_WIDTH - 2] = c->suit[0] - ('a' - 'A');
		display[startRow + CARD_HEIGHT - 3][startCol + 1] = values[c->value - 1];
		display[startRow + CARD_HEIGHT - 2][startCol + 1] = c->suit[0] - ('a' - 'A');
	}
}

void blitDisplay(char display[][DISPLAY_WIDTH], int displayHeight) {
	// blit the display to the console, char by char
	for (int row = 0; row < displayHeight; row++) {
		for (int col = 0; col < DISPLAY_WIDTH; col++) {
			printf("%c", display[row][col]);
		}
		printf("\n");
	}
}

void printDeckFancy(card *head) {
	// calculate the dimensions of the display
	int numCards = deckSize(head);
	int numRows = numCards / CARDS_PER_ROW + (numCards % CARDS_PER_ROW > 0);
	int displayHeight = CARD_HEIGHT * numRows;

	// create and zero the display
	char display[displayHeight][DISPLAY_WIDTH];
	for (int row = 0; row < displayHeight; row++) {
		for (int col = 0; col < DISPLAY_WIDTH; col++) {
			display[row][col] = ' ';
		}
	}

	// render each card to the display
	card *currCard = head;
	int row = 0;
	int col = 0;
	while (currCard) {
		renderCard(display, currCard, row, col);
		col += CARD_WIDTH;
		if (col > DISPLAY_WIDTH - 1) {
			col = 0;
			row += CARD_HEIGHT;
		}
		currCard = currCard->next;
	}

	// blit the display to the console
	blitDisplay(display, displayHeight);
}

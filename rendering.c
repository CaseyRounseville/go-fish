#include <stdio.h>
#include <stdlib.h>

#include "rendering.h"
#include "cards.h"
// card size and display size constants
#define CARD_WIDTH 9
#define CARD_HEIGHT 7
#define CARDS_PER_ROW 7
#define DISPLAY_WIDTH CARD_WIDTH * CARDS_PER_ROW

// the values to print on the cards
// 10 is a special case because it will take two chars
char values[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'J', 'Q', 'K'};

// render a card to a display
void renderCard(char display[][DISPLAY_WIDTH], card *c, int startRow, int startCol);

// print a display to the console
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
	// 10 is a special case because it will take two chars
	// for symbols, this just prints the capitalized first letter of the card's suit
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

void clearScreen(void) {
	int success;

	// windows command for clear screen is "cls"
	// unix/linux/mac command for clear screen is "clear"
	// try "cls" first, redirecting stderr to trash.txt, so
	// that an error message will not be displayed if we are
	// on unix/linux/mac
	// we can't use /dev/null and nul because if we are on
	// the wrong platform, it will produce an error message
    success = system("cls 2> trash.txt") == 0;
    
//    printf("Exit code in %d",system("cls"));
	// if "cls" failed, we are probably not on windows, so
	// try "clear"
	if (!success) {
        for (int i = 0; i < 200; i++) {
            printf("\n");
        }
	}

	// if "clear" failed, we might not be in an actual terminal,
	// so fallback to printing a bunch of line feeds
	
}

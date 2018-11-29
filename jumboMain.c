//
//  main.c
//  Fish
//
//  Created by nasser alhouti on 11/18/18.
//  Copyright © 2018 nasser alhouti. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
void trimTrailingWhiteSpace(char *str);
//
//  cards.h
//  Fish
//
//  Created by nasser alhouti on 11/18/18.
//  Copyright © 2018 nasser alhouti. All rights reserved.
// PROTOTYPES GO HERE !!!!!!!!!
//
typedef struct card card;
struct card{
    char suit[10];
    int value;
    struct card_s*pt;
    card *next;
    card *prev;
};
card *newcard(char suit[7] , int value);
card *newdeck(void);
card *removeCard(card *firstUnshuffledCard , card *newHead);
card *getLastCard(card *head);
card *laoddeck(FILE *filename); // loads a predefined deck
card *getCard(card *head,int index);
card *shuffleDeck(card *head);
int deckSize(card *head); // head is a pointer to the first card
void freeCard(card *c);
void printCard(card *c);
void printDeck(card *c);
void freeDeck(card *head); // points to the first card and recursively frees all the cards in the deck
void appendCard(card *head, card*c);
card *predefinedCards(void);
void appendCard(card *head , card *c); // points to a card that was alraedy created
// return a the value of the pointer
void shuffle(card *firstCard);
void getCardPosition(card *currentCard , int position);
// change the address of the first card using srand
// return pointer to the first card
//
//  player.h
//  Fish
//
//  Created by nasser alhouti on 11/18/18.
//  Copyright © 2018 nasser alhouti. All rights reserved.
//
typedef struct{
    card *firstCard;
}player;


//
//  game.h
//  Fish
//
//  Created by nasser alhouti on 11/18/18.
//  Copyright © 2018 nasser alhouti. All rights reserved.
//
typedef struct {
    player *player;
    int numberOfPlayers;
    player *currentPlayer;
    card *deckOfCards;
}game;

void computerMove( game *g);
void playerTurn(void);
void deckOfCards(void);
int checkIfWinner(game *g);




void printDeckFancy(card *c);

void clearScreen(void);

void waitForUserToPressEnter(void);
void greetings(void);
int main(int argc, const char * argv[]) {
	// seed the random number generator
	srand(time(NULL));

    char userAns;
    greetings();
    printf("Would you like to shuffle the cards (y/n)");
    scanf("%c%*c",&userAns);
    if (userAns == 'y') {
        card *deck = newdeck();
        deck = shuffleDeck(deck);
		printDeckFancy(deck);
    }else{
        card *deck = predefinedCards();
		printDeckFancy(deck);
    }
    printf("Wilma has\n");
    char suits[100]=  "🖤";
    char *print = suits;
    
    for (int i = 0; i < 1; i++) {
        printf("%s",print);
    }

	printf("Press Enter to clear the screen...\n");
	waitForUserToPressEnter();
	clearScreen();

    return 0;
}
void greetings(){
    FILE *greetingfile;
    greetingfile = fopen("go-fish-logo.txt", "r");
    char dashes;
    while(!feof(greetingfile)){
        fscanf(greetingfile, "%c",&dashes);
        printf("%c",dashes);
    }
}
//
//  cards.c
//  Fish
//
//  Created by nasser alhouti on 11/18/18.
//  Copyright © 2018 nasser alhouti. All rights reserved.
//

card *predefinedCards(void){
    FILE *cardDeckFile;
    int cardNum;
    char suit[9];
    cardDeckFile = fopen("cards.txt", "r");

	// read the first card
    fscanf(cardDeckFile, "%d %s",&cardNum, suit);
	trimTrailingWhiteSpace(suit);
	card *head = newcard(suit, cardNum);
	
	card *currCard = head;

	for (int i = 0; i < 51; i++) {
        fscanf(cardDeckFile, "%d %s",&cardNum, suit);
		trimTrailingWhiteSpace(suit);
        printf("%d %s",cardNum , suit);

		currCard->next = newcard(suit, cardNum);
		currCard = currCard->next;

        printf("\n");
    }

	// close the file
	fclose(cardDeckFile);

	// return a pointer to the first card in the deck
	return head;
}
card *newdeck(void){
    card *cardList;
    // First card
    cardList = newcard("hearts",1);
    
    char suits[][10]= {"hearts","diamonds","spades","clubs"};
    // Append card functions
    appendCard(cardList, newcard("diamonds",1));
     appendCard(cardList, newcard("spades",1));
     appendCard(cardList, newcard("clubs",1));
    for (int i = 2; i < 14; i++) {
        for (int j = 0; j < 4; j++) {
            appendCard(cardList, newcard(suits[j], i));
        }
    }
    return cardList;
}
card *getCard(card *head,int index){
    for (int i = 0; i < index; i++) {
        head = head->next;
    }
    return head;
}
card *removeCard(card *head, card *cardToBeRemoved){
    if (head == cardToBeRemoved) {
        return head->next;
    }
    card *currentCard = head;
    card *previousCard = currentCard;
    while(currentCard != cardToBeRemoved){
        previousCard = currentCard;
        currentCard = currentCard->next;
    }
    previousCard->next = currentCard->next;
    currentCard->next = NULL;
    return head;
}
card *shuffleDeck(card *firstUnshuffledCard){
    card *newHead = getCard(firstUnshuffledCard,rand()%52);// second argument is the random index
    firstUnshuffledCard = removeCard(firstUnshuffledCard , newHead);
    int numCardsRemovedSoFar = 1;
    while(firstUnshuffledCard != NULL){
        int index = rand()%(52-numCardsRemovedSoFar); // removes one card each time
        card *cardToRemove = getCard(firstUnshuffledCard, index);
        firstUnshuffledCard = removeCard(firstUnshuffledCard, cardToRemove);
        numCardsRemovedSoFar++;
        appendCard(newHead, cardToRemove);
        
    }
    printDeck(newHead);
    return newHead;
}

void printCard(card *c){
    printf("%d ",c->value);
    printf("%s",c->suit);
    
}
void printDeck(card *c){
    for (int i = 0; i < 52; i++) {
        printCard(c);
        c = c->next;
        printf("\n");
    }
}
// creats a card
card *newcard(char *suit, int value){
    card *c = (card*)malloc(sizeof(card));
    strcpy(c->suit,suit);
    c->value = value;
    c->next = NULL;
    return c;
}
// pointer to the last card that was created (previous)
card *getLastCard(card *head){
    // head is the first card curr is the card
    card *curr = head;
    while(curr-> next != NULL){
        curr = curr->next;
    }
    return curr;
}
// append card
void appendCard(card *head , card *c){
    // tail is the previous card
    card *tail = getLastCard(head);
    tail -> next = c;
    c -> prev = tail;
    c->next = NULL;
}
void freeDeck(card *c){
    if (c == NULL){
        return;
    }
    freeDeck(c->next);
    free(c);
}

int deckSize(card *head) {
	int count = 0;
	while (head) {
		count++;
		head = head->next;
	}
	return count;
}
// card size and display size constants
#define CARD_WIDTH 9
#define CARD_HEIGHT 7
#define CARDS_PER_ROW 13
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
	success = system("cls 2> trash.txt") != -1;

	// if "cls" failed, we are probably not on windows, so
	// try "clear"
	if (!success) {
		success = system("clear 2> trash.txt");
	}

	// if "clear" failed, we might not be in an actual terminal,
	// so fallback to printing a bunch of line feeds
	if (!success) {
		for (int i = 0; i < 200; i++) {
			printf("\n");
		}
	}
}
void waitForUserToPressEnter(void) {
	char c;
	do {
		scanf("%c", &c);
	} while (c != '\n');
}
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

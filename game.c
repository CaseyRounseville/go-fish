//
//  game.c
//  Fish
//
//  Created by nasser alhouti on 11/18/18.
//  Copyright © 2018 nasser alhouti. All rights reserved.
//

#include <string.h>
#include <stdlib.h>

#include "game.h"
#include "player.h"
#include "cards.h"
#include "rendering.h"
#include "input.h"
#include "stringUtil.h"

char playerNumbers[][1] = {"1", "2", "3", "4", "5", "6", "7"};

// these functions are not exposed to the rest of the program
void dealCards(game *g);
void printHands(game *g);
player *getPlayerByName(game *g, char name[]);
int takeCardsFromOnePlayerAndGiveThemToAnother(player *giver, player *receiver, int query);

game *newGame(card *deckOfCards) {
    //
	game *g = (game *)malloc(sizeof(game));
	g->deckOfCards = deckOfCards;
	g->numBooksTotal = 0;
	return g;
}

void playGame(game *g, char result[]) {
	// deal the cards
	printf("Dealing cards...");
	dealCards(g);
	printf("done.\n");
	printf("\n");

	// print everyone's hands
	printHands(g);

	// pause
	waitForUserToPressEnter("Press Enter to continue");
	clearScreen();

	// game loop
	int currPlayerIndex = 0;
    printf("%s",result);
	while (strcmp(result, "") == 0) {
		// get a pointer to the current player
		player *currPlayer = g->player + currPlayerIndex;

		if (currPlayer->isHuman) {
			while (playerTurn(g, currPlayer));
		} else {
			while (computerMove(g, currPlayer));
		}

		// increment the current player index
		currPlayerIndex = (currPlayerIndex + 1) % g->numberOfPlayers;

		// it doesn't matter whose turn it is when the game ends,
		// so it is safe to check for a winner after the current
		// player has been incremented
		checkIfWinner(g, result);
	}
}
 // cards in the table
int computerMove(game *g, player *p) {
	printf("It is %s's(automated) turn.\n", p->name);

	// for debugging purposes only
	printf("FOR DEBUGGING PURPOSES ONLY:\n");
	if (p->firstCard) {
		printf("This is %s's hand:\n", p->name);
		printDeckFancy(p->firstCard);
	} else {
		printf("%s does not have any cards in their hand.\n", p->name);
	}
	printf("\n");

	// print the player's books, if they have any
	if (p->firstCardOfFirstBook) {
		printf("These are %s's books:\n", p->name);
		printBooksFancy(p->firstCardOfFirstBook);
	} else {
		printf("%s does not have any books.\n", p->name);
	}
	printf("\n");

	// if the player's hand is empty, they cannot make a turn
	if (!p->firstCard) {
		printf("Because %s does not have any cards in their hand, they cannot ask for any. So, their turn is over.\n", p->name);
		printf("\n");
		waitForUserToPressEnter("Press Enter to continue.\n");
		clearScreen();
		return 0;
	}

	// randomly choose a card value to ask for
	card *cardToGetValueFrom = getCard(p->firstCard, rand() % deckSize(p->firstCard));
	int query = cardToGetValueFrom->value;

	// randomly choose someone to ask(not self)
	player *otherPlayer;
	do {
		otherPlayer = g->player + rand() % g->numberOfPlayers;
	} while (otherPlayer == p);

	printf("%s has chosen to ask for %d from %s.\n", p->name, query, otherPlayer->name);

	// take the cards from the other player, if they have the
	// cards that the automated player is looking for
	int numCardsTaken = takeCardsFromOnePlayerAndGiveThemToAnother(otherPlayer, p, query);

	if (numCardsTaken > 0) {
		// print how many cards were taken
		printf("%s had %d %d's.\n", otherPlayer->name, numCardsTaken, query);
		printf("\n");

		// determine if the player got a book on this turn
		int numBooksBeforeMove = deckSize(p->firstCardOfFirstBook) / 4;
		moveCardsFromHandToBook(p);
		int numBooksAfterMove = deckSize(p->firstCardOfFirstBook) / 4;
		if (numBooksAfterMove > numBooksBeforeMove) {
			// increment the total number of books
			g->numBooksTotal++;

			printf("%s just got a book.\n", p->name);
			printf("\n");
			printf("Here is the updated list of %s's books:\n", p->name);
			printf("\n");
			printBooksFancy(p->firstCardOfFirstBook);
		} else {
			printf("There is no change in %s's books.\n", p->name);
		}
		printf("\n");

		printf("FOR DEBUGGING PURPOSES ONLY:\n");
		if (p->firstCard) {
			printf("This is %s's new hand:\n", p->name);
			printDeckFancy(p->firstCard);
		} else {
			printf("%s's hand is now empty.\n", p->name);
		}
	} else {
		printf("%s did not have any %d's.\n", otherPlayer->name, query);
		printf("\n");
		printf("%s will go fish(take a card from the table).\n", p->name);
		printf("\n");

		// pick up a card form the table
		card *cardToPickUp = getCard(g->deckOfCards, 0);
		if (cardToPickUp) {
			g->deckOfCards = removeCard(g->deckOfCards, cardToPickUp);
			cardToPickUp->prev = NULL;
			cardToPickUp->next = NULL;

			// we don't have to worry about the player's hand being empty here
			// because if it were empty, they would not have been able to ask for
			// a card, not get what they asked for, and have to draw a card from
			// the table
			appendCard(p->firstCard, cardToPickUp);

			// check if the card the player picked up was the one they asked for
			// if they picked up the one they asked for, they get to go again
			if (cardToPickUp->value == query) {
				printf("It is %s's lucky day. They picked up a %d from the table.\n", p->name, query);
				printf("\n");
				numCardsTaken = 1;
			}

			// determine if picking up a card caused the player to get a book
			int numBooksBeforeMove = deckSize(p->firstCardOfFirstBook) / 4;
			moveCardsFromHandToBook(p);
			int numBooksAfterMove = deckSize(p->firstCardOfFirstBook) / 4;
			if (numBooksAfterMove > numBooksBeforeMove) {	
				// increment total number of books
				g->numBooksTotal++;

				printf("%s just got a book.\n", p->name);
				printf("\n");
				printf("Here is the updated list of %s's books:\n", p->name);
				printf("\n");
				printBooksFancy(p->firstCardOfFirstBook);
			} else {
				printf("There is no change in %s's books.\n", p->name);	
			}
			printf("\n");

			if (p->firstCard) {
				printf("This is %s's new hand:\n", p->name);
				printDeckFancy(p->firstCard);
			} else {
				printf("%s's hand is now empty.\n", p->name);
			}
			printf("\n");

			if (g->deckOfCards) {
				printf("This is the deck, after picking up a card:\n");
				printDeckFancy(g->deckOfCards);
			} else {
				printf("%s just picked up the last card from the table,\n", p->name);
				printf("so now there are no more cards on the table.\n");
			}
		} else {
			printf("Uh-oh, it appears there aren't any cards on the table.\n");
			printf("%s's hand did not change.\n", p->name);
		}
	}
	printf("\n");

	// get out if the game is over
	if (g->numBooksTotal == 13) {
		return numCardsTaken;
	}

	if (numCardsTaken > 0) {
		printf("Because %s received the card they asked for, they will go again.\n", p->name);
	} else {
		printf("Because %s did not recieve the card they asked for, their turn is over.\n", p->name);
	}
	printf("\n");

	waitForUserToPressEnter("Press Enter to continue.\n");
	clearScreen();
	return numCardsTaken;
}

int playerTurn(game *g, player *p) {
	printf("%s, it is your turn.\n", p->name);
	printf("\n");

	// print the player's cards, if they have any
	if (p->firstCard) {
		printf("This is your hand:\n");
        // Ascii art 
		printDeckFancy(p->firstCard);
	} else {
		printf("You do not have any cards in your hand.\n");
	}
	printf("\n");

	// print the player's books, if they have any
	if (p->firstCardOfFirstBook) {
		printf("These are your books:\n");
		printBooksFancy(p->firstCardOfFirstBook);
	} else {
		printf("You do not have any books.\n");
	}
	printf("\n");

	// check if the player is able to make a turn,
	// and end their turn if they are not
	if (!p->firstCard) {
		printf("You don't have any cards in your hand, so you cannot ask for any. So, your turn is over.\n");
		printf("\n");

		// jump out of the function
		waitForUserToPressEnter("Press Enter to continue.\n");
		clearScreen();
		return 0;
	}

	// ask the player what card they want to ask for
	int query;
	printf("What card do you want to ask for?(1-13): ");
	int numberOfIntsScanned = scanf("%d", &query);
	clearInputBuffer();

	// make sure they entered it right, and that they actually have the card
	while (numberOfIntsScanned == 0 || (query < 1 || query > 13) || !findCardByValue(p->firstCard, query)) {
		if (numberOfIntsScanned == 0) {
			printf("Sorry, I didn't understand that.\n");
			printf("Please use only integers.\n");
		} else if (query < 1 || query > 13) {
			printf("Please use only integers 1 to 13.\n");
		} else {
			printf("You can only ask for cards that you have in your hand.\n");
		}
		printf("\n");

		printf("What card do you want to ask for?(1-13): ");
		numberOfIntsScanned = scanf("%d", &query);
		clearInputBuffer();
	}
	printf("\n");

	// display the names of the other players
	printf("Your playmates are:\n");
	for (player *playmate = g->player; playmate < g->player + g->numberOfPlayers; playmate++) {
		// skip self
		if (playmate == p) {
			continue;
		}

		printf("%-20s(%s)\n", playmate->name, playmate->isHuman ? "human" : "automated");
	}
	printf("\n");
	
	// ask the player who they want to ask
	char otherPlayerName[100];
	printf("Who do you want to ask?(name): ");
	fgets(otherPlayerName, 100, stdin);
	trimTrailingWhiteSpace(otherPlayerName);

	player *otherPlayer = getPlayerByName(g, otherPlayerName);

	while (!otherPlayer || otherPlayer == p) {
		if (!otherPlayer) {
			printf("I don't know who that is.\n");
		} else {
			printf("You can't take cards from yourself.\n");
		}
		printf("Please enter one of the names listed above.\n");
		printf("\n");

		printf("Who do you want to ask?(name): ");
		fgets(otherPlayerName, 100, stdin);
		trimTrailingWhiteSpace(otherPlayerName);
		otherPlayer = getPlayerByName(g, otherPlayerName);
	}

	// take the cards from the other player, if they have any
	int numCardsTaken = takeCardsFromOnePlayerAndGiveThemToAnother(otherPlayer, p, query);

	if (numCardsTaken > 0) {
		// print how many cards were taken
		printf("%s had %d %d's.\n", otherPlayer->name, numCardsTaken, query);
		printf("\n");

		// determine if the player got a book on this turn
		int numBooksBeforeMove = deckSize(p->firstCardOfFirstBook) / 4;
		moveCardsFromHandToBook(p);
		int numBooksAfterMove = deckSize(p->firstCardOfFirstBook) / 4;
		if (numBooksAfterMove > numBooksBeforeMove) {
			// increment the total number of books
			g->numBooksTotal++;

			printf("You just got a book.\n");
			printf("\n");
			printf("Here is the updated list of your books:\n");
			printf("\n");
			printBooksFancy(p->firstCardOfFirstBook);
		} else {
			printf("There is no change in your books.\n");
		}
		printf("\n");

		if (p->firstCard) {
			printf("This is your new hand:\n");
			printDeckFancy(p->firstCard);
		} else {
			printf("Your hand is now empty.\n");
		}
	} else {
		printf("%s did not have any %d's.\n", otherPlayer->name, query);
		printf("\n");
		printf("You will go fish(take a card from the table).\n");
		printf("\n");

		// pick up a card from the table
		card *cardToPickUp = getCard(g->deckOfCards, 0);
		if (cardToPickUp) {
			g->deckOfCards = removeCard(g->deckOfCards, cardToPickUp);
			cardToPickUp->prev = NULL;
			cardToPickUp->next = NULL;

			// we don't have to worry about the player's hand being empty here
			// because if it were empty, they would not have been able to ask for
			// a card, not get what they asked for, and have to draw a card from
			// the table
			appendCard(p->firstCard, cardToPickUp);

			// check if the card the player picked up was the one they asked for
			// if they picked up the one they asked for, they get to go again
			if (cardToPickUp->value == query) {
				printf("It is your lucky day. You picked up a %d from the table.\n", query);
				printf("\n");
				numCardsTaken = 1;
			}

			// determine if picking up a card caused the player to get a book
			int numBooksBeforeMove = deckSize(p->firstCardOfFirstBook) / 4;
			moveCardsFromHandToBook(p);
			int numBooksAfterMove = deckSize(p->firstCardOfFirstBook) / 4;
			if (numBooksAfterMove > numBooksBeforeMove) {
				// increment the total number of books
				g->numBooksTotal++;

				printf("You just got a book.\n");
				printf("\n");
				printf("Here is an updated list of your books:\n");
				printf("\n");
				printBooksFancy(p->firstCardOfFirstBook);
			} else {
				printf("There is no change in your books.\n");
			}
			printf("\n");

			if (p->firstCard) {
				printf("This is your new hand:\n");
				printDeckFancy(p->firstCard);
			} else {
				printf("Your hand is now empty.\n");
			}
			printf("\n");

			if (g->deckOfCards) {
				printf("This is the deck, after picking up a card:\n");
				printDeckFancy(g->deckOfCards);
			} else {
				printf("You just picked up the last card from the table,\n");
				printf("so now there are no more cards on the table.\n");
			}
		} else {
			printf("Uh-oh, it appears there aren't any cards on the table.\n");
			printf("Your hand did not change.\n");
		}
	}
	printf("\n");

	// get out if the game is over
	if (g->numBooksTotal == 13) {
		return numCardsTaken;
	}

	if (numCardsTaken > 0) {
		printf("Because you received the card you asked for, you will go again.\n");
	} else {
		printf("Because you did not get what you asked for, your turn is over.\n");
	}
	printf("\n");

	waitForUserToPressEnter("Press Enter to continue.\n");
	clearScreen();
	return numCardsTaken;
}

void deckOfCards(game *g) {

}

void checkIfWinner(game *g, char result[]) {
	// if the total number of books completed is less than 13,
	// the game is not over yet
	if (g->numBooksTotal < 13) {
		strcpy(result, "");
		return;
	}

	// the total number of books is 13, so the game has ended,
	// so find the players with the most books
	strcpy(result, "");
	int maxBooks = 0;
	
	for (player *p = g->player; p < g->player + g->numberOfPlayers; p++) {
		int numBooks = deckSize(p->firstCardOfFirstBook) / 4;
		if (numBooks > maxBooks) {
			maxBooks = numBooks;
		}
	}

	// players will be represented as 0-9 in the result string because
	// the number 10 has two digits, and it is easier to have te players
	// be one digit each
	int playerIndex = 0;
	for (player *p = g->player; p < g->player + g->numberOfPlayers; p++) {
		int numBooks = deckSize(p->firstCardOfFirstBook) / 4;
		if (numBooks == maxBooks) {
			char temp[10] = "";
			sprintf(temp, "%c", '0' + playerIndex);
			strcat(result, temp);
		}
	}
}

void dealCards(game *g) {
	// deal each player 7 cards
	for (player *p = g->player; p < g->player + g->numberOfPlayers; p++) {
        
        for (int i = 0; i < (g->numberOfPlayers >3 ? 5 : 7); i++) {
			// get a pointer to the first card in the deck(the head)
			card *c = g->deckOfCards;
            
			// remove the card from the deck
			// head must be reassigned, because it won't be the head anymore
			// after it is removed
			//
			// IMPORTANT NOTE:
			//     the return value of the removeCard function is NOT the card that
			//     was removed
			//     instead, it is the new head of the deck
			//     you already have a pointer to the removed card
			g->deckOfCards = removeCard(g->deckOfCards, c);

			// IMPORTANT NOTE:
			//     the removeCard function does not NULL card->prev and card->next
			//     this is done here
			c->prev = NULL;
			c->next = NULL;

			// add the card to the player's hand
			// IMPORTANT NOTE:
			//     the appendCard function does not check if the player's hand is empty
			//     this is done here
			if (p->firstCard) {
				appendCard(p->firstCard, c);
			} else {
				p->firstCard = c;	
			}
		}

		// if the player was dealt a book, move the cards now
		// we don't have to worry about the player running out
		// of cards because of being dealt all books because
		// the player is dealt either 7 or 5 cards, neither of
		// which is divisble by 4, meaning that even if the player
		// was dealt a book, and those cards were removed from
		// their hand, they would still have cards left in their
		// hand
		moveCardsFromHandToBook(p);
	}
}

void printHands(game *g) {
	for (int i = 0; i < g->numberOfPlayers; i++) {
		player *p = g->player + i;
		printf("Player %d(%s), this is your hand:\n", i + 1, p->name);
		printDeckFancy(p->firstCard);
		printf("\n");
	}
}

player *getPlayerByName(game *g, char name[]) {
	for (player *p = g->player; p < g->player + g->numberOfPlayers; p++) {
		if (strcmp(name, p->name) == 0) {
			return p;
		}
	}

	return NULL;
}

int takeCardsFromOnePlayerAndGiveThemToAnother(player *giver, player *receiver, int query) {
	int numCardsTaken = 0;
	card *cardToTake;
	while ((cardToTake = findCardByValue(giver->firstCard, query)) != NULL) {
		// remove the card from the other player's hand
		// remember to reassign the head when removing a card
		giver->firstCard = removeCard(giver->firstCard, cardToTake);

		// I'm not sure if this is necessary, but it doesn't hurt anything
		cardToTake->prev = NULL;
		cardToTake->next = NULL;

		// put the card in the current player's hand
		appendCard(receiver->firstCard, cardToTake);

		// increment the number of cards taken
		numCardsTaken++;
	}
	return numCardsTaken;
}

void endOfGameResults(game *g, char result[]) {	
	clearScreen();
	printf("\aDING \aDING \aDING!\n");
	printf("\n");
	printf("And the game is overrrrrrr!\n");
	printf("\n");
	printf("Let's see who won.\n");
	printf("\n");
	waitForUserToPressEnter("Press enter to continue.\n");
	clearScreen();

	printf("And the winner is...\n");
	printf("\n");
	waitForUserToPressEnter("Press enter to continue.\n");
	clearScreen();

	if (strlen(result) > 1) {
		printf("Oh...\n");
		printf("\n");
		waitForUserToPressEnter("Press enter to continue.\n");
		clearScreen();

		printf("It appears there is a %d-way tie.\n", (int)strlen(result));
		printf("\n");
		waitForUserToPressEnter("Press enter to continue.\n");
		clearScreen();

		printf("And the winners are...\n");
		printf("\n");
		waitForUserToPressEnter("Press enter to continue.\n");

		for (int i = 0; i < strlen(result); i++) {
			printf("Player %d:%-20s(%s)\n", result[i] + 1 - '0', g->player[i].name, g->player[i].isHuman ? "human" : "automated");
		}
	} else {
		printf("Player %d:%-20s(%s)\n", result[0] + 1 - '0', g->player[result[0] - '0'].name, g->player[result[0] - '0'].isHuman ? "human" : "automated");
	}
	printf("\n");

	// print out everyone's books to make sure the result is correct
	printf("Final scores:\n");
	printf("\n");
	for (int i = 0; i < g->numberOfPlayers; i++) {
		printf("Player %d:%-20s(%s)\n", i + 1, g->player[i].name, g->player[i].isHuman ? "human" : "automated");
		printBooksFancy(g->player[i].firstCardOfFirstBook);
		printf("\n");
	}
	waitForUserToPressEnter("Press enter to continue.\n");
}

void credits() {
	printf("Thank you for playing.\n");
	printf("\n");

	printf("Credits:\n");
	printf("\n");

	printf("Nasser Alhouti\n");
	printf("Casey Rounseville\n");
	printf("\n");

	waitForUserToPressEnter("Press enter to exit program.\n");
}

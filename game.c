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

game *newGame(card *deckOfCards) {
	game *g = (game *)malloc(sizeof(game));
	g->deckOfCards = deckOfCards;
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
		printDeckFancy(p->firstCardOfFirstBook);
	} else {
		printf("%s does not have any books.\n", p->name);
	}
	printf("\n");

	// randomly choose a card to ask for
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
	int numCardsTaken = 0;
	card *cardToTake;
	while ((cardToTake = findCardByValue(otherPlayer->firstCard, query)) != NULL) {
		// remove the card from the other player's hand
		// remember to reassign the head when removing a card
		otherPlayer->firstCard = removeCard(otherPlayer->firstCard, cardToTake);

		// I'm not sure if this is necessary, but it doesn't hurt anything
		cardToTake->prev = NULL;
		cardToTake->next = NULL;

		// put the card in the current player's hand
		appendCard(p->firstCard, cardToTake);

		// increment the number of cards taken
		numCardsTaken++;
	}

	if (numCardsTaken > 0) {
		printf("%s had %d %d's.\n", otherPlayer->name, numCardsTaken, query);
		printf("\n");
		printf("FOR DEBUGGING PURPOSES ONLY:\n");
		printf("This is %s's new hand:\n", p->name);
		printDeckFancy(p->firstCard);
		printf("\n");
		printf("Because %s received the card they asked for, they will go again.\n", p->name);
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
			appendCard(p->firstCard, cardToPickUp);
			printf("This is %s's new hand:\n", p->name);
			printDeckFancy(p->firstCard);
			printf("\n");
			printf("This is the deck, after picking up a card:\n");
			printDeckFancy(g->deckOfCards);
		} else {
			printf("Uh-oh, it appears there aren't any cards on the table.\n");
			printf("%s's hand did not change.\n", p->name);
		}
		printf("\n");

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
		printDeckFancy(p->firstCard);
	} else {
		printf("You do not have any cards in your hand.\n");
	}
	printf("\n");

	// print the player's books, if they have any
	if (p->firstCardOfFirstBook) {
		printf("These are your books:\n");
		printDeckFancy(p->firstCardOfFirstBook);
	} else {
		printf("You do not have any books.\n");
	}
	printf("\n");

	// check if the player is able to make a turn,
	// and end their turn if they are not
	if (!p->firstCard) {
		printf("FIXME\n");
		printf("You don't have any cards in your hand.\n");
		printf("There are no more cards on the table,\n");
		printf("so you can't pick any up. ask for any.\n");
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

	// make sure they entered it right
	while (numberOfIntsScanned == 0 || (query < 1 || query > 13)) {
		if (numberOfIntsScanned == 0) {
			printf("Sorry, I didn't understand that.\n");
			printf("Please use only integers.\n");
		} else {
			printf("Please use only integers 1 to 13.\n");
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

	waitForUserToPressEnter("Press Enter to continue.\n");
	clearScreen();
	return 0;
}

void deckOfCards(game *g) {

}

void checkIfWinner(game *g, char result[]) {
	// if there are still cards on the table, the game is not over yet
	if (deckSize(g->deckOfCards) > 0) {
		strcpy(result, "");
		return;
	}

	// if players still have cards in their hand, the game is not over yet
	for (player *p = g->player; p < g->player + g->numberOfPlayers; p++) {
		if (deckSize(p->firstCard) > 0) {
			strcpy(result, "");
			return;
		}
	}

	// there are no cards on the table, and the players' hands are empty
	// the game has ended, so find the players with the most books
	// TODO: find the players with the most books
	strcpy(result, "");
	int maxBooks = 0;
	
	for (player *p = g->player; p < g->player + g->numberOfPlayers; p++) {
		int numBooks = deckSize(p->firstCardOfFirstBook) / 4;
		if (numBooks > maxBooks) {
			maxBooks = numBooks;
		}
	}

	int playerNum = 0;
	for (player *p = g->player; p < g->player + g->numberOfPlayers; p++) {
		int numBooks = deckSize(p->firstCardOfFirstBook) / 4;
		if (numBooks == maxBooks) {
			strcat(result, playerNumbers[playerNum]);
		}
	}
}

void dealCards(game *g) {
	// deal each player 7 cards
	for (player *p = g->player; p < g->player + g->numberOfPlayers; p++) {
		for (int i = 0; i < 7; i++) {
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

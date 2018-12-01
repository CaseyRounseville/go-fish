//
//  player.c
//  Fish
//
//  Created by nasser alhouti on 11/18/18.
//  Copyright © 2018 nasser alhouti. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "player.h"
#include "game.h"
#include "input.h"
#include "stringUtil.h"

int nameIsTaken(player *firstPlayer, int numPlayers, char name[]);

void setUpPlayers(game *g) {
	// figure out how many players
	printf("How many players this time?: ");
	int numIntsScanned = scanf("%d", &g->numberOfPlayers);
	clearInputBuffer();

	while (numIntsScanned == 0 || (g->numberOfPlayers < 2 || g->numberOfPlayers > 7)) {
		if (numIntsScanned == 0) {
			printf("Sorry, I didn't understand that.\n");
		} else {
			if (g->numberOfPlayers < 2) {
				printf("There must be at least 2 players.\n");
			} else {
				printf("There are only 52 cards in the deck.\n");
			}
			printf("Please only use integers 2-7.\n");
			printf("\n");

			printf("How many players this time?: ");
			numIntsScanned = scanf("%d", &g->numberOfPlayers);
			clearInputBuffer();
		}
	}

	// allocate exactly enough memory for the players
	g->player = (player *)calloc(g->numberOfPlayers, sizeof(player));

	// gather info about each player
	int playerNum = 1;
	char name[100];
	for (player *p = g->player; p < g->player + g->numberOfPlayers; p++) {
		char playerPrompt[100];
		sprintf(playerPrompt, "Shall player %d be human?(y/n): ", playerNum);
		if (scanYesOrNo(playerPrompt)) {
			// the player is human
			p->isHuman = 1;

			// read the player's name
			printf("Player %d, what is your name?: ", playerNum);
			fgets(name, 100, stdin);
			trimTrailingWhiteSpace(name);

			// make sure the name is not taken
			while (nameIsTaken(g->player, g->numberOfPlayers, name)) {
				printf("Uhh... that's alreay taken. Enter a different name: ");
				fgets(name, 100, stdin);
				trimTrailingWhiteSpace(name);
			}
		} else {
			// the player is not human
			p->isHuman = 0;
			printf("Player %d will be automated.\n", playerNum);


			// the names of all automated players' start with "COM-"
			// the end of the name is a randomly generated sequence of 5 letters
			do {
				strcpy(name, "COM-");
				for (int i = 0; i < 5; i++) {
					name[4 + i] = upperCaseLetter(rand() % 26);
					name[4 + i + 1] = '\0';
				}
			} while (nameIsTaken(g->player, g->numberOfPlayers, name));

			printf("Player %d's name is %s.\n", playerNum, name);
		}

		// give the player the name
		strcpy(p->name, name);

		playerNum++;

		printf("\n");
	}

	// print player info
	printf("Player information:\n");
	playerNum = 1;
	for (player *p = g->player; p < g->player + g->numberOfPlayers; p++) {
		printf("Player %d: %-20s(%s)\n", playerNum, p->name, p->isHuman ? "human" : "automated");
		playerNum++;
	}
}

void freePlayers(game *g) {
	free(g->player);
}

int nameIsTaken(player *firstPlayer, int numPlayers, char name[]) {
	for (player *currentPlayer = firstPlayer; currentPlayer < firstPlayer + numPlayers; currentPlayer++) {
		if (strcmp(currentPlayer->name, name) == 0) {
			return 1;
		}
	}
	return 0;
}

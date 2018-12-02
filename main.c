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
#include "game.h"
#include "cards.h"
#include "player.h"
#include "rendering.h"
#include "input.h"
void greetings(void);
int main(int argc, const char * argv[]) {
	// seed the random number generator
	srand((int)time(NULL));

    char userAns;
    greetings();
    printf("Would you like to shuffle the cards (y/n)");
    scanf("%c%*c",&userAns);
	card *deck;
    if (userAns == 'y') {
        deck = newdeck();
        deck = shuffleDeck(deck);
		printDeckFancy(deck);
    }else{
        deck = predefinedCards();
		printDeckFancy(deck);
    }
    printf("Wilma has\n");
    char suits[100]=  "🖤";
    char *print = suits;
    
    for (int i = 0; i < 1; i++) {
        printf("%s",print);
    }

	// pause
	waitForUserToPressEnter("Press Enter to clear the screen...\n");
	clearScreen();

	// create a new game instance
	game *g = newGame(deck);

	// set up the players
	setUpPlayers(g);

	// pause
	waitForUserToPressEnter("Press Enter to clear the screen...\n");
	clearScreen();

	// start the game
	char result[100];
	playGame(g, result);

	// alert everyone that the game has just ended, and print the winners
	endOfGameResults(g, result);
	clearScreen();

	// display the credits
	credits();
	clearScreen();

	// free the memory used by the game, players, and cards
	freeGame(g);

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


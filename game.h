//
//  game.h
//  Fish
//
//  Created by nasser alhouti on 11/18/18.
//  Copyright © 2018 nasser alhouti. All rights reserved.
//
#ifndef game_h
#define game_h

#include "player.h"
#include "cards.h"
typedef struct game game;
struct game {
    player *player;
    int numberOfPlayers;
    player *currentPlayer;
    card *deckOfCards;
	int numBooksTotal;
};

game *newGame(card *deckOfCards);
void playGame(game *g, char result[]);
int computerMove(game *g, player *p);
int playerTurn(game *g, player *p);
void deckOfCards(game *g);
void checkIfWinner(game *g, char result[]);
void endOfGameResults(game *g, char result[]);
void credits(void);
void freeGame(game *g);
#endif /* game_h */

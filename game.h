//
//  game.h
//  Fish
//
//  Created by nasser alhouti on 11/18/18.
//  Copyright © 2018 nasser alhouti. All rights reserved.
//
#include "player.h"
#include "cards.h"
typedef struct {
    player *player;
    int numberOfPlayers;
    player *currentPlayer;
    card *deckOfCards;
}game;
#ifndef game_h
#define game_h

void computerMove( game *g);
void playerTurn(void);
void deckOfCards(void);
int checkIfWinner(game *g);
#endif /* game_h */

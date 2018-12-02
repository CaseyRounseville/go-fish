//
//  player.h
//  Fish
//
//  Created by nasser alhouti on 11/18/18.
//  Copyright © 2018 nasser alhouti. All rights reserved.
//
#ifndef player_h
#define player_h

#include "cards.h"

typedef struct player player;
struct player {
	char name[100];
	int isHuman;
    card *firstCard;
	card *firstCardOfFirstBook;
};

typedef struct game game;
void setUpPlayers(game *g);

void moveCardsFromHandToBook(player *player);
#endif /* player_h */

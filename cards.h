//
//  cards.h
//  Fish
//
//  Created by nasser alhouti on 11/18/18.
//  Copyright © 2018 nasser alhouti. All rights reserved.
// PROTOTYPES GO HERE !!!!!!!!!
//
#ifndef cards_h
#define cards_h
#include <stdio.h>
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
void predefinedCards(void);
void appendCard(card *head , card *c); // points to a card that was alraedy created
// return a the value of the pointer
void shuffle(card *firstCard);
void getCardPosition(card *currentCard , int position);
// change the address of the first card using srand
// return pointer to the first card
#endif /* cards_h */

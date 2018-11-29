//
//  cards.c
//  Fish
//
//  Created by nasser alhouti on 11/18/18.
//  Copyright © 2018 nasser alhouti. All rights reserved.
//

#include <stdio.h>
#include "cards.h"
#include "stringUtil.h"
#include <stdlib.h>
#include <string.h>
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

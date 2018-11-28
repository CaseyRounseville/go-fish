//
//  main.c
//  Fish
//
//  Created by nasser alhouti on 11/18/18.
//  Copyright © 2018 nasser alhouti. All rights reserved.
//

#include <stdio.h>
#include "game.h"
#include "cards.h"
#include "player.h"
void greetings(void);
int main(int argc, const char * argv[]) {
    char userAns;
    greetings();
    printf("Would you like to shuffle the cards (y/n)");
    scanf("%c",&userAns);
    if (userAns == 'y') {
        card *deck = newdeck();
        deck = shuffleDeck(deck);
    }else{
        predefinedCards();
    }
    printf("Wilma has\n");
    char suits[100]=  "🖤";
    char *print = suits;
    
    for (int i = 0; i < 1; i++) {
        printf("%s",print);
    }
    return 0;
}
void greetings(){
    FILE *greetingfile;
    greetingfile = fopen("/Users/nasseralhouti/desktop/go-fish-logo.txt", "r");
    char dashes;
    while(!feof(greetingfile)){
        fscanf(greetingfile, "%c",&dashes);
        printf("%c",dashes);
    }
}


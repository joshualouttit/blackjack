#include <iostream>
#include <random>
#include <iomanip>
#include <string>
#include "objects.h"
#include "helpers.h"
#include "constants.h"

int main () {
    deck *new_deck = new deck;
    new_deck->shuffle(CASINO_SHUFFLE);
    
    
    hand *player1 = new hand("player1");
    std::cout << "PLAYER DEALT CARDS";
    player1->drawCard(new_deck);
    player1->drawCard(new_deck);

    printHand(player1);


    std::cout << "PLAYER HAS DRAWN CARD\n";
    player1->drawCard(new_deck);
    printHand(player1);

    player1->freeCards();
    new_deck->freeCards();
    freeDeck(new_deck);
    freeHand(player1);
    return 0;
}


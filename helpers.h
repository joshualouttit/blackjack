#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include "objects.h"

void printDeck(deck *target_deck);
int randomNumber (int range);
void freeDeck (deck *free_this);
void freeHand (hand *free_this);
int cardValue(card_type type);
void printHand(hand *player);
void printCard(card_type type, suit card_suit);
void registerUser ();
std::string sha256(const std::string& password);

#endif
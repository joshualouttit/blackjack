#include "objects.h"
#include "constants.h"
#include <iostream>
#include <random>
#include <iomanip>
#include <string>

int cardValue(card_type type) {
    int value = 2;

    for (int i = two; i <= ten; i++) {
        if (type == i) {
            return value;
        }
        value++;
    }

    //This wont trigger if function is used correctly
    return 0;
}

//Written mostly by curtesy of CHAT GPT
void printCard(card_type type, suit card_suit) {
    std::string suit_icon;
    switch (card_suit) {
        case heart:
            suit_icon = "♥";
            break;
        case diamond:
            suit_icon = "♦";
            break;
        case club:
            suit_icon = "♣";
            break;
        case spade:
            suit_icon = "♠";
            break;
    }

    std::string card_name;
    switch (type) {
        case ace:
            card_name = "Ace";
            break;
        case two:
            card_name = "2";
            break;
        case three:
            card_name = "3";
            break;
        case four:
            card_name = "4";
            break;
        case five:
            card_name = "5";
            break;
        case six:
            card_name = "6";
            break;
        case seven:
            card_name = "7";
            break;
        case eight:
            card_name = "8";
            break;
        case nine:
            card_name = "9";
            break;
        case ten:
            card_name = "10";
            break;
        case jack:
            card_name = "Jack";
            break;
        case queen:
            card_name = "Queen";
            break;
        case king:
            card_name = "King";
            break;
    }

    std::cout << "+---------+\n";
    std::cout << "|         |\n";
    std::cout << "|  " << std::left << std::setw(6) << suit_icon << "   |\n";
    std::cout << "|         |\n";
    std::cout << "| " << std::left << std::setw(5) << card_name << " " << suit_icon << " |\n";
    std::cout << "|         |\n";
    std::cout << "|   " << std::left << std::setw(6) << suit_icon << "  |\n";
    std::cout << "|         |\n";
    std::cout << "+---------+\n";
}

void printDeck(deck *target_deck) {
    card *cards = target_deck->top;
    int counter = 0;
    while (counter < 3) {
        printCard(cards->type, cards->card_suit);
        std::cout << '\n';
        cards = cards->next;
        counter++;
    }

    return;
}

//Establish Random Generator
std::random_device rd;  // obtain a random number from hardware
std::mt19937 gen(rd());  // seed the generator

int randomNumber (int range) {
    std::uniform_int_distribution<> distr(0, range);
    int random = distr(gen);
    return random;
}

void freeDeck (deck *free_this) {
    free_this->freeCards();
    delete free_this;
    return;
}

void freeHand (hand *free_this) {
    free_this->freeCards();
    delete free_this;
    return;
}


void printHand(hand *player) {
    card *player_cards = player->cards;
    while (player_cards != NULL) {
        printCard(player_cards->type, player_cards->card_suit);
        player_cards = player_cards->next;
    }

    std::cout << "HAND VALUE: " << player->hand_value << " BUSTED? " << player->bust << '\n';
}


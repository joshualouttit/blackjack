#include <iostream>
#include <random>
#include <iomanip>
#include <string>
#include "objects.h"
#include "helpers.h"
#include "constants.h"
#include <thread>
#include <chrono>

#define LOGIN 1
#define REGISTER 2
#define EXIT_PROGRAM 3
#define PLAYING true

//Function prototypes
void playRound(std::string username);

int main () {
    //Login in User
    bool log = !LOGGED_IN; 
    std::string username;

    while (log != LOGGED_IN) {
        int input;
        std::cout << "Welcome to JLouttit's blackjack :)\n";
        std::cout << "Enter 1 to Login, 2 to Register or 3 to exit the program: ";
        std::cin >> input;

        if (input == LOGIN) {
            username = logIn();
            if (username != "\0") {
                log = LOGGED_IN;
                std::cout << "Logging in... standby\n";
            }
        } else if (input == REGISTER) {
            registerUser();
        } else if (input == EXIT_PROGRAM) {
            return 0;
        }
    }

    //Game Loop
    bool playing = PLAYING;
    while (playing == PLAYING) {
        int command;
        std::cout << "Press 1 to begin game, 2 to exit: ";
        std::cin >> command;

        if (command == 1) {
            playRound(username);
        } else if (command == 2) {
            playing = !PLAYING;
        }
    }

    return 0;
}

void playRound(std::string username) {
    //Begin Round
    class round *round1 = new class round(username);
    round1->dealCards();
    printRound(round1);

    int command;

    
    std::cout << "1 to draw, 2 to sit, or 3 to split: ";
    std::cin >> command;
    while (command != 2) {
        
        if (command == 1) {
            round1->player->drawCard(round1->card_deck);
            
        } else if (command == 3) {
            round1->player->splitHand();
        }

        printRound(round1);

        if (round1->player->bust == BUSTED) {
            printEndGame("loss", round1->player->hand_value, round1->dealer->hand_value, round1->player->bust);
            round1->freeRound();
            delete round1;
            return;
        }

        std::cout << "1 to draw, 2 to sit: ";
        std::cin >> command;
    }

    //Undoing this will print the dealers cards face up now
    round1->dealer->dealer_flag = !DEALER;

    //Automation
    if (round1->dealer->hand_value < round1->player->hand_value) {
        while (round1->dealer->hand_value < round1->player->hand_value) {
            round1->dealer->drawCard(round1->card_deck);
            printRound(round1);
            std::this_thread::sleep_for(std::chrono::seconds(2));


            if (round1->dealer->bust == BUSTED) {
                printEndGame("win", round1->player->hand_value, round1->dealer->hand_value, round1->player->bust);
                round1->freeRound();
                delete round1;
                return;
            }
        }
    } else {
        printRound(round1);
        printEndGame("win", round1->player->hand_value, round1->dealer->hand_value, round1->player->bust);
        round1->freeRound();
        delete round1;
        return;
    }

    //End Round
    printEndGame("win", round1->player->hand_value, round1->dealer->hand_value, round1->player->bust);
    round1->freeRound();
    delete round1;
}

std::string analyseRound(class round *_round) {
    //First thing to do here is to actually know the rules of blackjack properly

    //Player blackjack wins immediatly unless dealer also has (becomes tie)

    return "NULL";
}

/*
Lets assess the rules of blackjack
Immediate win on blackjack for player (unless) - dealer also has

Player decisions are as follows: (5 options)
    hit - take card
    stand - take no more cards
    doubledown - increase inital bet by 100% and take another card
    Split - create new hand  (requires another bet) - note you cant hit blackjack on a split
    surrender - forfiet half the bet and end hand immediately
    
    players must stand on 21

Insurance
    if dealers face up is an ace you can place a bet that the dealer has a blackjack
    asked before first player plays
    can be up to half the players bet (on top of bet not separate)
    if dealer has blackjack insurance pays 2 - 1 

House edge
    Casino generally has stat advantage of 1%
    Some casinos play that dealer must hit or stand on a soft 17
*/
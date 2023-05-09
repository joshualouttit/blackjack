#include <iostream>
#include <random>
#include <iomanip>
#include <string>
#include "objects.h"
#include "helpers.h"
#include "constants.h"

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
    std::cout << "Welcome to the game of Blackjack!" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Get as close to 21 as possible without going over!" << std::endl;
    std::cout << "Try to beat the dealer and win the game." << std::endl;
    std::cout << "Face cards are worth 10, and Aces are worth 1 or 11." << std::endl;
    std::cout << "Good luck and have fun!" << std::endl;
    std::cout << "================================" << std::endl;
    round1->dealCards();
    printRound(round1);

    //ROUND MECHANICS
    //User decisions
    int command;
    std::cout << "1 to draw, 2 to sit: ";
    std::cin >> command;
    while (command != 2) {
        
        if (command == 1) {
            round1->player->drawCard(round1->card_deck);
            printRound(round1);
        }

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
    round1->freeRound();
    delete round1;
}

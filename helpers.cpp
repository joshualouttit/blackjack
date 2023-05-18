#include "objects.h"
#include "constants.h"
#include <iostream>
#include <random>
#include <iomanip>
#include <string>
#include <fstream>
#include <filesystem>
#include <CommonCrypto/CommonDigest.h>

//Necessary forward declarations
std::string sha256(const std::string& password);
void printCards(hand *_hand);
std::string cardName(card *_card);
std::string suitIcon(card *_card);
void printCardLine(hand *_hand, std::string type);

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

void printRound(class round *round1) {
    //Print Dealers Cards
    std::cout << "Welcome to the game of Blackjack! - (Adjust your terminal window so this is the highest line for the best experience)" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Get as close to 21 as possible without going over!" << std::endl;
    std::cout << "Try to beat the dealer and win the game." << std::endl;
    std::cout << "Face cards are worth 10, and Aces are worth 1 or 11." << std::endl;
    std::cout << "Good luck and have fun!" << std::endl;
    std::cout << "================================" << std::endl;
    
    std::cout << "HOUSE CARDS\n";
    printCards(round1->dealer);
    if (round1->dealer->bust == BUSTED) {
        std::cout << "BUSTED\n";
    } else if (round1->dealer->dealer_flag != DEALER) {
        std::cout << "Value: " << round1->dealer->hand_value;
    } else {
        std::cout << '\n';
    }

    std::cout << "\n\n\n";

    //Print Player Cards
    printCards(round1->player);
    if (round1->player->bust == BUSTED) {
        std::cout << "BUSTED\n";
    } else {
        std::cout << "Value: " << round1->player->hand_value << '\n';
    }
}

void printCards(hand *_hand) {
    
    printCardLine(_hand, "top");
    printCardLine(_hand, "body");
    printCardLine(_hand, "high_body");
    printCardLine(_hand, "body");
    printCardLine(_hand, "middle");
    printCardLine(_hand, "body");
    printCardLine(_hand, "low_body");
    printCardLine(_hand, "body");
    printCardLine(_hand, "bot");

    return;
}

void printCardLine(hand *_hand, std::string type) {
    card *_cards = _hand->cards;
    bool first_card_flag = true;
    if (type == "top" || type == "bot") {
        while(_cards != NULL) {
            std::cout << "+---------+   ";
            _cards = _cards->next;

            if (
                _cards == NULL &&
                _hand->next != NULL
            ) {
                _hand = _hand->next;
                _cards = _hand->cards;
                std::cout << "          ";
            }
        }
    } else if (type == "body") {
        while (_cards != NULL) {
            if (
                _hand->dealer_flag == DEALER &&
                first_card_flag == true
            ) {
                std::cout << "|+++++++++|   ";
            } else {
                std::cout << "|         |   ";
            }
            _cards = _cards->next;

            if (
                _cards == NULL &&
                _hand->next != NULL
            ) {
                _hand = _hand->next;
                _cards = _hand->cards;
                first_card_flag = true;
                std::cout << "          ";
            }
        }
    } else if (type == "middle") {
        while (_cards != NULL) {
            if (
                _hand->dealer_flag == DEALER &&
                first_card_flag == true
            ) {
                std::cout << "|+++++++++|   ";
            } else {
                std::string suit_icon = suitIcon(_cards);
                std::string card_name = cardName(_cards);
                std::cout << "| " << std::left << std::setw(5) << card_name << " " << suit_icon << " |   ";
            }
            _cards = _cards->next;

            if (
                _cards == NULL &&
                _hand->next != NULL
            ) {
                _hand = _hand->next;
                _cards = _hand->cards;
                first_card_flag = true;
                std::cout << "          ";
            }
        }
    } else if (type == "low_body") {
        while (_cards != NULL) {
            if (
                _hand->dealer_flag == DEALER &&
                first_card_flag == true
            ) {
                std::cout << "|+++++++++|   ";
            } else {
                std::string suit_icon = suitIcon(_cards);
                std::cout << "|   " << std::left << std::setw(6) << suit_icon << "  |   ";
            }
            _cards = _cards->next;

            if (
                _cards == NULL &&
                _hand->next != NULL
            ) {
                _hand = _hand->next;
                _cards = _hand->cards;
                first_card_flag = true;
                std::cout << "          ";
            }
        }
    } else if (type == "high_body") {
        while (_cards != NULL) {
            if (
                _hand->dealer_flag == DEALER &&
                first_card_flag == true
            ) {
                std::cout << "|+++++++++|   ";
            } else {
                std::string suit_icon = suitIcon(_cards);
                std::cout << "|  " << std::left << std::setw(6) << suit_icon << "   |   ";
            }
            _cards = _cards->next;

            if (
                _cards == NULL &&
                _hand->next != NULL
            ) {
                _hand = _hand->next;
                _cards = _hand->cards;
                first_card_flag = true;
                std::cout << "          ";
            }
        }
    }
    std::cout << '\n';

    return;
}

std::string cardName(card *_card) {
    std::string card_name;
    switch (_card->type) {
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

    return card_name;
}

std::string suitIcon(card *_card) {
    std::string suit_icon;
    switch (_card->card_suit) {
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

    return suit_icon;
}

//Registration
void registerUser () {
    std::string username;
    std::cout << "Create Username: ";
    std::cin >> username;

    //Check if username already taken (file exists or not)
    std::string user_file_path = "users/" + username + ".txt";
    std::ifstream file(user_file_path);
    if (file) {
        std::cout << "Username already taken." << '\n';
        return;
    }

    std::string password;
    std::cout << "Enter Password: ";
    std::cin >> password;
    password = sha256(password);

    std::string verify;
    std::cout << "Verify Password: ";
    std::cin >> verify;
    verify = sha256(verify);

    if (verify != password) {
        std::cout << "Passwords do not match!\n";
        return;
    }

    std::string directory = "users/" + username + ".txt";
    
    std::ofstream outfile(user_file_path);
    if (outfile.is_open()) {
        outfile << password;
        outfile.close();
        std::cout << "Registered!\n";
    } else {
        std::cout << "Failed to open file for writing";
    }
}

std::string sha256(const std::string& input) {
    unsigned char hash[CC_SHA256_DIGEST_LENGTH];
    CC_SHA256((const unsigned char*)input.c_str(), (CC_LONG)input.length(), hash);
    
    std::string hashStr;
    char hexChar[3];
    for (int i = 0; i < CC_SHA256_DIGEST_LENGTH; ++i) {
        snprintf(hexChar, sizeof(hexChar), "%02x", hash[i]);
        hashStr += hexChar;
    }
    
    return hashStr;
}


//Login
std::string logIn () {
    std::string username;
    std::cout << "Username: ";
    std::cin >> username;
    
    //Open user file
    std::string user_file_path = "users/" + username + ".txt";
    std::ifstream file(user_file_path);
    
    //In this case userfile cannot be opened
    if (!file) {
        std::cout << "Username not recognised\n";
        return "\0";
    }
    
    std::string password;
    std::cout << "Password: ";
    std::cin >> password;
    password = sha256(password);

    std::string password_hash;
    file >> password_hash;

    if (password == password_hash) {
        file.close();
        return username;
    } else {
        std::cout << "Incorrect password\n";
        file.close();
        return "\0";
    }
}

void printEndGame (std::string flag, int player_value, int dealer_value, bool bust) {
    if (bust == BUSTED) {
        std::cout << "OVER 21!\nYOU LOSE!\nGAMEOVER :)\n";
    } else if (
        dealer_value > 21
    ) {
        std::cout << "THE DEALER WENT BUST!\n";
    } else if (
        player_value > dealer_value
    ) {
        std::cout << "YOU HAVE " << player_value << " against the dealers " << dealer_value << "\n";
        std::cout << "YOU WIN!\n";
    } else if (player_value < dealer_value) {
        std::cout << "YOU HAVE " << player_value << " against the dealers " << dealer_value << "\n";
        std::cout << "YOU LOSE\n";
    } else if (player_value == dealer_value) {
    }
}
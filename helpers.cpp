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


//Login and registration
void registerUser () {
    std::filesystem::path directory = "users";
    std::filesystem::create_directory(directory);
    
    std::string username;
    std::cout << "Create Username: ";
    std::cin >> username;

    //Check if username already taken
    

    std::string password;
    std::cout << "Enter Password: ";
    std::cin >> password;
    password = sha256(password);

    std::string verify;
    std::cout << "Verify Password: ";
    std::cin >> verify;
    verify = sha256(password);

    if (verify != password) {
        std::cout << "Passwords do not match!";
        return;
    }


    
    
    //Check for 
    std::filesystem::path allusers = directory / ("users.txt");
    std::ofstream all_users 
    
    std::ofstream user_file;
    std::filesystem::path user_file = directory / (username + ".txt");
    std::ofstream user_file(user_file);
    if (user_file.is_open()) {
        user_file << password;
        user_file.close();
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
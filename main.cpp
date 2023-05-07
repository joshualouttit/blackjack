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

}


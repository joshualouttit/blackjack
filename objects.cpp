#include "objects.h"
#include "helpers.h"
#include <iostream>
#include <random>
#include <iomanip>
#include <string>
#include "constants.h"

deck::deck() {
    createDeck();
}

void deck::shuffle (int shuffletimes) {
    //This swap is necessary so we dont lose any cards we have allocated memory too
    for (int i = 0; i < shuffletimes; i++) {
        card *head_holder = top;
        top = NULL;
        
        int deck_count = CARDS_IN_DECK - 1;

        while (deck_count >= 0) {
            //Originally had head_holder != NULL as my while condition - 
            //not sure why that doesnt work (would keep going past 52 cards)
            //- could be a future memory issue
            
            int random = randomNumber(deck_count);

            //FIND MOVING CARD
            //This is the selected card to move to suffled deck
            card *holder = head_holder;
            card *previous = NULL;
            for (int i = 0; i < random; i++) {
                previous = holder;
                holder = holder->next;
            }

            //ADJUST HEAD_HOLDER LIST (original deck)
            if (previous == NULL) {
                head_holder = head_holder->next;
            } else {
                previous->next = holder->next;
            }
            //Removing connection
            holder->next = NULL;

            //ADD TO NEW LIST
            if (top == NULL) {
                top = holder;
            } else {
                card *tmp = top;
                while (tmp->next != NULL) {
                    tmp = tmp->next;
                }

                tmp->next = holder;
            }

            //DECREMENT DECK COUNT
            deck_count--;
        }
    }
    return;
}

void deck::drawn() {
    card *tmp = top->next;

    //Ends players linked list
    top->next = NULL;
    top = tmp;
    return;
}

void deck::freeCards() {
    
    while (top != NULL) {
        card *tmp = top;
        top = top->next;
        delete tmp;
    }

    return;
}

void deck::createDeck () {
    card *previous = NULL;
    for (int j = heart; j <= spade; j++) {
        for (int i = ace; i <= king; i++) {
            card *new_card = new card;
            
            new_card->card_suit = static_cast<suit>(j);
            new_card->type = static_cast<card_type>(i);
            new_card->next = NULL;

            //Linkage 
            if (previous != NULL) {
                previous->next = new_card;
            }

            //Link to top if necessary
            if (
                new_card->card_suit == heart &&
                i == ace
            ) {
                top = new_card;
            }



            previous = new_card;
        }
    }

    return;
}

hand::hand (std::string player_name) {
    name = player_name;
    number_of_cards = 0;
    hand_value = 0;
    cards = NULL;
    bust = !BUSTED;
}

void hand::drawCard(deck *draw_from) {
    //Add to players hand
    if (draw_from->top->type == ace) {
        number_aces++;
    }

    if (cards == NULL) {
        cards = draw_from->top;
    } else {
        card *tmp = cards;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }

        tmp->next = draw_from->top;
    }

    //Remove from top (also includes setting top->next = NULL)
    draw_from->drawn();

    number_of_cards++;
    valueHand();

    return;
}

void hand::freeCards() {
    
    while (cards != NULL) {
        card *tmp = cards;
        cards = cards->next;
        delete tmp;
    }

    return;
}

void hand::valueHand () {
    hand_value = 0;
    number_aces = 0;

    //Value hand by just adding
    card *tmp = cards;
    while (tmp != NULL) {
        if (
            tmp->type == king ||
            tmp->type == queen ||
            tmp->type == jack
        ) {
            hand_value += 10;
        } else if (
            tmp->type == ace
        ) {
            hand_value += 11;
            number_aces++;
        } else {
            hand_value += cardValue(tmp->type);
        }
        tmp = tmp->next;
    }
    
    if (hand_value > 21) {
        int test_value = hand_value;
        for(int i = 1; i <= number_aces; i++) {
            //Idea is that we change an ace to a 1
            std::cout << "Are we taking this step?\n";
            
            test_value -= (10 * i);
            if (
                (hand_value > 21 && test_value < 21)
            ) {
                hand_value = test_value;
            }
        }
    }

    //If hand value is still > 21 - mark as BUST
    if (hand_value > 21) {
        bust = BUSTED;
    }
}
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

hand::hand (std::string player_name, bool dealer) {
    name = player_name;
    number_of_hands = 1;
    number_of_cards = 0;
    hand_value = 0;
    cards = NULL;
    bust = !BUSTED;
    hand *next = NULL;

    if (dealer == DEALER) {
        dealer_flag = DEALER;
    } else {
        dealer_flag = !DEALER;
    }
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


//POTENTIAL MEMORY PROBLEM HERE
//If we get 2 4's -> split the hand and then get another 4 on hand 1
//calling split hands again will lose access to memory of the first split hand
//attempting to fix now - STATUS - UNFIXED

//Testing shows that this isnt actually the issue:
//Steps:
//Printed pointers and list is updating correctly
//Tested print function - islated issue to the lines:
//_hand = _hand->next;
//_cards = _hand->cards
//Indicates the next hand cards are NULL?
//Im an idiot - it doesnt print cause im not drawing a card
//Cant split nothing lmao
void hand::splitHand() {
    //Alter attributes of first hand
    number_of_hands += 1;
    number_of_cards = 1;
    
    hand *new_hand = new hand(name, dealer_flag);
    new_hand->number_of_cards = 1;
    new_hand->number_of_hands = number_of_hands + 1;

    //Move card over
    new_hand->cards = cards->next;
    cards->next = NULL;

    //Add new hand correctly (if tmp is NULL will add NULL correctly)
    hand *tmp = next;
    std::cout << "next: " << next << '\n';
    next = new_hand;
    std::cout << "new_hand is now next from round: " << next << '\n';
    new_hand->next = tmp;
    std::cout << "new_hand->next is now " << tmp << '\n';

    //Revalue current hand
    valueHand();

    //Value next hand
    new_hand->valueHand();

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

void hand::freeHand() {
    //Goal is to free the next hand (and any others) and its cards, 
    //and then this cards hand. The allocation of the pointer to THIS
    //HAND will need to be freed in free round function

    //Free current cards 
    freeCards();

    hand *tmp = next;

    while (tmp != NULL) {
        tmp->freeCards();

        hand *tmp2 = tmp;
        tmp = tmp->next;
        delete tmp2;
    }
}

round::round(std::string username) {
    //Allocate memory for dealer, player, card_deck
    //(Creating objects)

    //Create hands and allocate names
    hand *house = new hand("house", DEALER);
    hand *player1 = new hand(username, !DEALER);

    //create deck and shuffle
    deck *card_stack = new deck;
    card_stack->shuffle(CASINO_SHUFFLE);

    //Mark all attributes to round
    dealer = house;
    player = player1;
    card_deck = card_stack;
}

void round::freeRound() {
    dealer->freeHand();
    player->freeHand();
    card_deck->freeCards();
    
    //Free what pointers are pointing too
    delete dealer;
    delete player;
    delete card_deck;

    //Set attributes to NULL
    dealer = NULL;
    player = NULL;
    card_deck = NULL;
}

void round::dealCards() {
    player->drawCard(card_deck);
    dealer->drawCard(card_deck);
    player->drawCard(card_deck);
    dealer->drawCard(card_deck);

    return;
}
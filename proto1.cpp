#include <iostream>
#include <random>

#define CARDS_IN_DECK 52

enum card_type {
    ace,
    one,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king
};

enum suit {
    heart,
    diamond,
    club,
    spade
};

class card {
public:
    card_type type;
    suit card_suit;
    card *next;
};

//Prototype
card *orderedDeck ();

class deck {
public:
    card *top;

    deck() {
        top = orderedDeck();
    }

    card *orderedDeck () {

        card *top = NULL;
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

        return top;
    }
};

class player_hand {
public:
    card *cards;
    player_hand *next;
};

class round {
public:
    card *house_cards;
    player_hand *players;
};

//Establish Random Generator
std::random_device rd;  // obtain a random number from hardware
std::mt19937 gen(rd());  // seed the generator

//Prototypes
deck *newDeck();
void freeDeck(deck *deck1);
void freeCards(card *cards);
void printDeck(deck *target_deck);
int randomNumber (int range);
void shuffle (deck *target_deck);


int main () {
    deck *new_deck;

    printDeck(new_deck);

    freeDeck(new_deck);
}

deck *newDeck() {

    /*deck *ordered_deck = orderedDeck();

    shuffle(ordered_deck);

    return ordered_deck;
    */
   return NULL;
}

/*
deck *orderedDeck () {

    deck *ordered_deck = new deck;
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
                i == 0
            ) {
                ordered_deck->top = new_card;
            }



            previous = new_card;
        }
    }

    return ordered_deck;
}
*/

void freeDeck(deck *deck1) {

    freeCards(deck1->top);

    delete deck1;

    return;
}

void freeCards(card *cards) {
    while (cards != NULL) {
        card *tmp = cards;
        cards = cards->next;
        delete tmp;
    }

    return;
}

void printDeck(deck *target_deck) {
    card *cards = target_deck->top;

    while (cards != NULL) {
        std::cout << "Card: " << cards->type << " suit: " << cards->card_suit << '\n';
        cards = cards->next;
    }

    return;
}

void shuffle (deck *target_deck) {
    //This swap is necessary so we dont lose any cards we have allocated memory too
    
    card *head_holder = target_deck->top;
    target_deck->top = NULL;
    
    int deck_count = CARDS_IN_DECK - 1;

    while (deck_count >= 0) {
        //Originally had head_holder != NULL as my while condition - 
        //not sure why that doesnt work (would keep going past 52 cards)
        //- could be a future memory issue
        
        int random = randomNumber(deck_count);
        std::cout << "This is random: " << random << " This is the range: " << deck_count << '\n';

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
        if (target_deck->top == NULL) {
            target_deck->top = holder;
        } else {
            card *tmp = target_deck->top;
            while (tmp->next != NULL) {
                tmp = tmp->next;
            }

            tmp->next = holder;
        }

        //DECREMENT DECK COUNT
        deck_count--;
    }
    
    return;
}

int randomNumber (int range) {
    std::uniform_int_distribution<> distr(0, range);
    int random = distr(gen);
    return random;
}
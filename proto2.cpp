//Problem here is the local delcaration of deck doesnt let class hand
// make changes to deck given. Want to change to use pointers



#include <iostream>
#include <random>
#include <iomanip>
#include <string>

#define CARDS_IN_DECK 52
#define CASINO_SHUFFLE 50

enum card_type {
    ace,
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

//function prototypes
int randomNumber (int range);

class card {
public:
    card_type type;
    suit card_suit;
    card *next;
};

class deck {
public:
    card *top;

    deck () {
        createDeck();
    }

    void shuffle (int shuffletimes) {
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

    void drawn() {
        card *tmp = top->next;

        //Ends players linked list
        top->next = NULL;
        top = tmp;
        return;
    }

    void freeCards() {
        
        while (top != NULL) {
            card *tmp = top;
            top = top->next;
            delete tmp;
        }

        return;
    }

private:
    void createDeck () {
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
};

class hand {
public:
    std::string name;
    int number_of_cards;
    int hand_value;
    card *cards;

    hand (std::string player_name) {
        name = player_name;
        number_of_cards = 0;
        hand_value = 0;
        cards = NULL;
    }

    void drawCard(deck draw_from) {
        //Add to players hand
        if (cards == NULL) {
            cards = draw_from.top;
        } else {
            card *tmp = cards;
            while (tmp->next != NULL) {
                tmp = tmp->next;
            }

            tmp->next = draw_from.top;
        }

        //Remove from top (also includes setting top->next = NULL)
        draw_from.drawn();

        return;
    }

    void freeCards() {
        
        while (cards != NULL) {
            card *tmp = cards;
            cards = cards->next;
            delete tmp;
        }

        return;
    }
};

//Establish Random Generator
std::random_device rd;  // obtain a random number from hardware
std::mt19937 gen(rd());  // seed the generator

//Other prototypes
void printDeck(deck *target_deck);
void printCard(card_type type, suit card_suit); 

int main () {
    deck new_deck;

    new_deck.shuffle(CASINO_SHUFFLE);

    std::cout << "ORIGINAL DECK\n";
    printDeck(&new_deck);
    
    std::cout << "PLAYER1 HAND PRE DRAW\n";
    hand player1("player1");
    std::cout << player1.name << '\n';
    card *tmp = player1.cards;
    while (tmp != NULL) {
        printCard(tmp->type, tmp->card_suit);
    }

    std::cout << "PLAYER HAS DRAWN CARD\n";
    player1.drawCard(new_deck);

    std::cout << "DECK POST DRAW\n";
    printDeck(&new_deck);

    std::cout << "PLAYER HAND POST DRAW\n";
    std::cout << player1.name << '\n';
    card *tmp2 = player1.cards;
    while (tmp2 != NULL) {
        printCard(tmp->type, tmp->card_suit);
    }

    player1.freeCards();
    new_deck.freeCards();
    return 0;
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

int randomNumber (int range) {
    std::uniform_int_distribution<> distr(0, range);
    int random = distr(gen);
    return random;
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
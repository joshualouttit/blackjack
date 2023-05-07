#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>

//ENUMS
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

//CLASSES
class card {
public:
    card_type type;
    suit card_suit;
    card *next;
};

class deck {
public:
    card *top;
    deck ();
    void shuffle (int shuffletimes);
    void drawn();
    void freeCards();
private:
    void createDeck ();
};

class hand {
public:
    //Variables
    std::string name;
    int number_of_cards;
    int hand_value;
    bool bust;
    int number_aces;
    card *cards;
    hand *next;

    hand (std::string player_name);
    void drawCard(deck *draw_from);
    void freeCards();
private:
    void valueHand ();
};

#endif
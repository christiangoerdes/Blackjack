#pragma once
#include <iostream>

namespace Blackjack {

    // Card class
    class Card {
    public:
        // Constructor
        Card(char suit, std::string type);
        // Attributes
        char _suit;
        std::string _type;
    };

}
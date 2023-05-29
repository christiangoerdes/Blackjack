#pragma once

namespace Blackjack {

    class Card {
    public:
        Card(char suit, std::string type);

        char _suit;
        std::string _type;
    };

}
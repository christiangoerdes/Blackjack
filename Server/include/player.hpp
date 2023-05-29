#pragma once
#include <iostream>
#include <vector>

#include "card.hpp"

namespace Blackjack {

    class Player{
    public:
        Player(std::string name, std::string password, int init_balance);

        std::string _name;
        std::string _password;
        int _balance;
        std::vector<Card> _deck;
        int _bet = 0;
        bool _in_round;
        int _deck_value = 0;
    };

}
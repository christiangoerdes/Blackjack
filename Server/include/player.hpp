#pragma once
#include <iostream>
#include <vector>

#include "card.hpp"

namespace Blackjack {

    // Player class
    class Player{
    public:
        // Constructor
        Player(std::string name, std::string password, int init_balance);
        // Attributes 
        std::string _name;
        std::string _password;
        int _balance;
        std::vector<Card> _deck;
        int _bet = 0;
        bool _in_round;
        int _deck_value = 0;
    };

}
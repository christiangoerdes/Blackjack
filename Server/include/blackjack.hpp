#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <random>

namespace Blackjack {

using letter_t = std::string::value_type;
  
class BlackjackGame {
public:
    /**
     * @brief Initializes a game of blackjack with default balance of $1000
    */
    BlackjackGame();

    /**
     * @brief Initializes a game of blackjack with a custom balance (only integers allowed)
     * @param balance Initial balance (in dollars, only integers allowed)
    */
    BlackjackGame(const size_t balance);

    /**
     * @brief Place a bet at the start of a game (only integers allowed)
     * @param bet Bet to be placed (in dollars, only integers allowed)
    */
    bool place_bet(const size_t bet);

    /**
     * @brief Add a card to the deck
    */
    bool add_card(const size_t player_id);


    void join(const size_t player_id);

    void leave(const size_t player_id);

    std::string game_status();

  

private:
    void fill_deck();
    struct Card;
    struct Player;
    size_t balance_;
    std::vector<Card> deck;
    std::map<size_t, Player> players;
    friend std::ostream& operator<<(std::ostream&, const BlackjackGame&);
};



}

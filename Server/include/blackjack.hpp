#pragma once

#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

#include "card.hpp"
#include "player.hpp"

namespace Blackjack {

    class BlackjackGame {

    public:

        BlackjackGame();

        BlackjackGame(int init_balance);

        /// Game State 0: Not Started

        /**
         * @brief Starts a new game round
         * @return Feedback whether call was valid
        */
        bool start_round();

        /**
         * @brief Adds a new player to the game
         * @return Feedback whether call was valid
        */
        bool join(std::string name, std::string password);

        /**
         * @brief Player leaves the game
         * @return Feedback whether call was valid
        */
        bool leave(std::string name, std::string password);

        /// Game State 1: Placing Bets

        /**
         * @brief Places bet of player whose turn it is
         * @param name Name of the players
         * @param password Player's password
         * @param bet Size of the bet (in dollars, integers)
         * @return Feedback whether call was valid
        */
        bool place_bet(std::string name, std::string password, int bet);

        /**
         * @brief Current player draws a card
         * @return Feedback whether call was valid
        */
        bool draw(std::string name, std::string password);

        /**
         * @brief Lets players voluntarily skip their turn drawing cards
         * @return Feedback whether call was valid
        */
        bool skip(std::string name, std::string password);

        int getInitBalance() const;

        int getGameState() const;

        std::vector<Blackjack::Player> getPlayers() const;

        Blackjack::Player getDealer() const;

    private:

        int _init_balance;
        int _game_state;
        int _turn;
        int _MIN_BET = 5;
        std::vector<Card> _deck;
        std::vector<Player> _players;
        Player dealer;
        const std::vector<std::string> _card_types{"2", "3", "4", "5", "6", "7", "8", "9", "10", "A", "J", "Q", "K"}; // 13 types of cardes
        const std::vector<char> _suits{'C','S','D','H'}; // 4 types of suits: clubs, spades, diamonds and hearts

        /**
         * @brief Fills the game deck with all 52 cards in random order
        */
        void fill_deck();

        /**
         * @brief Hands player a card from the game deck
        */
        void hand_card(Player &player);

        /**
         * @brief Computes the value of a player's deck
         * @return Numerical value of the player's deck
        */
        int deck_value(const std::vector<Card> &deck) const;

        /**
         * @brief Rewards a winning player with twice their bet
        */
        void reward(Player &winner);

        /**
         * @brief Losing player loses their bet
        */
        void lose(Player &loser);

        /**
         * @brief Indicates whether any players are left in the round
         * @return True iff any players are still in the round
        */
        bool players_left() const;

        /**
         * @brief Checks whether player has won or lost. Updates turn variable and makes transactions where possible
        */
        void check_reward(Player &player);

        /**
         * @brief Determine id of the next player to make a turn
         * @return id of next player to make a turn
        */
        int next_player() const;

        /**
         * @brief Dealer draws cards after all players have drawn theirs
        */
        void draw_dealer();



    };

}
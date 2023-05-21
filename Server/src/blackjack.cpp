#pragma once

#include <blackjack.hpp>
#include <iostream>
#include <random>

namespace Blackjack {

class BlackjackGame{

    public:

    BlackjackGame(){
        balance_ = 1000;
    }

    BlackjackGame(const size_t balance){
        balance_ = balance;
    }

    bool start_round(){
        if (game_state != 0 && players.size() < 2){   // if game not already running and at least two players are present
            return false;
        }

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(0, players.size()-1); // define the range

        size_t dealer_id = distr(gen);  // determine dealer randomly
        players[dealer_id].dealer_ = true;

        game_state = 1; // set state to 1 (placing bets)
        if (dealer_id != 0){ // the dealer places no bets
            turn_ = 0;
        }
        else {
            turn_ = 1;
        }
        return true;
    }

    bool place_bet(const std::string name, const std::string password, const size_t bet){ // player places bet

        Player& current_better = players[turn_];

        if (game_state == 1){   // if all bets have not been placed already
            if (current_better.name_ == name && current_better.password_ == password && bet >= MIN_BET) { // if it is the player's turn and the bet is not below the minimum bet
                current_better.bet_ = bet;

                turn_++;
                if (turn_ >= players.size()){ // if all players have placed their bets
                    game_state = 2;
                    turn_ = 0;
                }
                else {
                    if (players[turn_].dealer_){ // if the next player in the list is the dealer
                        turn_++;
                        if (turn_ >= players.size()){ // if the dealer was the last better
                            game_state = 2;
                            turn_ = 0;
                        }
                    }
                }
                return true;
                
            }
        }
        return false;
    }

    bool add_card(const size_t player_id, const std::string password){
        if (deck.size() > 0) { // if there are still cards left in the deck
            players[player_id].player_deck_.push_back(deck.back());
            deck.pop_back();
        }
        else {
            return false; // no cards left to distribute
        }
    }

    void skip(size_t player_id, const std::string password) const{

    }

    bool join(const std::string name, const std::string password){ // adds a new player to the game
        players.push_back(Player(name, password, balance_));
    }

    private:

    void fill_deck(){ // fills the game deck
        deck.clear(); // empty deck
        std::vector<char> suits{'C','S','D','H'}; // clubs, spades, diamonds and hearts

        for (const char& suit : suits) {
            for (size_t value = 1; value < 11; value++){
                deck.push_back(Card(suit, value));
            }
        }

        auto rng = std::default_random_engine {};
        std::shuffle(std::begin(deck), std::end(deck), rng); // shuffle the deck
    }

    struct Card{
        Card(char suit, size_t value){
            suit_ = suit;
            value_ = value;
        }
        char suit_;
        size_t value_;
    };

    struct Player {
        Player(std::string name, std::string password, size_t balance){
            balance_ = balance;
            name_ = name;
            password_ = password;
            dealer_ = false;
            bet_ = 0;
        }
        std::vector<Card> player_deck_;
        std::string name_;
        std::string password_;
        size_t balance_;
        size_t bet_;
        bool dealer_;
    };

    size_t deck_value(const std::vector<Card>& player_deck) const{ // computes the value of a player's deck
        size_t total_deck_value;
        size_t num_aces = 0;

        for (const Card& card : player_deck){
            if (card.value_ > 1){ // sum up all regular cards
                total_deck_value += card.value_;
            }
            else { // determine number of aces in the player's deck
                num_aces += 1;
            }
        }

        if (num_aces > 0){
            if (total_deck_value + 11 + (num_aces-1) <= 21){ // if it is profitable to count one ace as having value 11
                total_deck_value += 11 + (num_aces-1);
            }
            else {
                total_deck_value += num_aces; // otherwise count all aces as having value 1
            }
        }

        return total_deck_value;
    }
    size_t game_state; // 0: not started; 1: placing bets; 2: ...
    size_t balance_;
    size_t turn_;
    std::vector<Card> deck;
    std::vector<Player> players;
    const size_t MIN_BET = 5;
    friend std::ostream& operator<<(std::ostream&, const BlackjackGame&);

};

}

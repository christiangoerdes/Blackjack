#pragma once

#include <blackjack.hpp>
#include <iostream>
#include <random>

namespace Blackjack {

class BlackjackGame{

    public:

    BlackjackGame(){
        balance_ = 1000;
        game_state = 0;
        players = std::vector<Player>();
    }

    BlackjackGame(const size_t balance){
        balance_ = balance;
        game_state = 0;
        players = std::vector<Player>();
    }

    bool start_round(){ // start a new round of the game
        if (game_state != 0 && players.size() < 2){   // if game not already running and at least two players are present
            return false;
        }

        players[dealer_id_].dealer_ = false; // unassign dealer position from previous round
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(0, players.size()-1); // define the range

        size_t dealer_id = distr(gen);  // determine dealer randomly
        players[dealer_id].dealer_ = true;

        game_state = 1; // set state to 1 (placing bets)
        turn_ = -1;
        turn_ = next_player_id(); // determine the first player to place a bet
        return true;
    }

    bool place_bet(const std::string name, const std::string password, const size_t bet){ // player places bet

        if (game_state != 1){   // if the game state is not "placing bets"
            return false;
        }

        Player& current_better = players[turn_];
        if (current_better.name_ == name && current_better.password_ == password && bet >= MIN_BET) { // if it is the player's turn and the bet is not below the minimum bet
            current_better.bet_ = bet;
            turn_ = next_player_id(); // assign next player to place a bet
            if (turn_ == -1){ // if all players have placed their bets
                game_state = 2; // change game mode to "drawing cards"
                turn_ = next_player_id(); // determine first player to draw cards
            }
            return true;
        }
    }

    bool add_card(const std::string name, const std::string password){ // adds a card to a player's or the dealer's deck

        if (game_state == 2){ // if the game state is "players drawing cards"
            Player& current_player = players[turn_];
            if (current_player.name_ == name && current_player.password_ == password) { // check credentials
                current_player.player_deck_.push_back(deck.back());
                deck.pop_back();

                size_t player_deck_value = deck_value(current_player.player_deck_); // determine the current player's deck value

                if (player_deck_value >= 21){
                    if (player_deck_value == 21){
                        current_player.in_round_ = false; // player leaves the round
                        current_player.balance_ += current_player.bet_*2; // player wins twice their bet
                        players[dealer_id_].balance_ -= current_player.bet_*2; // dealer loses twice the current player's bet
                    }
                    else {
                        current_player.in_round_ = false; // players leaves the round
                        current_player.balance_ -= current_player.bet_; // player busts (loses their bet)
                        players[dealer_id_].balance_ += current_player.bet_; // dealer gets the current player's bet
                    }
                    turn_ = next_player_id(); // automatic skipping if player leaves the round
                    if (turn_ == -1){ // if all players have skipped
                        game_state = 3; // it is the dealer's turn now
                    }
                }
                return true; // drawing successful
            }
        }
        else if (game_state == 3){ // if the game state is "dealer drawing cards"
            Player& dealer = players[dealer_id_];
            if (dealer.name_ == name && dealer.password_ == password){ // check credentials
                dealer.player_deck_.push_back(deck.back());
                deck.pop_back();

                size_t dealer_deck_value = deck_value(dealer.player_deck_); // determine the dealer's deck value
                if (dealer_deck_value >= 17){ // if the dealer can draw no more cards
                    game_state = 4; // enter game state "determine winners"
                }
                return true; // drawing successful
            }
        }
        return false;
    }

    void skip(const std::string name, const std::string password){
        Player& current_player = players[turn_];
        if (game_state == 2){
            if (current_player.name_ == name && current_player.password_ == password) { // if it is the player's turn
                turn_ = next_player_id(); // determine next player
                if (turn_ == -1){ // if all players have skipped
                    game_state = 3; // it is the dealer's turn now
                }
            }
        }
    }

    bool join(const std::string name, const std::string password){ // adds a new player to the game
        if (game_state == 0){ // TODO: perhaps should check if such a player already exists
            players.push_back(Player(name, password, balance_));
            return true;
        }
        return false;
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

    struct Card{ // TODO: modify to include type (as string probably) - also modify deck_value to work with types rather than values
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
            in_round_ = true;
        }
        std::vector<Card> player_deck_;
        std::string name_;
        std::string password_;
        size_t balance_;
        size_t bet_;
        bool in_round_;
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
    size_t dealer_id_;
    std::vector<Card> deck;
    std::vector<Player> players;

    size_t next_player_id(){ // determines the id of the next player
        size_t turn = turn_;
        turn++;

        while (turn < deck.size()){
            if (!players[turn].dealer_ && players[turn].in_round_){
                return turn;
            }
            else {
                turn++;
            }
        }
        return -1; // if there are no more players with turns to make
    }

    const size_t MIN_BET = 5;
    friend std::ostream& operator<<(std::ostream&, const BlackjackGame&);

};

}

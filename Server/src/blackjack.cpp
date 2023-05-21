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
        dealer_id_ = 0;
        players = std::vector<Player>();
    }

    BlackjackGame(const size_t balance){
        balance_ = balance;
        game_state = 0;
        dealer_id_ = 0;
        players = std::vector<Player>();
    }

    bool start_round(){ // start a new round of the game
        if (game_state != 0 && players.size() < 2){   // if game not already running and at least two players are present
            return false;
        }

        fill_deck(); // fill and shuffle the game deck
        assign_dealer(); // determine the dealer

        game_state = 1; // set state to 1 (placing bets)
        turn_ = next_player_id(-1); // determine the first player to place a bet
        return true;
    }

    bool determine_winners(){ // determine winners in final game state

        if (game_state != 4){ // if not all of the turns have been made yet
            return false;
        }

        Player& dealer = players[dealer_id_];
        size_t dealer_value = deck_value(dealer.player_deck_);

        for (Player& player : players){
            size_t player_value = deck_value(player.player_deck_);
            if (!player.dealer_ && player.in_round_){ // only consider non-dealer players who are still in the round
                if (dealer_value > 21 || player_value > dealer_value){ // if the dealer busts or the player deck value exceeds the dealer deck value
                    player.balance_ += player.bet_*2; // player wins twice their bet
                    dealer.balance_ -= player.bet_*2; // dealer loses twice the player's bet
                }
                else if (player_value < dealer_value){ // if the dealer does not bust: compare deck values
                    player.balance_ -= player.bet_; // player loses their bet
                    dealer.balance_ += player.bet_; // dealer wins the player's bet
                }
            }
        }
        game_state = 0; // set game mode to "not started"
        return true;
    }

    /**
     * @brief Places bet of player whose turn it is
     * @param name Name of the players
     * @param password Player's password
     * @param bet Size of the bet (in dollars, integers)
     * @return Feedback about whether the placing operation was successful (as bool)
    */
    bool place_bet(const std::string name, const std::string password, const size_t bet){ // player places bet

        if (game_state != 1){   // if the game state is not "placing bets"
            return false;
        }

        Player& current_better = players[turn_];

        if (current_better.name_ == name && current_better.password_ == password && bet >= MIN_BET && bet >= current_better.balance_) { // check credentials and bet validity

            current_better.bet_ = bet; // save the bet
            turn_ = next_player_id(turn_); // assign next player to place a bet

            if (turn_ == -1){ // if all players have placed their bets

                game_state = 2; // change game mode to "drawing cards"

                for (Player& player : players){ // hand each player (and the dealer) two cards from the deck
                    hand_card(player); // first card
                    hand_card(player); // second card
                }

                if (!players_left_in_round()){ // if all players have won after the first two cards
                    game_state = 0;
                }
            }

            return true;
        }
    }

    bool add_card(const std::string name, const std::string password){ // adds a card to a player's or the dealer's deck

        if (game_state == 2){ // if the game state is "players drawing cards"

            Player& current_player = players[turn_];

            if (current_player.name_ == name && current_player.password_ == password) { // check credentials

                hand_card(current_player);

                if (players_left_in_round()){ // if there are any players who have not won or bust

                    if (!current_player.in_round_){ // if the current player has won or bust after receiving the card
                        
                        //WIP: die korrekten Übergänge einbauen
                        
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

    bool skip(const std::string name, const std::string password){ // lets a player skip their turn drawing cards

        if (game_state != 2){
            return false;
        }

        Player& current_player = players[turn_];
        if (current_player.name_ == name && current_player.password_ == password) { // if it is the player's turn
            turn_ = next_player_id(); // determine next player
            if (turn_ == -1){ // if all players have skipped
                game_state = 3; // it is the dealer's turn now
            }
        }
    }

    bool join(const std::string name, const std::string password){ // adds a new player to the game
        // TODO: perhaps exclude possibility of same-name players
        if (game_state != 0){
            return false; // cannot join running game
        }
        players.push_back(Player(name, password, balance_));
        return true;
    }

    bool leave(const std::string name, const std::string password){ // player leaves the game
        if (game_state != 0){
            return false; // cannot leave running game
        }
        for (size_t player_idx = 0; player_idx < players.size(); player_idx++){
            Player& player = players[player_idx];
            if (player.name_ == name && player.password_ == password){ // check credentials
                players.erase(players.begin() + player_idx); // remove the player from the list of players
                return true;
            }
        }
        return false;
    }

private:

    size_t MIN_BET = 5;
    const std::vector<std::string> card_types{"2", "3", "4", "5", "6", "7", "8", "9", "10", "A", "J", "Q", "K"}; // 13 types of cardes
    const std::vector<char> suits{'C','S','D','H'}; // 4 types of suits: clubs, spades, diamonds and hearts

    void fill_deck(){ // fills the game deck
        deck.clear(); // empty deck
        
        for (const char& suit : suits) {
            for (const std::string& type : card_types){
                deck.push_back(Card(suit, type));
            }
        }

        auto rng = std::default_random_engine {};
        std::shuffle(std::begin(deck), std::end(deck), rng); // shuffle the deck
    }

    struct Card{
        Card(const char suit, const std::string& type){
            suit_ = suit;
            type_ = type;
        }
        char suit_;
        std::string type_;
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

    void hand_card(Player& player){
        Player& dealer = players[dealer_id_];

        player.player_deck_.push_back(deck.back());
        deck.pop_back();

        if (deck_value(player.player_deck_) == 21){
            player.balance_ += player.bet_*2; // player wins twice their bet
            players[dealer_id_].balance_ -= player.bet_*2; // dealer loses twice the player's bet
            player.in_round_ = false; // player leaves the round
        }
        else if (deck_value(player.player_deck_) > 21){ // bust
            player.balance_ -= player.bet_;
            dealer.balance_ += player.bet_;
            player.in_round_ = false;
        }

    }

    bool players_left_in_round(){
        size_t turn = next_player_id();
        return turn != -1;
    }

    void assign_dealer(){
        players[dealer_id_].dealer_ = false; // unassign dealer position from previous round
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(0, players.size()-1); // define the range

        size_t dealer_id = distr(gen);  // determine dealer randomly
        players[dealer_id].dealer_ = true;
    }

    size_t deck_value(const std::vector<Card>& player_deck) const{ // computes the value of a player's deck
        size_t total_deck_value;
        size_t num_aces = 0;

        for (const Card& card : player_deck){

            if (card.type_ != "A"){ // sum up all regular cards
                if (card.type_ != "J" && card.type_ != "Q" && card.type_ != "K"){
                    total_deck_value += std::stoi(card.type_);
                }
                else {
                    total_deck_value += 10; // J, Q, and K are valued 10 each
                }
            }
            else {
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

    size_t next_player_id(const size_t current_player_idx){ // determines the id of the next player
        
        size_t idx = current_player_idx+1;

        while (idx < deck.size()){
            if (!players[idx].dealer_ && players[idx].in_round_){
                return idx;
            }
            else {
                idx++;
            }
        }
        return -1; // if there are no more players with turns to make
    }

    friend std::ostream& operator<<(std::ostream&, const BlackjackGame&);

};

}

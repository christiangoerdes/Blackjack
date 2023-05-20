#pragma once

#include <blackjack.hpp>

namespace Blackjack {

class BlackjackGame{

    public:

    BlackjackGame(){
        balance_ = 1000;
    }

    BlackjackGame(const size_t balance){
        balance_ = balance;
    }

    bool place_bet(const size_t player_id, const size_t balance){

    }

    bool add_card(const size_t player_id){
        if (deck.size() > 0) { // if there are still cards left in the deck
            players[player_id].player_deck.push_back(deck.back());
            deck.pop_back();
        }
        else {
            return false; // no cards left to distribute
        }
    }

    void join(const size_t player_id){

    }

    void leave(const size_t player_id){

    }

    private:

    

    void fill_deck(){
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
        std::vector<Card> player_deck;
    };

    size_t deck_value(std::vector<Card> player_deck){
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

    size_t balance_;
    std::vector<Card> deck;
    std::map<size_t, Player> players;
    friend std::ostream& operator<<(std::ostream&, const BlackjackGame&);

};

}

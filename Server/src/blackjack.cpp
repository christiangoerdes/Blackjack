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
        if (deck.size() > 0) {
            players[player_id].player_deck.push_back(deck.back());
            deck.pop_back();
        }
        else {
            return false;
        }
    }

    void join(const size_t player_id){

    }

    void leave(const size_t player_id){

    }

    private:

    struct Card;
    struct Player {
        std::vector<Card> player_deck;
    };
    size_t balance_;
    std::vector<Card> deck;
    std::map<size_t, Player> players;
    friend std::ostream& operator<<(std::ostream&, const BlackjackGame&);

};

}

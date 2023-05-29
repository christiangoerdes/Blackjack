#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

#include "../include/blackjack.hpp"
#include "../include/card.hpp"
#include "../include/player.hpp"


Blackjack::BlackjackGame::BlackjackGame(){
    _init_balance = 1000;
    _game_state = 0;
}

Blackjack::BlackjackGame::BlackjackGame(int init_balance){
    _init_balance = init_balance;
    _game_state = 0;
}

/// Game State 0: Not Started

bool Blackjack::BlackjackGame::start_round() {

    if (_game_state != 0 || _players.size() == 0) { // if game already running or no players present
        return false;
    }

    fill_deck();
    _game_state = 1; // set state to "placing bets"
    _turn = 0;
    for (Player& player : _players) {
        player._in_round = true; // all players are initially in the game
        player._deck.clear();
    }
    dealer._deck.clear();

    return true;
}

bool Blackjack::BlackjackGame::join(std::string name, std::string password) {

    if (_game_state != 0) { // if game already running
        return false;
    }

    for (const Player& player : _players) {
        if (player._name == name) {
            return false; // no same-name players allowed
        }
    }

    _players.push_back(Player(name, password, _init_balance)); // add player to list of players
    return true;
}

bool Blackjack::BlackjackGame::leave(std::string name, std::string password) {

    if (_game_state != 0) { // if game already running
        return false;
    }

    for (int player_idx = 0; player_idx < _players.size(); player_idx++) {
        Player& player = _players[player_idx];
        if (player._name == name && player._password == password){ // check credentials
            _players.erase(_players.begin() + player_idx); // remove the player from the list of players
            return true;
        }
    }
    return false; // if player not in players list
}

/// Game State 1: Placing Bets

bool Blackjack::BlackjackGame::place_bet(std::string name, std::string password, int bet) {

    if (_game_state != 1) { // if game state not "placing bets"
        return false;
    }

    Player& current_player = _players[_turn];

    if (current_player._name != name || current_player._password != password) { // check credentials
        return false;
    }

    if (bet < _MIN_BET || bet > current_player._balance) { // if bet is too low or too high to afford
        return false;
    }

    current_player._bet = bet; // place the bet
    _turn++;

    if (_turn == _players.size()) { // if the last player has placed their bet

        for (Player& player : _players) { // each player gets two cards
            hand_card(player);
            hand_card(player);
            check_reward(player); // check if player has already won
        }

        hand_card(dealer); // dealer gets two cards
        hand_card(dealer);

        if (players_left()) { // if there are any players who have not won immediately
            _game_state = 2; // set game state to "drawing cards"
        }
        else {
            _game_state = 0; // set game state to "not started"
        }
        _turn = 0;

    }
    return true;
}

/// Game State 2: Drawing Cards

bool Blackjack::BlackjackGame::draw(std::string name, std::string password) {

    if (_game_state != 2) { // if game state is not "drawing cards"
        return false;
    }

    Player& current_player = _players[_turn]; // player whose turn it is

    if (current_player._name == name && current_player._password == password) { // check credentials

        hand_card(current_player);
        check_reward(current_player); // check if player has won or lost

        if (_turn == _players.size()) { // if all players have drawn their cards

            if (players_left()) { // if there are any players still in the round

                draw_dealer(); // dealer draws their cards
            }

            _game_state = 0; // set game state to "not started"
        }
    }

    return true;
}

bool Blackjack::BlackjackGame::skip(std::string name, std::string password) {

    if (_game_state != 2) { // if game state is not "drawing cards"
        return false;
    }

    Player& current_player = _players[_turn]; // player whose turn it is

    if (current_player._name == name && current_player._password == password) { // check credentials

        _turn = next_player();

        if (_turn == _players.size()) { // if all players have drawn their cards
            draw_dealer(); // dealer draws their cards
        }
    }

    return true;
}

int Blackjack::BlackjackGame::getInitBalance() const {
    return _init_balance;
}

int Blackjack::BlackjackGame::getGameState() const {
    return _game_state;
}

int Blackjack::BlackjackGame::getTurn() const {
    return _turn;
}

int Blackjack::BlackjackGame::getMinBet() const {
    return _MIN_BET;
}

std::vector<Blackjack::Card> Blackjack::BlackjackGame::getDeck() const {
    return _deck;
}

std::vector<Blackjack::Player> Blackjack::BlackjackGame::getPlayers() const {
    return _players;
}

Blackjack::Player Blackjack::BlackjackGame::getDealer() const {
    return dealer;
}

void Blackjack::BlackjackGame::fill_deck() {
    _deck.clear(); // empty current deck
    for (const char& suit : _suits) {
        for (const std::string& type : _card_types) {
            _deck.push_back(Card(suit, type));
        }
    }
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(_deck), std::end(_deck), rng); // shuffle the deck
}

void Blackjack::BlackjackGame::hand_card(Player& player) {
    player._deck.push_back(_deck.back());
    _deck.pop_back();
}

int Blackjack::BlackjackGame::deck_value(const std::vector<Card>& deck) const {
    int total_value = 0;
    int num_aces = 0;

    for (const Card& card : deck){

        if (card._type != "A"){ // sum up all "regular" cards (aces are dealt with separately)
            if (card._type == "J" || card._type == "Q" || card._type == "K"){
                total_value += 10; // J, Q, and K are valued 10 each
            }
            else {
                total_value += std::stoi(card._type); // face value
            }
        }
        else {
            num_aces += 1;
        }
    }

    if (num_aces > 0){
        if (total_value + 11 + (num_aces-1) <= 21){ // if it is profitable to count one ace as having value 11
            total_value += 11 + (num_aces-1);
        }
        else {
            total_value += num_aces; // otherwise count all aces as having value 1
        }
    }

    return total_value;
}

void Blackjack::BlackjackGame::reward(Player& winner) {
    winner._balance += winner._bet*2;
    winner._in_round = false;
}

void Blackjack::BlackjackGame::lose(Player& loser) {
    loser._balance -= loser._bet;
    loser._in_round = false;
}

bool Blackjack::BlackjackGame::players_left() const{
    for (const Player& player : _players) {
        if (player._in_round) {
            return true;
        }
    }
    return false;
}

void Blackjack::BlackjackGame::check_reward(Player& player) {

    int value = deck_value(player._deck);

    if (value >= 21) { // player wins/loses and leaves the game

        if (value == 21) {
            reward(player);
        }
        else {
            lose(player);
        }

        _turn = next_player();
    }
}

int Blackjack::BlackjackGame::next_player() const {
    int turn = _turn;
    do {
        turn++;
    }
    while (turn < _players.size() && !_players[turn]._in_round);
    return turn;
}

void Blackjack::BlackjackGame::draw_dealer() {
    while (deck_value(dealer._deck) < 17) { // dealer must draw as long as his value is below 17
        hand_card(dealer);
    }

    int dealer_value = deck_value(dealer._deck);

    for (Player& player : _players) {

        if (!player._in_round) { // if player not in round
            continue;
        }

        int player_value = deck_value(player._deck);

        if (player_value > dealer_value || dealer_value > 21) { // if the dealer has lost or the player has more value
            reward(player);
        }
        else {
            lose(player);
        }
    }
}

Blackjack::Card::Card(char suit, std::string type) {
    this->_suit = suit;
    this->_type = type;
}


Blackjack::Player::Player(std::string name, std::string password, int init_balance) {
    this->_name = name;
    this->_password = password;
    this->_balance = init_balance;
}

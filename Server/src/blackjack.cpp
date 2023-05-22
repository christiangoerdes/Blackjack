
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

namespace Blackjack {

class BlackjackGame {

public:

    BlackjackGame(){
        _init_balance = 1000;
        _game_state = 0;
        _players = std::vector<Player>();
    }

    BlackjackGame(const int balance){
        _init_balance = balance;
        _game_state = 0;
        _players = std::vector<Player>();
    }
    

    /// Game State 0: Not Started

    /**
     * @brief Starts a new game round
     * @return Feedback whether call was valid
    */
    bool start_round() {

        if (_game_state != 0 || _players.size() == 0) { // if game already running or no players present
            return false;
        }

        fill_deck();
        _game_state = 1; // set state to "placing bets"
        _turn = 0;
        for (Player& player : _players) {
            player._in_round = true; // all players are initially in the game
        }

    }

    /**
     * @brief Adds a new player to the game
     * @return Feedback whether call was valid
    */
    bool join(std::string name, std::string password) {

        if (_game_state != 0) { // if game already running
            return false;
        }

        for (const Player& player : _players) {
            if (player._name == name) {
                return false; // no same-name players allowed
            }
        }

        _players.push_back(Player{name, password, _init_balance}); // add player to list of players
        return true;
    }

    /**
     * @brief Player leaves the game
     * @return Feedback whether call was valid
    */
    bool leave(std::string name, std::string password) {

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

    /**
     * @brief Places bet of player whose turn it is
     * @param name Name of the players
     * @param password Player's password
     * @param bet Size of the bet (in dollars, integers)
     * @return Feedback whether call was valid
    */
    bool place_bet(std::string name, std::string password, int bet) {

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

        if (_turn == _players.size()-1) { // if the last player has placed their bet

            for (Player& player : _players) { // each player gets two cards
                hand_card(player);
                hand_card(player);
                check_reward(player);
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
        else {
            _turn++; // it is the next player's turn now
        }
        return true;
    }

    /// Game State 2: Drawing Cards

    /**
     * @brief Current player draws a card
     * @return Feedback whether call was valid
    */
    bool draw(std::string name, std::string password) {

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

    }

    /**
     * @brief Lets players voluntarily skip their turn drawing cards
     * @return Feedback whether call was valid
    */
    bool skip(std::string name, std::string password) {

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

    }



private:

    struct Card {
        char _suit;
        std::string _type;
    };

    struct Player {
        std::string _name;
        std::string _password;
        int _balance;
        std::vector<Card> _deck;
        int _bet;
        bool _in_round;
    };

    /**
     * @brief Fills the game deck with all 52 cards in random order
    */
    void fill_deck() {
        _deck.clear(); // empty current deck
        for (const char& suit : _suits){
            for (const std::string& type : _card_types){
                _deck.push_back(Card{suit, type});
            }
        }
        auto rng = std::default_random_engine {};
        std::shuffle(std::begin(_deck), std::end(_deck), rng); // shuffle the deck
    }

    /**
     * @brief Hands player a card from the game deck
    */
    void hand_card(Player& player) {
        player._deck.push_back(_deck.back());
        _deck.pop_back();
    }

    

    /**
     * @brief Computes the value of a player's deck
     * @return Numerical value of the player's deck
    */
    int deck_value(const std::vector<Card>& deck) const {
        int total_value;
        int num_aces = 0;

        for (const Card& card : deck){

            if (card._type != "A"){ // sum up all "regular" cards (aces are dealt with separately)
                if (card._type == "J" || card._type == "Q" || card._type == "K"){
                    total_value += 10; // J, Q, and K are valued 10 each
                }
                else {
                    total_value += std::stoi(card._type);
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

    /**
     * @brief Rewards a winning player with twice their bet
    */
    void reward(Player& winner) {
        winner._balance += winner._bet*2;
        winner._in_round = false;
    }

    /**
     * @brief Losing player loses their bet
    */
    void lose(Player& loser) {
        loser._balance -= loser._bet;
        loser._in_round = false;
    }

    /**
     * @brief Indicates whether any players are left in the round
     * @return True iff any players are still in the round
    */
    bool players_left() {
        for (const Player& player : _players) {
            if (player._in_round) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Checks whether player has won or lost. Updates turn variable and makes transactions where possible
    */
    void check_reward(Player& player) {
        
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

    /**
     * @brief Determine id of the next player to make a turn
     * @return id of next player to make a turn
    */
    int next_player() {
        int turn = _turn;
        do {
            turn++;
        }
        while (turn < _players.size() && !_players[turn]._in_round);
        return turn;
    }

    /**
     * @brief Dealer draws cards after all players have drawn theirs
    */
    void draw_dealer() {
        while (deck_value(dealer._deck) < 17) { // dealer must draw as long as his value is below 17
            hand_card(dealer);
        }

        int dealer_value = deck_value(dealer._deck);

        for (Player& player : _players) {

            int player_value = deck_value(player._deck);

            if (player_value > dealer_value || dealer_value > 21) { // if the dealer has lost or the player has more value
                reward(player);
            }
            else {
                lose(player);
            }

        }
    }
    

    int _init_balance;
    int _game_state;
    int _turn;
    int _MIN_BET = 5;
    std::vector<Card> _deck;
    std::vector<Player> _players;
    Player dealer;
    const std::vector<std::string> _card_types{"2", "3", "4", "5", "6", "7", "8", "9", "10", "A", "J", "Q", "K"}; // 13 types of cardes
    const std::vector<char> _suits{'C','S','D','H'}; // 4 types of suits: clubs, spades, diamonds and hearts



};


}
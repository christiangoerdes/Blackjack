#include <gtest/gtest.h>
#include "../include/blackjack.hpp"
#include "../include/player.hpp"

using namespace Blackjack;

TEST(BlackjackGameTest, GameTest) {

    BlackjackGame game;
    
    ASSERT_EQ(game.getGameState(), 0);      // game state is initialized with 0 ("game not started")
    ASSERT_FALSE(game.start_round());       // cannot start a round with no players
    ASSERT_TRUE(game.join("Player1", "password1"));     // players can join before game has started
    ASSERT_EQ(game.getPlayers().size(), 1);      // players are added to the players list
    ASSERT_TRUE(game.join("Player2", "password2"));      // second player can join
    ASSERT_EQ(game.getPlayers().size(), 2);      // more players can join
    ASSERT_FALSE(game.leave("Player2", "abcd"));        // valid credentials required to perform player operations
    ASSERT_TRUE(game.leave("Player2", "password2"));      // player can leave with valid credentials
    ASSERT_EQ(game.getPlayers().size(), 1);

    game.join("Player2", "password2");
    ASSERT_FALSE(game.place_bet("Player1", "password1", 10));       // placing bets not allowed before game has started
    game.start_round();
    ASSERT_EQ(game.getGameState(), 1);      // after starting round, game state is changed to 1 ("placing bets")
    ASSERT_EQ(game.getTurn(), 0);       // the first player in the list must place a bet
    ASSERT_FALSE(game.place_bet("Player2", "password2", 10));       // only players whose turn it is can take actions
    game.place_bet("Player1", "password1", 10);
    ASSERT_EQ(game.getPlayers()[0]._bet, 10);       // the bet is saved internally
    game.place_bet("Player2", "password2", 20);


}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

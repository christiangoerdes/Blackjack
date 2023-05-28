//#include <gtest/gtest.h>
//#include "../src/blackjack.cpp"
//
//using namespace Blackjack;
//
//TEST(BlackjackGameTest, StartRound) {
//BlackjackGame game;
//ASSERT_TRUE(game.start_round());
//}
//
//TEST(BlackjackGameTest, Join) {
//BlackjackGame game;
//ASSERT_TRUE(game.join("Player1", "password1"));
//}
//
//TEST(BlackjackGameTest, Leave) {
//BlackjackGame game;
//game.join("Player1", "password1");
//ASSERT_TRUE(game.leave("Player1", "password1"));
//}
//
//TEST(BlackjackGameTest, PlaceBet) {
//BlackjackGame game;
//game.join("Player1", "password1");
//ASSERT_TRUE(game.place_bet("Player1", "password1", 10));
//}
//
//TEST(BlackjackGameTest, Draw) {
//BlackjackGame game;
//game.join("Player1", "password1");
//game.place_bet("Player1", "password1", 10);
//ASSERT_TRUE(game.draw("Player1", "password1"));
//}
//
//TEST(BlackjackGameTest, Skip) {
//BlackjackGame game;
//game.join("Player1", "password1");
//game.place_bet("Player1", "password1", 10);
//ASSERT_TRUE(game.skip("Player1", "password1"));
//}
//
//TEST(BlackjackGameTest, ToJson) {
//BlackjackGame game;
//game.join("Player1", "password1");
//game.join("Player2", "password2");
//
//nlohmann::json expectedJson;
//expectedJson["_init_balance"] = 1000;
//expectedJson["_game_state"] = 0;
//expectedJson["_turn"] = 0;
//expectedJson["_MIN_BET"] = 5;
//expectedJson["_deck"] = nlohmann::json::array();
//expectedJson["_players"] = nlohmann::json::array();
//expectedJson["dealer"]["_name"] = "";
//expectedJson["dealer"]["_password"] = "";
//expectedJson["dealer"]["_balance"] = 0;
//expectedJson["dealer"]["_deck"] = nlohmann::json::array();
//expectedJson["dealer"]["_bet"] = 0;
//expectedJson["dealer"]["_in_round"] = false;
//
//nlohmann::json actualJson = game.to_json();
//ASSERT_EQ(expectedJson, actualJson);
//}
////TODO fix tests
//
//int main(int argc, char **argv) {
//    testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}

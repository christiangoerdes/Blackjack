#include "../src/blackjack.cpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


namespace py = pybind11;
using namespace Blackjack;

PYBIND11_MODULE(blackjack, m) {
    py::class_<Card>(m, "Card")
        .def(py::init<char, std::string>())
        .def_readwrite("_type", &Card::_type)
        .def_readwrite("_suit", &Card::_suit);

    py::class_<Player>(m, "Player")
        .def(py::init<std::string, std::string, int>())
        .def_readwrite("_name", &Player::_name)
        .def_readwrite("_password", &Player::_password)
        .def_readwrite("_balance", &Player::_balance)
        .def_readwrite("_deck", &Player::_deck)
        .def_readwrite("_bet", &Player::_bet)
        .def_readwrite("_in_round", &Player::_in_round)
        .def_readwrite("_deck_value", &Player::_deck_value);

    py::class_<BlackjackGame>(m, "BlackjackGame")
        .def(py::init())
        .def(py::init<int>())
        .def("start_round", &BlackjackGame::start_round)
        .def("join", &BlackjackGame::join)
        .def("leave", &BlackjackGame::leave)
        .def("place_bet", &BlackjackGame::place_bet)
        .def("draw", &BlackjackGame::draw)
        .def("skip", &BlackjackGame::skip)
        .def("getInitBalance", &BlackjackGame::getInitBalance)
        .def("getGameState", &BlackjackGame::getGameState)
        .def("getTurn", &BlackjackGame::getTurn)
        .def("getMinBet", &BlackjackGame::getMinBet)
        .def("getDeck", &BlackjackGame::getDeck)
        .def("getPlayers", &BlackjackGame::getPlayers)
        .def("getDealer", &BlackjackGame::getDealer);


}
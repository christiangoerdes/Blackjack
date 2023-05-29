#include "../src/blackjack.cpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../include/json.hpp"

namespace py = pybind11;
using namespace Blackjack;
namespace json = nlohmann; // Add an alias for the json namespace

PYBIND11_MODULE(blackjack, m) {
    py::class_<Player>(m, "Player")
        .def_readonly("name", &Player::name)
        .def_readonly("balance", &Player::balance)
        .def_readonly("hand", &Player::hand);
    py::class_<BlackjackGame>(m, "BlackjackGame")
        .def(py::init<>())
        .def(py::init<int>())
        .def("start_round", &BlackjackGame::start_round)
        .def("join", &BlackjackGame::join)
        .def("leave", &BlackjackGame::leave)
        .def("place_bet", &BlackjackGame::place_bet)
        .def("draw", &BlackjackGame::draw)
        .def("skip", &BlackjackGame::skip)
        .def("getInitBalance", &BlackjackGame::getInitBalance)
        .def("getGameState", &BlackjackGame::getGameState)
        .def("getPlayers", &BlackjackGame::getPlayers)
        .def("getDealer", &BlackjackGame::getDealer, py::return_value_policy::reference);
}
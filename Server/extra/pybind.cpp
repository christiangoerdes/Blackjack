#include "../src/blackjack.cpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../include/json.hpp"

namespace py = pybind11;
using namespace Blackjack;
namespace json = nlohmann; // Add an alias for the json namespace

PYBIND11_MODULE(blackjack, m) {
    py::class_<BlackjackGame>(m, "BlackjackGame")
        .def(py::init<>())
        .def(py::init<int>())
        .def("start_round", &BlackjackGame::start_round, "Starts a new game round")
        .def("join", &BlackjackGame::join, "Adds a new player to the game")
        .def("leave", &BlackjackGame::leave, "Player leaves the game")
        .def("place_bet", &BlackjackGame::place_bet, "Places bet of player whose turn it is")
        .def("draw", &BlackjackGame::draw, "Current player draws a card")
        .def("skip", &BlackjackGame::skip, "Lets players voluntarily skip their turn drawing cards")
        .def("to_json", &BlackjackGame::to_json, "Converts Object to json data format");
}


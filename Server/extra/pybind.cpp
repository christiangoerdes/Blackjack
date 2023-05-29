#include "../src/blackjack.cpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


namespace py = pybind11;
using namespace Blackjack;

PYBIND11_MODULE(blackjack, m) {
    py::class_<Card>(m, "Card")
        .def(py::init<char, std::string>());

    py::class_<Player>(m, "Player")
        .def(py::init())
        .def(py::init<std::string, std::string, int>());

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
        .def("getPlayers", &BlackjackGame::getPlayers)
        .def("getDealer", &BlackjackGame::getDealer);
}
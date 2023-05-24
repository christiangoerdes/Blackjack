#include "../include/blackjack.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Blackjack;

PYBIND11_MODULE(blackjack, m) {
    py::class_<BlackjackGame>(m, "BlackjackGame")
            .def(py::init<>())
            .def(py::init<int>())
            .def("start_round", &BlackjackGame::start_round)
            .def("join", &BlackjackGame::join)
            .def("leave", &BlackjackGame::leave)
            .def("place_bet", BlackjackGame::place_bet)
            .def("draw", &BlackjackGame::draw)
            .def("skip", &BlackjackGame::skip)
}
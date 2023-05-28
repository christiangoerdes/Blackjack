## Konfiguration des Buildsystems CMake ##

# Minimale Version des Buildsystems
cmake_minimum_required(VERSION 3.14)
# Name des Projekts
project(Hangman)

## Optionen ##

# Setzte verwendeten C++-Standard auf C++17
set(CMAKE_CXX_STANDARD 17)

# Prüfe ob Heapspeicher gefunden wird,
# der nicht freigegeben wurde
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

# Füge selbsgeschriebene Includes hinzu
include_directories(include)
# Füge externe Includes hinzu
include_directories(external)

set(pybind11_DIR "/home/chrisi/.local/lib/python3.10/site-packages/pybind11/share/cmake/pybind11")
find_package(pybind11 REQUIRED PATHS "${pybind11_DIR}")



## Baue Programme ##

# Baue das Programm 'demo1' aus den genannten Quelldateien
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(demo1
        examples/demo1.cpp
        ${SRC_FILES}
        )

pybind11_add_module(hangman
        examples/demo_pybind.cpp
        ${SRC_FILES}
        )

## Installiere die Bibliothek
install(TARGETS hangman
        COMPONENT python
        LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/extra")


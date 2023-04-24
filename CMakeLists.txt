cmake_minimum_required(VERSION 3.25)

project(2048 
        VERSION 1.0
        DESCRIPTION "A simple 2048 game with bots to play it."
        LANGUAGES CXX)

set(CXX_STANDARD C++17)

add_executable(main src/main.cpp)

add_library(moves STATIC src/moves.cpp src/moves.hpp)
add_library(game STATIC src/game.cpp src/game.hpp)
add_library(bitboard STATIC src/bitboard.cpp src/bitboard.hpp)

target_link_libraries(bitboard PUBLIC game)
target_link_libraries(moves PUBLIC game)

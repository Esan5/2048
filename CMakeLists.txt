cmake_minimum_required(VERSION 3.25)

project(2048 
        VERSION 1.0
        DESCRIPTION "A simple 2048 game with bots to play it."
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(main src/main.cpp)

add_library(moves STATIC src/moves.cpp src/moves.hpp)
add_library(game STATIC src/game.cpp src/game.hpp)
add_library(bitboard STATIC src/bitboard.cpp src/bitboard.hpp)
add_library(monte_carlo STATIC src/monte_carlo.cpp src/monte_carlo.hpp)
add_library(eval STATIC src/eval.cpp src/eval.hpp)
add_library(expectiminimax STATIC src/expectiminimax.cpp src/expectiminimax.hpp)

target_link_libraries(main PUBLIC eval)
target_link_libraries(main PUBLIC game)
target_link_libraries(main PUBLIC moves)
target_link_libraries(main PUBLIC bitboard)
target_link_libraries(main PUBLIC monte_carlo)
target_link_libraries(main PUBLIC expectiminimax)

target_link_libraries(moves PUBLIC bitboard)

target_link_libraries(game PUBLIC bitboard)
target_link_libraries(game PUBLIC moves)
target_link_libraries(game PUBLIC eval)

target_link_libraries(monte_carlo PUBLIC game)
target_link_libraries(monte_carlo PUBLIC eval)
target_link_libraries(monte_carlo PUBLIC bitboard)

target_link_libraries(expectiminimax PUBLIC eval)
target_link_libraries(expectiminimax PUBLIC moves)
target_link_libraries(expectiminimax PUBLIC game)

#include <_types/_uint64_t.h>

#include <iostream>

#include "expectiminimax.hpp"
#include "game.hpp"
#include "moves.hpp"
#include "monte_carlo.hpp"

/**
 * Allows the user to play the game.
 * Mainly for testing purposes.
 */
void play() {
  uint64_t board{0};
  board = game::populateBoard(game::populateBoard(board));
  while (!game::gameOver(board)) {
    std::cout << game::to_string(board) << "\n\n";
    char move;
    std::cin >> move;
    uint64_t new_board;
    switch (move) {
      case 'w':
        new_board = moves::moveUp(board);
        break;

      case 'a':
        new_board = moves::moveLeft(board);
        break;

      case 's':
        new_board = moves::moveDown(board);
        break;

      case 'd':
        new_board = moves::moveRight(board);
        break;

      default:
        break;
    }

    if (board != new_board) {
      board = game::populateBoard(new_board);
    }
  }
}

/**
 * Plays the game using expectiminimax.
 */
void play_exp() {
  uint64_t board{game::populateBoard(game::populateBoard(0))};
  while (!game::gameOver(board)) {
    board = game::populateBoard(moves::move(board, exp::bestMove(board)));
    std::cout << game::to_string(board) << "\n\n";
  }
}

/**
 * Plays the game using MonteCarlo search.
 */
void play_monte() {
    uint64_t board {game::populateBoard(game::populateBoard(0))};

    while (!game::gameOver(board)) {
        board = game::populateBoard(moves::move(board, MonteCarlo::bestMove(board)));
        std::cout << game::to_string(board) << "\n\n";
    }
}

int main() {
  // play_exp();
  play_monte();
  // play();
}

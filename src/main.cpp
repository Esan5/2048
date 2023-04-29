#include <iostream>
#include <vector>
#include <unordered_map>

#include "monte_carlo.hpp"
#include "game.hpp"
#include "bitboard.hpp"
#include "moves.hpp"
#include "expectiminimax.hpp"
#include "eval.hpp"

int main() {
  uint64_t board = game::populateBoard(game::populateBoard(0));

  while (!game::gameOver(board)) {
    moves::type best_move = exp::bestMove(board);

    board = game::populateBoard(moves::move(board, best_move));
    std::cout << game::to_string(board) << "\n\n";
  }

  uint32_t score {0};

  score = score + eval::scoreTable[(board & bitboard::row_1) >> 48];
  score = score + eval::scoreTable[(board & bitboard::row_2) >> 32];
  score = score + eval::scoreTable[(board & bitboard::row_3) >> 16];
  score = score + eval::scoreTable[(board & bitboard::row_4)];

  std::cout << static_cast<int>(score) << "\n";
}

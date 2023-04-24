#include <iostream>
#include <vector>

#include "game.hpp"
#include "bitboard.hpp"
#include "moves.hpp"

int main() {
  std::vector<std::vector<int> > test = {{2, 2, 2, 2}, {1, 2, 3, 4}, {4, 3, 2, 1}, {1, 1, 1, 1}};

  uint64_t board = game::getBoard(test);

  board = moves::moveLeft(board);
  std::cout << game::to_string(board) << '\n' << '\n';
}

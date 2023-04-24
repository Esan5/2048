#include "game.hpp"
#include "bitboard.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

/**
 * Construct a game board from an input board.
 */
uint64_t game::getBoard(std::vector<std::vector<int> > board) {
  uint64_t result = 0;

  for (int i = 0; i < 4; i++) {
    uint64_t row = 0;
    for (int j = 0; j < 4; j++) {
      row |= (board[i][j] << (4 * (3 - j)));
    }
    result |= (row << (16 * (3 - i)));
  }

  return result;
}

/**
 * Returns true if there are no more legal moves.
 */
bool game::gameOver(uint64_t board) {
  return true;
}

/**
 * A method to return a string representation of the board.
 */
std::string game::to_string(uint64_t board) {
  std::stringstream stream;

  stream << std::setw(4) << std::setfill('0') << std::hex << ((board & bitboard::row_1) >> 48) << '\n';
  stream << std::setw(4) << std::setfill('0') << std::hex << ((board & bitboard::row_2) >> 32) << '\n';
  stream << std::setw(4) << std::setfill('0') << std::hex << ((board & bitboard::row_3) >> 16) << '\n';
  stream << std::setw(4) << std::setfill('0') << std::hex << (board & bitboard::row_4);

  return stream.str();
}

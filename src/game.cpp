#include "game.hpp"
#include "bitboard.hpp"
#include "moves.hpp"
#include "eval.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <random>

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
  return (board == moves::moveLeft(board))  &&
         (board == moves::moveRight(board)) &&
         (board == moves::moveUp(board))    &&
         (board == moves::moveDown(board));
}

/**
 * Returns a uint64_t with F for currently open spaces.
 */
uint64_t game::openSpaces(uint64_t board) {
  board |= (board >> 1) & (0x7777777777777777);
  board |= (board << 1) & (0xEEEEEEEEEEEEEEEE);

  board |= (board >> 2) & (0x3333333333333333);
  board |= (board << 2) & (0xCCCCCCCCCCCCCCCC);
  return board ^ bitboard::full;
}

/**
 * Returns the number of filled tiles on the board. Can be used with game::openSpaces to find the number of open tiles on the board.
 */
uint8_t game::countTiles(uint64_t board) {
  return (4 - eval::zerosTable[(board & bitboard::row_1) >> 48]) +
         (4 - eval::zerosTable[(board & bitboard::row_2) >> 32]) +
         (4 - eval::zerosTable[(board & bitboard::row_3) >> 16]) +
         (4 - eval::zerosTable[(board & bitboard::row_4)]);
}

/**
 * Returns the maximum value tile on a board.
 */
uint8_t game::maxTile(uint64_t board) {
  if (!board) return 0;

  uint8_t result = 0;

  while (board) {
    if ((board & 0xF) > result)
      result = board & 0xF;
    board = board >> 4;
  }

  return result;
}

/**
 * Add a new tile to the board based on the rules of 2048.
 */
uint64_t game::populateBoard(uint64_t board) {
  uint64_t open = game::openSpaces(board);

  // If the board doesn't have any open spaces, just return
  if (!open)
    return board;

  // Get the number of open spaces
  uint8_t numSpaces = game::countTiles(open);

  std::random_device rd;
  std::mt19937 gen(rd());

  // Determine the location of the new tile
  std::uniform_int_distribution<> place(1, numSpaces);

  // Determine the value of the new tile
  std::uniform_int_distribution<> value(0, 9);

  uint8_t toFill = place(gen);

  // 10% of the time should be 4, 90% of the time should be 2
  uint64_t newTile = value(gen) ? 0x1 : 0x2;

  for (;;) {
    while (!(open & 0xF)) {
      open = open >> 4;
      newTile = newTile << 4;
    }

    // We have found an open position in the board
    toFill--;

    // We are at the correct index
    if (!toFill) {
      return board | newTile;
    }

    // We need to shift again for this index
    open = open >> 4;
    newTile = newTile << 4;
  }
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

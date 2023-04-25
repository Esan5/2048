#include "moves.hpp"
#include "bitboard.hpp"

/**
 * Makes a left movement on the board.
 */
uint64_t moves::moveLeft(uint64_t board) {
  return ((uint64_t) moves::moveTableLeft[(board & bitboard::row_1) >> 48]) << 48  |
         ((uint64_t) moves::moveTableLeft[(board & bitboard::row_2) >> 32]) << 32  |
         ((uint64_t) moves::moveTableLeft[(board & bitboard::row_3) >> 16]) << 16  |
         ((uint64_t) moves::moveTableLeft[(board & bitboard::row_4)]);
}

/**
 * Makes a right movement on the board.
 */
uint64_t moves::moveRight(uint64_t board) {
  return ((uint64_t) moves::moveTableRight[(board & bitboard::row_1) >> 48]) << 48  |
         ((uint64_t) moves::moveTableRight[(board & bitboard::row_2) >> 32]) << 32  |
         ((uint64_t) moves::moveTableRight[(board & bitboard::row_3) >> 16]) << 16  |
         ((uint64_t) moves::moveTableRight[(board & bitboard::row_4)]);
}

/**
 * Makes an up movement on the board.
 */
uint64_t moves::moveUp(uint64_t board) {
  return bitboard::counterRotate(moves::moveRight(bitboard::rotate(board)));
}

/**
 * Makes a down movement on the board.
 */
uint64_t moves::moveDown(uint64_t board) {
  return bitboard::counterRotate(moves::moveLeft(bitboard::rotate(board)));
}

uint64_t moves::move(uint64_t board, moves::type move) {
  switch (move) {
    case moves::type::UP:
      return moves::moveUp(board);
    case moves::type::DOWN:
      return moves::moveDown(board);
    case moves::type::LEFT:
      return moves::moveLeft(board);
    case moves::type::RIGHT:
      return moves::moveRight(board);
    default:
      return board;
  }
}

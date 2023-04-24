#include "moves.hpp"
#include "bitboard.hpp"

/**
 * Makes an up movement on the board.
 */
uint64_t moves::moveUp(uint64_t board) {
  return bitboard::rotate(moves::moveLeft(bitboard::counterRotate(board)));
}

/**
 * Makes a left movement on the board.
 */
uint64_t moves::moveLeft(uint64_t board) {
  return (uint64_t) moves::condenseRow((board & bitboard::row_1) >> 48) << 48 |
         (uint64_t) moves::condenseRow((board & bitboard::row_2) >> 32) << 32 |
                    moves::condenseRow((board & bitboard::row_3) >> 16) << 16 |
                    moves::condenseRow((board & bitboard::row_4));
}

/**
 * Makes a right movement on the board.
 */
uint64_t moves::moveRight(uint64_t board) {
  return bitboard::flipHorizontal(moves::moveLeft(bitboard::flipHorizontal(board)));
}

/**
 * Makes a down movement on the board.
 */
uint64_t moves::moveDown(uint64_t board) {
  return bitboard::counterRotate(moves::moveLeft(bitboard::rotate(board)));
}

/**
 * Condenses a row towards the left.
 */
uint16_t moves::condenseRow(uint16_t row) {
  if (!row) return row;

  uint16_t result = 0;

  while (!(row & 0xF000)) {
    row = row << 4;
  }

  while (!(row & 0x0F00) && (row & 0x00FF)) {
    row = (row & 0xF000) | ((row & 0x00FF) << 4);
  }

  if (!(row & 0x00F0))
    row = (row & 0xFF00) | ((row & 0x000F) << 4);

  if ((row & 0xF000) == ((row & 0x0F00) << 4)) {
    result |= (row & 0xF000) + 0x1000;

    if ((row & 0x00F0) == ((row & 0x000F) << 4)) {
      result |= ((row & 0x00F0) + 0x0010) << 4;
    } else {
      result |= (row & 0x00FF) << 4;
    }

  } else if ((row & 0x0F00) == ((row & 0x00F0) << 4)) {
    result |= (row & 0xF000) | ((row & 0x0F00) + 0x0100) | ((row & 0x000F) << 4);
  } else if ((row & 0x00F0) == ((row & 0x000F) << 4)) {
    result |= (row & 0xFF00) | ((row & 0x00F0) + 0x0010);
  } else {
    result = row;
  }

  return result;
}

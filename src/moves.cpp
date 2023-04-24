#include "moves.hpp"
#include "bitboard.hpp"

/**
 * Makes an up movement on the board.
 */
uint64_t moves::moveUp(uint64_t board) {
  return bitboard::counterRotate(moves::moveRight(bitboard::rotate(board)));
}

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
 * Makes a down movement on the board.
 */
uint64_t moves::moveDown(uint64_t board) {
  return bitboard::counterRotate(moves::moveLeft(bitboard::rotate(board)));
}

/**
 * Condenses a row towards the left.
 */
constexpr uint16_t moves::condenseRow(uint16_t row) {
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

  if ((row < 0xF000) && (row & 0xF000) == ((row & 0x0F00) << 4)) {
    result |= (row & 0xF000) + 0x1000;

    if ((row & 0x00F0) != 0x00F0 && (row & 0x00F0) == ((row & 0x000F) << 4)) {
      result |= ((row & 0x00F0) + 0x0010) << 4;
    } else {
      result |= (row & 0x00FF) << 4;
    }

  } else if ((row & 0x0F00) != 0x0F00 && (row & 0x0F00) == ((row & 0x00F0) << 4)) {
    result |= (row & 0xF000) | ((row & 0x0F00) + 0x0100) | ((row & 0x000F) << 4);
  } else if ((row & 0x00F0) != 0x00F0 && (row & 0x00F0) == ((row & 0x000F) << 4)) {
    result |= (row & 0xFF00) | ((row & 0x00F0) + 0x0010);
  } else {
    result = row;
  }

  return result;
}

/**
 * Initialize a move table for the left movement
 */
void moves::generateMovesLeft() {
  for (uint16_t i = 0; i < 0xFFFF; i++)
    moves::moveTableLeft[i] = moves::condenseRow(i);
}

/**
 * Initialize the move table for right movement
 */
void moves::generateMovesRight() {
  for (uint16_t i = 0; i < 0xFFFF; i++) {
    uint16_t j = ((i & 0xF000) >> 12) |
                 ((i & 0x0F00) >> 4)  |
                 ((i & 0x00F0) << 4)  |
                 ((i & 0x000F) << 12);
    uint16_t res_j = moves::moveTableLeft[j];
    moves::moveTableRight[i] = ((res_j & 0xF000) >> 12) |
                               ((res_j & 0x0F00) >> 4)  |
                               ((res_j & 0x00F0) << 4)  |
                               ((res_j & 0x000F) << 12);
  }

}

/**
 * Initialize the move tables for left and right
 */
void moves::initializeMoveTables() {
  moves::generateMovesLeft();
  moves::generateMovesRight();
}

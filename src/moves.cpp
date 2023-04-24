#include "moves.hpp"
#include "game.hpp"

/**
 * Makes an up movement on the board.
 */
uint64_t moves::moveUp(uint64_t board) {
  return board;
}

/**
 * Makes a left movement on the board.
 */
uint64_t moves::moveLeft(uint64_t board) {
  return board;
}

/**
 * Makes a right movement on the board.
 */
uint64_t moves::moveRight(uint64_t board) {
  return board;
}

/**
 * Makes a down movement on the board.
 */
uint64_t moves::moveDown(uint64_t board) {
  return board;
}

uint16_t moves::condenseRow(uint16_t row) {
  if (!row) return row;

  uint16_t result = 0;

  while (!(row & 0xF000)) {
    row = row << 4;
  }

  if ((row & 0xF000) == ((row & 0x0F00) << 4)) {
    result |= (row & 0xF000) << 1;

    if ((row & 0x00F0) == ((row & 0x000F) << 4)) {
      result |= (row & 0x00F0) << 5;
    } else {
      result |= (row & 0x00FF) << 4;
    }

  } else if ((row & 0x0F00) == ((row & 0x00F0) << 4)) {
    result |= (row & 0xFF00) << 1;
  }

  return result;
}

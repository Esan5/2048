#include "../headers/game.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

/**
 * Default constructor
 */
game::game() {
  this->board = 0;
}

/**
 * Initialize a game state from a uint64_t.
 */
game::game(uint64_t board) { this->board = board; }

/**
 * Construct a game board from an input board.
 */
game::game(std::vector<std::vector<int> > board) {
  this->board = 0;

  for (int i = 0; i < 4; i++) {
    uint64_t row = 0;
    for (int j = 0; j < 4; j++) {
      row = row + (board[i][j] << (4 * (3 - j)));
    }
    this->board = this->board + (row << (16 * (3 - i)));
  }
}

/**
 * Destructor
 */
game::~game() {
}

/**
 * Returns the current board as a uint64_t.
 */
uint64_t game::getBoard() { return this->board; }

/**
 * Makes an up movement on the board.
 */
void game::moveUp() {
}

/**
 * Makes a left movement on the board.
 */
void game::moveLeft() {
}

/**
 * Makes a right movement on the board.
 */
void game::moveRight() {
}

/**
 * Makes a down movement on the board.
 */
void game::moveDown() {
}

/**
 * Flips the board vertically such that row 1 becomes row 4, row 2 becomes row 3, and vice versa.
 * This takes eleven operations.
 */
void game::flipVertical() {
  this->board = ((this->board & game::row_1) >> 48) |
                ((this->board & game::row_2) >> 16) |
                ((this->board & game::row_3) << 16) |
                ((this->board & game::row_4) << 48);
}

/**
 * Flips the board horizontally such that column 1 becomes column 4, column 2 becomes column 3, and vice versa. 
 * This takes eleven operations.
 */
void game::flipHorizontal() {
  this->board = ((this->board & game::col_1) >> 12) |
                ((this->board & game::col_2) >>  4) |
                ((this->board & game::col_3) <<  4) |
                ((this->board & game::col_4) << 12);
}

/**
 * Flips the board across the diagonal defined by column index == row index. 
 * This takes nineteen operations.
 */
void game::flipDiagonal() {
  this->board = ((this->board & 0x000000000000F000) << 36) |
                ((this->board & 0x00000000F0000F00) << 24) |
                ((this->board & 0x0000F0000F0000F0) << 12) |
                ((this->board & 0x000F000000000000) >> 36) |
                ((this->board & 0x00F0000F00000000) >> 24) |
                ((this->board & 0x0F0000F0000F0000) >> 12) |
                ((this->board & 0xF0000F0000F0000F));
}

/**
 * Rotates the board 90 degrees clockwise. 
 * This takes 30 operations.
 */
void game::rotate() {
  this->flipVertical();
  this->flipDiagonal();
}

/**
 * Returns true if there are no more legal moves.
 */
bool game::gameOver() {
  return true;
}

/**
 * A method to return a string representation of the board.
 */
std::string game::to_string() {
  std::stringstream stream;

  stream << std::setw(4) << std::setfill('0') << std::hex << ((this->board & game::row_1) >> 48) << '\n';
  stream << std::setw(4) << std::setfill('0') << std::hex << ((this->board & game::row_2) >> 32) << '\n';
  stream << std::setw(4) << std::setfill('0') << std::hex << ((this->board & game::row_3) >> 16) << '\n';
  stream << std::setw(4) << std::setfill('0') << std::hex << (this->board & game::row_4);

  return stream.str();
}

#include "bitboard.hpp"

#include <vector>

/**
 * Flips the board vertically such that row 1 becomes row 4, row 2 becomes row 3, and vice versa.
 */
uint64_t bitboard::flipVertical(uint64_t board) {
  return ((board & bitboard::row_1) >> 48) |
         ((board & bitboard::row_2) >> 16) |
         ((board & bitboard::row_3) << 16) |
         ((board & bitboard::row_4) << 48);
}

/**
 * Flips the board horizontally such that column 1 becomes column 4, column 2 becomes column 3, and vice versa.
 */
uint64_t bitboard::flipHorizontal(uint64_t board) {
  return ((board & bitboard::col_1) >> 12) |
         ((board & bitboard::col_2) >> 4) |
         ((board & bitboard::col_3) << 4) |
         ((board & bitboard::col_4) << 12);
}

/**
 * Flips the board across the diagonal defined by column index == row index.
 */
uint64_t bitboard::flipDiagonal(uint64_t board) {
  return ((board & 0x000000000000F000) << 36) |
         ((board & 0x00000000F0000F00) << 24) |
         ((board & 0x0000F0000F0000F0) << 12) |
         ((board & 0x000F000000000000) >> 36) |
         ((board & 0x00F0000F00000000) >> 24) |
         ((board & 0x0F0000F0000F0000) >> 12) |
         ((board & 0xF0000F0000F0000F));
}

/**
 * Rotates the board 90 degrees clockwise.
 */
uint64_t bitboard::rotate(uint64_t board) {
  return bitboard::flipDiagonal(bitboard::flipVertical(board));
}

/**
 * Rotates the board 90 degrees counterclockwise.
 */
uint64_t bitboard::counterRotate(uint64_t board) {
  return bitboard::flipVertical(bitboard::flipDiagonal(board));
}

/**
 * Returns all possible rotations and mirrors of a board.
 * This way the transposition table can be filled out for each board.
 *
 * After later testing, I found that the bonus to cache hits for putting all possible orientations
 * into the table was not worth the overhead of generating and storing all possible orientations.
 */
std::vector<uint64_t> bitboard::allFlips(uint64_t board) {
  std::vector<uint64_t> result{};
  result.push_back(board);
  uint64_t transposition = bitboard::flipHorizontal(board);
  result.push_back(transposition);
  uint64_t diag_board = bitboard::flipDiagonal(board);
  uint64_t diag_trans = bitboard::flipDiagonal(transposition);
  result.push_back(diag_board);
  result.push_back(diag_trans);
  result.push_back(bitboard::flipVertical(board));
  result.push_back(bitboard::flipVertical(transposition));
  result.push_back(bitboard::flipHorizontal(diag_board));
  result.push_back(bitboard::flipHorizontal(diag_trans));

  return result;
}

#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <cstdint>
#include <vector>

namespace bitboard {
  // These masks return the specific rows of a board when the & operator is used.
  static constexpr uint64_t row_1{0xFFFF000000000000};
  static constexpr uint64_t row_2{0x0000FFFF00000000};
  static constexpr uint64_t row_3{0x00000000FFFF0000};
  static constexpr uint64_t row_4{0x000000000000FFFF};

  // These masks return the specific columns of a board when the & operator is used.
  static constexpr uint64_t col_1{0xF000F000F000F000};
  static constexpr uint64_t col_2{0x0F000F000F000F00};
  static constexpr uint64_t col_3{0x00F000F000F000F0};
  static constexpr uint64_t col_4{0x000F000F000F000F};

  static constexpr uint64_t full{0xFFFFFFFFFFFFFFFF};

  uint64_t flipVertical(uint64_t board);
  uint64_t flipHorizontal(uint64_t board);

  uint64_t flipDiagonal(uint64_t board);

  uint64_t rotate(uint64_t board);
  uint64_t counterRotate(uint64_t board);

  std::vector<uint64_t> allFlips(uint64_t board);

}  // namespace bitboard

#endif

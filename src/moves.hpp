#ifndef MOVES_HPP
#define MOVES_HPP

#include <cstdint>
#include <array>

namespace moves {
  static uint16_t moveTableLeft[0xFFFF];
  static uint16_t moveTableRight[0xFFFF];

  constexpr uint16_t condenseRow(uint16_t row);

  uint64_t moveDown(uint64_t board);
  uint64_t moveUp(uint64_t board);
  uint64_t moveRight(uint64_t board);
  uint64_t moveLeft(uint64_t board);

  void generateMovesLeft();
  void generateMovesRight();
  void initializeMoveTables();
};  // namespace moves

#endif

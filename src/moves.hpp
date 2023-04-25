#ifndef MOVES_HPP
#define MOVES_HPP

#include <cstdint>
#include <array>

namespace moves {
  uint64_t moveDown(uint64_t board);
  uint64_t moveUp(uint64_t board);
  uint64_t moveRight(uint64_t board);
  uint64_t moveLeft(uint64_t board);

  enum type {
    UP,
    DOWN,
    LEFT,
    RIGHT
  };

  uint64_t move(uint64_t board, moves::type type);

  /**
  * Condenses a row towards the left.
  */
  constexpr uint16_t condenseRow(uint16_t row) {
    // If the row is empty, change nothing
    if (!row) return row;

    uint16_t result = row;

    // Condense the row so that all tiles are as far left as possible
    if (!(result & 0x00F0))
      result = (result & 0xFF00) | ((result & 0x000F) << 4);
    if (!(result & 0x0F00))
      result = (result & 0xF000) | ((result & 0x00FF) << 4);
    if (!(result & 0xF000))
      result = result << 4;

    // Check if the first two tiles match, if not, check the middle tiles
    if ((result & 0xF000) == ((result & 0x0F00) << 4)) {
      result = ((result & 0xF000) | (result & 0x00FF)) + 0x1000;
    } else if ((result & 0x0F00) && (result & 0x0F00) == ((result & 0x00F0) << 4)) {
      // If they match, early return the merged output
      return ((result & 0xFF00) | ((result & 0x000F) << 4)) + 0x0100;
    }

    // Check if the last two tiles match
    if ((result & 0x00F0) && (result & 0x00F0) == ((result & 0x000F) << 4)) {
      result = (result & 0xFFF0) + 0x0010;
    }

    // Make sure there isn't any remaining spaces
    if (!(result & 0x0F00))
      result = (result & 0xF000) | ((result & 0x00FF) << 4);

    return result;
  }

  // These tables are made at compile time to store the results of moves
  constexpr std::array<uint16_t, 0xFFFF> moveTableLeft = [] {
    std::array<uint16_t, 0xFFFF> result {};
    for (uint16_t i = 0; i < 0xFFFF; i++)
      result[i] = condenseRow(i);
    return result;
  } ();

  constexpr std::array<uint16_t, 0xFFFF> moveTableRight = [] {
    std::array<uint16_t, 0xFFFF> result {};
    for (uint16_t i = 0; i < 0xFFFF; i++) {
      uint16_t j = ((i & 0xF000) >> 12) |
                  ((i & 0x0F00) >> 4)  |
                  ((i & 0x00F0) << 4)  |
                  ((i & 0x000F) << 12);
      uint16_t res_j = moveTableLeft[j];
      result[i] = ((res_j & 0xF000) >> 12) |
                  ((res_j & 0x0F00) >> 4)  |
                  ((res_j & 0x00F0) << 4)  |
                  ((res_j & 0x000F) << 12);
    }
    return result;
  } ();

};  // namespace moves

#endif

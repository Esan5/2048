#ifndef EVAL_HPP
#define EVAL_HPP

#include <cstdint>
#include <array>


namespace eval {
  constexpr uint32_t MONOTONICITY_MULTIPLIER = 0;

  uint32_t snakeEval(uint64_t board);
}

namespace eval {
  constexpr uint8_t evalMonotonicityRow(uint16_t row) {
    const uint8_t pos_1 = (row & 0xF000) >> 12;
    const uint8_t pos_2 = (row & 0x0F00) >> 8;
    const uint8_t pos_3 = (row & 0x00F0) >> 4;
    const uint8_t pos_4 = row & 0x000F;

    if (pos_1 <= pos_2 && pos_2 <= pos_3 && pos_3 <= pos_4)
      return 3;

    if (pos_1 >= pos_2 && pos_2 >= pos_3 && pos_3 >= pos_4)
      return 3;
    return 0;
  }

  constexpr std::array<uint8_t, 0xFFFF> monotoneTable = [] {
    std::array<uint8_t, 0xFFFF> result {};
    for (uint16_t i = 0; i < 0xFFFF; i++)
      result[i] = evalMonotonicityRow(i);
    return result;
  } ();

  constexpr uint8_t evalZeroes(uint16_t row) {
    return (!(row & 0xF000) ? 1 : 0) +
           (!(row & 0x0F00) ? 1 : 0) +
           (!(row & 0x00F0) ? 1 : 0) +
           (!(row & 0x000F) ? 1 : 0);
  }

  constexpr std::array<uint8_t, 0xFFFF> zeroesTable = [] {
    std::array<uint8_t, 0xFFFF> result {};
    for (uint16_t i = 0; i < 0xFFFF; i++)
      result[i] = evalZeroes(i);
    return result;
  } ();

  constexpr uint32_t evalScore(uint16_t row) {
    const uint8_t pos_1 = (row & 0xF000) >> 12;
    const uint8_t pos_2 = (row & 0x0F00) >> 8;
    const uint8_t pos_3 = (row & 0x00F0) >> 4;
    const uint8_t pos_4 = row & 0x000F;

    return (pos_1 ? ((pos_1 - 1) * (1 << pos_1)) : 0) +
           (pos_2 ? ((pos_2 - 1) * (1 << pos_2)) : 0) +
           (pos_3 ? ((pos_3 - 1) * (1 << pos_3)) : 0) +
           (pos_4 ? ((pos_4 - 1) * (1 << pos_4)) : 0);
  }

  constexpr std::array<uint32_t, 0xFFFF> scoreTable = [] {
    std::array<uint32_t, 0xFFFF> result {};
    for (uint16_t i = 0; i < 0xFFFF; i++)
      result[i] = evalScore(i);
    return result;
  } ();
}  // namespace eval

#endif

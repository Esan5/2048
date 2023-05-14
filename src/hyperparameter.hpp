#ifndef HYPERPARAMETER_HPP
#define HYPERPARAMETER_HPP

#include <cstdint>
#include <array>

#include "bitboard.hpp"

namespace parameter {

  typedef struct individual {
    float mono;
    float zero;
    float edge;
    float merge;
  } individual;

  static float MONOTONICITY_MULTIPLIER;
  static float ZEROS_MULTIPLIER;
  static float EDGE_MULTIPLIER;
  static float MERGE_MULTIPLIER;

}

namespace parameter {
  static std::array<float, 0xFFFF> heuristicTable;

  constexpr uint32_t evalMonotonicityRow(uint16_t row) {
    const uint8_t pos_1 = (row & 0xF000) >> 12;
    const uint8_t pos_2 = (row & 0x0F00) >> 8;
    const uint8_t pos_3 = (row & 0x00F0) >> 4;
    const uint8_t pos_4 = row & 0x000F;

    if (pos_1 <= pos_2 && pos_2 <= pos_3 && pos_3 <= pos_4)
      return ((1 << pos_1) + (1 << pos_2) + (1 << pos_3) + (1 << pos_4));

    if (pos_1 >= pos_2 && pos_2 >= pos_3 && pos_3 >= pos_4)
      return ((1 << pos_1) + (1 << pos_2) + (1 << pos_3) + (1 << pos_4));
    return 0;
  }

  constexpr std::array<uint32_t, 0xFFFF> monotoneTable = [] {
    std::array<uint32_t, 0xFFFF> result {};
    for (uint16_t i = 0; i < 0xFFFF; i++)
      result[i] = evalMonotonicityRow(i);
    return result;
  } ();

  constexpr uint32_t evalMonotonicity(uint64_t board) {
    uint64_t transpose = bitboard::flipDiagonal(board);
    return  monotoneTable[((board & bitboard::row_1) >> 48)] +
            monotoneTable[((board & bitboard::row_2) >> 32)] +
            monotoneTable[((board & bitboard::row_3) >> 16)] +
            monotoneTable[((board & bitboard::row_4))]       +
            monotoneTable[((transpose & bitboard::row_1) >> 48)] +
            monotoneTable[((transpose & bitboard::row_2) >> 32)] +
            monotoneTable[((transpose & bitboard::row_3) >> 16)] +
            monotoneTable[((transpose & bitboard::row_4))];
  }

  constexpr uint8_t evalZerosRow(uint16_t row) {
    return ((row & 0xF000) ? 0 : 1) +
           ((row & 0x0F00) ? 0 : 1) +
           ((row & 0x00F0) ? 0 : 1) +
           ((row & 0x000F) ? 0 : 1);
  }

  constexpr std::array<uint8_t, 0xFFFF> zerosTable = [] {
    std::array<uint8_t, 0xFFFF> result {};
    for (uint16_t i = 0; i < 0xFFFF; i++)
      result[i] = evalZerosRow(i);
    return result;
  } ();

  constexpr uint32_t evalZeros(uint64_t board) {
    return (zerosTable[((board & bitboard::row_1) >> 48)] +
            zerosTable[((board & bitboard::row_2) >> 32)] +
            zerosTable[((board & bitboard::row_3) >> 16)] +
            zerosTable[((board & bitboard::row_4))]);
  }

  constexpr uint32_t evalScoreRow(uint16_t row) {
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
      result[i] = evalScoreRow(i);
    return result;
  } ();

  constexpr uint32_t evalScore(uint64_t board) {
    return (scoreTable[((board & bitboard::row_1) >> 48)] +
            scoreTable[((board & bitboard::row_2) >> 32)] +
            scoreTable[((board & bitboard::row_3) >> 16)] +
            scoreTable[((board & bitboard::row_4))]);
  }

  constexpr uint8_t evalMergesRow(uint16_t row) {
    return ((((row & 0xF000) >> 4) ^ (row & 0x0F00)) ? 0 : 1) +
           ((((row & 0x0F00) >> 4) ^ (row & 0x00F0)) ? 0 : 1) +
           ((((row & 0x00F0) >> 4) ^ (row & 0x000F)) ? 0 : 1);
  }

  /**
  constexpr std::array<uint8_t, 0xFFFF> mergeTable = [] {
    std::array<uint8_t, 0xFFFF> result {};
    for (uint16_t i = 0; i < 0xFFFF; i++)
      result[i] = evalMergesRow(i);
    return result;
  } ();

  constexpr uint32_t evalMerges(uint64_t board) {
    uint64_t transpose = bitboard::flipDiagonal(board);
    return  mergeTable[((board & bitboard::row_1) >> 48)] +
            mergeTable[((board & bitboard::row_2) >> 32)] +
            mergeTable[((board & bitboard::row_3) >> 16)] +
            mergeTable[((board & bitboard::row_4))]       +
            mergeTable[((transpose & bitboard::row_1) >> 48)] +
            mergeTable[((transpose & bitboard::row_2) >> 32)] +
            mergeTable[((transpose & bitboard::row_3) >> 16)] +
            mergeTable[((transpose & bitboard::row_4))];
  }
  */

  constexpr uint32_t evalEdgesRow(uint16_t row) {
    return (2 << ((row & 0xF000) >> 12)) + (2 << (row & 0xF));
  }

  constexpr std::array<uint32_t, 0xFFFF> edgeTable = [] {
    std::array<uint32_t, 0xFFFF> result {};
    for (uint16_t i = 0; i < 0xFFFF; i++)
      result[i] = evalEdgesRow(i);
    return result;
  } ();

  constexpr uint32_t evalEdges(uint64_t board) {
    uint64_t transpose = bitboard::flipDiagonal(board);

    return  edgeTable[((board & bitboard::row_1) >> 48)] +
            edgeTable[((board & bitboard::row_2) >> 32)] +
            edgeTable[((board & bitboard::row_3) >> 16)] +
            edgeTable[((board & bitboard::row_4))]       +
            edgeTable[((transpose & bitboard::row_1) >> 48)] +
            edgeTable[((transpose & bitboard::row_2) >> 32)] +
            edgeTable[((transpose & bitboard::row_3) >> 16)] +
            edgeTable[((transpose & bitboard::row_4))];
  }

  static std::array<float, 0xFFFF> initializeTable(individual individual);

  float applyHeuristic(uint64_t board, const std::array<float, 0xFFFF> &eval_table);

  float evaluateIndividual(individual individual);

  uint32_t evaluateHueristics(individual individual);

  uint32_t playGame(const std::array<float, 0xFFFF> &eval_table);

}  // namespace parameter

#endif

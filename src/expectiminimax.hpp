#ifndef EXPECTIMINIMAX_HPP
#define EXPECTIMINIMAX_HPP

#include <cstdint>
#include <unordered_map>
#include <shared_mutex>

#include "moves.hpp"

namespace exp {
  constexpr uint8_t MAX_DEPTH = 8;
  constexpr double PROB_THRESH = 0.0001f;

  double stateNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, double> &table, double probability);

  double chanceNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, double> &table, double probability);

  moves::type bestMove(uint64_t board);
}  // namespace exp

#endif

#ifndef EXPECTIMINIMAX_HPP
#define EXPECTIMINIMAX_HPP

#include <cstdint>
#include <shared_mutex>
#include <unordered_map>

#include "moves.hpp"

namespace exp {
  // The maximum depth expectiminimax will search.
  constexpr uint8_t MAX_DEPTH = 8;

  // The minimum probability that states will be expanded to.
  // If a state has a probability of occurring less than this threshold, then it will not be further explored.
  constexpr double PROB_THRESH = 0.0001f;

  double stateNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, double> &table, double probability);

  double chanceNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, double> &table, double probability);

  moves::type bestMove(uint64_t board);
}  // namespace exp

#endif

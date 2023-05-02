#ifndef EXPECTIMINIMAX_HPP
#define EXPECTIMINIMAX_HPP

#include <cstdint>
#include <unordered_map>
#include <shared_mutex>

#include "moves.hpp"

namespace exp {
  constexpr uint8_t MAX_DEPTH = 8;
  constexpr float PROB_THRESH = 0.0001f;

  float stateNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, float> &table, float probability);

  float chanceNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, float> &table, float probability);

  moves::type bestMove(uint64_t board);
}  // namespace exp

#endif

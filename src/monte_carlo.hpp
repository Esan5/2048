#include <cstdint>
#include "moves.hpp"

class MonteCarlo {
  static constexpr uint16_t NUM_TRIALS = 300;

 public:
  static moves::type bestMove(uint64_t board);

 private:
  static uint64_t randomMove(uint64_t board);
  static uint64_t scoreState(uint64_t board);
};

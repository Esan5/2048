#include <cstdint>

namespace moves {
  uint16_t condenseRow(uint16_t row);

  uint64_t moveDown(uint64_t board);
  uint64_t moveUp(uint64_t board);
  uint64_t moveRight(uint64_t board);
  uint64_t moveLeft(uint64_t board);
};  // namespace moves

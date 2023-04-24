#include <cstdint>
#include <string>
#include <vector>

namespace game {
  static constexpr uint64_t row_1{0xFFFF000000000000};
  static constexpr uint64_t row_2{0x0000FFFF00000000};
  static constexpr uint64_t row_3{0x00000000FFFF0000};
  static constexpr uint64_t row_4{0x000000000000FFFF};

  static constexpr uint64_t col_1{0xF000F000F000F000};
  static constexpr uint64_t col_2{0x0F000F000F000F00};
  static constexpr uint64_t col_3{0x00F000F000F000F0};
  static constexpr uint64_t col_4{0x000F000F000F000F};

  static constexpr uint64_t full{0xFFFFFFFFFFFFFFFF};

  uint64_t getBoard(std::vector< std::vector< int > > board);

  bool gameOver(uint64_t board);

  std::string to_string(uint64_t board);
}  // namespace game

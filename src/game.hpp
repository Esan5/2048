#ifndef GAME_HPP
#define GAME_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace game {
  uint64_t getBoard(std::vector< std::vector< int > > board);

  bool gameOver(uint64_t board);

  std::string to_string(uint64_t board);
}  // namespace game

#endif

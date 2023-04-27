#ifndef GAME_HPP
#define GAME_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace game {
  uint64_t getBoard(std::vector< std::vector< int > > board);

  bool gameOver(uint64_t board);

  uint64_t openSpaces(uint64_t board);

  uint8_t countTiles(uint64_t board);

  uint8_t maxTile(uint64_t board);

  uint64_t populateBoard(uint64_t board);

  std::string to_string(uint64_t board);
}  // namespace game

#endif

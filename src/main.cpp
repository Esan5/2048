#include <iostream>
#include <vector>

#include "../headers/game.hpp"

int main() {
  std::vector<std::vector<int> > board = {{2, 2, 2, 2}, {1, 2, 3, 4}, {4, 3, 2, 1}, {1, 1, 1, 1}};

  game g(board);
  std::cout << g.to_string() << '\n' << '\n';

  g.flipVertical();
  std::cout << g.to_string() << '\n' << '\n';

  g.flipHorizontal();
  std::cout << g.to_string() << '\n' << '\n';

  g.rotate();
  std::cout << g.to_string() << '\n' << '\n';
}

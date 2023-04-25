#include "monte_carlo.hpp"
#include "game.hpp"

#include <random>
#include <vector>
#include <future>

uint64_t MonteCarlo::scoreState(uint64_t board) {
  if (game::gameOver(board)) return 0;

  uint64_t score {0};

  for (uint16_t i = 0; i < MonteCarlo::NUM_TRIALS; i++) {
    uint64_t trial = board;
    while (!game::gameOver(trial)) {
      score++;
      trial = game::populateBoard(MonteCarlo::randomMove(trial));
    }
  }

  return score;
}

uint64_t MonteCarlo::randomMove(uint64_t board) {
  if (game::gameOver(board))
    return board;

  std::random_device rd;
  std::mt19937 gen(rd());

  std::vector<uint64_t> next_states {};

  uint64_t left = moves::moveLeft(board);
  uint64_t right = moves::moveRight(board);
  uint64_t up = moves::moveUp(board);
  uint64_t down = moves::moveDown(board);

  if (left ^ board)
    next_states.push_back(left);
  if (right ^ board)
    next_states.push_back(right);
  if (up ^ board)
    next_states.push_back(up);
  if (down ^ board)
    next_states.push_back(down);

  std::uniform_int_distribution<> dist(0, next_states.size() - 1);

  return next_states[dist(gen)];
}

moves::type MonteCarlo::bestMove(uint64_t board) {
  if (game::gameOver(board)) return moves::type::NONE;

  uint64_t left = moves::moveLeft(board);
  uint64_t right = moves::moveRight(board);
  uint64_t down = moves::moveDown(board);
  uint64_t up = moves::moveUp(board);

  std::future<uint64_t> left_future;
  std::future<uint64_t> right_future;
  std::future<uint64_t> up_future;
  std::future<uint64_t> down_future;

  if (left ^ board)
    left_future = std::async(MonteCarlo::scoreState, left);
  if (right ^ board)
    right_future = std::async(MonteCarlo::scoreState, right);
  if (up ^ board)
    up_future = std::async(MonteCarlo::scoreState, up);
  if (down ^ board)
    down_future = std::async(MonteCarlo::scoreState, down);

  uint64_t left_score = left_future.valid() ? left_future.get() : 0;
  uint64_t right_score = right_future.valid() ? right_future.get() : 0;
  uint64_t up_score = up_future.valid() ? up_future.get() : 0;
  uint64_t down_score = down_future.valid() ? down_future.get() : 0;

  moves::type ret = moves::type::LEFT;
  uint64_t best_score = left_score;

  if (right_score > best_score) {
    ret = moves::type::RIGHT;
    best_score = right_score;
  }

  if (up_score > best_score) {
    ret = moves::type::UP;
    best_score = up_score;
  }
  if (down_score > best_score) {
    ret = moves::type::DOWN;
  }

  return ret;
}

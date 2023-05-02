#include "expectiminimax.hpp"
#include "bitboard.hpp"
#include "eval.hpp"
#include "moves.hpp"
#include "game.hpp"

#include <algorithm>
#include <iostream>

moves::type exp::bestMove(uint64_t board) {
  if (game::gameOver(board))
    return moves::type::NONE;

  std::unordered_map<uint64_t, float> table;

  uint64_t left = moves::moveLeft(board);
  uint64_t right = moves::moveRight(board);
  uint64_t down = moves::moveDown(board);
  uint64_t up = moves::moveUp(board);

  float left_score = (left ^ board) ? exp::stateNode(left, exp::MAX_DEPTH, table, 1) : 0;
  float right_score = (right ^ board) ? exp::stateNode(right, exp::MAX_DEPTH, table, 1) : 0;
  float up_score = (up ^ board) ? exp::stateNode(up, exp::MAX_DEPTH, table, 1) : 0;
  float down_score = (down ^ board) ? exp::stateNode(down, exp::MAX_DEPTH, table, 1) : 0;

  std::cout << "States Evaluated: " << table.size() << "\n\n";

  std::cout << "Left Score: " << left_score << "\n";
  std::cout << "Right Score: " << right_score << "\n";
  std::cout << "Down Score: " << down_score << "\n";
  std::cout << "Up Score: " << up_score << "\n\n";

  float best_score = up_score;
  moves::type best_move = moves::type::UP;

  if (right_score > best_score) {
    best_score = right_score;
    best_move = moves::type::RIGHT;
  }
  if (down_score > best_score) {
    best_score = down_score;
    best_move = moves::type::DOWN;
  }
  if (left_score > best_score) {
    best_move = moves::type::LEFT;
  }

  return best_move;
}

float exp::stateNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, float> &table, float probability) {
  auto found = table.find(board);
  if (found != table.end()) {
    return table[board];
  }

  if (game::gameOver(board))
    return 0;

  if (!depth || probability < exp::PROB_THRESH)
    return eval::applyHeuristic(board);

  uint64_t left = moves::moveLeft(board);
  uint64_t right = moves::moveRight(board);
  uint64_t down = moves::moveDown(board);
  uint64_t up = moves::moveUp(board);

  float left_score = (left ^ board) ? exp::chanceNode(left, depth - 1, table, probability) : 0.f;
  float right_score = (right ^ board) ? exp::chanceNode(right, depth - 1, table, probability) : 0.f;
  float up_score = (up ^ board) ? exp::chanceNode(up, depth - 1, table, probability) : 0.f;
  float down_score = (down ^ board) ? exp::chanceNode(down, depth - 1, table, probability) : 0.f;

  float best_score = std::max(std::max(left_score, right_score), std::max(up_score, down_score));

  for (uint64_t transition : bitboard::allFlips(board))
    table[transition] = best_score;

  return best_score;
}

float exp::chanceNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, float> &table, float probability) {
  uint64_t open_spaces = game::openSpaces(board);

  if (!(open_spaces))
    return exp::stateNode(board, depth, table, probability);

  uint8_t num_children { game::countTiles(open_spaces) };
  float prob_two { 0.9f * (1.0f / num_children) };
  float prob_four { 0.1f * (1.0f / num_children) };

  uint64_t new_two = 0x1;
  uint64_t new_four = 0x2;

  float value {0};

  while (open_spaces) {
    if (open_spaces & 0xF) {
      value += prob_two * (exp::stateNode(board | new_two, depth, table, probability * prob_two));
      value += prob_four * (exp::stateNode(board | new_four, depth, table, probability * prob_four));
    }
    new_two = new_two << 4;
    new_four = new_four << 4;
    open_spaces = open_spaces >> 4;
  }

  return value;
}

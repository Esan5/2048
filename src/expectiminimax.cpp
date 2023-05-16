#include "expectiminimax.hpp"

#include <algorithm>
#include <iostream>

#include "bitboard.hpp"
#include "eval.hpp"
#include "game.hpp"
#include "moves.hpp"

moves::type exp::bestMove(uint64_t board) {
  if (game::gameOver(board))
    return moves::type::NONE;

  std::unordered_map<uint64_t, double> table;

  uint64_t left = moves::moveLeft(board);
  uint64_t right = moves::moveRight(board);
  uint64_t down = moves::moveDown(board);
  uint64_t up = moves::moveUp(board);

  double left_score = (left ^ board) ? exp::chanceNode(left, exp::MAX_DEPTH, table, 1) : 0;
  double right_score = (right ^ board) ? exp::chanceNode(right, exp::MAX_DEPTH, table, 1) : 0;
  double up_score = (up ^ board) ? exp::chanceNode(up, exp::MAX_DEPTH, table, 1) : 0;
  double down_score = (down ^ board) ? exp::chanceNode(down, exp::MAX_DEPTH, table, 1) : 0;

  // std::cout << "Left Score: " << left_score << "\n";
  // std::cout << "Right Score: " << right_score << "\n";
  // std::cout << "Down Score: " << down_score << "\n";
  // std::cout << "Up Score: " << up_score << "\n\n";

  double best_score = up_score;
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

  std::cout << "Best Move: ";
  switch (best_move) {
    case moves::type::UP:
      std::cout << "up\n\n";
      break;
    case moves::type::DOWN:
      std::cout << "down\n\n";
      break;
    case moves::type::RIGHT:
      std::cout << "right\n\n";
      break;
    case moves::type::LEFT:
      std::cout << "left\n\n";
      break;
    default:
      break;
  }
  return best_move;
}

double exp::stateNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, double> &table, double probability) {
  if (table.find(board) != table.end()) {
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

  double left_score = (left ^ board) ? exp::chanceNode(left, depth - 1, table, probability) : 0.f;
  double right_score = (right ^ board) ? exp::chanceNode(right, depth - 1, table, probability) : 0.f;
  double up_score = (up ^ board) ? exp::chanceNode(up, depth - 1, table, probability) : 0.f;
  double down_score = (down ^ board) ? exp::chanceNode(down, depth - 1, table, probability) : 0.f;

  double best_score = std::max(std::max(left_score, right_score), std::max(up_score, down_score));

  for (uint64_t transition : bitboard::allFlips(board))
    table[transition] = best_score;

  return best_score;
}

double exp::chanceNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, double> &table, double probability) {
  uint64_t open_spaces = game::openSpaces(board);

  if (!(open_spaces))
    return exp::stateNode(board, depth, table, probability);

  uint8_t num_children{game::countTiles(open_spaces)};
  double prob_two{0.9f * (1.0f / num_children)};
  double prob_four{0.1f * (1.0f / num_children)};

  uint64_t new_two = 0x1;
  uint64_t new_four = 0x2;

  double value{0};

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

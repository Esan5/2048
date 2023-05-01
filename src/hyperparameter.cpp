#include <algorithm>
#include <unordered_map>

#include "hyperparameter.hpp"
#include "game.hpp"
#include "moves.hpp"
#include "eval.hpp"

namespace opt {
  const uint8_t MAX_DEPTH {5};
  const float PROB_THRESH {0.0001};

  float stateNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, float> &table, float probability);
  float chanceNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, float> &table, float probability);


  moves::type bestMove(uint64_t board) {
    if (game::gameOver(board))
      return moves::type::NONE;

    std::unordered_map<uint64_t, float> table;

    uint64_t left = moves::moveLeft(board);
    uint64_t right = moves::moveRight(board);
    uint64_t down = moves::moveDown(board);
    uint64_t up = moves::moveUp(board);

    float left_score = (left ^ board) ? stateNode(left, MAX_DEPTH, table, 1) : 0;
    float right_score = (right ^ board) ? stateNode(right, MAX_DEPTH, table, 1) : 0;
    float up_score = (up ^ board) ? stateNode(up, MAX_DEPTH, table, 1) : 0;
    float down_score = (down ^ board) ? stateNode(down, MAX_DEPTH, table, 1) : 0;

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

  float stateNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, float> &table, float probability) {
    auto found = table.find(board);
    if (found != table.end()) {
      return table[board];
    }

    if (game::gameOver(board))
      return 0;

    if (!depth || probability < PROB_THRESH)
      return parameter::applyHeuristic(board);

    uint64_t left = moves::moveLeft(board);
    uint64_t right = moves::moveRight(board);
    uint64_t down = moves::moveDown(board);
    uint64_t up = moves::moveUp(board);

    float left_score = (left ^ board) ? chanceNode(left, depth - 1, table, probability) : 0.f;
    float right_score = (right ^ board) ? chanceNode(right, depth - 1, table, probability) : 0.f;
    float up_score = (up ^ board) ? chanceNode(up, depth - 1, table, probability) : 0.f;
    float down_score = (down ^ board) ? chanceNode(down, depth - 1, table, probability) : 0.f;

    float best_score = std::max(std::max(left_score, right_score), std::max(up_score, down_score));

    for (uint64_t transition : bitboard::allFlips(board))
      table[transition] = best_score;

    return best_score;
  }

  float chanceNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, float> &table, float probability) {
    uint64_t open_spaces = game::openSpaces(board);

    uint8_t num_children { game::countTiles(open_spaces) };
    float prob_two { 0.9f * (1.0f / num_children) };
    float prob_four { 0.1f * (1.0f / num_children) };

    uint64_t new_two = 0x1;
    uint64_t new_four = 0x2;

    float value {0};

    while (open_spaces) {
      if (open_spaces & 0xF) {
        value += prob_two * (stateNode(board | new_two, depth, table, probability * prob_two));
        value += prob_four * (stateNode(board | new_four, depth, table, probability * prob_four));
      }
      new_two = new_two << 4;
      new_four = new_four << 4;
      open_spaces = open_spaces >> 4;
    }

    return value;
  }
}  // namespace opt

namespace parameter {
  static void initializeTable() {
    for (uint16_t i = 0; i < 0xFFFF; i++) {
      heuristicTable[i] = monotoneTable[i] * MONOTONICITY_MULTIPLIER + zerosTable[i] * ZEROS_MULTIPLIER + edgeTable[i] * EDGE_MULTIPLIER + evalMergesRow(i) * MERGE_MULTIPLIER;
    }
  }

  float applyHeuristic(uint64_t board) {
    uint64_t transpose = bitboard::flipDiagonal(board);

    return  heuristicTable[((board & bitboard::row_1) >> 48)] +
            heuristicTable[((board & bitboard::row_2) >> 32)] +
            heuristicTable[((board & bitboard::row_3) >> 16)] +
            heuristicTable[((board & bitboard::row_4))]       +
            heuristicTable[((transpose & bitboard::row_1) >> 48)] +
            heuristicTable[((transpose & bitboard::row_2) >> 32)] +
            heuristicTable[((transpose & bitboard::row_3) >> 16)] +
            heuristicTable[((transpose & bitboard::row_4))];
  }

  uint32_t evaluateHueristics(float mono, float zero, float edge, float merge) {
    MONOTONICITY_MULTIPLIER = mono;
    ZEROS_MULTIPLIER = zero;
    EDGE_MULTIPLIER = edge;
    MERGE_MULTIPLIER = merge;

    initializeTable();

    return playGame();
  }

  uint32_t playGame() {
    uint64_t board = game::populateBoard(game::populateBoard(0));

    while (!game::gameOver(board)) {
      board = game::populateBoard(moves::move(board, opt::bestMove(board)));
    }

    return eval::evalScore(board);
  }
}  // namespace parameter


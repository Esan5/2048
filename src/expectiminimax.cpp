#include "expectiminimax.hpp"

#include <algorithm>
#include <iostream>

#include "bitboard.hpp"
#include "eval.hpp"
#include "game.hpp"
#include "moves.hpp"

// Calculates the best possible move for a given state.
moves::type exp::bestMove(uint64_t board) {
  // If the game is over, no move can be made.
  if (game::gameOver(board))
    return moves::type::NONE;

  // Initialize a transposition table.
  std::unordered_map<uint64_t, double> table;

  // Generate all possible next moves.
  uint64_t left = moves::moveLeft(board);
  uint64_t right = moves::moveRight(board);
  uint64_t down = moves::moveDown(board);
  uint64_t up = moves::moveUp(board);

  // the XOR of the current board and a new board will be zero if they are the same.
  // If a move doesn't change the board, then it is an illegal move and gets a score of zero.
  double left_score = (left ^ board) ? exp::chanceNode(left, exp::MAX_DEPTH, table, 1) : 0;
  double right_score = (right ^ board) ? exp::chanceNode(right, exp::MAX_DEPTH, table, 1) : 0;
  double up_score = (up ^ board) ? exp::chanceNode(up, exp::MAX_DEPTH, table, 1) : 0;
  double down_score = (down ^ board) ? exp::chanceNode(down, exp::MAX_DEPTH, table, 1) : 0;

  // std::cout << "Left Score: " << left_score << "\n";
  // std::cout << "Right Score: " << right_score << "\n";
  // std::cout << "Down Score: " << down_score << "\n";
  // std::cout << "Up Score: " << up_score << "\n\n";

  // Determine the best possible move using the calculated scores.
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

// The state node of a given board. After a move has been made and the board has been populated, this function should be used.
double exp::stateNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, double> &table, double probability) {
  // if the board is in the transposition table, just return the stored value.
  if (table.find(board) != table.end()) {
    return table[board];
  }

  // If the game is over, the score is automatically zero.
  if (game::gameOver(board))
    return 0;

  // We stop searching if the current depth is 0 or we are lower than the probability threshold
  if (depth == 0 || probability < exp::PROB_THRESH)
    return eval::applyHeuristic(board);

  // Generate all possible next moves
  uint64_t left = moves::moveLeft(board);
  uint64_t right = moves::moveRight(board);
  uint64_t down = moves::moveDown(board);
  uint64_t up = moves::moveUp(board);

  // After generating a next move, we must evaluate all possible new board states, so we use exp::chanceNode()
  double left_score = (left ^ board) ? exp::chanceNode(left, depth - 1, table, probability) : 0.f;
  double right_score = (right ^ board) ? exp::chanceNode(right, depth - 1, table, probability) : 0.f;
  double up_score = (up ^ board) ? exp::chanceNode(up, depth - 1, table, probability) : 0.f;
  double down_score = (down ^ board) ? exp::chanceNode(down, depth - 1, table, probability) : 0.f;

  // The best score is the best score of the four possible moves.
  double best_score = std::max(std::max(left_score, right_score), std::max(up_score, down_score));

  // Store the current boards score so it doesn't have to be recalculated later.
  table[board] = best_score;

  return best_score;
}

// The chance node for a board. After a move is made, the board needs to be populated, the evaluates all generated boards.
double exp::chanceNode(uint64_t board, uint8_t depth, std::unordered_map<uint64_t, double> &table, double probability) {
  // Get a board with only the open spaces.
  uint64_t open_spaces = game::openSpaces(board);

  // Since this should always be called only directly after making a move, there should be no open spaces possible.
  // IF a move is made and no open space is generated, then the game must be over.
  if (!(open_spaces))
    return 0;

  // By counting the number of open spaces, we can determing half the number of children of this state.
  uint8_t num_children{game::countTiles(open_spaces)};

  // The probability of a two being generated is 90% while the probability of a four is 10%.
  // For any given open tile, the probability that that tile is selected is 1 / num_children.
  double prob_two{0.9f * (1.0f / num_children)};
  double prob_four{0.1f * (1.0f / num_children)};

  // These represent board states that hold the position of the new tiles to be added.
  uint64_t new_two = 0x1;
  uint64_t new_four = 0x2;

  // Value represents the value of the current state.
  // This should be the average of all new boards weighted by their probabilities.
  double value{0};

  // While there are still open spaces, there are still new boards to generate.
  while (open_spaces) {
    // If there is an open spaces in the first four bits, then we need to generate a new board.
    if (open_spaces & 0xF) {
      // Add the values of boards with a two and boards with a four to the current value.
      value += prob_two * (exp::stateNode(board | new_two, depth, table, probability * prob_two));
      value += prob_four * (exp::stateNode(board | new_four, depth, table, probability * prob_four));
    }
    // We then move new_two, new_four, and open_spaces for the next iteration.
    new_two = new_two << 4;
    new_four = new_four << 4;
    open_spaces = open_spaces >> 4;
  }

  return value;
}

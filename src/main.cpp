#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <future>

#include "hyperparameter.hpp"

int main() {
  const int num_trials = 8;

  float mono;
  float zero;
  float edge;
  float merge;
  float best_mono;
  float best_zero;
  float best_edge;
  float best_merge;

  float result {0.f};

  float best_result {0.f};

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1000);

  for (;;) {
    mono = dis(gen);
    zero = dis(gen);
    merge = dis(gen);
    edge = dis(gen);

    std::array<std::future<uint32_t>, num_trials> results;

    for (int i = 0; i < results.size(); i++) {
      results[i] = std::async(parameter::evaluateHueristics, mono, zero, edge, merge);
    }

    for (int i = 0; i < results.size(); i++) {
      result = result + results[i].get();
    }

    result = result / num_trials;

    if (best_result < result) {
      std::cout << "New best result: " << static_cast<int>(result) << "\n\n";
      best_mono = mono;
      best_zero = zero;
      best_edge = edge;
      best_merge = merge;

      std::cout << "Monotonicity Multiplier: " << best_mono << "\n";
      std::cout << "Zeros Multiplier: " << best_zero << "\n";
      std::cout << "Edge Multiplier: " << best_edge << "\n";
      std::cout << "Merge Multiplier: " << best_merge << "\n\n";

      best_result = result;
    }

    result = 0;
  }
}

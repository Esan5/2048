#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <future>
#include <array>
#include <queue>

#include "hyperparameter.hpp"
#include "expectiminimax.hpp"
#include "game.hpp"

parameter::individual cross(const parameter::individual &i1, const parameter::individual &i2) {
  parameter::individual result;
  result.edge = (i1.edge + i2.edge) / 2;
  result.zero = (i1.zero + i2.zero) / 2;
  result.mono = (i1.mono + i2.mono) / 2;
  result.merge = (i1.merge + i2.merge) / 2;

  return result;
}

void mutate(parameter::individual &i) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 2);

  std::array<float, 3> mutation {1, 0.80, 1.25};

  i.zero = i.zero * mutation[dis(gen)];
  i.merge = i.merge * mutation[dis(gen)];
  i.edge = i.edge * mutation[dis(gen)];
  i.mono = i.mono * mutation[dis(gen)];

}

void print_individual(parameter::individual const &i) {
  std::cout << "Edge: " << i.edge << "\n";
  std::cout << "Zero: " << i.zero << "\n";
  std::cout << "Mono: " << i.mono << "\n";
  std::cout << "Merge: " << i.merge << "\n\n";
}

void genetic() {
  float best_mono;
  float best_zero;
  float best_edge;
  float best_merge;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1000);

  std::array<parameter::individual, 16> population {};

  for (auto i = 0; i < 16; i++) {
    population[i].mono = dis(gen);
    population[i].zero = dis(gen);
    population[i].edge = dis(gen);
    population[i].merge = dis(gen);

  }

  auto cmp = [] (std::pair<float, parameter::individual> p1, std::pair<float, parameter::individual> p2) {
    return std::get<float>(p1) < std::get<float>(p2);
  };

  for (;;) {
    std::array<std::future<float>, 16> results;

    for (auto i = 0; i < 16; i++) {
      results[i] = std::async(parameter::evaluateIndividual, population[i]);
    }

    std::priority_queue<std::pair<float, parameter::individual>,
                        std::vector<std::pair<float, parameter::individual> >,
                        decltype(cmp)> pq(cmp);

    for (auto i = 0; i < 16; i++) {
      pq.push(std::make_pair(results[i].get(), population[i]));
    }

    std::array<parameter::individual, 4> top_4 {};
    population[0] = std::get<parameter::individual>(pq.top());
    std::cout << "Top Score: " << std::get<float>(pq.top()) << "\n";
    print_individual(population[0]);
    pq.pop();
    population[1] = std::get<parameter::individual>(pq.top());
    std::cout << "Second: " << std::get<float>(pq.top()) << "\n";
    print_individual(population[1]);
    pq.pop();
    population[2] = std::get<parameter::individual>(pq.top());
    std::cout << "Third: " << std::get<float>(pq.top()) << "\n";
    print_individual(population[2]);
    pq.pop();
    population[3] = std::get<parameter::individual>(pq.top());
    std::cout << "Fourth: " << std::get<float>(pq.top()) << "\n";
    print_individual(population[3]);
    pq.pop();

    int count = 4;
    for (auto i = 0; i < 4; i++) {
      for (auto j = i + 1; j < 4; j++) {
        population[count++] = cross(population[i], population[j]);
      }
    }

    for (auto i = 4; i < 16; i++)
      mutate(population[i]);
  }
}

void train_hypers() {
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
      parameter::individual in = {.mono = mono, .zero = zero, .edge = edge, .merge = merge};
      results[i] = std::async(std::launch::async, parameter::evaluateHueristics, in);
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

int main() {
  genetic();
}

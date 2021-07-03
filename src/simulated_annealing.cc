#include "simulated_annealing.h"

#include <cmath>
#include <fstream>
#include <limits>
#include <stdexcept>

SimulatedAnnealing::SimulatedAnnealing(const Args& args)
    : args_(args),
      random_device_(std::random_device()),
      generator_(std::mt19937(random_device_())) {}

SimulatedAnnealing::~SimulatedAnnealing() {}

const SimulatedAnnealing::Solution SimulatedAnnealing::Start() {
  auto csv_file = std::ofstream("results/results.csv",
                                std::fstream::out | std::fstream::trunc);
  csv_file << "temperature,value\n";

  auto solution = args_.initial_solution;
  double temperature = args_.initial_temperature;
  long double value_delta = std::numeric_limits<long double>::max();

  while (!Cutoff(temperature, value_delta)) {
    for (std::size_t i = 0; i < args_.num_iterations; ++i) {
      auto new_solution = Neighbor(solution);
      value_delta = EasomFunction::TwoVars(new_solution) -
                    EasomFunction::TwoVars(solution);

      auto u_dis = std::uniform_real_distribution<>(0, 1);
      if (value_delta <= 0.0 ||
          u_dis(generator_) < AcceptanceProbability(value_delta, temperature)) {
        solution = new_solution;
      }
    }
    std::cout << "f" << solution << " = "
              << (double)EasomFunction::TwoVars(solution) << std::endl;
    csv_file << temperature << "," << EasomFunction::TwoVars(solution) << "\n";
    DecrementTemperature(temperature);
  }

  csv_file.close();
  return solution;
}

const SimulatedAnnealing::Solution SimulatedAnnealing::Neighbor(
    const Solution& solution) {
  auto dis = std::uniform_real_distribution<>(-0.1, 0.1);
  auto neighbor = Solution({solution[EasomFunction::kX1] + dis(generator_),
                            solution[EasomFunction::kX2] + dis(generator_)});

  if (neighbor[EasomFunction::kX1] >= EasomFunction::kDomainMin &&
      neighbor[EasomFunction::kX2] >= EasomFunction::kDomainMin &&
      neighbor[EasomFunction::kX1] <= EasomFunction::kDomainMax &&
      neighbor[EasomFunction::kX2] <= EasomFunction::kDomainMax) {
    return neighbor;
  } else {
    return Solution::Random(EasomFunction::kDomainMin,
                            EasomFunction::kDomainMax);
  }
}

void SimulatedAnnealing::DecrementTemperature(double& temperature) const {
  if (args_.decrement_rule == DecrementRule::kLinear) {
    temperature -= args_.alpha;
  } else if (args_.decrement_rule == DecrementRule::kGeometric) {
    temperature *= args_.alpha;
  } else if (args_.decrement_rule == DecrementRule::kSlow) {
    temperature = args_.initial_temperature / (1 + (args_.beta * temperature));
  } else {
    throw std::logic_error("Unknown temperature decrement rule!");
  }
}

const bool SimulatedAnnealing::Cutoff(const double temperature,
                                      const long double value_delta) const {
  return temperature < args_.min_temperature;
  // return temperature < args_.min_temperature ||
  //        value_delta <= args_.value_delta_cutoff;
}

const double SimulatedAnnealing::AcceptanceProbability(
    const long double value_delta, const double temperature) {
  return std::exp(-value_delta / temperature);
}

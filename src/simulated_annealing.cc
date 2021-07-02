#include "simulated_annealing.h"

#include <cmath>
#include <fstream>
#include <limits>
#include <stdexcept>

SimulatedAnnealing::SimulatedAnnealing(const Args& args)
    : args_(args),
      random_device_(std::random_device()),
      generator_(std::mt19937(random_device_())) {
  AssertArgs(args);
}

SimulatedAnnealing::~SimulatedAnnealing() {}

const SimulatedAnnealing::Solution SimulatedAnnealing::Start() {
  auto csv_file =
      std::ofstream("results.csv", std::fstream::out | std::fstream::trunc);
  csv_file << "temperature,value\n";

  auto solution = args_.initial_solution;
  double temperature = args_.initial_temperature;
  double value_delta = std::numeric_limits<double>::max();

  while (!Cutoff(temperature, value_delta)) {
    for (std::size_t i = 0; i < args_.num_iterations; ++i) {
      auto new_solution = Neighbor(solution);
      double value_delta = EasomFunction::TwoVars(new_solution) -
                           EasomFunction::TwoVars(solution);

      if (value_delta <= 0.0) {
        solution = new_solution;
      } else {
        auto u_dis = std::uniform_int_distribution<>(0, 1);
        if (u_dis(generator_) <
            AcceptanceProbability(value_delta, temperature)) {
          solution = new_solution;
        }
      }
    }

    csv_file << temperature << "," << EasomFunction::TwoVars(solution) << "\n";
    DecrementTemperature(temperature);
  }

  csv_file.close();
  return solution;
}

void SimulatedAnnealing::AssertArgs(const Args& args) {
  if (args.initial_temperature < 0.0) {
    throw std::invalid_argument("Initial temperature must be > 0!");
  }

  if (args.alpha < 0.0 || args.alpha >= 1.0) {
    throw std::invalid_argument("Cooling rate must be in the range of [0, 1]!");
  }
}

const SimulatedAnnealing::Solution SimulatedAnnealing::Neighbor(
    const Solution& solution) {
  auto x1_dis = std::normal_distribution<>(solution[EasomFunction::kX1],
                                           kNeighborStepSize);
  auto x2_dis = std::normal_distribution<>(solution[EasomFunction::kX2],
                                           kNeighborStepSize);
  auto neighbor = Solution({x1_dis(generator_), x2_dis(generator_)});

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
    temperature = temperature / (1 + (args_.beta * temperature));
  } else {
    throw std::logic_error("Unknown temperature decrement rule!");
  }
}

const bool SimulatedAnnealing::Cutoff(const double temperature,
                                      const double value_delta) const {
  return temperature < args_.min_temperature ||
         value_delta <= args_.value_delta_cutoff;
}

const double SimulatedAnnealing::AcceptanceProbability(
    const double value_delta, const double temperature) {
  return std::exp(-value_delta / temperature);
}

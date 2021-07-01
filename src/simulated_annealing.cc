#include "simulated_annealing.h"

#include <cmath>
#include <limits>
#include <stdexcept>

SimulatedAnnealing::SimulatedAnnealing(const Args& args)
    : args_(args),
      random_device_(std::random_device()),
      generator_(std::mt19937(random_device_())) {
  AssertArgs(args);
}

SimulatedAnnealing::~SimulatedAnnealing() {}

const Solution<EasomFunction::kNumVars> SimulatedAnnealing::Start() {
  auto solution = Solution<EasomFunction::kNumVars>::RandomSolution(
      EasomFunction::kDomainMin, EasomFunction::kDomainMax);
  double temperature = args_.initial_temperature;
  double value_delta = std::numeric_limits<double>::max();

  do {
    std::cout << "Temperature: " << temperature << std::endl;

    for (std::size_t i = 0; i < args_.num_iterations; ++i) {
      auto new_solution = NeighborhoodSolution(solution);
      double value_delta = EasomFunction::TwoVars(new_solution) -
                           EasomFunction::TwoVars(solution);

      if ((args_.maximizing && value_delta > 0.0) ||
          (!args_.maximizing && value_delta <= 0.0)) {
        solution = new_solution;
      } else {
        auto distribution = std::uniform_int_distribution<>(0, 1);

        if (distribution(generator_) <
            AcceptanceProbability(value_delta, temperature, args_.maximizing)) {
          solution = new_solution;
        }
      }
    }

    DecrementTemperature(temperature);
    std::cout << "Solution: " << solution << std::endl;

  } while (!Cutoff(temperature, value_delta));

  return solution;
}

void SimulatedAnnealing::AssertArgs(const Args& args) {
  if (args.initial_temperature < 0.0) {
    throw std::invalid_argument("Initial temperature must be > 0!");
  }

  if (args.cooling_rate < 0.0 || args.cooling_rate >= 1.0) {
    throw std::invalid_argument("Cooling rate must be in the range of [0, 1]!");
  }
}

const Solution<EasomFunction::kNumVars>
SimulatedAnnealing::NeighborhoodSolution(
    const Solution<EasomFunction::kNumVars>& solution) {
  auto distribution =
      std::uniform_real_distribution<>(-kNeighborStepSize, kNeighborStepSize);
  return Solution<EasomFunction::kNumVars>(
      {solution[EasomFunction::kX1] + distribution(generator_),
       solution[EasomFunction::kX2] + distribution(generator_)});
}

void SimulatedAnnealing::DecrementTemperature(double& temperature) const {
  if (args_.decrement_rule == DecrementRule::kLinear) {
    temperature -= args_.cooling_rate;
  } else if (args_.decrement_rule == DecrementRule::kGeometric) {
    temperature *= args_.cooling_rate;
  } else if (args_.decrement_rule == DecrementRule::kSlow) {
    throw std::logic_error("Slow temperature decrement not yet implemented!");
  } else {
    throw std::logic_error("Unknown temperature decrement rule!");
  }
}

const bool SimulatedAnnealing::Cutoff(const double temperature,
                                      const double value_delta) const {
  return temperature < args_.min_temperature ||
         value_delta <= args_.value_delta_cutoff;
}

const double SimulatedAnnealing::AcceptanceProbability(const double value_delta,
                                                       const double temperature,
                                                       const bool maximizing) {
  return maximizing ? std::exp(value_delta / temperature)
                    : std::exp(-value_delta / temperature);
}

#include "simulated_annealing.h"

#include <cmath>
#include <stdexcept>

SimulatedAnnealing::SimulatedAnnealing(
    const double initial_temperature, const double cooling_rate,
    const std::size_t num_iterations,
    const TemperatureDecrementRule temperature_decrement_rule,
    const bool maximizing)
    : initial_temperature_(initial_temperature),
      cooling_rate_(cooling_rate),
      num_iterations_(num_iterations),
      temperature_decrement_rule_(temperature_decrement_rule),
      maximizing_(maximizing),
      random_device_(std::random_device()),
      generator_(std::mt19937(random_device_())) {
  if (cooling_rate_ < 0.0 || cooling_rate_ >= 1.0) {
    throw std::logic_error("Cooling rate must be in the range of [0, 1]!");
  }
}

SimulatedAnnealing::~SimulatedAnnealing() {}

const EasomFunction::Solution SimulatedAnnealing::Start() {
  auto solution = EasomFunction::Solution::RandomSolution();
  double temperature = initial_temperature_;

  do {
    std::cout << "Temperature: " << temperature << std::endl;

    for (std::size_t i = 0; i < num_iterations_; ++i) {
      auto new_solution = NeighborhoodSolution(solution);
      double value_delta =
          EasomFunction::f(new_solution) - EasomFunction::f(solution);

      if ((maximizing_ && value_delta > 0.0) ||
          (!maximizing_ && value_delta <= 0)) {
        solution = new_solution;
      } else {
        auto distribution = std::uniform_int_distribution<>(0, 1);
        int random = distribution(generator_);

        if (random <
            AcceptanceProbability(value_delta, temperature, maximizing_)) {
          solution = new_solution;
        }
      }
      // std::cout << "Iteration: " << i << std::endl;
      // std::cout << "Solution: " << solution << std::endl;
    }

    DecrementTemperature(temperature);
    std::cout << "Solution: " << solution << std::endl;

  } while (!Cutoff(temperature));

  return solution;
}

const EasomFunction::Solution SimulatedAnnealing::NeighborhoodSolution(
    const EasomFunction::Solution& solution) {
  auto distribution = std::uniform_real_distribution<>(-0.1, 0.1);
  return EasomFunction::Solution(solution.x1 + distribution(generator_),
                                 solution.x2 + distribution(generator_));
}

void SimulatedAnnealing::DecrementTemperature(double& temperature) const {
  if (temperature_decrement_rule_ == TemperatureDecrementRule::kLinear) {
    temperature -= cooling_rate_;
  } else if (temperature_decrement_rule_ ==
             TemperatureDecrementRule::kGeometric) {
    temperature *= cooling_rate_;
  } else if (temperature_decrement_rule_ == TemperatureDecrementRule::kSlow) {
    throw std::logic_error("Slow temperature decrement not yet implemented!");
  } else {
    throw std::logic_error("Unknown temperature decrement rule!");
  }
}

const bool SimulatedAnnealing::Cutoff(const double temperature) const {
  return temperature < kMinTemperature;
}

const double SimulatedAnnealing::AcceptanceProbability(const double value_delta,
                                                       const double temperature,
                                                       const bool maximizing) {
  return maximizing ? std::exp(value_delta / temperature)
                    : std::exp(-value_delta / temperature);
}
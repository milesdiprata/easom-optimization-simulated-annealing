#ifndef SIMULATED_ANNEALING_H_
#define SIMULATED_ANNEALING_H_

#include <cstddef>
#include <random>

#include "easom_function.h"

class SimulatedAnnealing {
  enum class TemperatureDecrementRule {
    kLinear,
    kGeometric,
    kSlow,
  };

 public:
  static constexpr double kDefaultInitialTemperature = 1000.0;
  static constexpr double kMinTemperature = 0.0;
  static constexpr double kDefaultCoolingRate = 0.1;
  static constexpr std::size_t kDefaultNumIterations = 100;

  SimulatedAnnealing(
      const double initial_temperature = kDefaultInitialTemperature,
      const double cooling_rate = kDefaultCoolingRate,
      const std::size_t num_iterations = kDefaultNumIterations,
      const TemperatureDecrementRule temperature_decrement_rule =
          TemperatureDecrementRule::kLinear,
      const bool maximizing = false);

  ~SimulatedAnnealing();

  const EasomFunction::Solution Start();

 private:
  const EasomFunction::Solution NeighborhoodSolution(
      const EasomFunction::Solution& solution);

  void DecrementTemperature(double& temperature) const;

  const bool Cutoff(const double temperature) const;

  static const double AcceptanceProbability(const double value_delta,
                                            const double temperature,
                                            const bool maximizing);

  double initial_temperature_;
  double cooling_rate_;
  std::size_t num_iterations_;
  TemperatureDecrementRule temperature_decrement_rule_;
  bool maximizing_;

  std::random_device random_device_;
  std::mt19937 generator_;
};

#endif  // SIMULATED_ANNEALING_H_
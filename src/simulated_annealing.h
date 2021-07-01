#ifndef SIMULATED_ANNEALING_H_
#define SIMULATED_ANNEALING_H_

#include <cstddef>
#include <random>

#include "easom_function.h"

class SimulatedAnnealing {
  enum class DecrementRule {
    kLinear,
    kGeometric,
    kSlow,
  };

  struct Args {
    static constexpr double kDefaultInitialTemperature = 1000.0;
    static constexpr double kDeafultMinTemperature = 0.1;
    static constexpr double kDefaultCoolingRate = 0.5;
    static constexpr std::size_t kDefaultNumIterations = 1000;
    static constexpr double kDefaultValueDeltaCutoff = 0.1;
    static constexpr bool kDefaultMaximization = false;

    constexpr Args(
        const double initial_temperature = kDefaultInitialTemperature,
        const double min_temperature = kDeafultMinTemperature,
        const double cooling_rate = kDefaultCoolingRate,
        const std::size_t num_iterations = kDefaultNumIterations,
        const DecrementRule decrement_rule = DecrementRule::kLinear,
        const double value_delta_cutoff = kDefaultValueDeltaCutoff,
        const bool maximizing = kDefaultMaximization)
        : initial_temperature(initial_temperature),
          min_temperature(min_temperature),
          cooling_rate(cooling_rate),
          num_iterations(num_iterations),
          decrement_rule(decrement_rule),
          value_delta_cutoff(value_delta_cutoff),
          maximizing(maximizing) {}

    constexpr ~Args() = default;

    double initial_temperature;
    double min_temperature;
    double cooling_rate;
    std::size_t num_iterations;
    DecrementRule decrement_rule;
    double value_delta_cutoff;
    const bool maximizing;
  };

 public:
  static constexpr double kNeighborStepSize = 0.1;

  SimulatedAnnealing(const Args& args = Args());
  ~SimulatedAnnealing();

  const EasomFunction::Solution Start();

 private:
  static void AssertArgs(const Args& args = Args());

  const EasomFunction::Solution NeighborhoodSolution(
      const EasomFunction::Solution& solution);

  void DecrementTemperature(double& temperature) const;

  const bool Cutoff(const double temperature, const double value_delta) const;

  static const double AcceptanceProbability(const double value_delta,
                                            const double temperature,
                                            const bool maximizing);

  Args args_;
  std::random_device random_device_;
  std::mt19937 generator_;
};

#endif  // SIMULATED_ANNEALING_H_
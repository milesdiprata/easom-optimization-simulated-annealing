#ifndef SIMULATED_ANNEALING_H_
#define SIMULATED_ANNEALING_H_

#include <cstddef>
#include <random>

#include "easom_function.h"
#include "solution.h"

class SimulatedAnnealing {
 public:
  using Solution = Solution<EasomFunction::kNumVars>;

  enum class DecrementRule {
    kLinear,
    kGeometric,
    kSlow,
  };

  struct Args {
    static constexpr double kDefaultInitialTemperature = 1000.0;
    static constexpr double kDeafultMinTemperature = 0.1;
    static constexpr std::size_t kDefaultNumIterations = 1000;
    static constexpr double kDefaultValueDeltaCutoff = 0.1;
    static constexpr double kDefaultAlpha = 0.5;
    static constexpr double kDefaultBeta = 5.0;

    constexpr Args(
        const Solution initial_solution = Solution(),
        const double initial_temperature = kDefaultInitialTemperature,
        const double min_temperature = kDeafultMinTemperature,
        const std::size_t num_iterations = kDefaultNumIterations,
        const DecrementRule decrement_rule = DecrementRule::kLinear,
        const double alpha = kDefaultAlpha, const double beta = kDefaultBeta,
        const double value_delta_cutoff = kDefaultValueDeltaCutoff)
        : initial_solution(initial_solution),
          initial_temperature(initial_temperature),
          min_temperature(min_temperature),
          num_iterations(num_iterations),
          decrement_rule(decrement_rule),
          alpha(alpha),
          beta(beta),
          value_delta_cutoff(value_delta_cutoff) {}

    constexpr ~Args() = default;

    Solution initial_solution;
    double initial_temperature;
    double min_temperature;
    std::size_t num_iterations;
    DecrementRule decrement_rule;
    double alpha;
    double beta;
    double value_delta_cutoff;
  };

  static constexpr double kNeighborStepSize = 0.1;

  SimulatedAnnealing(const Args& args = Args());
  ~SimulatedAnnealing();

  const Solution Start();

 private:
  static void AssertArgs(const Args& args = Args());

  const Solution Neighbor(const Solution& solution);

  void DecrementTemperature(double& temperature) const;

  const bool Cutoff(const double temperature, const double value_delta) const;

  static const double AcceptanceProbability(const double value_delta,
                                            const double temperature);

  Args args_;
  std::random_device random_device_;
  std::mt19937 generator_;
};

#endif  // SIMULATED_ANNEALING_H_
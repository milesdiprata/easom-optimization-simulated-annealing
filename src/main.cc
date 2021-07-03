#include <array>
#include <iostream>
#include <numbers>

#include "easom_function.h"
#include "simulated_annealing.h"

namespace experiment {

void one() {
  auto args = SimulatedAnnealing::Args();
  args.initial_solution = SimulatedAnnealing::Solution::Random(
      EasomFunction::kDomainMin, EasomFunction::kDomainMax);
  auto solver = SimulatedAnnealing(args);
  solver.Start();
}

void two() {
  auto temperatures = std::array<double, 10>{
      1.0, 10.0, 50.0, 100.0, 200.0, 500.0, 1000.0, 1500.0, 5000.0, 10000.0};
  auto args = SimulatedAnnealing::Args();
  args.initial_temperature = temperatures[0];
  args.initial_solution = SimulatedAnnealing::Solution({-50.0, 50.0});
  auto solver = SimulatedAnnealing(args);
  solver.Start();
}

void three() {
  auto beta_values = std::array<double, 10>{1.0,  2.0,  3.0,  4.0,   5.0,
                                            10.0, 15.0, 50.0, 100.0, 10000.0};
  auto args = SimulatedAnnealing::Args();
  args.beta = beta_values[0];
  args.initial_solution = SimulatedAnnealing::Solution({-50.0, 50.0});
  auto solver = SimulatedAnnealing(args);
  solver.Start();
}

}  // namespace experiment

int main(const int argc, const char* const argv[]) {
  auto solver = SimulatedAnnealing();
  solver.Start();

  return 0;
}
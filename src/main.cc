#include <iostream>

#include "simulated_annealing.h"
#include "solution.h"

int main(const int argc, const char* const argv[]) {
  auto solver = SimulatedAnnealing();
  auto solution = solver.Start();

  std::cout << solution << std::endl;

  return 0;
}
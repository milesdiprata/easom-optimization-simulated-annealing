#include <iostream>

#include "easom_function.h"
#include "simulated_annealing.h"

int main(const int argc, const char* const argv[]) {
  auto solver = SimulatedAnnealing();
  auto solution = solver.Start();

  std::cout << "f" << solution << " = " << EasomFunction::TwoVars(solution)
            << std::endl;

  return 0;
}
#include <iostream>

#include "easom_function.h"

int main(const int argc, const char* const argv[]) {
  // std::cout << "Hello World!\n";
  auto initial_solution = EasomFunction::Solution::RandomSolution();
  std::cout << initial_solution << "\n";
  std::cout << EasomFunction::f(initial_solution) << "\n";
  return 0;
}
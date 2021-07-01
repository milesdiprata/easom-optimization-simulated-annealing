#include "easom_function.h"

#include <cmath>
#include <numbers>
#include <random>

const double EasomFunction::Solution::RandomVariable() {
  static auto random_device = std::random_device();
  static auto generator = std::mt19937(random_device());
  auto distribution = std::uniform_real_distribution<>(
      EasomFunction::kDomainMin, EasomFunction::kDomainMax);

  return distribution(generator);
}

const EasomFunction::Solution EasomFunction::Solution::RandomSolution() {
  return Solution(RandomVariable(), RandomVariable());
}

constexpr EasomFunction::Solution EasomFunction::Solution::GlobalMinSolution() {
  return Solution(std::numbers::pi, std::numbers::pi);
}

const double EasomFunction::f(const Solution& s) {
  return -std::cos(s.x1) * std::cos(s.x2) *
         std::exp(-((s.x1 - std::numbers::pi) * (s.x1 - std::numbers::pi)) -
                  ((s.x2 - std::numbers::pi) * (s.x2 - std::numbers::pi)));
}

std::ostream& operator<<(std::ostream& os,
                         const EasomFunction::Solution& solution) {
  return os << "(" << solution.x1 << ", " << solution.x2 << ")";
}

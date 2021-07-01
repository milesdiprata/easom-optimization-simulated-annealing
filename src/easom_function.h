#ifndef EASOM_FUNCTION_H_
#define EASOM_FUNCTION_H_

#include <array>
#include <cmath>
#include <iostream>
#include <numbers>
#include <random>

struct EasomFunction {
  struct Solution {
    static constexpr double kDomainMin = -100.0;
    static constexpr double kDomainMax = 100.0;

    constexpr Solution(const double x1 = 0.0, const double x2 = 0.0)
        : x1(x1), x2(x2) {}

    static const Solution RandomSolution() {
      return Solution(RandomVariable(), RandomVariable());
    }

    double x1;
    double x2;

   private:
    static const double RandomVariable() {
      static auto random_device = std::random_device();
      static auto generator = std::mt19937(random_device());
      auto distribution =
          std::uniform_real_distribution<>(kDomainMin, kDomainMax);

      return distribution(generator);
    }
  };

  EasomFunction() = delete;

  static const double f(const Solution& s) {
    return -std::cos(s.x1) * std::cos(s.x2) *
           std::exp(-((s.x1 - std::numbers::pi) * (s.x1 - std::numbers::pi)) -
                    ((s.x2 - std::numbers::pi) * (s.x2 - std::numbers::pi)));
  }
};

constexpr bool operator==(const EasomFunction::Solution& lhs,
                          const EasomFunction::Solution& rhs) {
  return lhs.x1 == rhs.x1 && lhs.x2 == rhs.x2;
}

constexpr bool operator!=(const EasomFunction::Solution& lhs,
                          const EasomFunction::Solution& rhs) {
  return lhs.x1 != rhs.x1 || lhs.x2 != rhs.x2;
}

std::ostream& operator<<(std::ostream& os,
                         const EasomFunction::Solution& solution);

#endif  // EASOM_FUNCTION_H_

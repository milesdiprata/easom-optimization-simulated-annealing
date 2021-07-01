#ifndef EASOM_FUNCTION_H_
#define EASOM_FUNCTION_H_

#include <iostream>

struct EasomFunction {
  struct Solution {
    constexpr Solution(const double x1 = 0.0, const double x2 = 0.0)
        : x1(x1), x2(x2) {}

    ~Solution() = default;

    static constexpr Solution GlobalMinSolution();
    static const Solution RandomSolution();

    double x1;
    double x2;

   private:
    static const double RandomVariable();
  };

  static constexpr double kDomainMin = -100.0;
  static constexpr double kDomainMax = 100.0;

  EasomFunction() = delete;
  ~EasomFunction() = delete;

  static const double f(const Solution& s);
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

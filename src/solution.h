#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <array>
#include <cstddef>
#include <iostream>
#include <random>
#include <string>

template <std::size_t N>
struct Solution {
  constexpr Solution(
      const std::array<long double, N> vars = std::array<long double, N>())
      : vars(vars) {}

  constexpr Solution(const Solution<N>& Solution) : vars(Solution.vars) {}

  ~Solution() = default;

  constexpr long double operator[](const std::size_t i) const {
    return vars[i];
  }
  constexpr long double operator[](const std::size_t i) { return vars[i]; }

  static const Solution Random(const double var_min, const double var_max);

  std::array<long double, N> vars;
};

template <std::size_t N>
constexpr bool operator==(const Solution<N>& lhs, const Solution<N>& rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }

  return true;
}

template <std::size_t N>
constexpr bool operator!=(const Solution<N>& lhs, const Solution<N>& rhs) {
  for (std::size_t i = 0; i < N; ++i) {
    if (lhs[i] != rhs[i]) {
      return true;
    }
  }

  return false;
}

template <std::size_t N>
const Solution<N> Solution<N>::Random(const double var_min,
                                      const double var_max) {
  static auto random_device = std::random_device();
  static auto generator = std::mt19937(random_device());
  auto distribution = std::uniform_real_distribution<>(var_min, var_max);

  auto solution = Solution<N>();
  for (auto& var : solution.vars) {
    var = distribution(generator);
  }

  return solution;
}

template <std::size_t N>
std::ostream& operator<<(std::ostream& os, const Solution<N>& solution) {
  std::string seperator = "";
  os << "(";
  for (const auto& var : solution.vars) {
    os << seperator << var;
    seperator = ", ";
  }
  os << ")";

  return os;
}

#endif  // SOLUTION_H_

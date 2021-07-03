#ifndef EASOM_FUNCTION_H_
#define EASOM_FUNCTION_H_

#include "solution.h"

struct EasomFunction {
  static constexpr std::size_t kNumVars = 2;
  static constexpr std::size_t kX1 = 0;
  static constexpr std::size_t kX2 = 1;

  static constexpr double kDomainMin = -100.0;
  static constexpr double kDomainMax = 100.0;

  EasomFunction() = delete;
  ~EasomFunction() = delete;

  static const long double TwoVars(const Solution<kNumVars>& solution);
};

#endif  // EASOM_FUNCTION_H_

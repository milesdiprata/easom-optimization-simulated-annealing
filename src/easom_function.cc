#include "easom_function.h"

#include <cmath>
#include <numbers>

const double EasomFunction::TwoVars(const Solution<kNumVars>& solution) {
  return -std::cos(solution[kX1]) * std::cos(solution[kX2]) *
         std::exp(-((solution[kX1] - std::numbers::pi) *
                    (solution[kX1] - std::numbers::pi)) -
                  ((solution[kX2] - std::numbers::pi) *
                   (solution[kX2] - std::numbers::pi)));
}

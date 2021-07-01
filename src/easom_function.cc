#include "easom_function.h"

std::ostream& operator<<(std::ostream& os,
                         const EasomFunction::Solution& solution) {
  return os << "(" << solution.x1 << ", " << solution.x2 << ")";
}

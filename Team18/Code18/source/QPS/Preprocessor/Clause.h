#pragma once

#include <memory>
#include <string>
#include <vector>

#include "QpsEvaluator.h"

namespace spa {
class Clause {
 public:
  virtual std::unique_ptr<QpsEvaluator> getEvaluator() = 0;
  virtual std::vector<std::string> getArgValues() = 0;
};
}  // namespace spa

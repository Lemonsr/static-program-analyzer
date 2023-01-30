#pragma once

#include <string>

#include "QpsPreprocessor.h"
#include "QpsEvaluator.h"

namespace spa {
class QPS {
 public:
  std::string evaluate(std::string query);
};
}


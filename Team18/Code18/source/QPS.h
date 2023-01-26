#pragma once

#include <string>

#include "QpsPreprocessor.h"
#include "QpsEvaluator.h"

class QPS {
 public:
  std::string evaluate(std::string query);
};

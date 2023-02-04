#pragma once

#include <string>
#include <list>

#include "QpsPreprocessor.h"
#include "QpsEvaluator.h"
#include "QpsResult.h"

namespace spa {
class QPS {
 public:
  QpsResult evaluate(std::string query, PKBManager& pkbManager);
};
}


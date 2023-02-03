#pragma once

#include "QpsResultTable.h"

namespace spa {
class PKBManager;

class QpsEvaluator {
 public:
  virtual QpsResultTable evaluate(PKBManager& pkbManager) = 0;
};
}  // namespace spa

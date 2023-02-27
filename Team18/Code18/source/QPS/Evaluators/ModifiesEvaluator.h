#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class ModifiesEvaluator : public QpsEvaluator {
 private:
  PqlArgument& firstArg;
  PqlArgument& secondArg;
 public:
  ModifiesEvaluator(PqlArgument& firstArg, PqlArgument& secondArg);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa

#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class ParentStarEvaluator : public QpsEvaluator {
 private:
  PqlArgument& firstArg;
  PqlArgument& secondArg;
 public:
  ParentStarEvaluator(PqlArgument& firstArg, PqlArgument& secondArg);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa

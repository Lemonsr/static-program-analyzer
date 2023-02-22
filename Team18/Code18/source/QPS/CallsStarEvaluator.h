#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class CallsStarEvaluator : public QpsEvaluator {
 private:
  PqlArgument& firstArg;
  PqlArgument& secondArg;
 public:
  CallsStarEvaluator(PqlArgument& firstArg, PqlArgument& secondArg);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa

#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class CallsEvaluator : public QpsEvaluator {
 private:
  PqlArgument& firstArg;
  PqlArgument& secondArg;
 public:
  CallsEvaluator(PqlArgument& firstArg, PqlArgument& secondArg);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa

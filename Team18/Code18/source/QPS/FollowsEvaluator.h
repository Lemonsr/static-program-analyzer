#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class FollowsEvaluator : public QpsEvaluator {
 private:
  PqlArgument& firstArg;
  PqlArgument& secondArg;
 public:
  FollowsEvaluator(PqlArgument& firstArg, PqlArgument& secondArg);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa

#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class PatternContainerEvaluator : public QpsEvaluator {
 private:
  DesignEntityType entityType;
  PqlArgument& patternSynonym;
  PqlArgument& firstArg;
 public:
  PatternContainerEvaluator(DesignEntityType entityType, PqlArgument& patternSynonym, PqlArgument& firstArg);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa

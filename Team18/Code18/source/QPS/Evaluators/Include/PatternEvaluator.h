#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class PatternEvaluator : public QpsEvaluator {
 private:
  PqlArgument& assignSynonym;
  PqlArgument& firstArg;
  Pattern& pattern;
 public:
  PatternEvaluator(PqlArgument& assignSynonym, PqlArgument& firstArg, Pattern& pattern);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa

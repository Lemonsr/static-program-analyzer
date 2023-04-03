#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class CFGEvaluator : public QpsEvaluator {
 private:
  PqlArgument& firstArg;
  PqlArgument& secondArg;
  RelationshipType designAbstraction;
  bool isValidAffectsArgs();
 public:
  CFGEvaluator(PqlArgument& firstArg, PqlArgument& secondArg, RelationshipType designAbstraction);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa

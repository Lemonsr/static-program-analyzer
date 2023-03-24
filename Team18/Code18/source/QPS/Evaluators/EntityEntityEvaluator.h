#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class EntityEntityEvaluator : public QpsEvaluator {
 private:
  PqlArgument& firstArg;
  PqlArgument& secondArg;
  RelationshipType designAbstraction;
 public:
  EntityEntityEvaluator(PqlArgument& firstArg, PqlArgument& secondArg, RelationshipType designAbstraction);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa

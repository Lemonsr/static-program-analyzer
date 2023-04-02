#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class StmtStmtEvaluator : public QpsEvaluator {
 private:
  PqlArgument& firstArg;
  PqlArgument& secondArg;
  RelationshipType designAbstraction;
 public:
  StmtStmtEvaluator(PqlArgument& firstArg, PqlArgument& secondArg, RelationshipType designAbstraction);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa

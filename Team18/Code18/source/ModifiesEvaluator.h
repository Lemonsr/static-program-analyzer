#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QueryResult.h"
#include "SuchThatEvaluator.h"

namespace spa {
class ModifiesEvaluator : SuchThatEvaluator {
private:
  SuchThatClause& suchThatClause;
  PKBManager& pkbManager;
public:
  ModifiesEvaluator(SuchThatClause& suchThatClause, PKBManager& pkbManager);
  QueryResult evaluate();
};
}  // namespace spa

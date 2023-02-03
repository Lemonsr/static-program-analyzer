#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QueryResult.h"
#include "SuchThatEvaluator.h"

namespace spa {
class UsesEvaluator : SuchThatEvaluator {
private:
  SuchThatClause& suchThatClause;
  PKBManager& pkbManager;
public:
  UsesEvaluator(SuchThatClause& suchThatClause, PKBManager& pkbManager);
  QueryResult evaluate();
};
}  // namespace spa

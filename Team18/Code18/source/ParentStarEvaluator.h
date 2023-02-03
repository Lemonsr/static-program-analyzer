#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QueryResult.h"
#include "SuchThatEvaluator.h"

namespace spa {
class ParentStarEvaluator : SuchThatEvaluator {
private:
  SuchThatClause& suchThatClause;
  PKBManager& pkbManager;
public:
  ParentStarEvaluator(SuchThatClause& suchThatClause, PKBManager& pkbManager);
  QueryResult evaluate();
};
}  // namespace spa

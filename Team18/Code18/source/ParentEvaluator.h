#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QueryResult.h"
#include "SuchThatEvaluator.h"

namespace spa {
class ParentEvaluator : SuchThatEvaluator {
private:
  SuchThatClause& suchThatClause;
  PKBManager& pkbManager;
public:
  ParentEvaluator(SuchThatClause& suchThatClause, PKBManager& pkbManager);
  QueryResult evaluate();
};
}  // namespace spa

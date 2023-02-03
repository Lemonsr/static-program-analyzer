#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QueryResult.h"
#include "SuchThatEvaluator.h"

namespace spa {
class FollowsEvaluator : SuchThatEvaluator {
private:
  SuchThatClause& suchThatClause;
  PKBManager& pkbManager;
public:
  FollowsEvaluator(SuchThatClause& suchThatClause, PKBManager& pkbManager);
  QueryResult evaluate();
};
}  // namespace spa

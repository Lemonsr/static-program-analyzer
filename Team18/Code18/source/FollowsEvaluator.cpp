#include "FollowsEvaluator.h"

spa::FollowsEvaluator::FollowsEvaluator(SuchThatClause& suchThatClause, PKBManager& pkbManager) :
  suchThatClause(suchThatClause), pkbManager(pkbManager) {
}

spa::QueryResult spa::FollowsEvaluator::evaluate() {
  return QueryResult();
}

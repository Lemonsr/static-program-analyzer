#include "FollowsStarEvaluator.h"

spa::FollowsStarEvaluator::FollowsStarEvaluator(SuchThatClause& suchThatClause, PKBManager& pkbManager) :
  suchThatClause(suchThatClause), pkbManager(pkbManager) {
}

spa::QueryResult spa::FollowsStarEvaluator::evaluate() {
  return QueryResult();
}

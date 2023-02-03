#include "ParentStarEvaluator.h"

spa::ParentStarEvaluator::ParentStarEvaluator(SuchThatClause& suchThatClause, PKBManager& pkbManager) :
  suchThatClause(suchThatClause), pkbManager(pkbManager) {
}

spa::QueryResult spa::ParentStarEvaluator::evaluate() {
  return QueryResult();
}

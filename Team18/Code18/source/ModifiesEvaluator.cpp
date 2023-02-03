#include "ModifiesEvaluator.h"

spa::ModifiesEvaluator::ModifiesEvaluator(SuchThatClause& suchThatClause, PKBManager& pkbManager) :
  suchThatClause(suchThatClause), pkbManager(pkbManager) {
}

spa::QueryResult spa::ModifiesEvaluator::evaluate() {
  return QueryResult();
}

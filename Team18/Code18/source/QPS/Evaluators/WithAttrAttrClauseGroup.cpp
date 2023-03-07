#include "WithAttrAttrClauseGroup.h"

spa::QpsResultTable spa::WithAttrAttrClauseGroup::evaluate(PKBManager& pkbManager, QpsResultTable resultTable) {
  // TODO: Add with clause evaluation
  return resultTable;
}

bool spa::WithAttrAttrClauseGroup::addClause(WithClause& clause) {
  clauses.push_back(clause);
  return true;
}

bool spa::WithAttrAttrClauseGroup::isEmpty() {
  return clauses.empty();
}

bool spa::operator==(const WithAttrAttrClauseGroup& first, const WithAttrAttrClauseGroup& second) {
  if (first.clauses.size() != second.clauses.size()) {
    return false;
  }
  for (int i = 0; i < first.clauses.size(); ++i) {
    if (first.clauses[i] != second.clauses[i]) {
      return false;
    }
  }
  return true;
}

bool spa::operator!=(const WithAttrAttrClauseGroup& first, const WithAttrAttrClauseGroup& second) {
  return !(first == second);
}

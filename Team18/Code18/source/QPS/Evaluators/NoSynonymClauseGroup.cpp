#include "NoSynonymClauseGroup.h"

spa::QpsResultTable spa::NoSynonymClauseGroup::evaluate(PKBManager& pkbManager) {
  QpsResultTable resultTable;
  for (auto& clause : clauses) {
    std::unique_ptr<QpsEvaluator> evaluator = clause->getEvaluator();
    resultTable = evaluator->evaluate(pkbManager);
    if (resultTable.isEmpty()) {
      break;
    }
  }
  return resultTable;
}

bool spa::NoSynonymClauseGroup::addClause(Clause& clause) {
  clauses.push_back(&clause);
  return true;
}

bool spa::NoSynonymClauseGroup::isEmpty() {
  return clauses.empty();
}

bool spa::operator==(const NoSynonymClauseGroup& first, const NoSynonymClauseGroup& second) {
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

bool spa::operator!=(const NoSynonymClauseGroup& first, const NoSynonymClauseGroup& second) {
  return !(first == second);
}

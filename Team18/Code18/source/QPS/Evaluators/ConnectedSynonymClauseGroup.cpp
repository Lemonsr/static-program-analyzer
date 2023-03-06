#include "ConnectedSynonymClauseGroup.h"
#include "SimpleEvaluator.h"

spa::QpsResultTable spa::ConnectedSynonymClauseGroup::evaluate(PKBManager& pkbManager) {
  std::unique_ptr<QpsEvaluator> evaluator = clauses[0]->getEvaluator();
  QpsResultTable resultTable = evaluator->evaluate(pkbManager);
  for (auto& clause : clauses) {
    evaluator = clause->getEvaluator();
    resultTable = resultTable.innerJoin(evaluator->evaluate(pkbManager));
  }
  return resultTable;
}

bool spa::ConnectedSynonymClauseGroup::addClause(Clause& clause) {
  clauses.push_back(&clause);
  return true;
}

bool spa::operator==(const ConnectedSynonymClauseGroup& first, const ConnectedSynonymClauseGroup& second) {
  if (first.clauses.size() != second.clauses.size()) {
    return false;
  }

  if (first.connectedSynonyms.size() != second.connectedSynonyms.size()) {
    return false;
  }

  for (int i = 0; i < first.clauses.size(); ++i) {
    if (first.clauses[i] != second.clauses[i]) {
      return false;
    }
  }

  return true;
}

bool spa::operator!=(const ConnectedSynonymClauseGroup& first, const ConnectedSynonymClauseGroup& second) {
  return !(first == second);
}

#include "ConnectedSynonymClauseGroup.h"
#include "SimpleEvaluator.h"

spa::QpsResultTable spa::ConnectedSynonymClauseGroup::evaluate(PKBManager& pkbManager,
    std::unordered_map<std::string, DesignEntityType>& selectWithDeclarations) {
  std::unique_ptr<QpsEvaluator> evaluator = clauses[0]->getEvaluator();
  QpsResultTable resultTable = evaluator->evaluate(pkbManager);
  for (size_t i = 1; i < clauses.size(); ++i) {
    evaluator = clauses[i]->getEvaluator();
    resultTable = resultTable.innerJoin(evaluator->evaluate(pkbManager));
  }

  QpsResultTable filteredTable;
  filteredTable.addHeader("");
  if (resultTable.isEmpty()) {
    return filteredTable;
  }

  std::vector<std::string> selectWithSynonyms;
  for (auto& p : selectWithDeclarations) {
    selectWithSynonyms.push_back(p.first);
  }

  resultTable = resultTable.getColumns(selectWithSynonyms);
  if (resultTable.isEmpty()) {
    filteredTable.addRow({ QpsValue(0) });
    return filteredTable;
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

  for (size_t i = 0; i < first.clauses.size(); ++i) {
    if (first.clauses[i] != second.clauses[i]) {
      return false;
    }
  }

  return true;
}

bool spa::operator!=(const ConnectedSynonymClauseGroup& first, const ConnectedSynonymClauseGroup& second) {
  return !(first == second);
}

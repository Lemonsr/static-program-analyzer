#include "WithClauseGroup.h"

#include <memory>

#include "WithEvaluator.h"

spa::QpsResultTable spa::WithClauseGroup::evaluate(PKBManager& pkbManager, QpsResultTable resultTable) {
  for (auto& clause : attrValueClauses) {
    QpsResultTable result = clause.getEvaluator()->evaluate(pkbManager);
    resultTable.innerJoin(result);
  }

  std::vector<std::unique_ptr<QpsEvaluator>> evaluators;
  for (auto& clause : attrAttrClauses) {
    evaluators.push_back(clause.getEvaluator());
  }
  for (auto& evaluator : evaluators) {
    WithEvaluator* withPtr = static_cast<WithEvaluator*>(evaluator.get());
    resultTable = withPtr->filter(resultTable);
  }
  return resultTable;
}

bool spa::WithClauseGroup::addAttrAttrClause(WithClause& clause) {
  attrAttrClauses.push_back(clause);
  return true;
}

bool spa::WithClauseGroup::addAttrValueClause(WithClause& clause) {
  attrValueClauses.push_back(clause);
  return true;
}

bool spa::WithClauseGroup::isEmpty() {
  return attrAttrClauses.empty() && attrValueClauses.empty();
}

bool spa::operator==(const WithClauseGroup& first, const WithClauseGroup& second) {
  if (first.attrAttrClauses.size() != second.attrAttrClauses.size() ||
      first.attrValueClauses.size() != second.attrValueClauses.size()) {
    return false;
  }

  for (size_t i = 0; i < first.attrAttrClauses.size(); ++i) {
    if (first.attrAttrClauses[i] != second.attrAttrClauses[i]) {
      return false;
    }
  }

  for (size_t i = 0; i < first.attrValueClauses.size(); ++i) {
    if (first.attrValueClauses[i] != second.attrValueClauses[i]) {
      return false;
    }
  }
  return true;
}

bool spa::operator!=(const WithClauseGroup& first, const WithClauseGroup& second) {
  return !(first == second);
}

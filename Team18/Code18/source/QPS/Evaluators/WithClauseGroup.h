#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <string>

#include "QpsResultTable.h"
#include "ParsedQuery.h"

namespace spa {
class WithClauseGroup {
 private:
  std::vector<WithClause> attrAttrClauses;
  std::vector<WithClause> attrValueClauses;
 public:
  QpsResultTable evaluate(PKBManager& pkbManager, QpsResultTable resultTable);
  bool addAttrAttrClause(WithClause& clause);
  bool addAttrValueClause(WithClause& clause);
  bool isEmpty();

  friend bool operator==(const WithClauseGroup& first, const WithClauseGroup& second);
  friend bool operator!=(const WithClauseGroup& first, const WithClauseGroup& second);
};
}  // namespace spa

#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <string>

#include "QpsResultTable.h"
#include "ParsedQuery.h"

namespace spa {
class WithAttrAttrClauseGroup {
 private:
  std::vector<WithClause> clauses;
 public:
  QpsResultTable evaluate(PKBManager& pkbManager, QpsResultTable resultTable);
  bool addClause(WithClause& clause);

  friend bool operator==(const WithAttrAttrClauseGroup& first, const WithAttrAttrClauseGroup& second);
  friend bool operator!=(const WithAttrAttrClauseGroup& first, const WithAttrAttrClauseGroup& second);
};
}  // namespace spa

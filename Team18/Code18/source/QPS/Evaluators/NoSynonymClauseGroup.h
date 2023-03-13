#pragma once

#include <vector>
#include <memory>

#include "QpsResultTable.h"
#include "Clause.h"

namespace spa {
class NoSynonymClauseGroup {
 private:
  std::vector<Clause*> clauses;
 public:
  QpsResultTable evaluate(PKBManager& pkbManager);
  bool addClause(Clause& clause);
  bool isEmpty();

  friend bool operator==(const NoSynonymClauseGroup& first, const NoSynonymClauseGroup& second);
  friend bool operator!=(const NoSynonymClauseGroup& first, const NoSynonymClauseGroup& second);
};
}  // namespace spa

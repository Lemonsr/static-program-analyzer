#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <string>

#include "QpsResultTable.h"
#include "Clause.h"

namespace spa {
class ConnectedSynonymClauseGroup {
 private:
  std::vector<Clause*> clauses;
  std::unordered_set<std::string> connectedSynonyms;
 public:
  QpsResultTable evaluate(PKBManager& pkbManager);
  bool addClause(Clause& clause);

  friend bool operator==(const ConnectedSynonymClauseGroup& first, const ConnectedSynonymClauseGroup& second);
  friend bool operator!=(const ConnectedSynonymClauseGroup& first, const ConnectedSynonymClauseGroup& second);
};
}  // namespace spa

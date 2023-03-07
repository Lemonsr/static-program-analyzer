#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <string>
#include <utility>

#include "Clause.h"
#include "NoSynonymClauseGroup.h"
#include "ConnectedSynonymClauseGroup.h"
#include "ParsedQuery.h"

namespace spa {
class QueryOptimizer {
 private:
  std::unordered_map<std::string, std::vector<Clause*>> synonymClauseMap;
  NoSynonymClauseGroup noSynonymClauseGroup;
  std::vector<ConnectedSynonymClauseGroup> connectedSynonymClauseGroups;
  std::vector<Clause*> clausesWithSynonyms;
  std::unordered_set<Clause*> visitedClauses;

  void initialize(ParsedQuery& parsedQuery);
  ConnectedSynonymClauseGroup groupConnectedComponents(Clause* clause);
 public:
  std::pair<NoSynonymClauseGroup, std::vector<ConnectedSynonymClauseGroup>> getGroups(ParsedQuery& parsedQuery);
};
}  // namespace spa

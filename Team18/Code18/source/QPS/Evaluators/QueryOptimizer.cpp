#include "QueryOptimizer.h"

#include <unordered_set>
#include <queue>

spa::ConnectedSynonymClauseGroup spa::QueryOptimizer::groupConnectedComponents(Clause* clause) {
  std::queue<Clause*> connectedClauses;
  ConnectedSynonymClauseGroup connectedSynonymClauseGroup;
  connectedClauses.push(clause);

  while (!connectedClauses.empty()) {
    Clause* clause = connectedClauses.front();
    connectedSynonymClauseGroup.addClause(*clause);
    connectedClauses.pop();
    visitedClauses.insert(clause);

    for (auto& synonym : clause->getSynonyms()) {
      for (auto& nei : synonymClauseMap[synonym]) {
        if (visitedClauses.find(nei) != visitedClauses.end()) {
          continue;
        }
        connectedClauses.push(nei);
      }
    }
  }

  return connectedSynonymClauseGroup;
}

std::unordered_map<std::string, spa::ConnectedSynonymClauseGroup> spa::QueryOptimizer::getSynonymGroupMap() {
  return std::unordered_map<std::string, ConnectedSynonymClauseGroup>();
}

std::pair<spa::NoSynonymClauseGroup,
  std::vector<spa::ConnectedSynonymClauseGroup>> spa::QueryOptimizer::getGroups(ParsedQuery& parsedQuery) {
  initialize(parsedQuery);
  if (clausesWithSynonyms.empty()) {
    return { noSynonymClauseGroup, connectedSynonymClauseGroups };
  }

  for (auto& clause : clausesWithSynonyms) {
    if (visitedClauses.find(clause) != visitedClauses.end()) {
      continue;
    }
    connectedSynonymClauseGroups.push_back(groupConnectedComponents(clause));
  }

  return { noSynonymClauseGroup, connectedSynonymClauseGroups };
}

void spa::QueryOptimizer::initialize(ParsedQuery& parsedQuery) {
  for (auto& clause : parsedQuery.getSuchThatClauses()) {
    // add withClause to noSynoymClauseGroup if got no attribute
    if (clause.getFirstArgType() != SYNONYM && clause.getSecondArgType() != SYNONYM) {
      noSynonymClauseGroup.addClause(clause);
      continue;
    }

    clausesWithSynonyms.push_back(&clause);
    if (clause.getFirstArgType() == spa::SYNONYM) {
      synonymClauseMap[clause.getFirstArgValue()].push_back(&clause);
    }

    if (clause.getSecondArgType() == spa::SYNONYM) {
      synonymClauseMap[clause.getSecondArgValue()].push_back(&clause);
    }
  }

  for (auto& clause : parsedQuery.getPatternClauses()) {
    clausesWithSynonyms.push_back(&clause);
    synonymClauseMap[clause.getSynonymValue()].push_back(&clause);

    if (clause.getFirstArgType() == spa::SYNONYM) {
      synonymClauseMap[clause.getFirstArgValue()].push_back(&clause);
    }
  }
}

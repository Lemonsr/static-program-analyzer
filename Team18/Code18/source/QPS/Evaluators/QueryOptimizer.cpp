#include "QueryOptimizer.h"

#include <unordered_set>
#include <queue>

spa::ConnectedSynonymClauseGroup spa::QueryOptimizer::groupConnectedComponents(Clause* clause) {
  std::queue<Clause*> connectedClauses;
  ConnectedSynonymClauseGroup connectedSynonymClauseGroup;
  connectedClauses.push(clause);

  while (!connectedClauses.empty()) {
    Clause* clause = connectedClauses.front();
    connectedClauses.pop();
    if (visitedClauses.find(clause) != visitedClauses.end()) {
      continue;
    }
    connectedSynonymClauseGroup.addClause(*clause);
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

spa::ClauseGroups spa::QueryOptimizer::getGroups(ParsedQuery& parsedQuery) {
  initialize(parsedQuery);

  for (auto& clause : clausesWithSynonyms) {
    if (visitedClauses.find(clause) != visitedClauses.end()) {
      continue;
    }
    connectedSynonymClauseGroups.push_back(groupConnectedComponents(clause));
  }

  ClauseGroups clauseGroups(noSynonymClauseGroup, connectedSynonymClauseGroups, withAttrAttrClauseGroup);
  return clauseGroups;
}

void spa::QueryOptimizer::initialize(ParsedQuery& parsedQuery) {
  for (auto& clause : parsedQuery.getSuchThatClauses()) {
    std::vector<std::string> synonyms = clause.getSynonyms();
    if (synonyms.empty()) {
      noSynonymClauseGroup.addClause(clause);
      continue;
    }

    clausesWithSynonyms.push_back(&clause);
    for (auto& synonym : synonyms) {
      synonymClauseMap[synonym].push_back(&clause);
    }
  }

  for (auto& clause : parsedQuery.getPatternClauses()) {
    clausesWithSynonyms.push_back(&clause);
    std::vector<std::string> synonyms = clause.getSynonyms();
    for (auto& synonym : synonyms) {
      synonymClauseMap[synonym].push_back(&clause);
    }
  }

  for (auto& clause : parsedQuery.getWithClauses()) {
    std::vector<std::string> synonyms = clause.getSynonyms();
    if (synonyms.size() == 0) {
      noSynonymClauseGroup.addClause(clause);
    } else if (synonyms.size() == 2) {
      withAttrAttrClauseGroup.addClause(clause);
    } else {
      clausesWithSynonyms.push_back(&clause);
      synonymClauseMap[synonyms[0]].push_back(&clause);
    }
  }
}

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

  for (auto& clause : parsedQuery.getWithClauses()) {
    if (clause.getFirstArgType() == WithArgumentType::WITH_VALUE &&
      clause.getSecondArgType() == WithArgumentType::WITH_VALUE) {
      noSynonymClauseGroup.addClause(clause);
      continue;
    }

    if (clause.getFirstArgType() == WithArgumentType::WITH_ATTRIBUTE &&
      clause.getFirstArgType() == WithArgumentType::WITH_ATTRIBUTE) {
      withAttrAttrClauseGroup.addClause(clause);
      continue;
    }

    std::vector<std::string> synonyms = clause.getSynonyms();
    for (auto& synonym : synonyms) {
      synonymClauseMap[synonym].push_back(&clause);
    }
  }
}

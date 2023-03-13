#include "ClauseGroups.h"
#include "SimpleEvaluator.h"

spa::ClauseGroups::ClauseGroups(NoSynonymClauseGroup& noSynonymClauseGroup,
                                std::vector<ConnectedSynonymClauseGroup>& connectedSynonymClauseGroups,
                                WithClauseGroup& withClauseGroup) :
  noSynonymClauseGroup(noSynonymClauseGroup),
  connectedSynonymClauseGroups(connectedSynonymClauseGroups),
  withClauseGroup(withClauseGroup) {
}

spa::QpsResultTable spa::ClauseGroups::evaluate(PKBManager& pkbManager,
                                                std::unordered_map<std::string, DesignEntityType>& selectWithDeclarations) {
  if (!noSynonymClauseGroup.isEmpty()) {
    QpsResultTable noSynonymResultTable = noSynonymClauseGroup.evaluate(pkbManager);
    if (noSynonymResultTable.isEmpty()) {
      return noSynonymResultTable;
    }
  }

  std::vector<QpsResultTable> resultTables;
  for (auto& group : connectedSynonymClauseGroups) {
    resultTables.push_back(group.evaluate(pkbManager, selectWithDeclarations));
  }

  for (auto& declaration : selectWithDeclarations) {
    std::string declarationSynonym = declaration.first;
    DesignEntityType declarationType = declaration.second;
    std::unique_ptr<QpsEvaluator> evaluator = std::make_unique<SimpleEvaluator>(declarationSynonym, declarationType);
    resultTables.push_back(evaluator->evaluate(pkbManager));
  }

  QpsResultTable resultTable;
  resultTable.addHeader("");
  resultTable.addRow({ QpsValue(0) });
  for (auto& table : resultTables) {
    resultTable = resultTable.innerJoin(table);
  }

  resultTable = withClauseGroup.evaluate(pkbManager, resultTable);
  return resultTable;
}

spa::NoSynonymClauseGroup& spa::ClauseGroups::getNoSynonynmClauseGroup() {
  return noSynonymClauseGroup;
}

std::vector<spa::ConnectedSynonymClauseGroup>& spa::ClauseGroups::getConnectedSynonymClauseGroups() {
  return connectedSynonymClauseGroups;
}

spa::WithClauseGroup& spa::ClauseGroups::getWithClauseGroup() {
  return withClauseGroup;
}

#include "ClauseGroups.h"
#include "SimpleEvaluator.h"

spa::ClauseGroups::ClauseGroups(NoSynonymClauseGroup& noSynonymClauseGroup,
                                std::vector<ConnectedSynonymClauseGroup>& connectedSynonymClauseGroups,
                                WithAttrAttrClauseGroup& withAttrAttrClauseGroup) :
  noSynonymClauseGroup(noSynonymClauseGroup),
  connectedSynonymClauseGroups(connectedSynonymClauseGroups),
  withAttrAttrClauseGroup(withAttrAttrClauseGroup) {
}

spa::QpsResultTable spa::ClauseGroups::evaluate(PKBManager& pkbManager,
                                                std::unordered_map<std::string, DesignEntityType>& usedDeclarations) {
  if (!noSynonymClauseGroup.isEmpty()) {
    QpsResultTable noSynonymResultTable = noSynonymClauseGroup.evaluate(pkbManager);
    if (noSynonymResultTable.isEmpty()) {
      return noSynonymResultTable;
    }
  }

  std::vector<QpsResultTable> resultTables;
  for (auto& group : connectedSynonymClauseGroups) {
    resultTables.push_back(group.evaluate(pkbManager));
  }

  for (auto& declaration : usedDeclarations) {
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

  resultTable = withAttrAttrClauseGroup.evaluate(pkbManager, resultTable);
  return resultTable;
}

#include "SimpleEvaluator.h"

spa::SimpleEvaluator::SimpleEvaluator(std::string selectSynonym, DesignEntityType designEntityType) :
  selectSynonym(selectSynonym), designEntityType(designEntityType) {
}

spa::QpsResultTable spa::SimpleEvaluator::evaluate(PKBManager& pkbManager) {
  QueryResult result = pkbManager.getEntity(designEntityType);
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);

  if (designEntityType == VARIABLE || designEntityType == PROCEDURE || designEntityType == CONSTANT) {
    for (auto& name : result.getNames()) {
      resultTable.addRow({ QpsValue(name) });
    }
    return resultTable;
  }

  for (auto& lineNumber : result.getLineNumbers()) {
    resultTable.addRow({ QpsValue(lineNumber) });
  }
  return resultTable;
}

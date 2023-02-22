#pragma once

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

#include <string>
#include <functional>
#include <unordered_map>

namespace spa {
class SimpleEvaluator : public QpsEvaluator {
 private:
  std::unordered_map<DesignEntityType, std::function<QpsResultTable(SimpleEvaluator, PKBManager)>> designEntityEvaluateFunctionMap;
  std::string selectSynonym;
  DesignEntityType designEntityType;

 public:
  explicit SimpleEvaluator(std::string selectSynonym, DesignEntityType designEntityType);
  QpsResultTable evaluate(PKBManager& pkbManager);
  QpsResultTable evaluateProcedure(PKBManager& pkbManager);
  QpsResultTable evaluateStmt(PKBManager& pkbManager);
  QpsResultTable evaluateRead(PKBManager& pkbManager);
  QpsResultTable evaluatePrint(PKBManager& pkbManager);
  QpsResultTable evaluateAssign(PKBManager& pkbManager);
  QpsResultTable evaluateCall(PKBManager& pkbManager);
  QpsResultTable evaluateWhile(PKBManager& pkbManager);
  QpsResultTable evaluateIf(PKBManager& pkbManager);
  QpsResultTable evaluateVariable(PKBManager& pkbManager);
  QpsResultTable evaluateConstant(PKBManager& pkbManager);
};
}  // namespace spa

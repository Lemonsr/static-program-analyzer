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
  std::unordered_map<DesignEntityType,
                     std::function<QpsResultTable(SimpleEvaluator, PKBManager)>> designEntityEvaluateFunctionMap;
  std::string selectSynonym;
  DesignEntityType designEntityType;
  QpsResultTable evaluateRead(PKBManager& pkbManager);
  QpsResultTable evaluatePrint(PKBManager& pkbManager);
  QpsResultTable evaluateCall(PKBManager& pkbManager);
  QpsResultTable evaluateStatementNumbers(PKBManager& pkbManager);
  QpsResultTable evaluateNames(PKBManager& pkbManager);
  QpsResultTable evaluateConstants(PKBManager& pkbManager);

 public:
  explicit SimpleEvaluator(std::string selectSynonym, DesignEntityType designEntityType);
  QpsResultTable evaluate(PKBManager& pkbManager);
};
}  // namespace spa

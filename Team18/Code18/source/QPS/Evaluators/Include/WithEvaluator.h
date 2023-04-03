#pragma once

#include <string>

#include "PKBManager.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"

namespace spa {
class WithEvaluator : public QpsEvaluator {
 private:
  WithArgument& firstArg;
  WithArgument& secondArg;
  spa::QpsResultTable evaluateAttributes(PKBManager& pkbManager, const std::string& first, const std::string& second);
  spa::QpsResultTable evaluateAttributeValue(const std::string& attribute, const QpsValue& value);
  spa::QpsResultTable evaluateValues(const QpsValue& first, const QpsValue& second);
 public:
  WithEvaluator(WithArgument& firstArg, WithArgument& secondArg);
  QpsResultTable evaluate(PKBManager& pkbManager);
  QpsResultTable filter(QpsResultTable& table);
};
}  // namespace spa

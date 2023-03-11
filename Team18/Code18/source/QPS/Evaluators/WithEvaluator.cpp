#include "WithEvaluator.h"

#include <vector>
#include <stdexcept>

spa::WithEvaluator::WithEvaluator(WithArgument& firstArg, WithArgument& secondArg): firstArg(firstArg),
                                                                                    secondArg(secondArg) {
}

spa::QpsResultTable spa::WithEvaluator::evaluateAttributeValue(const std::string& attribute,
                                                               const QpsValue& value) {
  QpsResultTable result;
  result.addHeader(attribute);
  result.addRow({ value });
  return result;
}

spa::QpsResultTable spa::WithEvaluator::evaluateValues(const QpsValue& first,
                                                       const QpsValue& second) {
  QpsResultTable result;
  result.addHeader("");
  if (first == second) {
    result.addRow({ first });
  }
  return result;
}

spa::QpsResultTable spa::WithEvaluator::evaluate(PKBManager& pkbManager) {
  std::vector<const std::string*> attributes;
  std::vector<const QpsValue*> values;
  std::vector<WithArgument*> args { &firstArg, &secondArg };
  for (auto arg : args) {
    if (arg->getType() == WithArgumentType::WITH_VALUE) {
      values.push_back(&(arg->getValue()));
    } else {
      attributes.push_back(&(arg->getAttribute()));
    }
  }
  if (attributes.size() == 2) {
    throw std::runtime_error("Evaluating with two attributes");
  }
  if (attributes.size() == 1) {
    return evaluateAttributeValue(*(attributes[0]), *(values[0]));
  }
  return evaluateValues(*(values[0]), *(values[1]));
}

spa::QpsResultTable spa::WithEvaluator::filter(QpsResultTable& table) {
  return table.matchColumns(firstArg.getAttribute(), secondArg.getAttribute());
}

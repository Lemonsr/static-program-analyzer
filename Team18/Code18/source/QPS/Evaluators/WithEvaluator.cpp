#include "WithEvaluator.h"
#include "Literal.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <cassert>

spa::WithEvaluator::WithEvaluator(WithArgument& firstArg, WithArgument& secondArg): firstArg(firstArg),
                                                                                    secondArg(secondArg) {
}

spa::QpsResultTable spa::WithEvaluator::evaluateAttributes(PKBManager& pkbManager,
                                                           const std::string& first,
                                                           const std::string& second) {
  std::vector<QpsValue> values;
  std::string attribute = first.substr(first.find('.') + 1);
  if (attribute == STMT_NUM_ATTR_LITERAL) {
    auto result = pkbManager.getEntity(STMT);
    for (int num : result.getLineNumbers()) {
      values.push_back(QpsValue(num));
    }
  } else if (attribute == VALUE_ATTR_LITERAL) {
    auto result = pkbManager.getEntity(CONSTANT);
    for (auto& constant : result.getNames()) {
      values.push_back(QpsValue(std::stoi(constant)));
    }
  } else if (attribute == PROC_NAME_ATTR_LITERAL) {
    auto result = pkbManager.getEntity(PROCEDURE);
    for (auto& name : result.getNames()) {
      values.push_back(QpsValue(name));
    }
  } else {
    auto result = pkbManager.getEntity(VARIABLE);
    for (auto& name : result.getNames()) {
      values.push_back(QpsValue(name));
    }
  }
  QpsResultTable table;
  table.addHeader(first);
  table.addHeader(second);
  for (auto& val : values) {
    table.addRow({ val, val });
  }
  return table;
}

spa::QpsResultTable spa::WithEvaluator::evaluateAttributeValue(const std::string& attribute, const QpsValue& value) {
  QpsResultTable result;
  result.addHeader(attribute);
  result.addRow({ value });
  return result;
}

spa::QpsResultTable spa::WithEvaluator::evaluateValues(const QpsValue& first, const QpsValue& second) {
  QpsResultTable result;
  result.addEmptyHeader();
  if (first == second) {
    result.addRow({ first });
  }
  return result;
}

spa::QpsResultTable spa::WithEvaluator::evaluate(PKBManager& pkbManager) {
  std::vector<const std::string*> attributes;
  std::vector<const QpsValue*> values;
  std::vector<WithArgument*> args{ &firstArg, &secondArg };
  for (auto arg : args) {
    if (arg->getType() == WithArgumentType::WITH_VALUE) {
      values.push_back(&(arg->getValue()));
    } else {
      attributes.push_back(&(arg->getAttribute()));
    }
  }
  if (attributes.size() == 2) {
    return evaluateAttributes(pkbManager, *(attributes[0]), *(attributes[1]));
  } else if (attributes.size() == 1) {
    return evaluateAttributeValue(*(attributes[0]), *(values[0]));
  }
  return evaluateValues(*(values[0]), *(values[1]));
}

spa::QpsResultTable spa::WithEvaluator::filter(QpsResultTable& table) {
  return table.matchColumns(firstArg.getAttribute(), secondArg.getAttribute());
}

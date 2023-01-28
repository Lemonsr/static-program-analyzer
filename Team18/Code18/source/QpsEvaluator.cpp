#include <iostream>

#include "QpsEvaluator.h"

spa::QpsEvaluator::QpsEvaluator(ParsedQuery queryObject)
  : queryObject(queryObject) {
}

spa::QueryResult spa::QpsEvaluator::evaluate() {
  std::cout << "evaluate" << std::endl;
  return QueryResult{};
}

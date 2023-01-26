#include <iostream>

#include "QpsEvaluator.h"

QpsEvaluator::QpsEvaluator(QueryObject queryObject) : queryObject(queryObject) {
}

QueryResult QpsEvaluator::evaluate() {
  std::cout << "evaluate" << std::endl;
  return QueryResult{};
}

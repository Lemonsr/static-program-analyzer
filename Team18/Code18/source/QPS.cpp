#include <string>

#include "QPS.h"

std::string QPS::evaluate(std::string query) {
  QpsPreprocessor* preprocessor = new QpsPreprocessor();
  QueryObject queryObject = preprocessor->preprocess(query);

  QpsEvaluator* evaluator = new QpsEvaluator(queryObject);
  QueryResult queryResult = evaluator->evaluate();
  return "";
}

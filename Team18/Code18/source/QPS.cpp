#include <string>

#include "QPS.h"

std::string spa::QPS::evaluate(std::string query) {
  QpsPreprocessor* preprocessor = new spa::QpsPreprocessor();
  ParsedQuery parsedQuery = preprocessor->preprocess(query);

  QpsEvaluator* evaluator = new QpsEvaluator(parsedQuery);
  QueryResult queryResult = evaluator->evaluate();
  return "";
}

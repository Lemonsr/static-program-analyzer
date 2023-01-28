#include <string>

#include "QPS.h"
#include "ParsedQuery.h"

std::string spa::QPS::evaluate(std::string query) {
  QpsPreprocessor preprocessor;
  ParsedQuery parsedQuery = preprocessor.preprocess(query);

  QpsEvaluator evaluator(parsedQuery);
  QueryResult queryResult = evaluator.evaluate();
  return "";
}

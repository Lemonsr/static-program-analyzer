#include <string>
#include <optional>

#include "QPS.h"
#include "ParsedQuery.h"

std::string spa::QPS::evaluate(std::string query) {
  QpsPreprocessor preprocessor;
  std::optional<ParsedQuery> queryOpt = preprocessor.preprocess(query);
  if (!queryOpt) {
    return "Syntax error in query";
  }
  QpsEvaluator evaluator(queryOpt.value());
  QueryResult queryResult = evaluator.evaluate();
  return "";
}

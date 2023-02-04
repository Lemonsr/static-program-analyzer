#include <string>
#include <optional>
#include <memory>

#include "QPS.h"
#include "ParsedQuery.h"
#include "QpsQueryEvaluator.h"
#include "QpsTranslator.h"

spa::QpsResult spa::QPS::evaluate(std::string query, PKBManager& pkbManager) {
  QpsPreprocessor preprocessor;
  std::optional<ParsedQuery> queryOpt = preprocessor.preprocess(query);
  QpsResult result;
  if (!queryOpt) {
    result.setErrorMessage("Syntax error in query");
    return result;
  }

  std::unique_ptr<QpsEvaluator> qpsEvaluator = std::make_unique<QpsQueryEvaluator>(queryOpt.value());
  QpsResultTable resultTable = qpsEvaluator->evaluate(pkbManager);

  QpsTranslator translator(resultTable);
  std::list<std::string> translatedResult = translator.translate(queryOpt.value().getSelectSynonym());
  result.setResults(translatedResult);
  return result;
}

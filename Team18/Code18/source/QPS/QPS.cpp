#include <string>
#include <utility>
#include <memory>

#include "QPS.h"
#include "ParsedQuery.h"
#include "QpsQueryEvaluator.h"
#include "QpsTranslator.h"
#include "PqlSemanticChecker.h"

spa::QpsResult spa::QPS::evaluate(std::string query, PKBManager& pkbManager) {
  QpsPreprocessor preprocessor;
  std::pair<PqlParseStatus, ParsedQuery> parseResult = preprocessor.preprocess(query);
  QpsResult result;
  if (parseResult.first == PQL_PARSE_SYNTAX_ERROR) {
    result.setErrorMessage("SyntaxError");
    return result;
  }

  ParsedQuery& parsedQuery = parseResult.second;

  PqlSemanticChecker pqlSemanticChecker;
  bool isValid = pqlSemanticChecker.isSemanticallyValid(parsedQuery);
  if (!isValid) {
    result.setErrorMessage("SemanticError");
    return result;
  }

  std::unique_ptr<QpsEvaluator> qpsEvaluator = std::make_unique<QpsQueryEvaluator>(parsedQuery);
  QpsResultTable resultTable = qpsEvaluator->evaluate(pkbManager);

  QpsTranslator translator(resultTable);
  std::list<std::string> translatedResult = translator.translate(parsedQuery.getSelectColumns()[0]);
  result.setResults(translatedResult);
  return result;
}

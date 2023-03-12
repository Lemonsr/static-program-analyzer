#include "QpsPreprocessor.h"

#include <sstream>
#include <optional>
#include <iostream>

#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"
#include "PqlQueryParser.h"
#include "ParsedQuery.h"
#include "PqlSemanticChecker.h"

std::pair<spa::PqlParseStatus, spa::ParsedQuery> spa::QpsPreprocessor::preprocess(std::string query) {
  std::stringstream ss;
  ss.str(query);
  Tokenizer tokenizer;
  Stream<Token> tokens;
  try {
    tokens = tokenizer.tokenize(ss);
  } catch (std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return { PQL_PARSE_SYNTAX_ERROR, {} };
  }
  ParsedQuery parsedQuery;
  PqlQueryParser parser;
  PqlParseStatus status = parser.parse(tokens, parsedQuery);
  if (status != PQL_PARSE_SUCCESS) {
    return { status, parsedQuery };
  }

  PqlSemanticChecker pqlSemanticChecker;
  bool isValid = pqlSemanticChecker.isSemanticallyValid(parsedQuery);
  if (!isValid) {
    return { PQL_PARSE_SEMANTIC_ERROR, parsedQuery };
  }

  return { status, parsedQuery };
}

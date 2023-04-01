#include "QpsPreprocessor.h"

#include <sstream>
#include <iostream>

#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"
#include "PqlQueryParser.h"
#include "ParsedQuery.h"
#include "PqlSemanticChecker.h"

std::pair<spa::PqlParseStatus, spa::ParsedQuery> spa::QpsPreprocessor::preprocess(std::string query) {

  std::optional<Stream<Token>> tokens = tokenize(query);
  if (!tokens) {
    return { PQL_PARSE_SYNTAX_ERROR, {} };
  }
  
  ParsedQuery parsedQuery;
  PqlParseStatus status = parse(tokens.value(), parsedQuery);
  if (status != PQL_PARSE_SUCCESS) {
    return { status, parsedQuery };
  }

  bool isSemanticallyValid = checkSemantics(parsedQuery);
  if (!isSemanticallyValid) {
    return { PQL_PARSE_SEMANTIC_ERROR, parsedQuery };
  }

  return { status, parsedQuery };
}

std::optional<spa::Stream<spa::Token>> spa::QpsPreprocessor::tokenize(std::string query) {
  std::stringstream ss;
  ss.str(query);
  Tokenizer tokenizer;
  Stream<Token> tokens;
  try {
    tokens = tokenizer.tokenize(ss);
  }
  catch (std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return {};
  }
  return tokens;
}

spa::PqlParseStatus spa::QpsPreprocessor::parse(Stream<Token>& tokens, ParsedQuery& query) {
  PqlQueryParser parser;
  PqlParseStatus status = parser.parse(tokens, query);
  return status;
}

bool spa::QpsPreprocessor::checkSemantics(ParsedQuery& query) {
  PqlSemanticChecker pqlSemanticChecker;
  bool isValid = pqlSemanticChecker.isSemanticallyValid(query);
  return isValid;
}

#include "QpsPreprocessor.h"

#include <sstream>
#include <optional>
#include <iostream>

#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"
#include "PqlQueryParser.h"
#include "ParsedQuery.h"

std::pair<spa::PqlParseStatus, spa::ParsedQuery> spa::QpsPreprocessor::preprocess(std::string query) {
  std::stringstream ss;
  ss.str(query);
  Tokenizer tokenizer;
  Stream<Token> tokens;
  try {
    tokens = tokenizer.tokenize(ss);
  } catch (std::runtime_error& e) {
    return { PQL_PARSE_SYNTAX_ERROR, {} };
  }
  ParsedQuery parsedQuery;
  PqlQueryParser parser;
  PqlParseStatus status = parser.parse(tokens, parsedQuery);
  return { status, parsedQuery };
}

#include "QpsPreprocessor.h"

#include <sstream>
#include <optional>
#include <iostream>

#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"
#include "PqlQueryParser.h"
#include "ParsedQuery.h"

std::optional<spa::ParsedQuery> spa::QpsPreprocessor::preprocess(
    std::string query
) {
  std::stringstream ss;
  ss.str(query);
  Tokenizer tokenizer;
  Stream<Token> tokens;
  try {
    tokens = tokenizer.tokenize(ss);
  }
  catch (std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
  ParsedQuery parsedQuery;
  PqlQueryParser parser;
  if (parser.parse(tokens, parsedQuery) == PQL_PARSE_SUCCESS) {
    return { parsedQuery };
  }
  return {};
}

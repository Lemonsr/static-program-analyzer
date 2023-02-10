#include <vector>
#include <sstream>
#include <optional>

#include "QpsPreprocessor.h"
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
  Stream<Token> tokens = tokenizer.tokenize(ss);
  ParsedQuery parsedQuery;
  PqlQueryParser parser;
  if (parser.parse(tokens, parsedQuery) == PQL_PARSE_SUCCESS) {
    return { parsedQuery };
  }
  return {};
}

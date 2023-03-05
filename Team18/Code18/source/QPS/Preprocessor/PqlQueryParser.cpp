#include "PqlQueryParser.h"

#include <unordered_set>

spa::PqlParseStatus spa::PqlQueryParser::parseDeclarations(
    Stream<Token>& tokens,
    ParsedQuery& query
) {
  while (tokens.remaining() > 0) {
    PqlParseStatus status = declareParser.parse(tokens, query);
    if (status == PQL_PARSE_SYNTAX_ERROR) {
      return PQL_PARSE_SYNTAX_ERROR;
    } else if (status == PQL_PARSE_MISMATCH) {
      break;
    }
  }
  return PQL_PARSE_SUCCESS;
}

spa::PqlParseStatus spa::PqlQueryParser::parseClauses(Stream<Token>& tokens,
                                                      ParsedQuery& query) {
  std::unordered_set<PqlParser*> parsers {
    &suchThatParser,
    &patternParser,
    &andParser,
    &withParser
  };
  while (tokens.remaining() > 0) {
    bool parserUsed = false;
    for (auto parser : parsers) {
      PqlParseStatus status = parser->parse(tokens, query);
      if (status == PQL_PARSE_SUCCESS) {
        parserUsed = true;
        break;
      } else if (status != PQL_PARSE_MISMATCH) {
        return status;
      }
    }
    if (!parserUsed) {
      return PQL_PARSE_SYNTAX_ERROR;
    }
  }
  return PQL_PARSE_SUCCESS;
}

spa::PqlParseStatus spa::PqlQueryParser::parse(Stream<Token>& tokens,
                                               ParsedQuery& query) {
  if (parseDeclarations(tokens, query) == PQL_PARSE_SYNTAX_ERROR) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  if (selectParser.parse(tokens, query) != PQL_PARSE_SUCCESS) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  return parseClauses(tokens, query);
}

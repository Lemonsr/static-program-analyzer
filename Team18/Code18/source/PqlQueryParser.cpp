#include "PqlQueryParser.h"

#include <unordered_map> 

spa::PqlParseStatus spa::PqlQueryParser::parseDeclarations(
    Stream<Token>& tokens,
    ParsedQuery& query
) {
  while (tokens.remaining() > 0) {
    PqlParseStatus status = declareParser.parse(tokens, query);
    if (status == PQL_PARSE_ERROR) {
      return PQL_PARSE_ERROR;
    } else if (status == PQL_PARSE_MISMATCH) {
      break;
    }
  }
  return PQL_PARSE_SUCCESS;
}

spa::PqlParseStatus spa::PqlQueryParser::parseSelect(
    Stream<Token>& tokens,
    ParsedQuery& query
) {
  bool matchResult = tokens.match({
    { TOKEN_NAME, "Select" },
    { TOKEN_NAME, "" },
    });
  if (!matchResult) {
    return PQL_PARSE_ERROR;
  }
  if (!query.setSelectSynonym(tokens[1].getValue())) {
    return PQL_PARSE_ERROR;
  }
  tokens.seek(2);
  return PQL_PARSE_SUCCESS;
}

spa::PqlParseStatus spa::PqlQueryParser::parseClauses(Stream<Token>& tokens,
                                                      ParsedQuery& query) {
  std::unordered_map<PqlParser*, bool> parsersUseMap {
    { &suchThatParser, false },
    { &patternParser, false }
  };
  while (tokens.remaining() > 0) {
    bool parserUsed = false;
    for (auto it = parsersUseMap.begin(); it != parsersUseMap.end(); ++it) {
      if (it->second) {
        continue;
      }
      PqlParser* parser = it->first;
      PqlParseStatus status = parser->parse(tokens, query);
      if (status == PQL_PARSE_ERROR) {
        return PQL_PARSE_ERROR;
      }
      else if (status == PQL_PARSE_SUCCESS) {
        it->second = true;
        parserUsed = true;
      }
    }
    if (!parserUsed) {
      break;
    }
  }
  if (tokens.remaining() > 0) {
    return PQL_PARSE_ERROR;
  }
  return PQL_PARSE_SUCCESS;
}

spa::PqlParseStatus spa::PqlQueryParser::parse(Stream<Token>& tokens,
                                               ParsedQuery& query) {
  if (parseDeclarations(tokens, query) == PQL_PARSE_ERROR) {
    return PQL_PARSE_ERROR;
  }
  if (parseSelect(tokens, query) == PQL_PARSE_ERROR) {
    return PQL_PARSE_ERROR;
  }
  return parseClauses(tokens, query);
}

#include "PqlDeclarationParser.h"
#include "DesignEntity.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

const std::unordered_map<std::string, spa::DesignEntityType> entityTypesMap {
  {"procedure", spa::PROCEDURE},
  {"stmt", spa::STMT},
  {"read", spa::READ},
  {"print", spa::PRINT},
  {"assign", spa::ASSIGN},
  {"call", spa::CALL},
  {"while", spa::WHILE},
  {"if", spa::IF},
  {"variable", spa::VARIABLE},
  {"constant", spa::CONSTANT}
};

spa::PqlParseStatus spa::PqlDeclarationParser::addDeclarations(
    DesignEntityType type,
    Stream<Token>& tokens,
    ParsedQuery& query
) {
  while (tokens.remaining() >= 2) {
    if (tokens[0].getType() != TOKEN_NAME) {
      return PQL_PARSE_ERROR;
    }
    if (!query.addDeclaration(tokens[0].getValue(), type)) {
      return PQL_PARSE_ERROR;
    }
    if (tokens[1].getType() == TOKEN_SEMICOLON) {
      return PQL_PARSE_SUCCESS;
    }
    if (tokens[1].getType() != TOKEN_COMMA) {
      return PQL_PARSE_ERROR;
    }
    tokens.seek(2);
  }
  return PQL_PARSE_ERROR;
}

spa::PqlParseStatus spa::PqlDeclarationParser::parse(Stream<Token>& tokens,
                                                     ParsedQuery& query) {
  if (tokens[0].getType() != TOKEN_NAME) {
    return PQL_PARSE_MISMATCH;
  }
  auto it = entityTypesMap.find(tokens[0].getValue());
  if (it == entityTypesMap.end()) {
    return PQL_PARSE_MISMATCH;
  }
  DesignEntityType type = it->second;
  tokens.seek(1);
  return addDeclarations(type, tokens, query);
}

#include "PqlDeclarationParser.h"
#include "DesignEntity.h"
#include "Literal.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

const std::unordered_map<std::string, spa::DesignEntityType> entityTypesMap {
  {PROCEDURE_LITERAL, spa::PROCEDURE},
  {STMT_LITERAL, spa::STMT},
  {READ_LITERAL, spa::READ},
  {PRINT_LITERAL, spa::PRINT},
  {ASSIGN_LITERAL, spa::ASSIGN},
  {CALL_LITERAL, spa::CALL},
  {WHILE_LITERAL, spa::WHILE},
  {IF_LITERAL, spa::IF},
  {VARIABLE_LITERAL, spa::VARIABLE},
  {CONSTANT_LITERAL, spa::CONSTANT}
};

spa::PqlParseStatus spa::PqlDeclarationParser::addDeclarations(DesignEntityType type,
                                                               Stream<Token>& tokens,
                                                               ParsedQuery& query
) {
  while (tokens.remaining() >= 2) {
    if (tokens[0].getType() != TOKEN_NAME) {
      return PQL_PARSE_SYNTAX_ERROR;
    }
    query.addDeclaration(tokens[0].getValue(), type);
    TokenType tokenType = tokens[1].getType();
    if (tokenType != TOKEN_SEMICOLON && tokenType != TOKEN_COMMA) {
      return PQL_PARSE_SYNTAX_ERROR;
    }
    tokens.seek(2);
    if (tokenType == TOKEN_SEMICOLON) {
      return PQL_PARSE_SUCCESS;
    }
  }
  return PQL_PARSE_SYNTAX_ERROR;
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

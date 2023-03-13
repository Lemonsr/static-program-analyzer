#include "PqlSelectParser.h"

#include <string>

spa::PqlParseStatus spa::PqlSelectParser::parseSynonymOrAttribute(bool parseBoolean,
                                                                  Stream<Token>& tokens,
                                                                  ParsedQuery& query) {
  std::optional<std::string> attributeOpt = attributeParser.parse(tokens, query);
  if (attributeOpt) {
    query.setSelectClauseType(SelectClauseType::SELECT_TUPLE);
    query.addSelectColumn(attributeOpt.value());
    return PQL_PARSE_SUCCESS;
  }
  if (!tokens.match({ {TOKEN_NAME, ""} })) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  std::string synonym = tokens[0].getValue();
  tokens.seek(1);

  DesignEntityType entityType = query.getDeclarationType(synonym);
  if (parseBoolean && synonym == "BOOLEAN" && entityType == UNKNOWN) {
    query.setSelectClauseType(SelectClauseType::SELECT_BOOLEAN);
    return PQL_PARSE_SUCCESS;
  }
  query.setSelectClauseType(SelectClauseType::SELECT_TUPLE);
  query.addSelectColumn(synonym);
  query.addUsedDeclaration(synonym, entityType);
  query.addSelectWithDeclaration(synonym, entityType);
  return PQL_PARSE_SUCCESS;
}

spa::PqlParseStatus spa::PqlSelectParser::parseTuple(Stream<Token>& tokens, ParsedQuery& query) {
  tokens.seek(1);
  while (tokens.remaining() > 0) {
    PqlParseStatus status = parseSynonymOrAttribute(false, tokens, query);
    if (status != PQL_PARSE_SUCCESS || tokens.remaining() == 0) {
      break;
    }
    Token& token = tokens[0];
    tokens.seek(1);
    if (token.getType() == TOKEN_COND_GT) {
      return PQL_PARSE_SUCCESS;
    } else if (token.getType() != TOKEN_COMMA) {
      break;
    }
  }
  return PQL_PARSE_SYNTAX_ERROR;
}

spa::PqlParseStatus spa::PqlSelectParser::parse(Stream<Token>& tokens, ParsedQuery& query) {
  bool matchStatus = tokens.match({
    { TOKEN_NAME, "Select"}
  });
  if (!matchStatus) {
    return PQL_PARSE_MISMATCH;
  }
  tokens.seek(1);
  if (tokens.match({ { TOKEN_COND_LT, "<"} })) {
    return parseTuple(tokens, query);
  }
  return parseSynonymOrAttribute(true, tokens, query);
}

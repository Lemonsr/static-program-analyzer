#include "PqlArgumentParser.h"

#include <string>

#include "Literal.h"

std::optional<spa::PqlArgument> spa::PqlArgumentParser::getSynonym(
    Stream<Token>& tokens,
    ParsedQuery& query
) {
  if (!tokens.match({ { TOKEN_NAME, EMPTY_LITERAL} })) {
    return {};
  }
  std::string val = tokens[0].getValue();
  DesignEntityType entityType = query.getDeclarationType(val);
  tokens.seek(1);
  query.addUsedDeclaration(val, entityType);
  return { { SYNONYM, val, entityType } };
}

std::optional<spa::PqlArgument> spa::PqlArgumentParser::getWildcard(
    Stream<Token>& tokens
) {
  if (!tokens.match({ { TOKEN_UNDERSCORE, UNDERSCORE_LITERAL} })) {
    return {};
  }
  tokens.seek(1);
  return { { WILDCARD, UNDERSCORE_LITERAL, {} } };
}

std::optional<spa::PqlArgument> spa::PqlArgumentParser::getLineNo(
  Stream<Token>& tokens
) {
  if (!tokens.match({ { TOKEN_INTEGER, EMPTY_LITERAL} })) {
    return {};
  }
  std::string val = tokens[0].getValue();
  tokens.seek(1);
  return { { LINE_NO, val, {}}};
}

std::optional<spa::PqlArgument> spa::PqlArgumentParser::getLiteralString(
  Stream<Token>& tokens
) {
  bool matchResult = tokens.match({
    {TOKEN_DOUBLE_QUOTES, DOUBLE_QUOTES_LITERAL},
    {TOKEN_NAME, EMPTY_LITERAL},
    {TOKEN_DOUBLE_QUOTES, DOUBLE_QUOTES_LITERAL}
  });
  if (!matchResult) {
    return {};
  }
  std::string val = tokens[1].getValue();
  tokens.seek(3);
  return { { LITERAL_STRING, val, {} } };
}

std::optional<spa::PqlArgument> spa::PqlArgumentParser::parse(
    Stream<Token>& tokens,
    ParsedQuery& query
) {
  std::optional<spa::PqlArgument> sResult = getSynonym(tokens, query);
  if (sResult) {
    return sResult;
  }
  std::optional<spa::PqlArgument> wResult = getWildcard(tokens);
  if (wResult) {
    return wResult;
  }
  std::optional<spa::PqlArgument> lResult = getLineNo(tokens);
  if (lResult) {
    return lResult;
  }
  std::optional<spa::PqlArgument> vResult = getLiteralString(tokens);
  if (vResult) {
    return vResult;
  }
  return {};
}

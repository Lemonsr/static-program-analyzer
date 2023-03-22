#include "PqlArgumentParser.h"

#include <string>

std::optional<spa::PqlArgument> spa::PqlArgumentParser::getSynonym(
    Stream<Token>& tokens,
    ParsedQuery& query
) {
  if (!tokens.match({ { TOKEN_NAME, ""} })) {
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
  if (!tokens.match({ { TOKEN_UNDERSCORE, "_"} })) {
    return {};
  }
  tokens.seek(1);
  return { { WILDCARD, "_", {} } };
}

std::optional<spa::PqlArgument> spa::PqlArgumentParser::getLineNo(
  Stream<Token>& tokens
) {
  if (!tokens.match({ { TOKEN_INTEGER, ""} })) {
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
    {TOKEN_DOUBLE_QUOTES, "\""},
    {TOKEN_NAME, ""},
    {TOKEN_DOUBLE_QUOTES, "\""}
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

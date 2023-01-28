#include "PqlArgumentParser.h"

std::optional<spa::PqlArgument> spa::PqlArgumentParser::getSynonym(
    Stream<Token>& tokens,
    ParsedQuery& query
) {
  if (!tokens.match({ { TOKEN_NAME, ""} })) {
    return {};
  }
  std::string val = tokens[0].getValue();
  auto op = query.getType(val);
  if (!op) {
    return {};
  }
  tokens.seek(1);
  return { { SYNONYM, val, op } };
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

std::optional<spa::PqlArgument> spa::PqlArgumentParser::getVariableName(
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
  return { { VARIABLE_NAME, val, {} } };
}

std::optional<spa::PqlArgument> spa::PqlArgumentParser::parse(
    Stream<Token>& tokens,
    ParsedQuery& query
) {
  std::optional<spa::PqlArgument> result = getSynonym(tokens, query);
  if (result) {
    return result;
  }
  std::optional<spa::PqlArgument> result = getWildcard(tokens);
  if (result) {
    return result;
  }
  std::optional<spa::PqlArgument> result = getLineNo(tokens);
  if (result) {
    return result;
  }
  std::optional<spa::PqlArgument> result = getVariableName(tokens);
  if (result) {
    return result;
  }
  return {};
}

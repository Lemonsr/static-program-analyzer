#include "PqlWithSubParser.h"

#include <optional>
#include <string>

std::optional<spa::WithArgument> spa::PqlWithSubParser::parseArgument(Stream<Token>& tokens,
                                                                      ParsedQuery& query) {
  std::optional<std::string> attribOpt = attributeParser.parse(tokens, query);
  if (attribOpt) {
    return { WithArgument(attribOpt.value()) };
  }
  if (tokens.match({ { TOKEN_INTEGER, ""} })) {
    WithArgument arg(QpsValue(std::stoi(tokens[0].getValue())));
    tokens.seek(1);
    return { arg };
  }
  bool matchResult = tokens.match({
    {TOKEN_DOUBLE_QUOTES, "\""},
    {TOKEN_NAME, ""},
    {TOKEN_DOUBLE_QUOTES, "\""}
  });
  if (matchResult) {
    WithArgument arg(QpsValue(tokens[1].getValue()));
    tokens.seek(3);
    return { arg };
  }
  return {};
}

spa::PqlParseStatus spa::PqlWithSubParser::parse(Stream<Token>& tokens, ParsedQuery& query) {
  std::optional<WithArgument> firstOpt = parseArgument(tokens, query);
  if (!firstOpt.has_value()) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  if (!tokens.match({ { TOKEN_EQUAL, "="} })) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  tokens.seek(1);
  std::optional<WithArgument> secondOpt = parseArgument(tokens, query);
  if (!secondOpt.has_value()) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  query.addWithClause({ firstOpt.value(), secondOpt.value() });
  return PQL_PARSE_SUCCESS;
}

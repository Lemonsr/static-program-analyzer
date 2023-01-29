#include "PqlPatternParser.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "ParsedQuery.h"
#include "Token.h"

std::optional<spa::Pattern> spa::PqlPatternParser::parseWildcard(
    Stream<Token>& tokens,
    ParsedQuery& query
) {
  if (!tokens.match({ { TOKEN_UNDERSCORE, "_" } })) {
    return {};
  }
  tokens.seek(1);
  return { Pattern { ANY } };
}

std::optional<spa::Pattern> spa::PqlPatternParser::parseExact(
    Stream<Token>& tokens,
    ParsedQuery& query
) {
  if (!tokens.match({ { TOKEN_DOUBLE_QUOTES, "\"" } })) {
    return {};
  }
  tokens.seek(1);
  std::vector<Token> patternTokens;
  while (tokens.remaining() > 0) {
    if (tokens[0] == Token { TOKEN_DOUBLE_QUOTES, "\"" }) {
      break;
    }
    patternTokens.push_back(tokens[0]);
    tokens.seek(1);
  }
  if (!tokens.match({ { TOKEN_DOUBLE_QUOTES, "\"" } })) {
    return {};
  }
  tokens.seek(1);
  return { Pattern { EXACT, patternTokens } };
}

std::optional<spa::Pattern> spa::PqlPatternParser::parsePartial(
    Stream<Token>& tokens,
    ParsedQuery& query
) {
  bool matchResult = tokens.match({
    { TOKEN_UNDERSCORE, "_" },
    { TOKEN_DOUBLE_QUOTES, "\"" }
  });
  if (!matchResult) {
    return {};
  }
  tokens.seek(2);
  std::vector<Token> patternTokens;
  while (tokens.remaining() > 0) {
    if (tokens[0] == Token{ TOKEN_DOUBLE_QUOTES, "\"" }) {
      break;
    }
    patternTokens.push_back(tokens[0]);
    tokens.seek(1);
  }
  matchResult = tokens.match({
    { TOKEN_DOUBLE_QUOTES, "\"" },
    { TOKEN_UNDERSCORE, "_" }
  });
  if (!matchResult) {
    return {};
  }
  tokens.seek(2);
  return { Pattern { PARTIAL, patternTokens } };
}

std::optional<spa::Pattern> spa::PqlPatternParser::parsePattern(
    Stream<Token>& tokens,
    ParsedQuery& query
) {
  std::optional<Pattern> wOptional = parseWildcard(tokens, query);
  if (wOptional) {
    return wOptional;
  }
  std::optional<Pattern> eOptional = parseExact(tokens, query);
  if (eOptional) {
    return eOptional;
  }
  std::optional<Pattern> pOptional = parsePartial(tokens, query);
  if (pOptional) {
    return pOptional;
  }
  return {};
}

spa::PqlParseStatus spa::PqlPatternParser::parse(Stream<Token>& tokens,
                                                 ParsedQuery& query) {
  bool matchResult = tokens.match({
    { spa::TOKEN_NAME, "pattern"},
    { spa::TOKEN_NAME, ""},
    { spa::TOKEN_OPEN_BRACKET, "("},
  });
  if (!matchResult) {
    return PQL_PARSE_MISMATCH;
  }
  std::string synAssign = tokens[1].getValue();
  std::optional<DesignEntityType> synOpt = query.getType(synAssign);
  if (!synOpt || synOpt.value() != ASSIGN) {
    return PQL_PARSE_ERROR;
  }
  PqlArgument assignSynonym(SYNONYM, synAssign, { ASSIGN });
  tokens.seek(3);
  std::optional<PqlArgument> firstOpt = argParser.parse(tokens, query);
  if (!firstOpt) {
    return PQL_PARSE_ERROR;
  }
  if (!tokens.match({{ spa::TOKEN_COMMA, ","}})) {
    return PQL_PARSE_ERROR;
  }
  tokens.seek(1);
  std::optional<Pattern> patternOpt = parsePattern(tokens, query);
  if (!patternOpt) {
    return PQL_PARSE_ERROR;
  }
  if (!tokens.match({ { spa::TOKEN_CLOSE_BRACKET, ")"} })) {
    return PQL_PARSE_ERROR;
  }
  tokens.seek(1);
  query.setPatternClause(
    { assignSynonym, firstOpt.value(), patternOpt.value() });
  return PQL_PARSE_SUCCESS;
}

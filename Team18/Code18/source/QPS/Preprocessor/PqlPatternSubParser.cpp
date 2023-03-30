#include "PqlPatternSubParser.h"

#include <cstdint>
#include <string>
#include <unordered_set>
#include <vector>

#include "ParsedQuery.h"
#include "UtilsFunction.h"
#include "Token.h"
#include "PqlRefTypes.h"
#include "Literal.h"

std::optional<spa::Pattern> spa::PqlPatternSubParser::parseExact(
  Stream<Token>& tokens,
  ParsedQuery& query
) {
  if (!tokens.match({ { TOKEN_DOUBLE_QUOTES, DOUBLE_QUOTES_LITERAL } })) {
    return {};
  }
  tokens.seek(1);
  std::vector<Token> patternTokens;
  while (tokens.remaining() > 0) {
    if (tokens[0] == Token{ TOKEN_DOUBLE_QUOTES, DOUBLE_QUOTES_LITERAL }) {
      break;
    }
    patternTokens.push_back(tokens[0]);
    tokens.seek(1);
  }
  if (!tokens.match({ { TOKEN_DOUBLE_QUOTES, DOUBLE_QUOTES_LITERAL } })) {
    return {};
  }
  tokens.seek(1);

  if (!UtilsFunction::isValidExpr(patternTokens)) {
    return {};
  }

  return { Pattern { EXACT, patternTokens } };
}

std::optional<spa::Pattern> spa::PqlPatternSubParser::parsePartial(
  Stream<Token>& tokens,
  ParsedQuery& query
) {
  bool matchResult = tokens.match({
    { TOKEN_UNDERSCORE, UNDERSCORE_LITERAL },
    { TOKEN_DOUBLE_QUOTES, DOUBLE_QUOTES_LITERAL }
    });
  if (!matchResult) {
    return {};
  }
  tokens.seek(2);
  std::vector<Token> patternTokens;
  while (tokens.remaining() > 0) {
    if (tokens[0] == Token{ TOKEN_DOUBLE_QUOTES, DOUBLE_QUOTES_LITERAL }) {
      break;
    }
    patternTokens.push_back(tokens[0]);
    tokens.seek(1);
  }
  matchResult = tokens.match({
    { TOKEN_DOUBLE_QUOTES, DOUBLE_QUOTES_LITERAL },
    { TOKEN_UNDERSCORE, UNDERSCORE_LITERAL }
    });
  if (!matchResult) {
    return {};
  }
  tokens.seek(2);

  if (!UtilsFunction::isValidExpr(patternTokens)) {
    return {};
  }

  return { Pattern { PARTIAL, patternTokens } };
}

std::optional<spa::Pattern> spa::PqlPatternSubParser::parsePattern(
  Stream<Token>& tokens,
  ParsedQuery& query
) {
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

spa::PqlParseStatus spa::PqlPatternSubParser::parseOtherArgs(Stream<Token>& tokens,
                                                             ParsedQuery& query,
                                                             PqlArgument& designEntity,
                                                             PqlArgument& firstArg) {
  bool matchSingleWildcard = tokens.match({
    { TOKEN_UNDERSCORE, UNDERSCORE_LITERAL},
    { TOKEN_CLOSE_BRACKET, CLOSE_BRACKET_LITERAL},
    });
  if (matchSingleWildcard) {
    tokens.seek(2);
    query.addPatternClause({ designEntity, firstArg, Pattern { ANY }, 2 });
    return PQL_PARSE_SUCCESS;
  }
  bool matchDoubleWildcard = tokens.match({
    { TOKEN_UNDERSCORE, UNDERSCORE_LITERAL},
    { TOKEN_COMMA, COMMA_LITERAL},
    { TOKEN_UNDERSCORE, UNDERSCORE_LITERAL},
    { TOKEN_CLOSE_BRACKET, CLOSE_BRACKET_LITERAL},
    });
  if (matchDoubleWildcard) {
    tokens.seek(4);
    query.addPatternClause({ designEntity, firstArg, Pattern { ANY }, 3 });
    return PQL_PARSE_SUCCESS;
  }
  auto patternOpt = parsePattern(tokens, query);
  if (!patternOpt) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  if (!tokens.match({ {TOKEN_CLOSE_BRACKET, CLOSE_BRACKET_LITERAL} })) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  tokens.seek(1);
  query.addPatternClause({ designEntity, firstArg, patternOpt.value(), 2});
  return PQL_PARSE_SUCCESS;
}

spa::PqlParseStatus spa::PqlPatternSubParser::parse(Stream<Token>& tokens,
  ParsedQuery& query) {
  bool matchResult = tokens.match({
    { spa::TOKEN_NAME, EMPTY_LITERAL},
    { spa::TOKEN_OPEN_BRACKET, OPEN_BRACKET_LITERAL},
  });
  if (!matchResult) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  std::optional<spa::PqlArgument> designEntityOpt = argParser.parse(tokens, query);
  if (!designEntityOpt) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  spa::PqlArgument& designEntity = designEntityOpt.value();
  tokens.seek(1);
  std::optional<PqlArgument> firstArgOpt = argParser.parse(tokens, query);
  if (!firstArgOpt) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  spa::PqlArgument& firstArg = firstArgOpt.value();
  if (entityRef.find(firstArg.getType()) == entityRef.end()) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  if (!tokens.match({ { spa::TOKEN_COMMA, COMMA_LITERAL} })) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  tokens.seek(1);
  return parseOtherArgs(tokens, query, designEntity, firstArg);
}

#include "PqlPatternSubParser.h"

#include <cstdint>
#include <string>
#include <unordered_set>
#include <vector>

#include "ParsedQuery.h"
#include "Token.h"

std::optional<spa::Pattern> spa::PqlPatternSubParser::parseWildcard(
  Stream<Token>& tokens,
  ParsedQuery& query
) {
  if (!tokens.match({ { TOKEN_UNDERSCORE, "_" } })) {
    return {};
  }
  tokens.seek(1);
  return { Pattern { ANY } };
}

std::optional<spa::Pattern> spa::PqlPatternSubParser::parseExact(
  Stream<Token>& tokens,
  ParsedQuery& query
) {
  if (!tokens.match({ { TOKEN_DOUBLE_QUOTES, "\"" } })) {
    return {};
  }
  tokens.seek(1);
  std::vector<Token> patternTokens;
  while (tokens.remaining() > 0) {
    if (tokens[0] == Token{ TOKEN_DOUBLE_QUOTES, "\"" }) {
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

std::optional<spa::Pattern> spa::PqlPatternSubParser::parsePartial(
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
  std::optional<Pattern> wOptional = parseWildcard(tokens, query);
  if (wOptional) {
    return wOptional;
  }
  return {};
}

const std::unordered_set<spa::DesignEntityType> patternDesignEntities {
  spa::ASSIGN,
  spa::IF,
  spa::WHILE
};

std::optional<spa::PqlArgument> spa::PqlPatternSubParser::parseDesignEntity(Token& token, ParsedQuery& query) {
  std::string synName = token.getValue();
  std::optional<DesignEntityType> synOpt = query.getDeclarationType(synName);
  if (!synOpt || patternDesignEntities.find(synOpt.value()) == patternDesignEntities.end()) {
    return {};
  }
  query.addUsedDeclaration(synName, synOpt.value());
  return { PqlArgument(SYNONYM, synName, synOpt) };
}

spa::PqlParseStatus spa::PqlPatternSubParser::parseAssign(PqlArgument& designEntity, PqlArgument& firstArg,
                                                          Stream<Token>& tokens, ParsedQuery& query) {
  std::optional<Pattern> patternOpt = parsePattern(tokens, query);
  if (!patternOpt) {
    return PQL_PARSE_ERROR;
  }
  if (!tokens.match({ { spa::TOKEN_CLOSE_BRACKET, ")"} })) {
    return PQL_PARSE_ERROR;
  }
  tokens.seek(1);
  query.addPatternClause({ designEntity, firstArg, patternOpt.value() });
  return PQL_PARSE_SUCCESS;
}

spa::PqlParseStatus spa::PqlPatternSubParser::parseWhile(PqlArgument& designEntity, PqlArgument& firstArg,
                                                         Stream<Token>& tokens, ParsedQuery& query) {
  bool matchStatus = tokens.match({
    { TOKEN_UNDERSCORE, "_" },
    { TOKEN_CLOSE_BRACKET, ")" }
  });
  if (!matchStatus) {
    return PQL_PARSE_ERROR;
  }
  tokens.seek(2);
  query.addPatternClause({ designEntity, firstArg, Pattern { ANY } });
  return PQL_PARSE_SUCCESS;
}

spa::PqlParseStatus spa::PqlPatternSubParser::parseIf(PqlArgument& designEntity, PqlArgument& firstArg,
  Stream<Token>& tokens, ParsedQuery& query) {
  bool matchStatus = tokens.match({
    { TOKEN_UNDERSCORE, "_" },
    { TOKEN_COMMA, "," },
    { TOKEN_UNDERSCORE, "_" },
    { TOKEN_CLOSE_BRACKET, ")" }
    });
  if (!matchStatus) {
    return PQL_PARSE_ERROR;
  }
  tokens.seek(4);
  query.addPatternClause({ designEntity, firstArg, Pattern { ANY } });
  return PQL_PARSE_SUCCESS;
}

spa::PqlParseStatus spa::PqlPatternSubParser::parse(Stream<Token>& tokens,
  ParsedQuery& query) {
  bool matchResult = tokens.match({
    { spa::TOKEN_NAME, ""},
    { spa::TOKEN_OPEN_BRACKET, "("},
  });
  if (!matchResult) {
    return PQL_PARSE_ERROR;
  }
  std::optional<spa::PqlArgument> designEntityOpt = parseDesignEntity(tokens[0], query);
  if (!designEntityOpt) {
    return PQL_PARSE_ERROR;
  }
  spa::PqlArgument& designEntity = designEntityOpt.value();
  tokens.seek(2);
  std::optional<PqlArgument> firstArgOpt = argParser.parse(tokens, query);
  if (!firstArgOpt) {
    return PQL_PARSE_ERROR;
  }
  spa::PqlArgument& firstArg = firstArgOpt.value();
  if (!tokens.match({ { spa::TOKEN_COMMA, ","} })) {
    return PQL_PARSE_ERROR;
  }
  tokens.seek(1);
  switch (designEntity.getDesignEntity().value()) {
    case ASSIGN:
      return parseAssign(designEntity, firstArg, tokens, query);
    case WHILE:
      return parseWhile(designEntity, firstArg, tokens, query);
    case IF:
      return parseIf(designEntity, firstArg, tokens, query);
    default:
      return PQL_PARSE_ERROR;
  }
}

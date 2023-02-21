#include "PqlSuchThatSubParser.h"

#include <string>
#include <unordered_map>

#include "ParsedQuery.h"

const std::unordered_map<std::string, spa::RelationshipType> relationshipMap{
  {"Follows", spa::FOLLOWS},
  {"Follows*", spa::FOLLOWS_STAR},
  {"Parent", spa::PARENT},
  {"Parent*", spa::PARENT_STAR},
  {"Uses", spa::USES},
  {"Modifies", spa::MODIFIES},
};

spa::PqlParseStatus spa::PqlSuchThatSubParser::getArgs(RelationshipType type,
  Stream<Token>& tokens,
  ParsedQuery& query) {
  if (!tokens.match({ { TOKEN_OPEN_BRACKET, "(" } })) {
    return PQL_PARSE_ERROR;
  }
  tokens.seek(1);
  auto firstArg = argParser.parse(tokens, query);
  if (!firstArg) {
    return PQL_PARSE_ERROR;
  }
  if (!tokens.match({ { TOKEN_COMMA, "," } })) {
    return PQL_PARSE_ERROR;
  }
  tokens.seek(1);
  auto secondArg = argParser.parse(tokens, query);
  if (!secondArg) {
    return PQL_PARSE_ERROR;
  }
  if (!tokens.match({ { TOKEN_CLOSE_BRACKET, ")" } })) {
    return PQL_PARSE_ERROR;
  }
  tokens.seek(1);
  query.setSuchThatClause({ type, firstArg.value(), secondArg.value() });
  return PQL_PARSE_SUCCESS;
}

spa::PqlParseStatus spa::PqlSuchThatSubParser::parse(Stream<Token>& tokens,
  ParsedQuery& query) {
  if (!tokens.match({ { TOKEN_NAME, "" } })) {
    return PQL_PARSE_ERROR;
  }
  std::string relationship = tokens[0].getValue();
  tokens.seek(1);
  if (tokens.match({ { TOKEN_MULTIPLY, "*" } })) {
    relationship.push_back('*');
    tokens.seek(1);
  }
  auto it = relationshipMap.find(relationship);
  if (it == relationshipMap.end()) {
    return PQL_PARSE_ERROR;
  }
  RelationshipType type = it->second;
  return getArgs(type, tokens, query);
}

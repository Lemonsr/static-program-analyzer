#include "PqlSuchThatSubParser.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "ParsedQuery.h"
#include "PqlRefTypes.h"

const std::unordered_map<std::string, spa::RelationshipType> relationshipMap {
  {"Follows", spa::FOLLOWS},
  {"Follows*", spa::FOLLOWS_STAR},
  {"Parent", spa::PARENT},
  {"Parent*", spa::PARENT_STAR},
  {"Modifies", spa::MODIFIES},
  {"Uses", spa::USES},
  {"Calls", spa::CALLS},
  {"Calls*", spa::CALLS_STAR},
  {"Next", spa::NEXT},
  {"Next*", spa::NEXT_STAR},
  {"Affects", spa::AFFECTS},
  {"Affects*", spa::AFFECTS_STAR}
};

const std::unordered_map<spa::RelationshipType,
                         std::pair<const std::unordered_set<spa::ArgumentType>*,
                                   const std::unordered_set<spa::ArgumentType>*>> relationshipArgTypesMap {
  {spa::FOLLOWS, { &spa::stmtRef, &spa::stmtRef }},
  {spa::FOLLOWS_STAR, { &spa::stmtRef, &spa::stmtRef }},
  {spa::PARENT, { &spa::stmtRef, &spa::stmtRef }},
  {spa::PARENT_STAR, { &spa::stmtRef, &spa::stmtRef }},
  {spa::MODIFIES, { &spa::stmtEntityRef, &spa::entityRef }},
  {spa::USES, { &spa::stmtEntityRef, &spa::entityRef }},
  {spa::CALLS, { &spa::entityRef, &spa::entityRef }},
  {spa::CALLS_STAR, { &spa::entityRef, &spa::entityRef }},
  {spa::NEXT, { &spa::stmtRef, &spa::stmtRef }},
  {spa::NEXT_STAR, { &spa::stmtRef, &spa::stmtRef }},
  {spa::AFFECTS, { &spa::stmtRef, &spa::stmtRef }},
  {spa::AFFECTS_STAR, { &spa::stmtRef, &spa::stmtRef }}
};

spa::PqlParseStatus spa::PqlSuchThatSubParser::getArgs(RelationshipType type,
  Stream<Token>& tokens,
  ParsedQuery& query) {
  if (!tokens.match({ { TOKEN_OPEN_BRACKET, "(" } })) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  tokens.seek(1);
  auto& argTypePair = relationshipArgTypesMap.find(type)->second;
  auto& firstArgSetPtr = argTypePair.first;
  auto& secondArgSetPtr = argTypePair.second;
  auto firstArg = argParser.parse(tokens, query);
  if (!firstArg || firstArgSetPtr->find(firstArg.value().getType()) == firstArgSetPtr->end()) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  if (!tokens.match({ { TOKEN_COMMA, "," } })) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  tokens.seek(1);
  auto secondArg = argParser.parse(tokens, query);
  if (!secondArg || secondArgSetPtr->find(secondArg.value().getType()) == secondArgSetPtr->end()) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  if (!tokens.match({ { TOKEN_CLOSE_BRACKET, ")" } })) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  tokens.seek(1);
  query.addSuchThatClause({ type, firstArg.value(), secondArg.value() });
  return PQL_PARSE_SUCCESS;
}

spa::PqlParseStatus spa::PqlSuchThatSubParser::parse(Stream<Token>& tokens,
  ParsedQuery& query) {
  if (!tokens.match({ { TOKEN_NAME, "" } })) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  std::string relationship = tokens[0].getValue();
  int index = tokens[0].getIndex();
  tokens.seek(1);
  if (tokens.match({ { TOKEN_MULTIPLY, "*" } }) && tokens[0].getIndex() == index + relationship.size()) {
    relationship.push_back('*');
    tokens.seek(1);
  }
  auto it = relationshipMap.find(relationship);
  if (it == relationshipMap.end()) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  RelationshipType type = it->second;
  return getArgs(type, tokens, query);
}

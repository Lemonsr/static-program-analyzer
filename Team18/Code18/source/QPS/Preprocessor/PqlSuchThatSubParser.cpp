#include "PqlSuchThatSubParser.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "ParsedQuery.h"
#include "PqlRefTypes.h"
#include "Literal.h"

const std::unordered_map<std::string, spa::RelationshipType> relationshipMap {
  {FOLLOWS_LITERAL, spa::FOLLOWS},
  {FOLLOWS_STAR_LITERAL, spa::FOLLOWS_STAR},
  {PARENT_LITERAL, spa::PARENT},
  {PARENT_STAR_LITERAL, spa::PARENT_STAR},
  {MODIFIES_LITERAL, spa::MODIFIES},
  {USES_LITERAL, spa::USES},
  {CALLS_LITERAL, spa::CALLS},
  {CALLS_STAR_LITERAL, spa::CALLS_STAR},
  {NEXT_LITERAL, spa::NEXT},
  {NEXT_STAR_LITERAL, spa::NEXT_STAR},
  {AFFECTS_LITERAL, spa::AFFECTS},
  {AFFECTS_STAR_LITERAL, spa::AFFECTS_STAR}
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
  if (!tokens.match({ { TOKEN_OPEN_BRACKET, OPEN_BRACKET_LITERAL }})) {
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
  if (!tokens.match({ { TOKEN_COMMA, COMMA_LITERAL } })) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  tokens.seek(1);
  auto secondArg = argParser.parse(tokens, query);
  if (!secondArg || secondArgSetPtr->find(secondArg.value().getType()) == secondArgSetPtr->end()) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  if (!tokens.match({ { TOKEN_CLOSE_BRACKET, CLOSE_BRACKET_LITERAL } })) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  tokens.seek(1);
  query.addSuchThatClause({ type, firstArg.value(), secondArg.value() });
  return PQL_PARSE_SUCCESS;
}

spa::PqlParseStatus spa::PqlSuchThatSubParser::parse(Stream<Token>& tokens,
  ParsedQuery& query) {
  if (!tokens.match({ { TOKEN_NAME, EMPTY_LITERAL } })) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  std::string relationship = tokens[0].getValue();
  int index = tokens[0].getIndex();
  tokens.seek(1);
  if (tokens.match({ { TOKEN_MULTIPLY, MULTIPLY_LITERAL } }) && tokens[0].getIndex() == index + relationship.size()) {
    relationship = relationship + MULTIPLY_LITERAL;
    tokens.seek(1);
  }
  auto it = relationshipMap.find(relationship);
  if (it == relationshipMap.end()) {
    return PQL_PARSE_SYNTAX_ERROR;
  }
  RelationshipType type = it->second;
  return getArgs(type, tokens, query);
}

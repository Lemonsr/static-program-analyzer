#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <optional>

#include "Relationship.h"
#include "PqlArgument.h"
#include "Token.h"
#include "DesignEntity.h"
#include "Stream.h"
#include "PKBQueryTypes.h"

namespace spa {

class SuchThatClause {
 public:
  SuchThatClause(RelationshipType designAbstraction, PqlArgument firstArg,
    PqlArgument secondArg);
 private:
  RelationshipType designAbstraction;
  PqlArgument firstArg;
  PqlArgument secondArg;
};

class PatternClause {
 public:
  PatternClause(PqlArgument synonym, PqlArgument firstArg, Pattern secondArg);
 private:
  PqlArgument synonym;
  PqlArgument firstArg;
  Pattern secondArg;
};

class ParsedQuery {
 public:
  ParsedQuery();
  void addDeclaration(std::string synonym, DesignEntityType designEntity);
 private:
  DesignEntityType selectSynonym;
  std::optional<PatternClause> patternClause;
  std::optional<SuchThatClause> suchThatClause;
  std::unordered_map<std::string, DesignEntityType> declarations;
};

class PqlParser {
 public:
  explicit PqlParser(Stream<Token> tokens);
  ParsedQuery parse();
 private:
  Stream<Token> tokens;
};
}  // namespace spa


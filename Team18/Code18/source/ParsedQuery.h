#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <optional>

#include "PqlArgument.h"
#include "Token.h"
#include "DesignEntity.h"
#include "Stream.h"
#include "PKBQueryTypes.h"

namespace spa {
enum RelationshipType {
  FOLLOWS,
  FOLLOWS_STAR,
  PARENT,
  PARENT_STAR,
  MODIFIES,
  USES
};

class SuchThatClause {
 private:
  RelationshipType designAbstraction;
  PqlArgument firstArg;
  PqlArgument secondArg;
 public:
  SuchThatClause(RelationshipType designAbstraction, PqlArgument firstArg,
    PqlArgument secondArg);
};

class PatternClause {
 private:
  PqlArgument synonym;
  PqlArgument firstArg;
  Pattern secondArg;
 public:
  PatternClause(PqlArgument synonym, PqlArgument firstArg, Pattern secondArg);
};

class ParsedQuery {
 private:
  DesignEntityType selectSynonym;
  std::optional<PatternClause> patternClause;
  std::optional<SuchThatClause> suchThatClause;
  std::unordered_map<std::string, DesignEntityType> declarations;
 public:
  ParsedQuery();
  void addDeclaration(std::string synonym, DesignEntityType designEntity);
};
}  // namespace spa


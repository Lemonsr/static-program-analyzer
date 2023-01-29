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
  friend bool operator==(const SuchThatClause& s1, const SuchThatClause& s2);
  friend bool operator!=(const SuchThatClause& s1, const SuchThatClause& s2);
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
  std::string selectSynonym;
  std::optional<PatternClause> patternClause;
  std::optional<SuchThatClause> suchThatClause;
  std::unordered_map<std::string, DesignEntityType> declarations;
 public:
  bool addDeclaration(std::string synonym, DesignEntityType designEntity);
  int getDeclarationsCount();
  std::optional<DesignEntityType> getType(std::string synonym);
  bool setSelectSynonym(std::string synonym);
  void setSuchThatClause(SuchThatClause clause);
  void setPatternClause(PatternClause clause);
  const std::optional<SuchThatClause>& getSuchThatClause();
  const std::optional<PatternClause>& getPatternClause();
};
}  // namespace spa


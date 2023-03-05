#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <optional>
#include <memory>

#include "PqlArgument.h"
#include "Token.h"
#include "DesignEntity.h"
#include "Stream.h"
#include "PKBQueryTypes.h"
#include "QpsEvaluator.h"

namespace spa {
enum RelationshipType {
  FOLLOWS,
  FOLLOWS_STAR,
  PARENT,
  PARENT_STAR,
  MODIFIES,
  MODIFIES_P,
  USES,
  USES_P,
  CALLS,
  CALLS_STAR,
  NEXT,
  NEXT_STAR,
  AFFECTS,
  AFFECTS_STAR
};

class SuchThatClause {
 private:
  RelationshipType designAbstraction;
  PqlArgument firstArg;
  PqlArgument secondArg;

 public:
  SuchThatClause(RelationshipType designAbstraction, PqlArgument firstArg,
    PqlArgument secondArg);
  std::unique_ptr<spa::QpsEvaluator> getEvaluator();
  const RelationshipType& getDesignAbstraction();
  const PqlArgument& getFirstArg();
  const PqlArgument& getSecondArg();
  friend bool operator==(const SuchThatClause& s1, const SuchThatClause& s2);
  friend bool operator!=(const SuchThatClause& s1, const SuchThatClause& s2);
};

class PatternClause {
 private:
  PqlArgument synonym;
  PqlArgument firstArg;
  Pattern pattern;

 public:
  PatternClause(PqlArgument synonym, PqlArgument firstArg, Pattern pattern);
  std::unique_ptr<spa::QpsEvaluator> getEvaluator();
  const PqlArgument& getSynonym();
  const PqlArgument& getFirstArg();
  friend bool operator==(const PatternClause& p1, const PatternClause& p2);
  friend bool operator!=(const PatternClause& p1, const PatternClause& p2);
};

enum class WithArgumentType {
  WITH_VALUE,
  WITH_ATTRIBUTE
};

class WithArgument {
 private:
  WithArgumentType type = WithArgumentType::WITH_VALUE;
  std::optional<QpsValue> valueOpt;
  std::optional<std::string> attributeOpt;
 public:
  WithArgument() = default;
  explicit WithArgument(QpsValue value);
  explicit WithArgument(std::string attribute);
  const WithArgumentType& getType();
  const QpsValue& getValue();
  const std::string& getAttribute();

  friend bool operator==(const WithArgument& first, const WithArgument& second);
  friend bool operator!=(const WithArgument& first, const WithArgument& second);
};

class WithClause {
 private:
  WithArgument firstArg;
  WithArgument secondArg;
 public:
  WithClause() = default;
  WithClause(WithArgument firstArg, WithArgument secondArg);
  const WithArgument& getFirstArg();
  const WithArgument& getSecondArg();
  friend bool operator==(const WithClause& w1, const WithClause& w2);
  friend bool operator!=(const WithClause& w1, const WithClause& w2);
};

enum class SelectClauseType {
  SELECT_BOOLEAN,
  SELECT_TUPLE
};

enum class PqlClauseType {
  PATTERN_CLAUSE,
  SUCH_THAT_CLAUSE,
  WITH_CLAUSE
};

class ParsedQuery {
 private:
  SelectClauseType selectType;
  PqlClauseType lastAddedClause;
  std::vector<std::string> selectColumns;
  std::vector<PatternClause> patternClauses;
  std::vector<SuchThatClause> suchThatClauses;
  std::vector<WithClause> withClauses;
  std::unordered_map<std::string, DesignEntityType> declarations;
  std::unordered_map<std::string, int> declarationsCount;
  std::unordered_map<std::string, DesignEntityType> usedDeclarations;

 public:
  void addDeclaration(std::string synonym, DesignEntityType designEntity);
  std::unordered_map<std::string, int>& getDeclarationsCount();
  DesignEntityType getDeclarationType(std::string synonym);
  std::unordered_map<std::string, DesignEntityType>& getDeclarations();
  void setSelectClauseType(SelectClauseType selectType);
  SelectClauseType getSelectClauseType();
  PqlClauseType getLastAddedClause();
  void addSelectColumn(std::string selectColumn);
  std::vector<std::string>& getSelectColumns();
  void addSuchThatClause(SuchThatClause clause);
  std::vector<SuchThatClause>& getSuchThatClauses();
  void addPatternClause(PatternClause clause);
  std::vector<PatternClause>& getPatternClauses();
  void addWithClause(WithClause clause);
  std::vector<WithClause>& getWithClauses();
  bool addUsedDeclaration(std::string declaration, DesignEntityType designEntityType);
  std::unordered_map<std::string, DesignEntityType>& getUsedDeclarations();
  bool hasClauses();
};
}  // namespace spa

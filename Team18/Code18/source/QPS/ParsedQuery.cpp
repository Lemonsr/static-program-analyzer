#include <iostream>
#include <memory>
#include <stdexcept>

#include "ParsedQuery.h"
#include "Token.h"
#include "QpsEvaluator.h"
#include "ModifiesEvaluator.h"
#include "UsesEvaluator.h"
#include "FollowsEvaluator.h"
#include "FollowsStarEvaluator.h"
#include "ParentEvaluator.h"
#include "ParentStarEvaluator.h"
#include "PatternEvaluator.h"

bool spa::ParsedQuery::addDeclaration(std::string synonym,
  DesignEntityType designEntity) {
  if (declarations.find(synonym) != declarations.end()) {
    return false;
  }
  declarations.insert({ synonym, designEntity });
  return true;
}

int spa::ParsedQuery::getDeclarationsCount() {
  return declarations.size();
}

std::optional<spa::DesignEntityType> spa::ParsedQuery::getDeclarationType(
  std::string synonym
) {
  auto it = declarations.find(synonym);
  if (it == declarations.end()) {
    return {};
  }
  return { it->second };
}

std::unordered_map<std::string, spa::DesignEntityType>& spa::ParsedQuery::getDeclarations() {
  return declarations;
}

void spa::ParsedQuery::setSelectClauseType(SelectClauseType selectType) {
  this->selectType = selectType;
}

spa::SelectClauseType spa::ParsedQuery::getSelectClauseType() {
  return selectType;
}

spa::PqlClauseType spa::ParsedQuery::getLastAddedClause() {
  return lastAddedClause;
}

void spa::ParsedQuery::addSelectColumn(std::string selectColumn) {
  selectColumns.push_back(selectColumn);
}

std::vector<std::string>& spa::ParsedQuery::getSelectColumns() {
  return selectColumns;
}

void spa::ParsedQuery::addSuchThatClause(SuchThatClause clause) {
  suchThatClauses.push_back(clause);
  lastAddedClause = PqlClauseType::SUCH_THAT_CLAUSE;
}

std::vector<spa::SuchThatClause>& spa::ParsedQuery::getSuchThatClauses() {
  return suchThatClauses;
}

void spa::ParsedQuery::addPatternClause(PatternClause clause) {
  patternClauses.push_back(clause);
  lastAddedClause = PqlClauseType::PATTERN_CLAUSE;
}

std::vector<spa::PatternClause>& spa::ParsedQuery::getPatternClauses() {
  return patternClauses;
}

bool spa::ParsedQuery::hasClauses() {
  return suchThatClauses.size() > 0 || patternClauses.size() > 0;
}

spa::SuchThatClause::SuchThatClause(RelationshipType designAbstraction,
  PqlArgument firstArg, PqlArgument secondArg)
  : designAbstraction(designAbstraction), firstArg(firstArg),
  secondArg(secondArg) {
}

std::unique_ptr<spa::QpsEvaluator> spa::SuchThatClause::getEvaluator() {
  switch (designAbstraction) {
  case MODIFIES: {
    return std::make_unique<ModifiesEvaluator>(firstArg, secondArg);
  }
  case USES: {
    return std::make_unique<UsesEvaluator>(firstArg, secondArg);
  }
  case FOLLOWS: {
    return std::make_unique<FollowsEvaluator>(firstArg, secondArg);
  }
  case FOLLOWS_STAR: {
    return std::make_unique<FollowsStarEvaluator>(firstArg, secondArg);
  }
  case PARENT: {
    return std::make_unique<ParentEvaluator>(firstArg, secondArg);
  }
  case PARENT_STAR: {
    return std::make_unique<ParentStarEvaluator>(firstArg, secondArg);
  }
  default: {
    throw std::runtime_error("Unable to find evaluator");
  }
  }
}

const spa::RelationshipType& spa::SuchThatClause::getDesignAbstraction() {
  return designAbstraction;
}

const spa::PqlArgument& spa::SuchThatClause::getFirstArg() {
  return firstArg;
}

const spa::PqlArgument& spa::SuchThatClause::getSecondArg() {
  return secondArg;
}

bool spa::operator==(const SuchThatClause& s1, const SuchThatClause& s2) {
  bool typeMatch = s1.designAbstraction == s2.designAbstraction;
  bool firstArgMatch = s1.firstArg == s2.firstArg;
  bool secondArgMatch = s1.secondArg == s2.secondArg;
  return typeMatch && firstArgMatch && secondArgMatch;
}

bool spa::operator!=(const SuchThatClause& s1, const SuchThatClause& s2) {
  return !(s1 == s2);
}

spa::PatternClause::PatternClause(PqlArgument synonym, PqlArgument firstArg,
  Pattern pattern) : synonym(synonym), firstArg(firstArg),
                     pattern(pattern) {
}

std::unique_ptr<spa::QpsEvaluator> spa::PatternClause::getEvaluator() {
  return std::make_unique<spa::PatternEvaluator>(synonym, firstArg, pattern);
}

const spa::PqlArgument& spa::PatternClause::getFirstArg() {
  return firstArg;
}

bool spa::operator==(const PatternClause& p1, const PatternClause& p2) {
  bool synonymMatch = p1.synonym == p2.synonym;
  bool firstArgMatch = p1.firstArg == p2.firstArg;
  bool patternMatch = p1.pattern == p2.pattern;
  return synonymMatch && firstArgMatch && patternMatch;
}

bool spa::operator!=(const PatternClause& p1, const PatternClause& p2) {
  return !(p1 == p2);
}

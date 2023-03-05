#include <iostream>
#include <memory>
#include <stdexcept>

#include "ParsedQuery.h"
#include "Token.h"
#include "QpsEvaluator.h"
#include "CallsEvaluator.h"
#include "CallsStarEvaluator.h"
#include "ModifiesEvaluator.h"
#include "UsesEvaluator.h"
#include "FollowsEvaluator.h"
#include "FollowsStarEvaluator.h"
#include "ParentEvaluator.h"
#include "ParentStarEvaluator.h"
#include "PatternEvaluator.h"
#include "PatternContainerEvaluator.h"

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

void spa::ParsedQuery::addWithClause(spa::WithClause clause) {
  withClauses.push_back(clause);
}

std::vector<spa::WithClause>& spa::ParsedQuery::getWithClauses() {
  return withClauses;
}

std::unordered_map<std::string, spa::DesignEntityType> spa::ParsedQuery::getUsedDeclarations() {
  return usedDeclarations;
}

bool spa::ParsedQuery::addUsedDeclaration(std::string declaration, DesignEntityType designEntityType) {
  if (usedDeclarations.find(declaration) != usedDeclarations.end()) {
    return false;
  }

  usedDeclarations.insert({ declaration, designEntityType });
  return true;
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
  case CALLS: {
    return std::make_unique<CallsEvaluator>(firstArg, secondArg);
  }
  case CALLS_STAR: {
    return std::make_unique<CallsStarEvaluator>(firstArg, secondArg);
  }
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
  switch (synonym.getDesignEntity().value()) {
  case IF:
  case WHILE:
    return std::make_unique<spa::PatternContainerEvaluator>(synonym.getDesignEntity().value(), synonym, firstArg);
  case ASSIGN:
    return std::make_unique<spa::PatternEvaluator>(synonym, firstArg, pattern);
  }
}

const spa::PqlArgument& spa::PatternClause::getSynonym() {
  return synonym;
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

spa::WithArgument::WithArgument(QpsValue value) {
  this->type = WithArgumentType::WITH_VALUE;
  this->valueOpt = value;
}

spa::WithArgument::WithArgument(std::string attribute) {
  this->type = WithArgumentType::WITH_ATTRIBUTE;
  this->attributeOpt = attribute;
}

const spa::WithArgumentType& spa::WithArgument::getType() {
  return type;
}

const spa::QpsValue& spa::WithArgument::getValue() {
  return valueOpt.value();
}

const std::string& spa::WithArgument::getAttribute() {
  return attributeOpt.value();
}

bool spa::operator==(const WithArgument& first, const WithArgument& second) {
  if (first.type != second.type) {
    return false;
  }
  if (first.type == WithArgumentType::WITH_ATTRIBUTE) {
    return first.attributeOpt.value() == second.attributeOpt.value();
  }
  return first.valueOpt.value() == second.valueOpt.value();
}

bool spa::operator!=(const WithArgument& first, const WithArgument& second) {
  return !(first == second);
}

spa::WithClause::WithClause(WithArgument firstArg, WithArgument secondArg) {
  this->firstArg = firstArg;
  this->secondArg = secondArg;
}

const spa::WithArgument& spa::WithClause::getFirstArg() {
  return firstArg;
}

const spa::WithArgument& spa::WithClause::getSecondArg() {
  return secondArg;
}

bool spa::operator==(const WithClause& w1, const WithClause& w2) {
  return w1.firstArg == w2.firstArg && w1.secondArg == w2.secondArg;
}

bool spa::operator!=(const WithClause& w1, const WithClause& w2) {
  return !(w1 == w2);
}


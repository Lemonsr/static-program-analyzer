#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "ParsedQuery.h"
#include "Token.h"
#include "QpsEvaluator.h"
#include "EntityEntityEvaluator.h"
#include "ModifiesUsesEvaluator.h"
#include "WithEvaluator.h"
#include "PatternEvaluator.h"
#include "PatternContainerEvaluator.h"
#include "StmtStmtEvaluator.h"
#include "CFGEvaluator.h"

const std::string UNABLE_TO_FIND_EVALUATOR_ERROR = "Unable to find evaluator";

void spa::ParsedQuery::addDeclaration(std::string synonym, DesignEntityType designEntity) {
  declarations[synonym] = designEntity;
  ++declarationsCount[synonym];
}

std::unordered_map<std::string, int>& spa::ParsedQuery::getDeclarationsCount() {
  return declarationsCount;
}

spa::DesignEntityType spa::ParsedQuery::getDeclarationType(
  std::string synonym
) {
  auto it = declarations.find(synonym);
  if (it == declarations.end()) {
    return UNKNOWN;
  }
  return it->second;
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
  selectColumnsSet.insert(selectColumn);
}

std::vector<std::string>& spa::ParsedQuery::getSelectColumns() {
  return selectColumns;
}

bool spa::ParsedQuery::hasSelectColumn(const std::string& selectColumn) {
  return selectColumnsSet.find(selectColumn) != selectColumnsSet.end();
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
  lastAddedClause = PqlClauseType::WITH_CLAUSE;
}

std::vector<spa::WithClause>& spa::ParsedQuery::getWithClauses() {
  return withClauses;
}

std::unordered_map<std::string, spa::DesignEntityType>& spa::ParsedQuery::getUsedDeclarations() {
  return usedDeclarations;
}

void spa::ParsedQuery::addUsedDeclaration(std::string declaration, DesignEntityType designEntityType) {
  usedDeclarations.insert({ declaration, designEntityType });
}

void spa::ParsedQuery::addSelectWithDeclaration(std::string declaration, DesignEntityType designEntityType) {
  selectWithDeclarations.insert({ declaration, designEntityType });
}

std::unordered_map<std::string, spa::DesignEntityType>& spa::ParsedQuery::getSelectWithDeclarations() {
  return selectWithDeclarations;
}

bool spa::ParsedQuery::hasClauses() {
  return suchThatClauses.size() > 0 || patternClauses.size() > 0 || withClauses.size() > 0;
}

spa::SuchThatClause::SuchThatClause(RelationshipType designAbstraction,
  PqlArgument firstArg, PqlArgument secondArg)
  : designAbstraction(designAbstraction), firstArg(firstArg),
  secondArg(secondArg) {
}

std::unique_ptr<spa::QpsEvaluator> spa::SuchThatClause::getEvaluator() {
  switch (designAbstraction) {
  case CALLS:
  case CALLS_STAR: {
    return std::make_unique<EntityEntityEvaluator>(firstArg, secondArg, designAbstraction);
  }
  case MODIFIES:
  case USES: {
    return std::make_unique<ModifiesUsesEvaluator>(firstArg, secondArg, designAbstraction);
  }
  case FOLLOWS:
  case FOLLOWS_STAR:
  case PARENT:
  case PARENT_STAR:
  case NEXT: {
    return std::make_unique<StmtStmtEvaluator>(firstArg, secondArg, designAbstraction);
  }
  case NEXT_STAR:
  case AFFECTS:
  case AFFECTS_STAR: {
    return std::make_unique<CFGEvaluator>(firstArg, secondArg, designAbstraction);
  }
  default: {
    throw std::runtime_error(UNABLE_TO_FIND_EVALUATOR_ERROR);
  }
  }
}

std::vector<std::string> spa::SuchThatClause::getSynonyms() {
  std::vector<std::string> synonyms;
  if (getFirstArgType() == SYNONYM) {
    synonyms.push_back(getFirstArgValue());
  }
  if (getSecondArgType() == SYNONYM) {
    synonyms.push_back(getSecondArgValue());
  }
  return synonyms;
}

const spa::RelationshipType& spa::SuchThatClause::getDesignAbstraction() {
  return designAbstraction;
}

const spa::PqlArgument& spa::SuchThatClause::getFirstArg() {
  return firstArg;
}

const spa::ArgumentType& spa::SuchThatClause::getFirstArgType() {
  return firstArg.getType();
}

const std::string& spa::SuchThatClause::getFirstArgValue() {
  return firstArg.getValue();
}

const spa::PqlArgument& spa::SuchThatClause::getSecondArg() {
  return secondArg;
}

const spa::ArgumentType& spa::SuchThatClause::getSecondArgType() {
  return secondArg.getType();
}

const std::string& spa::SuchThatClause::getSecondArgValue() {
  return secondArg.getValue();
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
                                  Pattern pattern, int numArgs) : synonym(synonym), firstArg(firstArg),
                                                                  pattern(pattern), numArgs(numArgs) {
}

int spa::PatternClause::getNumArgs() {
  return numArgs;
}

std::unique_ptr<spa::QpsEvaluator> spa::PatternClause::getEvaluator() {
  switch (synonym.getDesignEntity().value()) {
  case IF:
  case WHILE: {
    return std::make_unique<spa::PatternContainerEvaluator>(synonym.getDesignEntity().value(), synonym, firstArg);
  }
  case ASSIGN: {
    return std::make_unique<spa::PatternEvaluator>(synonym, firstArg, pattern);
  }
  default: {
    throw std::runtime_error(UNABLE_TO_FIND_EVALUATOR_ERROR);
  }
  }
}

std::vector<std::string> spa::PatternClause::getSynonyms() {
  if (getFirstArgType() == SYNONYM) {
    return { getSynonymValue(), getFirstArgValue() };
  }
  return { getSynonymValue() };
}

spa::PqlArgument& spa::PatternClause::getSynonym() {
  return synonym;
}

spa::PqlArgument& spa::PatternClause::getFirstArg() {
  return firstArg;
}

const spa::ArgumentType& spa::PatternClause::getFirstArgType() {
  return firstArg.getType();
}

const std::string& spa::PatternClause::getFirstArgValue() {
  return firstArg.getValue();
}

spa::DesignEntityType spa::PatternClause::getSynonymType() {
  return synonym.getDesignEntity().value();
}

const std::string& spa::PatternClause::getSynonymValue() {
  return synonym.getValue();
}

spa::PatternType spa::PatternClause::getPatternType() {
  return pattern.getType();
}

bool spa::operator==(const PatternClause& p1, const PatternClause& p2) {
  bool synonymMatch = p1.synonym == p2.synonym;
  bool firstArgMatch = p1.firstArg == p2.firstArg;
  bool patternMatch = p1.pattern == p2.pattern;
  bool numArgsMatch = p1.numArgs == p2.numArgs;
  return synonymMatch && firstArgMatch && patternMatch && numArgsMatch;
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

std::unique_ptr<spa::QpsEvaluator> spa::WithClause::getEvaluator() {
  return std::make_unique<WithEvaluator>(firstArg, secondArg);
}

std::vector<std::string> spa::WithClause::getSynonyms() {
  std::vector<std::string> synonyms;

  if (getFirstArgType() == WithArgumentType::WITH_ATTRIBUTE) {
    std::string attribute = firstArg.getAttribute();
    std::string synonym = attribute.substr(0, attribute.find('.'));
    synonyms.push_back(synonym);
  }

  if (getSecondArgType() == WithArgumentType::WITH_ATTRIBUTE) {
    std::string attribute = secondArg.getAttribute();
    std::string synonym = attribute.substr(0, attribute.find('.'));
    synonyms.push_back(synonym);
  }

  return synonyms;
}

const spa::WithArgument& spa::WithClause::getFirstArg() {
  return firstArg;
}

const spa::WithArgumentType& spa::WithClause::getFirstArgType() {
  return firstArg.getType();
}

const spa::WithArgument& spa::WithClause::getSecondArg() {
  return secondArg;
}

const spa::WithArgumentType& spa::WithClause::getSecondArgType() {
  return secondArg.getType();
}

bool spa::operator==(const WithClause& w1, const WithClause& w2) {
  return w1.firstArg == w2.firstArg && w1.secondArg == w2.secondArg;
}

bool spa::operator!=(const WithClause& w1, const WithClause& w2) {
  return !(w1 == w2);
}


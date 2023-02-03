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

std::optional<spa::DesignEntityType> spa::ParsedQuery::getType(
  std::string synonym
) {
  auto it = declarations.find(synonym);
  if (it == declarations.end()) {
    return {};
  }
  return { it->second };
}

bool spa::ParsedQuery::setSelectSynonym(std::string synonym) {
  if (declarations.find(synonym) == declarations.end()) {
    return false;
  }
  this->selectSynonym = synonym;
  return true;
}

const std::string& spa::ParsedQuery::getSelectSynonym() {
  return selectSynonym;
}

void spa::ParsedQuery::setSuchThatClause(SuchThatClause clause) {
  suchThatClause = clause;
}

void spa::ParsedQuery::setPatternClause(PatternClause clause) {
  patternClause = clause;
}

const std::optional<spa::SuchThatClause>&
    spa::ParsedQuery::getSuchThatClause() {
  return suchThatClause;
}

const std::optional<spa::PatternClause>& spa::ParsedQuery::getPatternClause() {
  return patternClause;
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

bool spa::operator==(const PatternClause& p1, const PatternClause& p2) {
  bool synonymMatch = p1.synonym == p2.synonym;
  bool firstArgMatch = p1.firstArg == p2.firstArg;
  bool patternMatch = p1.pattern == p2.pattern;
  return synonymMatch && firstArgMatch && patternMatch;
}

bool spa::operator!=(const PatternClause& p1, const PatternClause& p2) {
  return !(p1 == p2);
}

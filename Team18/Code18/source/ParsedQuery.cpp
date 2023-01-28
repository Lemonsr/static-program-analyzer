#include <iostream>

#include "ParsedQuery.h"
#include "Token.h"

bool spa::ParsedQuery::addDeclaration(std::string synonym,
  DesignEntityType designEntity) {
  if (declarations.find(synonym) != declarations.end()) {
    return false;
  }
  declarations.insert({ synonym, designEntity });
  return true;
}

bool spa::ParsedQuery::setSelectSynonym(std::string synonym) {
  if (declarations.find(synonym) == declarations.end()) {
    return false;
  }
  this->selectSynonym = synonym;
  return true;
}

spa::SuchThatClause::SuchThatClause(RelationshipType designAbstraction,
  PqlArgument firstArg, PqlArgument secondArg)
  : designAbstraction(designAbstraction), firstArg(firstArg),
  secondArg(secondArg) {
}

spa::PatternClause::PatternClause(PqlArgument synonym, PqlArgument firstArg,
  Pattern secondArg) : synonym(synonym), firstArg(firstArg),
  secondArg(secondArg) {
}

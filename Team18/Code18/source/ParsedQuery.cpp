#include <iostream>

#include "ParsedQuery.h"
#include "Token.h"

void spa::ParsedQuery::addDeclaration(std::string synonym,
  DesignEntityType designEntity) {
  if (declarations.find(synonym) != declarations.end()) {
    throw std::runtime_error("Synonym has already been declared");
  }
  declarations.insert({ synonym, designEntity });
}

void spa::ParsedQuery::setSelectSynonym(std::string synonym) {
  if (declarations.find(synonym) == declarations.end()) {
    throw std::runtime_error("Synonym cannot be found");
  }
  this->selectSynonym = synonym;
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

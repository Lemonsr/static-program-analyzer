#include <iostream>

#include "PqlParser.h"
#include "Token.h"

spa::PqlParser::PqlParser(Stream<Token> tokens) : tokens(tokens) {
}

spa::ParsedQuery spa::PqlParser::parse() {
  std::cout << "parse" << std::endl;
  return ParsedQuery{};
}

void spa::ParsedQuery::addDeclaration(std::string synonym,
  DesignEntityType designEntity) {
  if (declarations.find(synonym) != declarations.end()) {
    throw std::runtime_error("Synonym has already been declared");
  }
  declarations.insert({ synonym, designEntity });
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

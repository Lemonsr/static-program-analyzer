#include "QpsPreprocessor.h"

#include <sstream>
#include <optional>
#include <iostream>

#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"
#include "PqlQueryParser.h"
#include "ParsedQuery.h"

std::optional<spa::ParsedQuery> spa::QpsPreprocessor::preprocess(std::string query) {
  std::stringstream ss;
  ss.str(query);
  Tokenizer tokenizer;
  Stream<Token> tokens;
  try {
    tokens = tokenizer.tokenize(ss);
  }
  catch (std::runtime_error& e) {
      return {};
  }
  ParsedQuery parsedQuery;
  PqlQueryParser parser;
  if (parser.parse(tokens, parsedQuery) != PQL_PARSE_SUCCESS) {
    return {};
  }

  initializeUsedDeclarations(parsedQuery);
  return { parsedQuery };
}

void spa::QpsPreprocessor::initializeUsedDeclarations(ParsedQuery& pq) {
  std::unordered_map<std::string, DesignEntityType> usedDeclarations;

  for (auto& selectSynonym : pq.getSelectColumns()) {
    usedDeclarations.insert({ selectSynonym, pq.getDeclarationType(selectSynonym).value() });
  }

  for (auto& stClause : pq.getSuchThatClauses()) {
    addUsedDeclaration(usedDeclarations, stClause.getFirstArg());
    addUsedDeclaration(usedDeclarations, stClause.getSecondArg());
  }

  for (auto& patternClause : pq.getPatternClauses()) {
    addUsedDeclaration(usedDeclarations, patternClause.getSynonym());
    addUsedDeclaration(usedDeclarations, patternClause.getFirstArg());
  }

  pq.setUsedDeclarations(usedDeclarations);
}

void spa::QpsPreprocessor::addUsedDeclaration(std::unordered_map<std::string, DesignEntityType>& usedDeclarations,
                                              PqlArgument& argument) {
  if (argument.getType() == spa::SYNONYM) {
    usedDeclarations.insert({ argument.getValue(), argument.getDesignEntity().value() });
  }
}

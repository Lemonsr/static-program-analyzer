#include "DesignExtractor.h"
#include "SP.h"
#include "Stream.h"
#include "SpValidator.h"
#include "SpParser.h"
#include "Tokenizer.h"
#include "Token.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

spa::SP::SP(std::string source, PKBManager& pkbManager) :
  sourceCode(source), pkbManager(pkbManager) {}

void spa::SP::processSource() {
    Stream<Token> convertedTokens = convertToken();
    SpValidator validator(convertedTokens);
    bool isValid = validator.validateGrammar();
    if (!isValid) {
        exit(1);
    }
    auto updatedStream = validator.getUpdatedStream();
    SpParser parser = SpParser(updatedStream);
    std::vector<std::shared_ptr<ProcedureStatement>>  procedureList = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(pkbManager, procedureList);
    designExtractor.extractRelationship();
}

spa::Stream<spa::Token> spa::SP::convertToken() {
  std::stringstream ss(sourceCode);
  Stream<Token> tokens;
  Tokenizer tokenizer;
  try {
    tokens = tokenizer.tokenize(ss);
  } catch (std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
  return tokens;
}

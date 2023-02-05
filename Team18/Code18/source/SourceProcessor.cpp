#include "SourceProcessor.h"

#include <iostream>
#include <sstream>
#include <string>

#include "Stream.h"
#include "Tokenizer.h"
#include "Token.h"
#include "Validator.h"
#include "SpParser.h"
#include "DesignExtractor.h"

spa::SourceProcessor::SourceProcessor(std::string source, PKBManager& pkbManager) : sourceCode(source), pkbManager(pkbManager) {}

void spa::SourceProcessor::processSource() {
    std::stringstream ss(sourceCode);
    Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(ss);
    spa::Validator validator(tokens);
    auto newTokens = validator.convertToken(tokens);
    newTokens.resetStartPosition();
    //std::cout << "TESTING VALIDATOR" << std::endl;
    //for (int64_t i = 0; i < newTokens.remaining(); i++) {
    //    std::cout << "Type: " << newTokens[i].getType() <<
    //        ", Value: " << newTokens[i].getValue() << std::endl;
    //}
    //std::cout << "END OF TESTING" << std::endl;
    validator.validateProgram();
    newTokens.resetStartPosition();
    SpParser parser = SpParser(newTokens);
    std::vector<ProcedureStatement> procedureList = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(pkbManager, procedureList);
    designExtractor.extractRelationship();
}

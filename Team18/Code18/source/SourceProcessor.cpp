#include "SourceProcessor.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Stream.h"
#include "Tokenizer.h"
#include "Token.h"
#include "Validator.h"
#include "SpParser.h"
#include "DesignExtractor.h"

spa::SourceProcessor::SourceProcessor(std::string source, PKBManager& pkbManager) : 
                                      sourceCode(source), pkbManager(pkbManager) {}

void spa::SourceProcessor::processSource() {
    std::stringstream ss(sourceCode);
    Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(ss);
    spa::Validator validator(tokens);
    auto newTokens = validator.convertToken(tokens);
    newTokens.resetStartPosition();
    validator.validateProgram();
    newTokens.resetStartPosition();
    SpParser parser = SpParser(newTokens);
    std::vector<ProcedureStatement> procedureList = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(pkbManager, procedureList);
    designExtractor.extractRelationship();
}

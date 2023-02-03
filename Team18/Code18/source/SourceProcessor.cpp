#include "SourceProcessor.h"

#include <iostream>

#include "Stream.h"
#include "Tokenizer.h"
#include "Token.h"
#include "Validator.h"
#include <sstream>
#include <string>

spa::SourceProcessor::SourceProcessor(std::string source) : sourceCode(source) {}

void spa::SourceProcessor::processSource() {
    std::stringstream ss(sourceCode);
    Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(ss);
    spa::Validator validator(tokens);
    auto newTokens = validator.convertToken(tokens);
    std::cout << "TESTING VALIDATOR" << std::endl;
    for (int64_t i = 0; i < newTokens.remaining(); i++) {
        std::cout << "Type: " << newTokens[i].getType() <<
            ", Value: " << newTokens[i].getValue() << std::endl;
    }
    std::cout << "END OF TESTING" << std::endl;
    validator.validateProgram();
}

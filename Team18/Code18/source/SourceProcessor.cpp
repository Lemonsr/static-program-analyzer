#include "SourceProcessor.h"
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

    std::cout << "TESTING TOKENIZER" << std::endl;
    for (int64_t i = 0; i < tokens.remaining(); i++) {
        std::cout << "Type: " << tokens[i].getType() <<
            ", Value: " << tokens[i].getValue() << std::endl;
    }
    std::cout << "END OF TESTING" << std::endl;

    spa::Validator validator(tokens);
    validator.validateProgram();
}

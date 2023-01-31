#include <vector>
#include <sstream>

#include "SourceProcessor.h"
#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"
#include "Validator.h"

void spa::SourceProcessor::processSource(const std::string source) {
    std::stringstream ss;
    ss.str(source);
    Tokenizer tokenizer;
    Stream<Token> tokens = tokenizer.tokenize(ss);

    std::cout << "TESTING TOKENIZER" << std::endl;
    for (int64_t i = 0; i < tokens.remaining(); i++) {
        std::cout <<
            "Type: " << tokens[i].getType() << ", Value: "
            << tokens[i].getValue() << std::endl;
    }
    std::cout << "END OF TESTING" << std::endl;

    spa::Validator validator = spa::Validator(tokens);
    validator.validateProgram();
}

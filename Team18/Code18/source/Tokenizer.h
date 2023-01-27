#pragma once

#include "Token.h"
#include "Stream.h"

#include <sstream>
#include <string>

namespace spa {
class Tokenizer {
 private:
    void pushWordToken(spa::Stream<spa::Token>& tokens, std::string& word);
    void pushSymbolToken(std::stringstream& srcStream,
                         spa::Stream<spa::Token>& tokens,
                         char c);
 public:
    spa::Stream<Token> tokenize(std::stringstream& srcStream);
};
}  // namespace spa

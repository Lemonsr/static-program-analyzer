#pragma once

#include "Token.h"
#include "Stream.h"

#include <istream>
#include <string>

namespace spa {
class Tokenizer {
 private:
    void pushWordToken(spa::Stream<spa::Token>& tokens, std::string& word);
    void pushSymbolToken(std::istream& srcStream,
                         spa::Stream<spa::Token>& tokens,
                         char c);
 public:
    spa::Stream<Token> tokenize(std::istream& srcStream);
};
}  // namespace spa

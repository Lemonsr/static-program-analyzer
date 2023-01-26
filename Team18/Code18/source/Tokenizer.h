#pragma once

#include "Token.h"
#include "Stream.h"

#include <sstream>

namespace spa {
    class Tokenizer {
    public:
        spa::Stream<Token> tokenize(std::stringstream& srcStream);
    };
}

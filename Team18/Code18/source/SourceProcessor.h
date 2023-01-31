#pragma once
#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"
#include "PKB.h"
#include <string>

namespace spa {
    class SourceProcessor {
    public:
        void processSource(std::string);
    };
} //namespace spa

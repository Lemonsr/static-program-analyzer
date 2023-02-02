#pragma once
#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"
#include "PKB.h"
#include <string>

namespace spa {
class SourceProcessor {
 public:
    SourceProcessor(std::string);
    void processSource();
private:
    std::string sourceCode;
};
}  // namespace spa

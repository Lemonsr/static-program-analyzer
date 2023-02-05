#pragma once
#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"
#include "PKB.h"
#include <string>

namespace spa {
class SourceProcessor {
 public:
    explicit SourceProcessor(std::string, PKBManager& pkbManager);
    void processSource();
 private:
    std::string sourceCode;
    PKBManager& pkbManager;
};
}  // namespace spa

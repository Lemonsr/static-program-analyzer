#pragma once
#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"
#include "PKB.h"
#include <string>
#include <unordered_map>

namespace spa {
class SP {
 public:
    explicit SP(std::string, PKBManager& pkbManager);
    void processSource();
 private:
    std::string sourceCode;
    PKBManager& pkbManager;
    Stream<Token> convertToken();
};
}  // namespace spa

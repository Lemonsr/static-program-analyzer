#include "DesignExtractor.h"
#include "SP.h"
#include "Stream.h"
#include "SpValidator.h"
#include "SpParser.h"
#include "Tokenizer.h"
#include "Token.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

spa::SP::SP(std::string source, PKBManager& pkbManager) :
    sourceCode(source), pkbManager(pkbManager) {}

void spa::SP::processSource() {
    Stream<Token> convertedTokens = convertToken();
    SpValidator validator(convertedTokens);
    bool isValid = validator.validateGrammar();
    if (!isValid) {
        exit(1);
    }

    SpParser parser = SpParser(convertedTokens);
    std::vector<ProcedureStatement> procedureList = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(pkbManager, procedureList);
    designExtractor.extractRelationship();
}

spa::Stream<spa::Token> spa::SP::convertToken() {
    std::stringstream ss(sourceCode);
    Stream<Token> tokens;
    Tokenizer tokenizer;
    try {
        tokens = tokenizer.tokenize(ss);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    for (int64_t i = 0; i < tokens.remaining(); i++) {
        Token currToken = tokens[i];
        TokenType currTokenType = currToken.getType();
        bool isTokenNameType = currTokenType == TOKEN_NAME;
        if (!isTokenNameType) {
            continue;
        }

        std::string currTokenValue = currToken.getValue();
        bool isValidTokenName = stmtTokensMap.count(currTokenValue);
        if (isValidTokenName) {
            // e.g. [i-1] => "print", [i] => "print" => continue
            // e.g. [i-1] => "read", [i] => "print" => continue
            // [i] should remain as TOKEN_NAME
            if (i > 0 && (tokens[i - 1].getValue() == currTokenValue ||
                stmtTokensMap.count(tokens[i - 1].getValue()))) {
                continue;
            }
            tokens[i] = Token(stmtTokensMap.at(currTokenValue), currTokenValue);
        }
    }
    return tokens;
}


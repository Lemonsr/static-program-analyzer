#include "Tokenizer.h"
#include "Token.h"
#include "Stream.h"

#include <iostream>
#include <istream>
#include <string>
#include <stdexcept>
#include <cctype>
#include <unordered_map>

const std::unordered_map<std::string, spa::TokenType> tokenTypes {
    {"{", spa::TOKEN_OPEN_BRACE},
    {"}", spa::TOKEN_CLOSE_BRACE},
    {"(", spa::TOKEN_OPEN_BRACKET},
    {")", spa::TOKEN_CLOSE_BRACKET},
    {";", spa::TOKEN_SEMICOLON},
    {"+", spa::TOKEN_PLUS},
    {"-", spa::TOKEN_MINUS},
    {"/", spa::TOKEN_DIVIDE},
    {"*", spa::TOKEN_MULTIPLY},
    {"%", spa::TOKEN_MODULO},
    {"=", spa::TOKEN_EQUAL},
    {"==", spa::TOKEN_COND_EQ},
    {"!=", spa::TOKEN_COND_NEQ},
    {"<", spa::TOKEN_COND_LT},
    {"<=", spa::TOKEN_COND_LTE},
    {">", spa::TOKEN_COND_GT},
    {">=", spa::TOKEN_COND_GTE},
    {"!", spa::TOKEN_BOOL_NOT},
    {"&&", spa::TOKEN_BOOL_AND},
    {"||", spa::TOKEN_BOOL_OR},
    {",", spa::TOKEN_COMMA},
    {"_", spa::TOKEN_UNDERSCORE},
    {"\"", spa::TOKEN_DOUBLE_QUOTES},
    {".", spa::TOKEN_FULL_STOP},
    {"#", spa::TOKEN_HASH}
};

void spa::Tokenizer::pushWordToken(spa::Stream<spa::Token>& tokens,
                                   std::string& word) {
    if (word.empty()) {
        return;
    }
    if (std::isalpha(word[0])) {
        tokens.pushBack({ spa::TOKEN_NAME, word });
    } else {
        for (char c : word) {
            if (!std::isdigit(c)) {
                throw std::runtime_error("Invalid name in SIMPLE code");
            }
        }
        tokens.pushBack({ spa::TOKEN_INTEGER, word });
    }
    word.clear();
}

void spa::Tokenizer::pushSymbolToken(std::istream& srcStream,
                                     spa::Stream<spa::Token>& tokens,
                                     char c) {
    std::string s(1, c);
    int next = srcStream.peek();
    if (next != EOF) {
        s.push_back(static_cast<char>(next));
        auto it = tokenTypes.find(s);
        if (it != tokenTypes.end()) {
            tokens.pushBack({ it->second, it->first });
            srcStream.get();
            return;
        }
        s.pop_back();
    }
    auto it = tokenTypes.find(s);
    if (it == tokenTypes.end()) {
        throw std::runtime_error(std::string("Unknown Symbol: ").append(s));
    }
    tokens.pushBack({ it->second, it->first });
}

spa::Stream<spa::Token> spa::Tokenizer::tokenize(std::istream& srcStream) {
    std::string word;
    Stream<Token> tokens;
    char c;
    while (!srcStream.get(c).eof()) {
        if (std::isspace(c)) {
            pushWordToken(tokens, word);
        } else if (std::isalnum(c)) {
            word.push_back(c);
        } else {
            pushWordToken(tokens, word);
            pushSymbolToken(srcStream, tokens, c);
        }
    }
    pushWordToken(tokens, word);
    return tokens;
}

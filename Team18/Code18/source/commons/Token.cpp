#include "Token.h"

#include <string>

spa::Token::Token(TokenType type, std::string value) : type(type),
                                                       value(value) {
}

spa::Token::Token(TokenType type, char c) : type(type), value(1, c) {
}

const spa::TokenType& spa::Token::getType() {
    return type;
}

const std::string& spa::Token::getValue() {
    return value;
}

bool spa::operator==(const spa::Token& t1, const spa::Token& t2) {
    bool strictValueMatch = !t1.value.empty() && !t2.value.empty();
    bool typeMatch = t1.type == t2.type;
    if (!strictValueMatch) {
        return typeMatch;
    }
    return typeMatch && (t1.value == t2.value);
}

bool spa::operator!=(const spa::Token& t1, const spa::Token& t2) {
    return !(t1 == t2);
}

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "ParsedQuery.h"
#include "PqlParser.h"
#include "PqlPatternParser.h"
#include "Stream.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPqlPatternParser) {
public:
  TEST_METHOD(TestCorrectSyntax) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "pattern" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_UNDERSCORE, "_" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("a", spa::ASSIGN);
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlPatternParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getPatternClauses().size(), size_t(1));
    spa::PatternClause& clause = query.getPatternClauses()[0];
    spa::PatternClause compare(
      { spa::SYNONYM, "a", spa::ASSIGN },
      spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }),
      spa::Pattern(spa::ANY), 2);
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestMismatchSyntax) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "such" });
    tokens.pushBack({ spa::TOKEN_NAME, "that" });
    tokens.pushBack({ spa::TOKEN_NAME, "Modifies" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("a", spa::ASSIGN);
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlPatternParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_MISMATCH);
  }

  TEST_METHOD(TestErrorSyntax) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "pattern" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    tokens.pushBack({ spa::TOKEN_NAME, "x" });
    tokens.pushBack({ spa::TOKEN_PLUS, "+" });
    tokens.pushBack({ spa::TOKEN_NAME, "y" });
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("a", spa::ASSIGN);
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlPatternParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SYNTAX_ERROR);
  }
  };
}  // namespace UnitTesting

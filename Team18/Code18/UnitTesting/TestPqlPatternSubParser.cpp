#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "ParsedQuery.h"
#include "PqlParser.h"
#include "PqlPatternSubParser.h"
#include "Stream.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPqlPatternSubParser) {
public:
  TEST_METHOD(TestWildcard) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_UNDERSCORE, "_" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("a", spa::ASSIGN);
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlPatternSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getPatternClauses().size(), size_t(1));
    spa::PatternClause& clause = query.getPatternClauses()[0];
    spa::PatternClause compare(
      { spa::SYNONYM, "a", spa::ASSIGN },
      spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }),
      spa::Pattern(spa::ANY));
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestPartial) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_UNDERSCORE, "_" });
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    tokens.pushBack({ spa::TOKEN_NAME, "x" });
    tokens.pushBack({ spa::TOKEN_PLUS, "+" });
    tokens.pushBack({ spa::TOKEN_NAME, "y" });
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    tokens.pushBack({ spa::TOKEN_UNDERSCORE, "_" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("a", spa::ASSIGN);
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlPatternSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getPatternClauses().size(), size_t(1));
    spa::PatternClause& clause = query.getPatternClauses()[0];
    spa::PatternClause compare(
      { spa::SYNONYM, "a", spa::ASSIGN },
      spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }),
      spa::Pattern(spa::PARTIAL, {
        { spa::TOKEN_NAME, "x" },
        { spa::TOKEN_PLUS, "+" },
        { spa::TOKEN_NAME, "y" }
        }));
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestExact) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
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
    spa::PqlPatternSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getPatternClauses().size(), size_t(1));
    spa::PatternClause& clause = query.getPatternClauses()[0];
    spa::PatternClause compare(
      { spa::SYNONYM, "a", spa::ASSIGN },
      spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }),
      spa::Pattern(spa::EXACT, {
        { spa::TOKEN_NAME, "x" },
        { spa::TOKEN_PLUS, "+" },
        { spa::TOKEN_NAME, "y" }
        }));
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestWhile) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "w" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_UNDERSCORE, "_" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("w", spa::WHILE);
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlPatternSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getPatternClauses().size(), size_t(1));
    spa::PatternClause& clause = query.getPatternClauses()[0];
    spa::PatternClause compare(
      { spa::SYNONYM, "w", spa::WHILE },
      spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }),
      spa::Pattern(spa::ANY)
    );
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestIncorrectWhile) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "w" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_UNDERSCORE, "_" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_UNDERSCORE, "_" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("w", spa::WHILE);
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlPatternSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_ERROR);
  }

  TEST_METHOD(TestIf) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "ifs" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_UNDERSCORE, "_" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_UNDERSCORE, "_" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("ifs", spa::IF);
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlPatternSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getPatternClauses().size(), size_t(1));
    spa::PatternClause& clause = query.getPatternClauses()[0];
    spa::PatternClause compare(
      { spa::SYNONYM, "ifs", spa::IF },
      spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }),
      spa::Pattern(spa::ANY)
    );
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestIncorrectIf) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "ifs" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_UNDERSCORE, "_" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("ifs", spa::IF);
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlPatternSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_ERROR);
  }
  };
}  // namespace UnitTesting

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "ParsedQuery.h"
#include "PqlParser.h"
#include "PqlSuchThatParser.h"
#include "Stream.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPqlSuchThatParser) {
public:
  TEST_METHOD(TestCorrectQuery) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "such" });
    tokens.pushBack({ spa::TOKEN_NAME, "that" });
    tokens.pushBack({ spa::TOKEN_NAME, "Modifies" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "s" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("s", spa::STMT);
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlSuchThatParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getSuchThatClauses().size(), size_t(1));
    spa::SuchThatClause& clause = query.getSuchThatClauses()[0];
    spa::SuchThatClause compare(
      spa::MODIFIES,
      spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
      spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestMismatchQuery) {
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
    spa::PqlSuchThatParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_MISMATCH);
  }

  TEST_METHOD(TestIncorrectSyntax) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "such" });
    tokens.pushBack({ spa::TOKEN_NAME, "that" });
    tokens.pushBack({ spa::TOKEN_NAME, "Parent" });
    tokens.pushBack({ spa::TOKEN_MULTIPLY, "*" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "s1" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("s", spa::STMT);
    query.addDeclaration("s1", spa::STMT);
    spa::PqlSuchThatParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SYNTAX_ERROR);
  }
  };
}  // namespace UnitTesting

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "ParsedQuery.h"
#include "PqlParser.h"
#include "PqlAndParser.h"
#include "Stream.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPqlAndParser) {
public:
  TEST_METHOD(TestMismatch) {
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
    spa::PqlAndParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_MISMATCH);
    Assert::AreEqual(tokens.remaining(), int64_t(8));
  }

  TEST_METHOD(TestNoClauses) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "and" });
    tokens.pushBack({ spa::TOKEN_NAME, "Modifies" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "s" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("s", spa::STMT);
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlAndParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SYNTAX_ERROR);
  }

  TEST_METHOD(TestAndPattern) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "and" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_UNDERSCORE, "_" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("w", spa::WHILE);
    query.addDeclaration("a", spa::ASSIGN);
    query.addDeclaration("v", spa::VARIABLE);
    query.addSuchThatClause({
      spa::MODIFIES,
      { spa::LINE_NO, "12", {}},
      { spa::SYNONYM, "v", { spa::VARIABLE }}
    });
    query.addPatternClause({
      { spa::SYNONYM, "w", { spa::WHILE } },
      { spa::SYNONYM, "v", { spa::VARIABLE }},
      spa::Pattern { spa::ANY }
    });
    spa::PqlAndParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getSuchThatClauses().size(), size_t(1));
    Assert::AreEqual(query.getPatternClauses().size(), size_t(2));
    spa::PatternClause& clause = query.getPatternClauses().back();
    spa::PatternClause compare {
      { spa::SYNONYM, "a", { spa::ASSIGN } },
      { spa::SYNONYM, "v", { spa::VARIABLE }},
      spa::Pattern { spa::ANY }
    };
    Assert::IsTrue(clause == compare);
  }

  TEST_METHOD(TestAndSuchThat) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "and" });
    tokens.pushBack({ spa::TOKEN_NAME, "Modifies" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_INTEGER, "12" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("w", spa::WHILE);
    query.addDeclaration("a", spa::ASSIGN);
    query.addDeclaration("v", spa::VARIABLE);
    query.addPatternClause({
      { spa::SYNONYM, "w", { spa::WHILE } },
      { spa::SYNONYM, "v", { spa::VARIABLE }},
      spa::Pattern { spa::ANY }
    });
    query.addSuchThatClause({
      spa::USES,
      { spa::LINE_NO, "14", {}},
      { spa::SYNONYM, "v", { spa::VARIABLE }}
    });
    spa::PqlAndParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getSuchThatClauses().size(), size_t(2));
    Assert::AreEqual(query.getPatternClauses().size(), size_t(1));
    spa::SuchThatClause& clause = query.getSuchThatClauses().back();
    spa::SuchThatClause compare{
      spa::MODIFIES,
      { spa::LINE_NO, "12", {}},
      { spa::SYNONYM, "v", { spa::VARIABLE }}
    };
    Assert::IsTrue(clause == compare);
  }
  };
}  // namespace UnitTesting

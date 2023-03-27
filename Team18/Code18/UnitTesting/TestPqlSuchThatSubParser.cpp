#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "ParsedQuery.h"
#include "PqlParser.h"
#include "PqlSuchThatSubParser.h"
#include "Stream.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPqlSuchThatSubParser) {
public:
  TEST_METHOD(TestModifies) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "Modifies" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "s" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("s", spa::STMT);
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlSuchThatSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getSuchThatClauses().size(), size_t(1));
    Assert::IsTrue(query.getLastAddedClause() == spa::PqlClauseType::SUCH_THAT_CLAUSE);
    spa::SuchThatClause& clause = query.getSuchThatClauses()[0];
    spa::SuchThatClause compare(
      spa::MODIFIES,
      spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
      spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestParentStar) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "Parent", 0 });
    tokens.pushBack({ spa::TOKEN_MULTIPLY, "*", 6 });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(", 7 });
    tokens.pushBack({ spa::TOKEN_NAME, "s", 8 });
    tokens.pushBack({ spa::TOKEN_COMMA, ",", 9 });
    tokens.pushBack({ spa::TOKEN_NAME, "s1", 10 });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")", 12 });
    spa::ParsedQuery query;
    query.addDeclaration("s", spa::STMT);
    query.addDeclaration("s1", spa::STMT);
    spa::PqlSuchThatSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getSuchThatClauses().size(), size_t(1));
    Assert::IsTrue(query.getLastAddedClause() == spa::PqlClauseType::SUCH_THAT_CLAUSE);
    spa::SuchThatClause& clause = query.getSuchThatClauses()[0];
    spa::SuchThatClause compare(
      spa::PARENT_STAR,
      spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
      spa::PqlArgument(spa::SYNONYM, "s1", { spa::STMT }));
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestStarAfterSpace) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "Follows", 0 });
    tokens.pushBack({ spa::TOKEN_MULTIPLY, "*", 8 });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(", 9 });
    tokens.pushBack({ spa::TOKEN_NAME, "s", 10 });
    tokens.pushBack({ spa::TOKEN_COMMA, ",", 11 });
    tokens.pushBack({ spa::TOKEN_NAME, "s1", 12 });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")", 14 });
    spa::ParsedQuery query;
    query.addDeclaration("s", spa::STMT);
    query.addDeclaration("s1", spa::STMT);
    spa::PqlSuchThatSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SYNTAX_ERROR);
  }

  TEST_METHOD(TestFirstArgMismatch) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "Follows" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    tokens.pushBack({ spa::TOKEN_NAME, "s" });
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "s1" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("s1", spa::STMT);
    spa::PqlSuchThatSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SYNTAX_ERROR);
  }

  TEST_METHOD(TestSecondArgMismatch) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "Calls" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "v1" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_INTEGER, "123" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("v1", spa::VARIABLE);
    spa::PqlSuchThatSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SYNTAX_ERROR);
  }
  };
}  // namespace UnitTesting

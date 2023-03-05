#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "ParsedQuery.h"
#include "PqlParser.h"
#include "PqlWithSubParser.h"
#include "Stream.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPqlWithSubParser) {
public:
  TEST_METHOD(TestNameAndName) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    tokens.pushBack({ spa::TOKEN_NAME, "hayley" });
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    tokens.pushBack({ spa::TOKEN_NAME, "alex" });
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    spa::ParsedQuery query;
    spa::PqlWithSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getWithClauses().size(), size_t(1));
    Assert::IsTrue(query.getLastAddedClause() == spa::PqlClauseType::WITH_CLAUSE);
    spa::WithClause& clause = query.getWithClauses()[0];
    spa::WithClause compare(
      spa::WithArgument(spa::QpsValue("hayley")),
      spa::WithArgument(spa::QpsValue("alex")));
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestIntAndInt) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_INTEGER, "100" });
    tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
    tokens.pushBack({ spa::TOKEN_INTEGER, "20" });
    spa::ParsedQuery query;
    spa::PqlWithSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getWithClauses().size(), size_t(1));
    Assert::IsTrue(query.getLastAddedClause() == spa::PqlClauseType::WITH_CLAUSE);
    spa::WithClause& clause = query.getWithClauses()[0];
    spa::WithClause compare(
      spa::WithArgument(spa::QpsValue(100)),
      spa::WithArgument(spa::QpsValue(20)));
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestAttributeAndName) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_FULL_STOP, "." });
    tokens.pushBack({ spa::TOKEN_NAME, "varName" });
    tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    tokens.pushBack({ spa::TOKEN_NAME, "dunphy" });
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    spa::ParsedQuery query;
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlWithSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getWithClauses().size(), size_t(1));
    Assert::IsTrue(query.getLastAddedClause() == spa::PqlClauseType::WITH_CLAUSE);
    spa::WithClause& clause = query.getWithClauses()[0];
    spa::WithClause compare(
      spa::WithArgument("v.varName"),
      spa::WithArgument(spa::QpsValue("dunphy")));
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestAttributeAndInt) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_INTEGER, "123" });
    tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
    tokens.pushBack({ spa::TOKEN_NAME, "c" });
    tokens.pushBack({ spa::TOKEN_FULL_STOP, "." });
    tokens.pushBack({ spa::TOKEN_NAME, "stmt" });
    tokens.pushBack({ spa::TOKEN_HASH, "#" });
    spa::ParsedQuery query;
    query.addDeclaration("c", spa::CALL);
    spa::PqlWithSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getWithClauses().size(), size_t(1));
    Assert::IsTrue(query.getLastAddedClause() == spa::PqlClauseType::WITH_CLAUSE);
    spa::WithClause& clause = query.getWithClauses()[0];
    spa::WithClause compare(
      spa::WithArgument(spa::QpsValue(123)),
      spa::WithArgument("c.stmt#"));
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestAttributeAndAttribute) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "s" });
    tokens.pushBack({ spa::TOKEN_FULL_STOP, "." });
    tokens.pushBack({ spa::TOKEN_NAME, "stmt" });
    tokens.pushBack({ spa::TOKEN_HASH, "#" });
    tokens.pushBack({ spa::TOKEN_EQUAL, "=" });
    tokens.pushBack({ spa::TOKEN_NAME, "c" });
    tokens.pushBack({ spa::TOKEN_FULL_STOP, "." });
    tokens.pushBack({ spa::TOKEN_NAME, "stmt" });
    tokens.pushBack({ spa::TOKEN_HASH, "#" });
    spa::ParsedQuery query;
    query.addDeclaration("s", spa::STMT);
    query.addDeclaration("c", spa::CALL);
    spa::PqlWithSubParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getWithClauses().size(), size_t(1));
    Assert::IsTrue(query.getLastAddedClause() == spa::PqlClauseType::WITH_CLAUSE);
    spa::WithClause& clause = query.getWithClauses()[0];
    spa::WithClause compare(
      spa::WithArgument("s.stmt#"),
      spa::WithArgument("c.stmt#"));
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }
  };
}  // namespace UnitTesting

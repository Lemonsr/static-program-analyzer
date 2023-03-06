#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "ParsedQuery.h"
#include "PqlParser.h"
#include "PqlSelectParser.h"
#include "Stream.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPqlSelectParser) {
public:
  TEST_METHOD(TestBoolean) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "Select" });
    tokens.pushBack({ spa::TOKEN_NAME, "BOOLEAN" });
    spa::ParsedQuery query;
    spa::PqlSelectParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::IsTrue(query.getSelectClauseType() == spa::SelectClauseType::SELECT_BOOLEAN);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestSimpleSynonym) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "Select" });
    tokens.pushBack({ spa::TOKEN_NAME, "BOOLEAN" });
    spa::ParsedQuery query;
    query.addDeclaration("BOOLEAN", spa::ASSIGN);
    spa::PqlSelectParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::IsTrue(query.getSelectClauseType() == spa::SelectClauseType::SELECT_TUPLE);
    Assert::AreEqual(query.getSelectColumns().size(), size_t(1));
    Assert::AreEqual(query.getSelectColumns()[0], std::string("BOOLEAN"));
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestSimpleAttribute) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "Select" });
    tokens.pushBack({ spa::TOKEN_NAME, "BOOLEAN" });
    tokens.pushBack({ spa::TOKEN_FULL_STOP, "." });
    tokens.pushBack({ spa::TOKEN_NAME, "procName" });
    spa::ParsedQuery query;
    query.addDeclaration("BOOLEAN", spa::CALL);
    spa::PqlSelectParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::IsTrue(query.getSelectClauseType() == spa::SelectClauseType::SELECT_TUPLE);
    Assert::AreEqual(query.getSelectColumns().size(), size_t(1));
    Assert::AreEqual(query.getSelectColumns()[0], std::string("BOOLEAN.procName"));
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestTuple) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "Select" });
    tokens.pushBack({ spa::TOKEN_COND_LT, "<" });
    tokens.pushBack({ spa::TOKEN_NAME, "BOOLEAN" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "BOOLEAN" });
    tokens.pushBack({ spa::TOKEN_FULL_STOP, "." });
    tokens.pushBack({ spa::TOKEN_NAME, "stmt"});
    tokens.pushBack({ spa::TOKEN_HASH, "#" });
    tokens.pushBack({ spa::TOKEN_COND_GT, ">" });
    spa::ParsedQuery query;
    query.addDeclaration("BOOLEAN", spa::ASSIGN);
    spa::PqlSelectParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::IsTrue(query.getSelectClauseType() == spa::SelectClauseType::SELECT_TUPLE);
    Assert::AreEqual(query.getSelectColumns().size(), size_t(2));
    Assert::AreEqual(query.getSelectColumns()[0], std::string("BOOLEAN"));
    Assert::AreEqual(query.getSelectColumns()[1], std::string("BOOLEAN.stmt#"));
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
    spa::PqlSelectParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_MISMATCH);
  }

  TEST_METHOD(TestIncorrectSyntax) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "Select" });
    tokens.pushBack({ spa::TOKEN_COND_LT, "<" });
    tokens.pushBack({ spa::TOKEN_NAME, "BOOLEAN" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "BOOLEAN" });
    tokens.pushBack({ spa::TOKEN_FULL_STOP, "." });
    tokens.pushBack({ spa::TOKEN_NAME, "stmt" });
    tokens.pushBack({ spa::TOKEN_HASH, "#" });
    tokens.pushBack({ spa::TOKEN_COND_GT, ">" });
    spa::ParsedQuery query;
    query.addDeclaration("BOOLEAN", spa::ASSIGN);
    spa::PqlSelectParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SYNTAX_ERROR);
  }
  };
}  // namespace UnitTesting

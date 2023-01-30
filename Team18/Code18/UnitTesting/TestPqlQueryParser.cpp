#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "ParsedQuery.h"
#include "PqlParser.h"
#include "PqlQueryParser.h"
#include "Stream.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPqlQueryParser) {
public:
  TEST_METHOD(TestOnlySelect) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "variable" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "Select" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    spa::ParsedQuery query;
    spa::PqlQueryParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getSelectSynonym(), std::string("v"));
    Assert::IsFalse(query.getSuchThatClause().has_value());
    Assert::IsFalse(query.getPatternClause().has_value());
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestSelectSuchThat) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "assign" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "variable" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "Select" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_NAME, "such" });
    tokens.pushBack({ spa::TOKEN_NAME, "that" });
    tokens.pushBack({ spa::TOKEN_NAME, "Modifies" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    spa::PqlQueryParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getSelectSynonym(), std::string("v"));
    Assert::IsFalse(query.getPatternClause().has_value());
    std::optional<spa::SuchThatClause> opt = query.getSuchThatClause();
    Assert::IsTrue(opt.has_value());
    spa::SuchThatClause& clause = opt.value();
    spa::SuchThatClause compare(
      spa::MODIFIES,
      spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
      spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestSelectPattern) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "assign" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "variable" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "Select" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_NAME, "pattern" });
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
    spa::PqlQueryParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getSelectSynonym(), std::string("a"));
    Assert::IsFalse(query.getSuchThatClause().has_value());
    std::optional<spa::PatternClause> opt = query.getPatternClause();
    Assert::IsTrue(opt.has_value());
    spa::PatternClause& clause = opt.value();
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

  TEST_METHOD(TestSelectSuchThatPattern) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "assign" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "variable" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "Select" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_NAME, "such" });
    tokens.pushBack({ spa::TOKEN_NAME, "that" });
    tokens.pushBack({ spa::TOKEN_NAME, "Modifies" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    tokens.pushBack({ spa::TOKEN_NAME, "pattern" });
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
    spa::PqlQueryParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getSelectSynonym(), std::string("a"));
    {
      std::optional<spa::SuchThatClause> opt = query.getSuchThatClause();
      Assert::IsTrue(opt.has_value());
      spa::SuchThatClause& clause = opt.value();
      spa::SuchThatClause compare(
        spa::MODIFIES,
        spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
        spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
      Assert::IsTrue(clause == compare);
    }
    {
      std::optional<spa::PatternClause> opt = query.getPatternClause();
      Assert::IsTrue(opt.has_value());
      spa::PatternClause& clause = opt.value();
      spa::PatternClause compare(
        { spa::SYNONYM, "a", spa::ASSIGN },
        spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }),
        spa::Pattern(spa::EXACT, {
          { spa::TOKEN_NAME, "x" },
          { spa::TOKEN_PLUS, "+" },
          { spa::TOKEN_NAME, "y" }
          }));
      Assert::IsTrue(clause == compare);
    }
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestSelectPatternSuchThat) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "assign" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "variable" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "Select" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_NAME, "pattern" });
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
    tokens.pushBack({ spa::TOKEN_NAME, "such" });
    tokens.pushBack({ spa::TOKEN_NAME, "that" });
    tokens.pushBack({ spa::TOKEN_NAME, "Modifies" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    spa::PqlQueryParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    Assert::AreEqual(query.getSelectSynonym(), std::string("a"));
    {
      std::optional<spa::SuchThatClause> opt = query.getSuchThatClause();
      Assert::IsTrue(opt.has_value());
      spa::SuchThatClause& clause = opt.value();
      spa::SuchThatClause compare(
        spa::MODIFIES,
        spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
        spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
      Assert::IsTrue(clause == compare);
    }
    {
      std::optional<spa::PatternClause> opt = query.getPatternClause();
      Assert::IsTrue(opt.has_value());
      spa::PatternClause& clause = opt.value();
      spa::PatternClause compare(
        { spa::SYNONYM, "a", spa::ASSIGN },
        spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }),
        spa::Pattern(spa::EXACT, {
          { spa::TOKEN_NAME, "x" },
          { spa::TOKEN_PLUS, "+" },
          { spa::TOKEN_NAME, "y" }
          }));
      Assert::IsTrue(clause == compare);
    }
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }
  };
}  // namespace UnitTesting

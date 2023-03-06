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
    Assert::AreEqual(query.getSelectColumns().size(), size_t(1));
    Assert::AreEqual(query.getSelectColumns()[0], std::string("v"));
    Assert::IsFalse(query.hasClauses());
    Assert::AreEqual(tokens.remaining(), int64_t(0));

    std::unordered_map<std::string, spa::DesignEntityType> usedDeclarations = query.getUsedDeclarations();
    Assert::IsTrue(usedDeclarations.size() == 1);
    Assert::IsTrue(usedDeclarations.find("v") != usedDeclarations.end());
  }

  TEST_METHOD(TestSelectSuchThat) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "assign" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "variable" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "while" });
    tokens.pushBack({ spa::TOKEN_NAME, "w" });
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
    Assert::IsTrue(query.hasClauses());
    Assert::AreEqual(query.getSelectColumns().size(), size_t(1));
    Assert::AreEqual(query.getSelectColumns()[0], std::string("v"));
    Assert::AreEqual(query.getPatternClauses().size(), size_t(0));
    Assert::AreEqual(query.getSuchThatClauses().size(), size_t(1));
    spa::SuchThatClause& clause = query.getSuchThatClauses()[0];
    spa::SuchThatClause compare(
      spa::MODIFIES,
      spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
      spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));

    std::unordered_map<std::string, spa::DesignEntityType> usedDeclarations = query.getUsedDeclarations();
    Assert::IsTrue(usedDeclarations.size() == 2);
    Assert::IsTrue(usedDeclarations.find("w") == usedDeclarations.end());
    Assert::IsTrue(usedDeclarations.find("a") != usedDeclarations.end());
    Assert::IsTrue(usedDeclarations.find("v") != usedDeclarations.end());
  }

  TEST_METHOD(TestSelectPattern) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "assign" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "variable" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "if" });
    tokens.pushBack({ spa::TOKEN_NAME, "i" });
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
    Assert::IsTrue(query.hasClauses());
    Assert::AreEqual(query.getSelectColumns().size(), size_t(1));
    Assert::AreEqual(query.getSelectColumns()[0], std::string("a"));
    Assert::AreEqual(query.getPatternClauses().size(), size_t(1));
    Assert::AreEqual(query.getSuchThatClauses().size(), size_t(0));
    spa::PatternClause& clause = query.getPatternClauses()[0];
    spa::PatternClause compare(
      { spa::SYNONYM, "a", spa::ASSIGN },
      spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }),
      spa::Pattern(spa::EXACT, {
        { spa::TOKEN_NAME, "x" },
        { spa::TOKEN_PLUS, "+" },
        { spa::TOKEN_NAME, "y" }
        }), 2);
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));

    std::unordered_map<std::string, spa::DesignEntityType> usedDeclarations = query.getUsedDeclarations();
    Assert::IsTrue(usedDeclarations.size() == 2);
    Assert::IsTrue(usedDeclarations.find("i") == usedDeclarations.end());
    Assert::IsTrue(usedDeclarations.find("a") != usedDeclarations.end());
    Assert::IsTrue(usedDeclarations.find("v") != usedDeclarations.end());
  }

  TEST_METHOD(TestSelectSuchThatPattern) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "assign" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
    tokens.pushBack({ spa::TOKEN_NAME, "variable" });
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "v1" });
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
    Assert::IsTrue(query.hasClauses());
    Assert::AreEqual(query.getSelectColumns().size(), size_t(1));
    Assert::AreEqual(query.getSelectColumns()[0], std::string("a"));
    Assert::AreEqual(query.getPatternClauses().size(), size_t(1));
    Assert::AreEqual(query.getSuchThatClauses().size(), size_t(1));
    {
      spa::SuchThatClause& clause = query.getSuchThatClauses()[0];
      spa::SuchThatClause compare(
        spa::MODIFIES,
        spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
        spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
      Assert::IsTrue(clause == compare);
    }
    {
      spa::PatternClause& clause = query.getPatternClauses()[0];
      spa::PatternClause compare(
        { spa::SYNONYM, "a", spa::ASSIGN },
        spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }),
        spa::Pattern(spa::EXACT, {
          { spa::TOKEN_NAME, "x" },
          { spa::TOKEN_PLUS, "+" },
          { spa::TOKEN_NAME, "y" }
          }), 2);
      Assert::IsTrue(clause == compare);
    }
    Assert::AreEqual(tokens.remaining(), int64_t(0));

    std::unordered_map<std::string, spa::DesignEntityType> usedDeclarations = query.getUsedDeclarations();
    Assert::IsTrue(usedDeclarations.size() == 2);
    Assert::IsTrue(usedDeclarations.find("v1") == usedDeclarations.end());
    Assert::IsTrue(usedDeclarations.find("a") != usedDeclarations.end());
    Assert::IsTrue(usedDeclarations.find("v") != usedDeclarations.end());
  }

  TEST_METHOD(TestSelectPatternSuchThat) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "assign" });
    tokens.pushBack({ spa::TOKEN_NAME, "a" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "a1" });
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
    Assert::IsTrue(query.hasClauses());
    Assert::AreEqual(query.getSelectColumns().size(), size_t(1));
    Assert::AreEqual(query.getSelectColumns()[0], std::string("a"));
    Assert::AreEqual(query.getPatternClauses().size(), size_t(1));
    Assert::AreEqual(query.getSuchThatClauses().size(), size_t(1));
    {
      spa::SuchThatClause& clause = query.getSuchThatClauses()[0];
      spa::SuchThatClause compare(
        spa::MODIFIES,
        spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
        spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
      Assert::IsTrue(clause == compare);
    }
    {
      spa::PatternClause& clause = query.getPatternClauses()[0];
      spa::PatternClause compare(
        { spa::SYNONYM, "a", spa::ASSIGN },
        spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }),
        spa::Pattern(spa::EXACT, {
          { spa::TOKEN_NAME, "x" },
          { spa::TOKEN_PLUS, "+" },
          { spa::TOKEN_NAME, "y" }
          }), 2);
      Assert::IsTrue(clause == compare);
    }
    Assert::AreEqual(tokens.remaining(), int64_t(0));

    std::unordered_map<std::string, spa::DesignEntityType> usedDeclarations = query.getUsedDeclarations();
    Assert::IsTrue(usedDeclarations.size() == 2);
    Assert::IsTrue(usedDeclarations.find("a1") == usedDeclarations.end());
    Assert::IsTrue(usedDeclarations.find("a") != usedDeclarations.end());
    Assert::IsTrue(usedDeclarations.find("v") != usedDeclarations.end());
  }
  };
}  // namespace UnitTesting

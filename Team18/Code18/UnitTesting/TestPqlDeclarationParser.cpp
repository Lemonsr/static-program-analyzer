#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "ParsedQuery.h"
#include "PqlParser.h"
#include "PqlDeclarationParser.h"
#include "Stream.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPqlDeclarationParser) {
    public:
      TEST_METHOD(TestSingleDeclaration) {
        spa::Stream<spa::Token> tokens;
        tokens.pushBack({ spa::TOKEN_NAME, "assign" });
        tokens.pushBack({ spa::TOKEN_NAME, "a" });
        tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
        spa::ParsedQuery query;
        spa::PqlDeclarationParser parser;
        spa::PqlParseStatus status = parser.parse(tokens, query);
        Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
        Assert::AreEqual(1, query.getDeclarationsCount());
        std::optional<spa::DesignEntityType> type = query.getType("a");
        Assert::IsTrue(type.has_value());
        Assert::IsTrue(spa::ASSIGN == type.value());
        Assert::AreEqual(int64_t(0), tokens.remaining());
      }

      TEST_METHOD(TestMultiDeclaration) {
        spa::Stream<spa::Token> tokens;
        tokens.pushBack({ spa::TOKEN_NAME, "read" });
        tokens.pushBack({ spa::TOKEN_NAME, "a" });
        tokens.pushBack({ spa::TOKEN_COMMA, "," });
        tokens.pushBack({ spa::TOKEN_NAME, "b" });
        tokens.pushBack({ spa::TOKEN_COMMA, "," });
        tokens.pushBack({ spa::TOKEN_NAME, "c" });
        tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
        spa::ParsedQuery query;
        spa::PqlDeclarationParser parser;
        spa::PqlParseStatus status = parser.parse(tokens, query);
        Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
        Assert::AreEqual(3, query.getDeclarationsCount());
        std::vector<std::string> variables { "a", "b", "c" };
        for (std::string& v : variables) {
          std::optional<spa::DesignEntityType> type = query.getType(v);
          Assert::IsTrue(type.has_value());
          Assert::IsTrue(spa::READ == type.value());
        }
        Assert::AreEqual(int64_t(0), tokens.remaining());
      }

      TEST_METHOD(TestMismatch) {
        spa::Stream<spa::Token> tokens;
        tokens.pushBack({ spa::TOKEN_NAME, "Select" });
        tokens.pushBack({ spa::TOKEN_NAME, "a" });
        tokens.pushBack({ spa::TOKEN_NAME, "such" });
        tokens.pushBack({ spa::TOKEN_NAME, "that" });
        spa::ParsedQuery query;
        spa::PqlDeclarationParser parser;
        spa::PqlParseStatus status = parser.parse(tokens, query);
        Assert::IsTrue(status == spa::PQL_PARSE_MISMATCH);
      }

      TEST_METHOD(TestDuplicateDeclaration) {
        spa::Stream<spa::Token> tokens;
        tokens.pushBack({ spa::TOKEN_NAME, "stmt" });
        tokens.pushBack({ spa::TOKEN_NAME, "a" });
        tokens.pushBack({ spa::TOKEN_COMMA, "," });
        tokens.pushBack({ spa::TOKEN_NAME, "b" });
        tokens.pushBack({ spa::TOKEN_COMMA, "," });
        tokens.pushBack({ spa::TOKEN_NAME, "a" });
        tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
        spa::ParsedQuery query;
        spa::PqlDeclarationParser parser;
        spa::PqlParseStatus status = parser.parse(tokens, query);
        Assert::IsTrue(status == spa::PQL_PARSE_ERROR);
      }

      TEST_METHOD(TestIncorrectDeclaration) {
        spa::Stream<spa::Token> tokens;
        tokens.pushBack({ spa::TOKEN_NAME, "procedure" });
        tokens.pushBack({ spa::TOKEN_NAME, "a" });
        tokens.pushBack({ spa::TOKEN_COMMA, "," });
        tokens.pushBack({ spa::TOKEN_COMMA, "b" });
        tokens.pushBack({ spa::TOKEN_COMMA, "," });
        tokens.pushBack({ spa::TOKEN_SEMICOLON, ";" });
        spa::ParsedQuery query;
        spa::PqlDeclarationParser parser;
        spa::PqlParseStatus status = parser.parse(tokens, query);
        Assert::IsTrue(status == spa::PQL_PARSE_ERROR);
      }
  };
}  // namespace UnitTesting

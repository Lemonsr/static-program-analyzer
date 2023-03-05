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
        Assert::AreEqual(std::size_t(1), query.getDeclarations().size());
        Assert::IsTrue(spa::ASSIGN == query.getDeclarationType("a"));
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
        Assert::AreEqual(std::size_t(3), query.getDeclarations().size());
        std::vector<std::string> variables { "a", "b", "c" };
        for (std::string& v : variables) {
          Assert::IsTrue(spa::READ == query.getDeclarationType(v));
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
        Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
        Assert::AreEqual(std::size_t(2), query.getDeclarations().size());
        Assert::IsTrue(spa::STMT == query.getDeclarationType("a"));
        Assert::IsTrue(spa::STMT == query.getDeclarationType("b"));
        auto& declarationsCount = query.getDeclarationsCount();
        Assert::AreEqual(declarationsCount["a"], 2);
        Assert::AreEqual(declarationsCount["b"], 1);
        Assert::AreEqual(int64_t(0), tokens.remaining());
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
        Assert::IsTrue(status == spa::PQL_PARSE_SYNTAX_ERROR);
      }
  };
}  // namespace UnitTesting

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "ParsedQuery.h"
#include "PqlAttributeParser.h"
#include "Stream.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPqlAttributeParser) {
public:
  TEST_METHOD(TestCorrectAttribute) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "r" });
    tokens.pushBack({ spa::TOKEN_FULL_STOP, "." });
    tokens.pushBack({ spa::TOKEN_NAME, "varName" });
    spa::ParsedQuery query;
    query.addDeclaration("r", spa::READ);
    spa::PqlAttributeParser parser;
    std::optional<std::string> attributeOpt = parser.parse(tokens, query);
    Assert::IsTrue(attributeOpt.has_value());
    Assert::AreEqual(attributeOpt.value(), std::string("r.varName"));
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestInvalidSyntax) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "s" });
    tokens.pushBack({ spa::TOKEN_FULL_STOP, "." });
    tokens.pushBack({ spa::TOKEN_HASH, "#" });
    tokens.pushBack({ spa::TOKEN_NAME, "stmt" });
    spa::ParsedQuery query;
    query.addDeclaration("s", spa::STMT);
    spa::PqlAttributeParser parser;
    std::optional<std::string> attributeOpt = parser.parse(tokens, query);
    Assert::IsFalse(attributeOpt.has_value());
    Assert::AreEqual(tokens.remaining(), int64_t(4));
  }

  TEST_METHOD(TestNonExistentSynonym) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "s" });
    tokens.pushBack({ spa::TOKEN_FULL_STOP, "." });
    tokens.pushBack({ spa::TOKEN_NAME, "stmt" });
    tokens.pushBack({ spa::TOKEN_HASH, "#" });
    spa::ParsedQuery query;
    query.addDeclaration("p", spa::PRINT);
    spa::PqlAttributeParser parser;
    std::optional<std::string> attributeOpt = parser.parse(tokens, query);
    Assert::IsFalse(attributeOpt.has_value());
    Assert::AreEqual(tokens.remaining(), int64_t(4));
  }

  TEST_METHOD(TestWrongAttribute) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "v" });
    tokens.pushBack({ spa::TOKEN_FULL_STOP, "." });
    tokens.pushBack({ spa::TOKEN_NAME, "stmt" });
    tokens.pushBack({ spa::TOKEN_HASH, "#" });
    spa::ParsedQuery query;
    query.addDeclaration("v", spa::VARIABLE);
    spa::PqlAttributeParser parser;
    std::optional<std::string> attributeOpt = parser.parse(tokens, query);
    Assert::IsFalse(attributeOpt.has_value());
    Assert::AreEqual(tokens.remaining(), int64_t(4));
  }
  };
}  // namespace UnitTesting

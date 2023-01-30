#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "ParsedQuery.h"
#include "PqlParser.h"
#include "PqlArgumentParser.h"
#include "Stream.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPqlArgumentParser) {
public:
  TEST_METHOD(TestSynonym) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "s" });
    spa::ParsedQuery query;
    query.addDeclaration("s", spa::STMT);
    spa::PqlArgumentParser parser;
    std::optional<spa::PqlArgument> opt = parser.parse(tokens, query);
    Assert::IsTrue(opt.has_value());
    spa::PqlArgument& arg = opt.value();
    spa::PqlArgument compare(spa::SYNONYM, "s", { spa::STMT });
    Assert::IsTrue(arg == compare);
    Assert::AreEqual(int64_t(0), tokens.remaining());
  }

  TEST_METHOD(TestWildcard) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_UNDERSCORE, "_" });
    spa::ParsedQuery query;
    spa::PqlArgumentParser parser;
    std::optional<spa::PqlArgument> opt = parser.parse(tokens, query);
    Assert::IsTrue(opt.has_value());
    spa::PqlArgument& arg = opt.value();
    spa::PqlArgument compare(spa::WILDCARD, "_", {});
    Assert::IsTrue(arg == compare);
    Assert::AreEqual(int64_t(0), tokens.remaining());
  }

  TEST_METHOD(TestLineNo) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_INTEGER, "123" });
    spa::ParsedQuery query;
    spa::PqlArgumentParser parser;
    std::optional<spa::PqlArgument> opt = parser.parse(tokens, query);
    Assert::IsTrue(opt.has_value());
    spa::PqlArgument& arg = opt.value();
    spa::PqlArgument compare(spa::LINE_NO, "123", {});
    Assert::IsTrue(arg == compare);
    Assert::AreEqual(int64_t(0), tokens.remaining());
  }

  TEST_METHOD(TestVariableName) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    tokens.pushBack({ spa::TOKEN_NAME, "x" });
    tokens.pushBack({ spa::TOKEN_DOUBLE_QUOTES, "\"" });
    spa::ParsedQuery query;
    spa::PqlArgumentParser parser;
    std::optional<spa::PqlArgument> opt = parser.parse(tokens, query);
    Assert::IsTrue(opt.has_value());
    spa::PqlArgument& arg = opt.value();
    spa::PqlArgument compare(spa::VARIABLE_NAME, "x", {});
    Assert::IsTrue(arg == compare);
    Assert::AreEqual(int64_t(0), tokens.remaining());
  }

  TEST_METHOD(TestUnknownSynonym) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "s" });
    spa::ParsedQuery query;
    spa::PqlArgumentParser parser;
    std::optional<spa::PqlArgument> opt = parser.parse(tokens, query);
    Assert::IsFalse(opt.has_value());
  }
  };
}  // namespace UnitTesting

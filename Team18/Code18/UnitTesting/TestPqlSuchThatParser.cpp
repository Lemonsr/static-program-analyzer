#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "ParsedQuery.h"
#include "PqlParser.h"
#include "PqlSuchThatParser.h"
#include "Stream.h"
#include "Token.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPqlSuchThatParser) {
public:
  TEST_METHOD(TestModifies) {
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
    spa::PqlSuchThatParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    std::optional<spa::SuchThatClause> opt = query.getSuchThatClause();
    Assert::IsTrue(opt.has_value());
    spa::SuchThatClause& clause = opt.value();
    spa::SuchThatClause compare(
      spa::MODIFIES, 
      spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
      spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE })
    );
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }

  TEST_METHOD(TestParentStar) {
    spa::Stream<spa::Token> tokens;
    tokens.pushBack({ spa::TOKEN_NAME, "such" });
    tokens.pushBack({ spa::TOKEN_NAME, "that" });
    tokens.pushBack({ spa::TOKEN_NAME, "Parent" });
    tokens.pushBack({ spa::TOKEN_MULTIPLY, "*" });
    tokens.pushBack({ spa::TOKEN_OPEN_BRACKET, "(" });
    tokens.pushBack({ spa::TOKEN_NAME, "s" });
    tokens.pushBack({ spa::TOKEN_COMMA, "," });
    tokens.pushBack({ spa::TOKEN_NAME, "s1" });
    tokens.pushBack({ spa::TOKEN_CLOSE_BRACKET, ")" });
    spa::ParsedQuery query;
    query.addDeclaration("s", spa::STMT);
    query.addDeclaration("s1", spa::STMT);
    spa::PqlSuchThatParser parser;
    spa::PqlParseStatus status = parser.parse(tokens, query);
    Assert::IsTrue(status == spa::PQL_PARSE_SUCCESS);
    std::optional<spa::SuchThatClause> opt = query.getSuchThatClause();
    Assert::IsTrue(opt.has_value());
    spa::SuchThatClause& clause = opt.value();
    spa::SuchThatClause compare(
      spa::PARENT_STAR,
      spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
      spa::PqlArgument(spa::SYNONYM, "s1", { spa::STMT })
    );
    Assert::IsTrue(clause == compare);
    Assert::AreEqual(tokens.remaining(), int64_t(0));
  }
  };
}  // namespace UnitTesting

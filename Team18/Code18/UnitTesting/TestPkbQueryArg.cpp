#include <cstdint>
#include <optional>
#include <string>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "PqlArgument.h"
#include "PKBQueryArg.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPKBQueryArg) {
public:
  TEST_METHOD(TestAssignSynonym) {
    spa::PqlArgument pqlArg(spa::SYNONYM, "s", { spa::ASSIGN });
    spa::PKBQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PKBQueryArgType::STATEMENT);
    Assert::IsTrue(queryArg.getStatement().statementType ==
                   spa::StatementType::ASSIGN);
  }

  TEST_METHOD(TestProcedureSynonym) {
    spa::PqlArgument pqlArg(spa::SYNONYM, "p", { spa::PROCEDURE });
    spa::PKBQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PKBQueryArgType::PROCEDURE);
  }

  TEST_METHOD(TestVariableSynonym) {
    spa::PqlArgument pqlArg(spa::SYNONYM, "v", { spa::VARIABLE });
    spa::PKBQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PKBQueryArgType::VARIABLE);
  }

  TEST_METHOD(TestConstantSynonym) {
    spa::PqlArgument pqlArg(spa::SYNONYM, "c", { spa::CONSTANT });
    spa::PKBQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PKBQueryArgType::CONSTANT);
  }

  TEST_METHOD(TestWildcard) {
    spa::PqlArgument pqlArg(spa::WILDCARD, "_", {});
    spa::PKBQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PKBQueryArgType::UNDERSCORE);
  }

  TEST_METHOD(TestLineNo) {
    spa::PqlArgument pqlArg(spa::LINE_NO, "123", {});
    spa::PKBQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PKBQueryArgType::LINE_NUMBER);
    Assert::AreEqual(queryArg.getLineNumber().lineNo, size_t(123));
  }

  TEST_METHOD(TestVariableName) {
    spa::PqlArgument pqlArg(spa::VARIABLE_NAME, "main", {});
    spa::PKBQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PKBQueryArgType::NAME);
    Assert::AreEqual(queryArg.getName().name, std::string("main"));
  }
  };
}  // namespace UnitTesting

#include <cstdint>
#include <optional>
#include <string>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignEntity.h"
#include "PqlArgument.h"
#include "PkbQueryArg.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPkbQueryArg) {
public:
  TEST_METHOD(TestAssignSynonym) {
    spa::PqlArgument pqlArg(spa::SYNONYM, "s", { spa::ASSIGN });
    spa::PkbQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PkbQueryArgType::ASSIGN);
    Assert::IsTrue(queryArg.getStatement().statementType == spa::StatementType::ASSIGN);
  }

  TEST_METHOD(TestProcedureSynonym) {
    spa::PqlArgument pqlArg(spa::SYNONYM, "p", { spa::PROCEDURE });
    spa::PkbQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PkbQueryArgType::PROCEDURE);
  }

  TEST_METHOD(TestVariableSynonym) {
    spa::PqlArgument pqlArg(spa::SYNONYM, "v", { spa::VARIABLE });
    spa::PkbQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PkbQueryArgType::VARIABLE);
  }

  TEST_METHOD(TestConstantSynonym) {
    spa::PqlArgument pqlArg(spa::SYNONYM, "c", { spa::CONSTANT });
    spa::PkbQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PkbQueryArgType::CONSTANT);
  }

  TEST_METHOD(TestWildcard) {
    spa::PqlArgument pqlArg(spa::WILDCARD, "_", {});
    spa::PkbQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PkbQueryArgType::UNDERSCORE);
  }

  TEST_METHOD(TestLineNo) {
    spa::PqlArgument pqlArg(spa::LINE_NO, "123", {});
    spa::PkbQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PkbQueryArgType::LINE_NUMBER);
    Assert::AreEqual(queryArg.getLineNumber().lineNo, size_t(123));
  }

  TEST_METHOD(TestVariableName) {
    spa::PqlArgument pqlArg(spa::VARIABLE_NAME, "main", {});
    spa::PkbQueryArg queryArg(pqlArg);
    Assert::IsTrue(queryArg.getType() == spa::PkbQueryArgType::NAME);
    Assert::AreEqual(queryArg.getName().name, std::string("main"));
  }
  };
}  // namespace UnitTesting

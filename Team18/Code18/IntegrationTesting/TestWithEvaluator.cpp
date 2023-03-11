#include <memory>

#include "stdafx.h"
#include "CppUnitTest.h"

#include "ParsedQuery.h"
#include "QpsEvaluator.h"
#include "QpsResultTable.h"
#include "PKB.h"
#include "PKBManager.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace IntegrationTesting {
  TEST_CLASS(TestWithEvaluator) {
public:
  TEST_METHOD(TestValueValueFalse) {
    spa::WithClause clause(spa::WithArgument(spa::QpsValue(1)), spa::WithArgument(spa::QpsValue(2)));
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 1);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestValueValueTrue) {
    spa::WithClause clause(spa::WithArgument(spa::QpsValue(1)), spa::WithArgument(spa::QpsValue(1)));
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 1);
    Assert::AreEqual(dim.second, 1);
    Assert::IsTrue(table.getRows()[0][0] == spa::QpsValue(1));
  }

  TEST_METHOD(TestAttributeValue) {
    spa::WithClause clause(spa::WithArgument("s.stmt#"), spa::WithArgument(spa::QpsValue(1)));
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 1);
    Assert::AreEqual(dim.second, 1);
    auto valSet = table.getColumn("s.stmt#");
    Assert::AreEqual(valSet.size(), size_t(1));
    Assert::IsTrue(valSet.find(spa::QpsValue(1)) != valSet.end());
  }

  TEST_METHOD(TestAttributeAttribute) {
    spa::WithClause clause(spa::WithArgument("s.stmt#"), spa::WithArgument("a.stmt#"));
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    spa::QpsResultTable table;
    table.addHeader("s.stmt#");
    table.addHeader("a.stmt#");
    table.addHeader("v");
    table.addRow({ spa::QpsValue(1), spa::QpsValue(1), spa::QpsValue("x")});
    table.addRow({ spa::QpsValue(1), spa::QpsValue(2), spa::QpsValue("x") });
    auto result = table.matchColumns("s.stmt#", "a.stmt#");
    auto dim = result.getDimension();
    Assert::AreEqual(dim.first, 3);
    Assert::AreEqual(dim.second, 1);
    auto& rows = result.getRows();
    Assert::IsTrue(rows[0][0] == rows[0][1]);
    Assert::IsTrue(rows[0][2] == spa::QpsValue("x"));
  }
  };
}  // namespace IntegrationTesting

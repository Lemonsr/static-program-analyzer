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
  TEST_CLASS(TestModifiesEvaluator) {
public:
  TEST_METHOD(TestLineVarNameExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::LINE_NO, "10", {}),
                               spa::PqlArgument(spa::VARIABLE_NAME, "x", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES, "10", "x");
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "12", "x");
    pkbManager->addRelationship(spa::MODIFIES, "13", "x");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestLineVarNameNotExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::LINE_NO, "10", {}),
                               spa::PqlArgument(spa::VARIABLE_NAME, "x", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES, "10", "y");
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "12", "x");
    pkbManager->addRelationship(spa::MODIFIES, "13", "x");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineUnderscoreExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::LINE_NO, "10", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES, "10", "y");
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "12", "z");
    pkbManager->addRelationship(spa::MODIFIES, "13", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestLineUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::LINE_NO, "10", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "12", "z");
    pkbManager->addRelationship(spa::MODIFIES, "13", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineVarExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::LINE_NO, "10", {}),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES, "10", "y");
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "12", "z");
    pkbManager->addRelationship(spa::MODIFIES, "13", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("v");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue("y")) != columnVals.end());
  }

  TEST_METHOD(TestLineVarNotExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::LINE_NO, "10", {}),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "12", "z");
    pkbManager->addRelationship(spa::MODIFIES, "13", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStmtVarNameExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::VARIABLE_NAME, "y", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES, "10", "y");
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "12", "y");
    pkbManager->addRelationship(spa::MODIFIES, "13", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(10)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(12)) != columnVals.end());
  }

  TEST_METHOD(TestStmtVarNameNotExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::VARIABLE_NAME, "val", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES, "10", "y");
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "12", "y");
    pkbManager->addRelationship(spa::MODIFIES, "13", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestAssignStmtUnderscore) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("10", spa::StatementType::READ);
    pkbManager->addStatementType("11", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("12", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("13", spa::StatementType::READ);
    pkbManager->addRelationship(spa::MODIFIES, "10", "y");
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "12", "y");
    pkbManager->addRelationship(spa::MODIFIES, "13", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(11)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(12)) != columnVals.end());
  }
  TEST_METHOD(TestReadStmtVar) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "r", { spa::READ }),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("10", spa::StatementType::READ);
    pkbManager->addStatementType("11", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("12", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("13", spa::StatementType::READ);
    pkbManager->addRelationship(spa::MODIFIES, "10", "y");
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "12", "z");
    pkbManager->addRelationship(spa::MODIFIES, "13", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("r");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(10)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(13)) != columnVals.end());
    columnVals = table.getColumn("v");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue("y")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("w")) != columnVals.end());
  }
  };
}  // namespace IntegrationTesting

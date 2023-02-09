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
  TEST_CLASS(TestUsesEvaluator) {
public:
  TEST_METHOD(TestLineVarNameExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::VARIABLE_NAME, "x", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestMultipleLineVarNameExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::VARIABLE_NAME, "x", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "2", "y");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestLineVarNameNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LINE_NO, "4", {}),
                               spa::PqlArgument(spa::VARIABLE_NAME, "z", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "x");
    pkbManager->addRelationship(spa::USES, "5", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineVarExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LINE_NO, "5", {}),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("v");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue("a")) != columnVals.end());
  }

  TEST_METHOD(TestLineVarMultipleExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LINE_NO, "5", {}),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    pkbManager->addRelationship(spa::USES, "5", "x");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("v");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue("a")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("x")) != columnVals.end());
  }

  TEST_METHOD(TestLineVarNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LINE_NO, "5", {}),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineUnderscoreExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestLineUnderscoreMultipleExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES, "2", "y");
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestLineUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStmtVarNameExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::VARIABLE_NAME, "x", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
  }

  TEST_METHOD(TestStmtVarNameMultipleExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::VARIABLE_NAME, "a", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "a");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestStmtVarNameNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::VARIABLE_NAME, "b", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestAssignStmtUnderscore) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::PRINT);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("5", spa::StatementType::PRINT);
    pkbManager->addStatementType("6", spa::StatementType::IF);
    pkbManager->addStatementType("10", spa::StatementType::WHILE);
    pkbManager->addStatementType("11", spa::StatementType::IF);
    pkbManager->addStatementType("15", spa::StatementType::WHILE);
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    pkbManager->addRelationship(spa::USES, "6", "x");
    pkbManager->addRelationship(spa::USES, "10", "y");
    pkbManager->addRelationship(spa::USES, "11", "z");
    pkbManager->addRelationship(spa::USES, "15", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }
  TEST_METHOD(TestPrintStmtVar) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::PRINT);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("5", spa::StatementType::PRINT);
    pkbManager->addStatementType("6", spa::StatementType::IF);
    pkbManager->addStatementType("10", spa::StatementType::WHILE);
    pkbManager->addStatementType("11", spa::StatementType::IF);
    pkbManager->addStatementType("15", spa::StatementType::WHILE);
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    pkbManager->addRelationship(spa::USES, "6", "x");
    pkbManager->addRelationship(spa::USES, "10", "y");
    pkbManager->addRelationship(spa::USES, "11", "z");
    pkbManager->addRelationship(spa::USES, "15", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
    columnVals = table.getColumn("v");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue("x")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("a")) != columnVals.end());
  }

  TEST_METHOD(TestIfStmtVar) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::PRINT);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("5", spa::StatementType::PRINT);
    pkbManager->addStatementType("6", spa::StatementType::IF);
    pkbManager->addStatementType("10", spa::StatementType::WHILE);
    pkbManager->addStatementType("11", spa::StatementType::IF);
    pkbManager->addStatementType("15", spa::StatementType::WHILE);
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    pkbManager->addRelationship(spa::USES, "6", "x");
    pkbManager->addRelationship(spa::USES, "10", "y");
    pkbManager->addRelationship(spa::USES, "11", "z");
    pkbManager->addRelationship(spa::USES, "15", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(6)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(11)) != columnVals.end());
    columnVals = table.getColumn("v");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue("x")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("z")) != columnVals.end());
  }

  TEST_METHOD(TestWhileStmtVar) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::PRINT);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("5", spa::StatementType::PRINT);
    pkbManager->addStatementType("6", spa::StatementType::IF);
    pkbManager->addStatementType("10", spa::StatementType::WHILE);
    pkbManager->addStatementType("11", spa::StatementType::IF);
    pkbManager->addStatementType("15", spa::StatementType::WHILE);
    pkbManager->addRelationship(spa::USES, "2", "x");
    pkbManager->addRelationship(spa::USES, "3", "y");
    pkbManager->addRelationship(spa::USES, "4", "z");
    pkbManager->addRelationship(spa::USES, "5", "a");
    pkbManager->addRelationship(spa::USES, "6", "x");
    pkbManager->addRelationship(spa::USES, "10", "y");
    pkbManager->addRelationship(spa::USES, "11", "z");
    pkbManager->addRelationship(spa::USES, "15", "a");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(10)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(15)) != columnVals.end());
    columnVals = table.getColumn("v");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue("y")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("a")) != columnVals.end());
  }
  };
}  // namespace IntegrationTesting

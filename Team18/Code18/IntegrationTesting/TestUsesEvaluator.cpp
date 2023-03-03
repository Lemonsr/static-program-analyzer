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
                               spa::PqlArgument(spa::LITERAL_STRING, "x", {}));
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
                               spa::PqlArgument(spa::LITERAL_STRING, "x", {}));
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
                               spa::PqlArgument(spa::LITERAL_STRING, "z", {}));
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
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 5 &&
                   resultRows[0][1].getString() == "a");
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
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 5 &&
                   resultRows[0][1].getString() == "a");
    Assert::IsTrue(resultRows[1][0].getInteger() == 5 &&
                   resultRows[1][1].getString() == "x");
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
                               spa::PqlArgument(spa::LITERAL_STRING, "x", {}));
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
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 2 &&
                   resultRows[0][1].getString() == "x");
  }

  TEST_METHOD(TestStmtVarNameMultipleExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::LITERAL_STRING, "a", {}));
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
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 3 &&
                   resultRows[0][1].getString() == "a");
    Assert::IsTrue(resultRows[1][0].getInteger() == 5 &&
                   resultRows[1][1].getString() == "a");
  }

  TEST_METHOD(TestStmtVarNameNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::LITERAL_STRING, "b", {}));
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
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 3 &&
                   resultRows[0][1].getString() == "y");
    Assert::IsTrue(resultRows[1][0].getInteger() == 4 &&
                   resultRows[1][1].getString() == "z");
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
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 2 &&
                   resultRows[0][1].getString() == "x");
    Assert::IsTrue(resultRows[1][0].getInteger() == 5 &&
                   resultRows[1][1].getString() == "a");
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
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 11 &&
                   resultRows[0][1].getString() == "z");
    Assert::IsTrue(resultRows[1][0].getInteger() == 6 &&
                   resultRows[1][1].getString() == "x");
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
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 10 &&
                   resultRows[0][1].getString() == "y");
    Assert::IsTrue(resultRows[1][0].getInteger() == 15 &&
                   resultRows[1][1].getString() == "a");
  }

  TEST_METHOD(TestProcVarNameExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::LITERAL_STRING, "x", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "y");
    pkbManager->addRelationship(spa::USES, "11", "x");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getString() == "A" &&
                   resultRows[0][1].getString() == "x");
  }

  TEST_METHOD(TestProcVarNameNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::LITERAL_STRING, "x", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "y");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcUnderscoreExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "y");
    pkbManager->addRelationship(spa::USES, "11", "x");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 4);
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getString() == "A" &&
                   resultRows[0][1].getString() == "y");
    Assert::IsTrue(resultRows[1][0].getString() == "A" &&
                   resultRows[1][1].getString() == "x");
    Assert::IsTrue(resultRows[2][0].getString() == "B" &&
                   resultRows[2][1].getString() == "z");
    Assert::IsTrue(resultRows[3][0].getString() == "C" &&
                   resultRows[3][1].getString() == "w");
  }


  TEST_METHOD(TestProcUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcVarExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "y");
    pkbManager->addRelationship(spa::USES, "11", "x");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 4);
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getString() == "A" &&
                   resultRows[0][1].getString() == "y");
    Assert::IsTrue(resultRows[1][0].getString() == "A" &&
                   resultRows[1][1].getString() == "x");
    Assert::IsTrue(resultRows[2][0].getString() == "B" &&
                   resultRows[2][1].getString() == "z");
    Assert::IsTrue(resultRows[3][0].getString() == "C" &&
                   resultRows[3][1].getString() == "w");
  }

  TEST_METHOD(TestProcVarNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestCallVarNameExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }),
                               spa::PqlArgument(spa::LITERAL_STRING, "y", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("20", spa::StatementType::CALL);
    pkbManager->addCallsProc(20, "A");
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "x");
    pkbManager->addRelationship(spa::USES, "11", "y");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    pkbManager->addRelationship(spa::USES, "20", "x");
    pkbManager->addRelationship(spa::USES, "20", "y");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 20 &&
                   resultRows[0][1].getString() == "y");
  }

  TEST_METHOD(TestCallVarNameNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }),
                               spa::PqlArgument(spa::LITERAL_STRING, "y", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("20", spa::StatementType::CALL);
    pkbManager->addCallsProc(20, "A");
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "x");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    pkbManager->addRelationship(spa::USES, "20", "x");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestCallUnderscoreExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("20", spa::StatementType::CALL);
    pkbManager->addStatementType("16", spa::StatementType::CALL);
    pkbManager->addCallsProc(20, "A");
    pkbManager->addCallsProc(16, "C");
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "y");
    pkbManager->addRelationship(spa::USES, "11", "x");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "16", "w");
    pkbManager->addRelationship(spa::USES, "19", "w");
    pkbManager->addRelationship(spa::USES, "20", "x");
    pkbManager->addRelationship(spa::USES, "20", "y");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 3);
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 16 &&
                   resultRows[0][1].getString() == "w");
    Assert::IsTrue(resultRows[1][0].getInteger() == 20 &&
                   resultRows[1][1].getString() == "x");
    Assert::IsTrue(resultRows[2][0].getInteger() == 20 &&
                   resultRows[2][1].getString() == "y");
  }

  TEST_METHOD(TestCallUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "y");
    pkbManager->addRelationship(spa::USES, "11", "x");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestCallVarExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("20", spa::StatementType::CALL);
    pkbManager->addCallsProc(20, "A");
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "x");
    pkbManager->addRelationship(spa::USES, "11", "y");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    pkbManager->addRelationship(spa::USES, "20", "x");
    pkbManager->addRelationship(spa::USES, "20", "y");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 20 &&
                   resultRows[0][1].getString() == "x");
    Assert::IsTrue(resultRows[1][0].getInteger() == 20 &&
                   resultRows[1][1].getString() == "y");
  }

  TEST_METHOD(TestCallVarNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "x");
    pkbManager->addRelationship(spa::USES, "11", "y");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcNameVarNameExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LITERAL_STRING, "B", {}),
                               spa::PqlArgument(spa::LITERAL_STRING, "z", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "x");
    pkbManager->addRelationship(spa::USES, "11", "y");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestProcNameVarNameNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LITERAL_STRING, "B", {}),
                               spa::PqlArgument(spa::LITERAL_STRING, "x", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "x");
    pkbManager->addRelationship(spa::USES, "11", "y");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcNameUnderscoreExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LITERAL_STRING, "C", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "x");
    pkbManager->addRelationship(spa::USES, "11", "y");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestProcNameUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LITERAL_STRING, "D", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "x");
    pkbManager->addRelationship(spa::USES, "11", "y");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcNameVarExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LITERAL_STRING, "C", {}),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "x");
    pkbManager->addRelationship(spa::USES, "11", "y");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getString() == "C" &&
                   resultRows[0][1].getString() == "w");
  }

  TEST_METHOD(TestProcNameVarNotExists) {
    spa::SuchThatClause clause(spa::USES,
                               spa::PqlArgument(spa::LITERAL_STRING, "D", {}),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::USES_P, "A", "x");
    pkbManager->addRelationship(spa::USES_P, "A", "y");
    pkbManager->addRelationship(spa::USES_P, "B", "z");
    pkbManager->addRelationship(spa::USES_P, "C", "w");
    pkbManager->addRelationship(spa::USES, "10", "x");
    pkbManager->addRelationship(spa::USES, "11", "y");
    pkbManager->addRelationship(spa::USES, "15", "z");
    pkbManager->addRelationship(spa::USES, "19", "w");
    pkbManager->addRelationship(spa::USES, "20", "x");
    pkbManager->addRelationship(spa::USES, "20", "y");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }
  };
}  // namespace IntegrationTesting

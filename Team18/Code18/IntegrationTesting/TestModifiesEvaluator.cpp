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
                               spa::PqlArgument(spa::LITERAL_STRING, "x", {}));
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
                               spa::PqlArgument(spa::LITERAL_STRING, "x", {}));
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
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 10 &&
                   resultRows[0][1].getString() == "y");
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
                               spa::PqlArgument(spa::LITERAL_STRING, "y", {}));
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
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 10 &&
                   resultRows[0][1].getString() == "y");
    Assert::IsTrue(resultRows[1][0].getInteger() == 12 &&
                   resultRows[1][1].getString() == "y");
  }

  TEST_METHOD(TestStmtVarNameNotExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::LITERAL_STRING, "val", {}));
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
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 11 &&
                   resultRows[0][1].getString() == "x");
    Assert::IsTrue(resultRows[1][0].getInteger() == 12 &&
                   resultRows[1][1].getString() == "y");
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
    std::vector<std::vector<spa::QpsValue>> resultRows = table.getRows();
    Assert::IsTrue(resultRows[0][0].getInteger() == 10 &&
                   resultRows[0][1].getString() == "y");
    Assert::IsTrue(resultRows[1][0].getInteger() == 13 &&
                   resultRows[1][1].getString() == "w");
  }

  TEST_METHOD(TestProcVarNameExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::LITERAL_STRING, "x", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "y");
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
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
    spa::SuchThatClause clause(spa::MODIFIES,
      spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
      spa::PqlArgument(spa::LITERAL_STRING, "x", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "y");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcUnderscoreExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "y");
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
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
    spa::SuchThatClause clause(spa::MODIFIES,
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
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "y");
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
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
    spa::SuchThatClause clause(spa::MODIFIES,
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
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }),
                               spa::PqlArgument(spa::LITERAL_STRING, "y", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("20", spa::StatementType::CALL);
    pkbManager->addCallsProc(20, "A");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "x");
    pkbManager->addRelationship(spa::MODIFIES, "11", "y");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
    pkbManager->addRelationship(spa::MODIFIES, "20", "x");
    pkbManager->addRelationship(spa::MODIFIES, "20", "y");
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
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }),
                               spa::PqlArgument(spa::LITERAL_STRING, "y", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("20", spa::StatementType::CALL);
    pkbManager->addCallsProc(20, "A");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "x");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
    pkbManager->addRelationship(spa::MODIFIES, "20", "x");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestCallUnderscoreExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("20", spa::StatementType::CALL);
    pkbManager->addStatementType("16", spa::StatementType::CALL);
    pkbManager->addCallsProc(20, "A");
    pkbManager->addCallsProc(16, "C");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "y");
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "16", "w");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
    pkbManager->addRelationship(spa::MODIFIES, "20", "x");
    pkbManager->addRelationship(spa::MODIFIES, "20", "y");
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
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "y");
    pkbManager->addRelationship(spa::MODIFIES, "11", "x");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestCallVarExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("20", spa::StatementType::CALL);
    pkbManager->addCallsProc(20, "A");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "x");
    pkbManager->addRelationship(spa::MODIFIES, "11", "y");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
    pkbManager->addRelationship(spa::MODIFIES, "20", "x");
    pkbManager->addRelationship(spa::MODIFIES, "20", "y");
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
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "x");
    pkbManager->addRelationship(spa::MODIFIES, "11", "y");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcNameVarNameExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::LITERAL_STRING, "B", {}),
                               spa::PqlArgument(spa::LITERAL_STRING, "z", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "x");
    pkbManager->addRelationship(spa::MODIFIES, "11", "y");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestProcNameVarNameNotExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::LITERAL_STRING, "B", {}),
                               spa::PqlArgument(spa::LITERAL_STRING, "x", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "x");
    pkbManager->addRelationship(spa::MODIFIES, "11", "y");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcNameUnderscoreExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::LITERAL_STRING, "C", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "x");
    pkbManager->addRelationship(spa::MODIFIES, "11", "y");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestProcNameUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::LITERAL_STRING, "D", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "x");
    pkbManager->addRelationship(spa::MODIFIES, "11", "y");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcNameVarExists) {
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::LITERAL_STRING, "C", {}),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "x");
    pkbManager->addRelationship(spa::MODIFIES, "11", "y");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
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
    spa::SuchThatClause clause(spa::MODIFIES,
                               spa::PqlArgument(spa::LITERAL_STRING, "D", {}),
                               spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "x");
    pkbManager->addRelationship(spa::MODIFIES_P, "A", "y");
    pkbManager->addRelationship(spa::MODIFIES_P, "B", "z");
    pkbManager->addRelationship(spa::MODIFIES_P, "C", "w");
    pkbManager->addRelationship(spa::MODIFIES, "10", "x");
    pkbManager->addRelationship(spa::MODIFIES, "11", "y");
    pkbManager->addRelationship(spa::MODIFIES, "15", "z");
    pkbManager->addRelationship(spa::MODIFIES, "19", "w");
    pkbManager->addRelationship(spa::MODIFIES, "20", "x");
    pkbManager->addRelationship(spa::MODIFIES, "20", "y");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }
  };
}  // namespace IntegrationTesting

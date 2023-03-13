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
  TEST_CLASS(TestFollowsStarEvaluator) {
public:
  TEST_METHOD(TestLineLineExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::LINE_NO, "5", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestLineLineNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::LINE_NO, "8", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineStatementExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 2);
    Assert::IsTrue(rows[0][1].getInteger() == 3);
    Assert::IsTrue(rows[1][0].getInteger() == 2);
    Assert::IsTrue(rows[1][1].getInteger() == 4);
  }

  TEST_METHOD(TestLineReadExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::READ);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 2);
    Assert::IsTrue(rows[0][1].getInteger() == 3);
    Assert::IsTrue(rows[1][0].getInteger() == 2);
    Assert::IsTrue(rows[1][1].getInteger() == 5);
  }

  TEST_METHOD(TestLineStatementNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineAssignNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::READ);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStatementLineExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::LINE_NO, "5", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 3);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 2);
    Assert::IsTrue(rows[0][1].getInteger() == 5);
    Assert::IsTrue(rows[1][0].getInteger() == 3);
    Assert::IsTrue(rows[1][1].getInteger() == 5);
    Assert::IsTrue(rows[2][0].getInteger() == 4);
    Assert::IsTrue(rows[2][1].getInteger() == 5);
  }

  TEST_METHOD(TestPrintLineExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::LINE_NO, "5", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::PRINT);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 2);
    Assert::IsTrue(rows[0][1].getInteger() == 5);
    Assert::IsTrue(rows[1][0].getInteger() == 4);
    Assert::IsTrue(rows[1][1].getInteger() == 5);
  }

  TEST_METHOD(TestStatementLineNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::LINE_NO, "7", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestWhileLineNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::LINE_NO, "5", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::WHILE);
    pkbManager->addStatementType("6", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineUnderscoreExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestLineUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "7", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreLineExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::LINE_NO, "7", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "7");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestUnderscoreLineNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::LINE_NO, "2", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStatementStatementExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s1", { spa::STMT }),
                               spa::PqlArgument(spa::SYNONYM, "s2", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 6);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 2);
    Assert::IsTrue(rows[0][1].getInteger() == 3);
    Assert::IsTrue(rows[1][0].getInteger() == 2);
    Assert::IsTrue(rows[1][1].getInteger() == 4);
    Assert::IsTrue(rows[2][0].getInteger() == 2);
    Assert::IsTrue(rows[2][1].getInteger() == 5);
    Assert::IsTrue(rows[3][0].getInteger() == 3);
    Assert::IsTrue(rows[3][1].getInteger() == 4);
    Assert::IsTrue(rows[4][0].getInteger() == 3);
    Assert::IsTrue(rows[4][1].getInteger() == 5);
    Assert::IsTrue(rows[5][0].getInteger() == 4);
    Assert::IsTrue(rows[5][1].getInteger() == 5);
  }

  TEST_METHOD(TestStatementAssignExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 3);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 2);
    Assert::IsTrue(rows[0][1].getInteger() == 5);
    Assert::IsTrue(rows[1][0].getInteger() == 3);
    Assert::IsTrue(rows[1][1].getInteger() == 5);
    Assert::IsTrue(rows[2][0].getInteger() == 4);
    Assert::IsTrue(rows[2][1].getInteger() == 5);
  }

  TEST_METHOD(TestStatementStatementNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s1", { spa::STMT }),
                               spa::PqlArgument(spa::SYNONYM, "s2", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStatementWhileNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::IF);
    pkbManager->addStatementType("7", spa::StatementType::IF);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStatementIfNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestReadStatementExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::READ);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 5);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 2);
    Assert::IsTrue(rows[0][1].getInteger() == 3);
    Assert::IsTrue(rows[1][0].getInteger() == 2);
    Assert::IsTrue(rows[1][1].getInteger() == 4);
    Assert::IsTrue(rows[2][0].getInteger() == 2);
    Assert::IsTrue(rows[2][1].getInteger() == 5);
    Assert::IsTrue(rows[3][0].getInteger() == 3);
    Assert::IsTrue(rows[3][1].getInteger() == 4);
    Assert::IsTrue(rows[4][0].getInteger() == 3);
    Assert::IsTrue(rows[4][1].getInteger() == 5);
  }

  TEST_METHOD(TestIfStatementNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestAssignAssignExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a1", { spa::ASSIGN }),
                               spa::PqlArgument(spa::SYNONYM, "a2", { spa::ASSIGN }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 3);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 2);
    Assert::IsTrue(rows[0][1].getInteger() == 4);
    Assert::IsTrue(rows[1][0].getInteger() == 2);
    Assert::IsTrue(rows[1][1].getInteger() == 5);
    Assert::IsTrue(rows[2][0].getInteger() == 4);
    Assert::IsTrue(rows[2][1].getInteger() == 5);
  }

  TEST_METHOD(TestIfWhileNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::PRINT);
    pkbManager->addStatementType("4", spa::StatementType::IF);
    pkbManager->addStatementType("5", spa::StatementType::WHILE);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStatementUnderscoreExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::READ);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::WHILE);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 6);
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestPrintUnderscoreExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::READ);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestStatementUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::READ);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::IF);
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestAssignUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::READ);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreStatementExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 6);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 2);
    Assert::IsTrue(rows[0][1].getInteger() == 3);
    Assert::IsTrue(rows[1][0].getInteger() == 2);
    Assert::IsTrue(rows[1][1].getInteger() == 4);
    Assert::IsTrue(rows[2][0].getInteger() == 2);
    Assert::IsTrue(rows[2][1].getInteger() == 5);
    Assert::IsTrue(rows[3][0].getInteger() == 3);
    Assert::IsTrue(rows[3][1].getInteger() == 4);
    Assert::IsTrue(rows[4][0].getInteger() == 3);
    Assert::IsTrue(rows[4][1].getInteger() == 5);
    Assert::IsTrue(rows[5][0].getInteger() == 4);
    Assert::IsTrue(rows[5][1].getInteger() == 5);
  }

  TEST_METHOD(TestUnderscoreWhileExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::WHILE);
    pkbManager->addStatementType("7", spa::StatementType::WHILE);
    pkbManager->addStatementType("10", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "10");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 3);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 2);
    Assert::IsTrue(rows[0][1].getInteger() == 3);
    Assert::IsTrue(rows[1][0].getInteger() == 2);
    Assert::IsTrue(rows[1][1].getInteger() == 7);
    Assert::IsTrue(rows[2][0].getInteger() == 3);
    Assert::IsTrue(rows[2][1].getInteger() == 7);
  }

  TEST_METHOD(TestUnderscoreStatementNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreIfNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("5", spa::StatementType::READ);
    pkbManager->addStatementType("6", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("7", spa::StatementType::WHILE);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreUnderscoreExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("7", spa::StatementType::WHILE);
    pkbManager->addStatementType("10", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "10");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestUnderscoreUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::IF);
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }
  };
}  // namespace IntegrationTesting

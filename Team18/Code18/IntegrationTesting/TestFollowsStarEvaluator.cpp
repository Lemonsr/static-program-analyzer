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
                               spa::PqlArgument(spa::LINE_NO, "7", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
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
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
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
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 5);
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(5));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(6)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(7)) != columnVals.end());
  }

  TEST_METHOD(TestLineReadExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(7)) != columnVals.end());
  }

  TEST_METHOD(TestLineAssignExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestLinePrintExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(6)) != columnVals.end());
  }

  TEST_METHOD(TestLineWhileExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addStatementType("18", spa::StatementType::WHILE);
    pkbManager->addStatementType("30", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "18");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "30");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(8)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(18)) != columnVals.end());
  }

  TEST_METHOD(TestLineIfExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addStatementType("18", spa::StatementType::WHILE);
    pkbManager->addStatementType("30", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "18");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "30");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(15)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(30)) != columnVals.end());
  }

  TEST_METHOD(TestLineStatementNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineReadNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
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
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLinePrintNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::READ);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::READ);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineWhileNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::READ);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addStatementType("18", spa::StatementType::READ);
    pkbManager->addStatementType("30", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "18");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "30");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineIfNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::LINE_NO, "2", {}),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::READ);
    pkbManager->addStatementType("18", spa::StatementType::WHILE);
    pkbManager->addStatementType("30", spa::StatementType::READ);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "18");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "30");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStatementLineExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::LINE_NO, "7", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "7");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 5);
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(5));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(6)) != columnVals.end());
  }

  TEST_METHOD(TestReadLineExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::LINE_NO, "15", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(7)) != columnVals.end());
  }

  TEST_METHOD(TestAssignLineExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::LINE_NO, "15", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestPrintLineExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::LINE_NO, "15", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(6)) != columnVals.end());
  }

  TEST_METHOD(TestWhileLineExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::LINE_NO, "15", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("10", spa::StatementType::IF);
    pkbManager->addStatementType("12", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "10", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "10", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "12", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(8)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(12)) != columnVals.end());
  }

  TEST_METHOD(TestIfLineExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::LINE_NO, "15", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("10", spa::StatementType::IF);
    pkbManager->addStatementType("12", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "10", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "10", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "12", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(10)) != columnVals.end());
  }

  TEST_METHOD(TestStatementLineNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::LINE_NO, "7", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestReadLineNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::LINE_NO, "15", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestAssignLineNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::LINE_NO, "15", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::READ);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::READ);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestPrintLineNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::LINE_NO, "15", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::READ);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::READ);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestWhileLineNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::LINE_NO, "15", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("10", spa::StatementType::IF);
    pkbManager->addStatementType("12", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "10", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "10", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "12", "15");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestIfLineNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::LINE_NO, "15", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
    pkbManager->addStatementType("7", spa::StatementType::READ);
    pkbManager->addStatementType("8", spa::StatementType::WHILE);
    pkbManager->addStatementType("10", spa::StatementType::WHILE);
    pkbManager->addStatementType("12", spa::StatementType::WHILE);
    pkbManager->addStatementType("15", spa::StatementType::IF);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "8");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "7", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "10");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "8", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "10", "12");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "10", "15");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "12", "15");
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
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
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
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
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
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
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
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "4");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "4", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "6");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "5", "7");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "6", "7");
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
    auto columnVals = table.getColumn("s1");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    columnVals = table.getColumn("s2");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
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
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestStatementReadExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
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
    Assert::AreEqual(dim.second, 4);
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestStatementPrintExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
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
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestStatementWhileExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 5);
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestStatementIfExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
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

  TEST_METHOD(TestStatementAssignNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
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

  TEST_METHOD(TestStatementReadNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStatementPrintNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::IF);
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

  TEST_METHOD(TestStatementWhileNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::IF);
    pkbManager->addStatementType("5", spa::StatementType::IF);
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

  TEST_METHOD(TestStatementIfNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestAssignStatementExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
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
    Assert::AreEqual(dim.second, 3);
    auto columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
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
    auto columnVals = table.getColumn("a1");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    columnVals = table.getColumn("a2");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestAssignReadExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
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
    auto columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestAssignPrintExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
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
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestAssignWhileExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestAssignIfExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
  }

  TEST_METHOD(TestAssignStatementNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestAssignAssignNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a1", { spa::ASSIGN }),
                               spa::PqlArgument(spa::SYNONYM, "a2", { spa::ASSIGN }));
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

  TEST_METHOD(TestAssignReadNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestAssignPrintNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::IF);
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

  TEST_METHOD(TestAssignWhileNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::IF);
    pkbManager->addStatementType("5", spa::StatementType::IF);
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

  TEST_METHOD(TestAssignIfNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    auto columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestReadAssignExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }));
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
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestReadReadExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re1", { spa::READ }),
                               spa::PqlArgument(spa::SYNONYM, "re2", { spa::READ }));
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
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("re1");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    columnVals = table.getColumn("re2");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestReadPrintExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
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
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestReadWhileExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestReadIfExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::READ);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
  }

  TEST_METHOD(TestReadStatementNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::READ);
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestReadAssignNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
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

  TEST_METHOD(TestReadReadNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re1", { spa::READ }),
                               spa::PqlArgument(spa::SYNONYM, "re2", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::READ);
    pkbManager->addStatementType("3", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestReadPrintNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::IF);
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

  TEST_METHOD(TestReadWhileNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::IF);
    pkbManager->addStatementType("5", spa::StatementType::IF);
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

  TEST_METHOD(TestReadIfNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestPrintStatementExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
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
    Assert::AreEqual(dim.second, 4);
    auto columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestPrintAssignExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
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
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestPrintReadExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::PRINT);
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
    Assert::AreEqual(dim.second, 3);
    auto columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestPrintPrintExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn1", { spa::PRINT }),
                               spa::PqlArgument(spa::SYNONYM, "pn2", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::PRINT);
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
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("pn1");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("pn2");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestPrintWhileExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::PRINT);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestPrintIfExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::PRINT);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::IF);
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
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestPrintStatementNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::PRINT);
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestPrintAssignNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
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

  TEST_METHOD(TestPrintReadNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::READ);
    pkbManager->addStatementType("3", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestPrintPrintNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn1", { spa::PRINT }),
                               spa::PqlArgument(spa::SYNONYM, "pn2", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::PRINT);
    pkbManager->addStatementType("4", spa::StatementType::IF);
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

  TEST_METHOD(TestPrintWhileNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::PRINT);
    pkbManager->addStatementType("4", spa::StatementType::IF);
    pkbManager->addStatementType("5", spa::StatementType::IF);
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

  TEST_METHOD(TestPrintIfNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("3", spa::StatementType::PRINT);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestWhileStatementExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
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
    auto columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestWhileAssignExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestWhileReadExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
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
    auto columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestWhilePrintExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::WHILE);
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
    auto columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestWhileWhileExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w1", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "w2", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::PRINT);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("w1");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    columnVals = table.getColumn("w2");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestWhileIfExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::IF);
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
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestWhileStatementNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestWhileAssignNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }));
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

  TEST_METHOD(TestWhileReadNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::READ);
    pkbManager->addStatementType("3", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestWhilePrintNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::PRINT);
    pkbManager->addStatementType("4", spa::StatementType::IF);
    pkbManager->addStatementType("5", spa::StatementType::READ);
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

  TEST_METHOD(TestWhileWhileNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w1", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "w2", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::PRINT);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::IF);
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

  TEST_METHOD(TestWhileIfNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("3", spa::StatementType::PRINT);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestIfStatementExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::IF);
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
    auto columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestIfAssignExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::IF);
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
    auto columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestIfReadExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::IF);
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
    auto columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestIfPrintExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("3", spa::StatementType::IF);
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
    auto columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestIfWhileExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestIfIfExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i1", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "i2", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::IF);
    pkbManager->addStatementType("5", spa::StatementType::IF);
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
    auto columnVals = table.getColumn("i1");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    columnVals = table.getColumn("i2");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestIfStatementNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::IF);
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestIfAssignNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::WHILE);
    pkbManager->addStatementType("6", spa::StatementType::IF);
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

  TEST_METHOD(TestIfReadNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::READ);
    pkbManager->addStatementType("3", spa::StatementType::IF);
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
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestIfPrintNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::PRINT);
    pkbManager->addStatementType("4", spa::StatementType::IF);
    pkbManager->addStatementType("5", spa::StatementType::IF);
    pkbManager->addStatementType("6", spa::StatementType::PRINT);
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

  TEST_METHOD(TestIfWhileNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::PRINT);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::IF);
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

  TEST_METHOD(TestIfIfNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i1", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "i2", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("3", spa::StatementType::PRINT);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::WHILE);
    pkbManager->addStatementType("6", spa::StatementType::IF);
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

  TEST_METHOD(TestStatementUnderscoreExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::IF);
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
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestAssignUnderscoreExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
  }

  TEST_METHOD(TestReadUnderscoreExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::READ);
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
    auto columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestPrintUnderscoreExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::IF);
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

  TEST_METHOD(TestWhileUnderscoreExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 4);
    auto columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestIfUnderscoreExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    auto columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
  }

  TEST_METHOD(TestStatementUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
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
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
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
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestReadUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::IF);
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

  TEST_METHOD(TestPrintUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("5", spa::StatementType::PRINT);
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

  TEST_METHOD(TestWhileUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::READ);
    pkbManager->addStatementType("5", spa::StatementType::WHILE);
    pkbManager->addStatementType("6", spa::StatementType::READ);
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

  TEST_METHOD(TestIfUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::IF);
    pkbManager->addStatementType("6", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("7", spa::StatementType::ASSIGN);
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
    auto columnVals = table.getColumn("s");
    Assert::AreEqual(columnVals.size(), size_t(3));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestUnderscoreAssignExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
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
    auto columnVals = table.getColumn("a");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
  }

  TEST_METHOD(TestUnderscoreReadExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
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
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("re");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
  }

  TEST_METHOD(TestUnderscorePrintExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
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
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("pn");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestUnderscoreWhileExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::WHILE);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
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
    auto columnVals = table.getColumn("w");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());
  }

  TEST_METHOD(TestUnderscoreIfExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "i", { spa::IF }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::IF);
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
    Assert::AreEqual(dim.second, 4);
    auto columnVals = table.getColumn("i");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue(5)) != columnVals.end());
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

  TEST_METHOD(TestUnderscoreAssignNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
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

  TEST_METHOD(TestUnderscoreReadNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
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
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscorePrintNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "pn", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::PRINT);
    pkbManager->addStatementType("3", spa::StatementType::IF);
    pkbManager->addStatementType("4", spa::StatementType::PRINT);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "3");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "3", "5");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreWhileNotExists) {
    spa::SuchThatClause clause(spa::FOLLOWS_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("2", spa::StatementType::WHILE);
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::FOLLOWS_STAR, "2", "5");
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
    pkbManager->addStatementType("3", spa::StatementType::READ);
    pkbManager->addStatementType("4", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("5", spa::StatementType::WHILE);
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
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::IF);
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

#include <memory>

#include "stdafx.h"
#include "CppUnitTest.h"

#include "ParsedQuery.h"
#include "PKB.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace IntegrationTesting {
  TEST_CLASS(TestParentStarEvaluator) {
public:
  TEST_METHOD(TestLineLineExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::LINE_NO, "1", {}),
                               spa::PqlArgument(spa::LINE_NO, "2", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);

    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestLineLineNotExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::LINE_NO, "9", {}),
                               spa::PqlArgument(spa::LINE_NO, "10", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);

    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineStatementExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::LINE_NO, "4", {}),
                               spa::PqlArgument(spa::SYNONYM, "s", {spa::DesignEntityType::STMT}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);

    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 4);
    Assert::IsTrue(rows[0][1].getInteger() == 5);
    Assert::IsTrue(rows[1][0].getInteger() == 4);
    Assert::IsTrue(rows[1][1].getInteger() == 6);
  }

  TEST_METHOD(TestLineStatementNotExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::LINE_NO, "9", {}),
                               spa::PqlArgument(spa::SYNONYM, "s", {spa::DesignEntityType::STMT}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);

    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStatementLineExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", {spa::DesignEntityType::STMT}),
                               spa::PqlArgument(spa::LINE_NO, "2", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);

    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 1);
    Assert::IsTrue(rows[0][1].getInteger() == 2);
  }

  TEST_METHOD(TestStatementLineNotExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", {spa::DesignEntityType::STMT}),
                               spa::PqlArgument(spa::LINE_NO, "9", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);

    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineUnderscoreExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::LINE_NO, "7", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);

    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestLineUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::LINE_NO, "9", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);

    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreLineExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::LINE_NO, "3", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);

    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestUnderscoreLineNotExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::LINE_NO, "9", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);

    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStatementStatementExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s1", {spa::DesignEntityType::STMT}),
                               spa::PqlArgument(spa::SYNONYM, "s2", {spa::DesignEntityType::STMT}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 6);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 1);
    Assert::IsTrue(rows[0][1].getInteger() == 2);
    Assert::IsTrue(rows[1][0].getInteger() == 1);
    Assert::IsTrue(rows[1][1].getInteger() == 3);
    Assert::IsTrue(rows[2][0].getInteger() == 2);
    Assert::IsTrue(rows[2][1].getInteger() == 3);
    Assert::IsTrue(rows[3][0].getInteger() == 4);
    Assert::IsTrue(rows[3][1].getInteger() == 5);
    Assert::IsTrue(rows[4][0].getInteger() == 4);
    Assert::IsTrue(rows[4][1].getInteger() == 6);
    Assert::IsTrue(rows[5][0].getInteger() == 7);
    Assert::IsTrue(rows[5][1].getInteger() == 8);
  }

  TEST_METHOD(TestStatementStatementNotExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s1", {spa::DesignEntityType::ASSIGN}),
                               spa::PqlArgument(spa::SYNONYM, "s2", {spa::DesignEntityType::STMT}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");
    pkbManager->addStatementType("1", spa::StatementType::IF);
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::CALL);
    pkbManager->addStatementType("6", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("7", spa::StatementType::IF);
    pkbManager->addStatementType("8", spa::StatementType::PRINT);

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStatementUnderscoreExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", {spa::DesignEntityType::STMT}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 6);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 1);
    Assert::IsTrue(rows[0][1].getInteger() == 2);
    Assert::IsTrue(rows[1][0].getInteger() == 1);
    Assert::IsTrue(rows[1][1].getInteger() == 3);
    Assert::IsTrue(rows[2][0].getInteger() == 2);
    Assert::IsTrue(rows[2][1].getInteger() == 3);
    Assert::IsTrue(rows[3][0].getInteger() == 4);
    Assert::IsTrue(rows[3][1].getInteger() == 5);
    Assert::IsTrue(rows[4][0].getInteger() == 4);
    Assert::IsTrue(rows[4][1].getInteger() == 6);
    Assert::IsTrue(rows[5][0].getInteger() == 7);
    Assert::IsTrue(rows[5][1].getInteger() == 8);
  }

  TEST_METHOD(TestStatementUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s", {spa::DesignEntityType::ASSIGN}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");
    pkbManager->addStatementType("1", spa::StatementType::IF);
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::CALL);
    pkbManager->addStatementType("6", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("7", spa::StatementType::IF);
    pkbManager->addStatementType("8", spa::StatementType::PRINT);

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreStatementExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::DesignEntityType::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 6);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 1);
    Assert::IsTrue(rows[0][1].getInteger() == 2);
    Assert::IsTrue(rows[1][0].getInteger() == 1);
    Assert::IsTrue(rows[1][1].getInteger() == 3);
    Assert::IsTrue(rows[2][0].getInteger() == 2);
    Assert::IsTrue(rows[2][1].getInteger() == 3);
    Assert::IsTrue(rows[3][0].getInteger() == 4);
    Assert::IsTrue(rows[3][1].getInteger() == 5);
    Assert::IsTrue(rows[4][0].getInteger() == 4);
    Assert::IsTrue(rows[4][1].getInteger() == 6);
    Assert::IsTrue(rows[5][0].getInteger() == 7);
    Assert::IsTrue(rows[5][1].getInteger() == 8);
  }

  TEST_METHOD(TestUnderscoreStatementNotExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::DesignEntityType::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");
    pkbManager->addStatementType("1", spa::StatementType::IF);
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::CALL);
    pkbManager->addStatementType("6", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("7", spa::StatementType::IF);
    pkbManager->addStatementType("8", spa::StatementType::PRINT);

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreUnderscoreExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestUnderscoreUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestWhileParent) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s1", {spa::DesignEntityType::WHILE}),
                               spa::PqlArgument(spa::SYNONYM, "s2", {spa::DesignEntityType::STMT}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");
    pkbManager->addStatementType("1", spa::StatementType::IF);
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::CALL);
    pkbManager->addStatementType("6", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("7", spa::StatementType::IF);
    pkbManager->addStatementType("8", spa::StatementType::PRINT);

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 4);
    Assert::IsTrue(rows[0][1].getInteger() == 5);
    Assert::IsTrue(rows[1][0].getInteger() == 4);
    Assert::IsTrue(rows[1][1].getInteger() == 6);
  }

  TEST_METHOD(TestWhileAssign) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s1", {spa::DesignEntityType::WHILE}),
                               spa::PqlArgument(spa::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");
    pkbManager->addStatementType("1", spa::StatementType::IF);
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::CALL);
    pkbManager->addStatementType("6", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("7", spa::StatementType::IF);
    pkbManager->addStatementType("8", spa::StatementType::PRINT);

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);

    auto columnVals = table.getColumn("s1");
    Assert::IsTrue(columnVals.find(spa::QpsValue(4)) != columnVals.end());

    columnVals = table.getColumn("a");
    Assert::IsTrue(columnVals.find(spa::QpsValue(6)) != columnVals.end());
  }

  TEST_METHOD(TestIfParent) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s1", {spa::DesignEntityType::IF}),
                               spa::PqlArgument(spa::SYNONYM, "s2", {spa::DesignEntityType::STMT}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");
    pkbManager->addStatementType("1", spa::StatementType::IF);
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::CALL);
    pkbManager->addStatementType("6", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("7", spa::StatementType::IF);
    pkbManager->addStatementType("8", spa::StatementType::PRINT);

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 4);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 1);
    Assert::IsTrue(rows[0][1].getInteger() == 2);
    Assert::IsTrue(rows[1][0].getInteger() == 1);
    Assert::IsTrue(rows[1][1].getInteger() == 3);
    Assert::IsTrue(rows[2][0].getInteger() == 2);
    Assert::IsTrue(rows[2][1].getInteger() == 3);
    Assert::IsTrue(rows[3][0].getInteger() == 7);
    Assert::IsTrue(rows[3][1].getInteger() == 8);
  }

  TEST_METHOD(TestIfPrint) {
    spa::SuchThatClause clause(spa::PARENT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "s1", {spa::DesignEntityType::IF}),
                               spa::PqlArgument(spa::SYNONYM, "p", {spa::DesignEntityType::PRINT}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "2");
    pkbManager->addRelationship(spa::PARENT_STAR, "1", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "2", "3");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "5");
    pkbManager->addRelationship(spa::PARENT_STAR, "4", "6");
    pkbManager->addRelationship(spa::PARENT_STAR, "7", "8");
    pkbManager->addStatementType("1", spa::StatementType::IF);
    pkbManager->addStatementType("2", spa::StatementType::IF);
    pkbManager->addStatementType("3", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("4", spa::StatementType::WHILE);
    pkbManager->addStatementType("5", spa::StatementType::CALL);
    pkbManager->addStatementType("6", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("7", spa::StatementType::IF);
    pkbManager->addStatementType("8", spa::StatementType::PRINT);

    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);

    auto columnVals = table.getColumn("s1");
    Assert::IsTrue(columnVals.find(spa::QpsValue(7)) != columnVals.end());

    columnVals = table.getColumn("p");
    Assert::IsTrue(columnVals.find(spa::QpsValue(8)) != columnVals.end());
  }
  };
}  // namespace IntegrationTesting

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
  TEST_CLASS(TestNextStarEvaluator) {
public:
  TEST_METHOD(TestLineLineExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::LINE_NO, "10", {}),
                               spa::PqlArgument(spa::LINE_NO, "11", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestLineLineNotExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::LINE_NO, "10", {}),
                               spa::PqlArgument(spa::LINE_NO, "11", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::NEXT_STAR, "12", "13");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineStatementExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::LINE_NO, "10", {}),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 10);
    Assert::IsTrue(rows[0][1].getInteger() == 11);
  }

  TEST_METHOD(TestLineStatementNotExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::LINE_NO, "10", {}),
                               spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::NEXT_STAR, "5", "6");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStatementLineExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::LINE_NO, "11", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("10", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("11", spa::StatementType::PRINT);
    pkbManager->addStatementType("12", spa::StatementType::PRINT);
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "12");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 10);
    Assert::IsTrue(rows[0][1].getInteger() == 11);

    clause = spa::SuchThatClause(spa::NEXT_STAR,
                                 spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                                 spa::PqlArgument(spa::LINE_NO, "12", {}));
    evaluator = clause.getEvaluator();
    table = evaluator->evaluate(*pkbManager);
    dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);

    rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 10);
    Assert::IsTrue(rows[0][1].getInteger() == 12);
    Assert::IsTrue(rows[1][0].getInteger() == 11);
    Assert::IsTrue(rows[1][1].getInteger() == 12);
  }

  TEST_METHOD(TestStatementLineNotExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                               spa::PqlArgument(spa::LINE_NO, "11", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("10", spa::StatementType::PRINT);
    pkbManager->addStatementType("11", spa::StatementType::PRINT);
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestLineUnderscoreExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::LINE_NO, "10", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestLineUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::LINE_NO, "10", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::NEXT_STAR, "5", "6");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreLineExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::LINE_NO, "11", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestUnderscoreLineNotExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::LINE_NO, "11", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::NEXT_STAR, "1", "2");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStatementStatementExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::SYNONYM, "pr", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("10", spa::StatementType::READ);
    pkbManager->addStatementType("11", spa::StatementType::PRINT);
    pkbManager->addStatementType("12", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("13", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "12", "13");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 10);
    Assert::IsTrue(rows[0][1].getInteger() == 11);

    clause = spa::SuchThatClause(spa::NEXT_STAR,
                                 spa::PqlArgument(spa::SYNONYM, "s1", { spa::STMT }),
                                 spa::PqlArgument(spa::SYNONYM, "s2", { spa::STMT }));
    evaluator = clause.getEvaluator();
    table = evaluator->evaluate(*pkbManager);
    dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 6);

    rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 10);
    Assert::IsTrue(rows[0][1].getInteger() == 11);
    Assert::IsTrue(rows[1][0].getInteger() == 10);
    Assert::IsTrue(rows[1][1].getInteger() == 12);
    Assert::IsTrue(rows[2][0].getInteger() == 10);
    Assert::IsTrue(rows[2][1].getInteger() == 13);
    Assert::IsTrue(rows[3][0].getInteger() == 11);
    Assert::IsTrue(rows[3][1].getInteger() == 12);
    Assert::IsTrue(rows[4][0].getInteger() == 11);
    Assert::IsTrue(rows[4][1].getInteger() == 13);
    Assert::IsTrue(rows[5][0].getInteger() == 12);
    Assert::IsTrue(rows[5][1].getInteger() == 13);
  }

  TEST_METHOD(TestStatementStatementNotExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a1", { spa::ASSIGN }),
                               spa::PqlArgument(spa::SYNONYM, "a2", { spa::ASSIGN }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("10", spa::StatementType::PRINT);
    pkbManager->addStatementType("11", spa::StatementType::PRINT);
    pkbManager->addStatementType("12", spa::StatementType::PRINT);
    pkbManager->addStatementType("13", spa::StatementType::PRINT);
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "12", "13");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestStatementUnderscoreExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "a1", { spa::ASSIGN }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("10", spa::StatementType::PRINT);
    pkbManager->addStatementType("11", spa::StatementType::READ);
    pkbManager->addStatementType("12", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("13", spa::StatementType::READ);
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "12", "13");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 12);
    Assert::IsTrue(rows[0][1].getInteger() == 13);

    clause = spa::SuchThatClause(spa::NEXT_STAR,
                                 spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                                 spa::PqlArgument(spa::WILDCARD, "_", {}));
    evaluator = clause.getEvaluator();
    table = evaluator->evaluate(*pkbManager);
    dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 6);

    rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 10);
    Assert::IsTrue(rows[0][1].getInteger() == 11);
    Assert::IsTrue(rows[1][0].getInteger() == 10);
    Assert::IsTrue(rows[1][1].getInteger() == 12);
    Assert::IsTrue(rows[2][0].getInteger() == 10);
    Assert::IsTrue(rows[2][1].getInteger() == 13);
    Assert::IsTrue(rows[3][0].getInteger() == 11);
    Assert::IsTrue(rows[3][1].getInteger() == 12);
    Assert::IsTrue(rows[4][0].getInteger() == 11);
    Assert::IsTrue(rows[4][1].getInteger() == 13);
    Assert::IsTrue(rows[5][0].getInteger() == 12);
    Assert::IsTrue(rows[5][1].getInteger() == 13);
  }

  TEST_METHOD(TestStatementUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("10", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("11", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("12", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("13", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "12", "13");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreStatementExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("10", spa::StatementType::READ);
    pkbManager->addStatementType("11", spa::StatementType::PRINT);
    pkbManager->addStatementType("12", spa::StatementType::READ);
    pkbManager->addStatementType("13", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "12", "13");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 10);
    Assert::IsTrue(rows[0][1].getInteger() == 12);
    Assert::IsTrue(rows[1][0].getInteger() == 11);
    Assert::IsTrue(rows[1][1].getInteger() == 12);

    clause = spa::SuchThatClause(spa::NEXT_STAR,
                                 spa::PqlArgument(spa::WILDCARD, "_", {}),
                                 spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }));
    evaluator = clause.getEvaluator();
    table = evaluator->evaluate(*pkbManager);
    dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 6);

    rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 10);
    Assert::IsTrue(rows[0][1].getInteger() == 11);
    Assert::IsTrue(rows[1][0].getInteger() == 10);
    Assert::IsTrue(rows[1][1].getInteger() == 12);
    Assert::IsTrue(rows[2][0].getInteger() == 10);
    Assert::IsTrue(rows[2][1].getInteger() == 13);
    Assert::IsTrue(rows[3][0].getInteger() == 11);
    Assert::IsTrue(rows[3][1].getInteger() == 12);
    Assert::IsTrue(rows[4][0].getInteger() == 11);
    Assert::IsTrue(rows[4][1].getInteger() == 13);
    Assert::IsTrue(rows[5][0].getInteger() == 12);
    Assert::IsTrue(rows[5][1].getInteger() == 13);
  }

  TEST_METHOD(TestUnderscoreStatementNotExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("10", spa::StatementType::READ);
    pkbManager->addStatementType("11", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("12", spa::StatementType::CALL);
    pkbManager->addStatementType("13", spa::StatementType::PRINT);
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "12", "13");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreUnderscoreExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("10", spa::StatementType::READ);
    pkbManager->addStatementType("11", spa::StatementType::ASSIGN);
    pkbManager->addStatementType("12", spa::StatementType::CALL);
    pkbManager->addStatementType("13", spa::StatementType::PRINT);
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "12", "13");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestUnderscoreUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestIfStatement) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "if", { spa::IF }),
                               spa::PqlArgument(spa::SYNONYM, "pr", { spa::PRINT }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("10", spa::StatementType::IF);
    pkbManager->addStatementType("11", spa::StatementType::PRINT);
    pkbManager->addStatementType("12", spa::StatementType::PRINT);
    pkbManager->addStatementType("13", spa::StatementType::ASSIGN);
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "13");
    pkbManager->addRelationship(spa::NEXT_STAR, "12", "13");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 10);
    Assert::IsTrue(rows[0][1].getInteger() == 11);
    Assert::IsTrue(rows[1][0].getInteger() == 10);
    Assert::IsTrue(rows[1][1].getInteger() == 12);
  }

  TEST_METHOD(TestWhileStatement) {
    spa::SuchThatClause clause(spa::NEXT_STAR,
                               spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                               spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addStatementType("10", spa::StatementType::WHILE);
    pkbManager->addStatementType("11", spa::StatementType::READ);
    pkbManager->addStatementType("12", spa::StatementType::READ);
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "11");
    pkbManager->addRelationship(spa::NEXT_STAR, "10", "12");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "10");
    pkbManager->addRelationship(spa::NEXT_STAR, "11", "12");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);

    std::vector<spa::QpsResultRow> rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 10);
    Assert::IsTrue(rows[0][1].getInteger() == 11);
    Assert::IsTrue(rows[1][0].getInteger() == 10);
    Assert::IsTrue(rows[1][1].getInteger() == 12);

    clause = spa::SuchThatClause(spa::NEXT_STAR,
                                 spa::PqlArgument(spa::SYNONYM, "re", { spa::READ }),
                                 spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }));
    evaluator = clause.getEvaluator();
    table = evaluator->evaluate(*pkbManager);
    dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);

    rows = table.getRows();
    Assert::IsTrue(rows[0][0].getInteger() == 11);
    Assert::IsTrue(rows[0][1].getInteger() == 10);
  }
  };
}  // namespace IntegrationTesting

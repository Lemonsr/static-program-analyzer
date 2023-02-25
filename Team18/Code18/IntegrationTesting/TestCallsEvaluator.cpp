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
  TEST_CLASS(TestCallsEvaluator) {
public:
  TEST_METHOD(TestNameNameExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::LITERAL_STRING, "first", {}),
                               spa::PqlArgument(spa::LITERAL_STRING, "second", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "first", "second");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestNameNameNotExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::LITERAL_STRING, "first", {}),
                               spa::PqlArgument(spa::LITERAL_STRING, "second", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "first", "third");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcNameUnderscoreExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::LITERAL_STRING, "first", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "first", "second");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestProcNameUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::LITERAL_STRING, "first", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "second", "first");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcNameProcExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::LITERAL_STRING, "first", {}),
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "first", "second");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("p");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue("second")) != columnVals.end());
  }

  TEST_METHOD(TestProcNameProcNotExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::LITERAL_STRING, "first", {}),
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "second", "first");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreProcNameExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::LITERAL_STRING, "second", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "first", "second");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }

  TEST_METHOD(TestUnderscoreProcNameNotExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::LITERAL_STRING, "second", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "first", "third");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreUnderscoreExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "first", "second");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
  }


  TEST_METHOD(TestUnderscoreUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestUnderscoreProcExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "first", "second");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("p");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue("second")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("third")) != columnVals.end());
  }

  TEST_METHOD(TestUnderscoreProcNotExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::WILDCARD, "_", {}),
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcProcNameExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::LITERAL_STRING, "second", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "first", "second");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 1);
    auto columnVals = table.getColumn("p");
    Assert::AreEqual(columnVals.size(), size_t(1));
    Assert::IsTrue(columnVals.find(spa::QpsValue("first")) != columnVals.end());
  }

  TEST_METHOD(TestProcProcNameNotExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::LITERAL_STRING, "second", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "first", "third");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcUnderscoreExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "first", "second");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("p");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue("first")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("second")) != columnVals.end());
  }

  TEST_METHOD(TestProcUnderscoreNotExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::WILDCARD, "_", {}));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }

  TEST_METHOD(TestProcProcExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::SYNONYM, "p1", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::SYNONYM, "p2", { spa::PROCEDURE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    pkbManager->addRelationship(spa::CALLS, "first", "second");
    pkbManager->addRelationship(spa::CALLS, "second", "third");
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 2);
    auto columnVals = table.getColumn("p1");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue("first")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("second")) != columnVals.end());
    columnVals = table.getColumn("p2");
    Assert::AreEqual(columnVals.size(), size_t(2));
    Assert::IsTrue(columnVals.find(spa::QpsValue("second")) != columnVals.end());
    Assert::IsTrue(columnVals.find(spa::QpsValue("third")) != columnVals.end());
  }

  TEST_METHOD(TestProcProcNotExists) {
    spa::SuchThatClause clause(spa::CALLS,
                               spa::PqlArgument(spa::SYNONYM, "p1", { spa::PROCEDURE }),
                               spa::PqlArgument(spa::SYNONYM, "p2", { spa::PROCEDURE }));
    std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
    std::unique_ptr<spa::QpsEvaluator> evaluator = clause.getEvaluator();
    spa::QpsResultTable table = evaluator->evaluate(*pkbManager);
    auto dim = table.getDimension();
    Assert::AreEqual(dim.first, 2);
    Assert::AreEqual(dim.second, 0);
  }
  };
}  // namespace IntegrationTesting

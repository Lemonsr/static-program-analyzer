#include <memory>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKBManager.h"
#include "PKB.h"
#include "QpsPreprocessor.h"
#include "QpsQueryEvaluator.h"
#include "PatternContainerEvaluator.h"
#include "PatternEvaluator.h"
#include "ParsedQuery.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace IntegrationTesting {
  TEST_CLASS(TestPatternContainerEvaluator) {
    TEST_METHOD(TestGetPatternIfUnderscore) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "1", "x"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "1", "y"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "2", "z"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "3", "z"));

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "ifs", { spa::DesignEntityType::IF });
      spa::PqlArgument firstArg(spa::ArgumentType::WILDCARD, "_", {});

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator =
        std::make_unique<spa::PatternContainerEvaluator>(spa::DesignEntityType::IF, synonym, firstArg);

      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::IsTrue(dim.first == 2);
      Assert::IsTrue(dim.second == 3);

      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[1][0].getInteger() == 1);
      Assert::IsTrue(rows[2][0].getInteger() == 2);

      Assert::IsTrue(rows[0][1].getString() == "x");
      Assert::IsTrue(rows[1][1].getString() == "y");
      Assert::IsTrue(rows[2][1].getString() == "z");
    }

    TEST_METHOD(TestGetPatternIfUnderscoreNotFound) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "ifs", { spa::DesignEntityType::IF });
      spa::PqlArgument firstArg(spa::ArgumentType::WILDCARD, "_", {});

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator =
        std::make_unique<spa::PatternContainerEvaluator>(spa::DesignEntityType::IF, synonym, firstArg);

      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::IsTrue(dim.first == 2);
      Assert::IsTrue(dim.second == 0);
    }

    TEST_METHOD(TestGetPatternIfVar) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "1", "x"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "1", "y"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "2", "z"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "3", "z"));

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "ifs", { spa::DesignEntityType::IF });
      spa::PqlArgument firstArg(spa::ArgumentType::SYNONYM, "v", { spa::DesignEntityType::VARIABLE });

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator =
        std::make_unique<spa::PatternContainerEvaluator>(spa::DesignEntityType::IF, synonym, firstArg);

      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::IsTrue(dim.first == 2);
      Assert::IsTrue(dim.second == 3);

      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[1][0].getInteger() == 1);
      Assert::IsTrue(rows[2][0].getInteger() == 2);

      Assert::IsTrue(rows[0][1].getString() == "x");
      Assert::IsTrue(rows[1][1].getString() == "y");
      Assert::IsTrue(rows[2][1].getString() == "z");
    }

    TEST_METHOD(TestGetPatternIfVarNotFound) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "ifs", { spa::DesignEntityType::IF });
      spa::PqlArgument firstArg(spa::ArgumentType::SYNONYM, "v", { spa::DesignEntityType::VARIABLE });

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator =
        std::make_unique<spa::PatternContainerEvaluator>(spa::DesignEntityType::IF, synonym, firstArg);

      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::IsTrue(dim.first == 2);
      Assert::IsTrue(dim.second == 0);
    }

    TEST_METHOD(TestGetPatternIfVarName) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "1", "x"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "1", "y"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "2", "z"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "3", "z"));

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "ifs", { spa::DesignEntityType::IF });
      spa::PqlArgument firstArg(spa::ArgumentType::VARIABLE_NAME, "x", {});

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator =
        std::make_unique<spa::PatternContainerEvaluator>(spa::DesignEntityType::IF, synonym, firstArg);

      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::IsTrue(dim.first == 2);
      Assert::IsTrue(dim.second == 1);

      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getString() == "x");
    }

    TEST_METHOD(TestGetPatternIfVarNameNotFound) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "1", "x"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "1", "y"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "2", "z"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "3", "z"));

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "ifs", { spa::DesignEntityType::IF });
      spa::PqlArgument firstArg(spa::ArgumentType::VARIABLE_NAME, "a", {});

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator =
        std::make_unique<spa::PatternContainerEvaluator>(spa::DesignEntityType::IF, synonym, firstArg);

      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::IsTrue(dim.first == 2);
      Assert::IsTrue(dim.second == 0);
    }
    
    TEST_METHOD(TestGetPatternWhileUnderscore) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "1", "x"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "1", "y"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "2", "z"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "3", "z"));

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "w", { spa::DesignEntityType::WHILE });
      spa::PqlArgument firstArg(spa::ArgumentType::WILDCARD, "_", {});

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator =
        std::make_unique<spa::PatternContainerEvaluator>(spa::DesignEntityType::WHILE, synonym, firstArg);

      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::IsTrue(dim.first == 2);
      Assert::IsTrue(dim.second == 3);

      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[1][0].getInteger() == 1);
      Assert::IsTrue(rows[2][0].getInteger() == 2);

      Assert::IsTrue(rows[0][1].getString() == "x");
      Assert::IsTrue(rows[1][1].getString() == "y");
      Assert::IsTrue(rows[2][1].getString() == "z");
    }

    TEST_METHOD(TestGetPatternWhileUnderscoreNotFound) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "w", { spa::DesignEntityType::WHILE });
      spa::PqlArgument firstArg(spa::ArgumentType::WILDCARD, "_", {});

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator =
        std::make_unique<spa::PatternContainerEvaluator>(spa::DesignEntityType::WHILE, synonym, firstArg);

      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::IsTrue(dim.first == 2);
      Assert::IsTrue(dim.second == 0);
    }

    TEST_METHOD(TestGetPatternWhileVar) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "1", "x"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "1", "y"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "2", "z"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "3", "z"));

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "w", { spa::DesignEntityType::WHILE });
      spa::PqlArgument firstArg(spa::ArgumentType::SYNONYM, "v", { spa::DesignEntityType::VARIABLE });

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator =
        std::make_unique<spa::PatternContainerEvaluator>(spa::DesignEntityType::WHILE, synonym, firstArg);

      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::IsTrue(dim.first == 2);
      Assert::IsTrue(dim.second == 3);

      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[1][0].getInteger() == 1);
      Assert::IsTrue(rows[2][0].getInteger() == 2);

      Assert::IsTrue(rows[0][1].getString() == "x");
      Assert::IsTrue(rows[1][1].getString() == "y");
      Assert::IsTrue(rows[2][1].getString() == "z");
    }

    TEST_METHOD(TestGetPatternWhileVarNotFound) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "w", { spa::DesignEntityType::WHILE });
      spa::PqlArgument firstArg(spa::ArgumentType::SYNONYM, "v", { spa::DesignEntityType::VARIABLE });

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator =
        std::make_unique<spa::PatternContainerEvaluator>(spa::DesignEntityType::WHILE, synonym, firstArg);

      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::IsTrue(dim.first == 2);
      Assert::IsTrue(dim.second == 0);
    }

    TEST_METHOD(TestGetPatternWhileVarName) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "1", "x"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "1", "y"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "2", "z"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "3", "z"));

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "w", { spa::DesignEntityType::WHILE });
      spa::PqlArgument firstArg(spa::ArgumentType::VARIABLE_NAME, "z", {});

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator =
        std::make_unique<spa::PatternContainerEvaluator>(spa::DesignEntityType::WHILE, synonym, firstArg);

      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::IsTrue(dim.first == 2);
      Assert::IsTrue(dim.second == 1);

      Assert::IsTrue(rows[0][0].getInteger() == 2);
      Assert::IsTrue(rows[0][1].getString() == "z");
    }

    TEST_METHOD(TestGetPatternWhileVarNameNotFound) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "1", "x"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "1", "y"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::WHILE, "2", "z"));
      Assert::IsTrue(pkbManager->addContainerPattern(spa::DesignEntityType::IF, "3", "z"));

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "w", { spa::DesignEntityType::WHILE });
      spa::PqlArgument firstArg(spa::ArgumentType::VARIABLE_NAME, "a", {});

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator =
        std::make_unique<spa::PatternContainerEvaluator>(spa::DesignEntityType::WHILE, synonym, firstArg);

      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::IsTrue(dim.first == 2);
      Assert::IsTrue(dim.second == 0);
    }
  };
}  // namespace IntegrationTesting

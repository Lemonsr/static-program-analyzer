#include <memory>
#include <optional>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKBManager.h"
#include "PKB.h"
#include "QpsPreprocessor.h"
#include "QpsQueryEvaluator.h"
#include "SimpleEvaluator.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace IntegrationTesting {
  TEST_CLASS(TestSimpleEvaluator) {
    TEST_METHOD(TestGetVars) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::VARIABLE, "x"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::VARIABLE, "y"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::VARIABLE, "z"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "v", spa::DesignEntityType::VARIABLE);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 3);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[1][0].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[2][0].getType() == spa::QpsValueType::STRING);

      Assert::IsTrue(rows[0][0].getString() == "x");
      Assert::IsTrue(rows[1][0].getString() == "y");
      Assert::IsTrue(rows[2][0].getString() == "z");
    }

    TEST_METHOD(TestGetProcs) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PROCEDURE, "func1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PROCEDURE, "func2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PROCEDURE, "func3"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "p", spa::DesignEntityType::PROCEDURE);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 3);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[1][0].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[2][0].getType() == spa::QpsValueType::STRING);

      Assert::IsTrue(rows[0][0].getString() == "func1");
      Assert::IsTrue(rows[1][0].getString() == "func2");
      Assert::IsTrue(rows[2][0].getString() == "func3");
    }

    TEST_METHOD(TestGetConsts) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::CONSTANT, "1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::CONSTANT, "2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::CONSTANT, "3"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "c", spa::DesignEntityType::CONSTANT);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 3);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[1][0].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[2][0].getType() == spa::QpsValueType::STRING);

      Assert::IsTrue(rows[0][0].getString() == "1");
      Assert::IsTrue(rows[1][0].getString() == "2");
      Assert::IsTrue(rows[2][0].getString() == "3");
    }

    TEST_METHOD(TestGetAllStatements) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::READ, "1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::ASSIGN, "2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PRINT, "3"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "s", spa::DesignEntityType::STMT);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 3);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[1][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[2][0].getType() == spa::QpsValueType::INTEGER);

      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[1][0].getInteger() == 2);
      Assert::IsTrue(rows[2][0].getInteger() == 3);
    }

    TEST_METHOD(TestGetReadStatements) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::READ, "1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::ASSIGN, "2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PRINT, "3"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "re", spa::DesignEntityType::READ);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][0].getInteger() == 1);
    }

    TEST_METHOD(TestGetAssignStatements) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::READ, "1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::ASSIGN, "2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PRINT, "3"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "a", spa::DesignEntityType::ASSIGN);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][0].getInteger() == 2);
    }

    TEST_METHOD(TestGetPrintStatements) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::READ, "1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::ASSIGN, "2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PRINT, "3"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "p", spa::DesignEntityType::PRINT);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][0].getInteger() == 3);
    }

    TEST_METHOD(TestGetIfStatements) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::IF, "1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::WHILE, "2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::CALL, "3"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "i", spa::DesignEntityType::IF);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][0].getInteger() == 1);
    }

    TEST_METHOD(TestGetWhileStatements) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::IF, "1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::WHILE, "2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::CALL, "3"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "w", spa::DesignEntityType::WHILE);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][0].getInteger() == 2);
    }

    TEST_METHOD(TestGetCallStatements) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::IF, "1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::WHILE, "2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::CALL, "3"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
"c", spa::DesignEntityType::CALL);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][0].getInteger() == 3);
    }
  };
}  // namespace IntegrationTesting

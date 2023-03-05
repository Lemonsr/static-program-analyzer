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

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 2);
      Assert::AreEqual(dim.second, 3);
      
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();
      Assert::IsTrue(rows[0][0].getString() == "x");
      Assert::IsTrue(rows[0][1].getString() == "x");
      Assert::IsTrue(rows[1][0].getString() == "y");
      Assert::IsTrue(rows[1][1].getString() == "y");
      Assert::IsTrue(rows[2][0].getString() == "z");
      Assert::IsTrue(rows[2][1].getString() == "z");
    }

    TEST_METHOD(TestGetProcs) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PROCEDURE, "func1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PROCEDURE, "func2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PROCEDURE, "func3"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "p", spa::DesignEntityType::PROCEDURE);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 2);
      Assert::AreEqual(dim.second, 3);

      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();
      Assert::IsTrue(rows[0][0].getString() == "func1");
      Assert::IsTrue(rows[0][1].getString() == "func1");
      Assert::IsTrue(rows[1][0].getString() == "func2");
      Assert::IsTrue(rows[1][1].getString() == "func2");
      Assert::IsTrue(rows[2][0].getString() == "func3");
      Assert::IsTrue(rows[2][1].getString() == "func3");
    }

    TEST_METHOD(TestGetConsts) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::CONSTANT, "1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::CONSTANT, "2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::CONSTANT, "3"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "c", spa::DesignEntityType::CONSTANT);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 2);
      Assert::AreEqual(dim.second, 3);

      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();
      Assert::IsTrue(rows[0][0].getString() == "1");
      Assert::IsTrue(rows[0][1].getString() == "1");
      Assert::IsTrue(rows[1][0].getString() == "2");
      Assert::IsTrue(rows[1][1].getString() == "2");
      Assert::IsTrue(rows[2][0].getString() == "3");
      Assert::IsTrue(rows[2][1].getString() == "3");
    }

    TEST_METHOD(TestGetAllStatements) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::READ, "1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::ASSIGN, "2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PRINT, "3"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "s", spa::DesignEntityType::STMT);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 2);
      Assert::AreEqual(dim.second, 3);

      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();
      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getInteger() == 1);
      Assert::IsTrue(rows[1][0].getInteger() == 2);
      Assert::IsTrue(rows[1][1].getInteger() == 2);
      Assert::IsTrue(rows[2][0].getInteger() == 3);
      Assert::IsTrue(rows[2][1].getInteger() == 3);
    }

    TEST_METHOD(TestGetReadStatements) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::READ, "1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::ASSIGN, "2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PRINT, "3"));
      Assert::IsTrue(pkbManager->addRelationship(spa::RelationshipType::MODIFIES, "1", "x"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "re", spa::DesignEntityType::READ);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 3);
      Assert::AreEqual(dim.second, 1);

      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();
      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getInteger() == 1);
      Assert::IsTrue(rows[0][2].getString() == "x");
    }

    TEST_METHOD(TestGetAssignStatements) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::READ, "1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::ASSIGN, "2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PRINT, "3"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "a", spa::DesignEntityType::ASSIGN);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 2);
      Assert::AreEqual(dim.second, 1);
      
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();
      Assert::IsTrue(rows[0][0].getInteger() == 2);
      Assert::IsTrue(rows[0][1].getInteger() == 2);
    }

    TEST_METHOD(TestGetPrintStatements) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::READ, "1"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::ASSIGN, "2"));
      Assert::IsTrue(pkbManager->addEntity(spa::DesignEntityType::PRINT, "3"));
      Assert::IsTrue(pkbManager->addRelationship(spa::RelationshipType::USES, "3", "x"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "pr", spa::DesignEntityType::PRINT);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 3);
      Assert::AreEqual(dim.second, 1);

      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();
      Assert::IsTrue(rows[0][0].getInteger() == 3);
      Assert::IsTrue(rows[0][1].getInteger() == 3);
      Assert::IsTrue(rows[0][2].getString() == "x");
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
      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getInteger() == 1);
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
      Assert::IsTrue(rows[0][0].getInteger() == 2);
      Assert::IsTrue(rows[0][1].getInteger() == 2);
    }

    TEST_METHOD(TestGetCallStatements) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addCallsProc(1, "A"));
      Assert::IsTrue(pkbManager->addCallsProc(2, "B"));
      Assert::IsTrue(pkbManager->addCallsProc(3, "C"));

      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = std::make_unique<spa::SimpleEvaluator>(
        "c", spa::DesignEntityType::CALL);
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 3);
      Assert::AreEqual(dim.second, 3);

      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();
      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getInteger() == 1);
      Assert::IsTrue(rows[0][2].getString() == "A");
      Assert::IsTrue(rows[1][0].getInteger() == 2);
      Assert::IsTrue(rows[1][1].getInteger() == 2);
      Assert::IsTrue(rows[1][2].getString() == "B");
      Assert::IsTrue(rows[2][0].getInteger() == 3);
      Assert::IsTrue(rows[2][1].getInteger() == 3);
      Assert::IsTrue(rows[2][2].getString() == "C");
    }
  };
}  // namespace IntegrationTesting

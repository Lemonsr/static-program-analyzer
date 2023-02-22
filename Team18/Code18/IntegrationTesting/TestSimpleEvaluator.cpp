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

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 2);
      Assert::AreEqual(dim.second, 3);

      auto columnVals = resultTable.getColumn("v");
      Assert::IsTrue(columnVals.find(spa::QpsValue("x")) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue("y")) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue("z")) != columnVals.end());

      columnVals = resultTable.getColumn("v.varName");
      Assert::IsTrue(columnVals.find(spa::QpsValue("x")) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue("y")) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue("z")) != columnVals.end());
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

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 2);
      Assert::AreEqual(dim.second, 3);

      auto columnVals = resultTable.getColumn("p");
      Assert::IsTrue(columnVals.find(spa::QpsValue("func1")) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue("func2")) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue("func3")) != columnVals.end());

      columnVals = resultTable.getColumn("p.procName");
      Assert::IsTrue(columnVals.find(spa::QpsValue("func1")) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue("func2")) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue("func3")) != columnVals.end());
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

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 2);
      Assert::AreEqual(dim.second, 3);

      auto columnVals = resultTable.getColumn("c");
      Assert::IsTrue(columnVals.find(spa::QpsValue("1")) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue("2")) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue("3")) != columnVals.end());

      columnVals = resultTable.getColumn("c.value");
      Assert::IsTrue(columnVals.find(spa::QpsValue("1")) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue("2")) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue("3")) != columnVals.end());
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

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 2);
      Assert::AreEqual(dim.second, 3);

      auto columnVals = resultTable.getColumn("s");
      Assert::IsTrue(columnVals.find(spa::QpsValue(1)) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());

      columnVals = resultTable.getColumn("s.stmt#");
      Assert::IsTrue(columnVals.find(spa::QpsValue(1)) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
      Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());
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
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 3);
      Assert::AreEqual(dim.second, 1);

      auto columnVals = resultTable.getColumn("re");
      Assert::IsTrue(columnVals.find(spa::QpsValue(1)) != columnVals.end());

      columnVals = resultTable.getColumn("re.stmt#");
      Assert::IsTrue(columnVals.find(spa::QpsValue(1)) != columnVals.end());

      columnVals = resultTable.getColumn("re.varName");
      Assert::IsTrue(columnVals.find(spa::QpsValue("x")) != columnVals.end());
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

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 2);
      Assert::AreEqual(dim.second, 1);

      auto columnVals = resultTable.getColumn("a");
      Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());

      columnVals = resultTable.getColumn("a.stmt#");
      Assert::IsTrue(columnVals.find(spa::QpsValue(2)) != columnVals.end());
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
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      auto dim = resultTable.getDimension();
      Assert::AreEqual(dim.first, 3);
      Assert::AreEqual(dim.second, 1);

      auto columnVals = resultTable.getColumn("pr");
      Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());

      columnVals = resultTable.getColumn("pr.stmt#");
      Assert::IsTrue(columnVals.find(spa::QpsValue(3)) != columnVals.end());

      columnVals = resultTable.getColumn("pr.varName");
      Assert::IsTrue(columnVals.find(spa::QpsValue("x")) != columnVals.end());
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

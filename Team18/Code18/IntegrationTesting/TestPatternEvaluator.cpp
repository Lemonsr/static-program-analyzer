#include <memory>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKBManager.h"
#include "PKB.h"
#include "QpsPreprocessor.h"
#include "QpsQueryEvaluator.h"
#include "PatternEvaluator.h"
#include "ParsedQuery.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace IntegrationTesting {
  TEST_CLASS(TestPatternEvaluator) {
    TEST_METHOD(TestGetAssignUnderscoreExact) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      std::string query = "assign a;\n Select a pattern a(_, \"x + 1\")";
      spa::QpsPreprocessor qpsPreprocessor;
      std::optional<spa::ParsedQuery> parsedQueryOpt = qpsPreprocessor.preprocess(query);
      Assert::IsTrue(parsedQueryOpt.has_value());

      std::optional<spa::PatternClause> patternClauseOpt = parsedQueryOpt.value().getPatternClause();
      Assert::IsTrue(patternClauseOpt.has_value());

      spa::PatternClause patternClause = patternClauseOpt.value();
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][1].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getString() == "x");
    }

    TEST_METHOD(TestGetAssignUnderscorePartial) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      std::string query = "assign a;\n Select a pattern a(_, _\"x\"_)";
      spa::QpsPreprocessor qpsPreprocessor;
      std::optional<spa::ParsedQuery> parsedQueryOpt = qpsPreprocessor.preprocess(query);
      Assert::IsTrue(parsedQueryOpt.has_value());

      std::optional<spa::PatternClause> patternClauseOpt = parsedQueryOpt.value().getPatternClause();
      Assert::IsTrue(patternClauseOpt.has_value());

      spa::PatternClause patternClause = patternClauseOpt.value();
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][1].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getString() == "x");
    }

    TEST_METHOD(TestGetAssignUnderscoreAny) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      std::string query = "assign a;\n Select a pattern a(_, _)";
      spa::QpsPreprocessor qpsPreprocessor;
      std::optional<spa::ParsedQuery> parsedQueryOpt = qpsPreprocessor.preprocess(query);
      Assert::IsTrue(parsedQueryOpt.has_value());

      std::optional<spa::PatternClause> patternClauseOpt = parsedQueryOpt.value().getPatternClause();
      Assert::IsTrue(patternClauseOpt.has_value());

      spa::PatternClause patternClause = patternClauseOpt.value();
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][1].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getString() == "x");
    }

    TEST_METHOD(TestGetAssignUnderscoreNotFound) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      std::string query = "assign a;\n Select a pattern a(_, \"x\")";
      spa::QpsPreprocessor qpsPreprocessor;
      std::optional<spa::ParsedQuery> parsedQueryOpt = qpsPreprocessor.preprocess(query);
      Assert::IsTrue(parsedQueryOpt.has_value());

      std::optional<spa::PatternClause> patternClauseOpt = parsedQueryOpt.value().getPatternClause();
      Assert::IsTrue(patternClauseOpt.has_value());

      spa::PatternClause patternClause = patternClauseOpt.value();
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 0);
    }

    TEST_METHOD(TestGetAssignVarExact) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      std::string query = "variable v;assign a;\n Select a pattern a(v, \"x + 1\")";
      spa::QpsPreprocessor qpsPreprocessor;
      std::optional<spa::ParsedQuery> parsedQueryOpt = qpsPreprocessor.preprocess(query);
      Assert::IsTrue(parsedQueryOpt.has_value());

      std::optional<spa::PatternClause> patternClauseOpt = parsedQueryOpt.value().getPatternClause();
      Assert::IsTrue(patternClauseOpt.has_value());

      spa::PatternClause patternClause = patternClauseOpt.value();
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][1].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getString() == "x");
    }

    TEST_METHOD(TestGetAssignVarPartial) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      std::string query = "variable v;assign a;\n Select a pattern a(v, _\"x\"_)";
      spa::QpsPreprocessor qpsPreprocessor;
      std::optional<spa::ParsedQuery> parsedQueryOpt = qpsPreprocessor.preprocess(query);
      Assert::IsTrue(parsedQueryOpt.has_value());

      std::optional<spa::PatternClause> patternClauseOpt = parsedQueryOpt.value().getPatternClause();
      Assert::IsTrue(patternClauseOpt.has_value());

      spa::PatternClause patternClause = patternClauseOpt.value();
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][1].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getString() == "x");
    }

    TEST_METHOD(TestGetAssignVarAny) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      std::string query = "variable v;assign a;\n Select a pattern a(v, _)";
      spa::QpsPreprocessor qpsPreprocessor;
      std::optional<spa::ParsedQuery> parsedQueryOpt = qpsPreprocessor.preprocess(query);
      Assert::IsTrue(parsedQueryOpt.has_value());

      std::optional<spa::PatternClause> patternClauseOpt = parsedQueryOpt.value().getPatternClause();
      Assert::IsTrue(patternClauseOpt.has_value());

      spa::PatternClause patternClause = patternClauseOpt.value();
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][1].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getString() == "x");
    }

    TEST_METHOD(TestGetAssignVarNotFound) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      std::string query = "variable v;assign a;\n Select a pattern a(v, \"x\")";
      spa::QpsPreprocessor qpsPreprocessor;
      std::optional<spa::ParsedQuery> parsedQueryOpt = qpsPreprocessor.preprocess(query);
      Assert::IsTrue(parsedQueryOpt.has_value());

      std::optional<spa::PatternClause> patternClauseOpt = parsedQueryOpt.value().getPatternClause();
      Assert::IsTrue(patternClauseOpt.has_value());

      spa::PatternClause patternClause = patternClauseOpt.value();
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 0);
    }

    TEST_METHOD(TestGetAssignVarNameExact) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      std::string query = "assign a;\n Select a pattern a(\"x\", \"x + 1\")";
      spa::QpsPreprocessor qpsPreprocessor;
      std::optional<spa::ParsedQuery> parsedQueryOpt = qpsPreprocessor.preprocess(query);
      Assert::IsTrue(parsedQueryOpt.has_value());

      std::optional<spa::PatternClause> patternClauseOpt = parsedQueryOpt.value().getPatternClause();
      Assert::IsTrue(patternClauseOpt.has_value());

      spa::PatternClause patternClause = patternClauseOpt.value();
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][1].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getString() == "x");
    }

    TEST_METHOD(TestGetAssignVarNamePartial) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      std::string query = "assign a;\n Select a pattern a(\"x\", _\"x\"_)";
      spa::QpsPreprocessor qpsPreprocessor;
      std::optional<spa::ParsedQuery> parsedQueryOpt = qpsPreprocessor.preprocess(query);
      Assert::IsTrue(parsedQueryOpt.has_value());

      std::optional<spa::PatternClause> patternClauseOpt = parsedQueryOpt.value().getPatternClause();
      Assert::IsTrue(patternClauseOpt.has_value());

      spa::PatternClause patternClause = patternClauseOpt.value();
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][1].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getString() == "x");
    }

    TEST_METHOD(TestGetAssignVarNameAny) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      std::string query = "assign a;\n Select a pattern a(\"x\", _)";
      spa::QpsPreprocessor qpsPreprocessor;
      std::optional<spa::ParsedQuery> parsedQueryOpt = qpsPreprocessor.preprocess(query);
      Assert::IsTrue(parsedQueryOpt.has_value());

      std::optional<spa::PatternClause> patternClauseOpt = parsedQueryOpt.value().getPatternClause();
      Assert::IsTrue(patternClauseOpt.has_value());

      spa::PatternClause patternClause = patternClauseOpt.value();
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 1);
      Assert::IsTrue(rows[0][0].getType() == spa::QpsValueType::INTEGER);
      Assert::IsTrue(rows[0][1].getType() == spa::QpsValueType::STRING);
      Assert::IsTrue(rows[0][0].getInteger() == 1);
      Assert::IsTrue(rows[0][1].getString() == "x");
    }

    TEST_METHOD(TestGetAssignVarNameNotFound) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      std::string query = "assign a;\n Select a pattern a(\"x\", \"x\")";
      spa::QpsPreprocessor qpsPreprocessor;
      std::optional<spa::ParsedQuery> parsedQueryOpt = qpsPreprocessor.preprocess(query);
      Assert::IsTrue(parsedQueryOpt.has_value());

      std::optional<spa::PatternClause> patternClauseOpt = parsedQueryOpt.value().getPatternClause();
      Assert::IsTrue(patternClauseOpt.has_value());

      spa::PatternClause patternClause = patternClauseOpt.value();
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 0);
    }
  };
}  // namespace IntegrationTesting

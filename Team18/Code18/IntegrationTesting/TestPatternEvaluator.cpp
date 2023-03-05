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

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN});
      spa::PqlArgument lhs(spa::ArgumentType::WILDCARD, "_", {});
      std::vector<spa::Token> patternTokens = {
        {spa::TokenType::TOKEN_NAME, "x"},
        {spa::TokenType::TOKEN_PLUS, "+"},
        {spa::TokenType::TOKEN_INTEGER, "1"},
      };
      spa::Pattern pattern(spa::PatternType::EXACT, patternTokens);

      spa::PatternClause patternClause(synonym, lhs, pattern, 2);
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

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "a", { spa::DesignEntityType::ASSIGN });
      spa::PqlArgument lhs(spa::ArgumentType::WILDCARD, "_", {});
      std::vector<spa::Token> patternTokens = {
        {spa::TokenType::TOKEN_NAME, "x"},
      };
      spa::Pattern pattern(spa::PatternType::PARTIAL, patternTokens);

      spa::PatternClause patternClause(synonym, lhs, pattern, 2);
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

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "a", { spa::DesignEntityType::ASSIGN });
      spa::PqlArgument lhs(spa::ArgumentType::WILDCARD, "_", {});
      std::vector<spa::Token> patternTokens = {
        {spa::TokenType::TOKEN_UNDERSCORE, "_"},
      };
      spa::Pattern pattern(spa::PatternType::ANY, patternTokens);

      spa::PatternClause patternClause(synonym, lhs, pattern, 2);
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

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "a", { spa::DesignEntityType::ASSIGN });
      spa::PqlArgument lhs(spa::ArgumentType::WILDCARD, "_", {});
      std::vector<spa::Token> patternTokens = {
        {spa::TokenType::TOKEN_NAME, "x"},
      };
      spa::Pattern pattern(spa::PatternType::EXACT, patternTokens);

      spa::PatternClause patternClause(synonym, lhs, pattern, 2);
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 0);
    }

    TEST_METHOD(TestGetAssignVarExact) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "a", { spa::DesignEntityType::ASSIGN });
      spa::PqlArgument lhs(spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE});
      std::vector<spa::Token> patternTokens = {
        {spa::TokenType::TOKEN_NAME, "x"},
        {spa::TokenType::TOKEN_PLUS, "+"},
        {spa::TokenType::TOKEN_INTEGER, "1"},
      };
      spa::Pattern pattern(spa::PatternType::EXACT, patternTokens);

      spa::PatternClause patternClause(synonym, lhs, pattern, 2);
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

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "a", { spa::DesignEntityType::ASSIGN });
      spa::PqlArgument lhs(spa::ArgumentType::SYNONYM, "v", { spa::DesignEntityType::VARIABLE });
      std::vector<spa::Token> patternTokens = {
        {spa::TokenType::TOKEN_NAME, "x"},
      };
      spa::Pattern pattern(spa::PatternType::PARTIAL, patternTokens);

      spa::PatternClause patternClause(synonym, lhs, pattern, 2);
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

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "a", { spa::DesignEntityType::ASSIGN });
      spa::PqlArgument lhs(spa::ArgumentType::SYNONYM, "v", { spa::DesignEntityType::VARIABLE });
      std::vector<spa::Token> patternTokens = {
        {spa::TokenType::TOKEN_UNDERSCORE, "_"},
      };
      spa::Pattern pattern(spa::PatternType::ANY, patternTokens);

      spa::PatternClause patternClause(synonym, lhs, pattern, 2);
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

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "a", { spa::DesignEntityType::ASSIGN });
      spa::PqlArgument lhs(spa::ArgumentType::SYNONYM, "v", { spa::DesignEntityType::VARIABLE });
      std::vector<spa::Token> patternTokens = {
        {spa::TokenType::TOKEN_NAME, "x"},
      };
      spa::Pattern pattern(spa::PatternType::EXACT, patternTokens);

      spa::PatternClause patternClause(synonym, lhs, pattern, 2);
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 0);
    }

    TEST_METHOD(TestGetAssignVarNameExact) {
      std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();
      Assert::IsTrue(pkbManager->addPattern("1", "x", "x 1 +"));

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "a", { spa::DesignEntityType::ASSIGN });
      spa::PqlArgument lhs(spa::ArgumentType::LITERAL_STRING, "x", {});
      std::vector<spa::Token> patternTokens = {
        {spa::TokenType::TOKEN_NAME, "x"},
        {spa::TokenType::TOKEN_PLUS, "+"},
        {spa::TokenType::TOKEN_INTEGER, "1"},
      };
      spa::Pattern pattern(spa::PatternType::EXACT, patternTokens);

      spa::PatternClause patternClause(synonym, lhs, pattern, 2);
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

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "a", { spa::DesignEntityType::ASSIGN });
      spa::PqlArgument lhs(spa::ArgumentType::LITERAL_STRING, "x", {});
      std::vector<spa::Token> patternTokens = {
        {spa::TokenType::TOKEN_NAME, "x"},
      };
      spa::Pattern pattern(spa::PatternType::PARTIAL, patternTokens);

      spa::PatternClause patternClause(synonym, lhs, pattern, 2);
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

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "a", { spa::DesignEntityType::ASSIGN });
      spa::PqlArgument lhs(spa::ArgumentType::LITERAL_STRING, "x", {});
      std::vector<spa::Token> patternTokens = {
        {spa::TokenType::TOKEN_UNDERSCORE, "_"},
      };
      spa::Pattern pattern(spa::PatternType::ANY, patternTokens);

      spa::PatternClause patternClause(synonym, lhs, pattern, 2);
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

      spa::PqlArgument synonym(spa::ArgumentType::SYNONYM, "a", { spa::DesignEntityType::ASSIGN });
      spa::PqlArgument lhs(spa::ArgumentType::LITERAL_STRING, "x", {});
      std::vector<spa::Token> patternTokens = {
        {spa::TokenType::TOKEN_NAME, "x"},
      };
      spa::Pattern pattern(spa::PatternType::EXACT, patternTokens);

      spa::PatternClause patternClause(synonym, lhs, pattern, 2);
      std::unique_ptr<spa::QpsEvaluator> qpsEvaluator = patternClause.getEvaluator();
      spa::QpsResultTable resultTable = qpsEvaluator->evaluate(*pkbManager);
      std::vector<std::vector<spa::QpsValue>> rows = resultTable.getRows();

      Assert::IsTrue(rows.size() == 0);
    }
  };
}  // namespace IntegrationTesting

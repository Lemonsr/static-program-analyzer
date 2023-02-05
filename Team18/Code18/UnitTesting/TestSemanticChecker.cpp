#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParsedQuery.h"
#include "PqlSemanticChecker.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestSemanticChecker) {
    std::vector<spa::PqlArgument> arguments = {
      {spa::ArgumentType::LINE_NO, "1", {}},
      {spa::ArgumentType::VARIABLE_NAME, "x", {}},
      {spa::ArgumentType::WILDCARD, "_", {}},
      {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
      {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
      {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
      {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
      {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
      {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
      {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
      {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
      {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
    };
    spa::PqlSemanticChecker semanticChecker;
    TEST_METHOD(TestModifiesValid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::VARIABLE_NAME, "x", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::MODIFIES, firstArg, secondArg);
          parsedQuery.setSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsTrue(isValid);
        }
      }
    }

    TEST_METHOD(TestModifiesInvalid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::VARIABLE_NAME, "x", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
      };

      std::vector<spa::PqlArgument> invalidFirstArgs = {
        {spa::ArgumentType::VARIABLE_NAME, "x", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
      };

      std::vector<spa::PqlArgument> invalidSecondArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::MODIFIES, firstArg, secondArg);
          parsedQuery.setSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::MODIFIES, firstArg, secondArg);
          parsedQuery.setSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::MODIFIES, firstArg, secondArg);
          parsedQuery.setSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }
    }

    TEST_METHOD(TestUsesValid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::VARIABLE_NAME, "x", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::MODIFIES, firstArg, secondArg);
          parsedQuery.setSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsTrue(isValid);
        }
      }
    }

    TEST_METHOD(TestUsesInvalid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::VARIABLE_NAME, "x", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
      };

      std::vector<spa::PqlArgument> invalidFirstArgs = {
        {spa::ArgumentType::VARIABLE_NAME, "x", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::READ}},
      };

      std::vector<spa::PqlArgument> invalidSecondArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::MODIFIES, firstArg, secondArg);
          parsedQuery.setSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::MODIFIES, firstArg, secondArg);
          parsedQuery.setSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::MODIFIES, firstArg, secondArg);
          parsedQuery.setSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }
    }

    TEST_METHOD(TestFollowsValid) {

    }

    TEST_METHOD(TestFollowsInvalid) {

    }

    TEST_METHOD(TestParentValid) {

    }

    TEST_METHOD(TestParentInvalid) {

    }

    TEST_METHOD(TestPatternValid) {

    }

    TEST_METHOD(TestPatternInvalid) {

    }
  };
}  // namespace UnitTesting

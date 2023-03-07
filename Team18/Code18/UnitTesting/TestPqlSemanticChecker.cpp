#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParsedQuery.h"
#include "PqlSemanticChecker.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestPqlSemanticChecker) {
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
        {spa::ArgumentType::LITERAL_STRING, "func1", {}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::LITERAL_STRING, "x", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::MODIFIES, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
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
        {spa::ArgumentType::LITERAL_STRING, "func1", {}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::LITERAL_STRING, "x", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
      };

      std::vector<spa::PqlArgument> invalidFirstArgs = {
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "co", {spa::DesignEntityType::CONSTANT}},
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
        {spa::ArgumentType::SYNONYM, "co", {spa::DesignEntityType::CONSTANT}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::MODIFIES, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::MODIFIES, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::MODIFIES, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }
    }

    TEST_METHOD(TestUsesValid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
        {spa::ArgumentType::LITERAL_STRING, "func1", {}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::LITERAL_STRING, "x", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::USES, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsTrue(isValid);
        }
      }
    }

    TEST_METHOD(TestUsesInvalid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
        {spa::ArgumentType::LITERAL_STRING, "func1", {}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::LITERAL_STRING, "x", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
      };

      std::vector<spa::PqlArgument> invalidFirstArgs = {
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "co", {spa::DesignEntityType::CONSTANT}},
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
        {spa::ArgumentType::SYNONYM, "co", {spa::DesignEntityType::CONSTANT}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::USES, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::USES, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::USES, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }
    }

    TEST_METHOD(TestFollowsValid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::FOLLOWS, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsTrue(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::FOLLOWS_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsTrue(isValid);
        }
      }
    }

    TEST_METHOD(TestFollowsInvalid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      std::vector<spa::PqlArgument> invalidFirstArgs = {
        {spa::ArgumentType::LITERAL_STRING, "x", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "co", {spa::DesignEntityType::CONSTANT}},
      };

      std::vector<spa::PqlArgument> invalidSecondArgs = {
        {spa::ArgumentType::LITERAL_STRING, "x", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "co", {spa::DesignEntityType::CONSTANT}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::FOLLOWS, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::FOLLOWS_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::FOLLOWS, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::FOLLOWS_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::FOLLOWS, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::FOLLOWS_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }
    }

    TEST_METHOD(TestParentValid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::PARENT, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsTrue(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::PARENT_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsTrue(isValid);
        }
      }
    }

    TEST_METHOD(TestParentInvalid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      std::vector<spa::PqlArgument> invalidFirstArgs = {
        {spa::ArgumentType::LITERAL_STRING, "x", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "co", {spa::DesignEntityType::CONSTANT}},
      };

      std::vector<spa::PqlArgument> invalidSecondArgs = {
        {spa::ArgumentType::LITERAL_STRING, "x", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "co", {spa::DesignEntityType::CONSTANT}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::PARENT, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::PARENT_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::PARENT, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::PARENT_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::PARENT, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::PARENT_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }
    }

    TEST_METHOD(TestCallsValid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LITERAL_STRING, "func1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::LITERAL_STRING, "func1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "q", {spa::DesignEntityType::PROCEDURE}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::CALLS, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsTrue(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::CALLS_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsTrue(isValid);
        }
      }
    }

    TEST_METHOD(TestCallsInvalid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LITERAL_STRING, "func1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::LITERAL_STRING, "func1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "q", {spa::DesignEntityType::PROCEDURE}},
      };

      std::vector<spa::PqlArgument> invalidFirstArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
        {spa::ArgumentType::SYNONYM, "co", {spa::DesignEntityType::CONSTANT}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
      };

      std::vector<spa::PqlArgument> invalidSecondArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
        {spa::ArgumentType::SYNONYM, "co", {spa::DesignEntityType::CONSTANT}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
      };


      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::CALLS, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::CALLS_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::CALLS, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::CALLS_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::CALLS, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::CALLS_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }
    }

    TEST_METHOD(TestNextValid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::FOLLOWS, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsTrue(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::FOLLOWS_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsTrue(isValid);
        }
      }
    }

    TEST_METHOD(TestNextInvalid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      std::vector<spa::PqlArgument> validSecondArgs = {
        {spa::ArgumentType::LINE_NO, "1", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "s", {spa::DesignEntityType::STMT}},
        {spa::ArgumentType::SYNONYM, "re", {spa::DesignEntityType::READ}},
        {spa::ArgumentType::SYNONYM, "a", {spa::DesignEntityType::ASSIGN}},
        {spa::ArgumentType::SYNONYM, "pr", {spa::DesignEntityType::PRINT}},
        {spa::ArgumentType::SYNONYM, "i", {spa::DesignEntityType::IF}},
        {spa::ArgumentType::SYNONYM, "w", {spa::DesignEntityType::WHILE}},
        {spa::ArgumentType::SYNONYM, "c", {spa::DesignEntityType::CALL}},
      };

      std::vector<spa::PqlArgument> invalidFirstArgs = {
        {spa::ArgumentType::LITERAL_STRING, "x", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "co", {spa::DesignEntityType::CONSTANT}},
      };

      std::vector<spa::PqlArgument> invalidSecondArgs = {
        {spa::ArgumentType::LITERAL_STRING, "x", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
        {spa::ArgumentType::SYNONYM, "p", {spa::DesignEntityType::PROCEDURE}},
        {spa::ArgumentType::SYNONYM, "co", {spa::DesignEntityType::CONSTANT}},
      };

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::FOLLOWS, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::FOLLOWS_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : validSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::FOLLOWS, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::FOLLOWS_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }

      for (auto& firstArg : invalidFirstArgs) {
        for (auto& secondArg : invalidSecondArgs) {
          spa::SuchThatClause suchThatClause(spa::RelationshipType::FOLLOWS, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);

          suchThatClause = spa::SuchThatClause(spa::RelationshipType::FOLLOWS_STAR, firstArg, secondArg);
          parsedQuery.addSuchThatClause(suchThatClause);
          isValid = semanticChecker.isSemanticallyValid(parsedQuery);
          Assert::IsFalse(isValid);
        }
      }
    }

    TEST_METHOD(TestPatternValid) {
      std::vector<spa::PqlArgument> validFirstArgs = {
        {spa::ArgumentType::LITERAL_STRING, "x", {}},
        {spa::ArgumentType::WILDCARD, "_", {}},
        {spa::ArgumentType::SYNONYM, "v", {spa::DesignEntityType::VARIABLE}},
      };

      spa::Pattern pattern(spa::PatternType::ANY, std::vector<spa::Token>());
      spa::PqlArgument assignSynonym(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN);
      spa::PqlArgument ifSynonym(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF);
      spa::PqlArgument whileSynonym(spa::ArgumentType::SYNONYM, "w", spa::DesignEntityType::WHILE);

      spa::ParsedQuery parsedQuery;
      for (auto& firstArg : validFirstArgs) {
        spa::PatternClause patternAssignClause(assignSynonym, firstArg, pattern, 2);
        spa::PatternClause patternIfClause(ifSynonym, firstArg, pattern, 3);
        spa::PatternClause patternWhileClause(whileSynonym, firstArg, pattern, 2);
        parsedQuery.addPatternClause(patternAssignClause);
        parsedQuery.addPatternClause(patternIfClause);
        parsedQuery.addPatternClause(patternWhileClause);
        bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
        Assert::IsTrue(isValid);
      }
    }

    TEST_METHOD(TestPatternInvalid) {
      std::vector<spa::PqlArgument> invalidFirstArgs = {
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

      spa::Pattern pattern(spa::PatternType::ANY, std::vector<spa::Token>());
      spa::PqlArgument assignSynonym(spa::ArgumentType::SYNONYM, "a", spa::DesignEntityType::ASSIGN);
      spa::PqlArgument ifSynonym(spa::ArgumentType::SYNONYM, "if", spa::DesignEntityType::IF);
      spa::PqlArgument whileSynonym(spa::ArgumentType::SYNONYM, "w", spa::DesignEntityType::WHILE);

      for (auto& firstArg : invalidFirstArgs) {
        spa::ParsedQuery parsedQuery;
        spa::PatternClause patternAssignClause(assignSynonym, firstArg, pattern, 2);
        parsedQuery.addPatternClause(patternAssignClause);
        bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
        Assert::IsFalse(isValid);
      }

      for (auto& firstArg : invalidFirstArgs) {
        spa::ParsedQuery parsedQuery;
        spa::PatternClause patternIfClause(ifSynonym, firstArg, pattern, 3);
        parsedQuery.addPatternClause(patternIfClause);
        bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
        Assert::IsFalse(isValid);
      }

      for (auto& firstArg : invalidFirstArgs) {
        spa::ParsedQuery parsedQuery;
        spa::PatternClause patternWhileClause(whileSynonym, firstArg, pattern, 2);
        parsedQuery.addPatternClause(patternWhileClause);
        bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
        Assert::IsFalse(isValid);
      }

      for (auto& patternType : { spa::PatternType::EXACT, spa::PatternType::PARTIAL }) {
        spa::PqlArgument firstArg(spa::ArgumentType::SYNONYM, "v", { spa::DesignEntityType::VARIABLE });
        spa::Pattern invalidPattern(patternType, std::vector<spa::Token>());
        spa::PatternClause patternIfClause(ifSynonym, firstArg, invalidPattern, 3);
        spa::ParsedQuery parsedQuery;
        parsedQuery.addPatternClause(patternIfClause);
        bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
        Assert::IsFalse(isValid);
      }

      for (auto& patternType : { spa::PatternType::EXACT, spa::PatternType::PARTIAL }) {
        spa::PqlArgument firstArg(spa::ArgumentType::SYNONYM, "v", { spa::DesignEntityType::VARIABLE });
        spa::Pattern invalidPattern(patternType, std::vector<spa::Token>());
        spa::PatternClause patternIfClause(whileSynonym, firstArg, invalidPattern, 3);
        spa::ParsedQuery parsedQuery;
        parsedQuery.addPatternClause(patternIfClause);
        bool isValid = semanticChecker.isSemanticallyValid(parsedQuery);
        Assert::IsFalse(isValid);
      }
    }

    TEST_METHOD(TestWithValid) {
      spa::ParsedQuery parsedQuery;
      spa::WithArgument stringAttr("c.procName");
      spa::WithArgument intAttr("s.stmt#");
      spa::WithArgument strVal(spa::QpsValue("func1"));
      spa::WithArgument intVal(spa::QpsValue(1));

      parsedQuery.addWithClause(spa::WithClause(stringAttr, stringAttr));
      parsedQuery.addWithClause(spa::WithClause(stringAttr, strVal));
      parsedQuery.addWithClause(spa::WithClause(strVal, stringAttr));
      parsedQuery.addWithClause(spa::WithClause(intAttr, intAttr));
      parsedQuery.addWithClause(spa::WithClause(intAttr, intVal));
      parsedQuery.addWithClause(spa::WithClause(intVal, intAttr));
      
      Assert::IsTrue(semanticChecker.isSemanticallyValid(parsedQuery));
    }

    TEST_METHOD(TestWithInvalid) {
      spa::ParsedQuery parsedQuery;
      spa::WithArgument stringAttr("c.procName");
      spa::WithArgument intAttr("s.stmt#");
      spa::WithArgument strVal(spa::QpsValue("func1"));
      spa::WithArgument intVal(spa::QpsValue(1));

      parsedQuery.addWithClause(spa::WithClause(stringAttr, intAttr));
      parsedQuery.addWithClause(spa::WithClause(intAttr, stringAttr));
      parsedQuery.addWithClause(spa::WithClause(stringAttr, intVal));
      parsedQuery.addWithClause(spa::WithClause(intVal, stringAttr));
      parsedQuery.addWithClause(spa::WithClause(strVal, intAttr));
      parsedQuery.addWithClause(spa::WithClause(intAttr, strVal));
      parsedQuery.addWithClause(spa::WithClause(strVal, intVal));
      parsedQuery.addWithClause(spa::WithClause(intVal, strVal));

      Assert::IsFalse(semanticChecker.isSemanticallyValid(parsedQuery));
    }
  };
}  // namespace UnitTesting

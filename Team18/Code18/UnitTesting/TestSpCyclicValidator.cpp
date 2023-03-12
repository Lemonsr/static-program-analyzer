#include <iostream>
#include <vector>

#include "stdafx.h"

#include "DesignExtractor.h"
#include "NonContainerStatement.h"
#include "SpParser.h"
#include "Token.h"
#include "UtilsFunction.h"
#include "SpCyclicValidator.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
    TEST_CLASS(TestSpCyclicValidator) {
        std::string varA = "a";
        std::string varB = "b";
        std::string varC = "c";
        std::string varD = "d";
        std::string varE = "e";
        std::string constant = "1";

        std::string openBracket = "(";
        std::string closeBracket = ")";
        std::string openBrace = "{";
        std::string closeBrace = "}";
        std::string semiColon = ";";

        std::string minusOp = "-";
        std::string multiplyOp = "*";
        std::string greaterEqOp = ">=";
        std::string equal = "=";

        std::string readString = "read";
        std::string callString = "call";
        std::string printString = "print";
        std::string procedure = "procedure";
        std::string ifString = "if";
        std::string thenString = "then";
        std::string elseString = "else";
        std::string whileString = "while";

        spa::Token tokenA = spa::Token(spa::TOKEN_NAME, varA);
        spa::Token tokenB = spa::Token(spa::TOKEN_NAME, varB);
        spa::Token tokenC = spa::Token(spa::TOKEN_NAME, varC);
        spa::Token tokenD = spa::Token(spa::TOKEN_NAME, varD);
        spa::Token tokenE = spa::Token(spa::TOKEN_NAME, varE);
        spa::Token tokenConstant = spa::Token(spa::TOKEN_INTEGER, constant);

        spa::Token tokenOpenBracket = spa::Token(spa::TOKEN_OPEN_BRACKET, openBracket);
        spa::Token tokenCloseBracket = spa::Token(spa::TOKEN_CLOSE_BRACKET, closeBracket);
        spa::Token tokenOpenBrace = spa::Token(spa::TOKEN_OPEN_BRACE, openBrace);
        spa::Token tokenCloseBrace = spa::Token(spa::TOKEN_CLOSE_BRACE, closeBrace);
        spa::Token tokenSemiColon = spa::Token(spa::TOKEN_SEMICOLON, semiColon);

        spa::Token tokenMinusOp = spa::Token(spa::TOKEN_MINUS, minusOp);
        spa::Token tokenMultiply = spa::Token(spa::TOKEN_MULTIPLY, multiplyOp);
        spa::Token tokenAssign = spa::Token(spa::TOKEN_EQUAL, equal);
        spa::Token tokenGreaterEqual = spa::Token(spa::TOKEN_COND_GTE, greaterEqOp);

        spa::Token tokenRead = spa::Token(spa::TOKEN_READ, readString);
        spa::Token tokenCall = spa::Token(spa::TOKEN_CALL, callString);
        spa::Token tokenPrint = spa::Token(spa::TOKEN_PRINT, printString);
        spa::Token tokenProcedure = spa::Token(spa::TOKEN_PROCEDURE, procedure);
        spa::Token tokenIf = spa::Token(spa::TOKEN_IF, ifString);
        spa::Token tokenThen = spa::Token(spa::TOKEN_THEN, thenString);
        spa::Token tokenElse = spa::Token(spa::TOKEN_ELSE, elseString);
        spa::Token tokenWhile = spa::Token(spa::TOKEN_WHILE, whileString);

        std::vector<spa::Token> tokenList{};

public:
    TEST_METHOD(TestSingleCallsNonCyclic) {
        /*
         *   procedure a {
         * 1.    b = 1;
         * 2.    c = b;
         * 3.    call d;
         *  }
         *   procedure d {
         * 4.    a = 5;
         *   }
         */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenC, tokenAssign, tokenB, tokenSemiColon,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };
        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }
        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

        std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
        for (auto& procedure : procedureList) {
          auto statements = procedure->getStatementLst();
          for (auto& statement : statements) {
            if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
              auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
              procedure->addCalledVars(callStatement->getVariableName());
            }
          }
          procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
            procedure->getCalledVars());
        }
        spa::SpCyclicValidator cyclicValidator(procCallMap);

        bool testCyclicCall = cyclicValidator.validateCyclic();
        bool expectedCyclicCall = false;

        Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }

    TEST_METHOD(TestDoubleCallsNonCyclicOne) {
      /*
       *   procedure a {
       * 1.    b = 1;
       * 2.    c = b;
       * 3.    call d;
       * 4.    call e;
       *  }
       *   procedure d {
       * 5.    a = 5;
       *   }
       *   procedure e {
       * 6.    b = 6;
       *   }
       */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenC, tokenAssign, tokenB, tokenSemiColon,
        tokenCall, tokenD, tokenSemiColon,
        tokenCall, tokenE, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenD, tokenOpenBrace,
        tokenA, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenE, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = false;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }

    TEST_METHOD(TestDoubleCallsCyclic) {
      /*
       *   procedure a {
       * 1.    b = 1;
       * 2.    c = b;
       * 3.    call d;
       *  }
       *   procedure d {
       * 4.    call a;
       *   }
       */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenC, tokenAssign, tokenB, tokenSemiColon,
        tokenCall, tokenD, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenD, tokenOpenBrace,
        tokenCall, tokenA, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = true;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }
    TEST_METHOD(TestTwoCallsNonCyclicTwo) {
      /*
       *   procedure a {
       * 1.    b = 1;
       * 2.    c = b;
       * 3.    call d;
       *  }
       *   procedure d {
       * 4.    call e;
       *   }
       *   procedure e {
       * 5.    b = 6;
       *   }
       */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenC, tokenAssign, tokenB, tokenSemiColon,
        tokenCall, tokenD, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenD, tokenOpenBrace,
        tokenCall, tokenE, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenE, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = false;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }

    TEST_METHOD(TestThreeCallsNonCyclic) {
      /*
       *   procedure a {
       * 1.    b = 1;
       * 2.    c = b;
       * 3.    call d;
       *  }
       *   procedure d {
       * 4.    call e;
       *   }
       *   procedure e {
       * 5.    call b;
       *   }
       *   procedure b {
       * 6.    d = 9;
       *   }
       *
       */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenC, tokenAssign, tokenB, tokenSemiColon,
        tokenCall, tokenD, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenD, tokenOpenBrace,
        tokenCall, tokenE, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenE, tokenOpenBrace,
        tokenCall, tokenB, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenB, tokenOpenBrace,
        tokenD, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = false;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }


    TEST_METHOD(TestThreeCallsCyclicOne) {
      /*
       *   procedure a {
       * 1.    b = 1;
       * 2.    c = b;
       * 3.    call d;
       * 4.    call e;
       *  }
       *   procedure d {
       * 5.    a = 5;
       *   }
       *   procedure e {
       * 6.    call a;
       *   }
       */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenC, tokenAssign, tokenB, tokenSemiColon,
        tokenCall, tokenD, tokenSemiColon,
        tokenCall, tokenE, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenD, tokenOpenBrace,
        tokenA, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenE, tokenOpenBrace,
        tokenCall, tokenA, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = true;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }
    TEST_METHOD(TestFourCallsNonCyclic) {
      /*
       *   procedure a {
       * 1.    b = 1;
       * 2.    c = b;
       * 3.    call d;
       *  }
       *   procedure d {
       * 4.    call e;
       *   }
       *   procedure e {
       * 5.    call b;
       *   }
       *   procedure b {
       * 6.    call c;
       *   }
       *
       *   procedure c{
       * 7.     a = 1;
       *    }
       *
       */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenC, tokenAssign, tokenB, tokenSemiColon,
        tokenCall, tokenD, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenD, tokenOpenBrace,
        tokenCall, tokenE, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenE, tokenOpenBrace,
        tokenCall, tokenB, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenB, tokenOpenBrace,
        tokenCall, tokenC, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenC, tokenOpenBrace,
        tokenA, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = false;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }
    TEST_METHOD(TestFourCallsCyclic) {
      /*
       *   procedure a {
       * 1.    b = 1;
       * 2.    c = b;
       * 3.    call d;
       *  }
       *   procedure d {
       * 4.    call e;
       *   }
       *   procedure e {
       * 5.    call b;
       *   }
       *   procedure b {
       * 6.    call c;
       *   }
       *
       *   procedure c{
       * 7.    call d;
       *    }
       *
       */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenC, tokenAssign, tokenB, tokenSemiColon,
        tokenCall, tokenD, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenD, tokenOpenBrace,
        tokenCall, tokenE, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenE, tokenOpenBrace,
        tokenCall, tokenB, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenB, tokenOpenBrace,
        tokenCall, tokenC, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenC, tokenOpenBrace,
        tokenCall, tokenD, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = true;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }
    TEST_METHOD(TestNestedOneCallsNonCyclic) {
      /*
      *  procedure a {
      *1.    a = 5;
      *2.    call b;
      *3.    while (b >= c) {
      *4.        call c;
      *      }
      *  }
      *5. procedure b {
      *6.    call c;
      *  }
      *7. procedure c{
      *8.    c = 10;
      *  }
      *
      */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCall, tokenB, tokenSemiColon,
        tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC, tokenCloseBracket, tokenOpenBrace,
        tokenCall, tokenC, tokenSemiColon,
        tokenCloseBrace, tokenCloseBrace,
        tokenProcedure, tokenB, tokenOpenBrace,
        tokenCall, tokenC, tokenSemiColon,
        tokenCloseBrace, tokenProcedure,
        tokenC, tokenOpenBrace,
        tokenC, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = false;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }

    TEST_METHOD(TestNestedOneCallsCyclic) {
      /*
      *  procedure a {
      *1.    a = 5;
      *2.    call b;
      *3.    while (b >= c) {
      *4.        call c;
      *      }
      *  }
      *5. procedure b {
      *6.    call a;
      *  }
      *7. procedure c{
      *8.    c = 10;
      *  }
      *
      */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCall, tokenB, tokenSemiColon,
        tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC, tokenCloseBracket, tokenOpenBrace,
        tokenCall, tokenC, tokenSemiColon,
        tokenCloseBrace, tokenCloseBrace,
        tokenProcedure, tokenB, tokenOpenBrace,
        tokenCall, tokenA, tokenSemiColon,
        tokenCloseBrace, tokenProcedure,
        tokenC, tokenOpenBrace,
        tokenC, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = true;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }

    TEST_METHOD(TestNestedTwoCallsOneNonCyclic) {
      /*
      *  procedure a{
      *1.    a = 5;
      *2.    call b;
      *3.    while (b >= c) {
      *4.        if (d >= 1) then {
      *5.            b = 3;
      *          }
      *          else {
      *6.            call c;
      *          }
      *      }
      *  }
      *    procedure b{
      *7.     call c;
      *  }
      *    procedure c{
      *8.     c = 10;
      *  }
      */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenA, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCall, tokenB, tokenSemiColon,
        tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC,
        tokenCloseBracket, tokenOpenBrace,
        tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
        tokenCloseBracket, tokenThen, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace, tokenElse, tokenOpenBrace,
        tokenCall, tokenC, tokenSemiColon,
        tokenCloseBrace, tokenCloseBrace, tokenCloseBrace,
        tokenProcedure, tokenB, tokenOpenBrace,
        tokenCall, tokenC, tokenSemiColon,
        tokenCloseBrace, tokenProcedure, tokenC, tokenOpenBrace,
        tokenC, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = false;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }

    TEST_METHOD(TestNestedTwoCallsOneCyclic) {
      /*
      *  procedure a{
      *1.    a = 5;
      *2.    call b;
      *3.    while (b >= c) {
      *4.        if (d >= 1) then {
      *5.            b = 3;
      *          }
      *          else {
      *6.            call c;
      *          }
      *      }
      *  }
      *    procedure b{
      *7.     call a;
      *  }
      *    procedure c{
      *8.     c = 10;
      *  }
      */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenA, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCall, tokenB, tokenSemiColon,
        tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC,
        tokenCloseBracket, tokenOpenBrace,
        tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
        tokenCloseBracket, tokenThen, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace, tokenElse, tokenOpenBrace,
        tokenCall, tokenA, tokenSemiColon,
        tokenCloseBrace, tokenCloseBrace, tokenCloseBrace,
        tokenProcedure, tokenB, tokenOpenBrace,
        tokenCall, tokenA, tokenSemiColon,
        tokenCloseBrace, tokenProcedure, tokenC, tokenOpenBrace,
        tokenC, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = true;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }

    TEST_METHOD(TestNestedTwoCallsTwoNonCyclic) {
      /*
      *  procedure a{
      *1.    a = 5;
      *2.    call b;
      *3.    while (b >= c) {
      *4.        if (d >= 1) then {
      *5.            b = 3;
      *6.            call c;
      *          }
      *          else {
      *7.           b = 3;
      *          }
      *      }
      *  }
      *  procedure b{
      *8.   call c;
      *  }
      *  procedure c{
      *9.   c = 10;
      *  }
      */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenA, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCall, tokenB, tokenSemiColon,
        tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC,
        tokenCloseBracket, tokenOpenBrace,
        tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
        tokenCloseBracket, tokenThen, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCall, tokenC, tokenSemiColon,
        tokenCloseBrace, tokenElse, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace,
        tokenCloseBrace, tokenCloseBrace,
        tokenProcedure, tokenB, tokenOpenBrace,
        tokenCall, tokenC, tokenSemiColon,
        tokenCloseBrace, tokenProcedure, tokenC, tokenOpenBrace,
        tokenC, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace
      };


      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = false;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }

    TEST_METHOD(TestNestedTwoCallsTwoCyclic) {
      /*
      *  procedure a{
      *1.    a = 5;
      *2.    call b;
      *3.    while (b >= c) {
      *4.        if (d >= 1) then {
      *5.            b = 3;
      *6.            call c;
      *          }
      *          else {
      *7.           b = 3;
      *          }
      *      }
      *  }
      *  procedure b{
      *8.   call c;
      *  }
      *  procedure c{
      *9.   call a;
      *  }
      */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenA, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCall, tokenB, tokenSemiColon,
        tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC,
        tokenCloseBracket, tokenOpenBrace,
        tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
        tokenCloseBracket, tokenThen, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCall, tokenC, tokenSemiColon,
        tokenCloseBrace, tokenElse, tokenOpenBrace,
        tokenB, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace,
        tokenCloseBrace, tokenCloseBrace,
        tokenProcedure, tokenB, tokenOpenBrace,
        tokenCall, tokenC, tokenSemiColon,
        tokenCloseBrace, tokenProcedure, tokenC, tokenOpenBrace,
        tokenCall, tokenA, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = true;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }

    TEST_METHOD(TestNestedMultipleCallsNonCyclic) {
      /*
      *  procedure a{
      *1.    a = 5;
      *2.    while (b >= c) {
      *3.        call c;
      *4.        while (c >= 8) {
      *5.            call b;
      *6.            while (d >= 6) {
      *7.                call d;
      *              }
      *          }
      *      }
      *  }
      *  procedure b{
      *8.  call c;
      *  }
      *  procedure c{
      *9.  c = 10;
      *  }
      *  procedure d{
      *10.  d = 10;
      *  }
      */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenA, tokenAssign, tokenConstant, tokenSemiColon,
        tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC, tokenCloseBracket, tokenOpenBrace,
        tokenCall, tokenC, tokenSemiColon,
        tokenWhile, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenOpenBrace,
        tokenCall, tokenB, tokenSemiColon,
        tokenWhile, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenOpenBrace,
        tokenCall, tokenD, tokenSemiColon,
        tokenCloseBrace, tokenCloseBrace, tokenCloseBrace, tokenCloseBrace,
        tokenProcedure, tokenB, tokenOpenBrace,
        tokenCall, tokenC, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenC, tokenOpenBrace,
        tokenC, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenD, tokenOpenBrace,
        tokenD, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

      spa::SpCyclicValidator cyclicValidator(procCallMap);

      bool testCyclicCall = cyclicValidator.validateCyclic();
      bool expectedCyclicCall = false;

      Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }

    TEST_METHOD(TestNestedMultipleCallsCyclic) {
      /*
      *  procedure a{
      *1.    a = 5;
      *2.    while (b >= c) {
      *3.        call c;
      *4.        while (c >= 8) {
      *5.            call b;
      *6.            while (d >= 6) {
      *7.                call d;
      *              }
      *          }
      *      }
      *  }
      *  procedure b{
      *8.  call c;
      *  }
      *  procedure c{
      *9.  call b;
      *  }
      *  procedure d{
      *10.  d = 10;
      *  }
      */
      tokenList = {
        tokenProcedure, tokenA, tokenOpenBrace,
        tokenA, tokenAssign, tokenConstant, tokenSemiColon,
        tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC, tokenCloseBracket, tokenOpenBrace,
        tokenCall, tokenC, tokenSemiColon,
        tokenWhile, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenOpenBrace,
        tokenCall, tokenB, tokenSemiColon,
        tokenWhile, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenOpenBrace,
        tokenCall, tokenD, tokenSemiColon,
        tokenCloseBrace, tokenCloseBrace, tokenCloseBrace, tokenCloseBrace,
        tokenProcedure, tokenB, tokenOpenBrace,
        tokenCall, tokenC, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenC, tokenOpenBrace,
        tokenCall, tokenB, tokenSemiColon,
        tokenCloseBrace,
        tokenProcedure, tokenD, tokenOpenBrace,
        tokenD, tokenAssign, tokenConstant, tokenSemiColon,
        tokenCloseBrace
      };

      spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
      for (auto token : tokenList) {
        tokenStream.pushBack(token);
      }
      auto parser = spa::SpParser(tokenStream);
      std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();

      std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
      for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
          if (std::dynamic_pointer_cast<spa::CallStatement>(statement)) {
            auto callStatement = std::dynamic_pointer_cast<spa::CallStatement>(statement);
            procedure->addCalledVars(callStatement->getVariableName());
          }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(),
          procedure->getCalledVars());
      }

        spa::SpCyclicValidator cyclicValidator(procCallMap);

        bool testCyclicCall = cyclicValidator.validateCyclic();
        bool expectedCyclicCall = true;

        Assert::IsTrue(expectedCyclicCall == testCyclicCall);
    }
    };
}  // namespace UnitTesting

#include "stdafx.h"

#include <iostream>
#include <vector>

#include "UtilsFunction.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestUtilsFunction) {
    std::string varA = "a";
    std::string varB = "b";
    std::string varC = "c";
    std::string varD = "d";
    std::string leftBracket = "(";
    std::string rightBracket = ")";
    std::string constant = "1";
    std::string minusOp = "-";
    std::string plusOp = "+";
    std::string multiplyOp = "*";
    std::string divideOp = "/";
    std::string greaterEqOp = ">=";
    std::string logicalOr = "||";
    std::string logicalAnd = "&&";

    spa::Token tokenA = spa::Token(spa::TOKEN_NAME, varA);
    spa::Token tokenB = spa::Token(spa::TOKEN_NAME, varB);
    spa::Token tokenC = spa::Token(spa::TOKEN_NAME, varC);
    spa::Token tokenD = spa::Token(spa::TOKEN_NAME, varD);
    spa::Token tokenLeftBracket = spa::Token(spa::TOKEN_NAME, leftBracket);
    spa::Token tokenRightBracket = spa::Token(spa::TOKEN_NAME, rightBracket);
    spa::Token tokenConstant = spa::Token(spa::TOKEN_NAME, constant);
    spa::Token tokenMinusOp = spa::Token(spa::TOKEN_NAME, minusOp);
    spa::Token tokenPlusOp = spa::Token(spa::TOKEN_NAME, plusOp);
    spa::Token tokenMultiplyOp = spa::Token(spa::TOKEN_NAME, multiplyOp);
    spa::Token tokenDivideOp = spa::Token(spa::TOKEN_NAME, divideOp);
    spa::Token tokenGreaterEqual = spa::Token(spa::TOKEN_NAME, greaterEqOp);
    spa::Token tokenLogicalOr = spa::Token(spa::TOKEN_NAME, logicalOr);
    spa::Token tokenLogicalAnd = spa::Token(spa::TOKEN_NAME, logicalAnd);


    std::string expectedResult = "";
    std::string testResults = "";
    UtilsFunction utils;

  public:
    TEST_METHOD(TestUtilsFunctionLTrim) {
      std::string stringToTest = "     abc";
      utils.ltrim(stringToTest);
      Assert::IsTrue(stringToTest == "abc");
    }

    TEST_METHOD(TestUtilsFunctionRTrim) {
      std::string stringToTest = "abc     ";
      utils.rtrim(stringToTest);
      Assert::IsTrue(stringToTest == "abc");
    }

    TEST_METHOD(TestUtilsFunctionTrimString) {
      std::string stringToTest = "     abc     ";
      utils.trimString(stringToTest);
      Assert::IsTrue(stringToTest == "abc");
    }

    TEST_METHOD(TestUtilsFunctionInfixToPostfixSamePrecedence) {
      // Test, Expected: "a-b+c-d", "ab-c+d-"
      std::vector<spa::Token> firstCase{
        tokenA, tokenMinusOp, tokenB,
        tokenPlusOp, tokenC, tokenMinusOp, tokenD
      };
      testResults = utils.infixToPostfix(firstCase);
      expectedResult = varA + " " + varB + " " + minusOp + " " + varC +
        " " + plusOp + " " + varD + " " + minusOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a/b*c/d", "ab/c*d/"
      std::vector<spa::Token> secondCase{
        tokenA, tokenDivideOp, tokenB,
        tokenMultiplyOp, tokenC, tokenDivideOp, tokenD
      };
      testResults = utils.infixToPostfix(secondCase);
      expectedResult = varA + " " + varB + " " + divideOp + " " + varC +
        " " + multiplyOp + " " + varD + " " + divideOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a/b*c/1", "ab/c*1/"
      std::vector<spa::Token> thirdCase{
        tokenA, tokenDivideOp, tokenB, tokenMultiplyOp, tokenC,
        tokenDivideOp, tokenConstant
      };
      testResults = utils.infixToPostfix(thirdCase);
      expectedResult = varA + " " + varB + " " + divideOp + " " + varC +
        " " + multiplyOp + " " + constant + " " +
        divideOp;
      Assert::IsTrue(expectedResult == testResults);
    }

    TEST_METHOD(TestUtilsFunctionInfixToPostfixDifferentPrecedence) {
      // Test, Expected: "a+b*c+d", "abc*+d+"
      std::vector<spa::Token> firstCase{
        tokenA, tokenPlusOp, tokenB,
        tokenMultiplyOp, tokenC, tokenPlusOp, tokenD
      };
      testResults = utils.infixToPostfix(firstCase);
      expectedResult = varA + " " + varB + " " + varC + " " + multiplyOp +
        " " + plusOp + " " + varD + " " + plusOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a*b*c-d", "ab*c*d-"
      std::vector<spa::Token> secondCase{
        tokenA, tokenMultiplyOp, tokenB, tokenMultiplyOp, tokenC,
        tokenMinusOp, tokenD
      };
      testResults = utils.infixToPostfix(secondCase);
      expectedResult = varA + " " + varB + " " + multiplyOp + " " + varC +
        " " + multiplyOp + " " + varD + " " +
        minusOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a*1*c-d", "a1*c*d-"
      std::vector<spa::Token> thirdCase{
        tokenA, tokenMultiplyOp, tokenConstant, tokenMultiplyOp, tokenC,
        tokenMinusOp, tokenD
      };
      testResults = utils.infixToPostfix(thirdCase);
      expectedResult = varA + " " + constant + " " + multiplyOp + " " + varC +
        " " + multiplyOp + " " + varD + " " +
        minusOp;
      Assert::IsTrue(expectedResult == testResults);
    }

    TEST_METHOD(TestUtilsFunctionInfixToPostfixSamePrecedenceWithBrackets) {
      // Test, Expected: "a+b-(c+d)", "ab+cd+-"
      std::vector<spa::Token> firstCase{
        tokenA, tokenPlusOp, tokenB, tokenMinusOp, tokenLeftBracket, tokenC,
        tokenPlusOp, tokenD, tokenRightBracket
      };
      testResults = utils.infixToPostfix(firstCase);
      expectedResult = varA + " " + varB + " " + plusOp + " " + varC + " " +
        varD + " " + plusOp + " " + minusOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a*(b*c/d)", "abc*d/*"
      std::vector<spa::Token> secondCase{
        tokenA, tokenMultiplyOp, tokenLeftBracket, tokenB, tokenMultiplyOp,
        tokenC, tokenDivideOp, tokenD,
        tokenRightBracket
      };
      testResults = utils.infixToPostfix(secondCase);
      expectedResult = varA + " " + varB + " " + varC + " " + multiplyOp +
        " " + varD + " " + divideOp + " " +
        multiplyOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a*(b*1/d)", "ab1*d/*"
      std::vector<spa::Token> thirdCase{
        tokenA, tokenMultiplyOp, tokenLeftBracket, tokenB, tokenMultiplyOp,
        tokenConstant, tokenDivideOp, tokenD, tokenRightBracket
      };
      testResults = utils.infixToPostfix(thirdCase);
      expectedResult = varA + " " + varB + " " + constant + " " + multiplyOp +
        " " + varD + " " + divideOp + " " +
        multiplyOp;
      Assert::IsTrue(expectedResult == testResults);
    }

    TEST_METHOD(TestUtilsFunctionInfixToPostfixDiffPrecedenceWithBrackets) {
      // Test, Expected: "a*(b-c)+d", "abc-*d+"
      std::vector<spa::Token> firstCase{
        tokenA, tokenMultiplyOp, tokenLeftBracket, tokenB, tokenMinusOp,
        tokenC, tokenRightBracket, tokenPlusOp, tokenD
      };
      testResults = utils.infixToPostfix(firstCase);
      expectedResult = varA + " " + varB + " " + varC + " " + minusOp + " " +
        multiplyOp + " " + varD + " " + plusOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "(a)*(b+c/d)", "abcd/+*"
      std::vector<spa::Token> secondCase{
        tokenLeftBracket, tokenA, tokenRightBracket, tokenMultiplyOp,
        tokenLeftBracket, tokenB, tokenPlusOp, tokenC,
        tokenDivideOp, tokenD, tokenRightBracket
      };
      testResults = utils.infixToPostfix(secondCase);
      expectedResult = varA + " " + varB + " " + varC + " " + varD + " " +
        divideOp + " " + plusOp + " " + multiplyOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "(1)*(b+c/1)", "1bc1/+*"
      std::vector<spa::Token> thirdCase{
        tokenLeftBracket, tokenConstant, tokenRightBracket, tokenMultiplyOp,
        tokenLeftBracket, tokenB, tokenPlusOp,
        tokenC, tokenDivideOp, tokenConstant, tokenRightBracket
      };
      testResults = utils.infixToPostfix(thirdCase);
      expectedResult = constant + " " + varB + " " + varC + " " + constant +
        " " + divideOp + " " + plusOp + " " +
        multiplyOp;
      Assert::IsTrue(expectedResult == testResults);
    }

    TEST_METHOD(TestUtilsFunctionInfixToPostfixWithMultiCharOperator) {
      // Test, Expected: "a>=b", "ab>="
      std::vector<spa::Token> firstCase{tokenA, tokenGreaterEqual, tokenB};
      testResults = utils.infixToPostfix(firstCase);
      expectedResult = varA + " " + varB + " " + greaterEqOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a>=b&&c>=d", "ab>=cd>=&&"
      std::vector<spa::Token> secondCase{
        tokenA, tokenGreaterEqual, tokenB, tokenLogicalAnd, tokenC,
        tokenGreaterEqual, tokenD
      };
      testResults = utils.infixToPostfix(secondCase);
      expectedResult = varA + " " + varB + " " + greaterEqOp + " " + varC +
        " " + varD + " " + greaterEqOp + " " +
        logicalAnd;
      Assert::IsTrue(expectedResult == testResults);
    }
  };
} // namespace UnitTesting
#include <iostream>
#include <vector>

#include "stdafx.h"
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

    std::vector<spa::Token> infixTokens = {};
    std::string expectedResult = "";
    std::string testResults = "";

  public:
    TEST_METHOD(TestUtilsFunctionLTrim) {
      std::string stringToTest = "     abc";
      spa::UtilsFunction::ltrim(stringToTest);
      Assert::IsTrue(stringToTest == "abc");
    }

    TEST_METHOD(TestUtilsFunctionRTrim) {
      std::string stringToTest = "abc     ";
      spa::UtilsFunction::rtrim(stringToTest);
      Assert::IsTrue(stringToTest == "abc");
    }

    TEST_METHOD(TestUtilsFunctionTrimString) {
      std::string stringToTest = "     abc     ";
      spa::UtilsFunction::trimString(stringToTest);
      Assert::IsTrue(stringToTest == "abc");
    }

    TEST_METHOD(TestUtilsFunctionInfixToPostfixSamePrecedence) {
      // Test, Expected: "a-b+c-d", "ab-c+d-"
      infixTokens = {
        tokenA, tokenMinusOp, tokenB,
        tokenPlusOp, tokenC, tokenMinusOp, tokenD
      };
      testResults = spa::UtilsFunction::infixToPostfix(infixTokens);
      expectedResult = varA + " " + varB + " " + minusOp + " " + varC +
        " " + plusOp + " " + varD + " " + minusOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a/b*c/d", "ab/c*d/"
      infixTokens = {
        tokenA, tokenDivideOp, tokenB,
        tokenMultiplyOp, tokenC, tokenDivideOp, tokenD
      };
      testResults = spa::UtilsFunction::infixToPostfix(infixTokens);
      expectedResult = varA + " " + varB + " " + divideOp + " " + varC +
        " " + multiplyOp + " " + varD + " " + divideOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a/b*c/1", "ab/c*1/"
      infixTokens = {
        tokenA, tokenDivideOp, tokenB, tokenMultiplyOp, tokenC,
        tokenDivideOp, tokenConstant
      };
      testResults = spa::UtilsFunction::infixToPostfix(infixTokens);
      expectedResult = varA + " " + varB + " " + divideOp + " " + varC +
        " " + multiplyOp + " " + constant + " " +
        divideOp;
      Assert::IsTrue(expectedResult == testResults);
    }

    TEST_METHOD(TestUtilsFunctionInfixToPostfixDifferentPrecedence) {
      // Test, Expected: "a+b*c+d", "abc*+d+"
      infixTokens = {
        tokenA, tokenPlusOp, tokenB,
        tokenMultiplyOp, tokenC, tokenPlusOp, tokenD
      };
      testResults = spa::UtilsFunction::infixToPostfix(infixTokens);
      expectedResult = varA + " " + varB + " " + varC + " " + multiplyOp +
        " " + plusOp + " " + varD + " " + plusOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a*b*c-d", "ab*c*d-"
      infixTokens = {
        tokenA, tokenMultiplyOp, tokenB, tokenMultiplyOp, tokenC,
        tokenMinusOp, tokenD
      };
      testResults = spa::UtilsFunction::infixToPostfix(infixTokens);
      expectedResult = varA + " " + varB + " " + multiplyOp + " " + varC +
        " " + multiplyOp + " " + varD + " " +
        minusOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a*1*c-d", "a1*c*d-"
      infixTokens = {
        tokenA, tokenMultiplyOp, tokenConstant, tokenMultiplyOp, tokenC,
        tokenMinusOp, tokenD
      };
      testResults = spa::UtilsFunction::infixToPostfix(infixTokens);
      expectedResult = varA + " " + constant + " " + multiplyOp + " " + varC +
        " " + multiplyOp + " " + varD + " " +
        minusOp;
      Assert::IsTrue(expectedResult == testResults);
    }

    TEST_METHOD(TestUtilsFunctionInfixToPostfixSamePrecedenceWithBrackets) {
      // Test, Expected: "a+b-(c+d)", "ab+cd+-"
      infixTokens = {
        tokenA, tokenPlusOp, tokenB, tokenMinusOp, tokenLeftBracket, tokenC,
        tokenPlusOp, tokenD, tokenRightBracket
      };
      testResults = spa::UtilsFunction::infixToPostfix(infixTokens);
      expectedResult = varA + " " + varB + " " + plusOp + " " + varC + " " +
        varD + " " + plusOp + " " + minusOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a*(b*c/d)", "abc*d/*"
      infixTokens = {
        tokenA, tokenMultiplyOp, tokenLeftBracket, tokenB, tokenMultiplyOp,
        tokenC, tokenDivideOp, tokenD,
        tokenRightBracket
      };
      testResults = spa::UtilsFunction::infixToPostfix(infixTokens);
      expectedResult = varA + " " + varB + " " + varC + " " + multiplyOp +
        " " + varD + " " + divideOp + " " +
        multiplyOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a*(b*1/d)", "ab1*d/*"
      std::vector<spa::Token> thirdCase{
        tokenA, tokenMultiplyOp, tokenLeftBracket, tokenB, tokenMultiplyOp,
        tokenConstant, tokenDivideOp, tokenD, tokenRightBracket
      };
      testResults = spa::UtilsFunction::infixToPostfix(thirdCase);
      expectedResult = varA + " " + varB + " " + constant + " " + multiplyOp +
        " " + varD + " " + divideOp + " " +
        multiplyOp;
      Assert::IsTrue(expectedResult == testResults);
    }

    TEST_METHOD(TestUtilsFunctionInfixToPostfixDiffPrecedenceWithBrackets) {
      // Test, Expected: "a*(b-c)+d", "abc-*d+"
      infixTokens = {
        tokenA, tokenMultiplyOp, tokenLeftBracket, tokenB, tokenMinusOp,
        tokenC, tokenRightBracket, tokenPlusOp, tokenD
      };
      testResults = spa::UtilsFunction::infixToPostfix(infixTokens);
      expectedResult = varA + " " + varB + " " + varC + " " + minusOp + " " +
        multiplyOp + " " + varD + " " + plusOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "(a)*(b+c/d)", "abcd/+*"
      infixTokens = {
        tokenLeftBracket, tokenA, tokenRightBracket, tokenMultiplyOp,
        tokenLeftBracket, tokenB, tokenPlusOp, tokenC,
        tokenDivideOp, tokenD, tokenRightBracket
      };
      testResults = spa::UtilsFunction::infixToPostfix(infixTokens);
      expectedResult = varA + " " + varB + " " + varC + " " + varD + " " +
        divideOp + " " + plusOp + " " + multiplyOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "(1)*(b+c/1)", "1bc1/+*"
      infixTokens = {
        tokenLeftBracket, tokenConstant, tokenRightBracket, tokenMultiplyOp,
        tokenLeftBracket, tokenB, tokenPlusOp,
        tokenC, tokenDivideOp, tokenConstant, tokenRightBracket
      };
      testResults = spa::UtilsFunction::infixToPostfix(infixTokens);
      expectedResult = constant + " " + varB + " " + varC + " " + constant +
        " " + divideOp + " " + plusOp + " " +
        multiplyOp;
      Assert::IsTrue(expectedResult == testResults);
    }

    TEST_METHOD(TestUtilsFunctionInfixToPostfixWithMultiCharOperator) {
      // Test, Expected: "a>=b", "ab>="
      infixTokens = {tokenA, tokenGreaterEqual, tokenB};
      testResults = spa::UtilsFunction::infixToPostfix(infixTokens);
      expectedResult = varA + " " + varB + " " + greaterEqOp;
      Assert::IsTrue(expectedResult == testResults);

      // Test, Expected: "a>=b&&c>=d", "ab>=cd>=&&"
      infixTokens = {
        tokenA, tokenGreaterEqual, tokenB, tokenLogicalAnd, tokenC,
        tokenGreaterEqual, tokenD
      };
      testResults = spa::UtilsFunction::infixToPostfix(infixTokens);
      expectedResult = varA + " " + varB + " " + greaterEqOp + " " + varC +
        " " + varD + " " + greaterEqOp + " " +
        logicalAnd;
      Assert::IsTrue(expectedResult == testResults);
    }
  };
}  // namespace UnitTesting

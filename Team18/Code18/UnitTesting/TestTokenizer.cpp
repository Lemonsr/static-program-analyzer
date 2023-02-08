#include <iostream>
#include <sstream>
#include <string>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "Tokenizer.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestTokenizer) {
public:
  TEST_METHOD(TestTokenizerTokenizePattern) {
    std::stringstream srcStream;
    srcStream << "assign a;\n";
    srcStream << "Select a pattern a(\"normSq\", _\"cenX * cenX\"_)";
    spa::Tokenizer tokenizer;
    spa::Stream<spa::Token> tokenStream = tokenizer.tokenize(srcStream);
    bool matchResult = tokenStream.match({
        { spa::TOKEN_NAME, "assign" },
        { spa::TOKEN_NAME, "a" },
        { spa::TOKEN_SEMICOLON, ";" },
        { spa::TOKEN_NAME, "Select" },
        { spa::TOKEN_NAME, "a" },
        { spa::TOKEN_NAME, "pattern" },
        { spa::TOKEN_NAME, "a" },
        { spa::TOKEN_OPEN_BRACKET, "(" },
        { spa::TOKEN_DOUBLE_QUOTES, "\"" },
        { spa::TOKEN_NAME, "normSq" },
        { spa::TOKEN_DOUBLE_QUOTES, "\"" },
        { spa::TOKEN_COMMA, "," },
        { spa::TOKEN_UNDERSCORE, "_" },
        { spa::TOKEN_DOUBLE_QUOTES, "\"" },
        { spa::TOKEN_NAME, "cenX" },
        { spa::TOKEN_MULTIPLY, "*" },
        { spa::TOKEN_NAME, "cenX" },
        { spa::TOKEN_DOUBLE_QUOTES, "\"" },
        { spa::TOKEN_UNDERSCORE, "_" },
        { spa::TOKEN_CLOSE_BRACKET, ")" },
      });
    Assert::IsTrue(matchResult);
  }

  TEST_METHOD(TestTokenizerTokenizeSuchThat) {
    std::stringstream srcStream;
    srcStream << "variable v;\n";
    srcStream << "Select v such that Modifies (6, v)";
    spa::Tokenizer tokenizer;
    spa::Stream<spa::Token> tokenStream = tokenizer.tokenize(srcStream);
    bool matchResult = tokenStream.match({
      { spa::TOKEN_NAME, "variable" },
      { spa::TOKEN_NAME, "v" },
      { spa::TOKEN_SEMICOLON, ";" },
      { spa::TOKEN_NAME, "Select" },
      { spa::TOKEN_NAME, "v" },
      { spa::TOKEN_NAME, "such" },
      { spa::TOKEN_NAME, "that" },
      { spa::TOKEN_NAME, "Modifies" },
      { spa::TOKEN_OPEN_BRACKET, "(" },
      { spa::TOKEN_INTEGER, "6" },
      { spa::TOKEN_COMMA, "," },
      { spa::TOKEN_NAME, "v" },
      { spa::TOKEN_CLOSE_BRACKET, ")" },
      });
    Assert::IsTrue(matchResult);
  }

  TEST_METHOD(TestTokenizerTokenizeSuchThatAndPattern) {
    std::stringstream srcStream;
    srcStream << "assign a; while w;\n";
    srcStream << "Select w such that Parent* (w, a)"
      "pattern a (\"count\", _)";
    spa::Tokenizer tokenizer;
    spa::Stream<spa::Token> tokenStream = tokenizer.tokenize(srcStream);
    bool matchResult = tokenStream.match({
      { spa::TOKEN_NAME, "assign" },
      { spa::TOKEN_NAME, "a" },
      { spa::TOKEN_SEMICOLON, ";" },
      { spa::TOKEN_NAME, "while" },
      { spa::TOKEN_NAME, "w" },
      { spa::TOKEN_SEMICOLON, ";" },
      { spa::TOKEN_NAME, "Select" },
      { spa::TOKEN_NAME, "w" },
      { spa::TOKEN_NAME, "such" },
      { spa::TOKEN_NAME, "that" },
      { spa::TOKEN_NAME, "Parent" },
      { spa::TOKEN_MULTIPLY, "*" },
      { spa::TOKEN_OPEN_BRACKET, "(" },
      { spa::TOKEN_NAME, "w" },
      { spa::TOKEN_COMMA, "," },
      { spa::TOKEN_NAME, "a" },
      { spa::TOKEN_CLOSE_BRACKET, ")" },
      { spa::TOKEN_NAME, "pattern" },
      { spa::TOKEN_NAME, "a" },
      { spa::TOKEN_OPEN_BRACKET, "(" },
      { spa::TOKEN_DOUBLE_QUOTES, "\"" },
      { spa::TOKEN_NAME, "count" },
      { spa::TOKEN_DOUBLE_QUOTES, "\"" },
      { spa::TOKEN_COMMA, "," },
      { spa::TOKEN_UNDERSCORE, "_" },
      { spa::TOKEN_CLOSE_BRACKET, ")"}
      });
    Assert::IsTrue(matchResult);
  }

  TEST_METHOD(TestTokenizerTokenizeInvalidSymbol) {
    bool hasException = false;
    bool matchResult;
    try {
      std::stringstream srcStream;
      srcStream << "stmt s;\n";
      srcStream << "Select s such that Uses* (w, &)";
      spa::Tokenizer tokenizer;
      spa::Stream<spa::Token> tokenStream = tokenizer.tokenize(srcStream);
      matchResult = tokenStream.match({
        { spa::TOKEN_NAME, "stmt" },
        { spa::TOKEN_NAME, "s" },
        { spa::TOKEN_SEMICOLON, ";" },
        { spa::TOKEN_NAME, "Select" },
        { spa::TOKEN_NAME, "s" },
        { spa::TOKEN_NAME, "such" },
        { spa::TOKEN_NAME, "that" },
        { spa::TOKEN_NAME, "Uses" },
        { spa::TOKEN_MULTIPLY, "*" },
        { spa::TOKEN_OPEN_BRACKET, "(" },
        { spa::TOKEN_NAME, "w" },
        { spa::TOKEN_COMMA, "," },
        { spa::TOKEN_BOOL_AND, "&" },
        { spa::TOKEN_CLOSE_BRACKET, ")" },
        });
    }
    catch (const std::runtime_error& runTimeError) {
      hasException = true;
      std::string expectedSubMsg = "Unknown Symbol:";
      std::string errorMsg = runTimeError.what();
      bool isInMsg = errorMsg.find(expectedSubMsg) != std::string::npos;
      Assert::IsTrue(isInMsg);
    }
    catch (...) {
      hasException = true;
      Assert::Fail();
    }
    if (!hasException) {
      Assert::IsTrue(matchResult);
    }
  }

  TEST_METHOD(TestTokenizerTokenizeInvalidName) {
    bool hasException = false;
    bool matchResult;
    try {
      std::stringstream srcStream;
      srcStream << "stmt 123Test;\n";
      srcStream << "Select 123Test such that Uses* (w, v)";
      spa::Tokenizer tokenizer;
      spa::Stream<spa::Token> tokenStream = tokenizer.tokenize(srcStream);
      matchResult = tokenStream.match({
        { spa::TOKEN_NAME, "stmt" },
        { spa::TOKEN_NAME, "123Test" },
        { spa::TOKEN_SEMICOLON, ";" },
        { spa::TOKEN_NAME, "Select" },
        { spa::TOKEN_NAME, "123Test" },
        { spa::TOKEN_NAME, "s" },
        { spa::TOKEN_NAME, "such" },
        { spa::TOKEN_NAME, "that" },
        { spa::TOKEN_NAME, "Uses" },
        { spa::TOKEN_MULTIPLY, "*" },
        { spa::TOKEN_OPEN_BRACKET, "(" },
        { spa::TOKEN_NAME, "w" },
        { spa::TOKEN_COMMA, "," },
        { spa::TOKEN_NAME, "v" },
        { spa::TOKEN_CLOSE_BRACKET, ")" },
        });
    }
    catch (const std::runtime_error& runTimeError) {
      hasException = true;
      std::string expectedSubMsg = "Invalid name in SIMPLE code";
      std::string errorMsg = runTimeError.what();
      bool isInMsg = errorMsg.find(expectedSubMsg) != std::string::npos;
      Assert::IsTrue(isInMsg);
    }
    catch (...) {
      hasException = true;
      Assert::Fail();
    }
    if (!hasException) {
      Assert::IsTrue(matchResult);
    }
  }
  };
}  // namespace UnitTesting

#include <string>
#include <unordered_set>
#include <iostream>
#include <vector>
#include "literal.h"
#include "PKB.h"
#include "SpValidator.h"
#include "Stream.h"
#include "UtilsFunction.h"

const std::string INVALID_SRC_CODE_ERROR = "Invalid Source Code: ";
const std::string NOT_STARTING_WITH_PROC_ERROR = "Program should start with procedure";
const std::string NO_PROC_NAME_ERROR = "Procedure needs a PROC_NAME";
const std::string NON_UNIQ_PROC_NAME_ERROR = "Unique PROC_NAME needed";
const std::string PROC_NO_OPEN_BRACE_ERROR = "Procedure name needs to be followed by: '{'";
const std::string PROC_NO_CLOSE_BRACE_ERROR = "Procedure needs to end with : '}'";
const std::string EMPTY_STMT_ERROR = "No stmt in stmtLst";
const std::string UNKNOWN_STMT_ERROR = "Unknown stmt";
const std::string INCOMPLETE_STMT_ERROR = "Stmt not completed";
const std::string INVALID_STMT_ERROR = "Invalid Stmt";
const std::string INVALID_ASSIGN_ERROR = "Invalid assign expression";
const std::string MISSING_SEMICOLON_ERROR = "Read, Print, Call, Assign stmt must end with ';'";
const std::string NON_EXISTENT_PROC_CALL_ERROR = "Calling non-existing procedure";
const std::string MISSING_WHILE_CLOSE_BRACKET_ERROR = "Missing closing ')' for WHILE after COND_EXPR";
const std::string MISSING_WHILE_OPEN_BRACE_ERROR = "Missing '{' for WHILE after COND_EXPR";
const std::string MISSING_WHILE_CLOSE_BRACE_ERROR = "Missing '}' for WHILE after STMTLST";
const std::string MISSING_IF_CLOSE_BRACKET_ERROR = "Missing ')' for IF after COND_EXPR";
const std::string MISSING_THEN_ERROR = "Missing THEN for IF";
const std::string MISSING_THEN_OPEN_BRACE_ERROR = "Missing '{' for THEN clause";
const std::string MISSING_THEN_CLOSE_BRACE_ERROR = "Missing '}' for THEN clause";
const std::string MISSING_ELSE_ERROR = "Missing ELSE for IF";
const std::string MISSING_ELSE_OPEN_BRACE_ERROR = "Missing '{' for ELSE clause";
const std::string MISSING_ELSE_CLOSE_BRACE_ERROR = "Missing '}' for ELSE clause";
const std::string INVALID_COND_EXPR_ERROR = "Invalid cond_expr";
const std::string MISSING_OPEN_BRACKET_REL_ERROR = "&&|| Missing an opening '('";

spa::SpValidator::SpValidator(Stream<Token> t) : tokens(t) {}

bool spa::SpValidator::validateGrammar() {
  try {
    while (hasRemaining()) {
      Token currToken = getToken();
      if (currToken.getValue() != PROCEDURE_LITERAL) {
        throw std::exception(NOT_STARTING_WITH_PROC_ERROR.data());
      }
      tokens[idx - 1] = Token(TOKEN_PROCEDURE, PROCEDURE_LITERAL);
      validateProcedure();
    }
    validateCallExists();
    return true;
  } catch (std::exception& e) {
    std::cerr << INVALID_SRC_CODE_ERROR;
    std::cerr << e.what() << std::endl;
    return false;
  }
}

bool spa::SpValidator::hasRemaining() {
  const int64_t streamSize = tokens.remaining();
  return idx < streamSize;
}

bool spa::SpValidator::hasRemaining(int64_t offset) {
  const int64_t streamSize = tokens.remaining();
  return idx + offset < streamSize;
}

void spa::SpValidator::next(int64_t offset) {
  idx = idx + offset;
}

spa::Token spa::SpValidator::getToken() {
  return tokens[idx++];
}

spa::Token spa::SpValidator::peekNextToken(int64_t offset) {
  return tokens[idx + offset];
}

void spa::SpValidator::validateProcedure() {
  Token currToken = getToken();
  TokenType currTokenType = currToken.getType();

  if (!hasRemaining() || currTokenType != TOKEN_NAME) {
    throw std::exception(NO_PROC_NAME_ERROR.data());
  }

  std::string currTokenValue = currToken.getValue();
  if (procNames.count(currTokenValue)) {
    throw std::exception(NON_UNIQ_PROC_NAME_ERROR.data());
  }

  procNames.insert(currTokenValue);
  checkValidBraces(PROC_NO_OPEN_BRACE_ERROR);
  validateStmtLst();
  checkValidBraces(PROC_NO_CLOSE_BRACE_ERROR, false);
}

void spa::SpValidator::validateStmtLst() {
  bool hasOneOrMoreStatement = false;
  while (hasRemaining()) {
    Token token = peekNextToken();
    TokenType tokenType = token.getType();
    std::string tokenValue = token.getValue();

    if (hasOneOrMoreStatement && tokenType == TOKEN_CLOSE_BRACE) {
      return;
    } else if (!hasOneOrMoreStatement && tokenType == TOKEN_CLOSE_BRACE) {
      throw std::exception(EMPTY_STMT_ERROR.data());
    }

    if (tokenType != TOKEN_NAME) {
      throw std::exception(UNKNOWN_STMT_ERROR.data());
    }
    hasOneOrMoreStatement = true;

    validateStmt();
  }
}

void spa::SpValidator::validateStmt() {
  if (!hasRemaining(1)) {
    throw std::exception(INCOMPLETE_STMT_ERROR.data());
  }

  Token token = peekNextToken(1);
  TokenType tokenType = token.getType();
  switch (tokenType) {
  case TOKEN_NAME:
    validateReadPrintCall();
    break;
  case TOKEN_OPEN_BRACKET:
    validateWhileIf();
    break;
  case TOKEN_EQUAL:
    validateEqual();
    break;
  default:
    throw std::exception(INVALID_STMT_ERROR.data());
  }
}

void spa::SpValidator::validateEqual() {
  next(2);
  std::vector<Token> tokensToCheck;
  while (hasRemaining()) {
    const bool isSemiColon = peekNextToken().getType() == TOKEN_SEMICOLON;
    if (isSemiColon) {
      break;
    }
    tokensToCheck.push_back(getToken());
  }

  const bool isValidExpr = UtilsFunction::isValidExpr(tokensToCheck);
  if (!isValidExpr) {
    throw std::exception(INVALID_ASSIGN_ERROR.data());
  }

  if (!hasRemaining() || getToken().getType() != TOKEN_SEMICOLON) {
    throw std::exception(MISSING_SEMICOLON_ERROR.data());
  }
}

void spa::SpValidator::validateReadPrintCall() {
  Token currToken = getToken();
  std::string currTokenValue = currToken.getValue();
  if (currTokenValue == READ_LITERAL) {
    tokens[idx - 1] = Token(TOKEN_READ, READ_LITERAL);
  } else if (currTokenValue == PRINT_LITERAL) {
    tokens[idx - 1] = Token(TOKEN_PRINT, PRINT_LITERAL);
  } else if (currTokenValue == CALL_LITERAL) {
    tokens[idx - 1] = Token(TOKEN_CALL, CALL_LITERAL);
    std::string callValue = peekNextToken(0).getValue();
    callNames.insert(callValue);
  } else {
    throw std::exception(UNKNOWN_STMT_ERROR.data());
  }

  next();

  if (!hasRemaining() || getToken().getType() != TOKEN_SEMICOLON) {
    throw std::exception(MISSING_SEMICOLON_ERROR.data());
  }
}

void spa::SpValidator::validateCallExists() {
  for (const auto& callName : callNames) {
    if (procNames.count(callName) == 0) {
      throw std::exception(NON_EXISTENT_PROC_CALL_ERROR.data());
    }
  }
}

void spa::SpValidator::validateWhileIf() {
  std::string nextTokenValue = peekNextToken().getValue();
  if (nextTokenValue == IF_LITERAL) {
    tokens[idx] = Token(TOKEN_IF, nextTokenValue);
    validateIf();
  } else if (nextTokenValue == WHILE_LITERAL) {
    tokens[idx] = Token(TOKEN_WHILE, nextTokenValue);
    validateWhile();
  } else {
    throw std::exception(UNKNOWN_STMT_ERROR.data());
  }
}


void spa::SpValidator::validateWhile() {
  next(2);

  validateCondExpr();

  checkValidBrackets(MISSING_WHILE_CLOSE_BRACKET_ERROR, false);

  checkValidBraces(MISSING_WHILE_OPEN_BRACE_ERROR);
  validateStmtLst();
  checkValidBraces(MISSING_WHILE_CLOSE_BRACE_ERROR, false);
}

void spa::SpValidator::validateIf() {
  next(2);

  validateCondExpr();
  checkValidBrackets(MISSING_IF_CLOSE_BRACKET_ERROR, false);

  if (!hasRemaining() || getToken().getValue() != THEN_LITERAL) {
    throw std::exception(MISSING_THEN_ERROR.data());
  }

  tokens[idx - 1] = Token(TOKEN_THEN, THEN_LITERAL);
  checkValidBraces(MISSING_THEN_OPEN_BRACE_ERROR);
  validateStmtLst();
  checkValidBraces(MISSING_THEN_CLOSE_BRACE_ERROR, false);

  if (!hasRemaining() || getToken().getValue() != ELSE_LITERAL) {
    throw std::exception(MISSING_ELSE_ERROR.data());
  }

  tokens[idx - 1] = Token(TOKEN_ELSE, ELSE_LITERAL);
  checkValidBraces(MISSING_ELSE_OPEN_BRACE_ERROR);
  validateStmtLst();
  checkValidBraces(MISSING_ELSE_CLOSE_BRACE_ERROR, false);
}

void spa::SpValidator::checkValidBraces(std::string errorMessage, bool isOpenBrace) {
  if (isOpenBrace) {
    if (!hasRemaining() || !UtilsFunction::isValidOpenBrace(getToken())) {
      throw std::exception(MISSING_ELSE_OPEN_BRACE_ERROR.data());
    }
    return;
  }

  if (!hasRemaining() || !UtilsFunction::isValidCloseBrace(getToken())) {
    throw std::exception(errorMessage.data());
  }
}

void spa::SpValidator::checkValidBrackets(std::string errorMessage, bool isOpenBracket) {
  if (isOpenBracket) {
    if (!hasRemaining() || !UtilsFunction::isValidOpenBracket(getToken())) {
      throw std::exception(MISSING_ELSE_OPEN_BRACE_ERROR.data());
    }
    return;
  }

  if (!hasRemaining() || !UtilsFunction::isValidCloseBracket(getToken())) {
    throw std::exception(errorMessage.data());
  }
}

void spa::SpValidator::validateCondExpr() {
  std::vector<Token> tokensToCheck;
  int unclosedBracketCount = 0;
  while (hasRemaining() && (unclosedBracketCount != 0 || !UtilsFunction::isValidCloseBracket(
    peekNextToken()))) {
    const TokenType tokenType = peekNextToken().getType();
    if (tokenType == TOKEN_OPEN_BRACKET) {
      unclosedBracketCount += 1;
    } else if (tokenType == TOKEN_CLOSE_BRACKET) {
      unclosedBracketCount -= 1;
    }

    tokensToCheck.push_back(getToken());
  }

  const bool isValidCondExpr = isCondExpr(tokensToCheck);
  if (!isValidCondExpr) {
    throw std::exception(INVALID_COND_EXPR_ERROR.data());
  }
}

bool spa::SpValidator::isCondExpr(std::vector<Token> tokensToCheck) {
  if (tokensToCheck.empty()) {
    return false;
  }

  Token firstToken = tokensToCheck[0];
  if (tokensToCheck.size() == 1 && UtilsFunction::isValidRelFactorToken(firstToken)) {
    return true;
  }

  Token secondToken = tokensToCheck[1];
  if (tokensToCheck.size() >= 3 && firstToken.getType() == TOKEN_BOOL_NOT && UtilsFunction::isValidOpenBracket(
    secondToken)
    && UtilsFunction::isValidCloseBracket(tokensToCheck.back())) {
    return isCondExpr(
      std::vector<Token>(tokensToCheck.begin() + 2, tokensToCheck.end() - 1));
  }

  if (tokensToCheck.size() < 5 || !UtilsFunction::isValidOpenBracket(tokensToCheck.front()) ||
      !UtilsFunction::isValidCloseBracket(tokensToCheck.back())) {
    return isRelExpr(tokensToCheck);
  }

  int unclosedBracketCount = 0;
  int index = -1;
  for (unsigned i = 0; i < tokensToCheck.size(); i++) {
    const TokenType tokenType = tokensToCheck[i].getType();
    if (tokenType == TOKEN_OPEN_BRACKET) {
      unclosedBracketCount += 1;
    } else if (tokenType == TOKEN_CLOSE_BRACKET) {
      unclosedBracketCount -= 1;
    }

    if (UtilsFunction::isValidCondExprToken(tokensToCheck[i]) && unclosedBracketCount == 0) {
      index = i;
      break;
    }
  }

  if (index == -1) {
    return isRelExpr(tokensToCheck);
  }

  if (tokensToCheck[index + 1].getType() != TOKEN_OPEN_BRACKET) {
    throw std::exception(MISSING_OPEN_BRACKET_REL_ERROR.data());
  }


  std::vector<Token> left(tokensToCheck.begin() + 1, tokensToCheck.begin() + index - 1);
  std::vector<Token> right(tokensToCheck.begin() + index + 2, tokensToCheck.end() - 1);

  return isCondExpr(left) && isCondExpr(right);
}

bool spa::SpValidator::isRelExpr(std::vector<Token> tokensToCheck) {
  const bool isValidRelExpr = tokensToCheck.size() >= 3;
  if (!isValidRelExpr) {
    return false;
  }

  int index = -1;
  for (unsigned i = 0; i < tokensToCheck.size(); i++) {
    if (UtilsFunction::relExprToken.count(tokensToCheck[i].getType())) {
      index = i;
      break;
    }
  }

  if (index == -1) {
    return false;
  }

  std::vector<Token> left(tokensToCheck.begin(), tokensToCheck.begin() + index);
  std::vector<Token> right(tokensToCheck.begin() + index + 1, tokensToCheck.end());

  return isRelFactor(left) && isRelFactor(right);
}

bool spa::SpValidator::isRelFactor(std::vector<Token> tokensToCheck) {
  if (tokensToCheck.size() == 1) {
    const Token token = tokensToCheck[0];
    return UtilsFunction::isValidRelFactorToken(token);
  }

  return UtilsFunction::isValidExpr(tokensToCheck);
}

spa::Stream<spa::Token> spa::SpValidator::getUpdatedStream() {
  return tokens;
}

#include <string>
#include <unordered_set>
#include <iostream>
#include <vector>
#include "SpValidator.h"
#include "Stream.h"
#include "PKB.h"
#include "UtilsFunction.h"

spa::SpValidator::SpValidator(Stream<Token> t) : tokens(t) {}

bool spa::SpValidator::validateGrammar() {
  try {
    while (hasRemaining()) {
      Token currToken = getToken();
      if (currToken.getValue() != "procedure") {
        throw std::exception("Program should start with procedure");
      }
      tokens[idx - 1] = Token(TOKEN_PROCEDURE, "procedure");
      validateProcedure();
    }
    validateCallExists();
    return true;
  } catch (std::exception& e) {
    std::cerr << "Invalid Source Code: ";
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


// procedure: 'procedure' proc_name '{' stmtLst '}'
void spa::SpValidator::validateProcedure() {
  Token currToken = getToken();
  TokenType currTokenType = currToken.getType();

  if (!hasRemaining() || currTokenType != TOKEN_NAME) {
    throw std::exception("Procedure needs a PROC_NAME");
  }

  std::string currTokenValue = currToken.getValue();
  if (procNames.count(currTokenValue)) {
    throw std::exception("Unique PROC_NAME needed");
  }

  procNames.insert(currTokenValue);
  currToken = getToken();
  if (!hasRemaining() || !UtilsFunction::isValidOpenBrace(currToken)) {
    throw std::exception("Procedure name needs to be followed by: '{'");
  }

  validateStmtLst();

  if (!hasRemaining() || !UtilsFunction::isValidCloseBrace(getToken())) {
    throw std::exception("Procedure needs to end with : '}'");
  }
}

// stmtLst: stmt +
// stmt : read | print | call | while | if | assign
void spa::SpValidator::validateStmtLst() {
  bool hasOneOrMoreStatement = false;
  while (hasRemaining()) {
    Token token = peekNextToken();
    TokenType tokenType = token.getType();
    std::string tokenValue = token.getValue();

    if (hasOneOrMoreStatement && tokenType == TOKEN_CLOSE_BRACE) {
      return;  // Empty proc
    } else if (!hasOneOrMoreStatement && tokenType == TOKEN_CLOSE_BRACE) {
      throw std::exception("No stmt in stmtLst");
    }

    if (tokenType != TOKEN_NAME) {
      throw std::exception("Unknown stmt");
    }
    hasOneOrMoreStatement = true;

    validateStmt();
  }
}

void spa::SpValidator::validateStmt() {
  if (!hasRemaining(1)) {
    throw std::exception("Stmt not completed");
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
      throw std::exception("Invalid Stmt");
  }
}

// assign: var_name '=' tokensToCheck ';'
void spa::SpValidator::validateEqual() {
  next(2);  // Already checked NAME + EQUAL
  std::vector<Token> tokensToCheck;
  while (hasRemaining()) {
    const bool isSemiColon = peekNextToken().getType() == TOKEN_SEMICOLON;
    if (isSemiColon) {
      break;
    }
    tokensToCheck.push_back(getToken());
  }

  // Need to validate the tokensToCheck
  const bool isValidExpr = UtilsFunction::isValidExpr(tokensToCheck);
  if (!isValidExpr) {
    throw std::exception("Invalid assign expression");
  }

  if (!hasRemaining() || getToken().getType() != TOKEN_SEMICOLON) {
    throw std::exception("Assign must end with ';'");
  }
}

// print: 'print' var_name';'
// read: 'read' var_name';'
void spa::SpValidator::validateReadPrintCall() {
  Token currToken = getToken();
  std::string currTokenValue = currToken.getValue();
  if (currTokenValue == "read") {
    tokens[idx - 1] = Token(TOKEN_READ, "read");
  } else if (currTokenValue == "print") {
    tokens[idx - 1] = Token(TOKEN_PRINT, "print");
  } else if (currTokenValue == "call") {
    tokens[idx - 1] = Token(TOKEN_CALL, "call");
    std::string callValue = peekNextToken(0).getValue();
    callNames.insert(callValue);
  } else {
    throw std::exception("Unknown Statement");
  }

  next();  // var_name

  // Check that stmt is closed with ;
  if (!hasRemaining() || getToken().getType() != TOKEN_SEMICOLON) {
    throw std::exception("Read, Print stmt must end with ';'");
  }
}

void spa::SpValidator::validateCallExists() {
  for (const auto& callName : callNames) {
    if (procNames.count(callName) == 0) {
      throw std::exception("Calling non-existing procedure");
    }
  }
}

void spa::SpValidator::validateWhileIf() {
  std::string nextTokenValue = peekNextToken().getValue();
  if (nextTokenValue == "if") {
    tokens[idx] = Token(TOKEN_IF, nextTokenValue);
    validateIf();
  } else if (nextTokenValue == "while") {
    tokens[idx] = Token(TOKEN_WHILE, nextTokenValue);
    validateWhile();
  } else {
    throw std::exception("Unknown stmt");
  }
}


// while: 'while' '(' cond_expr ')' '{' stmtLst '}'
void spa::SpValidator::validateWhile() {
  next(2);  // Already checked WHILE + (

  validateCondExpr();

  if (!hasRemaining() || !UtilsFunction::isValidCloseBracket(getToken())) {
    throw std::exception("Missing closing ')' for WHILE after COND_EXPR");
  }

  if (!hasRemaining() || !UtilsFunction::isValidOpenBrace(getToken())) {
    throw std::exception("Missing '{' for WHILE after COND_EXPR");
  }

  validateStmtLst();

  if (!hasRemaining() || !UtilsFunction::isValidCloseBrace(getToken())) {
    throw std::exception("Missing '}' for WHILE after STMTLST");
  }
}

// if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
void spa::SpValidator::validateIf() {
  next(2);  // Already checked IF + (

  validateCondExpr();

  if (!hasRemaining() || !UtilsFunction::isValidCloseBracket(getToken())) {
    throw std::exception("Missing ')' for IF after COND_EXPR");
  }

  if (!hasRemaining() || getToken().getValue() != "then") {
    throw std::exception("Missing THEN for IF");
  }

  tokens[idx - 1] = Token(TOKEN_THEN, "then");

  if (!hasRemaining() || !UtilsFunction::isValidOpenBrace(getToken())) {
    throw std::exception("Missing '{' for THEN clause");
  }

  validateStmtLst();

  if (!hasRemaining() || !UtilsFunction::isValidCloseBrace(getToken())) {
    throw std::exception("Missing '}' for THEN clause");
  }

  if (!hasRemaining() || getToken().getValue() != "else") {
    throw std::exception("Missing ELSE for IF");
  }

  tokens[idx - 1] = Token(TOKEN_ELSE, "else");

  if (!hasRemaining() || !UtilsFunction::isValidOpenBrace(getToken())) {
    throw std::exception("Missing '{' for ELSE clause");
  }

  validateStmtLst();

  if (!hasRemaining() || !UtilsFunction::isValidCloseBrace(getToken())) {
    throw std::exception("Missing '}' for ELSE clause");
  }
}

// cond_expr: rel_expr | '!' '(' cond_expr ')' |
// '(' cond_expr ')' '&&' '(' cond_expr ')' |
// '(' cond_expr ')' '||' '(' cond_expr ')'
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
    throw std::exception("Invalid cond_expr");
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
  // Case 1: '!' '(' cond_expr ')'
  if (tokensToCheck.size() >= 3 && firstToken.getType() == TOKEN_BOOL_NOT &&
    UtilsFunction::isValidOpenBracket(
      secondToken)
    && UtilsFunction::isValidCloseBracket(tokensToCheck.back())) {
    return isCondExpr(
      std::vector<Token>(tokensToCheck.begin() + 2, tokensToCheck.end() - 1));
  }

  // Case 2: | '(' cond_expr ')' '||&&' '(' cond_expr ')'
  if (tokensToCheck.size() < 5 || !UtilsFunction::isValidOpenBracket(tokensToCheck.front()) ||
    !UtilsFunction::isValidCloseBracket(tokensToCheck.back())) {
    return isRelExpr(tokensToCheck);
  }

  // Case 3: rel_expr
  // Find outermost || &&
  int unclosedBracketCount = 0;
  int index = -1;
  for (int i = 0; i < tokensToCheck.size(); i++) {
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
    throw std::exception("&&|| Missing an opening '('");
  }


  std::vector<Token> left(tokensToCheck.begin() + 1, tokensToCheck.begin() + index - 1);
  std::vector<Token> right(tokensToCheck.begin() + index + 2, tokensToCheck.end() - 1);

  return isCondExpr(left) && isCondExpr(right);
}

// rel_expr: rel_factor 'COND_OP' rel_factor
bool spa::SpValidator::isRelExpr(std::vector<Token> tokensToCheck) {
  const bool isValidRelExpr = tokensToCheck.size() >= 3;
  if (!isValidRelExpr) {
    return false;
  }

  int index = -1;
  for (int i = 0; i < tokensToCheck.size(); i++) {
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

// check for var_name | const_value
bool spa::SpValidator::isRelFactor(std::vector<Token> tokensToCheck) {
  // Short-hand checking
  if (tokensToCheck.size() == 1) {
    const Token token = tokensToCheck[0];
    return UtilsFunction::isValidRelFactorToken(token);
  }

  return UtilsFunction::isValidExpr(tokensToCheck);
}

spa::Stream<spa::Token> spa::SpValidator::getUpdatedStream() {
  return tokens;
}

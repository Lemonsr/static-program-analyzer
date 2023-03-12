#include "SpParser.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "ContainerStatement.h"
#include "NonContainerStatement.h"
#include "ProcedureStatement.h"
#include "ProgramStatement.h"
#include "UtilsFunction.h"

// Constructor for SpParser
spa::SpParser::SpParser(spa::Stream<spa::Token>& tokenStream) : tokenStream(tokenStream) {}

std::vector<std::shared_ptr<spa::ProcedureStatement>> spa::SpParser::parse() {
  std::vector<std::shared_ptr<ProcedureStatement>> procedureList;
  while (!isEndOfProgram()) {
    if (matchToken(spa::TOKEN_PROCEDURE)) {
      procedureList.push_back(processProcedure());
    }
  }
  return procedureList;
}

std::shared_ptr<spa::ProcedureStatement> spa::SpParser::processProcedure() {
  skipCurrToken();  // To skip over the procedure token
  Token procedureVarToken = getCurrTokenAndAdvance();
  skipCurrToken();  // Skip over open brace token
  std::unordered_set<int> whileStmtParents;
  std::unordered_set<int> ifStmtParents;
  std::vector<std::shared_ptr<ProgramStatement>> statementLst = processStmtList(
    procedureVarToken.getValue(),
    whileStmtParents,
    ifStmtParents);
  auto procedure = std::make_shared<ProcedureStatement>(procedureVarToken, statementLst);
  return procedure;
}

std::vector<std::shared_ptr<spa::ProgramStatement>> spa::SpParser::processStmtList(
  std::string parentProcedureVal,
  std::unordered_set<int> whileStmtParents,
  std::unordered_set<int> ifStmtParents) {
  std::vector<std::shared_ptr<ProgramStatement>> statements;
  while (!matchToken(spa::TOKEN_CLOSE_BRACE) && !isEndOfProgram()) {
    statements.push_back(handleStatements(parentProcedureVal, whileStmtParents, ifStmtParents));
  }
  skipCurrToken();  // Skip over close brace token
  return statements;
}

std::shared_ptr<spa::ProgramStatement> spa::SpParser::handleStatements(
  std::string parentProcedureVal,
  std::unordered_set<int> whileStmtParents,
  std::unordered_set<int> ifStmtParents) {
  TokenType currentTokenType = getCurrTokenAndAdvance().getType();
  switch (currentTokenType) {
    case TOKEN_READ:
      return processReadStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    case TOKEN_PRINT:
      return processPrintStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    case TOKEN_CALL:
      return processCallStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    case TOKEN_WHILE:
      return processWhileStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    case TOKEN_IF:
      return processIfStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    default:
      return processAssignStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
  }
}

std::shared_ptr<spa::ProgramStatement> spa::SpParser::processReadStatement(
  std::string parentProcedureVal,
  std::unordered_set<int> whileStmtParents,
  std::unordered_set<int> ifStmtParents) {
  Token readVariable = getCurrTokenAndAdvance();
  skipCurrToken();  // Skip over semi colon token
  auto readStatement = std::make_shared<ReadStatement>(parentProcedureVal,
    readVariable.getValue(),
    whileStmtParents,
    ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return readStatement;
}

std::shared_ptr<spa::ProgramStatement> spa::SpParser::processPrintStatement(
  std::string parentProcedureVal,
  std::unordered_set<int> whileStmtParents,
  std::unordered_set<int> ifStmtParents) {
  Token printVariable = getCurrTokenAndAdvance();
  skipCurrToken();  // Skip over semi colon token
  auto printStatement = std::make_shared<PrintStatement>(parentProcedureVal,
    printVariable.getValue(),
    whileStmtParents,
    ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return printStatement;
}

std::shared_ptr<spa::ProgramStatement> spa::SpParser::processCallStatement(
  std::string parentProcedureVal,
  std::unordered_set<int> whileStmtParents,
  std::unordered_set<int> ifStmtParents) {
  Token callVariable = getCurrTokenAndAdvance();
  skipCurrToken();  // Skip over semi colon token
  auto callStatement = std::make_shared<CallStatement>(parentProcedureVal,
    callVariable.getValue(),
    whileStmtParents,
    ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return callStatement;
}

std::shared_ptr<spa::ProgramStatement> spa::SpParser::processWhileStatement(
  std::string parentProcedureVal,
  std::unordered_set<int>
  whileStmtParents,
  std::unordered_set<int> ifStmtParents) {
  int currentLineNum = statementLineNum;
  whileStmtParents.insert(currentLineNum);
  std::vector<std::shared_ptr<ProgramStatement>> whileStatementBlock{};
  std::shared_ptr<ProgramStatement> whileConditionStatement = processWhileConditionStatement(
    parentProcedureVal,
    whileStmtParents,
    ifStmtParents);
  whileStatementBlock.push_back(whileConditionStatement);
  skipCurrToken();  // Skip over open brace token
  std::vector<std::shared_ptr<ProgramStatement>> whileStatementList = processStmtList(
    parentProcedureVal,
    whileStmtParents,
    ifStmtParents);
  std::shared_ptr<ProgramStatement> whileInnerBlockStatement = std::make_shared<
    InnerBlockStatement>(
    parentProcedureVal, whileStatementList);
  whileStatementBlock.push_back(whileInnerBlockStatement);
  std::shared_ptr<ProgramStatement> whileContainerStatement = std::make_shared<
    WhileContainerStatement>(parentProcedureVal,
    currentLineNum, whileStatementBlock);
  return whileContainerStatement;
}

std::shared_ptr<spa::ProgramStatement> spa::SpParser::processWhileConditionStatement(
  std::string parentProcedureVal,
  std::unordered_set<int> whileStmtParents,
  std::unordered_set<int> ifStmtParents) {
  std::vector<spa::Token> rawConditionExpression{};
  while (getCurrToken().getType() != spa::TOKEN_OPEN_BRACE) {
    spa::Token currToken = getCurrTokenAndAdvance();
    rawConditionExpression.push_back(currToken);
  }
  std::string postfixExpression = UtilsFunction::infixToPostfix(rawConditionExpression);
  std::shared_ptr<ProgramStatement> whileConditionStatement = std::make_shared<
    WhileConditionStatement>(parentProcedureVal,
    postfixExpression,
    whileStmtParents, ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return whileConditionStatement;
}

std::shared_ptr<spa::ProgramStatement> spa::SpParser::processIfStatement(
  std::string parentProcedureVal,
  std::unordered_set<int> whileStmtParents,
  std::unordered_set<int> ifStmtParents) {
  int currentLineNum = statementLineNum;
  ifStmtParents.insert(currentLineNum);
  std::vector<std::shared_ptr<ProgramStatement>> ifStatementBlock{};
  std::shared_ptr<ProgramStatement> ifConditionStatement = processIfConditionStatement(
    parentProcedureVal,
    whileStmtParents,
    ifStmtParents);
  ifStatementBlock.push_back(ifConditionStatement);
  skipCurrToken();  // Skip over then token
  skipCurrToken();  // Skip over open brace token
  std::vector<std::shared_ptr<ProgramStatement>> thenStatementList = processStmtList(
    parentProcedureVal,
    whileStmtParents,
    ifStmtParents);
  std::shared_ptr<ProgramStatement> thenStatementInnerBlock = std::make_shared<InnerBlockStatement>(
    parentProcedureVal, thenStatementList);
  ifStatementBlock.push_back(thenStatementInnerBlock);
  skipCurrToken();  // Skip over else token
  skipCurrToken();  // Skip over open brace token
  std::vector<std::shared_ptr<ProgramStatement>> elseStatementList = processStmtList(
    parentProcedureVal,
    whileStmtParents,
    ifStmtParents);
  std::shared_ptr<ProgramStatement> elseStatementInnerBlock = std::make_shared<InnerBlockStatement>(
    parentProcedureVal, elseStatementList);
  ifStatementBlock.push_back(elseStatementInnerBlock);
  std::shared_ptr<ProgramStatement> ifContainerStatement = std::make_shared<IfContainerStatement>(
    parentProcedureVal,
    currentLineNum, ifStatementBlock);
  return ifContainerStatement;
}

std::shared_ptr<spa::ProgramStatement> spa::SpParser::processIfConditionStatement(
  std::string parentProcedureVal,
  std::unordered_set<int> whileStmtParents,
  std::unordered_set<int> ifStmtParents) {
  std::vector<spa::Token> rawConditionExpression{};
  while (getCurrToken().getType() != spa::TOKEN_THEN) {
    spa::Token currToken = getCurrTokenAndAdvance();
    rawConditionExpression.push_back(currToken);
  }
  std::string postfixExpression = UtilsFunction::infixToPostfix(rawConditionExpression);
  std::shared_ptr<ProgramStatement> ifConditionStatement = std::make_shared<IfConditionStatement>(
    parentProcedureVal, postfixExpression,
    whileStmtParents, ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return ifConditionStatement;
}

std::shared_ptr<spa::ProgramStatement> spa::SpParser::processAssignStatement(
  std::string parentProcedureVal,
  std::unordered_set<int>
  whileStmtParents,
  std::unordered_set<int>
  ifStmtParents) {
  std::string assignmentVar = getPrevToken().getValue();
  skipCurrToken();  // Skip over equal token
  std::vector<spa::Token> rawAssignExpression = {};
  while (getCurrToken().getType() != spa::TOKEN_SEMICOLON) {
    spa::Token currToken = getCurrTokenAndAdvance();
    rawAssignExpression.push_back(currToken);
  }
  skipCurrToken();  // Skip over semi colon token
  std::string postfixExpression = UtilsFunction::infixToPostfix(rawAssignExpression);
  std::shared_ptr<ProgramStatement> assignStatement = std::make_shared<AssignStatement>(
    parentProcedureVal, assignmentVar,
    postfixExpression,
    whileStmtParents, ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return assignStatement;
}

// Private functions to traverse the stream of tokens
void spa::SpParser::advanceStream(int64_t offset) {
  tokenStream.seek(offset);
}

void spa::SpParser::skipCurrToken() {
  advanceStream(1);
}

bool spa::SpParser::matchToken(spa::TokenType tokenType) {
  if (isEndOfProgram()) {
    return false;
  }
  spa::TokenType nextTokenType = getCurrToken().getType();
  return nextTokenType == tokenType;
}

spa::Token spa::SpParser::getCurrToken() {
  return tokenStream[static_cast<int64_t>(0)];
}

spa::Token spa::SpParser::getPrevToken() {
  return tokenStream[static_cast<int64_t>(-1)];
}

spa::Token spa::SpParser::getCurrTokenAndAdvance() {
  spa::Token currToken = getCurrToken();
  advanceStream(1);
  return currToken;
}

bool spa::SpParser::isEndOfProgram() {
  if (tokenStream.remaining() == 0) {
    return true;
  }
  return false;
}

void spa::SpParser::increaseStatementLineNum() {
  statementLineNum++;
}

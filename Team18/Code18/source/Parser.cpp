#include <vector>
#include <unordered_set>

#include "Parser.h"

#include "ContainerStatement.h"
#include "NonContainerStatement.h"
#include "Statement.h"
#include "Procedure.h"
#include "UtilsFunction.h"

//Constructor for Parser
spa::Parser::Parser(spa::Stream<spa::Token>& tokenStream) : tokenStream(tokenStream) {}

std::vector<spa::Procedure> spa::Parser::parse() {
  std::vector<Procedure> procedureList;
  while (!isEndOfProgram()) {
    if (matchToken(spa::TOKEN_PROCEDURE)) {
      procedureList.push_back(processProcedure());
    }
  }
  return procedureList;
}

spa::Procedure spa::Parser::processProcedure() {
  skipCurrToken(); // To skip over the procedure token
  spa::Token procedureVarToken = getCurrTokenAndAdvance();
  skipCurrToken(); // Skip over open brace token
  std::unordered_set<int> whileStmtParents;
  std::unordered_set<int> ifStmtParents;
  std::vector<Statement*> statementLst = processStmtList(procedureVarToken.getValue(), whileStmtParents,
    ifStmtParents);
  Procedure procedure = Procedure(procedureVarToken, statementLst);
  return procedure;
}

std::vector<spa::Statement*> spa::Parser::processStmtList(std::string parentProcedureVal,
                                                          std::unordered_set<int> whileStmtParents,
                                                          std::unordered_set<int> ifStmtParents) {
  std::vector<Statement*> statements;
  while (!matchToken(spa::TOKEN_CLOSE_BRACE) && !isEndOfProgram()) {
    statements.push_back(handleStatements(parentProcedureVal, whileStmtParents, ifStmtParents));
  }
  skipCurrToken(); // Skip over close brace token
  return statements;
}

//TODO To replace all the TOKEN_OPEN_BRACE with if, while, call, read, print, assign stmt tokens
spa::Statement* spa::Parser::handleStatements(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                              std::unordered_set<int> ifStmtParents) {
  spa::TokenType currentTokenType = getCurrTokenAndAdvance().getType();
  switch (currentTokenType) {
    case spa::TOKEN_READ:
      return processReadStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    case spa::TOKEN_PRINT:
      return processPrintStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    case spa::TOKEN_CALL:
      return processCallStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    case spa::TOKEN_WHILE:
      return processWhileStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    case spa::TOKEN_IF:
      return processIfStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    default:
      return processAssignStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
  }
}


spa::Statement* spa::Parser::processReadStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                                  std::unordered_set<int> ifStmtParents) {
  spa::Token readVariable = getCurrTokenAndAdvance();
  skipCurrToken(); // Skip over semi colon token
  Statement* readStatement = new ReadStatement(parentProcedureVal, readVariable.getValue(), whileStmtParents,
    ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return readStatement;
}

spa::Statement* spa::Parser::processPrintStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                                   std::unordered_set<int> ifStmtParents) {
  spa::Token printVariable = getCurrTokenAndAdvance();
  skipCurrToken(); // Skip over semi colon token
  Statement* printStatement = new PrintStatement(parentProcedureVal, printVariable.getValue(), whileStmtParents,
    ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return printStatement;
}

spa::Statement* spa::Parser::processCallStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                                  std::unordered_set<int> ifStmtParents) {
  spa::Token callVariable = getCurrTokenAndAdvance();
  skipCurrToken(); // Skip over semi colon token
  Statement* callStatement = new CallStatement(parentProcedureVal, callVariable.getValue(), whileStmtParents,
    ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return callStatement;
}

spa::Statement* spa::Parser::processWhileStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                                   std::unordered_set<int> ifStmtParents) {
  int currentLineNum = statementLineNum;
  whileStmtParents.insert(currentLineNum);
  std::vector<Statement*> whileStatementBlock{};
  Statement* whileConditionStatement = processWhileConditionStatement(parentProcedureVal, whileStmtParents,
    ifStmtParents);
  whileStatementBlock.push_back(whileConditionStatement);
  skipCurrToken(); // Skip over open brace token
  std::vector<Statement*> whileStatementList = processStmtList(parentProcedureVal, whileStmtParents, ifStmtParents);
  Statement* whileInnerBlockStatement = new InnerBlockStatement(parentProcedureVal, whileStatementList);
  whileStatementBlock.push_back(whileInnerBlockStatement);
  Statement* whileContainerStatement = new WhileContainerStatement(parentProcedureVal, currentLineNum,
    whileStatementBlock);
  return whileContainerStatement;
}

spa::Statement* spa::Parser::processWhileConditionStatement(std::string parentProcedureVal,
                                                            std::unordered_set<int> whileStmtParents,
                                                            std::unordered_set<int> ifStmtParents) {
  std::vector<spa::Token> rawConditionExpression{};
  while (getCurrToken().getType() != spa::TOKEN_OPEN_BRACE) {
    spa::Token currToken = getCurrTokenAndAdvance();
    rawConditionExpression.push_back(currToken);
  }
  std::string postfixExpression = UtilsFunction::infixToPostfix(rawConditionExpression);
  Statement* whileConditionStatement = new WhileConditionStatement(parentProcedureVal, postfixExpression,
    whileStmtParents, ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return whileConditionStatement;
}

spa::Statement* spa::Parser::processIfStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                                std::unordered_set<int> ifStmtParents) {
  int currentLineNum = statementLineNum;
  ifStmtParents.insert(currentLineNum);
  std::vector<Statement*> ifStatementBlock{};
  Statement* ifConditionStatement = processIfConditionStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
  ifStatementBlock.push_back(ifConditionStatement);
  skipCurrToken(); // Skip over then token
  skipCurrToken(); // Skip over open brace token
  std::vector<Statement*> thenStatementList = processStmtList(parentProcedureVal, whileStmtParents, ifStmtParents);
  Statement* thenStatementInnerBlock = new InnerBlockStatement(parentProcedureVal, thenStatementList);
  ifStatementBlock.push_back(thenStatementInnerBlock);
  skipCurrToken(); // Skip over else token 
  skipCurrToken(); // Skip over open brace token
  std::vector<Statement*> elseStatementList = processStmtList(parentProcedureVal, whileStmtParents, ifStmtParents);
  Statement* elseStatementInnerBlock = new InnerBlockStatement(parentProcedureVal, elseStatementList);
  ifStatementBlock.push_back(elseStatementInnerBlock);
  Statement* ifContainerStatement = new IfContainerStatement(parentProcedureVal, currentLineNum, ifStatementBlock);
  return ifContainerStatement;
}


spa::Statement* spa::Parser::processIfConditionStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                                         std::unordered_set<int> ifStmtParents) {
  std::vector<spa::Token> rawConditionExpression{};
  while (getCurrToken().getType() != spa::TOKEN_THEN) {
    spa::Token currToken = getCurrTokenAndAdvance();
    rawConditionExpression.push_back(currToken);
  }
  std::string postfixExpression = UtilsFunction::infixToPostfix(rawConditionExpression);
  Statement* ifConditionStatement = new IfConditionStatement(
    parentProcedureVal, postfixExpression,
    whileStmtParents, ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return ifConditionStatement;
}

spa::Statement* spa::Parser::processAssignStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                                    std::unordered_set<int> ifStmtParents) {
  std::string assignmentVar = getPrevToken().getValue();
  skipCurrToken(); // Skip over equal token
  std::vector<spa::Token> rawAssignExpression = {};
  while (getCurrToken().getType() != spa::TOKEN_SEMICOLON) {
    spa::Token currToken = getCurrTokenAndAdvance();
    rawAssignExpression.push_back(currToken);
  }
  skipCurrToken(); // Skip over semi colon token
  std::string postfixExpression = UtilsFunction::infixToPostfix(rawAssignExpression);
  Statement* assignStatement = new AssignStatement(parentProcedureVal, assignmentVar, postfixExpression,
    whileStmtParents, ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return assignStatement;
}


// Private functions to traverse the stream of tokens
void spa::Parser::advanceStream(int64_t offset) {
  tokenStream.seek(offset);
}

void spa::Parser::skipCurrToken() {
  advanceStream(1);
}

bool spa::Parser::matchToken(spa::TokenType tokenType) {
  if (isEndOfProgram()) {
    return false;
  }
  spa::TokenType nextTokenType = getCurrToken().getType();
  return nextTokenType == tokenType;
}


spa::Token spa::Parser::getCurrToken() {
  return tokenStream[static_cast<int64_t>(0)];
}

spa::Token spa::Parser::getPrevToken() {
  return tokenStream[static_cast<int64_t>(-1)];
}


spa::Token spa::Parser::getCurrTokenAndAdvance() {
  spa::Token currToken = getCurrToken();
  advanceStream(1);
  return currToken;
}

bool spa::Parser::isEndOfProgram() {
  if (tokenStream.remaining() == 0) {
    return true;
  }
  return false;
}

void spa::Parser::increaseStatementLineNum() {
  statementLineNum++;
}

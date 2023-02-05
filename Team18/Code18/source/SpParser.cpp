#include <vector>
#include <unordered_set>

#include "SpParser.h"

#include "ContainerStatement.h"
#include "NonContainerStatement.h"
#include "ProgramStatement.h"
#include "ProcedureStatement.h"
#include "UtilsFunction.h"

//Constructor for SpParser
spa::SpParser::SpParser(spa::Stream<spa::Token>& tokenStream) : tokenStream(tokenStream) {}

std::vector<spa::ProcedureStatement> spa::SpParser::parse() {
  std::vector<ProcedureStatement> procedureList;
  while (!isEndOfProgram()) {
    if (matchToken(spa::TOKEN_PROCEDURE)) {
      procedureList.push_back(processProcedure());
    }
  }
  return procedureList;
}

spa::ProcedureStatement spa::SpParser::processProcedure() {
  skipCurrToken(); // To skip over the procedure token
  Token procedureVarToken = getCurrTokenAndAdvance();
  skipCurrToken(); // Skip over open brace token
  std::unordered_set<int> whileStmtParents;
  std::unordered_set<int> ifStmtParents;
  std::vector<ProgramStatement*> statementLst = processStmtList(procedureVarToken.getValue(), whileStmtParents,
    ifStmtParents);
  ProcedureStatement procedure = ProcedureStatement(procedureVarToken, statementLst);
  return procedure;
}

std::vector<spa::ProgramStatement*> spa::SpParser::processStmtList(std::string parentProcedureVal,
                                                          std::unordered_set<int> whileStmtParents,
                                                          std::unordered_set<int> ifStmtParents) {
  std::vector<ProgramStatement*> statements;
  while (!matchToken(spa::TOKEN_CLOSE_BRACE) && !isEndOfProgram()) {
    statements.push_back(handleStatements(parentProcedureVal, whileStmtParents, ifStmtParents));
  }
  skipCurrToken(); // Skip over close brace token
  return statements;
}

spa::ProgramStatement* spa::SpParser::handleStatements(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
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


spa::ProgramStatement* spa::SpParser::processReadStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                                  std::unordered_set<int> ifStmtParents) {
  Token readVariable = getCurrTokenAndAdvance();
  skipCurrToken(); // Skip over semi colon token
  ProgramStatement* readStatement = new ReadStatement(parentProcedureVal, readVariable.getValue(), whileStmtParents,
    ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return readStatement;
}

spa::ProgramStatement* spa::SpParser::processPrintStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                                   std::unordered_set<int> ifStmtParents) {
  Token printVariable = getCurrTokenAndAdvance();
  skipCurrToken(); // Skip over semi colon token
  ProgramStatement* printStatement = new PrintStatement(parentProcedureVal, printVariable.getValue(), whileStmtParents,
    ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return printStatement;
}

spa::ProgramStatement* spa::SpParser::processCallStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                                  std::unordered_set<int> ifStmtParents) {
  Token callVariable = getCurrTokenAndAdvance();
  skipCurrToken(); // Skip over semi colon token
  ProgramStatement* callStatement = new CallStatement(parentProcedureVal, callVariable.getValue(), whileStmtParents,
    ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return callStatement;
}

spa::ProgramStatement* spa::SpParser::processWhileStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                                   std::unordered_set<int> ifStmtParents) {
  int currentLineNum = statementLineNum;
  whileStmtParents.insert(currentLineNum);
  std::vector<ProgramStatement*> whileStatementBlock{};
  ProgramStatement* whileConditionStatement = processWhileConditionStatement(parentProcedureVal, whileStmtParents,
    ifStmtParents);
  whileStatementBlock.push_back(whileConditionStatement);
  skipCurrToken(); // Skip over open brace token
  std::vector<ProgramStatement*> whileStatementList = processStmtList(parentProcedureVal, whileStmtParents, ifStmtParents);
  ProgramStatement* whileInnerBlockStatement = new InnerBlockStatement(parentProcedureVal, whileStatementList);
  whileStatementBlock.push_back(whileInnerBlockStatement);
  ProgramStatement* whileContainerStatement = new WhileContainerStatement(parentProcedureVal, currentLineNum,
    whileStatementBlock);
  return whileContainerStatement;
}

spa::ProgramStatement* spa::SpParser::processWhileConditionStatement(std::string parentProcedureVal,
                                                            std::unordered_set<int> whileStmtParents,
                                                            std::unordered_set<int> ifStmtParents) {
  std::vector<spa::Token> rawConditionExpression{};
  while (getCurrToken().getType() != spa::TOKEN_OPEN_BRACE) {
    spa::Token currToken = getCurrTokenAndAdvance();
    rawConditionExpression.push_back(currToken);
  }
  std::string postfixExpression = UtilsFunction::infixToPostfix(rawConditionExpression);
  ProgramStatement* whileConditionStatement = new WhileConditionStatement(parentProcedureVal, postfixExpression,
    whileStmtParents, ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return whileConditionStatement;
}

spa::ProgramStatement* spa::SpParser::processIfStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                                std::unordered_set<int> ifStmtParents) {
  int currentLineNum = statementLineNum;
  ifStmtParents.insert(currentLineNum);
  std::vector<ProgramStatement*> ifStatementBlock{};
  ProgramStatement* ifConditionStatement = processIfConditionStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
  ifStatementBlock.push_back(ifConditionStatement);
  skipCurrToken(); // Skip over then token
  skipCurrToken(); // Skip over open brace token
  std::vector<ProgramStatement*> thenStatementList = processStmtList(parentProcedureVal, whileStmtParents, ifStmtParents);
  ProgramStatement* thenStatementInnerBlock = new InnerBlockStatement(parentProcedureVal, thenStatementList);
  ifStatementBlock.push_back(thenStatementInnerBlock);
  skipCurrToken(); // Skip over else token 
  skipCurrToken(); // Skip over open brace token
  std::vector<ProgramStatement*> elseStatementList = processStmtList(parentProcedureVal, whileStmtParents, ifStmtParents);
  ProgramStatement* elseStatementInnerBlock = new InnerBlockStatement(parentProcedureVal, elseStatementList);
  ifStatementBlock.push_back(elseStatementInnerBlock);
  ProgramStatement* ifContainerStatement = new IfContainerStatement(parentProcedureVal, currentLineNum, ifStatementBlock);
  return ifContainerStatement;
}


spa::ProgramStatement* spa::SpParser::processIfConditionStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                                         std::unordered_set<int> ifStmtParents) {
  std::vector<spa::Token> rawConditionExpression{};
  while (getCurrToken().getType() != spa::TOKEN_THEN) {
    spa::Token currToken = getCurrTokenAndAdvance();
    rawConditionExpression.push_back(currToken);
  }
  std::string postfixExpression = UtilsFunction::infixToPostfix(rawConditionExpression);
  ProgramStatement* ifConditionStatement = new IfConditionStatement(
    parentProcedureVal, postfixExpression,
    whileStmtParents, ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return ifConditionStatement;
}

spa::ProgramStatement* spa::SpParser::processAssignStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
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
  ProgramStatement* assignStatement = new AssignStatement(parentProcedureVal, assignmentVar, postfixExpression,
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

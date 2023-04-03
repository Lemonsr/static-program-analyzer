#pragma once
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "ProcedureStatement.h"
#include "ProgramStatement.h"
#include "Stream.h"
#include "Token.h"

namespace spa {
class SpParser {
 private:
  spa::Stream<spa::Token>& tokenStream;
  int statementLineNum = 1;

  std::shared_ptr<spa::ProcedureStatement> processProcedure();
  std::vector<std::shared_ptr<ProgramStatement>> processStmtList(std::string parentProcedureVal,
                                                                 std::unordered_set<int> whileStmtParents,
                                                                 std::unordered_set<int> ifStmtParents);
  std::shared_ptr<ProgramStatement> handleStatements(std::string parentProcedureVal,
                                                     std::unordered_set<int> whileStmtParents,
                                                     std::unordered_set<int> ifStmtParents);
  std::shared_ptr<ProgramStatement> processReadStatement(std::string parentProcedureVal,
                                                         std::unordered_set<int> whileStmtParents,
                                                         std::unordered_set<int> ifStmtParents);
  std::shared_ptr<ProgramStatement> processPrintStatement(std::string parentProcedureVal,
                                                          std::unordered_set<int> whileStmtParents,
                                                          std::unordered_set<int> ifStmtParents);
  std::shared_ptr<ProgramStatement> processCallStatement(std::string parentProcedureVal,
                                                         std::unordered_set<int> whileStmtParents,
                                                         std::unordered_set<int> ifStmtParents);
  std::shared_ptr<ProgramStatement> processWhileStatement(std::string parentProcedureVal,
                                                          std::unordered_set<int> whileStmtParents,
                                                          std::unordered_set<int> ifStmtParents);
  std::shared_ptr<ProgramStatement> processWhileConditionStatement(std::string parentProcedureVal,
                                                                   std::unordered_set<int> whileStmtParents,
                                                                   std::unordered_set<int> ifStmtParents);
  std::shared_ptr<ProgramStatement> processIfStatement(std::string parentProcedureVal,
                                                       std::unordered_set<int> whileStmtParents,
                                                       std::unordered_set<int> ifStmtParents);
  std::shared_ptr<ProgramStatement> processIfConditionStatement(std::string parentProcedureVal,
                                                                std::unordered_set<int> whileStmtParents,
                                                                std::unordered_set<int> ifStmtParents);
  std::shared_ptr<ProgramStatement> processAssignStatement(std::string parentProcedureVal,
                                                           std::unordered_set<int> whileStmtParents,
                                                           std::unordered_set<int> ifStmtParents);

  void advanceStream(int64_t offset);
  void skipCurrToken();
  bool matchToken(spa::TokenType tokenType);
  bool isEndOfProgram();
  spa::Token getCurrToken();
  spa::Token getPrevToken();
  spa::Token getCurrTokenAndAdvance();

  void increaseStatementLineNum();

 public:
  explicit SpParser(spa::Stream<spa::Token>& tokenStream);

  std::vector<std::shared_ptr<spa::ProcedureStatement>> spa::SpParser::parse();
};
}  // namespace spa


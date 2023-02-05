#pragma once
#include <unordered_set>

#include "ProcedureStatement.h"
#include "ProgramStatement.h"
#include "vector"
#include "Stream.h"
#include "Token.h"

namespace spa {
  class SpParser {
  private:
    spa::Stream<spa::Token>& tokenStream;
    int statementLineNum = 1;

    ProcedureStatement processProcedure();
    std::vector<ProgramStatement*> processStmtList(std::string parentProcedureVal,
                                            std::unordered_set<int> whileStmtParents,
                                            std::unordered_set<int> ifStmtParents);
    ProgramStatement* handleStatements(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                std::unordered_set<int> ifStmtParents);
    ProgramStatement* processReadStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                    std::unordered_set<int> ifStmtParents);
    ProgramStatement* processPrintStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                     std::unordered_set<int> ifStmtParents);
    ProgramStatement* processCallStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                    std::unordered_set<int> ifStmtParents);
    ProgramStatement* processWhileStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                     std::unordered_set<int> ifStmtParents);
    ProgramStatement* processWhileConditionStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                              std::unordered_set<int> ifStmtParents);
    ProgramStatement* processIfStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                  std::unordered_set<int> ifStmtParents);
    ProgramStatement* processIfConditionStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                           std::unordered_set<int> ifStmtParents);
    ProgramStatement* processAssignStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                      std::unordered_set<int> ifStmtParents);

    //Functions to traverse the stream of tokens
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

    std::vector<ProcedureStatement> parse();
  };
}  // namespace spa

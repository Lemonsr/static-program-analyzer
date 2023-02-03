#pragma once
#include <unordered_set>
#include <string>

#include "Procedure.h"
#include "vector"
#include "Stream.h"
#include "Token.h"

namespace spa {
  class SPParser {
   private:
    spa::Stream<spa::Token>& tokenStream;
    int statementLineNum = 1;

    Procedure processProcedure();
    std::vector<Statement*> processStmtList(std::string parentProcedureVal,
                                            std::unordered_set<int> whileStmtParents,
                                            std::unordered_set<int> ifStmtParents);
    Statement* handleStatements(std::string parentProcedureVal,
                                std::unordered_set<int> whileStmtParents,
                                std::unordered_set<int> ifStmtParents);
    Statement* processReadStatement(std::string parentProcedureVal,
                                    std::unordered_set<int> whileStmtParents,
                                    std::unordered_set<int> ifStmtParents);
    Statement* processPrintStatement(std::string parentProcedureVal,
                                     std::unordered_set<int> whileStmtParents,
                                     std::unordered_set<int> ifStmtParents);
    Statement* processCallStatement(std::string parentProcedureVal,
                                    std::unordered_set<int> whileStmtParents,
                                    std::unordered_set<int> ifStmtParents);
    Statement* processWhileStatement(std::string parentProcedureVal,
                                     std::unordered_set<int> whileStmtParents,
                                     std::unordered_set<int> ifStmtParents);
    Statement* processWhileConditionStatement(std::string parentProcedureVal,
                                              std::unordered_set<int> whileStmtParents,
                                              std::unordered_set<int> ifStmtParents);
    Statement* processIfStatement(std::string parentProcedureVal,
                                  std::unordered_set<int> whileStmtParents,
                                  std::unordered_set<int> ifStmtParents);
    Statement* processIfConditionStatement(std::string parentProcedureVal,
                                           std::unordered_set<int> whileStmtParents,
                                           std::unordered_set<int> ifStmtParents);
    Statement* processAssignStatement(std::string parentProcedureVal,
                                      std::unordered_set<int> whileStmtParents,
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
    explicit SPParser(spa::Stream<spa::Token>& tokenStream);

    std::vector<Procedure> parse();
  };
}  // namespace spa

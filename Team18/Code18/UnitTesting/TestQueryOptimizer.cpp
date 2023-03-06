#include <iostream>
#include <vector>

#include "stdafx.h"
#include "QueryOptimizer.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestQueryOptimizer) {
    TEST_METHOD(TestGetGroupsOnlyNoSynonym) {
      spa::ParsedQuery parsedQuery;
      spa::SuchThatClause modifiesLineNumberVarName = 
        spa::SuchThatClause(spa::MODIFIES,
                            spa::PqlArgument(spa::LINE_NO, "5", {}),
                            spa::PqlArgument(spa::LITERAL_STRING, "x", {}));

      spa::SuchThatClause followsLineNumberLineNumber =
        spa::SuchThatClause(spa::FOLLOWS,
                            spa::PqlArgument(spa::LINE_NO, "5", {}),
                            spa::PqlArgument(spa::LINE_NO, "6", {}));
      parsedQuery.addSuchThatClause(modifiesLineNumberVarName);
      parsedQuery.addSuchThatClause(followsLineNumberLineNumber);

      std::vector<spa::SuchThatClause>& suchThatClauses = parsedQuery.getSuchThatClauses();

      spa::NoSynonymClauseGroup expectedNoSynonymClauseGroup;
      expectedNoSynonymClauseGroup.addClause(suchThatClauses[0]);
      expectedNoSynonymClauseGroup.addClause(suchThatClauses[1]);

      spa::QueryOptimizer queryOptimizer;
      std::pair<spa::NoSynonymClauseGroup, std::vector<spa::ConnectedSynonymClauseGroup>> groups =
        queryOptimizer.getGroups(parsedQuery);
      spa::NoSynonymClauseGroup noSynonymClauseGroup = groups.first;

      Assert::IsTrue(noSynonymClauseGroup == expectedNoSynonymClauseGroup);
    }

    TEST_METHOD(TestGetGroupsOnlyConnectedSynonyms) {
      spa::ParsedQuery parsedQuery;
      spa::SuchThatClause modifiesStmtVar =
        spa::SuchThatClause(spa::MODIFIES,
                            spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                            spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));

      spa::SuchThatClause followsStmtStmt =
        spa::SuchThatClause(spa::FOLLOWS,
                            spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                            spa::PqlArgument(spa::SYNONYM, "s2", { spa::STMT }));

      spa::PatternClause patternIf =
        spa::PatternClause(spa::PqlArgument(spa::SYNONYM, "if", {spa::IF}),
                           spa::PqlArgument(spa::SYNONYM, "v", {spa::VARIABLE}),
                           spa::Pattern(spa::ANY, {spa::Token(spa::TOKEN_UNDERSCORE, "_")}), 3);

      parsedQuery.addSuchThatClause(modifiesStmtVar);
      parsedQuery.addSuchThatClause(followsStmtStmt);
      parsedQuery.addPatternClause(patternIf);
      std::vector<spa::SuchThatClause>& suchThatClauses = parsedQuery.getSuchThatClauses();
      std::vector<spa::PatternClause>& patternClauses = parsedQuery.getPatternClauses();

      spa::ConnectedSynonymClauseGroup expectedConnectedSynonymGroup;
      expectedConnectedSynonymGroup.addClause(suchThatClauses[0]);
      expectedConnectedSynonymGroup.addClause(suchThatClauses[1]);
      expectedConnectedSynonymGroup.addClause(patternClauses[0]);

      spa::QueryOptimizer queryOptimizer;
      std::pair<spa::NoSynonymClauseGroup, std::vector<spa::ConnectedSynonymClauseGroup>> groups =
        queryOptimizer.getGroups(parsedQuery);
      std::vector<spa::ConnectedSynonymClauseGroup> connectedSynonymGroups = groups.second;

      Assert::IsTrue(groups.first.isEmpty());
      Assert::IsTrue(connectedSynonymGroups.size() == size_t(1));
      Assert::IsTrue(connectedSynonymGroups[0] == expectedConnectedSynonymGroup);
    }
    TEST_METHOD(TestGetGroupsNoSynonymWithConnectedSynonym) {
      /*
        Modifies(5, "x")
        Follows(5, 6)

        pattern a("x", _)
        Next(a, s)
      */
      spa::ParsedQuery parsedQuery;
      spa::SuchThatClause modifiesLineNumberVarName = 
        spa::SuchThatClause(spa::MODIFIES,
                            spa::PqlArgument(spa::LINE_NO, "5", {}),
                            spa::PqlArgument(spa::LITERAL_STRING, "x", {}));

      spa::SuchThatClause followsLineNumberLineNumber =
        spa::SuchThatClause(spa::FOLLOWS,
                            spa::PqlArgument(spa::LINE_NO, "5", {}),
                            spa::PqlArgument(spa::LINE_NO, "6", {}));

      spa::SuchThatClause nextAssignStmt =
        spa::SuchThatClause(spa::NEXT,
                            spa::PqlArgument(spa::SYNONYM, "a", {spa::ASSIGN}),
                            spa::PqlArgument(spa::SYNONYM, "s", {spa::STMT}));

      spa::PatternClause patternAssign =
        spa::PatternClause(spa::PqlArgument(spa::SYNONYM, "a", {spa::ASSIGN}),
                           spa::PqlArgument(spa::LITERAL_STRING, "x", {}),
                           spa::Pattern(spa::ANY, {spa::Token(spa::TOKEN_UNDERSCORE, "_")}), 2);
      parsedQuery.addSuchThatClause(modifiesLineNumberVarName);
      parsedQuery.addSuchThatClause(followsLineNumberLineNumber);
      parsedQuery.addSuchThatClause(nextAssignStmt);
      parsedQuery.addPatternClause(patternAssign);
      std::vector<spa::SuchThatClause>& suchThatClauses = parsedQuery.getSuchThatClauses();

      spa::NoSynonymClauseGroup expectedNoSynonymClauseGroup;
      expectedNoSynonymClauseGroup.addClause(suchThatClauses[0]);
      expectedNoSynonymClauseGroup.addClause(suchThatClauses[1]);

      std::vector<spa::PatternClause>& patternClauses = parsedQuery.getPatternClauses();

      spa::ConnectedSynonymClauseGroup expectedConnectedSynonymClauseGroup;
      expectedConnectedSynonymClauseGroup.addClause(suchThatClauses[2]);
      expectedConnectedSynonymClauseGroup.addClause(patternClauses[0]);
      
      spa::QueryOptimizer queryOptimizer;
      std::pair<spa::NoSynonymClauseGroup, std::vector<spa::ConnectedSynonymClauseGroup>> groups =
        queryOptimizer.getGroups(parsedQuery);
      spa::NoSynonymClauseGroup noSynonymClauseGroup = groups.first;
      std::vector<spa::ConnectedSynonymClauseGroup> connectedSynonymGroups = groups.second;

      Assert::IsTrue(noSynonymClauseGroup == expectedNoSynonymClauseGroup);
      Assert::IsTrue(connectedSynonymGroups.size() == size_t(1));
      Assert::IsTrue(connectedSynonymGroups[0] == expectedConnectedSynonymClauseGroup);
    }

    TEST_METHOD(TestGetGroupsNoSynonymWithMultipleConnectedSynonym) {
      /*
        Uses(5, "x")

        Modifies(s, v)
        Follows(s, s2)
        pattern if(v, _, _)

        pattern a("x", _)
        Follows*(a, c)
      */
      spa::ParsedQuery parsedQuery;
      spa::SuchThatClause usesLineNumberVarName = 
        spa::SuchThatClause(spa::USES,
                            spa::PqlArgument(spa::LINE_NO, "5", {}),
                            spa::PqlArgument(spa::LITERAL_STRING, "x", {}));

      spa::SuchThatClause modifiesStmtVar =
        spa::SuchThatClause(spa::MODIFIES,
                            spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                            spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));

      spa::SuchThatClause followsStmtStmt =
        spa::SuchThatClause(spa::FOLLOWS,
                            spa::PqlArgument(spa::SYNONYM, "s", { spa::STMT }),
                            spa::PqlArgument(spa::SYNONYM, "s2", { spa::STMT }));

      spa::PatternClause patternIf =
        spa::PatternClause(spa::PqlArgument(spa::SYNONYM, "if", {spa::IF}),
                           spa::PqlArgument(spa::SYNONYM, "v", {spa::VARIABLE}),
                           spa::Pattern(spa::ANY, {spa::Token(spa::TOKEN_UNDERSCORE, "_")}), 3);

      spa::PatternClause patternAssign =
        spa::PatternClause(spa::PqlArgument(spa::SYNONYM, "a", {spa::ASSIGN}),
                           spa::PqlArgument(spa::LITERAL_STRING, "x", {}),
                           spa::Pattern(spa::ANY, {spa::Token(spa::TOKEN_UNDERSCORE, "_")}), 2);

      spa::SuchThatClause followsStarAssignCall =
        spa::SuchThatClause(spa::FOLLOWS,
                            spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                            spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }));

      parsedQuery.addSuchThatClause(usesLineNumberVarName);
      parsedQuery.addSuchThatClause(modifiesStmtVar);
      parsedQuery.addSuchThatClause(followsStmtStmt);
      parsedQuery.addPatternClause(patternIf);
      parsedQuery.addPatternClause(patternAssign);
      parsedQuery.addSuchThatClause(followsStarAssignCall);

      std::vector<spa::SuchThatClause>& suchThatClauses = parsedQuery.getSuchThatClauses();
      std::vector<spa::PatternClause>& patternClauses = parsedQuery.getPatternClauses();
      spa::NoSynonymClauseGroup expectedNoSynonymClauseGroup;
      expectedNoSynonymClauseGroup.addClause(suchThatClauses[0]);

      spa::ConnectedSynonymClauseGroup firstConnectedGroup;
      firstConnectedGroup.addClause(suchThatClauses[1]);
      firstConnectedGroup.addClause(suchThatClauses[2]);
      firstConnectedGroup.addClause(patternClauses[0]);

      spa::ConnectedSynonymClauseGroup secondConnectedGroup;
      secondConnectedGroup.addClause(suchThatClauses[3]);
      secondConnectedGroup.addClause(patternClauses[1]);

      spa::QueryOptimizer queryOptimizer;
      std::pair<spa::NoSynonymClauseGroup, std::vector<spa::ConnectedSynonymClauseGroup>> groups =
        queryOptimizer.getGroups(parsedQuery);
      spa::NoSynonymClauseGroup noSynonymClauseGroup = groups.first;
      std::vector<spa::ConnectedSynonymClauseGroup> connectedSynonymGroups = groups.second;

      Assert::IsTrue(noSynonymClauseGroup == expectedNoSynonymClauseGroup);
      Assert::IsTrue(connectedSynonymGroups.size() == size_t(2));
      Assert::IsTrue(connectedSynonymGroups[0] == firstConnectedGroup);
      Assert::IsTrue(connectedSynonymGroups[1] == secondConnectedGroup);
    }

    TEST_METHOD(TestGetGroupsWithNoClauses) {
      spa::ParsedQuery parsedQuery;

      spa::QueryOptimizer queryOptimizer;
      std::pair<spa::NoSynonymClauseGroup, std::vector<spa::ConnectedSynonymClauseGroup>> groups =
        queryOptimizer.getGroups(parsedQuery);
      spa::NoSynonymClauseGroup noSynonymClauseGroup = groups.first;
      std::vector<spa::ConnectedSynonymClauseGroup> connectedSynonymGroups = groups.second;

      Assert::IsTrue(noSynonymClauseGroup.isEmpty());
      Assert::IsTrue(connectedSynonymGroups.empty());
    }
  };
}  // namespace UnitTesting
#include <iostream>
#include <vector>

#include "stdafx.h"
#include "QueryOptimizer.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
  TEST_CLASS(TestQueryOptimizer) {
    TEST_METHOD(TestGetGroupsOnlyNoSynonym) {
      /*
        Modifies(5, "x")
        Follows(5, 6)
        with x = y
        with 1 = 1
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

      spa::WithClause withStringString = spa::WithClause(spa::WithArgument(spa::QpsValue("x")),
                                                         spa::WithArgument(spa::QpsValue("y")));

      spa::WithClause withIntegerInteger = spa::WithClause(spa::WithArgument(spa::QpsValue(1)),
                                                           spa::WithArgument(spa::QpsValue(1)));
      parsedQuery.addSuchThatClause(modifiesLineNumberVarName);
      parsedQuery.addSuchThatClause(followsLineNumberLineNumber);
      parsedQuery.addWithClause(withStringString);
      parsedQuery.addWithClause(withIntegerInteger);

      std::vector<spa::SuchThatClause>& suchThatClauses = parsedQuery.getSuchThatClauses();
      std::vector<spa::WithClause>& withClauses = parsedQuery.getWithClauses();

      spa::NoSynonymClauseGroup expectedNoSynonymClauseGroup;
      expectedNoSynonymClauseGroup.addClause(suchThatClauses[0]);
      expectedNoSynonymClauseGroup.addClause(suchThatClauses[1]);
      expectedNoSynonymClauseGroup.addClause(withClauses[0]);
      expectedNoSynonymClauseGroup.addClause(withClauses[1]);

      spa::QueryOptimizer queryOptimizer;
      spa::ClauseGroups groups = queryOptimizer.getGroups(parsedQuery);
      spa::NoSynonymClauseGroup noSynonymClauseGroup = groups.getNoSynonynmClauseGroup();

      Assert::IsTrue(noSynonymClauseGroup == expectedNoSynonymClauseGroup);
      Assert::IsTrue(groups.getConnectedSynonymClauseGroups().empty());
      Assert::IsTrue(groups.getWithClauseGroup().isEmpty());
    }

    TEST_METHOD(TestGetGroupsOnlyConnectedSynonyms) {
      /*
        with s.stmt# = 5
        with v.varName = "x"
        Modifies(s, v)
        Follows(s, s2)
        pattern if(v, _, _)
      */
      spa::ParsedQuery parsedQuery;
      spa::WithClause withStmtNum = spa::WithClause(spa::WithArgument("s.stmt#"),
                                                    spa::WithArgument(spa::QpsValue(5)));

      spa::WithClause withVarName = spa::WithClause(spa::WithArgument("v.varName"),
                                                    spa::WithArgument(spa::QpsValue("x")));
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
      parsedQuery.addWithClause(withStmtNum);
      parsedQuery.addWithClause(withVarName);
      std::vector<spa::SuchThatClause>& suchThatClauses = parsedQuery.getSuchThatClauses();
      std::vector<spa::PatternClause>& patternClauses = parsedQuery.getPatternClauses();
      std::vector<spa::WithClause>& withClauses = parsedQuery.getWithClauses();

      spa::ConnectedSynonymClauseGroup expectedConnectedSynonymGroup;
      expectedConnectedSynonymGroup.addClause(suchThatClauses[0]);
      expectedConnectedSynonymGroup.addClause(suchThatClauses[1]);
      expectedConnectedSynonymGroup.addClause(patternClauses[0]);

      spa::WithClauseGroup expectedWithClauseGroup;
      expectedWithClauseGroup.addAttrValueClause(withClauses[0]);
      expectedWithClauseGroup.addAttrValueClause(withClauses[1]);

      spa::QueryOptimizer queryOptimizer;
      spa::ClauseGroups groups = queryOptimizer.getGroups(parsedQuery);
      std::vector<spa::ConnectedSynonymClauseGroup> connectedSynonymGroups = groups.getConnectedSynonymClauseGroups();
      spa::WithClauseGroup withClauseGroup = groups.getWithClauseGroup();

      Assert::IsTrue(groups.getNoSynonynmClauseGroup().isEmpty());
      Assert::IsTrue(connectedSynonymGroups.size() == size_t(1));
      Assert::IsTrue(connectedSynonymGroups[0] == expectedConnectedSynonymGroup);
      Assert::IsTrue(withClauseGroup == expectedWithClauseGroup);
    }

    TEST_METHOD(TestGetGroupsOnlyWithAttrAttr) {
      /*
        with a.stmt# = c.stmt#
        with re.varName = pn.varName
      */
      spa::ParsedQuery parsedQuery;
      spa::WithClause withAssignCall = spa::WithClause(spa::WithArgument("a.stmt#"),
                                                       spa::WithArgument("c.stmt#"));

      spa::WithClause withReadPrint = spa::WithClause(spa::WithArgument("re.varName"),
                                                      spa::WithArgument("pn.varName"));
      parsedQuery.addWithClause(withAssignCall);
      parsedQuery.addWithClause(withReadPrint);

      std::vector<spa::WithClause>& withClauses = parsedQuery.getWithClauses();

      spa::WithClauseGroup expectedWithClauseGroup;
      expectedWithClauseGroup.addAttrAttrClause(withClauses[0]);
      expectedWithClauseGroup.addAttrAttrClause(withClauses[1]);

      spa::QueryOptimizer queryOptimizer;
      spa::ClauseGroups groups = queryOptimizer.getGroups(parsedQuery);
      spa::WithClauseGroup withAttrAttrClauseGroup = groups.getWithClauseGroup();

      Assert::IsTrue(withAttrAttrClauseGroup == expectedWithClauseGroup);
      Assert::IsTrue(groups.getNoSynonynmClauseGroup().isEmpty());
      Assert::IsTrue(groups.getConnectedSynonymClauseGroups().empty());
    }

    TEST_METHOD(TestGetGroupsNoSynonymWithConnectedSynonym) {
      /*
        Modifies(5, "x")
        Follows(5, 6)

        pattern a("x", _)
        Next(a, s)
        with s.stmt# = 5
      */
      spa::ParsedQuery parsedQuery;
      spa::WithClause withStmtNum = spa::WithClause(spa::WithArgument("s.stmt#"),
                                                    spa::WithArgument(spa::QpsValue(5)));

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
      parsedQuery.addWithClause(withStmtNum);
      std::vector<spa::SuchThatClause>& suchThatClauses = parsedQuery.getSuchThatClauses();

      spa::NoSynonymClauseGroup expectedNoSynonymClauseGroup;
      expectedNoSynonymClauseGroup.addClause(suchThatClauses[0]);
      expectedNoSynonymClauseGroup.addClause(suchThatClauses[1]);

      std::vector<spa::PatternClause>& patternClauses = parsedQuery.getPatternClauses();
      std::vector<spa::WithClause>& withClauses = parsedQuery.getWithClauses();

      spa::ConnectedSynonymClauseGroup expectedConnectedSynonymClauseGroup;
      expectedConnectedSynonymClauseGroup.addClause(suchThatClauses[2]);
      expectedConnectedSynonymClauseGroup.addClause(patternClauses[0]);

      spa::WithClauseGroup expectedWithClauseGroup;
      expectedWithClauseGroup.addAttrValueClause(withClauses[0]);

      spa::QueryOptimizer queryOptimizer;
      spa::ClauseGroups groups = queryOptimizer.getGroups(parsedQuery);
      spa::NoSynonymClauseGroup noSynonymClauseGroup = groups.getNoSynonynmClauseGroup();
      std::vector<spa::ConnectedSynonymClauseGroup> connectedSynonymGroups = groups.getConnectedSynonymClauseGroups();
      spa::WithClauseGroup withAttrAttrClauseGroup = groups.getWithClauseGroup();

      Assert::IsTrue(noSynonymClauseGroup == expectedNoSynonymClauseGroup);
      Assert::IsTrue(connectedSynonymGroups.size() == size_t(1));
      Assert::IsTrue(connectedSynonymGroups[0] == expectedConnectedSynonymClauseGroup);
      Assert::IsTrue(withAttrAttrClauseGroup == expectedWithClauseGroup);
    }

    TEST_METHOD(TestGetGroupsNoSynonymWithMultipleConnectedSynonym) {
      /*
        Uses(5, "x")
         
        Modifies(s, v)
        Follows(s, s2)
        pattern if(v, _, _)
        with s.stmt# = 5

        pattern a("x", _)
        Follows*(a, c)
        with a.stmt# = 10
      */
      spa::ParsedQuery parsedQuery;
      spa::WithClause withStmtNum = spa::WithClause(spa::WithArgument("s.stmt#"),
                                                    spa::WithArgument(spa::QpsValue(5)));

      spa::WithClause withAssignNum = spa::WithClause(spa::WithArgument("a.stmt#"),
                                                      spa::WithArgument(spa::QpsValue(10)));

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
      parsedQuery.addWithClause(withStmtNum);
      parsedQuery.addPatternClause(patternIf);
      parsedQuery.addPatternClause(patternAssign);
      parsedQuery.addSuchThatClause(followsStarAssignCall);
      parsedQuery.addWithClause(withAssignNum);

      std::vector<spa::SuchThatClause>& suchThatClauses = parsedQuery.getSuchThatClauses();
      std::vector<spa::PatternClause>& patternClauses = parsedQuery.getPatternClauses();
      std::vector<spa::WithClause>& withClauses = parsedQuery.getWithClauses();
      spa::NoSynonymClauseGroup expectedNoSynonymClauseGroup;
      expectedNoSynonymClauseGroup.addClause(suchThatClauses[0]);

      spa::ConnectedSynonymClauseGroup firstConnectedGroup;
      firstConnectedGroup.addClause(suchThatClauses[1]);
      firstConnectedGroup.addClause(suchThatClauses[2]);
      firstConnectedGroup.addClause(patternClauses[0]);

      spa::ConnectedSynonymClauseGroup secondConnectedGroup;
      secondConnectedGroup.addClause(suchThatClauses[3]);
      secondConnectedGroup.addClause(patternClauses[1]);

      spa::WithClauseGroup expectedWithClauseGroup;
      expectedWithClauseGroup.addAttrValueClause(withClauses[0]);
      expectedWithClauseGroup.addAttrValueClause(withClauses[1]);

      spa::QueryOptimizer queryOptimizer;
      spa::ClauseGroups groups = queryOptimizer.getGroups(parsedQuery);
      spa::NoSynonymClauseGroup noSynonymClauseGroup = groups.getNoSynonynmClauseGroup();
      std::vector<spa::ConnectedSynonymClauseGroup> connectedSynonymGroups = groups.getConnectedSynonymClauseGroups();
      spa::WithClauseGroup withAttrAttrClauseGroup = groups.getWithClauseGroup();

      Assert::IsTrue(noSynonymClauseGroup == expectedNoSynonymClauseGroup);
      Assert::IsTrue(connectedSynonymGroups.size() == size_t(2));
      Assert::IsTrue(connectedSynonymGroups[0] == firstConnectedGroup);
      Assert::IsTrue(connectedSynonymGroups[1] == secondConnectedGroup);
      Assert::IsTrue(withAttrAttrClauseGroup == expectedWithClauseGroup);
    }

    TEST_METHOD(TestGetGroups) {
      /*
        Next(5, 6)

        Calls(p, q)
        Modifies(r, v)
        pattern w(v, _)

        pattern a("x", _)
        Parent*(s, c)

        with s.stmt# = a.stmt#
        with q.procName = r.procName
      */
      spa::ParsedQuery parsedQuery;
      spa::WithClause withProcName = spa::WithClause(spa::WithArgument("p.procName"),
                                                    spa::WithArgument("r.procName"));

      spa::WithClause withStmtNum = spa::WithClause(spa::WithArgument("s.stmt#"),
                                                      spa::WithArgument("a.stmt#"));

      spa::SuchThatClause nextLineNumberLineNumber =
        spa::SuchThatClause(spa::NEXT,
                            spa::PqlArgument(spa::LINE_NO, "5", {}),
                            spa::PqlArgument(spa::LINE_NO, "6", {}));

      spa::SuchThatClause callsProcProc =
        spa::SuchThatClause(spa::CALLS,
                            spa::PqlArgument(spa::SYNONYM, "p", { spa::PROCEDURE }),
                            spa::PqlArgument(spa::SYNONYM, "q", { spa::PROCEDURE }));

      spa::SuchThatClause modifiesProcVar =
        spa::SuchThatClause(spa::MODIFIES,
                            spa::PqlArgument(spa::SYNONYM, "q", { spa::PROCEDURE }),
                            spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }));

      spa::PatternClause patternWhile =
        spa::PatternClause(spa::PqlArgument(spa::SYNONYM, "w", { spa::WHILE }),
                           spa::PqlArgument(spa::SYNONYM, "v", { spa::VARIABLE }),
                           spa::Pattern(spa::ANY, {spa::Token(spa::TOKEN_UNDERSCORE, "_")}), 2);

      spa::PatternClause patternAssign =
        spa::PatternClause(spa::PqlArgument(spa::SYNONYM, "a", {spa::ASSIGN}),
                           spa::PqlArgument(spa::LITERAL_STRING, "x", {}),
                           spa::Pattern(spa::ANY, {spa::Token(spa::TOKEN_UNDERSCORE, "_")}), 2);

      spa::SuchThatClause parentStarAssignCall =
        spa::SuchThatClause(spa::PARENT_STAR,
                            spa::PqlArgument(spa::SYNONYM, "a", { spa::ASSIGN }),
                            spa::PqlArgument(spa::SYNONYM, "c", { spa::CALL }));

      parsedQuery.addSuchThatClause(nextLineNumberLineNumber);
      parsedQuery.addSuchThatClause(callsProcProc);
      parsedQuery.addSuchThatClause(modifiesProcVar);
      parsedQuery.addPatternClause(patternWhile);
      parsedQuery.addPatternClause(patternAssign);
      parsedQuery.addSuchThatClause(parentStarAssignCall);
      parsedQuery.addWithClause(withProcName);
      parsedQuery.addWithClause(withStmtNum);

      std::vector<spa::SuchThatClause>& suchThatClauses = parsedQuery.getSuchThatClauses();
      std::vector<spa::PatternClause>& patternClauses = parsedQuery.getPatternClauses();
      std::vector<spa::WithClause>& withClauses = parsedQuery.getWithClauses();
      spa::NoSynonymClauseGroup expectedNoSynonymClauseGroup;
      expectedNoSynonymClauseGroup.addClause(suchThatClauses[0]);

      spa::ConnectedSynonymClauseGroup firstConnectedGroup;
      firstConnectedGroup.addClause(suchThatClauses[1]);
      firstConnectedGroup.addClause(suchThatClauses[2]);
      firstConnectedGroup.addClause(patternClauses[0]);

      spa::ConnectedSynonymClauseGroup secondConnectedGroup;
      secondConnectedGroup.addClause(suchThatClauses[3]);
      secondConnectedGroup.addClause(patternClauses[1]);

      spa::WithClauseGroup expectedWithClauseGroup;
      expectedWithClauseGroup.addAttrAttrClause(withClauses[0]);
      expectedWithClauseGroup.addAttrAttrClause(withClauses[1]);

      spa::QueryOptimizer queryOptimizer;
      spa::ClauseGroups groups = queryOptimizer.getGroups(parsedQuery);
      spa::NoSynonymClauseGroup noSynonymClauseGroup = groups.getNoSynonynmClauseGroup();
      std::vector<spa::ConnectedSynonymClauseGroup> connectedSynonymGroups = groups.getConnectedSynonymClauseGroups();
      spa::WithClauseGroup withAttrAttrClauseGroup = groups.getWithClauseGroup();

      Assert::IsTrue(noSynonymClauseGroup == expectedNoSynonymClauseGroup);
      Assert::IsTrue(connectedSynonymGroups.size() == size_t(2));
      Assert::IsTrue(connectedSynonymGroups[0] == firstConnectedGroup);
      Assert::IsTrue(connectedSynonymGroups[1] == secondConnectedGroup);
      Assert::IsTrue(withAttrAttrClauseGroup == expectedWithClauseGroup);
    }

    TEST_METHOD(TestGetGroupsWithNoClauses) {
      spa::ParsedQuery parsedQuery;

      spa::QueryOptimizer queryOptimizer;
      spa::ClauseGroups groups = queryOptimizer.getGroups(parsedQuery);
      spa::NoSynonymClauseGroup noSynonymClauseGroup = groups.getNoSynonynmClauseGroup();
      std::vector<spa::ConnectedSynonymClauseGroup> connectedSynonymGroups = groups.getConnectedSynonymClauseGroups();
      spa::WithClauseGroup withAttrAttrClauseGroup = groups.getWithClauseGroup();

      Assert::IsTrue(noSynonymClauseGroup.isEmpty());
      Assert::IsTrue(connectedSynonymGroups.empty());
      Assert::IsTrue(withAttrAttrClauseGroup.isEmpty());
    }
  };
}  // namespace UnitTesting

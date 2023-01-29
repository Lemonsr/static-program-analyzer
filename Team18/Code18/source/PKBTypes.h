#pragma once

#include <string>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>

namespace spa {
using VarName = std::string;
using VarNames = std::unordered_set<VarName>;
using ConstName = std::string;
using ConstNames = std::unordered_set<ConstName>;
using ProcName = std::string;
using ProcNames = std::unordered_set<ProcName>;
using LineNo = std::size_t;
using LineNos = std::unordered_set<LineNo>;
using LineNo_Var_Pairs = std::vector<std::pair<LineNo, VarName>>;
using PostfixString = std::string;
using Assignment = std::pair<VarName, PostfixString>;
using Follow = LineNo;
using Follows = std::unordered_set<Follow>;
using Parent = LineNo;
using Parents = std::unordered_set<Parent>;
using Child = LineNo;
using Children = std::unordered_set<Child>;
using Modifies = VarNames;
using Uses = VarNames;

enum class StatementType {
READ,
PRINT,
ASSIGN,
CALL,
WHILE,
IF
};

using VarTable = VarNames;
using ConstTable = ConstNames;
using ProcTable = ProcNames;
using FollowsTable = std::unordered_map<LineNo, Follow>;
using FollowsStarTable = std::unordered_map<LineNo, Follows>;
using ParentTable = std::unordered_map<Child, Parent>;
using ParentStarTable = std::unordered_map<Child, Parents>;
using ModifiesTable = std::unordered_map<LineNo, Modifies>;
using ModifiesProcTable = std::unordered_map<ProcName, Modifies>;
using UsesTable = std::unordered_map<LineNo, Uses>;
using UsesProcTable = std::unordered_map<ProcName, Uses>;
using AssignTable = std::unordered_map<LineNo, Assignment>;
using StatementTypeTable = std::unordered_map<LineNo, StatementType>;
using StatementProcTable = std::unordered_map<LineNo, ProcName>;
}  // namespace spa

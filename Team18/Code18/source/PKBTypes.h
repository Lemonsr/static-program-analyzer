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
using LineNo_Var_Pairs = std::unordered_set<std::pair<LineNo, VarName>>;
using PostfixString = std::string;
using Assignment = std::pair<VarName, PostfixString>;

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
using FollowsTable = std::unordered_map<LineNo, LineNo>;
using FollowsStarTable = std::unordered_map<LineNo, LineNo>;
using ParentTable = std::unordered_map<LineNo, LineNo>;
using ParentStarTable = std::unordered_map<LineNo, LineNo>;
using ModifiesTable = std::unordered_map<LineNo, VarName>;
using ModifiesProcTable = std::unordered_map<ProcName, VarName>;
using UsesTable = std::unordered_map<LineNo, VarName>;
using UsesProcTable = std::unordered_map<ProcName, VarName>;
using AssignTable = std::unordered_map<LineNo, Assignment>;
using StatementTypeTable = std::unordered_map<LineNo, StatementType>;
using StatementProcTable = std::unordered_map<LineNo, ProcName>;
}  // namespace spa

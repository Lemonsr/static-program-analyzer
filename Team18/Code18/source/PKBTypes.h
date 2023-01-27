#pragma once

#include <string>
#include <set>
#include <unordered_set>
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
using FollowsTable = std::vector<std::pair<LineNo, LineNo>>;
using FollowsStarTable = std::vector<std::pair<LineNo, LineNo>>;
using ParentTable = std::vector<std::pair<LineNo, LineNo>>;
using ParentStarTable = std::vector<std::pair<LineNo, LineNo>>;
using ModifiesTable = std::vector<std::pair<LineNo, VarName>>;
using ModifiesProcTable = std::vector<std::pair<ProcName, VarName>>;
using UsesTable = std::vector<std::pair<LineNo, VarName>>;
using UsesProcTable = std::vector<std::pair<ProcName, VarName>>;
using AssignTable = std::vector<std::pair<LineNo, Assignment>>;
using StatementTypeTable = std::vector<std::pair<LineNo, StatementType>>;
using StatementProcTable = std::vector<std::pair<LineNo, ProcName>>;
}  // namespace spa

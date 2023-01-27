#pragma once

#include "PKBTypes.h"

#include <string>

namespace spa {
	struct Variable {
		VarName varName;
	};

	struct Statement {
		StatementType statementType;
	};

	struct Procedure {
		ProcName procName;
	};

	struct LineNumber {
		LineNo lineNo;
	};

	// Procedure / Variable name
	struct Name {
		std::string name;
	};

	struct Underscore { };
}
#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ProcedureStatement.h"

namespace spa {
class SpCyclicValidator {
 private:
    std::unordered_map<std::string, std::unordered_set<std::string>> validateCallMap;
    bool dfsCheckCyclicCall(std::string parent, std::unordered_set<std::string>* currentSeen,
        std::unordered_map<std::string, bool>* visited);

 public:
    explicit SpCyclicValidator(std::unordered_map<std::string,
                               std::unordered_set<std::string>> procCallMap);
    bool validateCyclic();
};
}  // namespace spa

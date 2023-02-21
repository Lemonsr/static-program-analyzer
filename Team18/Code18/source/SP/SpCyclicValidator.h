#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ProcedureStatement.h"

namespace spa {
class SpCyclicValidator {
 private:
    std::vector<ProcedureStatement*> procedureList;
    std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;
    bool dfsCheckCyclicCall(std::string parent, std::unordered_set<std::string>* currentSeen,
        std::unordered_map<std::string, bool>* visited);
    void generateProcCallMap();

 public:
    explicit SpCyclicValidator(std::vector<ProcedureStatement*> pl);
    bool validateCyclic();
};
}  // namespace spa

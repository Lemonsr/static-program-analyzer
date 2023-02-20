#include <vector>

#include "ProcedureStatement.h"

namespace spa {
class SpCyclicValidator {
 private:
    std::vector<ProcedureStatement> procedureList;
    std::unordered_map<std::string,std::unordered_set<std::string>> procCallMap;
    bool dfsCheckCyclicCall(std::string parent, std::unordered_set<std::string>* currentSeen, std::unordered_map<std::string, bool>* visited);
    void generateProcCallMap();
 public:
    SpCyclicValidator(std::vector<ProcedureStatement> pl);
    bool validateCyclic();
    std::unordered_map<std::string, ProcedureStatement*> getProcCallMap();
    };
}

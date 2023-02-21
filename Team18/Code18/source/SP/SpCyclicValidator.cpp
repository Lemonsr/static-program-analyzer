#include "SpCyclicValidator.h"

#include "NonContainerStatement.h"

spa::SpCyclicValidator::SpCyclicValidator(std::vector<spa::ProcedureStatement*> pl) {
    procedureList = pl;
}

void spa::SpCyclicValidator::generateProcCallMap() {
    for (auto& procedure : procedureList) {
        auto statements = procedure->getStatementLst();
        for (auto& statement : statements) {
            if (dynamic_cast<CallStatement*> (statement)) {
                auto callStatement = dynamic_cast<CallStatement*> (statement);
                callStatement->addCallToProcedure(procedure);
            }
        }
        procCallMap.emplace(procedure->getProcedureVarToken().getValue(), procedure->getCalledVars());
    }
}

bool spa::SpCyclicValidator::validateCyclic() {
    generateProcCallMap();
    std::unordered_map<std::string, bool> visited = {};
    for (auto& procedure : procedureList) {
        auto parent = procedure->getProcedureVarToken().getValue();
        if (visited.find(parent) != visited.end()) {
            continue; // this proc has been checked
        }

        std::unordered_set<std::string> currentSeen = {};
        if (dfsCheckCyclicCall(parent, &currentSeen, &visited)) {
            // cyclic detected
            return true;
        }
    }
    return false;
}

bool spa::SpCyclicValidator::dfsCheckCyclicCall(std::string parent, std::unordered_set<std::string>* currentSeen, std::unordered_map<std::string, bool>* visited) {
    if (currentSeen->find(parent) != currentSeen->end()) {
        visited->emplace(parent, true);
        return true;
    }
    else if (visited->find(parent) != visited->end()) {
        return visited->at(parent);
    }
    else {
        currentSeen->emplace(parent);
        auto children = procCallMap.at(parent);
        for (auto child : children) {
            if (dfsCheckCyclicCall(child, currentSeen, visited)) {
                visited->emplace(child, true);
                return true;
            }
        }
    }

    // cache result
    visited->emplace(parent, false);
    currentSeen->erase(parent);

    return false;
}

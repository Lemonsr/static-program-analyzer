#include <string>
#include <unordered_map>
#include <unordered_set>

#include "SpCyclicValidator.h"
#include "NonContainerStatement.h"

spa::SpCyclicValidator::SpCyclicValidator(std::unordered_map<std::string,
                                          std::unordered_set<std::string>> procCallMap) {
    validateCallMap = procCallMap;
}

bool spa::SpCyclicValidator::validateCyclic() {
    std::unordered_map<std::string, bool> visited = {};
    for (auto& procedure : validateCallMap) {
        auto parent = procedure.first;
        if (visited.find(parent) != visited.end()) {
            continue;
        }

        std::unordered_set<std::string> currentSeen = {};
        if (dfsCheckCyclicCall(parent, &currentSeen, &visited)) {
            return true;
        }
    }
    return false;
}

bool spa::SpCyclicValidator::dfsCheckCyclicCall(std::string parent,
                                                std::unordered_set<std::string>* currentSeen,
                                                std::unordered_map<std::string, bool>* visited) {
    if (currentSeen->find(parent) != currentSeen->end()) {
        visited->emplace(parent, true);
        return true;
    } else if (visited->find(parent) != visited->end()) {
        return visited->at(parent);
    } else {
        currentSeen->emplace(parent);
        auto children = validateCallMap.at(parent);
        for (auto child : children) {
            if (dfsCheckCyclicCall(child, currentSeen, visited)) {
                visited->emplace(child, true);
                return true;
            }
        }
    }

    visited->emplace(parent, false);
    currentSeen->erase(parent);

    return false;
}

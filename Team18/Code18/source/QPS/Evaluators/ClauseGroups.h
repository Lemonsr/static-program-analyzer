#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <string>

#include "QpsResultTable.h"
#include "NoSynonymClauseGroup.h"
#include "ConnectedSynonymClauseGroup.h"
#include "WithClauseGroup.h"

namespace spa {
class ClauseGroups {
 private:
  NoSynonymClauseGroup& noSynonymClauseGroup;
  std::vector<ConnectedSynonymClauseGroup>& connectedSynonymClauseGroups;
  WithClauseGroup& withClauseGroup;
 public:
  ClauseGroups(NoSynonymClauseGroup& noSynonymClauseGroup,
                std::vector<ConnectedSynonymClauseGroup>& connectedSynonymClauseGroups,
    WithClauseGroup& withClauseGroup);
  QpsResultTable evaluate(PKBManager& pkbManager, std::unordered_map<std::string, DesignEntityType>& usedDeclarations);

  NoSynonymClauseGroup& getNoSynonynmClauseGroup();
  std::vector<ConnectedSynonymClauseGroup>& getConnectedSynonymClauseGroups();
  WithClauseGroup& getWithClauseGroup();
};
}  // namespace spa

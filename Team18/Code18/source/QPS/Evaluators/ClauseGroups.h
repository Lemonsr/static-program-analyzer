#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <string>

#include "QpsResultTable.h"
#include "NoSynonymClauseGroup.h"
#include "ConnectedSynonymClauseGroup.h"
#include "WithAttrAttrClauseGroup.h"

namespace spa {
class ClauseGroups {
 private:
  NoSynonymClauseGroup& noSynonymClauseGroup;
  std::vector<ConnectedSynonymClauseGroup>& connectedSynonymClauseGroups;
  WithAttrAttrClauseGroup& withAttrAttrClauseGroup;
 public:
  ClauseGroups(NoSynonymClauseGroup& noSynonymClauseGroup,
                std::vector<ConnectedSynonymClauseGroup>& connectedSynonymClauseGroups,
                 WithAttrAttrClauseGroup& withAttrAttrClauseGroup);
  QpsResultTable evaluate(PKBManager& pkbManager, std::unordered_map<std::string, DesignEntityType>& usedDeclarations);

  NoSynonymClauseGroup& getNoSynonynmClauseGroup();
  std::vector<ConnectedSynonymClauseGroup>& getConnectedSynonymClauseGroups();
  WithAttrAttrClauseGroup& getWithAttrAttrClauseGroup();
};
}  // namespace spa

#pragma once
#include <string>
#include <unordered_set>

namespace spa {
class CFGNode {
private:
  int lineNumber;
  std::unordered_set<std::string> modifiedVariables;
  std::unordered_set<CFGNode*> incomingEdges;
  std::unordered_set<CFGNode*> outgoingEdges;

 public:
  explicit CFGNode(int lineNumber);
  int getLineNumber() const;
  void linkTo(CFGNode* node);
  void addModifiedVariable(std::string variable);
  std::unordered_set<std::string> getModifiedVariables() const;
  std::unordered_set<CFGNode*> getIncomingEdges() const;
  std::unordered_set<CFGNode*> getOutgoingEdges() const;
  void addInEdges(std::unordered_set<CFGNode*> edges);
  void addOutEdges(std::unordered_set<CFGNode*> edges);
  void removeIncomingNode(CFGNode* node);
  void removeOutgoingNode(CFGNode* node);
  void removeNodeFromGraph();
};
}  // namespace spa


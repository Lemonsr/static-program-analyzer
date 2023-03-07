#pragma once
#include <string>
#include <unordered_set>

namespace spa {
class CFGNode {
 private:
  int lineNumber = -1;
  std::unordered_set<std::string> modifiedVariables;
  std::unordered_set<CFGNode*> incomingEdges;
  std::unordered_set<CFGNode*> outgoingEdges;

 public:
  CFGNode() = default;
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
  friend bool operator==(const CFGNode& s1, const CFGNode& s2);
  friend bool operator!=(const CFGNode& s1, const CFGNode& s2);
};
}  // namespace spa


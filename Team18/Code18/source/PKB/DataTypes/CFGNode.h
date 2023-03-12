#pragma once

#include <string>
#include <unordered_set>

namespace spa {
class PKBManager;

class CFGNode {
 private:
  int lineNumber = -1;
  bool isDummy = true;
  std::unordered_set<std::string> modifiedVariables;
  std::unordered_set<CFGNode*> incomingEdges;
  std::unordered_set<CFGNode*> outgoingEdges;

 public:
  CFGNode() = default;
  explicit CFGNode(int lineNumber);
  explicit CFGNode(int lineNumber, std::string modifiedVariable);
  explicit CFGNode(int lineNumber, std::unordered_set<std::string> variables);
  static CFGNode createDummyNode();
  bool isDummyNode();
  int getLineNumber() const;
  void linkTo(CFGNode* node);
  void addModifiedVariable(std::string variable);
  std::unordered_set<std::string> getModifiedVariables() const;
  std::unordered_set<CFGNode*> getIncomingEdges() const;
  std::unordered_set<CFGNode*> getOutgoingEdges() const;
  void addIncomingEdges(std::unordered_set<CFGNode*> nodes);
  void addOutgoingEdges(std::unordered_set<CFGNode*> nodes);
  void addIncomingEdge(CFGNode* node);
  void addOutgoingEdge(CFGNode* node);
  void removeIncomingEdge(CFGNode* node);
  void removeOutgoingEdge(CFGNode* node);
  void removeNodeFromGraph();

  friend bool operator==(const CFGNode& s1, const CFGNode& s2);
  friend bool operator!=(const CFGNode& s1, const CFGNode& s2);
};
}  // namespace spa

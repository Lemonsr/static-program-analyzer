#include "CFGNode.h"

#include "PKBManager.h"

spa::CFGNode::CFGNode(int lineNumber) {
  this->lineNumber = lineNumber;
  isDummy = false;
}

spa::CFGNode::CFGNode(int lineNumber, std::string variable) {
  this->lineNumber = lineNumber;
  this->modifiedVariables.insert(variable);
  isDummy = false;
}

spa::CFGNode::CFGNode(int lineNumber, std::unordered_set<std::string> modifiedVariables,
                      std::unordered_set<std::string> usesVariables) {
  this->lineNumber = lineNumber;
  this->modifiedVariables.insert(modifiedVariables.begin(), modifiedVariables.end());
  this->usesVariables.insert(usesVariables.begin(), usesVariables.end());
  isDummy = false;
}

int spa::CFGNode::getLineNumber() const {
  return lineNumber;
}

bool spa::CFGNode::isDummyNode() {
  return isDummy;
}

void spa::CFGNode::linkTo(CFGNode* node) {
  outgoingEdges.insert(node);
  node->incomingEdges.insert(this);
}

void spa::CFGNode::addModifiedVariable(std::string variable) {
  modifiedVariables.insert(variable);
}

std::unordered_set<std::string> spa::CFGNode::getModifiedVariables() const {
  return modifiedVariables;
}

void spa::CFGNode::addUsesVariable(std::string variable) {
  usesVariables.insert(variable);
}

std::unordered_set<std::string> spa::CFGNode::getUsesVariables() const {
  return usesVariables;
}

std::unordered_set<spa::CFGNode*> spa::CFGNode::getIncomingEdges() const {
  return incomingEdges;
}

std::unordered_set<spa::CFGNode*> spa::CFGNode::getOutgoingEdges() const {
  return outgoingEdges;
}

void spa::CFGNode::addIncomingEdges(std::unordered_set<CFGNode*> nodes) {
  incomingEdges.insert(nodes.begin(), nodes.end());
}

void spa::CFGNode::addOutgoingEdges(std::unordered_set<CFGNode*> nodes) {
  outgoingEdges.insert(nodes.begin(), nodes.end());
}

void spa::CFGNode::addIncomingEdge(CFGNode* node) {
  incomingEdges.insert(node);
}

void spa::CFGNode::addOutgoingEdge(CFGNode* node) {
  outgoingEdges.insert(node);
}

void spa::CFGNode::removeIncomingEdge(CFGNode* node) {
  incomingEdges.erase(node);
}

void spa::CFGNode::removeOutgoingEdge(CFGNode* node) {
  outgoingEdges.erase(node);
}

void spa::CFGNode::removeNodeFromGraph() {
  for (auto node : incomingEdges) {
    node->removeOutgoingEdge(this);
  }
  for (auto node : outgoingEdges) {
    node->removeIncomingEdge(this);
  }
}

bool spa::operator==(const CFGNode& s1, const CFGNode& s2) {
  if (s1.lineNumber != s2.lineNumber ||
    s1.modifiedVariables.size() != s2.modifiedVariables.size() ||
    s1.incomingEdges.size() != s2.incomingEdges.size() ||
    s1.outgoingEdges.size() != s2.outgoingEdges.size()) {
    return false;
  }
  if (s1.usesVariables != s2.usesVariables) {
    return false;
  }

  for (auto& itr = s1.modifiedVariables.begin(); itr != s1.modifiedVariables.end(); itr++) {
    if (s2.modifiedVariables.find(*itr) == s2.modifiedVariables.end()) {
      return false;
    }
  }
  for (auto& itr = s1.incomingEdges.begin(); itr != s1.incomingEdges.end(); itr++) {
    if (s2.incomingEdges.find(*itr) == s2.incomingEdges.end()) {
      return false;
    }
  }
  for (auto& itr = s1.outgoingEdges.begin(); itr != s1.outgoingEdges.end(); itr++) {
    if (s2.outgoingEdges.find(*itr) == s2.outgoingEdges.end()) {
      return false;
    }
  }
  return true;
}

bool spa::operator!=(const CFGNode& s1, const CFGNode& s2) {
  return !(s1 == s2);
}

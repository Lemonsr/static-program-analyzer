#include "CFGNode.h"

spa::CFGNode::CFGNode(int lineNumber) {
  this->lineNumber = lineNumber;
}

int spa::CFGNode::getLineNumber() const {
  return lineNumber;
}

void spa::CFGNode::linkTo(CFGNode* edge) {
  outgoingEdges.insert(edge);
  edge->incomingEdges.insert(this);
}

void spa::CFGNode::addModifiedVariable(std::string variable) {
  modifiedVariables.insert(variable);
}

std::unordered_set<std::string> spa::CFGNode::getModifiedVariables() const {
  return modifiedVariables;
}

std::unordered_set<spa::CFGNode*> spa::CFGNode::getIncomingEdges() const {
  return incomingEdges;
}

std::unordered_set<spa::CFGNode*> spa::CFGNode::getOutgoingEdges() const {
  return outgoingEdges;
}

void spa::CFGNode::addInEdges(std::unordered_set<CFGNode*> edges) {
  incomingEdges.insert(edges.begin(), edges.end());
}

void spa::CFGNode::addOutEdges(std::unordered_set<CFGNode*> edges) {
  outgoingEdges.insert(edges.begin(), edges.end());
}

void spa::CFGNode::addIncomingEdge(CFGNode* incomingEdge) {
  incomingEdges.insert(incomingEdge);
}

void spa::CFGNode::addOutgoingEdge(CFGNode* outgoingEdge) {
  outgoingEdges.insert(outgoingEdge);
}

void spa::CFGNode::removeIncomingEdge(CFGNode* incomingEdge) {
  incomingEdges.erase(incomingEdge);
}

void spa::CFGNode::removeOutgoingEdge(CFGNode* outgoingEdge) {
  outgoingEdges.erase(outgoingEdge);
}

void spa::CFGNode::removeNodeFromGraph() {
  for (auto edge : incomingEdges) {
    edge->removeOutgoingEdge(this);
  }
  for (auto edge : outgoingEdges) {
    edge->removeIncomingEdge(this);
  }
}

bool spa::operator==(const CFGNode& s1, const CFGNode& s2) {
  if (s1.lineNumber != s2.lineNumber ||
      s1.modifiedVariables.size() != s2.modifiedVariables.size() ||
      s1.incomingEdges.size() != s2.incomingEdges.size() ||
      s1.outgoingEdges.size() != s2.outgoingEdges.size()) {
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

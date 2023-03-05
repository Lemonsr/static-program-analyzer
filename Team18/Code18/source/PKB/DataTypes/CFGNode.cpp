#include "CFGNode.h"

#include "PKBManager.h"

spa::CFGNode::CFGNode(int lineNumber) {
  this->lineNumber = lineNumber;
}

spa::CFGNode::CFGNode(int lineNumber, bool isDummy) {
  this->lineNumber = lineNumber;
  this->isDummy = isDummy;
}

spa::CFGNode::CFGNode(int lineNumber, std::string variable) {
  this->lineNumber = lineNumber;
  modifiedVariables.insert(variable);
}

spa::CFGNode* spa::CFGNode::createDummyNode() {
  return new CFGNode(-1, true);
}

int spa::CFGNode::getLineNumber() const {
  return lineNumber;
}

bool spa::CFGNode::isDummyNode() {
  return isDummy;
}

void spa::CFGNode::linkNodes(CFGNode* tail, CFGNode* head, PKBManager& pkbManager) {
  std::string headStrLineNum = std::to_string(head->getLineNumber());
  if (tail->isDummyNode()) {
    std::unordered_set<CFGNode*> nodesBeforeDummyNode = tail->getIncomingEdges();
    for (CFGNode* node : nodesBeforeDummyNode) {
      node->removeOutgoingNode(tail);
      node->linkTo(head);
      std::string currNodeStrLineNum = std::to_string(node->getLineNumber());
      pkbManager.addRelationship(NEXT, currNodeStrLineNum, headStrLineNum);
    }
    delete tail;
    return;
  }
  std::string tailStrLineNum = std::to_string(tail->getLineNumber());
  tail->linkTo(head);
  pkbManager.addRelationship(NEXT, tailStrLineNum, headStrLineNum);
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

void spa::CFGNode::removeIncomingNode(CFGNode* node) {
  incomingEdges.erase(node);
}

void spa::CFGNode::removeOutgoingNode(CFGNode* node) {
  outgoingEdges.erase(node);
}

void spa::CFGNode::removeNodeFromGraph() {
  for (auto node : incomingEdges) {
    node->removeOutgoingNode(this);
  }
  for (auto node : outgoingEdges) {
    node->removeIncomingNode(this);
  }
}

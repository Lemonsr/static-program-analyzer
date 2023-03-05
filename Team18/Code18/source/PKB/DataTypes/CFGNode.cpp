#include "CFGNode.h"


spa::CFGNode::CFGNode(int lineNumber) {
    this->lineNumber = lineNumber;
}

int spa::CFGNode::getLineNumber() const {
    return lineNumber;
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

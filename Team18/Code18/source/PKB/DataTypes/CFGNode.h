#pragma once
#include <string>
#include <unordered_set>


namespace spa {
class PKBManager;

class CFGNode {
 public:
    explicit CFGNode(int lineNumber);
    explicit CFGNode(int lineNumber, bool isDummy);
    explicit CFGNode(int lineNumber, std::string modifiedVariable);
    static CFGNode* createDummyNode();
    bool isDummyNode();
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

    static void linkNodes(CFGNode* tail, CFGNode* head, PKBManager& pkbManager);

 private:
    int lineNumber;
    bool isDummy = false;
    std::unordered_set<std::string> modifiedVariables;
    std::unordered_set<CFGNode*> incomingEdges;
    std::unordered_set<CFGNode*> outgoingEdges;
};;
}  // namespace spa


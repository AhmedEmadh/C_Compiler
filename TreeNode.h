#pragma once    
#include <iostream>
#include <string>
#include <vector>

class TreeNode {
private:
    TreeNode* parent; // This variable represents the parent node of the current node.
    TreeNode* sibling; // This variable represents the next sibling node of the current node.
    std::vector<TreeNode*> children; // This vector stores pointers to the child nodes of the current node.
    static int nodeID; // This static variable is used to generate unique identifiers for each node.
    std::string nodeName; // This variable holds the name of the node.

public:
    std::string desc; // This variable stores a description or label associated with the node.

    TreeNode(std::string d); // Constructor
    void addChild(TreeNode* x); // This method adds a child node (x) to the list of children of the current node.
    void setSibling(TreeNode* sibling); // This method sets the next sibling node of the current node.
    void setDesc(std::string desc); // This method sets the description of the node.
    std::string getNodeName(); // This method returns the name of the node.
    std::string getDesc(); // This method returns the description of the node.
    std::vector<TreeNode*> getChildren(); // This method returns the children of the node.
    bool hasChildren(); // This method returns true if the node has children, false otherwise.
    
};
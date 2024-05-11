#include "TreeNode.h"


//tree node class implementation

#include "TreeNode.h"

int TreeNode::nodeID = 0;

TreeNode::TreeNode(std::string d) {
    desc = d;
    nodeName = "Node" + std::to_string(nodeID++);
}

void TreeNode::addChild(TreeNode* x) {
    children.push_back(x);
}

void TreeNode::setSibling(TreeNode* sibling) {
    this->sibling = sibling;
}

void TreeNode::setDesc(std::string desc) {
    this->desc = desc;
}

std::string TreeNode::getNodeName() {
    return nodeName;
}

std::string TreeNode::getDesc() {
    return desc;
}
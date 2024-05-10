//this code is implimintation of the parser(syntax analyzer) according to grammar.txt
#pragma once
#include <iostream>
#include <deque>

//parse tree node
class Node {
public:
    bool is_terminal;
    std::string label;
    std::deque<Node*> children;
    Node(std::string l, bool t) : label(l), is_terminal(t) {}
};
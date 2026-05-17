#include <iostream>
#include <vector>
#include <string>
#include "domain/tree.hpp"
using namespace std;

Node::Node(string value, vector<Node *> children, int priority)
{
    this->value = value;
    this->children = children;
    this->priority = priority;
}

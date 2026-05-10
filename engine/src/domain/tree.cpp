#include <iostream>
#include <vector>
#include <string>
#include "tree.hpp"
using namespace std;

Node::Node(string value, vector<Node *> children)
{
    this->value = value;
    this->children = children;
}

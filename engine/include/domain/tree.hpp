#pragma once

#include <iostream>
#include <vector>
using namespace std;

class Node
{
public:
    string value;
    vector<Node *> children;

    Node(string value, vector<Node *> children);
};

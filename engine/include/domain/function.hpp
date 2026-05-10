#pragma once

#include <iostream>
#include <vector>
#include "tree.hpp"
using namespace std;

class Plus : public Node
{
public:
    Plus(vector<Node *> children);

    string display();
};
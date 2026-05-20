#pragma once

#include <iostream>
#include <vector>
#include <string>

class Node
{
    public:
        std::string value;
        std::vector<Node *> children;

        Node(std::string value, std::vector<Node *> children);
};

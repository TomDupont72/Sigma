#include <iostream>
#include <vector>
#include "tree.hpp"
#include "function.hpp"
using namespace std;

Plus::Plus(vector<Node *> children) : Node("+", children) {}

string Plus::display()
{
    string res = "";
    int n = this->children.size();

    for (int i = 0; i < n; i++)
    {
        res += (*this->children[i]).value;

        if (i < n - 1)
        {
            res += " " + this->value + " ";
        }
    }

    return res;
}
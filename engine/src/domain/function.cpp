#include <iostream>
#include <vector>
#include "domain/tree.hpp"
#include "domain/function.hpp"
using namespace std;

Plus::Plus(vector<Node *> children) : Node("+", children, 1) {}

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

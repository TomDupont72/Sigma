#include "domain/tree.hpp"
#include "algorithms/utils.hpp"
#include<string>

using namespace std;

string displayExpression(Node * node)
{
    if (node->children.empty()) return node->value;

    if (node->value == "+" || node->value == "*")
    {
        vector<string> parts;

        for (Node * child: node->children)
        {
            parts.push_back(displayExpression(child));
        }

        return join(parts, " " + node->value + " ");
    }

    if (node->children.size() == 1)
    {
        return node->value + "(" + displayExpression(node->children[0]) + ")";
    }

    return "";
}

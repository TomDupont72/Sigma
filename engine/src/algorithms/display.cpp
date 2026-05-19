#include "domain/tree.hpp"
#include "algorithms/utils.hpp"
#include "algorithms/display.hpp"
#include<string>

using namespace std;

string displayExpression(Node * node, int parentPriority)
{
    string res = "";

    if (node->children.empty()) return node->value;

    if (node->value == "+" || node->value == "*")
    {
        vector<string> parts;

        for (Node * child: node->children)
        {
            parts.push_back(displayExpression(child, node->priority));
        }

        res = join(parts, " " + node->value + " ");
    }

    if (node->children.size() == 1)
    {
        res = node->value + displayExpression(node->children[0], node->priority);
    }

    if (node->priority < parentPriority) return "(" + res + ")";

    return res;
}

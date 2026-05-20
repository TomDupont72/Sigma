#include "domain/tree.hpp"
#include "algorithms/utils.hpp"
#include "algorithms/display.hpp"
#include "types/tokenTypes.hpp"
#include<string>

using namespace std;

string displayExpression(Node * node, int parentPriority)
{
    string res = "";

    if (node->children.empty()) return node->value;

    if (node->value == "+")
    {
        vector<string> parts;

        for (Node * child: node->children)
        {
            if (child->children.empty() && child->value == neutralElement[node->value]) continue;
            parts.push_back(displayExpression(child, priority(node)));
        }

        if (parts.empty()) return neutralElement[node->value];
        res = join(parts, "" + node->value + "");
    }

    if (node->value == "*")
    {
        vector<string> parts;

        for (Node * child: node->children)
        {
            if (child->children.empty() && child->value == neutralElement[node->value]) continue;
            parts.push_back(displayExpression(child, priority(node)));
        }

        if (parts.empty()) return neutralElement[node->value];
        res = join(parts, "");
    }

    if (node->value == "^")
    {
        string base = displayExpression(node->children[0], priority(node));
        string exponent = displayExpression(node->children[1], priority(node));

        res = base + "^{" + exponent + "}";
    }

    if (node->children.size() == 1)
    {
        res = node->value + "(" + displayExpression(node->children[0], 0) + ")";
    }

    if (priority(node) < parentPriority) return "(" + res + ")";

    return res;
}

int priority(Node * node)
{
    if (node->value == "+") return 1;

    if (node -> value == "*") return 2;

    if (node -> value == "^") return 3;

    return 100;
}

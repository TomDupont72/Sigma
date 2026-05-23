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

            bool isNegativeTerm = child->value == "*" && !child->children.empty() && child->children[0]->value == "-1";

            if (isNegativeTerm)
            {
                vector <Node *> rest(child->children.begin() + 1, child->children.end());
                Node* positivePart = rest.size() == 1 ? rest[0] : new Node("*", rest);

                parts.push_back("-" + displayExpression(positivePart, priority(node)));
            }
            else parts.push_back(displayExpression(child, priority(node)));
        }

        if (parts.empty()) return neutralElement[node->value];
        res = joinSum(parts);
    }

    if (node->value == "*")
    {
        vector<string> numeratorParts;
        vector<string> denominatorParts;

        for (Node* child : node->children)
        {
            if (child->children.empty() && child->value == neutralElement[node->value])
                continue;

            bool isInverseTerm = child->value == "^" && child->children.size() == 2 && child->children[1]->children.empty() && child->children[1]->value == "-1";
            if (isInverseTerm)
            {
                Node* denominator = child->children[0];
                denominatorParts.push_back(displayExpression(denominator, 0));
            }
            else
            {
                numeratorParts.push_back(displayExpression(child, priority(node)));
            }
        }

        if (numeratorParts.empty())
            numeratorParts.push_back("1");

        if (denominatorParts.empty())
            res = join(numeratorParts, "");
        else
            res = "\\frac{" + join(numeratorParts, "") + "}{" + join(denominatorParts, "") + "}";
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

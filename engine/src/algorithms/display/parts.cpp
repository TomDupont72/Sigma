#include "algorithms/display/parts.hpp"
#include "algorithms/utils.hpp"
#include "domain/tree.hpp"
#include "types/tokenTypes.hpp"
#include <string>
#include <vector>

using namespace std;

string displaySum(Node *node)
{
    vector<string> parts;

    for (Node *child : node->children)
    {
        if (child->children.empty() && child->value == neutralElement[node->value])
            continue;

        bool isNegativeTerm = child->value == "*" && !child->children.empty() && child->children[0]->value == "-1";

        if (isNegativeTerm)
        {
            vector<Node *> rest(child->children.begin() + 1, child->children.end());
            Node *positivePart = rest.size() == 1 ? rest[0] : new Node("*", rest);

            parts.push_back("-" + displayExpression(positivePart, priority(node)));
        }
        else
            parts.push_back(displayExpression(child, priority(node)));
    }

    if (parts.empty())
        return neutralElement[node->value];

    return joinSum(parts);
}

string displayProduct(Node *node)
{
    vector<Node *> numeratorNodes;
    vector<string> numeratorParts;
    vector<string> denominatorParts;

    for (Node *child : node->children)
    {
        if (child->children.empty() && child->value == neutralElement[node->value])
            continue;

        bool isInverseTerm = child->value == "^" && child->children.size() == 2 && child->children[1]->children.empty() && isNumber(child->children[1]->value) && stof(child->children[1]->value) < 0;
        if (isInverseTerm)
        {
            Node *base = child->children[0];
            float exponent = -stof(child->children[1]->value);

            if (exponent == 1)
                denominatorParts.push_back(displayExpression(base, 0));
            else
                denominatorParts.push_back(displayExpression(new Node("^", {base, new Node(numberToString(exponent), {})}), 0));
        }
        else
        {
            numeratorNodes.push_back(child);
        }
    }

    int negativeFactors = 0;

    for (Node *child : numeratorNodes)
    {
        if (child->children.empty() && child->value == "-1")
            negativeFactors++;
        else
        {
            int childPriority = denominatorParts.empty() || numeratorNodes.size() > 1 ? priority(node) : 0;
            numeratorParts.push_back(displayExpression(child, childPriority));
        }
    }

    if (numeratorParts.empty())
        numeratorParts.push_back("1");

    string res;

    if (denominatorParts.empty())
        res = join(numeratorParts, "");
    else
        res = "\\displaystyle\\frac{" + join(numeratorParts, "") + "}{" + join(denominatorParts, "") + "}";

    if (negativeFactors % 2 == 1)
        res = "-" + res;

    return res;
}

string displayPower(Node *node)
{
    if (node->children[1]->children.empty() && isNumber(node->children[1]->value) && stof(node->children[1]->value) < 0)
    {
        Node *base = node->children[0];
        float exponent = -stof(node->children[1]->value);

        if (exponent == 1)
            return "\\displaystyle\\frac{1}{" + displayExpression(base, 0) + "}";

        return "\\displaystyle\\frac{1}{" + displayExpression(new Node("^", {base, new Node(numberToString(exponent), {})}), 0) + "}";
    }

    string base = displayExpression(node->children[0], priority(node));
    string exponent = displayExpression(node->children[1], priority(node));

    return base + "^{" + exponent + "}";
}

string displayUnaryFunction(Node *node, string name)
{
    return name + "\\left(" + displayExpression(node->children[0], 0) + "\\right)";
}

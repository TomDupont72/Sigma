#include "domain/tree.hpp"
#include "algorithms/utils.hpp"
#include "algorithms/display.hpp"
#include "types/tokenTypes.hpp"
#include <string>

using namespace std;

string displayExpression(Node *node, int parentPriority)
{
    string res = "";

    if (node->value == "+")
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
        res = joinSum(parts);
    }

    else if (node->value == "*")
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

        for (Node *child : numeratorNodes)
            numeratorParts.push_back(displayExpression(child, denominatorParts.empty() ? priority(node) : 0));

        if (numeratorParts.empty())
            numeratorParts.push_back("1");

        if (denominatorParts.empty())
            res = join(numeratorParts, "");
        else
            res = "\\displaystyle\\frac{" + join(numeratorParts, "") + "}{" + join(denominatorParts, "") + "}";
    }

    else if (node->value == "^")
    {
        if (node->children[1]->children.empty() && isNumber(node->children[1]->value) && stof(node->children[1]->value) < 0)
        {
            Node *base = node->children[0];
            float exponent = -stof(node->children[1]->value);

            if (exponent == 1)
                res = "\\displaystyle\\frac{1}{" + displayExpression(base, 0) + "}";
            else
                res = "\\displaystyle\\frac{1}{" + displayExpression(new Node("^", {base, new Node(numberToString(exponent), {})}), 0) + "}";
        }
        else
        {
            string base = displayExpression(node->children[0], priority(node));
            string exponent = displayExpression(node->children[1], priority(node));

            res = base + "^{" + exponent + "}";
        }
    }

    else if (node->value == "ln")
    {
        res = "\\textup{ln}\\left(" + displayExpression(node->children[0], 0) + "\\right)";
    }

    else if (node->value == "exp")
    {
        res = "\\textup{e}^{" + displayExpression(node->children[0], 0) + "}";
    }

    else if (node->value == "cos")
    {
        res = "\\textup{cos}\\left(" + displayExpression(node->children[0], 0) + "\\right)";
    }

    else if (node->value == "sin")
    {
        res = "\\textup{sin}\\left(" + displayExpression(node->children[0], 0) + "\\right)";
    }

    else if (node->value == "sum")
    {
        res = "\\displaystyle\\sum_{" + node->children[0]->value + "=" + displayExpression(node->children[1], 0) + "}^{" + displayExpression(node->children[2], 0) + "}" + displayExpression(node->children[3], 0);
    }

    else if (node->value == "prod")
    {
        res = "\\displaystyle\\prod_{" + node->children[0]->value + "=" + displayExpression(node->children[1], 0) + "}^{" + displayExpression(node->children[2], 0) + "}" + displayExpression(node->children[3], 0);
    }

    else if (node->value == "binom")
    {
        res = "\\displaystyle\\binom{" + displayExpression(node->children[0], 0) + "}{" + displayExpression(node->children[1], 0) + "}";
    }

    else if (node->value == "pi")
    {
        res = "\\pi";
    }

    else if (node->children.empty())
        return node->value;

    else if (node->children.size() == 1)
    {
        res = node->value + "\\left(" + displayExpression(node->children[0], 0) + "\\right)";
    }

    if (priority(node) < parentPriority)
        return "\\left(" + res + "\\right)";

    return res;
}

int priority(Node *node)
{
    if (node->value == "+")
        return 1;

    if (node->value == "*" || node->value == "sum" || node->value == "prod")
        return 2;

    if (node->value == "^")
        return 3;

    return 100;
}

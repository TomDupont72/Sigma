#include "algorithms/display.hpp"
#include "algorithms/display/parts.hpp"
#include "algorithms/utils.hpp"
#include "domain/tree.hpp"
#include <string>

using namespace std;

string displayExpression(Node *node, int parentPriority)
{
    string res = "";

    if (node->value == "+")
        res = displaySum(node);

    else if (node->value == "*")
        res = displayProduct(node);

    else if (node->value == "^")
        res = displayPower(node);

    else if (node->value == "ln")
        res = displayUnaryFunction(node, "\\textup{ln}");

    else if (node->value == "exp")
        res = "\\textup{e}^{" + displayExpression(node->children[0], 0) + "}";

    else if (node->value == "cos")
        res = displayUnaryFunction(node, "\\textup{cos}");

    else if (node->value == "sin")
        res = displayUnaryFunction(node, "\\textup{sin}");

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

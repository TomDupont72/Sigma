#include "algorithms/simplify/internal.hpp"
#include "algorithms/utils.hpp"
#include "domain/tree.hpp"
#include <vector>

using namespace std;

Node *applyRewriteRules(Node *node)
{
    for (Node *&child : node->children)
        child = applyRewriteRules(child);

    if (node->value == "+")
    {
        if ((node->children[0])->value == "0" && node->children.size() == 2)
            return node->children[1];
        if (Node *res = rewriteSumRules(node))
            return res;
    }

    if (node->value == "*")
    {
        if ((node->children[0])->value == "0")
            return new Node("0", {});
    }

    if (node->value == "^")
    {
        if ((node->children[1])->value == "0")
            return new Node("1", {});
    }

    if (node->value == "ln")
    {
        if ((node->children[0])->value == "1")
            return new Node("0", {});
        if ((node->children[0])->value == "exp")
            return (node->children[0])->children[0];
    }

    if (node->value == "exp")
    {
        if ((node->children[0])->value == "0")
            return new Node("1", {});
        if ((node->children[0])->value == "ln")
            return (node->children[0])->children[0];
    }

    if (node->value == "sin")
    {
        if (node->children[0]->value == "0")
            return new Node("0", {});
        if (isExactDivision(node->children[0], new Node("pi", {}), new Node("2", {})))
            return new Node("1", {});
        if (node->children[0]->value == "pi")
            return new Node("0", {});
        if (isExactDivision(node->children[0], new Node("*", {new Node("3", {}), new Node("pi", {})}), new Node("2", {})))
            return new Node("-1", {});
    }

    if (node->value == "cos")
    {
        if (node->children[0]->value == "0")
            return new Node("1", {});
        if (isExactDivision(node->children[0], new Node("pi", {}), new Node("2", {})))
            return new Node("0", {});
        if (node->children[0]->value == "pi")
            return new Node("-1", {});
        if (isExactDivision(node->children[0], new Node("*", {new Node("3", {}), new Node("pi", {})}), new Node("2", {})))
            return new Node("0", {});
    }

    if (node->value == "sum")
    {
        if (isNumber(node->children[3]->value))
        {
            return new Node("*", {node->children[3], new Node("+", {node->children[2], new Node("*", {new Node("-1", {}), node->children[1]}), new Node("1", {})})});
        }
        if (node->children[3]->value == "^" && node->children[3]->children[1]->value == node->children[0]->value)
        {
            Node *numerator = new Node("+", {power(node->children[3]->children[0], node->children[1]), negative(power(node->children[3]->children[0], addConstant(node->children[2], "1")))});
            Node *denominator = new Node("+", {new Node("1", {}), negative(node->children[3]->children[0])});
            return new Node("*", {numerator, inverse(denominator)});
        }
    }

    if (node->value == "prod")
    {
        if (isNumber(node->children[3]->value))
        {
            return new Node("^", {node->children[3], new Node("+", {node->children[2], new Node("*", {new Node("-1", {}), node->children[1]}), new Node("1", {})})});
        }
    }

    if (node->value == "binom")
    {
        if (isNumber(node->children[0]->value) && isNumber(node->children[1]->value))
        {
            return new Node(numberToString(binom(stof(node->children[0]->value), stof(node->children[1]->value))), {});
        }
    }

    return node;
}

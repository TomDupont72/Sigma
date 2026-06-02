#include "algorithms/derive.hpp"
#include "algorithms/derive/rules.hpp"
#include "algorithms/utils.hpp"
#include "domain/tree.hpp"

using namespace std;

Node *derive(Node *node, string variable)
{
    if (node->value == "+")
        node = derivePlus(node, variable);
    else if (node->value == "*")
        node = deriveMultiply(node, variable);
    else if (node->value == "^")
        node = derivePower(node, variable);
    else if (isNumber(node->value))
        node = deriveNumber(node);
    else if (node->value == "ln")
        node = deriveLn(node, variable);
    else if (node->value == "exp")
        node = deriveExp(node, variable);
    else if (node->value == "sin")
        node = deriveSin(node, variable);
    else if (node->value == "cos")
        node = deriveCos(node, variable);
    else if (node->value == "sum")
        node = deriveSum(node, variable);
    else
        node = deriveIdentifier(node, variable);

    return node;
}

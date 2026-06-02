#include "algorithms/simplify.hpp"
#include "algorithms/simplify/internal.hpp"
#include "algorithms/display.hpp"
#include "domain/tree.hpp"
#include <string>
#include <vector>

using namespace std;

int MAX_ITERATION = 100;

Node *normalize(Node *node)
{
    Node *current = node;
    int current_iteration = 1;

    while (true)
    {
        string before = displayExpression(current, 0);

        current = simplify(current);
        current = applyRewriteRules(current);

        string after = displayExpression(current, 0);

        if (before == after)
            break;

        current_iteration++;
    }

    return current;
}

Node *simplify(Node *node)
{
    if (node->children.empty())
        return node;

    vector<Node *> simplifiedChildren;

    for (Node *child : node->children)
        simplifiedChildren.push_back(simplify(child));

    node->children = simplifiedChildren;

    if (node->value == "+" || node->value == "*" || node->value == "^")
        node = simplifyOperation(node, node->value);

    return node;
}

#include "algorithms/derive.hpp"
#include "algorithms/derive/rules.hpp"
#include "algorithms/utils.hpp"
#include "domain/tree.hpp"
#include <vector>

using namespace std;

Node *deriveNumber(Node *node)
{
    return new Node("0", {});
}

Node *deriveIdentifier(Node *node, string value)
{
    if (node->value == value)
        return new Node("1", {});
    return new Node("0", {});
}

Node *derivePlus(Node *node, string variable)
{
    vector<Node *> resChildren;

    for (Node *child : node->children)
    {
        resChildren.push_back(derive(child, variable));
    }

    return new Node("+", resChildren);
}

Node *deriveMultiply(Node *node, string variable)
{
    vector<Node *> resChildren;

    for (size_t i = 0; i < node->children.size(); i++)
    {
        vector<Node *> productChildren = copyWithoutIndex(node->children, i);
        productChildren.push_back(derive(node->children[i], variable));

        resChildren.push_back(new Node("*", productChildren));
    }

    return new Node("+", resChildren);
}

Node *derivePower(Node *node, string variable)
{
    Node *firstTerm = new Node("*", {derive(node->children[1], variable), new Node("ln", {node->children[0]})});
    Node *secondTerm = new Node("*", {node->children[1], derive(node->children[0], variable), new Node("^", {node->children[0], new Node("-1", {})})});

    return new Node("*", {new Node("+", {firstTerm, secondTerm}), node});
}

Node *deriveLn(Node *node, string variable)
{
    return new Node("*", {derive(node->children[0], variable), new Node("^", {node->children[0], new Node("-1", {})})});
}

Node *deriveExp(Node *node, string variable)
{
    return new Node("*", {derive(node->children[0], variable), node});
}

Node *deriveSin(Node *node, string variable)
{
    return new Node("*", {derive(node->children[0], variable), new Node("cos", {node->children[0]})});
}

Node *deriveCos(Node *node, string variable)
{
    return new Node("*", {derive(node->children[0], variable), new Node("*", {new Node("-1", {}), new Node("sin", {node->children[0]})})});
}

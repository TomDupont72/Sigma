#include "domain/tree.hpp"
#include "algorithms/simplify.hpp"
#include "algorithms/utils.hpp"
#include "algorithms/display.hpp"
#include<vector>
#include<map>


Node * simplify(Node * node)
{
    if (node->children.empty()) return node;

    vector<Node *> simplifiedChildren;

    for (Node * child: node->children) simplifiedChildren.push_back(simplify(child));

    node->children = simplifiedChildren;

    if (node->value == "+") node = simplifyPlus(node);

    return node;
}

Node * simplifyPlus(Node * node)
{
    vector<Node *> resChildren;
    map<string, int> sumMapping;
    float sumNumber = 0;
    float coeff = 1;

    vector<Node *> terms;
    collectSumTerms(node, terms);

    map<string, Node *> symbolicMapping;

    for (Node * child: terms)
    {
        if (isNumber(child->value)) sumNumber += stof(child->value);
        else if (child->value != "*") sumMapping[child->value] ++;
        else
        {
            coeff = stof((child->children[0])->value);
            vector<Node *> nonNumericFactors((child->children).begin() + 1, (child->children).end());

            Node * symbolicPart = new Node("*", nonNumericFactors, 2);

            string key = displayExpression(symbolicPart, 0);

            sumMapping[key] += coeff;
            symbolicMapping[key] = symbolicPart;
        }
    }

    for (const auto& pair: sumMapping)
    {
        if (symbolicMapping.contains(pair.first))
        {
            Node * symbolicPart;
            vector<Node *> nonNumericFactors = (symbolicMapping[pair.first])->children;

            if (nonNumericFactors.size() == 1) symbolicPart = nonNumericFactors[0];
            else symbolicPart = new Node("*", nonNumericFactors, 2);

            resChildren.push_back(new Node("*", {new Node(numberToString(sumMapping[pair.first]), {}, 100), symbolicPart}, 2));
        }
        else
        {
            if (pair.second == 1) resChildren.push_back(new Node(pair.first, {}, 100));
            else resChildren.push_back(new Node("*", {new Node(numberToString(pair.second), {}, 100), new Node(pair.first, {}, 100)}, 2));
        }
    }

    if (sumNumber != 0) resChildren.push_back(new Node(numberToString(sumNumber), {}, 100));

    if (resChildren.size() == 1) node = resChildren[0];
    else node->children = resChildren;

    return node;
}

void collectSumTerms(Node * node, vector<Node *>& terms)
{
    if (node->value == "+")
    {
        for (Node * child: node->children) collectSumTerms(child, terms);
    }
    else terms.push_back(node);
}

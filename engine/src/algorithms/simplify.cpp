#include "domain/tree.hpp"
#include "algorithms/simplify.hpp"
#include "algorithms/utils.hpp"
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
    vector<Node *> resChildren = {};
    map<string, int> sumMapping = {};
    float sumNumber = 0;

    for (Node * child: node->children)
    {
        if (isNumber(child->value)) sumNumber += stof(child->value);

        if (child->value == "+") resChildren.insert(resChildren.end(), child->children.begin(), child->children.end());
        else if (child->value != "*")
        {
            if (sumMapping.contains(child->value)) sumMapping[child->value] ++;
            else sumMapping[child->value] = 1;
        } else resChildren.push_back(child); // Faire la logique
    }

    for (const auto& pair: sumMapping)
    {
        cout << pair.first + " : " + numberToString(pair.second) + "\n";
        if (pair.second == 1) resChildren.push_back(new Node(pair.first, {}, 100));
        else resChildren.push_back(new Node("*", {new Node(numberToString(pair.second), {}, 100), new Node(pair.first, {}, 100)}, 2));
    }

    if (sumNumber != 0) resChildren.push_back(new Node(numberToString(sumNumber), {}, 100));

    if (resChildren.size() == 1) node = resChildren[0];
    else node->children = resChildren;

    return node;
}

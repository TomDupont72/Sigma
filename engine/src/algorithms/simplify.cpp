#include "domain/tree.hpp"
#include "algorithms/simplify.hpp"
#include "algorithms/utils.hpp"
#include "algorithms/display.hpp"
#include "types/tokenTypes.hpp"
#include<vector>
#include<map>

int MAX_ITERATION = 100;

Node * normalize(Node * node)
{
    Node * current = node;
    int current_iteration = 1;

    while(true)
    {
        string before = displayExpression(current, 0);

        current = simplify(current);
        current = applyRewriteRules(current);

        string after = displayExpression(current, 0);

        if (before == after) break;

        current_iteration++;
    }

    return current;
}

Node * simplify(Node * node)
{
    if (node->children.empty()) return node;

    vector<Node *> simplifiedChildren;

    for (Node * child: node->children) simplifiedChildren.push_back(simplify(child));

    node->children = simplifiedChildren;

    if (node->value == "+" || node->value == "*") node = simplifyOperation(node,node->value);

    return node;
}

Node * simplifyOperation(Node * node, string value)
{
    vector<Node *> terms;
    vector<Node *> resChildren;

    collectTerms(node, terms, value);

    if (node->value == "+")
    {
        auto [mapping, symbolicMapping, number] = createMappings(terms, nextValue[value]);
        resChildren = constructChildren(mapping, symbolicMapping, number, nextValue[value]);
    }
    else
    {
        auto [mapping, symbolicMapping, number] = createProductMappings(terms, nextValue[value]);
        resChildren = constructPowerChildren(mapping, symbolicMapping, number);
    }

    if (resChildren.size() == 1) node = resChildren[0];
    else node->children = resChildren;

    return node;
}

void collectTerms(Node * node, vector<Node *>& terms, string value)
{
    if (node->value == value)
    {
        for (Node * child: node->children) collectTerms(child, terms, value);
    }
    else terms.push_back(node);
}

tuple<map<string, float>, map<string, Node *>, float> createMappings(vector<Node *> terms, string value)
{
    float number = 0;

    map<string, float> mapping;
    map<string, Node *> symbolicMapping;

    for (Node * child: terms)
    {
        if (isNumber(child->value)) number = operation(number, stof(child->value), previousValue[value]);
        else if (child->value != value) {
            string key = displayExpression(child, 0);

            mapping[key] ++;
            symbolicMapping[key] = child;
        }
        else
        {
            float coeff = stof((child->children[0])->value);
            vector<Node *> nonNumericFactors((child->children).begin() + 1, (child->children).end());

            Node * symbolicPart;

            if (nonNumericFactors.size() == 1) symbolicPart = nonNumericFactors[0];
            else symbolicPart = new Node(value, nonNumericFactors);

            string key = displayExpression(symbolicPart, 0);

            mapping[key] += coeff;
            symbolicMapping[key] = symbolicPart;
        }
    }

    return {mapping, symbolicMapping, number};
}

tuple<map<string, float>, map<string, Node *>, float> createProductMappings(vector<Node *> terms, string value)
{
    float number = 1;

    map<string, float> mapping;
    map<string, Node *> symbolicMapping;

    for (Node* child : terms)
    {
        if (isNumber(child->value))
        {
            number *= stof(child->value);
        }
        else if (child->value == "^")
        {
            Node* base = child->children[0];
            Node* exponent = child->children[1];

            string key = displayExpression(base, 0);
            mapping[key] += stof(exponent->value);
            symbolicMapping[key] = base;
        }
        else
        {
            string key = displayExpression(child, 0);
            mapping[key] += 1;
            symbolicMapping[key] = child;
        }
    }

    return {mapping, symbolicMapping, number};
}

vector<Node *> constructChildren(map<string, float> mapping, map<string, Node *> symbolicMapping, float number, string value)
{
    vector<Node *> resChildren;

    resChildren.push_back(new Node(numberToString(number), {}));

    for (const auto& pair: mapping)
    {
        if (symbolicMapping.contains(pair.first))
        {
            Node * symbolicPart = symbolicMapping[pair.first];

            if (pair.second == stof(neutralElement[value])) resChildren.push_back(symbolicPart);
            else resChildren.push_back(new Node(value, {new Node(numberToString(pair.second), {}), symbolicPart}));
        }
        else
        {
            if (pair.second == 1) resChildren.push_back(new Node(pair.first, {}));
            else resChildren.push_back(new Node(value, {new Node(numberToString(pair.second), {}), new Node(pair.first, {})}));
        }
    }

    return resChildren;
}

vector<Node *> constructPowerChildren(map<string, float> mapping, map<string, Node *> symbolicMapping, float number)
{
    vector<Node *> resChildren;

    resChildren.push_back(new Node(numberToString(number), {}));

    for (const auto& pair: mapping)
    {
        if (symbolicMapping.contains(pair.first))
        {
            Node * symbolicPart = symbolicMapping[pair.first];

            if (pair.second == 1) resChildren.push_back(symbolicPart);
            else resChildren.push_back(new Node("^", {symbolicPart, new Node(numberToString(pair.second), {})}));
        }
        else
        {
            if (pair.second == 1) resChildren.push_back(new Node(pair.first, {}));
            else resChildren.push_back(new Node("^", {new Node(pair.first, {}), new Node(numberToString(pair.second), {})}));
        }
    }

    return resChildren;
}

Node * applyRewriteRules(Node * node)
{
    for (Node*& child : node->children) child = applyRewriteRules(child);

    if (node->value == "*")
    {
        if((node->children[0])->value == "0") return new Node("0", {});
    }

    if (node->value == "ln")
    {
        if((node->children[0])->value == "1") return new Node("0", {});
    }

    return node;
}

#include "algorithms/simplify/internal.hpp"
#include "algorithms/display.hpp"
#include "algorithms/utils.hpp"
#include "domain/tree.hpp"
#include "types/tokenTypes.hpp"
#include <cmath>
#include <map>
#include <numeric>
#include <tuple>
#include <vector>

using namespace std;

Node *simplifyOperation(Node *node, string value)
{
    vector<Node *> terms;
    vector<Node *> resChildren;

    collectTerms(node, terms, value);

    if (node->value == "+")
    {
        auto [mapping, symbolicMapping, number] = createMappings(terms, nextValue[value]);
        resChildren = constructChildren(mapping, symbolicMapping, number, nextValue[value]);
    }

    if (node->value == "*")
    {
        auto [mapping, symbolicMapping, number] = createProductMappings(terms, nextValue[value]);
        resChildren = constructPowerChildren(mapping, symbolicMapping, number);
        resChildren = combineProductPowers(resChildren);
    }

    if (node->value == "^")
    {
        if (node->children.size() != 2)
            return node;

        auto [base, exponent] = createPowerMapping(node);

        return constructPower(base, exponent);
    }

    if (resChildren.size() == 1)
        node = resChildren[0];
    else
        node->children = resChildren;

    return node;
}

vector<Node *> combineProductPowers(vector<Node *> children)
{
    map<string, Node *> bases;
    map<string, vector<Node *>> exponents;
    vector<string> baseOrder;
    vector<Node *> resChildren;

    for (Node *child : children)
    {
        if (child->children.empty() && isNumber(child->value))
        {
            resChildren.push_back(child);
            continue;
        }

        Node *base = child;
        Node *exponent = new Node("1", {});

        if (child->value == "^" && child->children.size() == 2)
        {
            base = child->children[0];
            exponent = child->children[1];
        }

        string key = displayExpression(base, 0);

        if (!bases.contains(key))
        {
            bases[key] = base;
            baseOrder.push_back(key);
        }

        exponents[key].push_back(exponent);
    }

    for (const string &key : baseOrder)
    {
        Node *base = bases[key];
        vector<Node *> exponentTerms = exponents[key];

        if (exponentTerms.size() == 1)
        {
            Node *exponent = exponentTerms[0];

            if (exponent->children.empty() && exponent->value == "1")
                resChildren.push_back(base);
            else
                resChildren.push_back(new Node("^", {base, exponent}));

            continue;
        }

        Node *exponent = new Node("+", exponentTerms);
        resChildren.push_back(new Node("^", {base, exponent}));
    }

    return resChildren;
}

void collectTerms(Node *node, vector<Node *> &terms, string value)
{
    if (node->value == value)
    {
        for (Node *child : node->children)
            collectTerms(child, terms, value);
    }
    else
        terms.push_back(node);
}

tuple<map<string, float>, map<string, Node *>, float> createMappings(vector<Node *> terms, string value)
{
    float number = 0;

    map<string, float> mapping;
    map<string, Node *> symbolicMapping;

    for (Node *child : terms)
    {
        if (isNumber(child->value))
            number = operation(number, stof(child->value), previousValue[value]);
        else if (child->value != value)
        {
            string key = displayExpression(child, 0);

            mapping[key]++;
            symbolicMapping[key] = child;
        }
        else
        {
            float coeff = 1;
            size_t firstNonNumericIndex = 0;

            if (!child->children.empty() && child->children[0]->children.empty() && isNumber(child->children[0]->value))
            {
                coeff = stof(child->children[0]->value);
                firstNonNumericIndex = 1;
            }

            vector<Node *> nonNumericFactors((child->children).begin() + firstNonNumericIndex, (child->children).end());

            Node *symbolicPart;

            if (nonNumericFactors.size() == 1)
                symbolicPart = nonNumericFactors[0];
            else
                symbolicPart = new Node(value, nonNumericFactors);

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
    int denominator = 1;

    map<string, float> mapping;
    map<string, Node *> symbolicMapping;

    for (Node *child : terms)
    {
        if (isNumber(child->value))
        {
            number *= stof(child->value);
        }
        else if (child->value == "^")
        {
            Node *base = child->children[0];
            Node *exponent = child->children[1];

            if (base->children.empty() && exponent->children.empty() && isNumber(base->value) && isNumber(exponent->value) && stof(exponent->value) == -1 && stof(base->value) == stoi(base->value))
                denominator *= stoi(base->value);
            else if (base->children.empty() && exponent->children.empty() && isNumber(base->value) && isNumber(exponent->value))
                number *= pow(stof(base->value), stof(exponent->value));
            else if (!exponent->children.empty() || !isNumber(exponent->value))
            {
                string key = displayExpression(child, 0);
                mapping[key] += 1;
                symbolicMapping[key] = child;
            }
            else
            {
                string key = displayExpression(base, 0);
                mapping[key] += stof(exponent->value);
                symbolicMapping[key] = base;
            }
        }
        else
        {
            string key = displayExpression(child, 0);
            mapping[key] += 1;
            symbolicMapping[key] = child;
        }
    }

    if (denominator != 1 && number == (int)number)
    {
        int numerator = (int)number;
        int divisor = gcd(abs(numerator), abs(denominator));

        number = numerator / divisor;
        denominator /= divisor;

        if (denominator != 1)
        {
            string key = numberToString(denominator);
            mapping[key] -= 1;
            symbolicMapping[key] = new Node(key, {});
        }
    }

    return {mapping, symbolicMapping, number};
}

tuple<Node *, Node *> createPowerMapping(Node *node)
{
    Node *base = node->children[0];
    Node *exponent = node->children[1];

    float power = 1;

    if (exponent->children.empty() && isNumber(exponent->value))
    {
        power *= stof(exponent->value);
    }
    else
    {
        return {base, exponent};
    }

    while (base->value == "^" && base->children.size() == 2 && base->children[1]->children.empty() && isNumber(base->children[1]->value))
    {
        power *= stof(base->children[1]->value);
        base = base->children[0];
    }

    return {base, new Node(numberToString(power), {})};
}

vector<Node *> constructChildren(map<string, float> mapping, map<string, Node *> symbolicMapping, float number, string value)
{
    vector<Node *> resChildren;

    for (const auto &pair : mapping)
    {
        if (symbolicMapping.contains(pair.first))
        {
            Node *symbolicPart = symbolicMapping[pair.first];

            if (pair.second == stof(neutralElement[value]))
                resChildren.push_back(symbolicPart);
            else
                resChildren.push_back(new Node(value, {new Node(numberToString(pair.second), {}), symbolicPart}));
        }
        else
        {
            if (pair.second == 1)
                resChildren.push_back(new Node(pair.first, {}));
            else
                resChildren.push_back(new Node(value, {new Node(numberToString(pair.second), {}), new Node(pair.first, {})}));
        }
    }

    if (number != stof(neutralElement[previousValue[value]]) || resChildren.empty())
        resChildren.push_back(new Node(numberToString(number), {}));

    return resChildren;
}

vector<Node *> constructPowerChildren(map<string, float> mapping, map<string, Node *> symbolicMapping, float number)
{
    vector<Node *> resChildren;

    resChildren.push_back(new Node(numberToString(number), {}));

    for (const auto &pair : mapping)
    {
        if (symbolicMapping.contains(pair.first))
        {
            Node *symbolicPart = symbolicMapping[pair.first];

            if (pair.second == 1)
                resChildren.push_back(symbolicPart);
            else
                resChildren.push_back(new Node("^", {symbolicPart, new Node(numberToString(pair.second), {})}));
        }
        else
        {
            if (pair.second == 1)
                resChildren.push_back(new Node(pair.first, {}));
            else
                resChildren.push_back(new Node("^", {new Node(pair.first, {}), new Node(numberToString(pair.second), {})}));
        }
    }

    return resChildren;
}

Node *constructPower(Node *base, Node *exponent)
{
    if (exponent->value == "0")
        return new Node("1", {});

    if (exponent->value == "1")
        return base;

    if (base->value == "*")
    {
        vector<Node *> resChildren;

        for (Node *child : base->children)
        {
            if (child->children.empty() && child->value == neutralElement[base->value])
                continue;

            if (child->value == "^" && child->children.size() == 2 && child->children[1]->children.empty() && isNumber(child->children[1]->value))
            {
                float childExponent = stof(child->children[1]->value);
                resChildren.push_back(constructPower(child->children[0], new Node("*", {new Node(numberToString(childExponent), {}), exponent})));
            }
            else
                resChildren.push_back(constructPower(child, exponent));
        }

        if (resChildren.empty())
            return new Node("1", {});

        if (resChildren.size() == 1)
            return resChildren[0];

        return new Node("*", resChildren);
    }

    return new Node("^", {base, exponent});
}

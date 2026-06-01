#include "domain/tree.hpp"
#include "algorithms/simplify.hpp"
#include "algorithms/utils.hpp"
#include "algorithms/display.hpp"
#include "types/tokenTypes.hpp"
#include <vector>
#include <map>
#include <cmath>
#include <numeric>

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
        if (node->children[0]->value == "pi")
            return new Node("0", {});
        // if (node->children[0]->value == "*" && node->children[0]->children[0]->value == "-1")
        //     return new Node("*", {new Node("-1", {}), node->children[0]->children[1]});
    }

    if (node->value == "cos")
    {
        if (node->children[0]->value == "0")
            return new Node("1", {});
        if (node->children[0]->value == "pi")
            return new Node("-1", {});
    }

    if (node->value == "sum")
    {
        if (isNumber(node->children[3]->value))
        {
            return new Node("*", {node->children[3], new Node("+", {node->children[2], new Node("*", {new Node("-1", {}), node->children[1]}), new Node("1", {})})});
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

Node *rewriteSumRules(Node *node)
{
    vector<Node *> terms = node->children;

    SumIndex index = buildSumIndex(terms);

    if (Node *res = rewriteTrigIdentity(node, index))
        return res;

    if (Node *res = rewriteAdjacentSums(node, index))
        return res;

    return node;
}

Node *rewriteTrigIdentity(Node *node, SumIndex index)
{
    for (const auto &pair : index.sinSquaredByArg)
    {
        string argKey = pair.first;
        size_t sinIndex = pair.second;

        if (!index.cosSquaredByArg.contains(argKey))
            continue;

        size_t cosIndex = index.cosSquaredByArg[argKey];

        vector<Node *> newChildren;

        for (size_t i = 0; i < node->children.size(); i++)
        {
            if (i == sinIndex || i == cosIndex)
                continue;

            newChildren.push_back(node->children[i]);
        }

        newChildren.push_back(new Node("1", {}));

        if (newChildren.size() == 1)
            return newChildren[0];

        return new Node("+", newChildren);
    }

    return nullptr;
}

bool sameExpression(Node *left, Node *right)
{
    return displayExpression(left, 0) == displayExpression(right, 0);
}

bool isSuccessor(Node *candidate, Node *base)
{
    if (candidate->value == "+" && candidate->children.size() == 2)
    {
        bool firstIsOne = candidate->children[0]->children.empty() && candidate->children[0]->value == "1";
        bool secondIsOne = candidate->children[1]->children.empty() && candidate->children[1]->value == "1";

        if (firstIsOne && sameExpression(candidate->children[1], base))
            return true;

        if (secondIsOne && sameExpression(candidate->children[0], base))
            return true;
    }

    if (candidate->children.empty() && base->children.empty() && isNumber(candidate->value) && isNumber(base->value))
        return stof(candidate->value) == stof(base->value) + 1;

    return false;
}

Node *rewriteAdjacentSums(Node *node, SumIndex index)
{
    for (const auto &pair : index.sumsByVariableAndBody)
    {
        vector<int> sumIndexes = pair.second;

        for (size_t i = 0; i < sumIndexes.size(); i++)
        {
            for (size_t j = i + 1; j < sumIndexes.size(); j++)
            {
                size_t firstIndex = sumIndexes[i];
                size_t secondIndex = sumIndexes[j];

                Node *first = node->children[firstIndex];
                Node *second = node->children[secondIndex];

                if (first->children.size() != 4 || second->children.size() != 4)
                    continue;

                Node *firstStart = first->children[1];
                Node *firstEnd = first->children[2];
                Node *secondStart = second->children[1];
                Node *secondEnd = second->children[2];

                Node *mergedStart = nullptr;
                Node *mergedEnd = nullptr;
                Node *mergedVariable = first->children[0];
                Node *mergedBody = first->children[3];

                if (isSuccessor(secondStart, firstEnd))
                {
                    mergedStart = firstStart;
                    mergedEnd = secondEnd;
                }
                else if (isSuccessor(firstStart, secondEnd))
                {
                    mergedStart = secondStart;
                    mergedEnd = firstEnd;
                }
                else
                    continue;

                Node *merged = new Node("sum", {mergedVariable, mergedStart, mergedEnd, mergedBody});
                vector<Node *> newChildren;

                for (size_t k = 0; k < node->children.size(); k++)
                {
                    if (k == firstIndex || k == secondIndex)
                        continue;

                    newChildren.push_back(node->children[k]);
                }

                newChildren.push_back(merged);

                if (newChildren.size() == 1)
                    return newChildren[0];

                return new Node("+", newChildren);
            }
        }
    }

    return nullptr;
}

SumIndex buildSumIndex(vector<Node *> terms)
{
    SumIndex index;

    for (size_t i = 0; i < terms.size(); i++)
    {
        Node *term = terms[i];

        if (term->value == "^")
        {
            Node *base = term->children[0];
            Node *exponent = term->children[1];

            bool isSquare = exponent->value == "2";

            if (isSquare && base->value == "sin")
            {
                string argKey = displayExpression(base->children[0], 0);
                index.sinSquaredByArg[argKey] = i;
            }

            if (isSquare && base->value == "cos")
            {
                string argKey = displayExpression(base->children[0], 0);
                index.cosSquaredByArg[argKey] = i;
            }
        }

        if (term->value == "sum")
        {
            Node *variable = term->children[0];
            Node *body = term->children[3];

            string key = "#|" + displayExpression(canonicalExpression(body, variable->value), 0);

            index.sumsByVariableAndBody[key].push_back(i);
        }
    }

    return index;
}

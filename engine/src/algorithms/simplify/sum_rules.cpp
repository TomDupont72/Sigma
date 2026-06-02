#include "algorithms/simplify/internal.hpp"
#include "algorithms/display.hpp"
#include "algorithms/utils.hpp"
#include "domain/tree.hpp"
#include <string>
#include <vector>

using namespace std;

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

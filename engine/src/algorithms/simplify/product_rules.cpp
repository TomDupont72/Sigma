#include "algorithms/simplify/internal.hpp"
#include "algorithms/display.hpp"
#include "algorithms/utils.hpp"
#include "domain/tree.hpp"
#include <string>
#include <vector>

using namespace std;

bool isSuccessor(Node *candidate, Node *base)
{
    if (candidate->value == "+" && candidate->children.size() == 2)
    {
        bool firstIsOne = candidate->children[0]->children.empty() && candidate->children[0]->value == "1";
        bool secondIsOne = candidate->children[1]->children.empty() && candidate->children[1]->value == "1";

        if (firstIsOne && displayExpression(candidate->children[1], 0) == displayExpression(base, 0))
            return true;

        if (secondIsOne && displayExpression(candidate->children[0], 0) == displayExpression(base, 0))
            return true;
    }

    if (candidate->children.empty() && base->children.empty() && isNumber(candidate->value) && isNumber(base->value))
        return stof(candidate->value) == stof(base->value) + 1;

    return false;
}

Node *rewriteAdjacentProducts(Node *node, ProductIndex index)
{
    for (const auto &pair : index.productsByVariableAndBody)
    {
        vector<int> productIndexes = pair.second;

        for (size_t i = 0; i < productIndexes.size(); i++)
        {
            for (size_t j = i + 1; j < productIndexes.size(); j++)
            {
                size_t firstIndex = productIndexes[i];
                size_t secondIndex = productIndexes[j];

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

                Node *merged = new Node("product", {mergedVariable, mergedStart, mergedEnd, mergedBody});
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

ProductIndex buildProductIndex(vector<Node *> terms)
{
    ProductIndex index;

    for (size_t i = 0; i < terms.size(); i++)
    {
        Node *term = terms[i];

        if (term->value == "prod")
        {
            Node *variable = term->children[0];
            Node *body = term->children[3];

            string key = "#|" + displayExpression(canonicalExpression(body, variable->value), 0);

            index.productsByVariableAndBody[key].push_back(i);
        }
    }

    return index;
}

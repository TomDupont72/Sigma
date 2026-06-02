#include "algorithms/utils.hpp"
#include "domain/tree.hpp"
#include <sstream>
#include <vector>

using namespace std;

string join(const vector<string> &values, const string &separator)
{
    stringstream ss;

    for (size_t i = 0; i < values.size(); i++)
    {
        ss << values[i];

        if (i < values.size() - 1)
            ss << separator;
    }

    return ss.str();
}

string joinSum(const vector<string> &parts)
{
    string res = "";

    for (size_t i = 0; i < parts.size(); i++)
    {
        if (i == 0)
        {
            res += parts[i];
        }
        else if (!parts[i].empty() && parts[i][0] == '-')
        {
            res += parts[i];
        }
        else
        {
            res += "+" + parts[i];
        }
    }

    return res;
}

bool isNumber(const string &s)
{
    bool dotSeen = false;
    bool firstChar = true;

    if (s.empty())
        return false;

    for (char c : s)
    {
        if (c == '.' || (c == '-' && firstChar))
        {
            firstChar = false;
            if (dotSeen)
                return false;

            dotSeen = true;
        }
        else if (!isdigit(c))
            return false;
    }

    return true;
}

string numberToString(float value)
{
    ostringstream oss;
    oss << value;
    return oss.str();
}

void printTree(Node *node, string prefix, bool isLast)
{
    cout << prefix;

    cout << (isLast ? "\\-- " : "|-- ");

    cout << node->value << endl;

    for (size_t i = 0; i < node->children.size(); i++)
    {
        printTree(
            node->children[i],
            prefix + (isLast ? "    " : "|   "),
            i == node->children.size() - 1);
    }

    cout << "\n";
}

float operation(float a, float b, string value)
{
    if (value == "+")
        return a + b;

    if (value == "*")
        return a * b;

    return 0;
}

vector<Node *> copyWithoutIndex(const vector<Node *> &children, int indexToRemove)
{
    vector<Node *> result;

    for (int i = 0; i < children.size(); i++)
    {
        if (i != indexToRemove)
        {
            result.push_back(children[i]);
        }
    }

    return result;
}

int binom(int n, int k)
{
    if (k < 0 || k > n)
        return 0;

    if (k == 0 || k == n)
        return 1;

    if (k > n - k)
        k = n - k;

    int result = 1;

    for (int i = 1; i <= k; i++)
    {
        result = result * (n - k + i) / i;
    }

    return result;
}

Node *canonicalExpression(Node *node, string boundVariable)
{
    if (node->children.empty())
    {
        if (node->value == boundVariable)
            return new Node("#", {});

        return new Node(node->value, {});
    }

    vector<Node *> canonicalChildren;

    for (Node *child : node->children)
        canonicalChildren.push_back(canonicalExpression(child, boundVariable));

    return new Node(node->value, canonicalChildren);
}

bool sameExpression(Node *left, Node *right)
{
    if (left->value != right->value || left->children.size() != right->children.size())
        return false;

    for (size_t i = 0; i < left->children.size(); i++)
    {
        if (!sameExpression(left->children[i], right->children[i]))
            return false;
    }

    return true;
}

bool isInverseOf(Node *node, Node *base)
{
    return node->value == "^" &&
           node->children.size() == 2 &&
           sameExpression(node->children[0], base) &&
           node->children[1]->children.empty() &&
           node->children[1]->value == "-1";
}

bool isExactDivision(Node *node, Node *numerator, Node *denominator)
{
    bool numeratorIsOne = numerator->children.empty() && numerator->value == "1";

    if (isInverseOf(node, denominator))
        return numeratorIsOne;

    if (node->value != "*")
        return false;

    vector<Node *> numeratorFactors;
    bool foundDenominator = false;

    for (Node *child : node->children)
    {
        if (child->children.empty() && child->value == "1")
            continue;

        if (isInverseOf(child, denominator))
        {
            if (foundDenominator)
                return false;

            foundDenominator = true;
            continue;
        }

        numeratorFactors.push_back(child);
    }

    if (!foundDenominator)
        return false;

    Node *actualNumerator;

    if (numeratorFactors.empty())
        actualNumerator = new Node("1", {});
    else if (numeratorFactors.size() == 1)
        actualNumerator = numeratorFactors[0];
    else
        actualNumerator = new Node("*", numeratorFactors);

    return sameExpression(actualNumerator, numerator);
}

Node *negative(Node *node)
{
    return new Node("*", {new Node("-1", {}), node});
}

Node *inverse(Node *node)
{
    return new Node("^", {node, new Node("-1", {})});
}

Node *power(Node *nodeBase, Node *nodeExponent)
{
    return new Node("^", {nodeBase, nodeExponent});
}

Node *addConstant(Node *node, string constant)
{
    return new Node("+", {node, new Node(constant, {})});
}

#include "algorithms/utils.hpp"
#include "domain/tree.hpp"
#include <sstream>
#include<vector>

using namespace std;

string join(const vector<string>& values, const string& separator)
{
    stringstream ss;

    for (size_t i = 0; i < values.size(); i++)
    {
        ss << values[i];

        if (i < values.size() - 1) ss << separator;
    }

    return ss.str();
}

bool isNumber(const string& s)
{
    bool dotSeen = false;

    if (s.empty()) return false;

    for (char c : s)
    {
        if (c == '.')
        {
            if (dotSeen)
                return false;

            dotSeen = true;
        }
        else if (!isdigit(c)) return false;
    }

    return true;
}

string numberToString(float value)
{
    ostringstream oss;
    oss << value;
    return oss.str();
}

void printTree(Node* node, string prefix, bool isLast)
{
    cout << prefix;

    cout << (isLast ? "\\-- " : "|-- ");

    cout << node->value << endl;

    for (size_t i = 0; i < node->children.size(); i++)
    {
        printTree(
            node->children[i],
            prefix + (isLast ? "    " : "|   "),
            i == node->children.size() - 1
        );
    }

    cout << "\n";
}

float operation(float a, float b, string value)
{
    if (value == "+") return a + b;

    if (value == "*") return a * b;

    return 0;
}

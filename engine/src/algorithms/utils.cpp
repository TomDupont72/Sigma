#include "algorithms/utils.hpp"
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

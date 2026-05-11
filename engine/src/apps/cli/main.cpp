#include <iostream>
#include <vector>
#include "domain/tree.hpp"
#include "domain/function.hpp"
#include "algorithms/lexer.hpp"

using namespace std;

int main()
{
    string input = "a + b + ln(6 + 7)";

    vector<Token> res = lexer(input);

    for (int i = 0; i < res.size(); i++)
    {
        cout << res[i].value;
        cout << " ";
    }

    return 0;
};
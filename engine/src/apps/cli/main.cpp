#include <iostream>
#include <vector>
#include "domain/tree.hpp"
#include "domain/function.hpp"
#include "domain/parser.hpp"
#include "algorithms/lexer.hpp"

using namespace std;

int main()
{
    string input = "a + b + ln(6 + 7)";

    vector<Token> res = lexer(input);

    Parser parser(res);

    Node * tree = parser.parseExpression();

    cout << (*(*tree).children[0]).value;

    return 0;
};

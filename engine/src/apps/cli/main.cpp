#include <iostream>
#include <vector>
#include "domain/tree.hpp"
#include "domain/function.hpp"
#include "domain/parser.hpp"
#include "algorithms/lexer.hpp"
#include "algorithms/display.hpp"
#include "algorithms/simplify.hpp"
#include "algorithms/utils.hpp"

using namespace std;

int main()
{
    string input = "((x^3/x^2) + (2*x*y + 3*y*x) + 1/(x*x*x*x)^2 + ln(1) + 0*(a+b) + ((2+3)-1) + (4*x-2*x) + (1/(x^2)^3) + (x-x))";

    vector<Token> res = lexer(input);

    Parser parser(res);

    Node * tree = parser.parseExpression();

    printTree(tree);

    cout << displayExpression(tree, 0) + '\n';

    tree = normalize(tree);

    printTree(tree);

    cout << displayExpression(tree, 0);

    return 0;
};

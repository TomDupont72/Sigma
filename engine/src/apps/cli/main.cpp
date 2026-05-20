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
    string input = "2*x*y + 3*y*x + x*x*x + x^2*x^3 + ln(1) + 0*(a+b) + 5 - 2 + y - y + 4*x - x";

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

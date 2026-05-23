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
    string input = "((x*x)^2*(x^3)/(x*x*x))^2/(x*(x^2)^5)+(2*x+3*x-x)+(y*y)^3/(y^2*y)-(4*a-2*a+a)+(z^2)^0+(b^1)^2";

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

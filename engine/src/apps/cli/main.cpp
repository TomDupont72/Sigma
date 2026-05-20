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
    string input = "ln(1) + 0*(x + y) + 2*x - x + 3*y*x + 4*x*y + (5 + 0)";

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

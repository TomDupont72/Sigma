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
    string input = "(a+2*b)+c +a";

    vector<Token> res = lexer(input);

    Parser parser(res);

    Node * tree = parser.parseExpression();

    printTree(tree);

    cout << displayExpression(tree, 0) + '\n';

    tree = simplify(tree);

    printTree(tree);

    cout << displayExpression(tree, 0);

    return 0;
};

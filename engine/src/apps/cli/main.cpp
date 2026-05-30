#include <iostream>
#include <vector>
#include "domain/tree.hpp"
#include "domain/function.hpp"
#include "domain/parser.hpp"
#include "algorithms/lexer.hpp"
#include "algorithms/display.hpp"
#include "algorithms/simplify.hpp"
#include "algorithms/utils.hpp"
#include "algorithms/derive.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    string command = argv[1];
    string expression = argv[2];
    string variable = argv[3];

    vector<Token> res = lexer(expression);
    Parser parser(res);
    Node *tree = parser.parseExpression();

    if (command == "display")
    {
        cout << displayExpression(tree, 0);
    }

    if (command == "simplify")
    {
        tree = normalize(tree);
        cout << displayExpression(tree, 0);
    }

    if (command == "derive")
    {
        tree = derive(tree, variable);
        printTree(tree);
        tree = normalize(tree);
        printTree(tree);
        cout << displayExpression(tree, 0);
    }

    return 0;
}

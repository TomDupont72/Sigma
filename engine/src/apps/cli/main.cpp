#include <iostream>
#include <vector>
#include "domain/tree.hpp"
#include "domain/function.hpp"
#include "domain/parser.hpp"
#include "algorithms/lexer.hpp"
#include "algorithms/display.hpp"

using namespace std;

int main()
{
    string input = "a + b + ln((65 + 17) * 3)";

    vector<Token> res = lexer(input);

    for (const Token& token : res)
    {
        cout << token.value << " : " << static_cast<int>(token.type) << endl;
    }

    Parser parser(res);

    Node * tree = parser.parseExpression();

    cout << "Token restant : "
     << parser.currentToken().value
     << " type "
     << static_cast<int>(parser.currentToken().type)
     << endl;

    if (parser.currentToken().type != TokenType::End)
    {
        throw runtime_error("Expression invalide : tokens restants.");
    }

    cout << displayExpression(tree);

    return 0;
};

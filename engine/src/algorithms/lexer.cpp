#include "domain/token.hpp"
#include "types/tokenTypes.hpp"
#include "algorithms/lexer.hpp"
#include <vector>
#include <string>
#include <cctype>

using namespace std;

vector<Token> lexer(string expression)
{
    vector<Token> res = {};
    int expressionLength = expression.size();

    for (int i = 0; i < expressionLength; i++)
    {
        char currentChar = expression[i];

        if (isdigit(currentChar))
        {
            parseDigit(expression, res, i, expressionLength);
        }

        if (isalpha(currentChar))
        {
            parseAlpha(expression, res, i, expressionLength);
        }

        if (operatorMapping.find(currentChar) != operatorMapping.end())
        {
            Token currentToken(operatorMapping[currentChar], string(1, currentChar));
            res.push_back(currentToken);
        }

        if (parenMapping.find(currentChar) != parenMapping.end())
        {
            Token currentToken(parenMapping[currentChar], string(1, currentChar));
            res.push_back(currentToken);
        }
    }

    Token currentToken(TokenType::End, "");
    res.push_back(currentToken);

    return res;
}

void parseDigit(string &expression, vector<Token> &res, int &index, int expressionLength)
{
    Token currentToken(TokenType::Number, string(1, expression[index]));

    while (++index < expressionLength && isdigit(expression[index]))
    {
        currentToken.value += expression[index];
    }

    res.push_back(currentToken);
    index--;
}

void parseAlpha(string &expression, vector<Token> &res, int &index, int expressionLength)
{
    Token currentToken(TokenType::Number, string(1, expression[index]));

    while (++index < expressionLength && isalpha(expression[index]))
    {
        currentToken.value += expression[index];
    }

    res.push_back(currentToken);
    index--;
};

#include "domain/token.hpp"
#include "domain/parser.hpp"
#include "domain/tree.hpp"
#include "types/tokenTypes.hpp"
#include <vector>
#include <stdexcept>

using namespace std;

Parser::Parser(const vector<Token>& tokens)
{
    this->tokens = tokens;
    this->pos = 0;
}

Node * Parser::parseExpression()
{
    vector<Node *> terms;

    terms.push_back(parseTerm());

    while (currentToken().type == TokenType::Plus || currentToken().type == TokenType::Minus)
    {
        TokenType operation = currentToken().type;
        advance();

        Node * right = parseTerm();

        if (operation == TokenType::Plus) terms.push_back(right);
        else terms.push_back(new Node("*", { new Node("-1", {}), right }));

    }

    if (terms.size() == 1) return terms[0];

    return new Node("+", terms);
}

Node * Parser::parseTerm()
{
    vector<Node *> terms;

    terms.push_back(parsePower());

    while (currentToken().type == TokenType::Multiply || currentToken().type == TokenType::Divide)
    {
        TokenType operation = currentToken().type;
        advance();

        Node * right = parsePower();

        if (operation == TokenType::Multiply) terms.push_back(right);
        else terms.push_back(new Node("^", { right, new Node("-1", {}) }));

    }

    if (terms.size() == 1) return terms[0];

    return new Node("*", terms);
}

Node * Parser::parsePower()
{
    Node* left = parseFactor();

    while (currentToken().type == TokenType::Power)
    {
        advance();

        Node* right = parseFactor();

        return new Node("^", { left, right });
    }

    return left;
}

Node * Parser::parseFactor()
{
    if (currentToken().type == TokenType::Number)
    {
        string value = currentToken().value;
        advance();

        return new Node(value, {});
    }

    if (currentToken().type == TokenType::Identifier)
    {
        string name = currentToken().value;
        advance();

        if (currentToken().type == TokenType::LeftParen)
        {
            advance();

            Node * argument = parseExpression();

            if (currentToken().type != TokenType::RightParen) throw runtime_error("Parenthèse fermante manquante.");

            advance();

            return new Node(name, { argument });
        }

        return new Node(name, {});
    }

    if (currentToken().type == TokenType::LeftParen)
    {
        advance();

        Node * expression = parseExpression();

        if (currentToken().type != TokenType::RightParen) throw runtime_error("Parenthèse fermante manquante.");

        advance();

        return expression;
    }

    throw runtime_error("Token invalide.");
}

Token Parser::currentToken()
{
    if (pos >= tokens.size()) return Token(TokenType::End, "");

    return tokens[pos];
}

void Parser::advance()
{
    if (pos < tokens.size()) pos++;
}

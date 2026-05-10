#include "types/tokenTypes.hpp"
#include "domain/token.hpp"
#include <string>

using namespace std;

Token::Token(TokenType type, string value)
{
    this->type = type;
    this->value = value;
}
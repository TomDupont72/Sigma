#pragma once

#include "types/tokenTypes.hpp"
#include <string>

using namespace std;

class Token
{
public:
    TokenType type;
    string value;

    Token(TokenType type, string value);
};
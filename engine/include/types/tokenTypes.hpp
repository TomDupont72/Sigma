#pragma once

#include <map>

using namespace std;

enum class TokenType
{
    Number,
    Identifier,

    Plus,
    Minus,
    Multiply,
    Divide,
    Power,

    LeftParen,
    RightParen,

    End,
    Invalid
};

inline map<char, TokenType> operatorMapping =
    {
        {'+', TokenType::Plus},
        {'-', TokenType::Minus},
        {'*', TokenType::Multiply},
        {'/', TokenType::Divide},
        {'^', TokenType::Power}};

inline map<char, TokenType> parenMapping =
    {
        {'(', TokenType::LeftParen},
        {')', TokenType::RightParen}};
#pragma once

#include <map>

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

map<char, TokenType> operatorMapping =
    {
        {'+', TokenType::Plus},
        {'-', TokenType::Minus},
        {'*', TokenType::Multiply},
        {'/', TokenType::Divide},
        {'^', TokenType::Power}};

map<char, TokenType> parenMapping =
    {
        {'(', TokenType::LeftParen},
        {')', TokenType::RightParen}};
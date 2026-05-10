#pragma once

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
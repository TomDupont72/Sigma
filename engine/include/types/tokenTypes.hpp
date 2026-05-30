#pragma once

#include <map>
#include <set>
#include <string>

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

inline map<string, string> nextValue =
    {
        {"+", "*"},
        {"*", "^"}};

inline map<string, string> previousValue =
    {
        {"*", "+"},
        {"^", "*"}};

inline map<string, string> neutralElement =
    {
        {"+", "0"},
        {"*", "1"},
        {"^", "1"}};

inline set<string> operations = {"+", "*", "^"};

inline set<string> identifiers = {"ln"};

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType
{
    Identifier,
    IntLiteral,
    FloatLiteral,
    BoolLiteral,
    StringLiteral,
    // ключевые слова
    Var,
    Func,
    If,
    Then,
    Else,
    End,
    While,
    Do,
    For,
    Return,
    Read,
    Print,
    True,
    False,
    Int,
    Float,
    Bool,
    String,
    New,
    // операторы и разделители
    Semicolon,
    Comma,
    Colon,
    LParen,
    RParen,
    LBracket,
    RBracket,
    LBrace,
    RBrace,
    Assign,
    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    EQ,
    NEQ,
    LT,
    LTE,
    GT,
    GTE,
    And,
    Or,
    Not,
    EndOfFile,
    Unknown
};

struct Token
{
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};

#endif
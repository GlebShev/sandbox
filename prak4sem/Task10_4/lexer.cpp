#include "lexer.h"
#include <cctype>
#include <stdexcept>

using namespace std;

// словарь ключевых слов
const pair<string, TokenType> Lexer::keywords[] = {
    {"var", TokenType::Var}, {"func", TokenType::Func}, {"if", TokenType::If}, {"then", TokenType::Then}, {"else", TokenType::Else}, {"end", TokenType::End}, {"while", TokenType::While}, {"do", TokenType::Do}, {"for", TokenType::For}, {"return", TokenType::Return}, {"read", TokenType::Read}, {"print", TokenType::Print}, {"true", TokenType::True}, {"false", TokenType::False}, {"int", TokenType::Int}, {"float", TokenType::Float}, {"bool", TokenType::Bool}, {"string", TokenType::String}, {"new", TokenType::New}};

const int Lexer::keywordsCount = sizeof(Lexer::keywords) / sizeof(Lexer::keywords[0]);

Lexer::Lexer(const string &src) : source(src), pos(0), line(1), column(1) {}

vector<Token> Lexer::tokenize()
{
    vector<Token> tokens;
    while (!isAtEnd())
    {
        skipWhitespaceAndComments();
        if (isAtEnd())
            break;

        int tokLine = line, tokCol = column;
        char c = advance();

        if (isalpha(c) || c == '_')
        {
            pos--; // уже прочитали символ, что identifierOrKeyword корректно отработал отходим назад
            column--;
            tokens.push_back(identifierOrKeyword());
            continue;
        }
        if (isdigit(c))
        {
            pos--;
            column--;
            tokens.push_back(numberLiteral());
            continue;
        }
        if (c == '"')
        {
            tokens.push_back(stringLiteral());
            continue;
        }

        // двухсимвольные операторы
        switch (c)
        {
        case '=':
            tokens.push_back(match('=') ? makeToken(TokenType::EQ, "==") : makeToken(TokenType::Assign, "="));
            break;
        case '!':
            tokens.push_back(match('=') ? makeToken(TokenType::NEQ, "!=") : makeToken(TokenType::Not, "!"));
            break;
        case '<':
            tokens.push_back(match('=') ? makeToken(TokenType::LTE, "<=") : makeToken(TokenType::LT, "<"));
            break;
        case '>':
            tokens.push_back(match('=') ? makeToken(TokenType::GTE, ">=") : makeToken(TokenType::GT, ">"));
            break;
        case '&':
            tokens.push_back(match('&') ? makeToken(TokenType::And, "&&") : makeToken(TokenType::Unknown, "&"));
            break;
        case '|':
            tokens.push_back(match('|') ? makeToken(TokenType::Or, "||") : makeToken(TokenType::Unknown, "|"));
            break;
        default:
            break;
        }

        switch (c)
        {
        case ';':
            tokens.push_back(makeToken(TokenType::Semicolon, ";"));
            break;
        case ',':
            tokens.push_back(makeToken(TokenType::Comma, ","));
            break;
        case ':':
            tokens.push_back(makeToken(TokenType::Colon, ":"));
            break;
        case '(':
            tokens.push_back(makeToken(TokenType::LParen, "("));
            break;
        case ')':
            tokens.push_back(makeToken(TokenType::RParen, ")"));
            break;
        case '{':
            tokens.push_back(makeToken(TokenType::LBrace, "{"));
            break;
        case '}':
            tokens.push_back(makeToken(TokenType::RBrace, "}"));
            break;
        case '[':
            tokens.push_back(makeToken(TokenType::LBracket, "["));
            break;
        case ']':
            tokens.push_back(makeToken(TokenType::RBracket, "]"));
            break;
        case '+':
            tokens.push_back(makeToken(TokenType::Plus, "+"));
            break;
        case '-':
            tokens.push_back(makeToken(TokenType::Minus, "-"));
            break;
        case '*':
            tokens.push_back(makeToken(TokenType::Star, "*"));
            break;
        case '/':
            tokens.push_back(makeToken(TokenType::Slash, "/"));
            break;
        case '%':
            tokens.push_back(makeToken(TokenType::Percent, "%"));
            break;
        default:
            tokens.push_back(makeToken(TokenType::Unknown, string(1, c)));
            break;
        }
    }
    tokens.push_back(makeToken(TokenType::EndOfFile, ""));
    return tokens;
}

char Lexer::peek() const
{
    return isAtEnd() ? '\0' : source[pos];
}

char Lexer::peekNext() const
{
    return (pos + 1 >= source.size()) ? '\0' : source[pos + 1];
}

char Lexer::advance()
{
    char c = source[pos++];
    if (c == '\n')
    {
        line++;
        column = 1;
    }
    else
    {
        column++;
    }
    return c;
}

bool Lexer::isAtEnd() const
{
    return pos >= source.size();
}

bool Lexer::match(char expected)
{
    if (isAtEnd() || source[pos] != expected)
        return false;
    pos++;
    column++;
    return true;
}

void Lexer::skipWhitespaceAndComments()
{
    while (true)
    {
        char c = peek();
        if (isspace(c))
        {
            advance();
            continue;
        }
        if (c == '/' && peekNext() == '/')
        {
            advance();
            advance();
            while (peek() != '\n' && !isAtEnd())
                advance();
            continue;
        }
        if (c == '/' && peekNext() == '*')
        {
            advance();
            advance();
            while (!(peek() == '*' && peekNext() == '/') && !isAtEnd())
                advance();
            if (!isAtEnd())
            {
                advance();
                advance();
            }
            continue;
        }
        break;
    }
}

Token Lexer::makeToken(TokenType type, const string &lexeme)
{
    return Token{type, lexeme, line, column};
}

Token Lexer::identifierOrKeyword()
{
    int start = pos;
    int tokLine = line;
    int tokCol = column;
    while (isalnum(peek()) || peek() == '_')
        advance();
    string text = source.substr(start, pos - start);
    for (int i = 0; i < keywordsCount; i++)
    {
        if (keywords[i].first == text)
        {
            return Token{keywords[i].second, text, tokLine, tokCol};
        }
    }
    return Token{TokenType::Identifier, text, tokLine, tokCol};
}

Token Lexer::numberLiteral()
{
    int start = pos;
    int tokLine = line;
    int tokCol = column;
    bool isFloat = false;
    while (isdigit(peek()))
        advance();
    if (peek() == '.' && isdigit(peekNext()))
    {
        isFloat = true;
        advance();
        while (isdigit(peek()))
            advance();
    }
    string text = source.substr(start, pos - start);
    return Token{isFloat ? TokenType::FloatLiteral : TokenType::IntLiteral, text, tokLine, tokCol};
}

Token Lexer::stringLiteral()
{
    int tokLine = line;
    int tokCol = column;
    string value;
    while (!isAtEnd() && peek() != '"')
    {
        char c = advance();
        if (c == '\\' && !isAtEnd())
        {
            value.push_back(c);
            c = advance();
            value.push_back(c);
        }
        else
        {
            value.push_back(c);
        }
    }
    if (isAtEnd())
    {
        throw runtime_error("Unterminated string literal at line " + to_string(tokLine));
    }
    advance();
    return Token{TokenType::StringLiteral, value, tokLine, tokCol};
}

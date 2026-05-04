#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <vector>
#include <utility> // для std::pair

class Lexer
{
public:
    explicit Lexer(const std::string &source); // конструктор из исходной строки
    std::vector<Token> tokenize();             // главный метод который возваращает вектор из лексем
private:
    const std::string source; // исходная строка
    int pos = 0;              // смещение по строке
    int line = 1;
    int column = 1;

    char peek() const; // возвращает текущий символ не двигая pos
    char peekNext() const;
    char advance();            // съедает текущий символ, возрващает и двигает индексы
    bool isAtEnd() const;      // проверяет конец ли чтобы оставновить лексер
    bool match(char expected); // проверяет совпдает этот ли символ с expected (для == итд)

    void skipWhitespaceAndComments();
    Token makeToken(TokenType type, const std::string &lexeme = "");
    Token identifierOrKeyword();
    Token numberLiteral();
    Token stringLiteral();

    // Словарь ключевых слов, массив пар <строка, тип токена>
    static const std::pair<std::string, TokenType> keywords[];
    static const int keywordsCount;
};

#endif
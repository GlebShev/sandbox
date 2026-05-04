#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"
#include "token.h"

using namespace std;

void printTokens(const vector<Token> &tokens)
{
    for (int i = 0; i < tokens.size(); i++)
    {
        const Token &tok = tokens[i];
        cout << static_cast<int>(tok.type) << "\t" << tok.lexeme
             << "\t(line " << tok.line << ", col " << tok.column << ")";
        cout << '\n';
    }
}

void test(const string &input, const vector<pair<TokenType, string>> &expected)
{
    cout << "Input:   " << input << "\n";
    Lexer lexer(input);
    auto tokens = lexer.tokenize();
    cout << "Got:     \n";
    printTokens(tokens);
    cout << "Expected:\n";
    for (size_t i = 0; i < expected.size(); ++i)
    {
        cout << static_cast<int>(expected[i].first) << "\t" << expected[i].second << '\n';
    }
    cout << string(40, '-') << "\n";
}

int main()
{
    test("var x; func test() {};", {{TokenType::Var, "var"},
                                    {TokenType::Identifier, "x"},
                                    {TokenType::Semicolon, ";"},
                                    {TokenType::Func, "func"},
                                    {TokenType::Identifier, "test"},
                                    {TokenType::LParen, "("},
                                    {TokenType::RParen, ")"},
                                    {TokenType::LBrace, "{"},
                                    {TokenType::RBrace, "}"},
                                    {TokenType::Semicolon, ";"},
                                    {TokenType::EndOfFile, ""}});

    test("123 45.67", {{TokenType::IntLiteral, "123"},
                       {TokenType::FloatLiteral, "45.67"},
                       {TokenType::EndOfFile, ""}});

    test("\"hello\\nworld\\\"\" text", {{TokenType::StringLiteral, "hello\\nworld\\\""},
                                        {TokenType::Identifier, "text"},
                                        {TokenType::EndOfFile, ""}});

    test("a==b != c <= d && e || f +", {{TokenType::Identifier, "a"},
                                        {TokenType::EQ, "=="},
                                        {TokenType::Identifier, "b"},
                                        {TokenType::NEQ, "!="},
                                        {TokenType::Identifier, "c"},
                                        {TokenType::LTE, "<="},
                                        {TokenType::Identifier, "d"},
                                        {TokenType::And, "&&"},
                                        {TokenType::Identifier, "e"},
                                        {TokenType::Or, "||"},
                                        {TokenType::Identifier, "f"},
                                        {TokenType::Plus, "+"},
                                        {TokenType::EndOfFile, ""}});
}

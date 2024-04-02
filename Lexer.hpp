/**
 * @file lexer.hpp
 * @brief Declaration of the Lexer class for tokenizing source code.
 * 
 * The Lexer class is responsible for converting a raw string of source code into a sequence of tokens. These tokens
 * are then used by the Parser to construct an abstract syntax tree (AST). The Lexer analyzes the source code
 * character by character, grouping them into tokens that represent identifiable units such as keywords, symbols,
 * literals, identifiers, and operators.
 * 
 * Supported Token Types:
 * - Basic arithmetic operators: PLUS, MINUS, MUL, DIV
 * - Parentheses: LPAREN, RPAREN
 * - Identifiers and keywords: IDENTIFIER, PRINT, IF, ELSE
 * - Literal values: INTEGER, STRING
 * - Comparison and assignment operators: ASSIGN, EQUAL, NOT_EQUAL, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL
 * - Other syntactic markers: SEMICOLON, COMMA, COLON, END_OF_FILE, UNKNOWN
 * 
 * Usage:
 * Instantiate the Lexer with a string containing the source code, then call `tokenize()` to generate the tokens:
 * The resulting vector of tokens can then be passed to a Parser instance for constructing the AST.
 * 
 */


#pragma once
#include <string>
#include <vector>

enum class TokenType {
    INTEGER, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, IDENTIFIER, ASSIGN, END_OF_FILE, UNKNOWN, PRINT,
    SEMICOLON, IF, ELSE, STRING, COMMA, EQUAL, GREATER, LESS, NOT_EQUAL, GREATER_EQUAL, LESS_EQUAL,
    COLON, TAB

};

struct Token {
    TokenType type;
    std::string lexeme;
    Token(TokenType type, const std::string& lexeme) : type(type), lexeme(lexeme) {}
};

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    std::vector<Token> tokens;
    size_t start = 0;
    size_t current = 0;
    bool isAtEnd() const;
    char advance();
    void scanToken();
    void tokenizeNumber();
    void tokenizeIdentifier();
    char peek() const;
    void addToken(TokenType type, const std::string& text);
    void tokenizeString();
    void tokenizeEqual();
    void tokenizeNotEqual();
    void tokenizeLess();
    void tokenizeGreater();
};

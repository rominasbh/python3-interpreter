/**
 * @file lexer.cpp
 * @brief Implementation of the Lexer class.
 * 
 * The Lexer class processes a string of source code and converts it into a sequence of tokens.
 * This implementation supports a simplified subset of a Python-like language, recognizing 
 * basic arithmetic operators, parentheses, identifiers, literals, and keywords.
 * 
 * Key Methods:
 * - tokenize(): Processes the entire source string and converts it into tokens.
 * - scanToken(): Determines the type of the next token and calls the appropriate handler.
 * 
 * Special Handling:
 * - Strings: Recognized by surrounding double quotes. Supports escaped characters.
 * - Comments: Starts with '#' and continues to the end of the line. Comments are ignored during tokenization.
 * - Greater/Less and their equals: Handles both simple ('>', '<') and compound ('>=', '<=') comparison operators.
 * - Equal/Not Equal: Recognizes '==' and '!=' as comparison operators.
 * - Numbers and Identifiers: Distinguishes between numeric literals and identifiers (which may include numbers as part of their name).
 * 
 * Error Handling:
 * - Unterminated strings: Throws a runtime_error exception if a string literal is not properly closed before the end of the source.
 */

#include "Lexer.hpp"
#include <iostream>
#include <unordered_map>

Lexer::Lexer(const std::string& source) : source(source) {}

    std::vector<Token> Lexer::tokenize() {
        while (!isAtEnd()) {
            start = current; // We are at the beginning of the next lexeme
            scanToken();

        }

        tokens.push_back(Token(TokenType::END_OF_FILE, ""));
        return tokens;
    }

    void Lexer::addToken(TokenType type, const std::string& text) {
        tokens.push_back(Token(type, text));
    }

    bool Lexer::isAtEnd() const {
        return current >= source.length();
    }

    char Lexer::advance() {
        return source[current++];
        
    }

    void Lexer::scanToken() {
    char c = advance();
    switch (c) {
        case '+': addToken(TokenType::PLUS, "+"); break;
        case '-': addToken(TokenType::MINUS, "-"); break;
        case '*': addToken(TokenType::MUL, "*"); break;
        case '/': addToken(TokenType::DIV, "/"); break;
        case '(': addToken(TokenType::LPAREN, "("); break;
        case ')': addToken(TokenType::RPAREN, ")"); break;
        case ',': addToken(TokenType::COMMA, ","); break;
        case ':': addToken(TokenType::COLON, ":"); break;
        case '>': tokenizeGreater(); break;
        case '<': tokenizeLess(); break;
        case '!': tokenizeNotEqual(); break;
        case '=': tokenizeEqual(); break;
        case '"': tokenizeString(); break; // double quotes for strings
        case '#':
            // Handle comment 
            while(peek() != '\n' && !isAtEnd()) advance(); // Skip to end of line
            break;
        default:
            if (isdigit(c)) {
                tokenizeNumber();
            } else if (isalpha(c)) {
                tokenizeIdentifier();
            } else if (!isspace(c)) {
                addToken(TokenType::UNKNOWN, std::string(1, c));
                //maybe this should be reported as an error instead?
            }
            break;
    }
}



void Lexer::tokenizeGreater(){
    size_t lookahead = current +1 ;
    if(source[lookahead]==' '){
        addToken(TokenType::GREATER_EQUAL, ">=");
        advance();
    }
    else{addToken(TokenType::GREATER, ">");}
}
void Lexer::tokenizeLess(){
    size_t lookahead = current +1 ;
    if(source[lookahead]==' '){
        addToken(TokenType::LESS_EQUAL, "<=");
        advance();
    }
    else{addToken(TokenType::LESS, "<");}
}
void Lexer::tokenizeNotEqual(){
    size_t lookahead = current +1 ;
    if(source[lookahead]==' '){
        addToken(TokenType::NOT_EQUAL, "!=");
        advance();
    }
}


void Lexer::tokenizeEqual() {
    size_t lookahead = current + 1;
    if (source[lookahead] == ' ') {
        addToken(TokenType::EQUAL, "==");
        advance(); // Consume the second '='
    } else {
        addToken(TokenType::ASSIGN, "=");
    }
}

void Lexer::tokenizeNumber() {
    while (isdigit(peek())) advance(); // Consume digits
    
    // Lookahead to see if next non-space character is alphabetic (invalid number token)
    size_t lookahead = current;
    while (!isspace(source[lookahead])) lookahead++;
    if (isalpha(source[lookahead])) {
        // It's not a valid number; treat as an identifier
        current = lookahead; // Move current to where lookahead stopped
        tokenizeIdentifier(); // handle as identifier
        return;
    }

    // It's a valid number; capture it
    std::string number = source.substr(start, current - start);
    addToken(TokenType::INTEGER, number);
}

void Lexer::tokenizeIdentifier() {
    while (isalnum(peek())) advance(); // Consume alphanumeric characters

    std::string text = source.substr(start, current - start);

    // Keywords map
    static const std::unordered_map<std::string, TokenType> keywords = {
        {"print", TokenType::PRINT},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
    
    };

    auto keyword = keywords.find(text);
    if (keyword != keywords.end()) {
        addToken(keyword->second, text);
    } else {
        addToken(TokenType::IDENTIFIER, text);
    }
}

void Lexer::tokenizeString() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\\') { // Check for escape character
            advance(); // Skip the escape character
        }
        advance(); // Move to next character
    }

    if (isAtEnd()) {
        throw std::runtime_error("Unterminated string.");
    }

    // Consume the closing "
    advance();

    // Trim the surrounding quotes
    std::string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
}


char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}





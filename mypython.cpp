/**
 * Main entry point for the mypython interpreter.
 * 
 * This program simulates a basic Python interpreter capable of executing a subset
 * of the Python language, focusing on integer arithmetic, variable assignments, and
 * conditional statements. It reads a Python source file, tokenizes the input, parses
 * it into an abstract syntax tree (AST), and finally interprets the AST to execute the
 * program.
 * 
 * Usage:
 *   ./mypython <file.py>
 * 
 * The program expects exactly one command-line argument, which is the path to the
 * Python source file to be interpreted. It demonstrates a simplified workflow of a
 * programming language interpreter by leveraging three major components:
 * 
 * - Lexer: Tokenizes the input source code into a sequence of tokens.
 * - Parser: Analyzes the tokens to build an abstract syntax tree representing the
 *   structure of the source code.
 * - Interpreter: Walks the AST and executes the code according to the semantics
 *   of the language.
 * 
 * This file integrates these components and orchestrates the process from reading
 * the source file to executing the interpreted code.
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"


int main(int argc, char* argv[]) {
    // Check for correct usage
    if (argc != 2) {
        std::cerr << "Usage: mypython <source_file>" << std::endl;
        return 1;
    }

    // Open the source file
    std::string filename = argv[1];
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return 1;
    }

    // Read the file's content into a string
    std::stringstream buffer;
    buffer << file.rdbuf(); // Read the file's content into a string stream
    std::string source = buffer.str(); // Convert the string stream into a string
    file.close();

    // Tokenize the source code
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    // Parse the tokens into an AST
    Parser parser(tokens);
    auto ast = parser.parse(); 

    // Ensure parsing resulted in an AST node
    if (!ast) {
        std::cerr << "Parsing error encountered." << std::endl;
        return 1;
    }

    // Interpret the AST
    Interpreter interpreter;
    interpreter.interpret(std::move(ast)); 

    return 0;
}

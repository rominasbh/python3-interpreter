/**
 * Main entry point for the mypython interpreter.
 * 
 * This program simulates a basic Python interpreter capable of executing a subset
 * of the Python language, focusing on integer arithmetic, variable assignments, and
 * conditional statements. It reads a Python source file, tokenizes the input, parses
 * it into an abstract syntax tree (AST), and finally interprets the AST to execute the
 * program. There is an addiitonal features for debugging and traceability: It appends 
 * execution details, including timestamps and file names, to a trace file ('trace.log'). 
 * This facilitates tracking the execution of various scripts over time and capturing any 
 *runtime errors for analysis.
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
 * - Utilities: Includes utility functions like 'tee' for output redirection and logging to a trace file.
 * This file integrates these components and orchestrates the process from reading
 * the source file to executing the interpreted code.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include "Utilities.hpp"
#include <chrono>
#include <ctime>



int main(int argc, char* argv[]) {

    //    std::ofstream traceFile("trace.log");
    std::ofstream traceFile("trace.log", std::ios::app); // Open for appending
    //time trace log is opened
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    


    if (!traceFile.is_open()) {
        std::cerr << "Failed to open trace file for writing." << std::endl;
        return 1;
    }

    // Redirect std::cout and std::cerr to both console and trace file
    tee(std::cout, std::cout, traceFile);
    tee(std::cerr, std::cerr, traceFile);

    try{
        // Check for correct usage
        if (argc != 2) {
            std::cerr << "Usage: mypython <source_file>" << std::endl;
            return 1;
        }

        // Writing the timestamp and the filename to the trace file
        traceFile << '\n' << "Run at: " << std::ctime(&now_time) << "File: " << argv[1] << std::endl;

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
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }

        return 0;
 }

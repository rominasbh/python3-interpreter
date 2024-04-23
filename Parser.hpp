/**
 * parser.hpp - Documentation
 *
 * Overview:
 * The parser module transforms a sequence of tokens from the lexer into an abstract syntax tree (AST),
 * which represents the structure of the source code. This AST is then used by the interpreter for execution.
 * The parser supports expressions, variable assignments, conditional statements (if/else), and print statements.
 *
 * Data Structures:
 * - ASTNode: Base class for all nodes in the AST, used to represent both expressions and statements.
 * - Expr and Stmt: Derived from ASTNode, these abstract classes represent expressions and statements in the AST.
 *   Specific types of expressions and statements are further derived from these.
 * - std::unique_ptr<Expr> and std::unique_ptr<Stmt>: Smart pointers used for memory management of AST nodes,
 *   ensuring automatic deallocation of dynamically allocated nodes.
 * - std::vector<std::unique_ptr<Stmt>>: A vector of unique pointers to statement nodes, used for blocks of
 *   statements, allowing the parser to represent compound statements and control structures.
 *
 * Key Classes:
 * - BinaryExpr, LiteralExpr, VarExpr, AssignExpr: Derived from Expr, these classes represent binary operations,
 *   integer literals, variable references, and assignments, respectively.
 * - AssignStmt, PrintStmt, IfStmt, BlockStmt: Derived from Stmt, these classes represent assignment statements,
 *   print statements, conditional statements, and blocks of statements, respectively.
 *
 * Variables:
 * The parser does not directly store variables; it constructs nodes representing variable assignments and
 * references. The actual storage and retrieval of variable values are handled by the environment in the
 * interpreter module.
 */


#pragma once
#include "Lexer.hpp"
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include "Env.hpp"

// Forward declaration
class Interpreter;
class Expr;
class Stmt;

class ASTNode {
public:
    enum NodeType {
        ExprNode,
        StmtNode
    };
    ASTNode() = default;
    virtual ~ASTNode() = default;
    virtual void execute(Interpreter& interpreter, Environment& env) = 0;    
    NodeType type;
    std::unique_ptr<Expr> expr;
    std::unique_ptr<Stmt> stmt;

    // Constructor for expression nodes
    ASTNode(std::unique_ptr<Expr> e) : type(ExprNode), expr(std::move(e)) {}

    // Constructor for statement nodes
    ASTNode(std::unique_ptr<Stmt> s) : type(StmtNode), stmt(std::move(s)) {}
};



/**
 * The base class for all expressions in the AST. Requires implementation of the evaluate method.
 */
class Expr : public ASTNode {
public:
    virtual ~Expr() = default;
    virtual int evaluate(Environment& env) = 0; // Accept an Environment reference
};

class BinaryExpr : public Expr {
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    TokenType op;

public:
    BinaryExpr(std::unique_ptr<Expr> left, TokenType op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    // Getter for left
    const std::unique_ptr<Expr>& getLeft() const { return left; }
    
    // Getter for right
    const std::unique_ptr<Expr>& getRight() const { return right; }

    // Getter for op 
    const TokenType getOp() const { return op; }

    int evaluate(Environment& env) override { // accepts an Environment reference
        int leftVal = left->evaluate(env); // Pass the environment to left expression
        int rightVal = right->evaluate(env); // Pass the environment to right expression
        int result;
        switch (op) {
            case TokenType::PLUS: return leftVal + rightVal;
            case TokenType::MINUS: return leftVal - rightVal;
            case TokenType::MUL: return leftVal * rightVal;
            case TokenType::DIV:
                if (rightVal == 0) {
                    throw std::runtime_error("Division by zero.");
                }
                // Adjust for floor division in cases with different signs
                result = leftVal / rightVal;
                // Check if correction is needed for floor division (operands have different signs and division is not exact)
                if ((leftVal < 0) ^ (rightVal < 0) && (leftVal % rightVal != 0)) {
                    result--;
                }
                return result;
            case TokenType::EQUAL:
            // std::cout << "binary operation == result " << (leftVal == rightVal) << std::endl;
            return leftVal == rightVal;
            case TokenType::LESS_EQUAL:    
            return leftVal <= rightVal;
            case TokenType::LESS:
            return leftVal < rightVal;
            case TokenType::GREATER:
            return leftVal > rightVal;
            case TokenType::GREATER_EQUAL:
            return leftVal >= rightVal;
            default:
                throw std::runtime_error("Unsupported binary operator.");
        }
    }
    void execute(Interpreter& interpreter, Environment& env) override {
        
        std::cout << "BinaryExpr value: " << evaluate(env) << std::endl;
    }
};


class LiteralExpr : public Expr {
    int value;

public:
    // Constructor initializes 'value' with the integer value passed to it.
    LiteralExpr(int value) : value(value) {}

    //  accepts an Environment reference.
    // The environment is not used for literal expressions, but it's included to match the Expr interface.
    int evaluate(Environment& env) override {
        return value;
    }

    // Getter method for 'value', indicating the method doesn't modify any class members.
    const int& getValue() const { return value; }

    void execute(Interpreter& interpreter, Environment& env) override {
        
        std::cout << "LiteralExpr value: " << value << std::endl;
    }
};

class VarExpr : public Expr {
    std::string name;

public:
    VarExpr(const std::string& name) : name(name) {}

    int evaluate(Environment& env) override {
        return env.get(name); // Use the environment to look up the variable's value
    }
    // Getter for name
    const std::string& getName() const { return name; }

    void execute(Interpreter& interpreter, Environment& env) override {
        
        std::cout << "VarExpr value: " << evaluate(env) << std::endl;
    }
};


class AssignExpr : public Expr {
    std::string name;
    std::unique_ptr<Expr> value;

public:
    AssignExpr(const std::string& name, std::unique_ptr<Expr> value)
        : name(name), value(std::move(value)) {}
    
    int evaluate(Environment& env) override {
        int val = value->evaluate(env); // Evaluate the right-hand side expression with the current environment
        env.define(name, val); // Update the environment with the new value for this variable
        return val; // Return the assigned value, allowing for expressions like a = b = 5
    }

    const std::unique_ptr<Expr>& getValue() const {return value;}
    const std::string& getName() const {return name;}
    void execute(Interpreter& interpreter, Environment& env) override {
        
        std::cout << "AssignExpr value: " << evaluate(env) << std::endl;
    }
};


/**
 * The base class for all statements in the AST. Requires implementation of the execute method.
 */
class Stmt : public ASTNode {
public:
    virtual ~Stmt() = default;
    virtual void execute(Interpreter& interpreter, Environment& env) = 0; // include Environment reference
};

class AssignStmt : public Stmt {
    std::string name;
    std::unique_ptr<Expr> value;
public:
    //AssignStmt(const std::string& name, std::unique_ptr<Expr> value) : name(name), value(std::move(value));
    AssignStmt(const std::string& name, std::unique_ptr<Expr> value);
    // Getter for value
    const std::unique_ptr<Expr>& getValue() const { return value; }
    // Getter for name
    const std::string& getName() const { return name; }

    void execute(Interpreter& interpreter, Environment& env) override;

};


class StringLiteralExpr : public Expr {
    std::string value;
public:
    StringLiteralExpr(const std::string& value) : value(value) {}

    const std::string& getValue() const { return value; }

    std::string evaluatee(Environment& env) {
        
        return value;
    }
    int evaluate(Environment& env) override {return 0;}

    void execute(Interpreter& interpreter, Environment& env) override {
        std::cout << evaluatee(env) << std::endl; // Print the evaluated expression result
    }
};

class CallExpr : public Expr {
    std::string functionName;
    std::vector<std::unique_ptr<Expr>> arguments;
    Interpreter& interpreter;

public:
    CallExpr(const std::string& functionName, std::vector<std::unique_ptr<Expr>> arguments, Interpreter& interpreter)
        : functionName(functionName), arguments(std::move(arguments)), interpreter(interpreter) {}
   
    virtual int evaluate(Environment& env) override;
    virtual void execute(Interpreter& interpreter, Environment& env) override;

    // Utility to convert argument expressions to their evaluated results
    std::vector<int> convertArgumentsToValues(const std::vector<std::unique_ptr<Expr>>& args, Environment& env) {
        std::vector<int> result;
        for (const auto& arg : args) {
            result.push_back(arg->evaluate(env));
        }
        return result;
    }
};




class IfStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> ifBranch;  // Actions to execute if the condition is true
    std::unique_ptr<Stmt> elseBranch; // Actions to execute if the condition is false

    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> ifBranch, std::unique_ptr<Stmt> elseBranch=nullptr)
        : condition(std::move(condition)), ifBranch(std::move(ifBranch)), elseBranch(std::move(elseBranch)) {}

    void execute(Interpreter& interpreter, Environment& env) override ;

    
};



class PrintStmt : public Stmt {
    std::vector<std::unique_ptr<Expr>> expressions;
    

public:
    PrintStmt(std::vector<std::unique_ptr<Expr>> expressions) : expressions(std::move(expressions)) {}


    void execute(Interpreter& interpreter, Environment& env) override {
        for (const auto& expr : expressions) {
            if (auto stringExpr = dynamic_cast<StringLiteralExpr*>(expr.get())) {
                std::cout << stringExpr->getValue(); // Assuming StringLiteralExpr has a `getValue` method.
            } else {
                // Fallback for other expression types, converting numeric results to strings for display.
                std::cout << expr->evaluate(env);
            }
            std::cout << " "; // Separate arguments with spaces.
        }
        std::cout << std::endl; // End the print statement with a newline.
    }

};
class ExpressionStmt : public Stmt {
    std::unique_ptr<Expr> expression;

public:
    ExpressionStmt(std::unique_ptr<Expr> expr) : expression(std::move(expr)) {}

    virtual void execute(Interpreter& interpreter, Environment& env) override {
        expression->evaluate(env);  // The return value can be ignored if not needed
    }
};

class ReturnStmt : public Stmt {
    std::unique_ptr<Expr> returnValue;
public:
    ReturnStmt(std::unique_ptr<Expr> returnValue) : returnValue(std::move(returnValue)) {}
    void execute(Interpreter& interpreter, Environment& env) override;
};

class FunctionStmt : public Stmt {
private:
    std::string name;  // Function name
    std::vector<std::string> parameters;  // List of parameter names
    std::unique_ptr<Stmt> body;  // The body of the function

public:
    // Constructor
    FunctionStmt(const std::string& name, std::vector<std::string> parameters, std::unique_ptr<Stmt> body)
        : name(name), parameters(std::move(parameters)), body(std::move(body)) {}

    // Execute function in interpreter context
    virtual void execute(Interpreter& interpreter, Environment& env) override;

    // Getters for the function's components
    const std::string& getName() const { return name; }
    const std::vector<std::string>& getParameters() const { return parameters; }
    const std::unique_ptr<Stmt>& getBody() const { return body; }
};


class BlockStmt : public Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;
public:
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements);
    
    void execute(Interpreter& interpreter, Environment& env) override;
    
    // Getter for statement
    const std::vector<std::unique_ptr<Stmt>>& getStatements() {return statements;}
};



class Parser {
private:
    std::vector<Token> tokens;
    size_t current = 0;
    Interpreter& interpreter;

    // Utility methods...

    bool isAtEnd() const {
        return current >= tokens.size();
    }

    Token advance() {
        if (!isAtEnd()) return tokens[current++];
        //this line is for debugging
        if (current < tokens.size()) std::cout << "Consuming Token: " << tokens[current].lexeme << std::endl;  
        return tokens.back(); // Return the last token if at end (should be EOF token)
    }

    Token peek() const {
        if (!isAtEnd()) return tokens[current];
        return tokens.back(); // Safeguard against going past the end
    }

    bool match(std::initializer_list<TokenType> types) {
        for (TokenType type : types) {
            if (!isAtEnd() && peek().type == type) {
                advance();
                return true;
            }
        }
        return false;
    }

    void consume(TokenType type, const std::string& message) {
    if (check(type)) {
        advance();
    } else {
        // throw std::runtime_error(message);
        // this one is for degugging replace with ^ when done debugging
        error(peek(), message + " instead found token type: " + std::to_string(static_cast<int>(peek().type)));
        throw std::runtime_error(message + " instead found " + peek().lexeme);
    }
    }

    bool check(TokenType type) const {
        if (isAtEnd()) return false;
        return peek().type == type;
    }

    Token previous() const {
        return tokens.at(current - 1);
    }
 
    void error(const Token& token, const std::string& message) {
    std::cerr << "Error at " << token.lexeme << ": " << message << std::endl;
    // You might want to throw an exception or handle the error based on your application's needs.
    }


public:
    // Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}
    Parser(const std::vector<Token>& tokens, Interpreter& interpreter)
        : tokens(tokens), current(0), interpreter(interpreter) {}


    std::unique_ptr<Stmt> parse();
    std::unique_ptr<Expr> parseExpression();
    std::unique_ptr<Stmt> parseStatement();
    // Helper methods for parsing different precedence levels of expressions
    std::unique_ptr<Stmt> parseBlock();
    std::unique_ptr<Expr> parsePrimary();
    std::unique_ptr<Expr> parseUnary();
    std::unique_ptr<Expr> parseFactor();  // Multiplication and Division
    std::unique_ptr<Expr> parseTerm();    // Addition and Subtraction
    std::unique_ptr<Stmt> parsePrintStatement();
    std::unique_ptr<Expr> parseComparison();
    void synchronize();
    std::unique_ptr<Stmt> parseIfStatement();
    std::unique_ptr<Stmt> parseFunctionDefinition();
    std::unique_ptr<Stmt> parseReturnStatement();
    std::unique_ptr<Expr> parseFunctionCall(const std::string& functionName);

};


/**
 * @file interpreter.hpp
 * @brief Defines the Interpreter class for executing AST nodes.
 * 
 * The Interpreter class is the core component that executes the abstract syntax tree (AST) produced by the parser.
 * It evaluates expressions and executes statements by traversing the AST and applying the operations defined in each node.
 * The class uses an Environment to track variable definitions and scopes throughout the execution process.
 * 
 * Key Features:
 * - interpret(): Begins the execution process by traversing the AST starting from the root node.
 * - evaluateExpr(): Evaluates expressions and returns their integer values.
 * - executeStatement(): Executes individual statements, including variable assignments and print operations.
 * - executeBlock(): Executes a series of statements in a new scope, allowing for local variable definitions.
 * 
 * The Interpreter class handles the execution of basic arithmetic operations, if-else conditional statements, and variable assignments.
 * It operates within a global environment but can create scoped environments for block execution, supporting local variables.
 * 
 * Usage:
 * The interpreter is designed to be used after parsing. Once an AST is obtained from the parser, the interpret() method
 * can be called to execute the program represented by the AST.
 */

#include <string>
#include "Env.hpp"
#include "Utilities.hpp"




/**
 * The Interpreter class is responsible for interpreting and executing the statements
 * and expressions of the AST (Abstract Syntax Tree).
 */

class Interpreter {
    Environment globalEnvironment; // The global environment, serving as the outermost scope

public:

    Interpreter() : globalEnvironment() {}

   
    void interpret(std::unique_ptr<ASTNode> root) {
        if (!root) return; // Early return if the AST is empty

        // virtual method like execute or evaluate overridden by derived classes. 
        //The interpret method does not need to know the specific type of the AST node.
        root->execute(*this, globalEnvironment);
    }

    /**
     * Evaluates an expression within a given environment.
     * @param expr The expression to evaluate.
     * @param env The environment within which the expression is evaluated.
     * @return The result of the expression evaluation as an integer.
     */
    int evaluateExpr(const std::unique_ptr<Expr>& expr, Environment& env);

    /**
     * Executes a statement within a given environment.
     * @param stmt The statement to execute.
     * @param env The environment within which the statement is executed.
     */
    void executeStatement(const std::unique_ptr<Stmt>& stmt, Environment& env);

     /**
     * Executes a block of statements, creating a new scope for the block.
     * @param statements The statements within the block to execute.
     * @param environment Scoped environment for the block.
     */   
    void executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements, Environment& environment);
    

    int callFunction(const std::string& name, const std::vector<int>& arguments,Environment& currentEnv);
    void executeFunction(const std::unique_ptr<Stmt>& functionStmt, Environment& env);
    void defineFunction(const std::string& name, std::shared_ptr<FunctionStmt> functionStmt);
};

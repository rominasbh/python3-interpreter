/**
 * @file interpreter.cpp
 * @brief Implementation of the Interpreter class.
 * 
 * This file contains the definitions of the Interpreter class methods. The Interpreter is responsible for
 * executing the abstract syntax tree (AST) by traversing it and performing the operations defined in each node.
 * It supports evaluating expressions, executing statements, and managing scoped execution blocks.
 * 
 * Implementation Details:
 * - evaluateExpr(): Calls the `evaluate` method of expression nodes, passing the environment to resolve variables.
 * - executeStatement(): Delegates the execution to the `execute` method of statement nodes.
 * - executeBlock(): Iterates over a list of statements, executing each within a newly created scoped environment.
 * 
 * The Interpreter works closely with the Environment class to track variable states and supports basic arithmetic,
 * conditional logic, and variable assignment. This implementation ensures that expressions and statements are executed
 * according to the semantics of the designed language.
 * 
 * Dependencies:
 * This implementation relies on the `Parser.hpp` for the AST structure and `Environment.hpp` for variable scope management.
 */

#include "Parser.hpp" 
#include "Interpreter.hpp" 


int Interpreter::evaluateExpr(const std::unique_ptr<Expr>& expr, Environment& env) {
    // Directly call the evaluate method on the expression, passing the current environment.
    // This removes the need for explicit dynamic casting and handling each expression type here.
    return expr->evaluate(env);
}




void Interpreter::executeStatement(const std::unique_ptr<Stmt>& stmt, Environment& env) {
    stmt->execute(*this, env); // Pass the current environment
}




void Interpreter::executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements, Environment& newEnvironment) {
    for (const auto& stmt : statements) {
        // Execute each statement within the block using the new scoped environment.
        stmt->execute(*this, newEnvironment);
    }
}

int Interpreter::callFunction(const FunctionStmt& function, std::vector<int> arguments) {
    Environment localEnvironment(&globalEnvironment);
    // Assign arguments to parameters in localEnvironment
    // Execute the function body using localEnvironment
    // Return the result of the function
}
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




void Interpreter::executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements, Environment& environment) {
    try {
        for (const auto& stmt : statements) {
            stmt->execute(*this, environment);
        }
    } catch (const ReturnValue& returnValue) {
        // If a ReturnValue is caught here, rethrow it to continue passing it up the call stack
        throw;
    }
}

// int Interpreter::callFunction(const FunctionStmt& function, std::vector<int> arguments) {
//     Environment localEnvironment(&globalEnvironment);
//     // Assign arguments to parameters in localEnvironment

//     try {
//         function.body()->execute(*this, localEnvironment);
//     } catch (const ReturnValue& returnValue) {
//         return returnValue.value; // Catch the return value and pass it back
//     }

//     return 0; // Default return if no value is returned by the function
// }

// int Interpreter::callFunction(const std::string& name, const std::vector<int>& arguments) {
//     // Retrieve the function statement from the environment
//     auto functionStmt = globalEnvironment.getFunction(name);
//     if (!functionStmt) {
//         throw std::runtime_error("Function '" + name + "' is not defined.");
//     }

//     // Create a new environment for this function call, with the current environment as its parent
//     Environment localEnvironment(&globalEnvironment);

//     // Check if the number of provided arguments matches the number of parameters
//     const auto& parameters = functionStmt->getParameters();
//     if (arguments.size() != parameters.size()) {
//         throw std::runtime_error("Incorrect number of arguments provided to function '" + name + "'.");
//     }

//     // Map the passed arguments to the function's parameters in the new local environment
//     for (size_t i = 0; i < arguments.size(); i++) {
//         localEnvironment.define(parameters[i], arguments[i]);
//     }

//     // Execute the function body in the new environment
//     try {
//         functionStmt->getBody()->execute(*this, localEnvironment);
//     } catch (const ReturnValue& returnValue) {
//         return returnValue.value; // Catch the ReturnValue and pass it back as the function's return value
//     }

//     return 0; // Default return if no value is explicitly returned by the function
// }

int Interpreter::callFunction(const std::string& name, const std::vector<int>& arguments, Environment& currentEnv) {
    auto functionStmt = currentEnv.getFunction(name);
    if (!functionStmt) {
        throw std::runtime_error("Function '" + name + "' is not defined.");
    }

    // Correctly initialize the local environment with currentEnv as the parent
    Environment localEnvironment(&currentEnv);

    const auto& parameters = functionStmt->getParameters();
    if (arguments.size() != parameters.size()) {
        throw std::runtime_error("Incorrect number of arguments provided to function '" + name + "'.");
    }

    for (size_t i = 0; i < arguments.size(); i++) {
        localEnvironment.define(parameters[i], arguments[i]);
    }

    try {
        functionStmt->getBody()->execute(*this, localEnvironment);
    } catch (const ReturnValue& returnValue) {
        return returnValue.value;
    }

    return 0;
}

void Interpreter::executeFunction(const std::unique_ptr<Stmt>& functionStmt, Environment& env) {
    try {
        functionStmt->execute(*this, env);
    } catch (const ReturnValue& ret) {
        // Handle the returned value
        std::cout << "Function returned: " << ret.value << std::endl;
    }
}

void Interpreter::defineFunction(const std::string& name, std::shared_ptr<FunctionStmt> functionStmt) {
    globalEnvironment.defineFunction(name, functionStmt);
}

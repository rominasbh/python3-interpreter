#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
class FunctionStmt;

/**
 * A simple environment for variable storage. Supports nested scopes through parent pointers.
 */
class Environment {
    std::unordered_map<std::string, int> values; // Stores variable names and their values.
    std::unordered_map<std::string, std::shared_ptr<FunctionStmt>> functions;  // Function storage
    Environment* parent; // Pointer to parent environment, nullptr for global scope


public:

    /**
     * Constructor that optionally takes a parent environment to support nested scopes.
     * @param parent The parent environment, nullptr if this is the global environment.
     */
    Environment(Environment* parent = nullptr) : parent(parent) {}
   
    /**
     * Defines or updates a variable in the environment.
     * @param name The name of the variable.
     * @param value The value to be assigned to the variable.
     */
    void define(const std::string& name, int value) {
        values[name] = value;
    }

    /**
     * Retrieves the value of a variable from the environment. Searches in the current
     * environment and up through parent environments if necessary.
     * @param name The name of the variable to retrieve.
     * @return The value of the variable.
     * @throws std::runtime_error If the variable is not defined in the environment or any parent environment.
     */
    int get(const std::string& name) {
        if (values.count(name)) {
            return values[name];
        } else if (parent != nullptr) {
            return parent->get(name);
        } else {
            throw std::runtime_error("Variable '" + name + "' is not defined.");
        }
        
    }


    void mergeChanges(const Environment& childEnv) {
    // merge variables from childEnv back into this environment.
        for (const auto& var : childEnv.values) {  
            this->values[var.first] = var.second;
        }
}



    void defineFunction(const std::string& name, std::shared_ptr<FunctionStmt> function);
    std::shared_ptr<FunctionStmt> getFunction(const std::string& name);


};
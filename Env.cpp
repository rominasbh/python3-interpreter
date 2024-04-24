#include "Env.hpp"
#include "Parser.hpp"  

void Environment::defineFunction(const std::string& name, std::shared_ptr<FunctionStmt> function) {
    functions[name] = function;
}

std::shared_ptr<FunctionStmt> Environment::getFunction(const std::string& name) {
    auto it = functions.find(name);
    if (it != functions.end()) {
        return it->second;
    } else if (parent != nullptr) {
        return parent->getFunction(name);
    }
    throw std::runtime_error("Function '" + name + "' is not defined.");
}


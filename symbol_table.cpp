#include "symbol_table.h"
#include "error_handler.h"
#include <iostream>
#include <iomanip>
#include <fstream>

// Constructor
SymbolTable::SymbolTable(std::shared_ptr<ErrorHandler> errHandler)
    : currentScopeLevel(0), errorHandler(errHandler) {
    // Push global scope
    scopeStack.push(currentScopeLevel);
}

// Set error handler
void SymbolTable::setErrorHandler(std::shared_ptr<ErrorHandler> errHandler) {
    errorHandler = errHandler;
}

// Enter a new scope
void SymbolTable::enterScope() {
    currentScopeLevel++;
    scopeStack.push(currentScopeLevel);
}

// Exit current scope
void SymbolTable::exitScope() {
    if (!scopeStack.empty()) {
        scopeStack.pop();
        if (!scopeStack.empty()) {
            currentScopeLevel = scopeStack.top();
        } else {
            // Fallback to global scope if stack is empty
            currentScopeLevel = 0;
            scopeStack.push(currentScopeLevel);
        }
    }
}

// Get current scope level
int SymbolTable::getCurrentScope() const {
    return currentScopeLevel;
}

// Insert a symbol in current scope
bool SymbolTable::insert(const std::string& name, DataType type, int line, int column) {
    return insertInScope(name, type, line, column, currentScopeLevel);
}

// Insert a symbol in a specific scope
bool SymbolTable::insertInScope(const std::string& name, DataType type, int line, int column, int scope) {
    std::pair<std::string, int> key = {name, scope};
    
    // Check if symbol already exists in this scope
    if (symbols.find(key) != symbols.end()) {
        if (errorHandler) {
            errorHandler->semanticError("Redeclaration of '" + name + "' in the same scope", line, column);
        }
        return false;
    }
    
    // Create and insert the new symbol
    auto symbol = std::make_shared<Symbol>(name, type, line, column, scope);
    symbols[key] = symbol;
    return true;
}

// Lookup a symbol in all visible scopes
std::shared_ptr<Symbol> SymbolTable::lookup(const std::string& name) const {
    // Start from current scope and move up to parent scopes
    for (int scope = currentScopeLevel; scope >= 0; scope--) {
        auto symbol = lookupInScope(name, scope);
        if (symbol) {
            return symbol;
        }
    }
    return nullptr;
}

// Lookup a symbol in a specific scope
std::shared_ptr<Symbol> SymbolTable::lookupInScope(const std::string& name, int scope) const {
    std::pair<std::string, int> key = {name, scope};
    auto it = symbols.find(key);
    if (it != symbols.end()) {
        return it->second;
    }
    return nullptr;
}

// Check if a symbol exists in any visible scope
bool SymbolTable::exists(const std::string& name) const {
    return lookup(name) != nullptr;
}

// Check if a symbol exists in a specific scope
bool SymbolTable::existsInScope(const std::string& name, int scope) const {
    return lookupInScope(name, scope) != nullptr;
}

// Update symbol type
bool SymbolTable::updateType(const std::string& name, DataType type) {
    auto symbol = lookup(name);
    if (!symbol) {
        if (errorHandler) {
            errorHandler->semanticError("Cannot update type of undeclared variable '" + name + "'", 0, 0);
        }
        return false;
    }
    
    symbol->type = type;
    return true;
}

// Update symbol int value
bool SymbolTable::updateIntValue(const std::string& name, int value) {
    auto symbol = lookup(name);
    if (!symbol) {
        if (errorHandler) {
            errorHandler->semanticError("Cannot assign to undeclared variable '" + name + "'", 0, 0);
        }
        return false;
    }
    
    if (symbol->type != DataType::INT) {
        if (errorHandler) {
            errorHandler->semanticError("Type mismatch: cannot assign integer to non-integer variable '" + name + "'", 0, 0);
        }
        return false;
    }
    
    symbol->value = value;
    symbol->initialized = true;
    return true;
}

// Update symbol float value
bool SymbolTable::updateFloatValue(const std::string& name, float value) {
    auto symbol = lookup(name);
    if (!symbol) {
        if (errorHandler) {
            errorHandler->semanticError("Cannot assign to undeclared variable '" + name + "'", 0, 0);
        }
        return false;
    }
    
    if (symbol->type != DataType::FLOAT) {
        if (errorHandler) {
            errorHandler->semanticError("Type mismatch: cannot assign float to non-float variable '" + name + "'", 0, 0);
        }
        return false;
    }
    
    symbol->value = value;
    symbol->initialized = true;
    return true;
}

// Get all symbols in a specific scope
std::vector<std::shared_ptr<Symbol>> SymbolTable::getSymbolsInScope(int scope) const {
    std::vector<std::shared_ptr<Symbol>> result;
    for (const auto& [key, symbol] : symbols) {
        if (key.second == scope) {
            result.push_back(symbol);
        }
    }
    return result;
}

// Print the symbol table
void SymbolTable::print() const {
    std::cout << "==== Symbol Table ====" << std::endl;
    std::cout << std::left << std::setw(15) << "Name" 
              << std::setw(10) << "Type" 
              << std::setw(15) << "Value" 
              << std::setw(10) << "Line" 
              << std::setw(10) << "Column"
              << std::setw(10) << "Scope"
              << std::setw(15) << "Initialized" 
              << std::endl;
    std::cout << std::string(85, '-') << std::endl;
    
    for (const auto& [key, symbol] : symbols) {
        std::string typeStr;
        std::string valueStr = "N/A";
        
        // Get type string
        switch (symbol->type) {
            case DataType::INT: typeStr = "INT"; break;
            case DataType::FLOAT: typeStr = "FLOAT"; break;
            default: typeStr = "UNKNOWN"; break;
        }
        
        // Get value string if initialized
        if (symbol->initialized) {
            if (symbol->type == DataType::INT) {
                valueStr = std::to_string(std::get<int>(symbol->value));
            } else if (symbol->type == DataType::FLOAT) {
                valueStr = std::to_string(std::get<float>(symbol->value));
            }
        }
        
        std::cout << std::left << std::setw(15) << symbol->name 
                  << std::setw(10) << typeStr 
                  << std::setw(15) << valueStr 
                  << std::setw(10) << symbol->line 
                  << std::setw(10) << symbol->column
                  << std::setw(10) << symbol->scope
                  << std::setw(15) << (symbol->initialized ? "Yes" : "No")
                  << std::endl;
    }
    
    std::cout << "Current scope level: " << currentScopeLevel << std::endl;
    std::cout << "======================" << std::endl;
}

// Write symbol table to file
void SymbolTable::writeToFile(const std::string& filename) const {
    // Create output directory if it doesn't exist
    #ifdef _WIN32
    std::system("if not exist output mkdir output");
    #else
    std::system("mkdir -p output");
    #endif
    
    std::string outputPath = "output/" + filename;
    std::ofstream file(outputPath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << outputPath << " for writing symbol table." << std::endl;
        return;
    }
    
    file << "Name,Type,Value,Line,Column,Scope,Initialized" << std::endl;
    
    for (const auto& [key, symbol] : symbols) {
        std::string typeStr;
        std::string valueStr = "N/A";
        
        // Get type string
        switch (symbol->type) {
            case DataType::INT: typeStr = "INT"; break;
            case DataType::FLOAT: typeStr = "FLOAT"; break;
            default: typeStr = "UNKNOWN"; break;
        }
        
        // Get value string if initialized
        if (symbol->initialized) {
            if (symbol->type == DataType::INT) {
                valueStr = std::to_string(std::get<int>(symbol->value));
            } else if (symbol->type == DataType::FLOAT) {
                valueStr = std::to_string(std::get<float>(symbol->value));
            }
        }
        
        file << symbol->name << ","
             << typeStr << ","
             << valueStr << ","
             << symbol->line << ","
             << symbol->column << ","
             << symbol->scope << ","
             << (symbol->initialized ? "Yes" : "No")
             << std::endl;
    }
    
    file.close();
}

// Get all symbols
std::vector<std::shared_ptr<Symbol>> SymbolTable::getAllSymbols() const {
    std::vector<std::shared_ptr<Symbol>> result;
    for (const auto& [key, symbol] : symbols) {
        result.push_back(symbol);
    }
    return result;
}

// Clear the symbol table
void SymbolTable::clear() {
    symbols.clear();
    
    // Reset scope
    while (!scopeStack.empty()) {
        scopeStack.pop();
    }
    currentScopeLevel = 0;
    scopeStack.push(currentScopeLevel);
} 
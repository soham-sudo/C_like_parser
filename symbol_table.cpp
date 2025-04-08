#include "symbol_table.h"
#include "error_handler.h"
#include <iostream>
#include <fstream>
#include <filesystem>

// Constructor
SymbolTable::SymbolTable(std::shared_ptr<ErrorHandler> errHandler)
    : errorHandler(errHandler), nextSerialNo(1) {}

// Set error handler
void SymbolTable::setErrorHandler(std::shared_ptr<ErrorHandler> errHandler) {
    errorHandler = errHandler;
}

// Insert a symbol
bool SymbolTable::insert(const std::string& name, int line, int column) {
    // Check if symbol already exists
    if (exists(name)) {
        if (errorHandler) {
            errorHandler->semanticError("Symbol '" + name + "' already declared", line, column);
        }
        return false;
    }
    
    // Create and insert new symbol with serial number
    symbols[name] = std::make_shared<Symbol>(nextSerialNo++, name, line, column);
    return true;
}

// Lookup a symbol
std::shared_ptr<Symbol> SymbolTable::lookup(const std::string& name) const {
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        return it->second;
    }
    return nullptr;
}

// Check if symbol exists
bool SymbolTable::exists(const std::string& name) const {
    return symbols.find(name) != symbols.end();
}

// Print the symbol table (for debugging)
void SymbolTable::print() const {
    std::cout << "Symbol Table:" << std::endl;
    std::cout << "------------" << std::endl;
    
    for (const auto& pair : symbols) {
        const auto& symbol = pair.second;
        std::cout << "Serial No: " << symbol->serialNo
                  << ", Name: " << symbol->name 
                  << ", Line: " << symbol->line
                  << ", Column: " << symbol->column
                  << std::endl;
    }
}

// Write symbol table to file
void SymbolTable::writeToFile(const std::string& filename) const {
    // Create output directory if it doesn't exist
    std::filesystem::create_directories("output");
    
    std::ofstream file("output/" + filename);
    if (!file.is_open()) {
        if (errorHandler) {
            errorHandler->semanticError("Could not open file for writing symbol table", 0, 0);
        }
        return;
    }
    
    // Write CSV header
    file << "Serial No,Name,Line,Column\n";
    
    // Write symbol entries
    for (const auto& pair : symbols) {
        const auto& symbol = pair.second;
        file << symbol->serialNo << ","
             << symbol->name << ","
             << symbol->line << ","
             << symbol->column << "\n";
    }
}

// Get all symbols
std::vector<std::shared_ptr<Symbol>> SymbolTable::getAllSymbols() const {
    std::vector<std::shared_ptr<Symbol>> result;
    for (const auto& pair : symbols) {
        result.push_back(pair.second);
    }
    return result;
}

// Clear the symbol table
void SymbolTable::clear() {
    symbols.clear();
    nextSerialNo = 1;
} 
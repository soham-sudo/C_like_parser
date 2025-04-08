#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

// Forward declaration
class ErrorHandler;

// Symbol entry in the symbol table
struct Symbol {
    int serialNo;        // Serial number for the symbol
    std::string name;    // Symbol name
    int line;           // Line number in source
    int column;         // Column number in source
    
    // Constructor
    Symbol(int sn, const std::string& n, int l, int c)
        : serialNo(sn), name(n), line(l), column(c) {}
};

// Symbol Table class
class SymbolTable {
private:
    // Main hash table using identifier names as keys
    std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols;
    
    // Error handler reference
    std::shared_ptr<ErrorHandler> errorHandler;
    
    // Serial number counter
    int nextSerialNo;
    
public:
    // Constructor
    SymbolTable(std::shared_ptr<ErrorHandler> errHandler = nullptr);
    
    // Set error handler
    void setErrorHandler(std::shared_ptr<ErrorHandler> errHandler);
    
    // Insert a symbol
    bool insert(const std::string& name, int line, int column);
    
    // Lookup a symbol
    std::shared_ptr<Symbol> lookup(const std::string& name) const;
    
    // Check if symbol exists
    bool exists(const std::string& name) const;
    
    // Print the symbol table (for debugging)
    void print() const;
    
    // Write symbol table to file
    void writeToFile(const std::string& filename) const;
    
    // Get all symbols
    std::vector<std::shared_ptr<Symbol>> getAllSymbols() const;
    
    // Clear the symbol table
    void clear();
};

#endif // SYMBOL_TABLE_H 
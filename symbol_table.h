#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <variant>
#include <stack>

// Forward declaration
class ErrorHandler;

// Data type enum
enum class DataType {
    INT,
    FLOAT,
    UNKNOWN
};

// Symbol entry in the symbol table
struct Symbol {
    std::string name;
    DataType type;
    std::variant<int, float, std::monostate> value;
    int line;
    int column;
    bool initialized;
    int scope;  // Added scope level
    
    // Constructor
    Symbol(const std::string& n, DataType t, int l, int c, int s)
        : name(n), type(t), value(std::monostate{}), line(l), column(c), initialized(false), scope(s) {}
};

// Hash function for scoped identifiers (name + scope)
struct ScopedIdentifierHash {
    std::size_t operator()(const std::pair<std::string, int>& p) const {
        return std::hash<std::string>()(p.first) ^ std::hash<int>()(p.second);
    }
};

// Symbol Table class
class SymbolTable {
private:
    // Main hash table using scoped identifiers (name, scope) as keys
    std::unordered_map<std::pair<std::string, int>, std::shared_ptr<Symbol>, ScopedIdentifierHash> symbols;
    
    // Scope management
    std::stack<int> scopeStack;
    int currentScopeLevel;
    
    // Error handler reference
    std::shared_ptr<ErrorHandler> errorHandler;
    
public:
    // Constructor
    SymbolTable(std::shared_ptr<ErrorHandler> errHandler = nullptr);
    
    // Set error handler
    void setErrorHandler(std::shared_ptr<ErrorHandler> errHandler);
    
    // Scope management
    void enterScope();
    void exitScope();
    int getCurrentScope() const;
    
    // Insert a symbol in current scope
    bool insert(const std::string& name, DataType type, int line, int column);
    
    // Insert a symbol in specific scope
    bool insertInScope(const std::string& name, DataType type, int line, int column, int scope);
    
    // Lookup a symbol (search in current and all parent scopes)
    std::shared_ptr<Symbol> lookup(const std::string& name) const;
    
    // Lookup a symbol in specific scope
    std::shared_ptr<Symbol> lookupInScope(const std::string& name, int scope) const;
    
    // Update symbol type
    bool updateType(const std::string& name, DataType type);
    
    // Update symbol value
    bool updateIntValue(const std::string& name, int value);
    bool updateFloatValue(const std::string& name, float value);
    
    // Check if symbol exists in current or parent scopes
    bool exists(const std::string& name) const;
    
    // Check if symbol exists in specific scope
    bool existsInScope(const std::string& name, int scope) const;
    
    // Get all symbols in a specific scope
    std::vector<std::shared_ptr<Symbol>> getSymbolsInScope(int scope) const;
    
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
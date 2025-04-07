#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>

// Error types
enum class ErrorType {
    LEXICAL_ERROR,
    SYNTAX_ERROR,
    SEMANTIC_ERROR,
    WARNING
};

// Error structure
struct Error {
    ErrorType type;
    std::string message;
    int line;
    int column;
    
    Error(ErrorType t, const std::string& msg, int ln, int col)
        : type(t), message(msg), line(ln), column(col) {}
};

// Error Handler class
class ErrorHandler {
private:
    std::vector<Error> errors;
    std::string errorLog;
    bool hasErrors;
    bool outputToConsole;
    
    // Helper methods for error reporting
    void writeErrorsByType(std::ofstream& file, ErrorType type, const std::string& typeTitle);
    void writeToErrorFile(const std::string& errorMsg);
    
public:
    // Constructor
    ErrorHandler(bool consoleOutput = true);
    
    // Add an error to the error list
    void reportError(ErrorType type, const std::string& message, int line, int column);
    
    // Add a lexical error
    void lexicalError(const std::string& message, int line, int column);
    
    // Add a syntax error
    void syntaxError(const std::string& message, int line, int column);
    
    // Add a semantic error
    void semanticError(const std::string& message, int line, int column);
    
    // Add a warning
    void warning(const std::string& message, int line, int column);
    
    // Check if there are any errors
    bool hasCompileErrors() const;
    
    // Get all errors
    const std::vector<Error>& getErrors() const;
    
    // Write errors to a file
    void writeErrorsToFile(const std::string& filename);
    
    // Print errors to console
    void printErrors() const;
    
    // Clear all errors
    void clear();
};

#endif // ERROR_HANDLER_H 
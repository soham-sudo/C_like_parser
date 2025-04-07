#include "error_handler.h"

// Constructor
ErrorHandler::ErrorHandler(bool consoleOutput) 
    : hasErrors(false), outputToConsole(consoleOutput) {
    // Create output directory if it doesn't exist
    #ifdef _WIN32
    std::system("if not exist output mkdir output");
    #else
    std::system("mkdir -p output");
    #endif
    
    // Clear error files
    std::ofstream clearFile("output/error.txt", std::ios::trunc);
    clearFile.close();
    
    clearFile.open("output/errors.txt", std::ios::trunc);
    clearFile.close();
}

// Report an error
void ErrorHandler::reportError(ErrorType type, const std::string& message, int line, int column) {
    Error error(type, message, line, column);
    errors.push_back(error);
    
    // Create error message
    std::string typeStr;
    switch (type) {
        case ErrorType::LEXICAL_ERROR: typeStr = "Lexical Error"; break;
        case ErrorType::SYNTAX_ERROR: typeStr = "Syntax Error"; break;
        case ErrorType::SEMANTIC_ERROR: typeStr = "Semantic Error"; break;
        case ErrorType::WARNING: typeStr = "Warning"; break;
    }
    
    std::string errorMsg = typeStr + " at line " + std::to_string(line) + 
                          ", column " + std::to_string(column) + ": " + message;
    
    // Add to error log
    errorLog += errorMsg + "\n";
    
    // Output to console if enabled
    if (outputToConsole) {
        std::cerr << errorMsg << std::endl;
    }
    
    // Always write to error.txt file in real-time
    writeToErrorFile(errorMsg);
    
    // Set error flag (if not a warning)
    if (type != ErrorType::WARNING) {
        hasErrors = true;
    }
}

// Report a lexical error
void ErrorHandler::lexicalError(const std::string& message, int line, int column) {
    reportError(ErrorType::LEXICAL_ERROR, message, line, column);
}

// Report a syntax error
void ErrorHandler::syntaxError(const std::string& message, int line, int column) {
    reportError(ErrorType::SYNTAX_ERROR, message, line, column);
}

// Report a semantic error
void ErrorHandler::semanticError(const std::string& message, int line, int column) {
    reportError(ErrorType::SEMANTIC_ERROR, message, line, column);
}

// Report a warning
void ErrorHandler::warning(const std::string& message, int line, int column) {
    reportError(ErrorType::WARNING, message, line, column);
}

// Check if there are any errors
bool ErrorHandler::hasCompileErrors() const {
    return hasErrors;
}

// Get all errors
const std::vector<Error>& ErrorHandler::getErrors() const {
    return errors;
}

// Write errors to a file
void ErrorHandler::writeErrorsToFile(const std::string& filename) {
    // Create output directory if needed
    #ifdef _WIN32
    std::system("if not exist output mkdir output");
    #else
    std::system("mkdir -p output");
    #endif
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing errors." << std::endl;
        return;
    }
    
    file << "=== Compilation Errors and Warnings ===\n";
    file << "Total errors: " << std::count_if(errors.begin(), errors.end(), 
            [](const Error& e) { return e.type != ErrorType::WARNING; }) << "\n";
    file << "Total warnings: " << std::count_if(errors.begin(), errors.end(), 
            [](const Error& e) { return e.type == ErrorType::WARNING; }) << "\n\n";
    
    // Group errors by type
    file << "=== Errors by Type ===\n";
    writeErrorsByType(file, ErrorType::LEXICAL_ERROR, "Lexical Errors");
    writeErrorsByType(file, ErrorType::SYNTAX_ERROR, "Syntax Errors");
    writeErrorsByType(file, ErrorType::SEMANTIC_ERROR, "Semantic Errors");
    writeErrorsByType(file, ErrorType::WARNING, "Warnings");
    
    // Write all errors in order of occurrence
    file << "\n=== All Errors (in order of occurrence) ===\n";
    file << errorLog;
    
    file.close();
}

// Write errors of a specific type to a file stream
void ErrorHandler::writeErrorsByType(std::ofstream& file, ErrorType type, const std::string& typeTitle) {
    file << "\n" << typeTitle << ":\n";
    bool found = false;
    
    for (const auto& error : errors) {
        if (error.type == type) {
            file << "  Line " << error.line << ", Column " << error.column 
                 << ": " << error.message << "\n";
            found = true;
        }
    }
    
    if (!found) {
        file << "  None\n";
    }
}

// Write a single error message to the error file
void ErrorHandler::writeToErrorFile(const std::string& errorMsg) {
    std::ofstream file("output/error.txt", std::ios::app);
    if (file.is_open()) {
        file << errorMsg << std::endl;
        file.close();
    }
}

// Print errors to console
void ErrorHandler::printErrors() const {
    std::cout << "=== Compilation Errors and Warnings ===" << std::endl;
    if (errors.empty()) {
        std::cout << "No errors or warnings." << std::endl;
    } else {
        for (const auto& error : errors) {
            std::string typeStr;
            switch (error.type) {
                case ErrorType::LEXICAL_ERROR: typeStr = "Lexical Error"; break;
                case ErrorType::SYNTAX_ERROR: typeStr = "Syntax Error"; break;
                case ErrorType::SEMANTIC_ERROR: typeStr = "Semantic Error"; break;
                case ErrorType::WARNING: typeStr = "Warning"; break;
            }
            
            std::cout << typeStr << " at line " << error.line << ", column " << error.column 
                     << ": " << error.message << std::endl;
        }
    }
    std::cout << "=====================================" << std::endl;
}

// Clear all errors
void ErrorHandler::clear() {
    errors.clear();
    errorLog.clear();
    hasErrors = false;
    
    // Clear error.txt file
    std::ofstream file("output/error.txt", std::ios::trunc);
    file.close();
} 
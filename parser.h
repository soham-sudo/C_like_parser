#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "symbol_table.h"
#include "error_handler.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <stack>
#include <fstream>

// Forward declaration
class Grammar;

// LL(1) Parser class
class Parser {
private:
    std::shared_ptr<LexicalAnalyzer> lexer;
    std::shared_ptr<SymbolTable> symbolTable;
    std::shared_ptr<ErrorHandler> errorHandler;
    std::shared_ptr<Grammar> grammar;
    
    // Parse table: [non-terminal][terminal] -> production index
    std::map<std::string, std::map<std::string, int>> parseTable;
    
    // Stack for parsing
    std::stack<std::string> parseStack;
    
    // Current token
    Token currentToken;
    
    // Output file streams
    std::ofstream firstFollowFile;
    std::ofstream parseTableFile;
    std::ofstream parsingStagesFile;
    
    // State tracking
    bool isInDeclaration;  // Track if we're currently processing a declaration
    
    // Helper methods
    void initParseTable();
    void addToParseTable(const std::string& nonTerminal, const std::string& terminal, int productionIndex);
    bool isNonTerminal(const std::string& symbol);
    void panic();  // Error recovery
    void handleIdentifier(const Token& token);  // Handle identifier tokens based on context
    
    // Function to write parsing stages to file
    void writeParsingStage(const std::string& stackContent, const std::string& input, 
                          const std::string& production, const std::string& action);
    
    // Convert stack to string
    std::string stackToString(std::stack<std::string> s);
    
    // Helper method to convert token to the string used in the parse table
    std::string tokenToString(const Token& token) const;

public:
    // Constructor
    Parser(std::shared_ptr<LexicalAnalyzer> lex, 
           std::shared_ptr<SymbolTable> symTab,
           std::shared_ptr<ErrorHandler> errHandler,
           std::shared_ptr<Grammar> gram);
    
    // Parse the input
    bool parse();
    
    // Generate first and follow sets
    void generateFirstAndFollowSets();
    
    // Generate parse table
    void generateParseTable();
    
    // Output the first and follow sets to a file
    void writeFirstAndFollowSetsToFile();
    
    // Output the parse table to a file
    void writeParseTableToFile();
    
    // Get the current token
    Token getCurrentToken() const;
    
    // Advance to the next token
    void advance();
};

#endif // PARSER_H 
#include "lexer.h"
#include "symbol_table.h"
#include "error_handler.h"
#include "grammar.h"
#include "parser.h"
#include <iostream>
#include <memory>
#include <string>
#include <fstream>

int main(int argc, char* argv[]) {
    // Create components with shared ownership
    auto errorHandler = std::make_shared<ErrorHandler>(true);
    
    // Clear error files if they exist - this is now handled in ErrorHandler constructor
    
    auto symbolTable = std::make_shared<SymbolTable>(errorHandler);
    auto lexer = std::make_shared<LexicalAnalyzer>(symbolTable, errorHandler);
    
    // Create and initialize grammar
    auto grammar = std::make_shared<Grammar>();
    grammar->initializeGrammar();
    
    // Create parser
    auto parser = std::make_shared<Parser>(lexer, symbolTable, errorHandler, grammar);
    
    // Process input file
    std::string inputFile;
    
    if (argc > 1) {
        // Use file provided as command line argument
        inputFile = argv[1];
    } else {
        // Use default sample file
        inputFile = "sample_correct.txt";
        std::cout << "No input file specified. Using default: " << inputFile << std::endl;
        std::cout << "Use " << argv[0] << " sample_error.txt to test error detection." << std::endl;
    }
    
    // Verify file exists
    std::ifstream checkFile(inputFile);
    if (!checkFile.is_open()) {
        std::cerr << "Error: Could not open file " << inputFile << std::endl;
        std::cerr << "Make sure that sample_correct.txt and sample_error.txt exist." << std::endl;
        return 1;
    }
    checkFile.close();
    
    // Step 1: Perform lexical analysis
    std::cout << "\nStep 1: Performing lexical analysis on " << inputFile << "..." << std::endl;
    lexer->tokenizeFile(inputFile);
    
    // Report lexical errors (if any)
    if (errorHandler->hasCompileErrors()) {
        std::cout << "  Lexical errors detected!" << std::endl;
        errorHandler->printErrors();
    } else {
        std::cout << "  Lexical analysis completed successfully." << std::endl;
    }
    
    // Step 2: Generate FIRST and FOLLOW sets
    std::cout << "\nStep 2: Generating FIRST and FOLLOW sets..." << std::endl;
    parser->generateFirstAndFollowSets();
    std::cout << "  FIRST and FOLLOW sets written to first_follow.txt" << std::endl;
    
    // Step 3: Generate parse table
    std::cout << "\nStep 3: Generating parse table..." << std::endl;
    parser->generateParseTable();
    std::cout << "  Parse table written to parse_table.txt" << std::endl;
    
    // Step 4: Perform parsing
    std::cout << "\nStep 4: Performing parsing..." << std::endl;
    bool parseSuccess = parser->parse();
    
    if (parseSuccess && !errorHandler->hasCompileErrors()) {
        std::cout << "  Parsing completed successfully." << std::endl;
    } else {
        std::cout << "  Parsing failed with errors." << std::endl;
        errorHandler->printErrors();
        errorHandler->writeErrorsToFile("output/errors.txt");
    }
    
    // Step 5: Write symbol table to file
    std::cout << "\nStep 5: Writing symbol table to file..." << std::endl;
    symbolTable->writeToFile("symbol_table.txt");
    std::cout << "  Symbol table written to output/symbol_table.txt" << std::endl;
    
    // Print output file locations
    std::cout << "\nOutput files generated in the 'output' directory:" << std::endl;
    std::cout << " - output/tokens.txt: Detailed token information" << std::endl;
    std::cout << " - output/token_stream.txt: Token stream for parser" << std::endl;
    std::cout << " - output/first_follow.txt: FIRST and FOLLOW sets" << std::endl;
    std::cout << " - output/parse_table.txt: LL(1) parsing table" << std::endl;
    std::cout << " - output/parsing_stages.txt: Step-by-step parsing process" << std::endl;
    std::cout << " - output/symbol_table.txt: Symbol table entries" << std::endl;
    
    if (errorHandler->hasCompileErrors()) {
        std::cout << " - output/error.txt: Real-time error logging" << std::endl;
        std::cout << " - output/errors.txt: Comprehensive error report" << std::endl;
    }
    
    return errorHandler->hasCompileErrors() ? 1 : 0;
} 
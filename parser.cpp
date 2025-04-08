#include "parser.h"
#include "grammar.h"
#include <iostream>
#include <algorithm>
#include <sstream>

// Constructor
Parser::Parser(std::shared_ptr<LexicalAnalyzer> lex, 
               std::shared_ptr<SymbolTable> symTab,
               std::shared_ptr<ErrorHandler> errHandler, 
               std::shared_ptr<Grammar> gram)
    : lexer(lex), symbolTable(symTab), errorHandler(errHandler), grammar(gram),
      isInDeclaration(false) {
    
    // Create output directory if it doesn't exist
    #ifdef _WIN32
    std::system("if not exist output mkdir output");
    #else
    std::system("mkdir -p output");
    #endif
    
    // Open output files in the output directory
    firstFollowFile.open("output/first_follow.txt");
    parseTableFile.open("output/parse_table.txt");
    parsingStagesFile.open("output/parsing_stages.txt");
    
    // Check if files opened successfully
    if (!firstFollowFile.is_open()) {
        std::cerr << "Error: Could not open output/first_follow.txt for writing." << std::endl;
    }
    if (!parseTableFile.is_open()) {
        std::cerr << "Error: Could not open output/parse_table.txt for writing." << std::endl;
    }
    if (!parsingStagesFile.is_open()) {
        std::cerr << "Error: Could not open output/parsing_stages.txt for writing." << std::endl;
    }
    
    // Setup parsing stages file header
    parsingStagesFile << "| Stack Contents | Current Input | Production Used | Action |\n";
    parsingStagesFile << "|---------------|---------------|-----------------|--------|\n";
}

// Handle identifier tokens based on context
void Parser::handleIdentifier(const Token& token) {
    if (isInDeclaration) {
        // Only insert into symbol table if this is a declaration
        symbolTable->insert(token.lexeme, token.line, token.column);
    } else {
        // For references, check if the variable exists
        if (!symbolTable->exists(token.lexeme)) {
            if (errorHandler) {
                errorHandler->semanticError("Use of undeclared variable '" + token.lexeme + "'",
                                         token.line, token.column);
            }
        }
    }
}

// Parse the input
bool Parser::parse() {
    // Initialize stack with start symbol and EOF marker
    while (!parseStack.empty()) parseStack.pop();
    parseStack.push("$");  // End marker
    parseStack.push(grammar->getStartSymbol().name);
    
    // Get first token
    advance();
    
    // Debug output
    std::cout << "DEBUG: Start Symbol = " << grammar->getStartSymbol().name << std::endl;
    std::cout << "DEBUG: First Token = " << currentToken.getTypeAsString() << ", lexeme = '" << currentToken.lexeme << "'" << std::endl;
    
    // Begin parsing
    writeParsingStage(stackToString(parseStack), tokenToString(currentToken), "", "Initial stack setup");
    
    // Main parsing loop
    while (!parseStack.empty()) {
        std::string top = parseStack.top();
        parseStack.pop();
        
        // If end of stack and end of input, parsing successful
        if (top == "$" && currentToken.type == TokenType::END_OF_FILE) {
            writeParsingStage("$", "$", "", "Accepted");
            return true;
        }
        
        // For terminals, match exactly what's on the stack
        if (!isNonTerminal(top)) {
            std::string tokenStr = tokenToString(currentToken);
            
            // Track when we enter a declaration
            if (top == "int" || top == "float") {
                isInDeclaration = true;
            }
            // Track when we exit a declaration
            else if (top == ";") {
                isInDeclaration = false;
            }
            
            if (top == tokenStr) {
                // If this is an identifier token, handle it appropriately
                if (currentToken.type == TokenType::IDENTIFIER) {
                    handleIdentifier(currentToken);
                }
                
                writeParsingStage(stackToString(parseStack), tokenStr, "", "Match: " + top);
                advance();
                continue;
            } else {
                // Terminal mismatch
                std::string errorMsg = "Syntax error: expected '" + top + "', found '" + 
                                     currentToken.lexeme + "'";
                if (errorHandler) {
                    errorHandler->syntaxError(errorMsg, currentToken.line, currentToken.column);
                }
                
                writeParsingStage(stackToString(parseStack), tokenStr, "", 
                                "ERROR: Terminal mismatch");
                panic();  // Error recovery
                return false;
            }
        }
        
        // If non-terminal, look up in parse table
        std::string terminal = tokenToString(currentToken);
        std::cout << "DEBUG: Looking up [" << top << ", " << terminal << "] in parse table" << std::endl;
        
        // Look up production in parse table
        auto ntIt = parseTable.find(top);
        if (ntIt != parseTable.end() && ntIt->second.find(terminal) != ntIt->second.end()) {
            int prodIndex = ntIt->second[terminal];
            const auto& production = grammar->getProductions()[prodIndex];
            
            std::cout << "DEBUG: Found production #" << prodIndex << " in parse table" << std::endl;
            
            // Build production string for logging
            std::string prodString = top + " → ";
            for (const auto& symbol : production.rightSide) {
                prodString += symbol.name + " ";
            }
            
            // Push production RHS onto stack in reverse order
            for (int i = production.rightSide.size() - 1; i >= 0; --i) {
                if (production.rightSide[i].name != "ε") {  // Don't push epsilon
                    parseStack.push(production.rightSide[i].name);
                }
            }
            
            writeParsingStage(stackToString(parseStack), terminal, prodString, "Expand non-terminal");
        } else {
            // Syntax error - no matching production
            std::string errorMsg = "Syntax error: unexpected token '" + currentToken.lexeme + 
                                 "' for non-terminal '" + top + "'";
            if (errorHandler) {
                errorHandler->syntaxError(errorMsg, currentToken.line, currentToken.column);
            }
            
            writeParsingStage(stackToString(parseStack), tokenToString(currentToken), "", 
                            "ERROR: No matching production");
            panic();  // Error recovery
            return false;
        }
    }
    
    // If we reach here, there's an error
    if (currentToken.type != TokenType::END_OF_FILE) {
        std::string errorMsg = "Syntax error: unexpected token '" + currentToken.lexeme + 
                             "' after end of input";
        if (errorHandler) {
            errorHandler->syntaxError(errorMsg, currentToken.line, currentToken.column);
        }
        writeParsingStage("", tokenToString(currentToken), "", "ERROR: Unexpected token after input");
        return false;
    }
    
    return true;
}

// Generate first and follow sets
void Parser::generateFirstAndFollowSets() {
    grammar->computeFirstSets();
    grammar->computeFollowSets();
    writeFirstAndFollowSetsToFile();
}

// Generate parse table
void Parser::generateParseTable() {
    initParseTable();
    
    // For each production
    const auto& productions = grammar->getProductions();
    for (size_t i = 0; i < productions.size(); ++i) {
        const auto& prod = productions[i];
        const auto& lhs = prod.leftSide;
        const auto& rhs = prod.rightSide;
        
        // Get FIRST of the right side
        std::set<GrammarSymbol> firstOfRHS = grammar->getFirstSetOfSequence(rhs);
        
        // For each terminal in FIRST(RHS), add production to table
        for (const auto& terminal : firstOfRHS) {
            if (terminal.name == "ε") {
                // For epsilon, use FOLLOW(LHS)
                const auto& followLHS = grammar->getFollowSet(lhs);
                for (const auto& followTerm : followLHS) {
                    addToParseTable(lhs.name, followTerm.name, i);
                }
            } else {
                // For terminal, add to table
                addToParseTable(lhs.name, terminal.name, i);
            }
        }
    }
    
    writeParseTableToFile();
}

// Initialize parse table
void Parser::initParseTable() {
    parseTable.clear();
    
    // Initialize with empty entries
    for (const auto& nt : grammar->getNonTerminals()) {
        std::map<std::string, int> row;
        parseTable[nt.name] = row;
    }
}

// Add entry to parse table
void Parser::addToParseTable(const std::string& nonTerminal, const std::string& terminal, int productionIndex) {
    auto& row = parseTable[nonTerminal];
    
    // Check for conflicts (overwriting existing entry)
    if (row.find(terminal) != row.end()) {
        // Parse table conflict - not LL(1)
        std::string errorMsg = "Parse table conflict for [" + nonTerminal + ", " + terminal + "]";
        std::cerr << "Warning: " << errorMsg << std::endl;
        
        if (errorHandler) {
            errorHandler->syntaxError(errorMsg, 0, 0);
        }
    }
    
    // Add entry to table
    row[terminal] = productionIndex;
}

// Check if a symbol is a non-terminal
bool Parser::isNonTerminal(const std::string& symbol) {
    for (const auto& nt : grammar->getNonTerminals()) {
        if (nt.name == symbol) {
            return true;
        }
    }
    return false;
}

// Panic mode error recovery
void Parser::panic() {
    // Simple implementation: skip tokens until a semicolon is found
    writeParsingStage(stackToString(parseStack), currentToken.getTypeAsString(), "", "Panic mode: skip until ';'");
    
    while (currentToken.type != TokenType::END_OF_FILE && 
           currentToken.type != TokenType::SEMICOLON) {
        advance();
    }
    
    // Skip the semicolon
    if (currentToken.type == TokenType::SEMICOLON) {
        advance();
    }
    
    // Clear stack and restart from a safe point
    while (!parseStack.empty()) parseStack.pop();
    parseStack.push("$");
    
    // Find a suitable recovery symbol based on grammar
    const auto& nonTerminals = grammar->getNonTerminals();
    for (const auto& nt : nonTerminals) {
        if (nt.name == "stmt" || nt.name == "stmts" || 
            nt.name == "decl" || nt.name == "expr_stmt") {
            parseStack.push(nt.name);
            break;
        }
    }
    
    writeParsingStage(stackToString(parseStack), currentToken.getTypeAsString(), "", "Resumed parsing");
}

// Convert stack to string
std::string Parser::stackToString(std::stack<std::string> s) {
    if (s.empty()) return "ε";
    
    std::vector<std::string> elements;
    while (!s.empty()) {
        elements.push_back(s.top());
        s.pop();
    }
    
    std::reverse(elements.begin(), elements.end());
    
    std::stringstream ss;
    for (const auto& elem : elements) {
        ss << elem << " ";
    }
    
    std::string result = ss.str();
    if (!result.empty()) {
        result.pop_back();  // Remove trailing space
    }
    
    return result;
}

// Write parsing stage to file
void Parser::writeParsingStage(const std::string& stackContent, const std::string& input,
                             const std::string& production, const std::string& action) {
    parsingStagesFile << "| " << stackContent << " | " << input << " | " 
                     << production << " | " << action << " |\n";
}

// Write first and follow sets to file
void Parser::writeFirstAndFollowSetsToFile() {
    if (!firstFollowFile.is_open()) return;
    
    firstFollowFile << "=== FIRST SETS ===\n";
    for (const auto& nt : grammar->getNonTerminals()) {
        firstFollowFile << "FIRST(" << nt.name << ") = { ";
        
        const auto& firstSet = grammar->getFirstSet(nt);
        bool first = true;
        for (const auto& terminal : firstSet) {
            if (!first) firstFollowFile << ", ";
            firstFollowFile << terminal.name;
            first = false;
        }
        
        firstFollowFile << " }\n";
    }
    
    firstFollowFile << "\n=== FOLLOW SETS ===\n";
    for (const auto& nt : grammar->getNonTerminals()) {
        firstFollowFile << "FOLLOW(" << nt.name << ") = { ";
        
        const auto& followSet = grammar->getFollowSet(nt);
        bool first = true;
        for (const auto& terminal : followSet) {
            if (!first) firstFollowFile << ", ";
            firstFollowFile << terminal.name;
            first = false;
        }
        
        firstFollowFile << " }\n";
    }
}

// Write parse table to file
void Parser::writeParseTableToFile() {
    if (!parseTableFile.is_open()) return;
    
    // Get all terminals
    std::set<std::string> terminals;
    for (const auto& term : grammar->getTerminals()) {
        if (term.name != "ε") {  // Skip epsilon
            terminals.insert(term.name);
        }
    }
    
    // Write header
    parseTableFile << "| Non-Terminal |";
    for (const auto& term : terminals) {
        parseTableFile << " " << term << " |";
    }
    parseTableFile << "\n|";
    
    // Write header separator
    parseTableFile << "--------------|";
    for (size_t i = 0; i < terminals.size(); ++i) {
        parseTableFile << "---------|";
    }
    parseTableFile << "\n";
    
    // Write table content
    for (const auto& ntEntry : parseTable) {
        parseTableFile << "| " << ntEntry.first << " |";
        
        for (const auto& term : terminals) {
            parseTableFile << " ";
            auto it = ntEntry.second.find(term);
            if (it != ntEntry.second.end()) {
                int prodIndex = it->second;
                
                // Write production number
                parseTableFile << prodIndex << " ";
            } else {
                parseTableFile << "  ";
            }
            parseTableFile << " |";
        }
        parseTableFile << "\n";
    }
}

// Get current token
Token Parser::getCurrentToken() const {
    return currentToken;
}

// Advance to next token
void Parser::advance() {
    currentToken = lexer->getNextToken();
}

// Helper method to convert token to the string used in the parse table
std::string Parser::tokenToString(const Token& token) const {
    // Keywords are represented by their lexeme
    if (token.lexeme == "int" || token.lexeme == "float" || 
        token.lexeme == "main" || token.lexeme == "while" ||
        token.lexeme == ";" || token.lexeme == "," ||
        token.lexeme == "(" || token.lexeme == ")" ||
        token.lexeme == "{" || token.lexeme == "}" ||
        token.lexeme == "=" || token.lexeme == "<" ||
        token.lexeme == ">" || token.lexeme == "++" ||
        token.lexeme == "--" || token.lexeme == "+" ||
        token.lexeme == "-" || token.lexeme == "*" ||
        token.lexeme == "/") {
        return token.lexeme;
    }
    
    // Convert IDENTIFIER to ID to match grammar
    if (token.type == TokenType::IDENTIFIER) {
        return "ID";
    }
    
    // Convert literals to CONST to match grammar
    if (token.type == TokenType::INTEGER_LITERAL || 
        token.type == TokenType::FLOAT_LITERAL) {
        return "CONST";
    }
    
    // Other tokens are represented by their type
    return token.getTypeAsString();
} 
#include "lexer.h"
#include "symbol_table.h"
#include "error_handler.h"
#include <iostream>
#include <cctype>
#include <regex>
#include <fstream>
#include <sstream>

// Static initialization
std::map<std::string, TokenType> LexicalAnalyzer::keywords;

// Token to string conversion for debugging
std::string Token::toString() const {
    std::string typeStr;
    
    switch (type) {
        case TokenType::INT: typeStr = "INT"; break;
        case TokenType::FLOAT: typeStr = "FLOAT"; break;
        case TokenType::WHILE: typeStr = "WHILE"; break;
        case TokenType::MAIN: typeStr = "MAIN"; break;
        case TokenType::PLUS: typeStr = "PLUS"; break;
        case TokenType::MINUS: typeStr = "MINUS"; break;
        case TokenType::MULTIPLY: typeStr = "MULTIPLY"; break;
        case TokenType::DIVIDE: typeStr = "DIVIDE"; break;
        case TokenType::INCREMENT: typeStr = "INCREMENT"; break;
        case TokenType::DECREMENT: typeStr = "DECREMENT"; break;
        case TokenType::ASSIGN: typeStr = "ASSIGN"; break;
        case TokenType::LESS_THAN: typeStr = "LESS_THAN"; break;
        case TokenType::GREATER_THAN: typeStr = "GREATER_THAN"; break;
        case TokenType::SEMICOLON: typeStr = "SEMICOLON"; break;
        case TokenType::COMMA: typeStr = "COMMA"; break;
        case TokenType::LEFT_PAREN: typeStr = "LEFT_PAREN"; break;
        case TokenType::RIGHT_PAREN: typeStr = "RIGHT_PAREN"; break;
        case TokenType::LEFT_BRACE: typeStr = "LEFT_BRACE"; break;
        case TokenType::RIGHT_BRACE: typeStr = "RIGHT_BRACE"; break;
        case TokenType::IDENTIFIER: typeStr = "IDENTIFIER"; break;
        case TokenType::INTEGER_LITERAL: typeStr = "INTEGER_LITERAL"; break;
        case TokenType::FLOAT_LITERAL: typeStr = "FLOAT_LITERAL"; break;
        case TokenType::END_OF_FILE: typeStr = "END_OF_FILE"; break;
        case TokenType::ERROR: typeStr = "ERROR"; break;
    }
    
    return "Token(" + typeStr + ", '" + lexeme + "', line=" + 
           std::to_string(line) + ", col=" + std::to_string(column) + ")";
}

// Get just the token type as string
std::string Token::getTypeAsString() const {
    switch (type) {
        case TokenType::INT: return "INT";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::WHILE: return "WHILE";
        case TokenType::MAIN: return "MAIN";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::INCREMENT: return "INCREMENT";
        case TokenType::DECREMENT: return "DECREMENT";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::LESS_THAN: return "LESS_THAN";
        case TokenType::GREATER_THAN: return "GREATER_THAN";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INTEGER_LITERAL: return "INTEGER_LITERAL";
        case TokenType::FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        case TokenType::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// Initialize keywords map
void LexicalAnalyzer::initKeywords() {
    if (keywords.empty()) {
        keywords["int"] = TokenType::INT;
        keywords["float"] = TokenType::FLOAT;
        keywords["while"] = TokenType::WHILE;
        keywords["main"] = TokenType::MAIN;
    }
}

// Constructor
LexicalAnalyzer::LexicalAnalyzer(std::shared_ptr<SymbolTable> symTable, 
                                std::shared_ptr<ErrorHandler> errHandler)
    : position(0), line(1), column(1), 
      symbolTable(symTable), errorHandler(errHandler) {
    initKeywords();
    initPatterns();
}

// Set symbol table
void LexicalAnalyzer::setSymbolTable(std::shared_ptr<SymbolTable> symTable) {
    symbolTable = symTable;
}

// Set error handler
void LexicalAnalyzer::setErrorHandler(std::shared_ptr<ErrorHandler> errHandler) {
    errorHandler = errHandler;
}

// Initialize regex patterns
void LexicalAnalyzer::initPatterns() {
    // Keywords
    patterns.emplace_back("\\bint\\b", TokenType::INT, true);
    patterns.emplace_back("\\bfloat\\b", TokenType::FLOAT, true);
    patterns.emplace_back("\\bwhile\\b", TokenType::WHILE, true);
    patterns.emplace_back("\\bmain\\b", TokenType::MAIN, true);
    
    // Operators
    patterns.emplace_back("\\+\\+", TokenType::INCREMENT);
    patterns.emplace_back("--", TokenType::DECREMENT);
    patterns.emplace_back("\\+", TokenType::PLUS);
    patterns.emplace_back("-", TokenType::MINUS);
    patterns.emplace_back("\\*", TokenType::MULTIPLY);
    patterns.emplace_back("/", TokenType::DIVIDE);
    patterns.emplace_back("=", TokenType::ASSIGN);
    patterns.emplace_back("<", TokenType::LESS_THAN);
    patterns.emplace_back(">", TokenType::GREATER_THAN);
    
    // Punctuation
    patterns.emplace_back(";", TokenType::SEMICOLON);
    patterns.emplace_back(",", TokenType::COMMA);
    patterns.emplace_back("\\(", TokenType::LEFT_PAREN);
    patterns.emplace_back("\\)", TokenType::RIGHT_PAREN);
    patterns.emplace_back("\\{", TokenType::LEFT_BRACE);
    patterns.emplace_back("\\}", TokenType::RIGHT_BRACE);
    
    // Literals
    patterns.emplace_back("\\b[0-9]+\\.[0-9]+\\b", TokenType::FLOAT_LITERAL);
    patterns.emplace_back("\\b[0-9]+\\b", TokenType::INTEGER_LITERAL);
    
    // Identifiers (must come after keywords)
    patterns.emplace_back("[a-zA-Z_][a-zA-Z0-9_]*", TokenType::IDENTIFIER);
}

// Tokenize a file
void LexicalAnalyzer::tokenizeFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        if (errorHandler) {
            errorHandler->lexicalError("Could not open file " + filename, 0, 0);
        } else {
            std::cerr << "Error: Could not open file " << filename << std::endl;
        }
        return;
    }
    
    // Read the entire file into a string
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    tokenizeString(content);
    
    // Create output directory if it doesn't exist
    #ifdef _WIN32
    std::system("if not exist output mkdir output");
    #else
    std::system("mkdir -p output");
    #endif
    
    // Generate output files
    writeTokensToFile("output/tokens.txt");
    writeTokenStreamToFile("output/token_stream.txt");
}

// Tokenize a string
void LexicalAnalyzer::tokenizeString(const std::string& input) {
    inputBuffer = input;
    position = 0;
    line = 1;
    column = 1;
    tokenStream.clear();
    
    while (position < inputBuffer.length()) {
        skipWhitespaceAndComments();
        if (position >= inputBuffer.length()) break;
        
        Token token = findNextToken();
        tokenStream.push_back(token);
        
        // If error token, report it
        if (token.type == TokenType::ERROR && errorHandler) {
            errorHandler->lexicalError("Invalid token: '" + token.lexeme + "'", token.line, token.column);
        }
    }
    
    // Add EOF token
    tokenStream.push_back(Token(TokenType::END_OF_FILE, "", line, column));
}

// Skip whitespace and comments
void LexicalAnalyzer::skipWhitespaceAndComments() {
    static const std::regex whitespaceRegex("^[ \t\r\n]+");
    static const std::regex singleLineCommentRegex("^//[^\n]*");
    static const std::regex multiLineCommentRegex("^/\\*.*?\\*/");
    
    bool continueSkipping = true;
    while (position < inputBuffer.length() && continueSkipping) {
        continueSkipping = false;
        std::string remaining = inputBuffer.substr(position);
        
        // Try to match whitespace
        std::smatch match;
        if (std::regex_search(remaining, match, whitespaceRegex, std::regex_constants::match_continuous)) {
            std::string matched = match.str();
            // Count newlines for line tracking
            for (char c : matched) {
                if (c == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
            }
            position += matched.length();
            continueSkipping = true;
        }
        
        // Try to match single-line comment
        else if (std::regex_search(remaining, match, singleLineCommentRegex, std::regex_constants::match_continuous)) {
            std::string matched = match.str();
            position += matched.length();
            continueSkipping = true;
            // No need to update line since we'll hit the newline in whitespace matching
        }
        
        // Try to match multi-line comment
        else if (std::regex_search(remaining, match, multiLineCommentRegex, std::regex_constants::match_continuous)) {
            std::string matched = match.str();
            // Count newlines in the comment
            for (char c : matched) {
                if (c == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
            }
            position += matched.length();
            continueSkipping = true;
        }
    }
}

// Find the next token
Token LexicalAnalyzer::findNextToken() {
    auto [matched, token] = matchTokenAtPosition();
    
    if (matched) {
        // If token is an identifier and symbol table exists, add to symbol table
        if (token.type == TokenType::IDENTIFIER && symbolTable) {
            // Check if this is a declaration (handled by parser, not here)
            // For now, just note that we saw this identifier
            if (!symbolTable->exists(token.lexeme)) {
                // For variables that aren't declared yet, we'll let the parser handle them
                // This is just for reference tracking
            }
        }
        return token;
    } else {
        // No match found, return error token and advance one character
        std::string errorChar = inputBuffer.substr(position, 1);
        std::string errorMsg = "Invalid character: '" + errorChar + "'";
        
        // Check for common lexical errors
        if (errorChar == "\n" || errorChar == "\r") {
            // Look back to check for missing semicolon
            size_t lastPos = position;
            while (lastPos > 0 && (inputBuffer[lastPos-1] == ' ' || inputBuffer[lastPos-1] == '\t')) {
                lastPos--;
            }
            
            if (lastPos > 0 && inputBuffer[lastPos-1] != ';' && inputBuffer[lastPos-1] != '{' && 
                inputBuffer[lastPos-1] != '}' && inputBuffer[lastPos-1] != '\n' && inputBuffer[lastPos-1] != '\r') {
                errorMsg = "Missing semicolon";
                if (errorHandler) {
                    errorHandler->lexicalError(errorMsg, line, column);
                }
            }
        }
        
        Token errorToken(TokenType::ERROR, errorChar, line, column);
        updatePosition(1);
        return errorToken;
    }
}

// Try to match a token at the current position
std::pair<bool, Token> LexicalAnalyzer::matchTokenAtPosition() {
    if (position >= inputBuffer.length()) {
        return {false, Token()};
    }
    
    std::string remaining = inputBuffer.substr(position);
    std::smatch match;
    
    // Try each pattern in order
    for (const auto& tokenPattern : patterns) {
        if (std::regex_search(remaining, match, tokenPattern.pattern, 
                             std::regex_constants::match_continuous)) {
            std::string matched = match.str();
            Token token(tokenPattern.type, matched, line, column);
            updatePosition(matched.length());
            return {true, token};
        }
    }
    
    return {false, Token()};
}

// Update position and line/column tracking
void LexicalAnalyzer::updatePosition(size_t length) {
    // Update column for all characters, and line for newlines
    for (size_t i = 0; i < length && position + i < inputBuffer.length(); ++i) {
        if (inputBuffer[position + i] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }
    position += length;
}

// Get the next token from the stream
Token LexicalAnalyzer::getNextToken() {
    static size_t currentPos = 0;
    
    if (currentPos < tokenStream.size()) {
        return tokenStream[currentPos++];
    } else {
        return Token(TokenType::END_OF_FILE, "", line, column);
    }
}

// Peek at a future token
Token LexicalAnalyzer::peekToken(int ahead) const {
    static size_t currentPos = 0;
    
    size_t index = currentPos + ahead - 1;
    if (index < tokenStream.size()) {
        return tokenStream[index];
    } else {
        return Token(TokenType::END_OF_FILE, "", line, column);
    }
}

// Get the token stream
const std::vector<Token>& LexicalAnalyzer::getTokenStream() const {
    return tokenStream;
}

// Print the token stream
void LexicalAnalyzer::printTokenStream() const {
    std::cout << "Token Stream:" << std::endl;
    for (const auto& token : tokenStream) {
        std::cout << token.toString() << std::endl;
    }
}

// Write tokens to a file (detailed version)
void LexicalAnalyzer::writeTokensToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing tokens." << std::endl;
        return;
    }
    
    file << "Token Type,Lexeme,Line,Column" << std::endl;
    
    for (const auto& token : tokenStream) {
        file << token.getTypeAsString() << ","
             << "\"" << token.lexeme << "\","
             << token.line << ","
             << token.column << std::endl;
    }
    
    file.close();
}

// Write token stream to a file (simplified version for parsing)
void LexicalAnalyzer::writeTokenStreamToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing token stream." << std::endl;
        return;
    }
    
    for (const auto& token : tokenStream) {
        file << token.getTypeAsString();
        if (token.type == TokenType::IDENTIFIER || 
            token.type == TokenType::INTEGER_LITERAL || 
            token.type == TokenType::FLOAT_LITERAL) {
            file << " (" << token.lexeme << ")";
        }
        file << std::endl;
    }
    
    file.close();
}

// Helper methods
char LexicalAnalyzer::peek() const {
    if (isAtEnd()) return '\0';
    return inputBuffer[position];
}

char LexicalAnalyzer::advance() {
    char c = peek();
    position++;
    column++;
    return c;
}

bool LexicalAnalyzer::isAtEnd() const {
    return position >= inputBuffer.length();
}

bool LexicalAnalyzer::match(char expected) {
    if (isAtEnd() || peek() != expected) return false;
    advance();
    return true;
}

void LexicalAnalyzer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
        } else if (c == '\n') {
            advance();
            line++;
            column = 1;
        } else if (c == '/') {
            // Check for comments
            if (position + 1 < inputBuffer.length() && inputBuffer[position + 1] == '/') {
                // Single-line comment
                position += 2;
                column += 2;
                
                // Consume until end of line
                while (!isAtEnd() && peek() != '\n') {
                    advance();
                }
            } else if (position + 1 < inputBuffer.length() && inputBuffer[position + 1] == '*') {
                // Multi-line comment
                position += 2;
                column += 2;
                
                bool endFound = false;
                while (!isAtEnd() && !endFound) {
                    if (peek() == '*' && position + 1 < inputBuffer.length() &&
                        inputBuffer[position + 1] == '/') {
                        // End of multi-line comment found
                        position += 2;
                        column += 2;
                        endFound = true;
                    } else if (peek() == '\n') {
                        advance();
                        line++;
                        column = 1;
                    } else {
                        advance();
                    }
                }
            } else {
                // Not a comment, just a divide operator
                break;
            }
        } else {
            // Not whitespace or comment
            break;
        }
    }
}
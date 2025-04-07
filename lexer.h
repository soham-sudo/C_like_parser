#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#include <regex>
#include <memory>

// Forward declarations
class SymbolTable;
class ErrorHandler;

// Token types
enum class TokenType {
    // Keywords
    INT, FLOAT, WHILE, MAIN,
    
    // Operators
    PLUS, MINUS, MULTIPLY, DIVIDE,
    INCREMENT, DECREMENT,
    ASSIGN, LESS_THAN, GREATER_THAN,
    
    // Punctuation
    SEMICOLON, COMMA, LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    
    // Identifiers and literals
    IDENTIFIER, INTEGER_LITERAL, FLOAT_LITERAL,
    
    // Special tokens
    END_OF_FILE, ERROR
};

// Token structure
struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    
    // Constructor
    Token(TokenType t, const std::string& l, int ln, int col)
        : type(t), lexeme(l), line(ln), column(col) {}
    
    // Default constructor
    Token() : type(TokenType::ERROR), lexeme(""), line(0), column(0) {}
    
    // For debugging
    std::string toString() const;
    
    // Get token type as string
    std::string getTypeAsString() const;
};

// Pattern structure for regex-based lexical analysis
struct TokenPattern {
    std::regex pattern;
    TokenType type;
    bool isKeyword;
    
    TokenPattern(const std::string& regexStr, TokenType t, bool isKw = false)
        : pattern(regexStr), type(t), isKeyword(isKw) {}
};

// Lexical Analyzer
class LexicalAnalyzer {
private:
    std::string inputBuffer;
    size_t position;
    int line;
    int column;
    std::vector<Token> tokenStream;
    
    // Keywords map
    static std::map<std::string, TokenType> keywords;
    
    // Reference to symbol table and error handler
    std::shared_ptr<SymbolTable> symbolTable;
    std::shared_ptr<ErrorHandler> errorHandler;
    
    // Token patterns
    std::vector<TokenPattern> patterns;
    
    // Initialize token patterns
    void initPatterns();
    
    // Initialize keywords
    void initKeywords();
    
    // Helper methods
    char peek() const;
    char advance();
    bool isAtEnd() const;
    bool match(char expected);
    void skipWhitespace();
    
    // Skip whitespace and comments
    void skipWhitespaceAndComments();
    
    // Find the next token match
    Token findNextToken();
    
    // Check if a token is at the current position
    std::pair<bool, Token> matchTokenAtPosition();
    
    // Update position and line/column tracking
    void updatePosition(size_t length);
    
public:
    // Constructor
    LexicalAnalyzer(std::shared_ptr<SymbolTable> symTable = nullptr, 
                   std::shared_ptr<ErrorHandler> errHandler = nullptr);
    
    // Set communication channels
    void setSymbolTable(std::shared_ptr<SymbolTable> symTable);
    void setErrorHandler(std::shared_ptr<ErrorHandler> errHandler);
    
    // Tokenize a file
    void tokenizeFile(const std::string& filename);
    
    // Tokenize a string
    void tokenizeString(const std::string& input);
    
    // Access the token stream
    const std::vector<Token>& getTokenStream() const;
    
    // Get the next token
    Token getNextToken();
    
    // Peek at a token without consuming it
    Token peekToken(int ahead = 1) const;
    
    // Print the token stream
    void printTokenStream() const;
    
    // Write tokens to files
    void writeTokensToFile(const std::string& filename) const;
    void writeTokenStreamToFile(const std::string& filename) const;
};

#endif // LEXER_H
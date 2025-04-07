# Compiler Features

This document provides a detailed overview of all the features implemented in this compiler for a simplified C-like language.

## 1. Lexical Analysis

### Token Recognition
- **Regex-Based Scanning**: Uses C++ regex for efficient pattern matching
- **Complete Token Classification**: Recognizes all language constructs (keywords, operators, identifiers, literals)
- **Source Position Tracking**: Records line and column numbers for error reporting
- **Comment Handling**: Processes both single-line (`//`) and multi-line (`/* */`) comments
- **Whitespace Handling**: Properly skips all forms of whitespace while preserving line counting

### Token Types
- **Keywords**: `int`, `float`, `while`, `main`
- **Operators**: `+`, `-`, `*`, `/`, `++`, `--`, `=`, `<`, `>`
- **Delimiters**: `;`, `,`, `(`, `)`, `{`, `}`
- **Identifiers**: Variable names following C-style naming rules
- **Literals**: Integer literals and floating-point literals
- **Special Tokens**: End-of-file marker and error token

### Error Detection
- **Invalid Characters**: Reports characters not valid in the language
- **Malformed Identifiers**: Detects invalid identifiers (e.g., starting with numbers)
- **Invalid Literals**: Identifies malformed number literals
- **Missing Semicolons**: Detects missing statement terminators
- **Unclosed Comments**: Reports multi-line comments without closing delimiter

### Output Generation
- **Detailed Token Information**: Outputs complete token information to `tokens.txt`
- **Simplified Token Stream**: Generates a token stream for the parser in `token_stream.txt`

## 2. Symbol Table Management

### Data Structures
- **Hash Table Implementation**: Uses C++ unordered_map for O(1) lookups
- **Symbol Records**: Stores name, type, value, location, and initialization status
- **Scoped Symbol Management**: Supports nested scopes with proper visibility rules

### Scope Operations
- **Scope Entry/Exit**: Provides functions to enter and exit scopes
- **Scoped Lookups**: Symbol lookup follows proper scope hierarchy
- **Multiple Scopes**: Supports variable declarations in multiple scopes (including within loop bodies)

### Symbol Operations
- **Symbol Insertion**: Adds new symbols with type information
- **Symbol Lookup**: Retrieves symbol information by name
- **Type Management**: Handles both int and float types
- **Value Storage**: Manages variable values using std::variant

### File Output
- **Symbol Table Dump**: Can output entire symbol table to a CSV file
- **Symbol Table Visualization**: Prints a formatted table to console

## 3. Grammar Processing

### Grammar Representation
- **Symbol Representation**: Differentiates terminals and non-terminals
- **Production Rules**: Represents grammar productions in a structured format
- **Start Symbol**: Designates main as the start symbol of the grammar

### Key Algorithms
- **FIRST Set Computation**: Calculates FIRST sets for all non-terminals
- **FOLLOW Set Computation**: Calculates FOLLOW sets for all non-terminals
- **Sequence FIRST Computation**: Handles FIRST of symbol sequences
- **Epsilon Handling**: Properly handles epsilon (ε) in FIRST/FOLLOW sets

### Grammar Rules
- **Program Structure**: Rules for overall program structure
- **Declaration Statements**: Rules for variable declarations
- **Loop Constructs**: Rules for while loops
- **Expression Rules**: Rules for arithmetic expressions
- **Operator Precedence**: Enforces operator precedence through grammar structure

### File Output
- **FIRST/FOLLOW Visualization**: Outputs computed sets to `first_follow.txt`

## 4. LL(1) Parsing

### Parse Table Construction
- **Automatic Table Generation**: Builds parse table from FIRST/FOLLOW sets
- **Conflict Detection**: Detects and reports LL(1) grammar conflicts
- **Terminal and Non-Terminal Mapping**: Maps all grammar symbols to table entries

### Predictive Parsing
- **Stack-Based Implementation**: Uses a parse stack for top-down parsing
- **Table-Driven Parsing**: Makes parsing decisions based on parse table
- **Token Consumption**: Matches and consumes input tokens against expected terminals
- **Production Expansion**: Expands non-terminals according to parse table entries

### Error Recovery
- **Panic Mode**: Implements panic mode recovery for syntax errors
- **Synchronization Points**: Uses statement boundaries as synchronization points
- **Stack Adjustment**: Adjusts parse stack to recover from errors
- **Error Reporting**: Reports detailed syntax error messages

### Parsing Process Visualization
- **Step-by-Step Logging**: Records each parsing step in `parsing_stages.txt`
- **Stack Contents**: Shows parse stack at each step
- **Current Input**: Displays current input token
- **Production Used**: Indicates which production is being applied
- **Action Taken**: Describes the parsing action (match, expand, error, etc.)

## 5. Error Handling

### Error Types
- **Lexical Errors**: Invalid tokens, characters, etc.
- **Syntax Errors**: Grammar violations, unexpected tokens
- **Semantic Errors**: Type mismatches, scope violations

### Error Reporting
- **Position Information**: Reports line and column numbers
- **Descriptive Messages**: Provides clear error descriptions
- **Error Categorization**: Classifies errors by type
- **Real-Time Logging**: Records errors as they occur in `error.txt`
- **Comprehensive Reports**: Generates detailed error reports in `errors.txt`

### Error Recovery
- **Lexical Recovery**: Skips invalid characters and continues
- **Syntax Recovery**: Implements panic mode to synchronize parsing
- **Continuation After Errors**: Attempts to find and report multiple errors

## 6. Project Infrastructure

### Build System
- **Makefile Support**: Provides a comprehensive Makefile for building
- **Dependency Management**: Tracks file dependencies for efficient compilation
- **Clean Targets**: Includes targets for removing generated files

### File Organization
- **Modular Design**: Separates functionality into focused components
- **Header/Implementation Separation**: Uses proper header/source file organization
- **Consistent Naming**: Follows a consistent naming convention

### Modern C++ Features
- **Smart Pointers**: Uses shared_ptr for shared ownership
- **STL Containers**: Leverages standard library containers
- **Variant Types**: Uses std::variant for variable values
- **Regular Expressions**: Uses std::regex for token matching
- **Modern C++ Practices**: Follows C++17 best practices 
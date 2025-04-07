# Compiler Project Report

## Overview
This report documents the implementation of a compiler for a simplified C-like programming language. The project implements a complete compilation pipeline including lexical analysis, syntax analysis with an LL(1) parser, and symbol table management. The implementation follows the requirements specified in the project requirements document, supporting a subset of C-like syntax focusing on basic variable declarations, assignments, and while loops.

## Language Specification
The implemented language supports:
- Basic data types: `int` and `float`
- Variable declarations with optional initialization
- Multiple variable declarations using comma separator
- While loops with relational conditions
- Arithmetic expressions with operators (+, -, *, /, ++, --)
- Assignment statements
- Relational operators (< and >)

## Compiler Components

### 1. Lexical Analyzer (Lexer)
The lexical analyzer scans the input source code and converts it into a stream of tokens. Each token is classified according to its type (keyword, identifier, operator, etc.).

Key features:
- Regex-based pattern matching for token recognition
- Support for all required operators and keywords
- Proper handling of identifiers and literals
- Line and column tracking for error reporting

### 2. Symbol Table
The symbol table maintains information about all identifiers in the program, including:
- Variable names
- Data types
- Scope information
- Memory allocation (conceptual)

### 3. Syntax Analyzer (Parser)
The parser implements an LL(1) parsing algorithm, which is a top-down parsing technique that constructs a parse tree starting from the root (start symbol) and going down to the leaves.

Key features:
- Grammar definition with productions
- Computation of FIRST and FOLLOW sets
- Construction of the parsing table
- Predictive parsing algorithm
- Error recovery using panic mode

### 4. Error Handling
The compiler includes comprehensive error handling:
- Lexical errors (invalid tokens)
- Syntax errors (unexpected tokens)
- Panic mode recovery to continue parsing after errors

## Grammar Implementation
The grammar for the language is implemented as an LL(1) grammar. Below are the key non-terminals and their productions:

```
program → int main() { stmts }
stmts → stmt stmts | ε
stmt → decl | expr_stmt | while_stmt
decl → type id_list ;
type → int | float
id_list → ID id_tail
id_tail → , ID id_tail | init_opt
init_opt → = expr | ε
while_stmt → while ( cond ) { stmts }
expr_stmt → ID expr_stmt_tail | unary_op ID ;
expr_stmt_tail → = expr ; | ++ ; | -- ;
expr → term expr_tail
expr_tail → add_op term expr_tail | ε
term → factor term_tail
term_tail → mul_op factor term_tail | ε
factor → ID | CONST | ( expr )
cond → expr rel_op expr
```

The grammar was designed to be LL(1) compatible, requiring careful handling of left-recursion removal and left-factoring.

## Parser Implementation Details

### Terminal Symbol Matching
A critical aspect of the implementation was ensuring that token types from the lexer correctly mapped to terminal symbols in the grammar. This was accomplished through the `tokenToString` method, which converts tokens to the appropriate string representation used in the parse table:

- Keywords map to their lexeme (e.g., "int", "while")
- Identifiers map to "ID"
- Constants map to "CONST"
- Operators map to their character representation

### Parse Table Construction
The parse table is constructed by computing FIRST and FOLLOW sets for each non-terminal in the grammar. These sets are then used to determine which production to use when a specific token is encountered during parsing.

### Parsing Algorithm
The parsing algorithm uses a stack to keep track of the symbols yet to be processed. The algorithm continually compares the top of the stack with the current input token:
- If they match (for terminals), the token is consumed
- If the top is a non-terminal, the appropriate production is looked up in the parse table and pushed onto the stack in reverse order
- If no production is found, an error is reported and recovery is attempted

### Error Recovery
The parser implements panic mode error recovery, which:
1. Discards tokens until a synchronizing token (like a semicolon) is found
2. Pops symbols from the stack until a suitable recovery point is reached
3. Attempts to continue parsing from that point

## Testing
The compiler was tested with various test files to verify its functionality:

1. `sample_test.txt`: Basic program structure with variable declaration and assignment
2. `sample_test2.txt`: Program with while loop and float variables
3. `sample_test3.txt`: Comprehensive test with all language features
4. `sample_test4.txt`: Test for multiple variable declarations in a single statement

All test files were successfully parsed, demonstrating the robustness of the implementation.

## Challenges and Solutions

### Symbol Name Conflicts
An initial challenge was a naming conflict between the `Symbol` struct in `grammar.h` and `symbol_table.h`. This was resolved by renaming the struct in `grammar.h` to `GrammarSymbol` to avoid redefinition errors.

### Terminal Matching Issues
The parser initially had difficulties matching tokens with grammar terminals due to differences in representation. This was resolved by implementing the `tokenToString` method, which properly converts token types to their corresponding grammar terminal names.

### Parse Table Conflicts
Some ambiguities in the grammar led to conflicts in the parse table. These were resolved by:
1. Refactoring the expression statement grammar
2. Adding a new non-terminal `expr_stmt_tail` to disambiguate cases
3. Ensuring the grammar remained LL(1) compatible

## Conclusion
The implemented compiler successfully fulfills all the requirements specified in the project document. It demonstrates a functioning LL(1) parser for a simplified C-like language, with proper error handling and recovery mechanisms. The modular design allows for future extensions to support more complex language features.

The project provides a solid foundation for understanding compiler construction principles, particularly the implementation of lexical analysis, predictive parsing, and symbol table management. 
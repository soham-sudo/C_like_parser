# Compiler Frontend for C-like Language

This project implements a compiler frontend for a simplified C-like language, including a lexical analyzer and LL(1) parser.

## Features

- Regex-based lexical analyzer
- Symbol table with scope management
- LL(1) parser with FIRST/FOLLOW set computation
- Detailed error reporting with line and column information
- Comprehensive output files for debugging and visualization

## Requirements

- C++17 compatible compiler (g++ recommended)
- Standard C++ libraries

## How to Build

```bash
make clean
make
```

## Running the Compiler

The program can be run in two ways:

1. **Default mode** (using sample_correct.txt):
   ```bash
   ./compiler
   ```

2. **With a specified file**:
   ```bash
   ./compiler sample_test.txt
   ```

Two sample files are provided:
- `sample_correct.txt`: Contains valid code according to the language specification
- `sample_error.txt`: Contains deliberate errors to test error detection

## Compiler Pipeline

The compiler performs the following steps:

1. **Lexical Analysis**: Scans the source code and generates tokens
2. **FIRST/FOLLOW Set Generation**: Computes FIRST and FOLLOW sets for the grammar
3. **Parse Table Generation**: Creates an LL(1) parsing table
4. **Parsing**: Performs table-driven predictive parsing

## Output Files

The compiler generates the following output files:

- `tokens.txt`: Detailed token information (token type, lexeme, line, column)
- `token_stream.txt`: Simplified token stream for the parser
- `first_follow.txt`: FIRST and FOLLOW sets for each non-terminal
- `parse_table.txt`: LL(1) parsing table
- `parsing_stages.txt`: Step-by-step parsing process with stack contents
- `error.txt`: Real-time error logging with line and column information
- `errors.txt`: Comprehensive error report (generated if errors are detected)

## Error Handling

The compiler detects various errors including:
- **Lexical errors**: Invalid characters, malformed tokens, etc.
- **Syntax errors**: Incorrect syntax according to the grammar
- **Parser errors**: Missing or unexpected tokens

The parser includes panic mode error recovery to continue after errors.

## Project Structure

- `lexer.h/cpp`: Lexical analyzer implementation
- `symbol_table.h/cpp`: Symbol table for tracking variables
- `error_handler.h/cpp`: Error reporting and logging
- `grammar.h/cpp`: Grammar definition and FIRST/FOLLOW set computation
- `parser.h/cpp`: LL(1) parser implementation
- `main.cpp`: Driver program 

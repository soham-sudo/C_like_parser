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

## Visual Demonstrations

### Screenshots

1. **Lexical Analysis Output**
   ![Lexical Analysis](assets/lexical_analysis_output.png)
   *Token stream and detailed token information*

2. **Parse Table Generation**
   ![Parse Table](assets/parse_table_generation.png)
   *LL(1) parse table showing production rules*

3. **Symbol Table**
   ![Symbol Table](assets/symbol_table_output.png)
   *Variable declarations and their attributes*

4. **Error Reporting**
   ![Error Report](assets/error_reporting.png)
   *Detailed error messages with line and column information*

### Video Demonstrations

1. **Full Compilation Process**
   [![Compilation Process](assets/compilation_process_thumbnail.png)](assets/compilation_process_demo.mp4)
   *Complete demonstration of the compiler pipeline*

2. **Error Handling**
   [![Error Handling](assets/error_handling_thumbnail.png)](assets/error_handling_demo.mp4)
   *Demonstration of error detection and recovery*

## Test Files

The project includes several test files to demonstrate different language features:

- `sample_correct.txt`: Contains valid code according to the language specification
- `sample_error.txt`: Contains deliberate errors to test error detection
- `sample_test.txt`: Basic program with variable declaration and assignment
- `sample_test2.txt`: Program with while loop and float variables
- `sample_test3.txt`: Comprehensive test with all language features
- `sample_test4.txt`: Test for multiple variable declarations in a single statement

## Compiler Pipeline

The compiler performs the following steps:

1. **Lexical Analysis**: Scans the source code and generates tokens
2. **FIRST/FOLLOW Set Generation**: Computes FIRST and FOLLOW sets for the grammar
3. **Parse Table Generation**: Creates an LL(1) parsing table
4. **Parsing**: Performs table-driven predictive parsing
5. **Symbol Table Generation**: Tracks variables and their attributes

## Output Files

The compiler generates the following output files in the `output` directory:

- `tokens.txt`: Detailed token information (token type, lexeme, line, column)
- `token_stream.txt`: Simplified token stream for the parser
- `first_follow.txt`: FIRST and FOLLOW sets for each non-terminal
- `parse_table.txt`: LL(1) parsing table
- `parsing_stages.txt`: Step-by-step parsing process with stack contents
- `symbol_table.txt`: Symbol table entries with variable information
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

## Documentation

The project includes several documentation files:

- `README.md`: This file, providing an overview of the project
- `compiler_project_report.md`: Comprehensive report on the compiler implementation
- `language_syntax.md`: Quick reference for the language syntax with examples
- `requirements.md`: Original project requirements and specifications

## Language Syntax

The implemented language supports:
- Basic data types: `int` and `float`
- Variable declarations with optional initialization
- Multiple variable declarations using comma separator
- While loops with relational conditions
- Arithmetic expressions with operators (+, -, *, /, ++, --)
- Assignment statements
- Relational operators (< and >)

See `language_syntax.md` for detailed examples.

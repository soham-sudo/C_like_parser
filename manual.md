# Compiler User Manual

This manual provides detailed instructions on how to build, run, and use the compiler for the simplified C-like language.

## Table of Contents

1. [System Requirements](#system-requirements)
2. [Building the Compiler](#building-the-compiler)
3. [Running the Compiler](#running-the-compiler)
4. [Input File Format](#input-file-format)
5. [Output Files](#output-files)
6. [Interpreting Results](#interpreting-results)
7. [Error Handling](#error-handling)
8. [Troubleshooting](#troubleshooting)

## System Requirements

- **Operating System**: Linux, macOS, or Windows with WSL/MinGW
- **Compiler**: g++ with C++17 support (version 7.0 or higher)
- **Make**: GNU Make utility
- **File System**: Read/write permissions for the current directory
- **Memory**: Minimal (less than 50MB)

## Building the Compiler

1. **Clone/Download the Project**
   Ensure all source files are in the same directory.

2. **Clean the Build Directory**
   ```bash
   make clean
   ```
   This removes any previously compiled object files and executables.

3. **Compile the Project**
   ```bash
   make
   ```
   This compiles all source files and generates the `compiler` executable.

4. **Verify the Build**
   After successful compilation, you should see the `compiler` executable in the current directory.

## Running the Compiler

The compiler can be run in two ways:

### Default Mode

```bash
./compiler
```

This runs the compiler on the default file `sample_correct.txt`.

### Specified File Mode

```bash
./compiler <input_file>
```

For example:
```bash
./compiler sample_error.txt
```

This allows you to specify a custom input file to compile.

### Execution Stages

When you run the compiler, it will execute the following stages:

1. **Lexical Analysis**: Scans source code and produces tokens
2. **FIRST/FOLLOW Set Generation**: Computes FIRST and FOLLOW sets for the grammar
3. **Parse Table Generation**: Creates an LL(1) parsing table
4. **Parsing**: Performs table-driven predictive parsing

The compiler will report progress and results for each stage on the console.

## Input File Format

The compiler accepts source files written in the simplified C-like language. Here's the format:

### Program Structure

```c
int main() {
    // Declarations and statements
}
```

### Supported Features

- **Data Types**: `int` and `float`
- **Variable Declarations**: With initialization (e.g., `int x = 5;`)
- **Loop Construct**: Single `while` loop (no nesting)
- **Arithmetic Operations**: `+`, `-`, `*`, `/`, `++`, `--`
- **Relational Operations**: `<`, `>`
- **Comments**: Single-line (`//`) and multi-line (`/* */`)

### Example Valid Program

```c
int main() {
    int counter = 10;
    float sum = 0.0;
    
    while (counter > 0) {
        sum = sum + counter;
        counter--;
    }
}
```

## Output Files

The compiler generates several output files:

1. **tokens.txt**
   - Detailed information about all tokens found during lexical analysis
   - Format: Token Type, Lexeme, Line, Column

2. **token_stream.txt**
   - Simplified token stream for the parser
   - Lists all tokens in sequence

3. **first_follow.txt**
   - FIRST and FOLLOW sets for all non-terminals in the grammar
   - Used in constructing the parse table

4. **parse_table.txt**
   - The LL(1) parsing table showing which production to use for each (non-terminal, terminal) pair
   - Format: Table with non-terminals as rows and terminals as columns

5. **parsing_stages.txt**
   - Step-by-step record of the parsing process
   - Shows stack contents, current input, production used, and action taken at each stage

6. **error.txt**
   - Real-time error logging (created if errors are detected)
   - Lists errors as they are encountered

7. **errors.txt**
   - Comprehensive error report (created if errors are detected)
   - Categorizes errors by type with detailed information

## Interpreting Results

### Console Output

The compiler provides a summary of each stage on the console:
- Success/failure of lexical analysis
- Generation of FIRST and FOLLOW sets
- Generation of parse table
- Success/failure of parsing
- List of output files generated

### Lexical Analysis Results

Check `tokens.txt` for a detailed list of all tokens recognized in the source file.

### Parsing Results

1. **FIRST/FOLLOW Sets**
   - Check `first_follow.txt` to understand which terminals can appear at the beginning of each non-terminal
   - This is crucial for understanding the grammar and parse table

2. **Parse Table**
   - Examine `parse_table.txt` to see how the parser makes decisions
   - Empty cells represent syntax errors

3. **Parsing Process**
   - Look at `parsing_stages.txt` to see the step-by-step execution of the parser
   - This helps understand how the parser processes the input

### Error Information

If the compiler detects errors:
1. Examine `error.txt` for real-time error logging
2. Check `errors.txt` for a comprehensive error report categorized by error type

## Error Handling

### Types of Errors

1. **Lexical Errors**
   - Invalid characters
   - Malformed identifiers
   - Invalid literals
   - Unclosed comments

2. **Syntax Errors**
   - Unexpected tokens
   - Missing tokens
   - Grammar violations

### Error Messages

Error messages include:
- Error type (Lexical, Syntax, Semantic)
- Line and column number
- Brief description of the error
- Context information (when available)

### Error Recovery

The compiler implements error recovery mechanisms:
- **Lexical Analysis**: Skips invalid characters and continues
- **Parsing**: Uses panic mode to skip to a synchronization point (e.g., `;`)

## Troubleshooting

### Common Issues

1. **Compilation Fails**
   - Ensure you have a g++ compiler with C++17 support
   - Check for missing source files

2. **Input File Not Found**
   - Verify the file exists in the correct location
   - Check file permissions

3. **Unexpected Parse Errors**
   - Ensure your input follows the grammar of the simplified C-like language
   - Check for typos or syntax errors in your source code

4. **Output Files Not Generated**
   - Check write permissions in the current directory
   - Look for error messages on the console

### Getting Help

If you encounter issues not covered in this manual, check the following resources:
- Review `features.md` for a detailed list of supported features
- Examine the `README.md` for project overview
- Check the grammar definition in `grammar.cpp` for language syntax 
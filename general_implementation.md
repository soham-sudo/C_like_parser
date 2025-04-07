Project Overview
You're developing a compiler for a simplified C-like language that supports:

Basic data types (int and float)
Variable declarations with initialization
While loops (non-nested)
Simple arithmetic expressions
Limited operators (relational and arithmetic)
Only a main function without arguments or returns

Key Modules to Implement
1. CFG (Context-Free Grammar) Design
You'll need to define the grammar rules for your language, including:

Program structure
Declaration statements
While loop construct
Expression syntax
Statement types (assignments, increments/decrements)

2. Lexical Analyzer
This module scans your source code and produces tokens:

Token definition structure (type, value, line number, etc.)
Scanner implementation to recognize:

Keywords (int, float, while)
Identifiers (variable names)
Constants (numeric literals)
Operators (+, -, *, /, ++, --, <, >, =)
Delimiters ({, }, (, ), ;)


Error handling for invalid characters

3. Symbol Table
This maintains information about variables:

Table structure (hash table, balanced tree, etc.)
Entry format (name, type, value, scope, etc.)
Operations: insert, lookup, delete
Scope management (though simplified with just main function)

4. Parser
This implements the top-down parsing algorithm:

Left recursion elimination
FIRST and FOLLOW set computation
LL(1) parsing table construction
Error recovery mechanisms (panic mode)

5. Integration Components

Driver program to coordinate components
Error reporting system
Input/output handling

Implementation Approach

Start with formal CFG definition
Implement lexical analyzer
Design symbol table structure
Develop parsing components
Add error recovery
Integrate all modules
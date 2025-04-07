Consider a C-like program representing a single while loop in a main function.
Data Type: Basic data types integer (int) and floating point (float).
There are declaration statements at the beginning for declaring the data types of the variables. Variables are assigned with values at the time of declaration.
Loop Constructs: while (condition) {S}
Nested loops are not supported.
Within the loop, the following two simple statements are supported:
(i)Arithmetic expressions assigning a value to a variable. An arithmetic expression may contain constants and variables. There can be one or more arithmetic expressions within a loop.
(ii)A statement that increments or decrements the loop index variable.
Variables may be declared inside the loop as well.
Relational operators supported in the loop construct are {< and >}, i.e. less than, greater than.
    Arithmetic operators supported are {+, –, *, /, ++ and --}, i.e. addition, subtraction, multiplication, division, increment and decrement operators. Assignment operator =’ is supported.
    Only function is main(), there is no other function. The main() function does not have arguments and return statements.
Part I – Construct a CFG for this language.
Part II – Write a lexical analyzer to scan the stream of characters from a program written in the above language and generate stream of tokens.
Part III – Maintain a symbol table with appropriate data structures.
Part IV – Write a top-down parser for this language (modules include Left recursion removal, FIRST, FOLLOW, parsing table construction and parsing). Insert the panic mode error recovery rules in the table.

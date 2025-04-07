# Language Syntax Reference

This document provides quick reference examples for the syntax of our simplified C-like language.

## Basic Program Structure

```
int main() {
    // Program statements go here
}
```

## Variable Declarations

### Single Variable Declaration

```
int a;
float b;
```

### Multiple Variable Declaration

```
int a, b, c;
float x, y, z;
```

### Declaration with Initialization

```
int a = 5;
float pi = 3.14159;
```

## Assignments

```
a = 10;
b = 20;
c = a + b;
```

## Arithmetic Expressions

```
// Addition
sum = a + b;

// Subtraction
difference = a - b;

// Multiplication
product = a * b;

// Division
quotient = a / b;

// Complex expressions
result = (a + b) * (c - d) / 2;
```

## Increment and Decrement

```
// Increment
count++;

// Decrement
count--;
```

## While Loops

```
// Basic while loop
while (count < 10) {
    sum = sum + count;
    count++;
}

// While loop with complex condition
while (a > b) {
    a = a - 1;
}
```

## Nested Blocks

```
while (i < 5) {
    int temp = i * 2;
    sum = sum + temp;
    i++;
}
```

## Complete Example

```
int main() {
    int count = 0;
    int sum = 0;
    float average, result;
    
    while (count < 5) {
        sum = sum + count;
        count++;
    }
    
    average = sum / 5;
    
    int i = 10;
    while (i > 0) {
        i--;
    }
    
    result = average * 2;
} 
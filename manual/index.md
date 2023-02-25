# Pascual User's Manual

This is the user manual to [Pascual](https://github.com/jtarrio/pascual), a Pascal compiler written by [Jacobo Tarrío](https://jacobo.tarrio.org).

Pascual is a Pascal compiler written in Pascal, which has been able to compile itself since January 2nd, 2023.

Instead of assembly code, Pascual compiles into C code that can be further compiled into executable code by the GNU C compiler or by Clang.

Pascual currently supports a subset of the Turbo Pascal 3 syntax and features:

* `Boolean`, `integer`, `real`, `char`, and `string`; enumerated types, subranges; arrays, records, pointers, and sets; text files.
* Type definitions, variable definitions, untyped constant definitions, typed constant definitions.
* Functions and procedures, pass by value, pass by reference, and pass by const reference.
* `While`, `repeat until`, `for`, `if`, `case`.
* `Write`, `writeln`, `read`, `readln`, `str`.

Many features still need to be implemented before a "version 1" of Pascual can be released, such as:
* Non-text files;
* nested subroutines;
* arrays passed as subroutine arguments;
* many standard library functions.

However, Pascual is already useful for many real-world applications, as evidenced by the fact that it is used to develop itself.

Pascual is made available under the terms of the Apache License 2.0.

This manual is made available under a [Creative Commons Attribution-ShareAlike 4.0 International License](https://creativecommons.org/licenses/by-sa/4.0/).

## In this manual

Chapter 1, "[Getting Started with Pascual](chapter1.md)", describes how to download and build Pascual, and how to use it.

Chapter 2, "How to Program in Pascal", is an introduction to the Pascal programming language. Even if you don't know how to program, this chapter should tell you all you need to know to get you started.

The following chapters give you more in-depth information about all the aspects of Pascal programming:
* Chapter 3, "Basic elements" describes the symbols and reserved words of the language, the standard data types, and the syntax used to write numbers, character strings, and comments.
* Chapter 4, "User-defined elements" describes user-defined types, variables, constants, procedures, and functions.
* Chapter 5, "Structure of a program" describes the blocks a program is composed of, 
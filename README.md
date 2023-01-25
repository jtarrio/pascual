# Pascual: A self-hosting Pascal compiler

Pascual is a Pascal compiler by [Jacobo Tarrío](https://jacobo.tarrio.org). Pascual is written in Pascal and can compile itself.

Pascual currently supports a subset of the Turbo Pascal 3 syntax and features:

* Boolean, integer, char, and string; enumerated types; arrays, records, and pointers; text files.
    * Missing real numbers, ordinal subranges, sets, non-text files.
    * Missing arrays indexed by non-integer ordinal subranges.
    * Missing comma syntax for multidimensional arrays.
    * Missing packed arrays and records.
    * Missing variants.
    * Missing "with" statement.
    * Missing bounds checking.
    * Missing active I/O result checking.
* Type definitions, variable definitions, untyped constant definitions, typed constant definitions.
    * Missing record typed constants.
* Functions and procedures, pass by value and pass by reference.
    * Missing functions and procedures as arguments.
    * Missing nested functions and procedures.
    * Missing parameterized array bounds.
* While, repeat until, for, if, case.
* Write, writeln, read, readln, str.
    * Missing format specifiers.
* $I (include) directive.

The first objective with Pascual is to make it comparable to Turbo Pascal 3. The next objective is to introduce compilation units and make it comparable to Turbo Pascal 5. A stretch objective is to introduce Object Pascal features and make it comparable to Turbo Pascal 7.

Pascual compiles to the C programming language. A stretch objective is to add an assembly language backend and so turn it into a “real” compiler, but it is expected to retain the ability to compile to C.

Pascual is a platform for learning how to write a compiler; as such, even though I hope to make it as complete as possible, it may always be a bit of a toy.

# Pascual: A self-hosting Pascal compiler

Pascual is a Pascal compiler by [Jacobo Tarrío](https://jacobo.tarrio.org). Pascual is written in Pascal and can compile itself.

Pascual generates C code that can then be compiled into an executable file.

# Features

Pascual currently supports a subset of the Turbo Pascal 3 syntax and features:

* `Boolean`, `integer`, `real`, `char`, and `string`; enumerated types, subranges; arrays, records, pointers, and sets; text files.
    * Missing non-text files.
    * Missing narrow machine types for subranges (eg 16-bit word for 0..65535).
    * Missing packed arrays and records.
    * Missing active I/O result checking.
* Type definitions, variable definitions, untyped constant definitions, typed constant definitions.
    * Missing record typed constants.
* Functions and procedures, pass by value, pass by reference, and pass by const reference.
    * Missing functions and procedures as arguments.
    * Missing nested functions and procedures.
    * Missing parameterized array bounds.
* `While`, `repeat until`, `for`, `if`, `case`.
    * Missing `goto`.
* `Write`, `writeln`, `read`, `readln`, `str`.
    * Missing format specifiers.
* Compiler directives:
    * `{$I name.pas}`: include the file `name.pas`
    * `{$R[-+]}`: disable or enable bounds checking (enabled by default)

# Objectives

The first objective with Pascual is to make it comparable to Turbo Pascal 3. The next objective is to introduce compilation units and make it comparable to Turbo Pascal 5. A stretch objective is to introduce Object Pascal features and make it comparable to Turbo Pascal 7.

Pascual compiles to the C programming language. A stretch objective is to add an assembly language backend and so turn it into a “real” compiler, but it is expected to retain the ability to compile to C.

Pascual is a platform for learning how to write a compiler; as such, even though I hope to make it as complete as possible, it may always be a bit of a toy.

# How to build

You need Make and a C compiler to build Pascual. I use GCC but Clang should work as well.

To build Pascual, type the following command:

```shell
$ make
```

Pascual should be available on the `dist` subdirectory.

# How to use

There are two executables: `pascual` and `pascualc`. The `pascual` executable is the Pascal-to-C converter; `pascualc` is a shell script that uses `pascual` to implement a regular compiler that outputs an executable file.

(The `pascualc` script will, one day, be removed, and `pascual` will be able to call the C compiler itself.)

To convert a Pascal program to C, run `pascual` like this:

```shell
$ dist/pascual input.pas -o output.c
```

If you don't specify an output file name, it will be the same as the input file name, with its `.pas` extension replaced with `.c`.

You can also read from `stdin` and write to `stdout` by using `-` (hyphen) as the input or output file names.

To compile to an executable, run `pascualc` like this:

```shell
$ dist/pascualc input.pas -o output
```

If you don't specify an output file name, it will be the same as the input file, with its `.pas` extension removed.

You can also convert a program to C and then compile it afterwards with your C compiler, by doing it like this:

```shell
$ dist/pascual input.pas -o output.c
$ gcc -Idist -Ldist -o output output.c -lpascual
```

# What it looks like

I've seen so many programs that convert a language to some other language, and they never show examples. Not Pascual: you can see some examples, with their conversions to C, in the `examples` subdirectory.

# Documentation

I'm going to write it and it's going to be awesome.

In the meantime, you can look at the `examples` subdirectory to see what kinds of features are supported and how they are used.

# License

Pascual, its runtime library, examples, and tests are made available under the terms of the Apache License 2.0.
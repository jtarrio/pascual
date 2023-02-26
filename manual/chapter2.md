# How to Program in Pascal

## A little history of Pascal

Pascal is a programming language that was released in 1970 by Niklaus Wirth. It was an improvement on the ALGOL-60 language, and it was designed to encourage good programming practices through structured programming and data structures.

It was modestly successful until Borland released Turbo Pascal, an affordable Pascal compiler for home computers that was extremely fast and came with an IDE. As Borland released newer and bigger versions, its popularity grew; many games and applications from the early 90s were written in Turbo Pascal and in Borland Pascal. In 1995, Borland released Delphi, which is the dialect of Pascal most people are familiar with nowadays.

As for Niklaus Wirth, Pascal wasn't his first language or his last one. Before Pascal he developed Euler, PL360, and ALGOL W, and after Pascal he worked on Modula and Modula-2. In 1984 he won the Turing Award (the highest distinction in computer science), but he didn't stop there and went on to develop the Oberon family of languages and operating systems.

## Preparing the computer

Before we try to write our first program in Pascal, we need to set up our development environment. Follow the instructions in [chapter 1](chapter1.md) to download and build Pascual. Afterwards, you can write your programs in the `pascual` or `pascual-main` directory and use the `dist/pascualc` command to compile them.

## Pascal and Pascual

This chapter shows you how to program in the Pascal programming language using the Pascual compiler.

Let me explain.

The Pascal language is a set of rules on how to write and organize a program. It tells you how to write a number, how to write a character string, how to write an assignment statement, a conditional statement, etc. When you write a program following those rules, we say that this program was written in the Pascal language.

A Pascal compiler is a program. It takes a program written in the Pascal language and converts it into a form that is executable by the computer.

There are many Pascal compilers, and they all work slightly differently from each other. Computers have changed and evolved a lot since the rules for the Pascal language were written, and different compilers adopted slightly different rules to write programs for the new things computers can do. However, there is a "core" Pascal language that most (if not all) compilers agree on.

Pascual is a Pascal compiler, and of course it has its own quirks.

So, in this chapter, when I talk about "Pascal", I refer to the Pascal language, especially to the parts that most compilers agree on. When I talk about "Pascual", I refer to the Pascual compiler, or to the parts of the language that are specific to Pascual.

## Writing the first program

Traditionally, when a programmer wants to try out a language, they write a program that prints out the text "Hello world"; let's do exactly that!

Type out the following program into a file named `helloworld.pas` (you _could_ copy and paste, but I find that typing out example programs by hand gives me a better feel for the language and helps me learn better:)

```pascal
program HelloWorld;
begin
  writeln('Hello world!')
end.
```

After saving your program, compile it and execute it:

```
$ dist/pascualc helloworld.pas
$ ./helloworld
Hello world!
$
```

That's it! That's your first program. Now, if you want, you can modify it to see what other things you can make it say; just write the text you want between the single quote signs (`'`), then compile and run again.

Let's go through the elements of this program:

The first line declares the program's name. You can write any name you want, as long as it's a single word; so you could write `program Hello` or `program JacoboSaysHello` or `program ThisIsMyFirstProgram`, but not `program I have no idea what I am doing`.

Declaring the name of a program in the program itself used to be important in old computers, but not in modern ones, so you can write whatever name you want in there. It is a good practice, though, to write a real name for the program.

Notice the semicolon (`;`) at the end of the line. It marks the end of the `program` definition, so don't forget to write it! I'll talk more about semicolons later.

The next line, `begin`, tells the compiler that the program's code comes after.

The next line is the first (and only) statement in the program. It causes the string `Hello, world!` to be printed out to the screen, and it does that through the `writeln` procedure.

In Pascal, a procedure is a type of subroutine (or subprogram, if you may). It's a piece of code with a name, that can be called and executed from different parts of the program by using that name. In this program, the `writeln` procedure prints out the value of its argument to the screen. The arguments are specified between parentheses, and, in this case, the single quotes that enclose the argument (`'`) indicate that it's a character string.

In the final line, `end.` with the final period marks the end of the program.

### Some things to try

You should try making a few changes to this program before continuing to the next section. Get a feel for the language! Here are a few things you can try:

The Pascal language is case-insensitive (that's computerese for "it doesn't care whether you use capital letters or small letters"). Therefore, whether you write `program` or `PROGRAM` or `pRoGrAm`, it is all the same. Try changing some of the keywords to upper case (that's computerese for "capitals") like in `BEGIN` or `WRITELN`, or capitalize the first letter only like in `Begin` or `Writeln`. Which option looks most pleasing to you? Different people have different opinions.

How about the character string between the single quotes? Does it make any difference to write `'Hello world!'` or `'HELLO WORLD!'`?

Try removing the semicolon at the end of the `program` line, or the period at the end of the `end` line. Does the program compile, or does Pascual give you an error message? What does the error message look like?

Try declaring different program names. I already told you it needs to be a single word, but try adding numbers, like `program Number1`, or `program 1stProgram`. Try some symbols too, like the underscore (`_`) or the hyphen (`-`). What works? What does not work?

This program was written in four lines. What happens if you just mash everything together in a single line? What if you split a line in two? Does it matter where you split the line?

Feel free to play around as much as you like. You won't break anything; the worst thing that will happen is that Pascual will give you an error message. So just make changes, try them, read the error messages, try to fix them, and have fun!

## Writing the second program

The "Hello World" program is the traditional first program, but it is not very interesting: it only contains one statement and always does the same thing. Therefore, let's write a slightly more complicated example.

Type out the following program and save it into a file named `helloname.pas`:

```pascal
program HelloName;
var Who : string;
begin
  write('What is your name? ');
  readln(Who);
  writeln('Hello, ', Who, '!')
end.
```

When you compile and run this program, it will ask your name. After you write it, the program will greet you by name!

```
$ dist/pascualc helloname.pas
$ ./helloname
What is your name? Jacobo
Hello, Jacobo!
$
```

There are a couple of differences between the previous program and this one, that you may notice immediately.

The first one is the line that starts with the keyword `var`. That keyword begins a _variable declaration block_.

A variable is a name for a location in the computer's memory, which can be used to store modifiable data. You would use variables to store the results of calculations, information provided by the user, or generally anything that is not hardcoded into the program.

In the Pascal language, variables need to be declared before the code that uses them. When you declare the variable, you give it a name and assign a type to it. This program declares a single variable, named `Who`, of type `string`.

The other difference is that, between the keywords `begin` and `end`, there are three statements instead of just one: a call to the `write` procedure, a call to `readln`, and, finally, a call to `writeln`. Notice how the three statements are separated with semicolons (`;`). Some languages want you to put each statement in a separate line; other languages require a semicolon after each statement, but Pascal uses it as a separator.


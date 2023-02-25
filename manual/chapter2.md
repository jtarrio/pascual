# How to Program in Pascal

## A little history of Pascal

Pascal is a programming language that was released in 1970 by Niklaus Wirth. It was an improvement on the ALGOL-60 language, and it was designed to encourage good programming practices through structured programming and data structures.

It was modestly successful until Borland released Turbo Pascal, an affordable Pascal compiler for home computers that was extremely fast and came with an IDE. As Borland released newer and bigger versions, its popularity grew; many games and applications from the early 90s were written in Turbo Pascal and in Borland Pascal. In 1995, Borland released Delphi, which is the dialect of Pascal most people are familiar with nowadays.

As for Niklaus Wirth, Pascal wasn't his first language or his last one. Before Pascal he developed Euler, PL360, and ALGOL W, and after Pascal he worked on Modula and Modula-2. In 1984 he won the Turing Award (the highest distinction in computer science), but he didn't stop there and then went on to develop the Oberon family of languages and operating systems.

## Preparing the computer

Before we try to write our first program in Pascal, we need to set up our development environment. Follow the instructions in [chapter 1](chapter1.md) to download and build Pascual. Afterwards, you can write your programs in the `pascual` or `pascual-main` directory and use the `dist/pascualc` command to compile them.

## Writing the first program

Traditionally, when a programmer wants to try out a language, they write a program that prints out the text "Hello world"; let's do exactly that!

Type out the following program into a file named `helloworld.pas` (you _could_ copy and paste, but I find that typing out example programs by hand gives me a better feel for the language and helps me learn better:)

```pascal
program HelloWorld;
begin
  writeln('Hello world!')
end.
```

Don't worry too much about using capital or small letters (computer people call them "upper case" and "lower case" letters). For the most part, Pascal doesn't care about that (or, as the computer people say, Pascal is not case sensitive). So you can write `program HelloWorld` but also `Program helloworld` or even `pRoGrAm HeLlOwOrLd`; it will all be the same to Pascal. The only exception is the bit between quote signs (`'`), but we'll get to that later.

After saving your program, compile and run it:

```
$ dist/pascualc helloworld.pas
$ ./helloworld
Hello world!
$
```

That's it! That's your first program. Now, if you want, you can modify it to see what other things you can make it say; just write the text you want between the single quote signs (`'`), then compile and run again.

Let's go through the elements of this program:

The first line declares the program's name. You can write any name you want, as long as it's a single word; so you could write `program Hello` or `program JacoboSaysHello` or `program ThisIsMyFirstProgram`, but not `program I have no idea what I am doing`. Nowadays, the compiler ignores whatever you write in there, so it is mostly for the programmer's benefit, but it is still a mandatory part of a Pascal program.

Notice the semicolon (`;`) at the end of the line. It marks the end of the `program` definition, so don't forget to write it! I'll talk more about semicolons later.

The next line, `begin`, tells the compiler that the following lines contain the program's code.

The next line is the first (and only) statement in the program. It calls the `writeln` procedure, passing it the string `'Hello, world!'` as an argument. The `writeln` procedure prints out the value of its argument. The single quotes that enclose the argument (`'`) tell Pascual that it's a character string. Therefore, `writeln` will write out the string "`Hello world!`" 

In the final line, `end.` with the final period marks the end of the program.

### Some things to try

Play around with this program before proceeding to the next section. For example, some people like to write everything in uppercase (`PROGRAM`, `BEGIN`, `WRITELN`, `END`), while other people like capitalizing the first letter only (`Program`, `Begin`, `Writeln`, `End`). Does it make a difference?

How about the character string between the single quotes? Does it make any difference to write `'Hello world!'` or `'HELLO WORLD!'`?

Try removing the semicolon at the end of the `program` line, or the period at the end of the `end` line. Does the program compile, or does Pascual give you an error message? What does the error message look like?

Try declaring different program names. I already told you it needs to be a single word, but try adding numbers, like `program Number1`, or `program 1stProgram`. Try some symbols too, like the underscore (`_`) or the hyphen (`-`). What works? What does not work?

This program was given to you in four lines. What happens if you just mash everything together in a single line? What if you split a line in two?

Feel free to play around as much as you like. You can't break anything by accident; the worst thing that will happen is that Pascual will give you an error message. So just make changes, try them, read the error messages, try to fix them, and have fun!

## Writing the second program

The "Hello World" program is the traditional first program, but it is not very interesting: it only contains one statement and always does the same thing. Therefore, let's write a slightly more complicated example.

Type out the following program and save it into a file named `helloname.pas`:

```pascal
program HelloName;

var Name : string;

begin
  write('What is your name? ');
  readln(Name);
  write('Hello, ', Name, '!')
end.
```


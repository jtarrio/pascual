# How to Program in Pascal

This chapter shows you how to program in the Pascal language using the Pascual compiler. Even if you don't know how to program, this chapter has everything you need to know to get you started.

## A little history of Pascal

Pascal is a programming language that was released in 1970 by Niklaus Wirth. It was an improvement on the ALGOL-60 language, and it was designed to encourage good programming practices through structured programming and data structures.

It was modestly successful until Borland released Turbo Pascal, an affordable Pascal compiler for home computers that was extremely fast and came with an IDE. As Borland released newer and bigger versions, its popularity grew; many games and applications from the early 90s were written in Turbo Pascal and in Borland Pascal. In 1995, Borland released Delphi, which is the dialect of Pascal most people are familiar with nowadays.

As for Niklaus Wirth, Pascal wasn't his first language or his last one. Before Pascal he developed Euler, PL360, and ALGOL W, and after Pascal he worked on Modula and Modula-2. In 1984 he won the Turing Award (the highest distinction in computer science), but he didn't stop there and went on to develop the Oberon family of languages and operating systems.

## Pascal and Pascual

The Pascal language is a set of rules on how to write and organize a program. It tells you how to write a number, how to write a character string, how to write an assignment statement, a conditional statement, etc. When you write a program following those rules, we say that this program was written in the Pascal language.

A Pascal compiler is a program. It takes a program written in the Pascal language and converts it into a form that is executable by the computer.

There are many Pascal compilers, and they all work slightly differently from each other. Computers have changed and evolved a lot since the rules for the Pascal language were written, and different compilers adopted slightly different rules to write programs for the new things computers can do. However, there is a "core" Pascal language that most (if not all) compilers agree on.

Pascual is a Pascal compiler, and of course it has its own quirks.

So, in this chapter, when I talk about "Pascal", I refer to the Pascal language, especially to the parts that most compilers agree on. When I talk about "Pascual", I refer to the Pascual compiler, or to the parts of the language that are specific to Pascual.

## Preparing the computer

Before we try to write our first program in Pascal, we need to set up our development environment. Follow the instructions in [chapter 1](chapter1.md) to download and build Pascual. Afterwards, you can write your programs in the `pascual` or `pascual-main` directory and use the `dist/pascualc` command to compile them.

## The first program

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

I am now going to explain what's in this program. Since it is our first program, it's going to be a bit long, but don't worry: it may be long, but not complicated.

The first line declares the program's name. You can write any name you want, as long as it's a single word; so you could write `program Hello` or `program JacoboSaysHello` or `program ThisIsMyFirstProgram`, but not `program I have no idea what I am doing`.

Some old computers used to require a program's name to be declared in the program itself, but that's not needed in modern ones. However, Pascal still requires it, so we must do it. Even though most compilers ignore the name, it is a good practice to write a real name in there, so that someone who is reading your source code can look at it and know what the program is about.

Notice the semicolon (`;`) at the end of the line. It separates the `program` definition from what comes after. Pascal uses semicolons as separators between the different parts of a program, and also to separate the statements in a sequence of statements that must be executed one after the other. It is a common mistake to forget to type a semicolon where it's required!

The `begin` keyword in the next line tells the compiler that this is the beginning of the program's code. The following lines until the `end` keyword will contain _statements_ that will be executed by the program.

This program only contains one statement, which causes the string `Hello world!` to be written out to the screen. This is done through the `writeln` procedure.

In Pascal, a procedure is a type of subroutine. A subroutine is a piece of code with a name, that can be executed (or _called_) from different parts of the program. It's a bit like having a smaller program inside your larger program, that your larger program can execute several times with different arguments. The arguments to the procedure are specified between parentheses right after the name of the procedure to be called.

Pascal compilers provide many subroutines which our programs can execute to do lots of different useful things. Programmers can also define their own subroutines, which I'll show you how to do later in this chapter. For now, in this program, we call the `writeln` procedure, which our compiler provides, with the argument `'Hello world!'`, and this procedure writes out the value of its argument to the screen.

What is the value of the argument? You can see that it is enclosed by a pair of single quotes (`'`), which tells the compiler that it is a character string. Therefore, the value of the argument `'Hello world!'` is the character string `Hello world!`, which is exactly what the `writeln` procedure writes out to the screen!

And, with this, we reach the `end` keyword with a period (`.`) at the end, which marks the end of the program.

### Some things to try

You should try making a few changes to this program before continuing to the next section. Get a feel for the language! Here are a few things you can try:

The Pascal language is case-insensitive (that's computerese for "it doesn't care whether you use capital letters or small letters"). Therefore, whether you write `program` or `PROGRAM` or `pRoGrAm`, it is all the same. Try changing some of the keywords to upper case (that's computerese for "capitals") like in `BEGIN` or `WRITELN`, or capitalize the first letter only like in `Begin` or `Writeln`. Which option looks most pleasing to you? Different people have different opinions.

How about the character string between the single quotes? Does it make any difference to write `'Hello world!'` or `'HELLO WORLD!'`?

Try removing the semicolon at the end of the `program` line, or the period at the end of the `end` line. Does the program compile, or does Pascual give you an error message? What does the error message look like?

Try declaring different program names. I already told you it needs to be a single word, but try adding numbers, like `program Number1`, or `program 1stProgram`. Try some symbols too, like the underscore (`_`) or the hyphen (`-`). What works? What does not work?

This program was written in four lines. What happens if you just mash everything together in a single line? What if you split a line in two? Does it matter where you split the line?

Feel free to play around as much as you like. You won't break anything; the worst thing that will happen is that Pascual will give you an error message. So just make changes, try them, read the error messages, try to fix them, and have fun!

## Variables

The "Hello World" program is the traditional first program, but it is not very interesting: it only contains one statement and always does the same thing. Therefore, let's write a slightly more complicated example.

Type out the following program and save it into a file named `helloname.pas`:

```pascal
program HelloName;
var Who : string;
begin
  writeln('What is your name? ');
  readln(Who);
  writeln('Hello, ', Who, '!')
end.
```

When you compile and run this program, it will ask your name. After you write it, the program will greet you by name!

```
$ dist/pascualc helloname.pas
$ ./helloname
What is your name?
Jacobo
Hello, Jacobo!
$
```

There are a couple of differences between the previous program and this one, that you may notice immediately.

The first one is the line that starts with the keyword `var`. That keyword begins a _variable declaration block_.

A variable is a chunk of memory that is used by the program to store data. You can read data from variables and you can modify their content, so you can use them to store information provided by the user or read from the network, or the results of calculations, or intermediate steps, or generally anything that is not hardcoded into the program.

In the Pascal language, variables need to be declared before the code that uses them. When you declare the variable, you give it a name so the program can refer to it and access its content, and you also assign a type to it so the compiler knows how much memory needs to be reserved for the variable and what operations can be performed on it.

This program declares a single variable, named `Who`, of type `string`, which can hold a character string.

The other difference between this program and the previous one is that, between the keywords `begin` and `end`, there are three statements instead of just one: a call to the `writeln` procedure, a call to `readln`, and, finally, another call to `writeln`. Notice how the three statements are separated with semicolons (`;`). As I told you in the previous section, Pascal also uses semicolons to separate the statements in a sequence of statements.

The first statement writes out a prompt asking for the user's name, using the `writeln` procedure.

The `readln` procedure in the second statement reads some text from the keyboard and puts it in the variable indicated in its argument. Therefore, when the user types `Jacobo` or whatever their name is, this `Jacobo` text string gets put in the variable `Who`.

In the next line, `writeln` gets three arguments: the string `'Hello, '`, the variable `Who`, and the string `'!'`. As you remember, `writeln` writes out the values of its arguments to the screen. For the first argument, it writes out `Hello, `; for the second argument, it writes out the content of the `Who` variable (`Jacobo`, in my example); finally, for the third argument, it writes out `!`. All together, that forms `Hello, Jacobo!`.

Finally, as in the previous example, the `end` keyword with a period at the end (`.`) marks the end of the program.

### Some things to try

This program does many more things than the first one, and it is so much more interesting! Here are some ideas for things you can try doing to it:

Try removing the variable declaration block. Does the program compile? What is the error message? What does the error message _mean_?

What happens if you try to use a variable that you haven't declared?

So far, we've seen calls to `writeln` with one argument and with three arguments. How many arguments can `writeln` take? Can it take two? Four? Twenty? Zero? How do you even pass zero arguments to `writeln`? Do you need to use parentheses if you have zero arguments? What even _happens_ if you pass zero arguments to `writeln`?

Can you modify the program so it will also ask where the user is from? You may need to add another variable of type `string`. How do you think you can add another variable? Will semicolons be involved? Do you need to use the keyword `var` twice, or can you get away with using it only once? (Hint: I said that the keyword `var` _begins_ a variable declaration block.)

Do you need to have each statement in a separate line? How can you join them? Do you need to have each statement in a single line? Where can you split them?
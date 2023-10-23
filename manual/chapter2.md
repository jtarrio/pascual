# How to Program in Pascal

If you want to learn how to write programs in the Pascal language using the Pascual compiler, this chapter is for you.

It doesn't matter whether you are new to programming or an expert in other languages; this chapter has everything you need to know to get started.

This chapter is not a complete Pascal course; it leaves many things unsaid. Read the following chapters to learn the language in depth.

## A little history of Pascal

Pascal is a programming language that was released in 1970 by Niklaus Wirth. He built it upon the ALGOL-60 language and designed it to encourage good programming practices through structured programming and data structures.

Pascal was modestly successful until Borland released Turbo Pascal, an affordable Pascal compiler for home computers that was extremely fast and came with an Integrated Development Environment (IDE). Its popularity grew as Borland released newer and more powerful versions; the authors of many games and applications from the early 90s chose Turbo Pascal and Borland Pascal to write them. In 1995, Borland released Delphi, the dialect of Pascal most people are familiar with nowadays.

As for Niklaus Wirth, Pascal wasn't his first or last language. Before Pascal, he had developed Euler, PL360, and ALGOL W; after Pascal, he worked on Modula and Modula-2. In 1984, he won the Turing Award (the highest distinction in computer science), but he didn't stop there and went on to develop the Oberon family of languages and operating systems.

## Pascal and Pascual

The Pascal language is a set of rules for writing and organizing a program. It tells you how to write a number, how to write a character string, how to write an assignment statement, a conditional statement, etc. When you write a program following those rules, we say that you wrote this program in the Pascal language.

A Pascal compiler is a program that takes another program written in the Pascal language and converts it into a form that the computer can execute.

There are many Pascal compilers, and they all work slightly differently from each other. Computers have changed and evolved a lot since Wirth first published the rules for the Pascal language, and different compilers adopted somewhat different rules to write programs for the new things computers can do. However, there is a "core" Pascal language that most (if not all) compilers agree on.

Pascual is a Pascal compiler with some quirks of its own, of course. Therefore, in this chapter, when I talk about "Pascal," I refer to the Pascal language, especially the parts that most compilers agree on. When I talk about "Pascual," I refer to the Pascual compiler or the details of the language that are specific to Pascual.

## Preparing the computer

Before we try to write our first program in Pascal, we need to set up our development environment. Follow the instructions in [chapter 1](chapter1.md) to download and build Pascual. Afterward, you can write your programs in the `pascual` or `pascual-main` directory and use the `dist/pascualc` command to compile them.

## The first program

Traditionally, when a programmer wants to try out a language, they write a program that writes out the text "Hello world;" let's do that!

Type out the following program into a file named `helloworld.pas` (you _could_ copy and paste, but I find that typing out example programs by hand gives me a better feel for the language and helps me learn better):

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

That's it! That's your first program. Now, if you want, you can modify it to see what other things you can make it say; write the text you want between the single quote signs (`'`), then compile and run again.

I am now going to explain what's in this program. Since it is our first program, the explanation will be a bit long, but don't worry: it may be long, but it's easy.

The first line declares the program's name. You can write any name you want, as long as it's a single word; so you could write `program Hello` or `program JacoboSaysHello` or `program ThisIsMyFirstProgram`, but not `program I have no idea what I am doing`.

In some old computers, programs needed to declare their name, but that's unnecessary in modern ones. However, Pascal still requires it, so we must do it. Even though most compilers ignore it, writing a real name is a good practice so that someone reading your source code can look at it and know what the program is about.

Notice the semicolon (`;`) at the end of the line. It separates the `program` definition from what comes after. Pascal uses semicolons as separators between the different parts of a program and between the statements in a sequence. It is common to forget to type a semicolon where it's required!

The `begin` keyword in the following line tells the compiler that this is the beginning of the program's code. The following lines until the `end` keyword contain _statements_ that the program will execute.

This program only contains one statement, which causes the string `Hello world!` to be written out on the screen. The `writeln` procedure does the job.

In Pascal, a procedure is a type of subroutine. A subroutine is a code segment with a name that can be invoked (or _called_) from different points in the program. It's like having a mini-program that the program can execute several times with different parameters. In Pascal, you specify the parameters for the procedure between parentheses right after the procedure's name.

Pascal compilers provide many subroutines our programs can invoke to do useful things. Programmers can also define their subroutines, which I'll show you how to do later in this chapter. For now, in this program, we call the `writeln` procedure, which our compiler provides, with the parameter `'Hello world!'`, and this procedure writes out the value of its parameter to the screen.

What is the value of the parameter? It appears enclosed by a pair of single quotes (`'`), which tells the compiler it is a character string. Therefore, the value of the parameter `'Hello world!'` is the character string `Hello world!`, which is what the `writeln` procedure writes out on the screen!

And with this, we reach the `end` keyword with a period (`.`) at the end, which marks the end of the program.

### Some things you can try

Try making a few changes to this program before continuing to the next section. Get a feel for the language! Here are a few things you can try:

The Pascal language is case-insensitive (that's computerese for "it doesn't care whether you use capital letters or small letters"). Therefore, it is all the same whether you write `program`, `PROGRAM`, or `pRoGrAm`. Try changing some of the keywords to upper case (that's computerese for "capitals") like in `BEGIN` or `WRITELN`, or capitalize the first letter only like in `Begin` or `Writeln`. Which option looks most pleasing to you? Different people have different opinions.

How about the character string between the single quotes? Does it make any difference to write `'Hello world!'` or `'HELLO WORLD!'`?

Try removing the semicolon at the end of the `program` line or the period at the end of the `end` line. Does the program compile, or does Pascual give you an error message? What does the error message look like?

Try declaring different program names. I already told you it needs to be a single word, but try adding numbers, like `program Number1`, or `program 1stProgram`. Try some symbols, like the underscore (`_`) or the hyphen (`-`). What works? What does not work?

This program occupies four lines. What happens if you mash everything together in a single line? What if you split a line in two? Does it matter where you break it?

Feel free to play around as much as you like. You won't break anything; at most, Pascual will give you an error message. So make changes, try them, read the error messages, try to fix them, and have fun!

## Variables

The "Hello World" program is the traditional first program, but it is not very interesting: it only contains one statement and always does the same thing. Therefore, let's write a slightly more complex example.

Type out the following program and save it into a file named `helloname.pas`:

```pascal
program HelloName;
var Who : string;
begin
  writeln('What is your name?');
  readln(Who);
  writeln('Hello, ', Who, '!')
end.
```

When you compile and run this program, it will ask what your name is. After you write it, the program will greet you by name!

```
$ dist/pascualc helloname.pas
$ ./helloname
What is your name?
Jacobo
Hello, Jacobo!
$
```

There are a couple of differences from the previous program that you may notice immediately.

The first is the line starting with the `var` keyword. That keyword begins a _variable declaration block_.

A variable is a portion of memory the program uses to store data. You can read data from variables and modify their content, so you can use them to store a value (either provided by the user or read from a file or the network), the results of calculations, intermediate steps, or generally anything that is not hardcoded into the program.

In the Pascal language, you must declare variables before using them. When you do it, you give the variable a name so the program can refer to it and access its content. You also assign a type so the compiler knows how much memory it needs to reserve for the variable, how to represent its content in memory, and what operations are allowed on it.

This program declares a single variable, named `Who`, of type `string`, which can hold a character string.

The other difference is that, between the keywords `begin` and `end`, there are three statements instead of just one: a call to the `writeln` procedure, a call to `readln`, and, finally, another call to `writeln`. Also, notice how the three statements form a sequence and appear separated with semicolons (`;`).

The first statement uses the `writeln` procedure to write a prompt asking for the user's name.

The `readln` procedure in the second statement reads some text from the keyboard and puts it in the variable indicated in its parameter. Therefore, when the user types `Jacobo` or whatever their name is, this `Jacobo` text string gets stored in the variable `Who`.

In the third line, `writeln` receives three parameters: the string `'Hello, '`, the variable `Who`, and the string `'!'`. As you remember, `writeln` writes the values of its parameters to the screen. For the first parameter, it writes out `Hello, `; for the second parameter, it writes out the content of the `Who` variable (`Jacobo`, in my example); for the third parameter, it writes out `!`. All together, that forms `Hello, Jacobo!`.

Finally, as in the previous example, the `end` keyword with a period at the end (`.`) marks the end of the program.

### Some things you can try

This program does many more things than the first one and is more interesting! Here are some ideas you can try on it:

Try removing the variable declaration block. Does the program compile? What is the error message? What does the error message _mean_?

What happens if you try to use a variable you didn't declare beforehand?

So far, we've seen calls to `writeln` with one and three parameters. How many parameters can `writeln` take? Can it take two? Four? Twenty? Zero? How do you even pass zero parameters to `writeln`? Do you need to use parentheses if you have zero parameters? What even _happens_ if you pass zero parameters to `writeln`?

Can you modify the program so it will also ask where the user is from? You may need to add another variable of type `string`. How do you think you can add another variable? Will semicolons be involved? Do you need to use the `var` keyword twice, or can you get away with using it only once? (Hint: I said the `var` keyword _begins_ a variable declaration block.)

Do you need to have each statement in a separate line? How can you join them? Do you need to have each statement in a single line? Where can you split them?

## If-then-else

So far, I've only shown you programs that always do the same thing. Yes, the second one will display different names if you type other names into it, but it will always execute the same statements no matter what.

There is a way to let your program execute some statements or others depending on a condition: the `if` statement, which I show you in the following example:

```pascal
program PasswordGame;
var Pass : string;
begin
  writeln('In an alleyway, a painted steel door tries to blend with the wall.');
  writeln('After you knock on the door, a voice asks:');
  writeln('"What is the password?"');
  readln(Pass);
  if Pass = 'swordfish' then
    writeln('"That''s right, but we''re full tonight," the voice says.')
  else
    writeln('"Go away!" the voice says.');
  writeln('You knock on the door again, but it remains stubbornly shut.')
end.
```

In this excerpt from an adventure game set during Prohibition, you, as the detective, approach a locked steel door, knock on it, and depending on whether you know the password or not, you get one response or another.

The "if-then-else" statement provides this ability to do one thing or another. This statement takes three things: a condition to check, a statement to execute if the condition is true, and a statement to execute if the condition is false.

In this example, the condition is `Pass = 'swordfish'`: it is true if the variable `Pass` contains a value equal to the string `swordfish`. If so, the program executes the statement after the `then` keyword; otherwise, it executes the one that appears after the `else` keyword.

Try it! Compile and run the program, and give it the correct password once and an incorrect password in the next run. See the difference?

If you keep track of where the semicolons go, you will notice the absence of one inside the "if-then-else" statement. That's because it's all a single statement, so there is nothing to separate:

```pascal
if condition then statement_true else statement_false
```

If you added a semicolon between the `statement_true` and the `else`, the compiler would think that the `else` belonged to a different statement, giving you an error message when you tried to compile it.

You can have "if-then-else" statements inside other "if-then-else" statements. In particular, "if-then-else if" is a typical pattern for checking several conditions, one after the other, when you want to do something for the first one that matches.

```pascal
if Pass = 'swordfish' then
  writeln('"That''s right, but we''re full tonight," the voice says.')
else if Pass = 'sturgeon' then
  writeln('"Welcome to the... HEY, YOU ARE NOT THE GOVERNOR!" the voice says.')
else if Pass = 'hammerhead' then
  writeln('"The service entrance is in the front," the voice says.')
else
  writeln('"Go away!" the voice says.');
```

There are two more things I want to say about "if-then-else": the first one is that the `else` part is not mandatory. If there is a statement you want to execute if the condition is true, but you don't have anything to do for a false condition, you can write an "if-then" statement:

```pascal
if Pass = 'swordfish' then
  writeln('"That''s right, but we''re full tonight," the voice says.');
writeln('You knock on the door again, but it remains stubbornly shut.')
```

The second thing is quite important, so let me say it in a new paragraph:

Wherever Pascal wants you to write one statement, you can write a sequence of several statements if you enclose them between a pair of `begin`/`end` keywords. In Pascal's eyes, the `begin` and `end` keywords turn the sequence of statements into a single one.

Why is it so important? Because it lets you execute more than one statement if the condition is true or false and because this is useful in many places other than "if-then-else" statements.

Let's put this into practice. Imagine you don't know about using `begin`/`end` and want to change the game so that if the player guesses the password, the door opens and the player gets a glimpse of the inside:

```pascal
if Pass = 'swordfish' then
  writeln('The door opens with a click. "Welcome to Horse Feathers."');
  writeln('There is a thick curtain in front of you. As the doorman pulls it');
  writeln('aside, you see a wood-paneled room with a bar in the middle.');
  writeln('A bartender is pouring an unidentifiable liquid into a shot glass.');
  writeln('Before you can step in, a furious woman appears and kicks you out.');
  writeln('"Don''t let him in, Johnnie! He reeks of cop!" she shouts.');
  writeln('The door closes and another click announces it''s been locked.')
else
  writeln('"Go away!" the voice says.');
writeln('You knock on the door again, but it remains stubbornly shut.')
```

The code in this example would not compile because the compiler would think that the first `writeln` was the single statement that goes with the "if-then" statement, and then it would be shocked to find a single `else` keyword later in the source code. The proper way to write this is by enclosing the sequence of `writeln` statements in a pair of `begin`/`end` keywords:

```pascal
if Pass = 'swordfish' then
begin
  writeln('The door opens with a click. "Welcome to Horse Feathers."');
  writeln('There is a thick curtain in front of you. As the doorman pulls it');
  writeln('aside, you see a wood-paneled room with a bar in the middle.');
  writeln('A bartender is pouring an unidentifiable liquid into a shot glass.');
  writeln('Before you can step in, a furious woman appears and kicks you out.');
  writeln('"Don''t let him in, Johnnie! He reeks of cop!" she shouts.');
  writeln('The door closes and another click announces it''s been locked.')
end
else
  writeln('"Go away!" the voice says.');
writeln('You knock on the door again, but it remains stubbornly shut.')
```

Another interesting thing I didn't mention: check out the text displayed when you give the correct password. In particular, look at the line where the furious lady shouts at the doorman. She says "don't," but I didn't write it as `don't` but as `don''t`. Why do you think I did it that way? What appears on the screen when you run the program?

## Loop statements

Programs rarely start at the beginning and go straight through to the end. Quite often, they execute some parts over and over until something happens. Imagine a program that waits until you press a key: it's going to check if you pressed a key, and if you didn't, it's going to check again, and again, and again, until it detects that you've finally pressed a key.

We call this kind of construct (where one or more statements repeat) a _loop statement_, and Pascal has three of them: "repeat-until," "while-do," and "for."

### Repeat-until

The first loop statement is called "repeat-until," and I will show you how to use it in a guess-the-number game:

```pascal
program GuessMyNumber;
var
  Number : integer;
  Guess : integer;
  Tries : integer;
begin
  Tries := 0;
  Randomize;
  Number := Random(100) + 1;
  writeln('I''ve chosen a number between 1 and 100.');
  writeln('Try to guess it in 10 tries or less!');
  repeat
    Tries := Tries + 1;
    write('Attempt ', Tries, ': What is your guess? ');
    readln(Guess);
    if Guess < Number then
      writeln('Go higher!')
    else if Guess > Number then
      writeln('Go lower!')
  until (Guess = Number) or (Tries = 10);
  if Guess = Number then
    writeln('You guessed it in ', Tries, ' tries! It was ', Number, '!')
  else
    writeln('Sorry, you didn''t guess the number. It was ', Number)
end.
```

This program contains lots of exciting new stuff! The first one is that we declare three variables, all of type `integer`. `integer` variables hold whole numbers, so they are the right choice for a guess-the-number game. The three variables are `Number`, which will store the number to guess; `Guess`, which will hold the player's latest guess; and `Tries`, which will keep the count of times the user has tried so far.

In the first statement, the symbol `:=` means that it is an assignment statement; it will assign the result of computing the expression on the right of the `:=` to the variable on the left. Therefore, it will give the value `0` to the `Tries` variable.

(People who are used to other programming languages often forget to write `:=` and write `=` instead, which results in an "invalid statement" error message from the compiler. In Pascal, `=` is always a comparison operator, while `:=` is always an assignment operator. It is supposed to remind you of the shape of an arrow, as in `Tries ⇐ 0`.)

The second statement is a call to the `Randomize` procedure, which takes no parameters and, therefore, doesn't need any parentheses after it. The `Randomize` procedure initializes the random number generator. If you don't know what this means, it's a little hard to explain, but without a call to `Randomize`, the program would always have you guess the same number, and it wouldn't be a fun game for very long.

The third statement is the one that chooses a number at random. It is an assignment statement, like before, which assigns the result of computing `Random(100) + 1` to the variable `Number`.

So, what is `Random(100) + 1`? It is an arithmetic expression. As you can imagine, the `+` means that it is an addition: its value will result from adding 1 to the result of `Random(100)`.

As for `Random`, it is a function: another type of subroutine. Like a procedure, it has a name and can receive parameters, but unlike a procedure, it returns a value that you can use in an expression. The `Random` function returns a random integer between 0 and its parameter minus one; since the parameter is 100 in our program, it produces a number between 0 and 99.

With this, we have everything we need to know what the third statement does: it gets a random value between 0 and 99 through the `Random` function, adds 1 to make it between 1 and 100, and assigns the result to the `Number` variable.

After this, we have a couple of `writeln` procedure calls, and we get to the heart of this section: the "repeat-until" loop statement.

This statement has a `repeat` keyword and a sequence of statements followed by an `until` keyword and a condition. It will execute the sequence of statements and then check the condition. If it is not true, it will loop: rerun the statements and recheck the condition; if it is still not true, it will loop again and again until, finally, the condition becomes true.

In this example, the condition is `(Guess = Number) or (Tries = 10)`; therefore, it will execute the statements between the `repeat` and `until` keywords until the player guesses the number or until they have tried 10 times.

Let's now look at what's inside the "repeat-until" block. First, we add 1 to the value of `Tries`, and then we ask the player for their guessed number, which goes into the `Guess` variable. Next, we use "if-then-else if" to check if the player's guess is lower or higher than the number and tell them to go higher or lower, as appropriate. Finally, we reach the `until` keyword, which will decide whether we execute the statements again or leave the loop.

Finally, we check whether the user guessed the number or reached the maximum number of tries and write the appropriate message.

#### Other things to know and try

Notice how we use the operators `>`, `<`, and `=` to check if one number is greater than, smaller than, or equal to another. You might be interested to know that we also have operators for "greater than or equal to" (`>=`), "smaller than or equal to" (`<=`), and "not equal to" (`<>`).

Also, look at the `until` condition: we use an `or` operator to check if either one of the comparisons is true. What would you say if I told you that Pascal also has an `and` operator to check if both are true simultaneously?

In one line, this program uses the procedure `write` instead of the `writeln` we had already seen. Both procedures write their parameters to the screen, but there is a difference. What do you think it is? Try replacing some `writeln` procedure calls with `write` and vice versa to see if you notice a pattern.

### While-do

The second loop statement in the Pascal language is "while-do," which is the opposite of "repeat-until." Where "repeat-until" first executes the statements and then checks if it should stop looping, "while-do" first checks the condition, and if it is true, it runs the statement and then loops.

Since "while-do" is the opposite of "repeat-until," let's see it in action in a program opposite to the game in the previous section: this time, the player chooses a number, and the computer tries to guess it.

```pascal
program ComputerGuesses;
var
  Low, High, Guess : integer;
  Tries : integer;
  Answer : string;
  Guessed : boolean;
begin
  Low := 1;
  High := 100;
  Tries := 0;
  Guessed := false;
  writeln('Choose a number between 1 and 100!');
  writeln('I will guess it in 7 tries or less.');
  while not Guessed and (Low < High) do
  begin
    Tries := Tries + 1;
    Guess := (Low + High) div 2;
    writeln('Is your number ', Guess, '?');
    repeat
      write('Answer (yes/higher/lower): ');
      readln(Answer)
    until (Answer = 'yes') or (Answer = 'higher') or (Answer = 'lower');
    if Answer = 'yes' then Guessed := true
    else if Answer = 'higher' then Low := Guess + 1
    else if Answer = 'lower' then High := Guess - 1
  end;
  if Guessed then writeln('Your number is ', Guess)
  else writeln('Your number is ', Low);
  writeln('I guessed your number in ', Tries, ' tries')
end.
```

As usual, this program starts by declaring its variables, and we can see something new here: we declare one variable with the type `boolean`. Instead of numbers or strings, Boolean variables hold "true" and "false" values. That's the same type as the conditional expressions I talked about earlier in this chapter, so you can also store the result of one of those expressions in a Boolean variable.

Also, look at how the three variables, `Low`, `High`, and `Guess` are being declared as `integer` simultaneously.

After a couple of variable assignments and `writeln` statements, we reach the `while` keyword. Unlike "repeat-until," "while-do" takes only one statement to be looped, so if you want to use more than one statement (like in this example), you need to enclose them with `begin`/`end`.

The conditional expression for the `while` statement also has some new things. One is the `not` operator. It takes what comes after it (the variable `Guessed`) and negates it: if it is true, it makes it false, and if it is false, it makes it true. So when `Guessed` is false, `not Guessed` is true and vice versa. The other new thing is the `and` operator, which returns true when the expressions on both sides of the `and` are true.

The `while` statement starts by checking if the condition is true, and if so, it loops the following sequence of statements over and over until the condition evaluates to false.

The looped statements show a guess to the user, ask if they guessed correctly or if the chosen number is higher or lower, and adjust the next guess accordingly. We have already seen most of them: a few variable assignments, `write`, `writeln`, `readln`, "if-then-else if," "repeat-until," ... The only new thing is the expression that computes a new value for the `Guess` variable.

In the `Guess := (Low + High) div 2` statement, the `div` operator performs an _integer division_. That's a division operation discarding any remainder or decimals. So if `Low` is 1 and `High` is 100, the result of `Low + High` is 101, and `101 div 2` is `50`, not `50.5`.

As I said before, these statements keep looping until the user tells the program that it guessed the number (signified by the variable `Guessed` receiving the value `true`) or the program is sure that it knows the number (indicated by the variables `Low` and `High` having the same value). In the end, the program shows the guessed number.

#### Differences between "repeat-until" and "while-do"

The "repeat-until" and "while-do" statements are similar but have three crucial differences.

The first difference is how the program decides when to break the loop. For "repeat-until," the statements keep looping _until_ the condition is true. So, as long as it is false, it continues looping. For "while-do," the statement keeps executing _while_ the condition is true. So, as long as it is true, it continues looping.

The second difference is when the condition is checked. "Repeat-until" checks the condition _after_ executing the statements, and "while-do" checks it _before_.

The third difference is in how many statements each construct takes. "Repeat-until" takes a sequence of statements between the `repeat` and `until` keywords. "While-do" takes a single statement after the `do` keyword. (However, programmers often use `begin`/`end` to have a sequence of statements for a `while`, like we do for "if-then-else.")

So this is what a "repeat-until" looks like:

```pascal
repeat
  statement_1;
  statement_2;
  statement_n
until condition
```

And this is what a "while-do" looks like:

```pascal
while condition do statement;

{ or }

while condition do
begin
  statement_1;
  statement_2;
  statement_n
end
```

How would you choose between "repeat-until" and "while-do"? Generally, if you want to execute the looped statements at least once (for example, to initialize the variables used in the conditional expression), you would use a "repeat-until" loop since it only checks the condition after executing the statements.

```pascal
repeat
  IsWarning := CheckWarning;
  if IsWarning then DisplayWarning
until not IsWarning;
```

If you don't want to execute the loop statements if the condition is initially false, a "while-do" statement is appropriate.

```pascal
readln(Thumps);
while Thumps > 0 do
begin
  writeln('Thump!');
  Thump := Thump - 1
end
```

Sometimes, when the distinction is not as sharp as above, you can use one or the other depending on what makes your code easier to write.

### For

Pascal's third loop statement is the "for" statement. You can use it to count from one number to another, executing a statement for each value.

An example will make it easier to explain, so I will use the well-known "FizzBuzz" program.

```pascal
program FizzBuzz;
var Count, Limit : integer;
begin
  write('How many? ');
  readln(Limit);
  for Count := 1 to Limit do
  begin
    if (Count mod 3 = 0) and (Count mod 5 = 0) then
      write('fizzbuzz ')
    else if Count mod 3 = 0 then
      write('fizz ')
    else if Count mod 5 = 0 then
      write('buzz ')
    else write(Count, ' ')
  end;
  writeln;
end.
```

I will get it out of the way first: the `mod` operator is the modulo operator, which returns the remainder of a division. So `Count mod 3` returns the remainder of the division of `Count` and 3. If it equals 0, that means that `Count` is divisible by 3.

The `for Count := 1 to Limit do` statement means: assign the value `1` to `Count` and execute the statement; then assign `2` and execute; then `3` and so on until you finally reach `Limit`, execute the statement for the last time, and then exit the loop.

So if the user entered `7` as the value of `Limit`, the output of this program would be: `1 2 fizz 4 buzz fizz 7`.

In Pascal, the `for` statement can only count 1 at a time, up or down. If you want to count down, you need to use the `downto` keyword instead of `to`:

```pascal
program Countdown;
var Num : integer;
begin
  for Num := 10 downto 1 do
    write(Num, ' ');
  writeln('Ignition!')
end.
```

## Types

Types are an essential concept in Pascal. Each piece of data that a program generates or uses has a type. When you declare a variable, you assign it a type. When you write a number or a string in your program, that number or string also has a type.

The type determines what operations are allowed on a value, how to store it in memory, how to represent it in the source code and on the screen, and how much memory it uses.

We've seen some of the built-in Pascal types in this chapter. For example, `integer` is the type to which whole numbers such as `42`, `5`, `-12`, or `0` belong. You can do arithmetic operations on integers, and you can compare them.

Another type we've seen is `string`, which we use to hold text. You can specify `string` values in the source code by putting them between single quotes, like in `'Hello, world!'`. You can concatenate two strings, and you can also compare them.

The third type we've seen in this chapter is `boolean`, which may contain the values `false` or `true`. The result of a comparison is a value of type `boolean`, and you can operate Boolean values using logical operators (`and`, `or`, and `not`).

Pascal also has two other built-in types we haven't seen in this chapter: `real`, used for numbers with a decimal or fractional part, and `char`, used for single bytes.

Pascal programmers are not limited to those five types: they can define their own to use in their programs. This ability to define new types is commonplace in modern programming languages but was new when Wirth published Pascal in 1970.

### Enumerated types

Pascal's `boolean` type allows two values: `false` and `true`. You, too, can create your own _enumerated type_ that allows the values you specify. For example, you could create a type for cardinal directions that accepts the values `North`, `South`, `East`, and `West`:

```pascal
program EnumeratedTypes1;
var Direction : (North, South, East, West);
begin
  Direction := North;
  writeln('The direction is ', Direction);
  Direction := East;
  writeln('Now the direction is ', Direction)
end.
```

A variable that belongs to an enumerated type cannot take values from a different enumerated type:

```pascal
program EnumeratedTypes2;
var Month : (Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec);
    Day : integer;
    WeekDay : (Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday);
begin
  Month := Mar;
  Day := 24;
  WeekDay := Saturday;
  writeln('The date is ', Weekday, ', ', Month, ' ', Day);
  { WeekDay := Jul; <== This is not valid! }
end.
```

In this example, the variables `Month` and `WeekDay` belong to different enumerated types. You can assign values such as `Jan` or `Aug` to `Month` and values like `Monday` or `Thursday` to `Weekday`, but you cannot assign `Jul` to `WeekDay` because that value belongs to the wrong type for the variable.

You can compare two values from the same enumerated type. Their order is the same order in which you defined them.

```pascal
program EnumeratedTypeComparison;
var Rating1, Rating2 : (Terrible, Poor, OK, Good, Great);
    Improved : boolean;
begin
  Rating1 := Poor;
  Rating2 := Good;
  Improved := Rating1 < Rating2;
  writeln('Has improved: ', Improved)
end.
```

You can also use `for` loops to iterate through the values of an enumerated type. This program uses two `for` loops to show a list of all playing cards in a deck:

```pascal
program CardDeck;
var Suit : (Diamonds, Hearts, Clubs, Spades);
    Number : (Ace, Two, Three, Four, Five, Six, Seven,
              Eight, Nine, Ten, Jack, Queen, King);
begin
  for Suit := Diamonds to Spades do
    for Number := Ace to King do
      write(Number, ' of ', Suit, '   ');
  writeln
end.
```

Try this at home: can you define two enumerated types with some values allowed in both types?

### Arrays

Sometimes, our programs need to work with lists of values. We could do this by defining as many variables as list elements, but it would soon become unwieldy. Instead, Pascal lets us define _array types_.

An array is a series of values of a particular type. You can access the elements of the array (the values it contains) through an index.

The following program implements a sieve of Eratosthenes for all natural numbers up to 100 to demonstrate how to declare and use arrays:

```pascal
program SieveOfEratosthenes;
var IsPrime : array[2..100] of boolean;
    I, Multiple : integer;
begin
  for I := 2 to 100 do IsPrime[I] := true;
  for Multiple := 2 to 10 do
  begin
    I := Multiple * 2;
    while I < 100 do
    begin
      IsPrime[I] := false;
      I := I + Multiple
    end
  end;
  write('Primes: ');
  for I := 2 to 100 do
    if IsPrime[I] then write(I, ' ');
  writeln
end.
```

(You may have forgotten what a sieve of Eratosthenes is, so I'll remind you. You build one by making a list of all numbers from 2 to 100 and then strike out every number that's a multiple of 2 greater than 2, then every multiple of 3 greater than 3, then 4, 5, etc. When you finish, all the numbers remaining in the list are prime.)

The declaration for the `IsPrime` variable says it is an array of booleans numbered from 2 to 100. Each element of `IsPrime` tells whether the number corresponding to its index is prime.

This program has three `for` loops: the first sets all the elements of `IsPrime` to true, the second executes the sieve algorithm, and the third displays all the prime numbers in the sieve.

As you can see, you declare an array type using the syntax <code>array[_start_.._end_] of _type_</code>, where _start_ and _end_ are the first and last indices of the array and _type_ is the type for each element of the array.

(In most other programming languages, all arrays start at index 0 or 1, but you must specify the starting index in Pascal. This lets you use the starting index that's most convenient for your program; in this example, we used 2 because we don't need to check if 1 is prime.)

To access an array element, you use the syntax <code>_variable_[_index_]</code>, where _index_ is a valid index in the array. You can use a variable to specify the index, as in the `for` loops.

Try this at home: can you define arrays of arrays? How can you access an element in such a data type?

### Records

Sometimes, we don't need to manage multiple elements of the same type; instead, we need to manipulate several related values of different types. With Pascal, we can define _record types_ that let us hold and use all the related values together. Record types are helpful when we deal with structured data, such as entries in a file.

The different values contained in a record are called _fields_, each with a name and a type. Here is an example of a program that declares and uses a variable belonging to a record type:

```pascal
program RecordExample;
var Person : record
               FirstName : string;
               LastName : string;
               BirthDate : record
                 Year : integer;
                 Month : (Jan, Feb, Mar, Apr, Jun, Jul, Aug, Sep, Oct, Nov, Dec);
                 Day : integer;
               end;
             end;
begin
  Person.FirstName := 'John';
  Person.LastName := 'Smith';
  Person.BirthDate.Year := 2002;
  Person.BirthDate.Month := Apr;
  Person.BirthDate.Day := 28;
  writeln('Name: ', Person.FirstName, ' ', Person.LastName);
  writeln('Birth date: ', Person.BirthDate.Year, ' ', Person.BirthDate.Month, ' ', Person.BirthDate.Day)
end.
```

The variable `Person` is a record with three fields: `FirstName` and `LastName` (both of type `string`), and `BirthDate`, which is, in turn, another record with three fields.

As you can see, you can access a field through the <code>_variable_._fieldname_</code> syntax. To access fields inside a record-type field you can add more <code>._fieldname_</code> as needed.

Try this at home: you can combine all the custom types. You can build arrays of records, records with enumerated, array, or record fields, and arrays of arrays. How would you access an element in an array of records?

### Type declarations

In the previous examples, we defined our custom types with the variables that used them. This works if we only need to use the custom types with one or two variables, but it breaks down if we need to use them in several parts of the program because we cannot declare the same custom type twice.

To solve this problem, we can use _type declarations_ to define our types and give them names to use them elsewhere in our program. We put our type declarations in a _type declaration block_, which starts with the `type` keyword.

```pascal
program TypeDeclarations;
type TMonth = Month : (Jan, Feb, Mar, Apr, May, Jun,
                       Jul, Aug, Sep, Oct, Nov, Dec);
     TDate = record
               Year : integer;
               Month : TMonth;
               Day : integer;
     end;
     TPerson = record
                 Name : string;
                 Birthdate : TDate;
               end;
var People : array [1..3] of TPerson;
    Person : TPerson;
    I : integer;
begin
  People[1].Name := 'John Smith';
  People[1].Birthdate.Year := 1983;
  People[1].Birthdate.Month := Jun;
  People[1].Birthdate.Day := 3;
  People[2].Name := 'Mary Knight';
  People[1].Birthdate.Year := 1979;
  People[1].Birthdate.Month := Aug;
  People[1].Birthdate.Day := 19;
  People[3].Name := 'Pedro Ozores';
  People[1].Birthdate.Year := 2004;
  People[1].Birthdate.Month := Feb;
  People[1].Birthdate.Day := 29;
  for I := 1 to 3 do
  begin
    Person := People[I];
    writeln('Name: ', Person.Name, ',
            Birthdate: ', Person.Birthdate.Day, '-',
                          Person.Birthdate.Month, '-',
                          Person.Birthdate.Year)
  end
end.
```

This example declares three custom types (one array type and two record types) and gives them the names `TMonth`, `TDate`, and `TPerson`. Those names are then used in the variable definitions to refer to those types.

(The names of the types in this example start with the letter `T`, which is a custom among many programmers to avoid confusing type names with variable names.)

Note that there is an equals sign (`=`) between the type name and its definition, not a semicolon (`:`). Semicolons are for variable and field definitions, not for type definitions.

Try this at home: can you define a type that is just another name for `integer`? Can you define a type for a previously defined type? Can you declare a variable with the same name as a type?

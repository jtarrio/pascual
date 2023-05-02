# How to Program in Pascal

If you want to learn how to write programs in the Pascal language using the Pascual compiler, this chapter is for you.

It doesn't matter whether you don't know how to program or you know how to program in other languages; this chapter has everything you need to know to get you started.

After you know the basics, you should read the following chapters to learn the language in depth.

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

Traditionally, when a programmer wants to try out a language, they write a program that writes out the text "Hello world"; let's do exactly that!

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

In Pascal, a procedure is a type of subroutine. A subroutine is a piece of code with a name, that can be executed (or _called_) from different parts of the program. It's a bit like having a smaller program inside your larger program, that your larger program can execute several times with different parameters. The parameters to the procedure are specified between parentheses right after the name of the procedure to be called.

Pascal compilers provide many subroutines which our programs can execute to do lots of different useful things. Programmers can also define their own subroutines, which I'll show you how to do later in this chapter. For now, in this program, we call the `writeln` procedure, which our compiler provides, with the parameter `'Hello world!'`, and this procedure writes out the value of its parameter to the screen.

What is the value of the parameter? You can see that it is enclosed by a pair of single quotes (`'`), which tells the compiler that it is a character string. Therefore, the value of the parameter `'Hello world!'` is the character string `Hello world!`, which is exactly what the `writeln` procedure writes out to the screen!

And, with this, we reach the `end` keyword with a period (`.`) at the end, which marks the end of the program.

### Some things you can try

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
  writeln('What is your name?');
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

The `readln` procedure in the second statement reads some text from the keyboard and puts it in the variable indicated in its parameter. Therefore, when the user types `Jacobo` or whatever their name is, this `Jacobo` text string gets put in the variable `Who`.

In the next line, `writeln` gets three parameters: the string `'Hello, '`, the variable `Who`, and the string `'!'`. As you remember, `writeln` writes out the values of its parameters to the screen. For the first parameter, it writes out `Hello, `; for the second parameter, it writes out the content of the `Who` variable (`Jacobo`, in my example); finally, for the third parameter, it writes out `!`. All together, that forms `Hello, Jacobo!`.

Finally, as in the previous example, the `end` keyword with a period at the end (`.`) marks the end of the program.

### Some things you can try

This program does many more things than the first one, and it is so much more interesting! Here are some ideas for things you can try doing to it:

Try removing the variable declaration block. Does the program compile? What is the error message? What does the error message _mean_?

What happens if you try to use a variable that you haven't declared?

So far, we've seen calls to `writeln` with one parameter and with three parameters. How many parameters can `writeln` take? Can it take two? Four? Twenty? Zero? How do you even pass zero parameters to `writeln`? Do you need to use parentheses if you have zero parameters? What even _happens_ if you pass zero parameters to `writeln`?

Can you modify the program so it will also ask where the user is from? You may need to add another variable of type `string`. How do you think you can add another variable? Will semicolons be involved? Do you need to use the keyword `var` twice, or can you get away with using it only once? (Hint: I said that the keyword `var` _begins_ a variable declaration block.)

Do you need to have each statement in a separate line? How can you join them? Do you need to have each statement in a single line? Where can you split them?

## If-then-else

So far, I've only shown you programs that always do the same thing. Yes, the second one will display different names if you type different names into it, but it will always execute the same statements no matter what.

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

In this excerpt from an adventure game set during Prohibition, you as the detective approach a locked steel door, knock on it and, depending on whether you know the password or not, you get one response or another.

This is achieved through the "if-then-else" statement. This statement takes three things: a condition to check, a statement to execute if the condition is true, and a statement to execute if the condition is false.

In this example, the condition is `Pass = 'swordfish'`. That is: the condition is true if the variable `Pass` contains a value that's equal to the string `swordfish`. If so, the statement after the `then` keyword is executed; otherwise, the statement after the `else` keyword is executed.

Try it! Compile and run the program, and give it the correct password once and an incorrect password in the next run. See the difference?

If you are keeping track of where the semicolons go, you will notice that there isn't one inside the "if-then-else" statement. That's because it's all a single statement, so there is nothing to separate:

```pascal
if condition then statement_true else statement_false
```

If you added a semicolon between the `statement_true` and the `else`, Pascual would think that the `else` belonged to a different statement and it would give you an error message when you tried to compile it.

You can have "if-then-else" statements inside other "if-then-else" statements. In particular, "if-then-else if" is a very common pattern for when you are checking several conditions, one after the other, and you want to do something for the first one that matches.

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

There are two more things I want to say about "if-then-else": the first one is that the `else` part is not mandatory. If there is a statement you want to execute if the condition is true but you don't have anything for a false condition, you can just write an "if-then" statement:

```pascal
if Pass = 'swordfish' then
  writeln('"That''s right, but we''re full tonight," the voice says.');
writeln('You knock on the door again, but it remains stubbornly shut.')
```

The second thing is actually quite important, so let me say it in a new paragraph:

Wherever Pascal wants you to write one statement, you can actually write a sequence of several statements if you enclose them between a pair of `begin`/`end` keywords. This turns the sequence of statements into a single statement in Pascal's eyes.

Why is it so important? Because it lets you execute more than one statement if the condition is true or false, and because this is useful in many places other than "if-then-else" statements.

Let's put this into practice. Imagine you want to change the game so that, if the player guesses the password, the door is opened and the player gets a glimpse of the inside, and you don't know about using `begin`/`end`:

```pascal
if Pass = 'swordfish' then
  writeln('The door opens with a click. "Welcome to Horse Feathers."');
  writeln('There is a thick curtain in front of you. As the doorman pulls it');
  writeln('aside, you see a wood paneled room with a bar in the middle.');
  writeln('A bartender is pouring an unidentifiable liquid into a shot glass.');
  writeln('Before you can step in, a furious woman appears and kicks you out.');
  writeln('"Don''t let him in, Johnnie! He reeks of cop!", she shouts.');
  writeln('The door closes and another click announces it''s been locked.')
else
  writeln('"Go away!" the voice says.');
writeln('You knock on the door again, but it remains stubbornly shut.')
```

The code in this example would not compile, because Pascual would think that the first `writeln` was the single statement that goes with the "if-then" statement, and then it would be very surprised to find a single `else` keyword later in the source code. The proper way to write this is by enclosing the sequence of `writeln` statements in a pair of `begin`/`end` keywords:

```pascal
if Pass = 'swordfish' then
begin
  writeln('The door opens with a click. "Welcome to Horse Feathers."');
  writeln('There is a thick curtain in front of you. As the doorman pulls it');
  writeln('aside, you see a wood paneled room with a bar in the middle.');
  writeln('A bartender is pouring an unidentifiable liquid into a shot glass.');
  writeln('Before you can step in, a furious woman appears and kicks you out.');
  writeln('"Don''t let him in, Johnnie! He reeks of cop!", she shouts.');
  writeln('The door closes and another click announces it''s been locked.')
end
else
  writeln('"Go away!" the voice says.');
writeln('You knock on the door again, but it remains stubbornly shut.')
```

Another interesting thing I didn't mention above: look at the description of what happens when you give the correct password. In particular, look at the line where the furious lady shouts at the doorman. She says "don't", but I didn't write it as `don't` but as `don''t`. Why do you think I did it that way? What is written out to the screen when you run the program?

## Loop statements

Programs rarely start at the beginning and then go straight through to the end. Quite often, there are some parts that are executed over and over until something happens. Imagine a program that waits until you press a key: it's going to check if you pressed a key, and if you didn't, it's going to check again, and again, and again, until it detects that you've finally pressed a key.

This kind of construct (where one or more statements repeat) is called a _loop statement_, and Pascal has three of them: "repeat-until", "while-do", and "for".

### Repeat-until

The first loop statement is called "repeat-until", and I'm going to show you how it would be used in a guess-the-number game:

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

There are lots of interesting stuff in here! We can start with the fact that we are declaring three variables, all of type `integer`. Variables of type `integer` hold whole numbers, so they are definitely the right choice for a guess-the-number game. The three variables are `Number`, which will hold the number that needs to be guessed; `Guess`, which will hold the player's latest guess; and `Tries`, which will hold the number of times the user has tried so far.

In the first statement, the symbol `:=` means that it is an assignment statement; it will assign the value of the expression on the right of the `:=` to the variable on the left. So it will assign the value `0` to the `Tries` variable.

(People who are used to other programming languages often forget to write `:=` and write `=` instead, which results in an "invalid statement" error message from the compiler. In Pascal, `=` is always a comparison operator, while `:=` is always an assignment operator. It is supposed to remind you of the shape of an arrow, as in `Tries ⇐ 0`.)

The second statement is a call to the `Randomize` procedure, which takes no parameters and therefore doesn't need any parentheses after it. The `Randomize` procedure initializes the random number generator. It's a little hard to explain what this means, but without a call to `Randomize`, the program would always have you guess the same number, and it wouldn't be a fun game for very long.

The third statement is the one that chooses a number at random. It is an assignment statement, like before, which assigns the result of `Random(100) + 1` to the variable `Number`.

So, what is `Random(100) + 1`? It is an arithmetic expression. As you can imagine, the `+` means that it is an addition: its value will be the result of adding 1 to the result of `Random(100)`.

As for `Random`, it is a function. A function is another type of subroutine. Like a procedure, it has a name and can receive parameters, but unlike a procedure, it returns a value that can be used in an expression. The `Random` function returns a random integer between 0 and the value of its parameter minus one; therefore, in this program, it returns a number between 0 and 99.

And with this, we have everything we need to know what the third statement does: it gets a random value between 0 and 99 through the `Random` function, adds 1 so it will be between 1 and 100, and assigns the result to the `Number` variable.

After this we have a couple of `writeln` procedure calls and we get to the heart of this section: the "repeat-until" loop statement.

This statement has a `repeat` keyword and a sequence of statements followed by an `until` keyword and a condition. It will execute the sequence of statements and then check if the condition is true; if it is not true, it will loop: execute the statements again and check the condition; if it is still not true, it will loop over and over until, finally, the condition is true.

In this example, the condition is `(Guess = Number) or (Tries = 10)`; therefore, it will execute the statements between the `repeat` and `until` keywords until the player guesses the number or until they have tried 10 times.

So let's look at what's inside the "repeat-until" block. First of all, we add 1 to the value of `Tries`, and then we ask the player for their guess, which goes into the `Guess` variable. Using "if-then-else if", we check if the player's guess is lower or higher than the number and we tell them to go higher or lower, as appropriate. Finally, we reach the `until` keyword, which will decide whether we execute the statements again or we leave the loop.

Finally, we check whether the user guessed the number or reached the maximum number of tries without guessing it, and we write out the appropriate message.

#### Other things to know and try

Notice how we use the operators `>`, `<`, and `=` to check if one number is greater than, smaller than, or equal to another one. I thought you might be interested to know that we also have operators for "greater than or equal to" (`>=`), "smaller than or equal to" (`<=`), and "not equal to" (`<>`).

Also, look at the `until` condition: we use an `or` operator to check if either one of the comparisons is true. What would you say if I told you that Pascal also has an `and` operator to check if both are true at the same time?

In one line, this program uses the procedure `write` instead of the procedure `writeln` we had already seen. They both write their parameters out to the screen, but there is a difference. What do you think it is? Try replacing some `writeln` with `write` and vice versa to see if you notice a pattern.

### While-do

The second loop statement in the Pascal language is "while-do", and it kind of works in the opposite way as "repeat-until". Where "repeat-until" first executes the statements and then checks if it should stop looping, "while-do" first checks the condition and, if it is true, it executes the statement and then loops.

Since "while-do" is the opposite of "repeat-until", let's see it in action in a program that is the opposite of the game in the previous section: this time, the player chooses a number and the computer tries to guess it.

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

As usual, this program starts by declaring its variables, and we can see something new in here: one variable is being declared with the type `boolean`. Instead of numbers or strings, boolean variables hold "true" and "false" values. That's the same type as the conditional expressions that I talked about earlier in this chapter, so you can also store the result of one of those expressions in a boolean variable.

Also look at how the three variables, `Low`, `High`, and `Guess` are being declared as `integer` at the same time.

After a couple of variable assignments and `writeln` statements, we get to the `while` keyword. Unlike "repeat-until", "while-do" takes only one statement to be looped, so if you want to loop more than one statement (like in this example), you need to enclose them with `begin`/`end`.

The conditional expression for the `while` statement also has a couple of new things. One is the `not` operator. It takes what comes after it (the variable `Guessed`) and, if it is true, it makes it false, and if it is false, it makes it true. So when `Guessed` is false, `not Guessed` is true and vice versa. The other new thing is the `and` operator, which returns true when the expressions on both sides of the `and` are true.

The `while` statement starts by checking if the condition is true and, if so, it loops the following sequence of statements over and over until the condition evaluates to false.

The looped statements show a guess to the user and ask the user if they guessed correctly or they chosen number is higher or lower, and adjust the next guess accordinly. Most of them are things we have seen before: there are a few variable assignments, `write`, `writeln`, `readln`, "if-then-else if", "repeat-until", ... The only new thing is the expression that computes a new value for the `Guess` variable.

In the `Guess := (Low + High) div 2` statement, the `div` operator performs an _integer division_. That's a division operation discarding any remainder or decimals. So if `Low` is 1 and `High` is 100, the result of `Low + High` would be 101, and `101 div 2` would be `50`, not `50.5`.

As I said before, these statements keep looping until the user tells the program that it guessed the number (signified by the variable `Guessed` receiving the value `true`) or the program is sure that it knows the number (signified by the variables `Low` and `High` having the same value). At the end, the program shows the guessed number.

#### Differences between "repeat-until" and "while-do"

The "repeat-until" and "while-do" statements do very similar things, but they have three crucial differences.

The first difference is how the program decides when to break the loop. For "repeat-until", the statements keep looping _until_ the condition is true. So, as long as it is false, it continues looping. For "while-do", however, the statement keeps executing _while_ the condition is true. So, as long as it is true, it continues looping.

The second difference is when the condition is checked. For "repeat-until", the condition is checked _after_ executing the statements. For "while-do", it is checked _before_.

The third difference is in how many statements each construct takes. "Repeat-until" takes a sequence of statements between the `repeat` and `until` keywords. "While-do", however, takes a single statement after the `do` keyword. (However, it is common to use `begin`/`end` to have a sequence of statements for a `while`, like we do for "if-then-else".)

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

How would you choose between using "repeat-until" and using "while-do"? Mainly, depending on whether you need to execute the looped statements at least once (for example, to initialize the variables that are used in the conditional expression). If so, you would use a "repeat-until" loop statement, since it only checks the condition after executing the statements.

```pascal
repeat
  IsWarning := CheckWarning;
  if IsWarning then DisplayWarning
until not IsWarning;
```

If the loop statements must not be executed if the condition is false, a "while-do" statement is appropriate.

```pascal
readln(Thumps);
while Thumps > 0 do
begin
  writeln('Thump!');
  Thump := Thump - 1
end
```

Other times, the need to use one or other is not so clear; in that case, you would choose depending on what makes the surrounding code or the conditional expression easier to write.

### For

Pascal's third loop statement is the "for" statement. It is used to count from one number to another, executing a statement for each value in that count.

It will be easier to explain with an example, and for that, I'm going to use the very well-known "FizzBuzz" program.

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

I'm going to get it out of the way first so I can talk about the "for" loop: the `mod` operator is the modulo operator, which returns the remainder of a division. So `Count mod 3` would return the remainder of the division of `Count` and 3. If it is equal to 0, that means that `Count` is divisible by 3.

The `for Count := 1 to Limit` statement means: assign the value `1` to `Count` and execute the statement; then assign `2` and execute; then `3` and so on until you finally reach `Limit`, execute the statement for the last time, and then exit the loop.

So if the user entered `7` as the value of `Limit`, the output of this program would be: `1 2 fizz 4 buzz fizz 7`.

In Pascal, the `for` statement can only count 1 at a time, up or down. If you want to count down, you need to use the keyword `downto` instead of `to`:

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

Types are an important concept in Pascal. Every possible value you can use in a program belongs to a type, which tells the compiler how much memory that value takes up in memory, what operations can be done on the value and how to represent that value in the program, in memory, and on the screen.

So far we've seen variables defined as `string` and `integer`, but Pascal has three more built-in types: `boolean` (which handles true or false values), `real` (which handles floating-point numbers) and `char` (for single bytes in a `string`.)

Programmers can also create their own types to use in their programs. This is something we take for granted now, but it was new when Pascal came out.

### Enumerated types

For example, you can create an _enumerated type_, which can hold one value from a list of named values. Here is an example:

```pascal
program EnumeratedTypes;
var Direction : (North, South, East, West);
    Month : (Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec);
    Day : integer;
    WeekDay : (Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday);
begin
  Direction := North;
  writeln('The direction is ', Direction);
  Direction := East;
  writeln('Now the direction is ', Direction);
  Month := Mar;
  Day := 24;
  WeekDay := Saturday;
  writeln('The date is ', Weekday, ', ', Month, ' ', Day);
  { WeekDay := Jul;  // This is not valid! }
end.
```

In the example above, the variables `Direction`, `Month`, and `WeekDay` belong to different enumerated types. Each one can only have a value from its list of enumerated values.

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

This example shows you that you can compare two values from the same enumerated type, just like you can compare two numbers or two strings.

You can also iterate through the values in an enumerated type, as in this example:

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

### Arrays

Another custom type you can create is an _array_, which is a series of values of a particular type that you can access through an index. Arrays are useful when you need a list of values.

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

The example above is an implementation of a Sieve of Eratosthenes. The `IsPrime` variable contains an array of booleans, numbered from 2 to 100; each boolean tells whether the boolean for its corresponding number is prime. Then, the first `for` loop sets all the booleans to `true`, and the second `for` loop executes the sieve, setting to `false` all the multiples of 2, 3, 4, all the way to 10. Finally, the last `for` loop iterates through the array, checking if each number is prime, and writing it to the screen if so.

As in the example above, arrays can be defined with the syntax <code>array[_start_.._end_] of _type_</code>, where _start_ and _end_ are the first and last indexes of the array, and are accessed with the syntax <code>_variable_[_index_]</code>.

In other languages, arrays always start at index 0 or 1, but in Pascal you always define the start index. In this example we start at index 2 because we don't need to check if 1 is prime, so we don't need an array element for it.

### Records

Another common custom type is the _record_, which is a collection of values, possibly of different types, that are stored together. Records are useful when you deal with structured data, such as entries in a file.

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

In this example, the variable `Person` is a record that contains three fields: `FirstName` and `LastName` (both of type `string`), and `BirthDate`, which is itself a record with three fields. As you can see, a field is accessed through the <code>_variable_._fieldname_</code> syntax, adding more <code>._fieldname_</code> as needed if you want to access a record within a record.

You can combine all the custom types at will. For example, you can build an array of records as in this example:

```pascal
program CardDeck;
var People : array [1..3] of record
               Name : string;
               Age : integer;
              end;
    I : integer;
begin
  People[1].Name := 'John Smith';
  People[1].Age := 39;
  People[2].Name := 'Mary Knight';
  People[2].Age := 43;
  People[3].Name := 'Pedro Ozores';
  People[3].Age := 42;
  for I := 1 to 3 do
    writeln('Name: ', People[I].Name, ' ', People[I].Age)
end.
```

### Type declarations

In the previous examples we defined our custom types along with the variables that use them. That works well if we only need to use that type in one or two variables, but it breaks down if we need to use the same type in several variables in different parts of the program. Enumeration types cannot be defined twice, and if you try defining the same record twice, it will result in two different types that look exactly the same.

This example shows you what happens if you try to declare a `People` array of records and a `Person` variable holding a record of the same type as the `People` array:

```pascal
program DifferentTypesError;
var People : array [1..3] of record
               Name : string;
               Age : integer;
              end;
    Person : record
               Name : string;
               Age : integer;
              end;
    I : integer;
begin
  People[1].Name := 'John Smith';
  People[1].Age := 39;
  People[2].Name := 'Mary Knight';
  People[2].Age := 43;
  People[3].Name := 'Pedro Ozores';
  People[3].Age := 42;
  for I := 1 to 3 do
  begin
    { Compile error: cannot assign People[I] to Person because they 
      belong to different types, even if their definitions are identical. }
    Person := People[I];
    writeln('Name: ', Person.Name, ', Age: ', Person.Age)
  end
end.
```

In this example, the elements of the `People` array and the variable `Person` don't belong to the same type. They are both records, and they have the same fields with the same names and types; however, they were declared separately, so they are separate types.

If we want to use the same type for the elements of `People` and for the variable `Person`, we need to create a name for our custom type and use it when we declare the variables:

```pascal
program TypeDefinition;
type TPerson = record
                 Name : string;
                 Age : integer;
                end;
var People : array [1..3] of TPerson;
    Person : TPerson;
    I : integer;
begin
  People[1].Name := 'John Smith';
  People[1].Age := 39;
  People[2].Name := 'Mary Knight';
  People[2].Age := 43;
  People[3].Name := 'Pedro Ozores';
  People[3].Age := 42;
  for I := 1 to 3 do
  begin
    Person := People[I];
    writeln('Name: ', Person.Name, ', Age: ', Person.Age)
  end
end.
```

In this example, the keyword `type` starts a _type declaration block_. It is similar to a variable declaration block, but it gives names to types. Notice that it uses an equals sign (`=`) between the type name and its definition; this tells us that `TPerson` is not a variable of that type; _it is_ that type.

After the type `TPerson` is declared, it can be used in variable declarations to refer to that type.

(It is customary for declared type names to start with the letter `T` to avoid confusing them with variable names. If we had named the type `Person`, we wouldn't be allowed to have a variable named `Person` too.)


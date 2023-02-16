{ Some flow control structures. }
program flowcontrol;

{ if-then-else }
procedure ShowIf;
begin
  { A straightforward if-then-else }
  if true then writeln('true')
  else writeln('false');

  { Quite often, we omit the 'else' }
  if true then writeln('true');

  { You can also omit the 'then' if you need to }
  if true then
  else writeln('false');
  { You may be surprised to learn you can do this too }
  if true then;

  { Use begin-end if you need to execute several statements }
  if true then
  begin
    writeln('true');
    writeln('true dat')
  end
  else
  begin
    writeln('false');
    writeln('no really')
  end;

  { You can chain if-then-else. This is very common. }
  if true then writeln('true')
  else if true then writeln('true2')
  else writeln('false');

  { You can also nest if-then-else. }
  { You can use begin-end to make it clear which else goes with each then. }
  if true then
  begin
    if true then writeln('true')
    else writeln('false')
  end;

  { Otherwise, the rule is that an 'else' goes with the latest 'then'. }
  if true then
    if true then writeln('true')
  else writeln('false');
end;

{ repeat-until }
procedure ShowRepeatUntil;
var i : integer;
begin
  { Repeats the statements inside until the condition is true. }
  i := 0;
  repeat
    i := i + 1
  until i = 5;

  { You don't need to use begin-end, as they are implicit. }
  i := 0;
  repeat
    i := i + 1;
    writeln(i)
  until i = 5;

  { If you are not careful writing the condition, }
  { you may end up with an infinite loop. }
  i := 10;
  repeat
    i := i + 1
  until i = 5;

  { Of course you can do an empty block for whatever reason. }
  repeat
  until false;
end;

{ while }
procedure ShowWhile;
var i : integer;
begin
  { Repeats the following statement while the condition is true }
  i := 0;
  while i <> 5 do
    i := i + 1;

  { Use begin-end to execute a block of statements }
  i := 0;
  while i <> 5 do
  begin
    i := i + 1;
    writeln(i)
  end;

  { Can you have an empty while? You can have an empty while }
  while false do;
end;

{ for }
procedure ShowFor;
var i : integer;
begin
  { You can count up }
  for i := 1 to 10 do
    writeln(i);

  { And you can count down }
  for i := 10 downto 1 do
    writeln(i);

  { You can use a block }
  for i := 1 to 10 do
  begin
    write('The number is ');
    writeln(i)
  end;

  { The ends of the 'for' loop are evaluated only once, so you can do this: }
  i := 10;
  for i := 1 to i do
    writeln(i);
  { It will loop up to 10, which was the value of i before the loop. }

  { If the initial value for the 'for' is already past the final value, }
  { the loop is not executed and the variable doesn't get the initial value. }
  i := 0;
  for i := 20 to 10 do
    writeln(i);
  writeln('The value of i is 0, not 20: ', i)
end;

{ case }
procedure ShowCase;
var i : char;
begin
  i := 'b';

  { Jumps to the statement associated with the label for the value of i }
  { and only executes that statement (no fallthrough). }
  case i of 
    'a': writeln('a');
    'b': writeln('b');
    'c': writeln('c');
  end;

  { If there isn't a label for the current value, no block will be executed. }
  { You can use an 'else' clause to specify one. }
  case i of 
    'd' : writeln('d');
    'e' : writeln('e');
    else writeln('other')
  end
end;

begin
  ShowIf;
  ShowRepeatUntil;
  ShowWhile;
  ShowFor;
  ShowCase
end.

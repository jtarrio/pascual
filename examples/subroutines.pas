{ Some things you can do with procedures and functions. }
program subroutines;

{ A simple procedure. }
procedure SayHello;
begin
  writeln('Hello!')
end;

{ Taking some arguments }
procedure Say(What : string; Exclamation : boolean);
begin
  write(What);
  if Exclamation then write('!');
  writeln
end;

{ By default, subroutine arguments are pass-by-value, which means that the }
{ subroutine gets a copy of the value you pass to it, and if you make any }
{ changes to the argument, those changes are only visible in the subroutine. }
{ If you want the changes to be visible outside of the subroutine, you can use }
{ pass-by-reference. Note that, with pass-by-reference, you cannot pass an }
{ immediate or ephemeral or constant value: it needs to be a variable. }
procedure Swap(var a, b : integer);
var c : integer;
begin
  c := a;
  a := b;
  b := c
end;

{ If you are passing strings or large data structures, you can avoid making }
{ copies using pass-by-const-reference. It is like pass-by-reference, but }
{ the procedure cannot modify the content of the argument, and you can pass }
{ ephemeral values to the subroutine, since Pascual makes a copy if needed. }
procedure SayConstRef(const What : string);
begin
  writeln(What)
end;

{ Functions are like procedures, but they return a value. }
function Add(a, b : integer) : integer;
begin
  Add := a + b
end;

{ The classic Pascal way to set the return value is to assign it to a }
{ pseudovariable named after the function itself, like above. }
{ In some modern Pascal dialects (and Pascual), you can assign it to a }
{ predefined variable named Result. }
{ The advantage of this is that Result is a real variable, so you can }
{ read and write from it. }
function Multiply(a, b : integer) : integer;
begin
  Result := a;
  Result := Result * b
  { If you tried doing this in the Add function above, Pascual would complain }
  { that you are trying to call the function without arguments. }
end;

{ Of course you can also use pass by reference and pass by const reference }
{ with functions, but I'm not going to show it because it's more of the same. }

{ You can call subroutines recursively. }
function Fib(n : integer) : integer;
begin
  if n <= 1 then Fib := 1
  else Fib := Fib(n - 1) + Fib(n - 2)
end;

{ Before you can call a subroutine, it needs to be declared. This can be a }
{ problem if you have two subroutines that call each other, as one of them }
{ must be declared before the other. This problem can be solved by }
{ forward declaring one of the subroutines before the other, and then }
{ defining it afterwards. }
procedure Second(Name : string; Level : integer);
forward;

procedure First(Name : string; Level : integer);
begin
  writeln('First ', Level, ' ', Name);
  if Level > 1 then Second(Name, Level - 1)
end;

procedure Second(Name : string; Level : integer);
begin
  writeln('Second ', Level, ' ', Name);
  if Level > 1 then First(Name, Level - 1)
end;

{ We don't have nested subroutines yet, but they will come. }

var
  x, y : integer;
begin
  { If a subrouting doesn't take any arguments, you can call it by just }
  { mentioning its name. }
  SayHello;
  { You can also use a pair of empty parentheses if you like. }
  SayHello();
  { Passing arguments. }
  Say('Hola', true);
  { Pass by reference does not change the syntax. }
  x := 1;
  y := 2;
  writeln('x=', x, ' y=', y);
  Swap(x, y);
  writeln('x=', x, ' y=', y);
  { Swap(1,2) would result in an error. }
  { But you can do it with pass by const reference. }
  SayConstRef('Hello');
  { By the way, the order of evaluation of subroutine arguments }
  { is not guaranteed. }

  { Functions are called like procedures, but inside expressions. }
  x := Add(y, 3);
  { And of course you can nest calls. }
  x := Add(y, Multiply(x, 2));

  writeln(Fib(10))
end.

(* Test for procedures and functions *)
program procedures;

var Num : integer;

procedure SayHello;
begin
  writeln('Hello!')
end;

procedure Display(Num : integer);
forward;

procedure Display(Num : integer);
begin
  writeln(Num)
end;

procedure Sum1(var Num : integer);
begin
  Num := Num + 1
end;

function Sum(N1 : integer; N2 : integer) : integer;
begin
  Sum := N1 + N2
end;

function SumInPlace(var N1 : integer; N2 : integer) : integer;
begin
  N1 := N1 + N2;
  SumInPlace := N1
end;

function Fibonacci(N : integer) : integer;
begin
  if N <= 1 then
    Fibonacci := 1
  else
    Fibonacci := Fibonacci(N - 1) + Fibonacci(N - 2)
end;

function Three : integer;
begin
  Three := 3
end;

begin
  SayHello;
  Display(Three);
  Num := 1;
  Display(Num);
  Sum1(Num);
  Display(Num);
  Display(Sum(Num, 1));
  Display(SumInPlace(Num, 2));
  Display(Num);
  for Num := 1 to 5 do Display(Fibonacci(Num))
end.

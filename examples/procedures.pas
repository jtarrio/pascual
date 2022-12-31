program procedures;

var Num : integer;

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

begin
  Num := 1;
  Display(Num);
  Sum1(Num);
  Display(Num);
  Display(Sum(Num, 1));
  Display(SumInPlace(Num, 2));
  Display(Num);
end.

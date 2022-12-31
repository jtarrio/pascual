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

begin
  Num := 1;
  Display(Num);
  Sum1(Num);
  Display(Num)
end.

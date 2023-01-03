(* Tests for flow control *)
program flowcontrol;

procedure TestIf;
begin
  writeln('if');

  if true then writeln('1 ok');
  if false then writeln('1 no');

  if true then writeln('2 ok')
  else writeln('2 no');
  if false then writeln('3 no')
  else writeln('3 ok');

  if true then
    if true then writeln('4 ok')
    else writeln('4 no');

  if true then
    if false then writeln('5 no')
    else writeln('5 ok');

  if true then;
  if true then else writeln('6 no');
  if false then else writeln('6 ok');
end;

procedure TestRepeat;
var
  i : integer;

begin
  writeln('repeat');

  i := 1;
  repeat
    writeln('1 ', i);
    i := i + 1
  until i > 5;
  writeln('1 expect 6: ', i);

  i := 5;
  repeat
    writeln('2 ', i);
    i := i + 1
  until i > 5;
  writeln('2 expect 6: ', i);

  i := 6;
  repeat
    writeln('3 ', i);
    i := i + 1
  until i > 5;
  writeln('3 expect 7: ', i);

  i := 7;
  repeat
    writeln('4 ', i);
    i := i + 1
  until i > 5;
  writeln('3 expect 8: ', i);

  repeat until true;
  repeat ; until true;
end;

procedure TestWhile;
var
  i : integer;
begin
  writeln('while');

  i := 1;
  while i <= 5 do
  begin
    writeln('1 ', i);
    i := i + 1
  end;
  writeln('1 expect 6: ', i);

  i := 5;
  while i <= 5 do
  begin
    writeln('2 ', i);
    i := i + 1
  end;
  writeln('2 expect 6: ', i);

  i := 6;
  while i <= 5 do
  begin
    writeln('3 ', i, ' no');
    i := i + 1
  end;
  writeln('3 expect 6: ', i);

  i := 7;
  while i <= 5 do
  begin
    writeln('4 ', i, ' no');
    i := i + 1
  end;
  writeln('4 expect 7: ', i);

  while false do;
end;

procedure TestFor;
var
  i : integer;
begin
  writeln('for');

  for i := 1 to 5 do
    writeln('1 ', i);
  writeln('1 expect 5: ', i);

  for i := 6 to 5 do
    writeln('2 ', i, ' no');
  writeln('2 expect 5: ', i);

  for i := 5 downto 1 do
    writeln('3 ', i);
  writeln('3 expect 1: ', i);

  for i := 0 downto 1 do
    writeln('4 ', i, ' no');
  writeln('4 expect 1: ', i);

  for i := 1 to 5 do;
end;

begin
  TestIf();
  TestRepeat();
  TestWhile();
  TestFor();
end.

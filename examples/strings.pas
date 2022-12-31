program strings;

procedure Literals;
begin
  writeln('a');
  writeln('ab');
  writeln('a' + 'b');
  writeln('ab' + 'c');
  writeln('a' + 'bc');
  writeln('ab' + 'cd');
  writeln('' + 'a');
  writeln('a' + '');

  writeln('a' = 'a');
  writeln('a' = 'b');
  writeln('a' = 'ab');
  writeln('ab' = 'a');
  writeln('ab' = 'ab');
  writeln('ab' = 'cd');
end;

procedure Variables;
var
  c : char;
  s : string;
  t : string;
begin
  c := 'a';
  writeln(c);

  s := 'b';
  writeln(s);
  s := 'cd';
  writeln(s);

  t := 'e' + s;
  writeln(t);
  writeln(s)
end;

function Concat(A : string; B : string) : string;
begin
  Concat := A + B
end;

procedure Append(var A : string; B : string);
begin
  A := A + B
end;

procedure Arguments;
var
  s : string;
begin
  writeln(Concat('a', 'cd'));
  s := 'abc';
  Append(s, 'def');
  writeln(s)
end;

begin
  Literals();
  Variables();
  Arguments()
end.

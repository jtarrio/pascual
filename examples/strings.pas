program strings;

procedure Literals;
begin
  writeln('''');
  writeln('foo''bar');
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
  writeln(s);

  s := 'ABC';
  write(s[1]);
  write(s[2]);
  write(s[3]);
  writeln();
  writeln(Length(s));
end;

procedure Manipulation;
var
  s : string;
  t : string;
begin
  s := '12345';
  t := copy(s, 2, 3);
  writeln('Expect 234 ', t);
  delete(s, 1, 3);
  writeln('Expect 45 ', s)
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

procedure Conversion;
var
  e : (One, Two, Three);
  s : string;
begin
  Str(One, s);
  writeln(s);
  Str(123, s);
  writeln(s);
  Str(True, s);
  writeln(s);
  Str('a', s);
  writeln(s);
  Str('abc', s);
  writeln(s)
end;

begin
  Literals();
  Variables();
  Manipulation();
  Arguments();
  Conversion()
end.

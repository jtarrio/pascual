program pointers;

type
  StrP = ^string;
  RecType = record
    foo : string;
    bar : integer
  end;
  RecP = ^RecType;

var
  foo : string;
  str : StrP;
  rec : RecP;

begin
  foo := 'foo';

  New(str);
  str^ := foo;
  writeln(str^);
  str^ := 'bar';
  foo := str^;
  writeln(foo);
  Dispose(str);

  New(rec);
  rec^.foo := 'foo';
  rec^.bar := 1;
  writeln(rec^.foo);
  writeln(rec^.bar);
  Dispose(rec)
end.

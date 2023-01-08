program cmdline;

var
  i : integer;

begin
  for i := 0 to ParamCount() do
    writeln(i, ': ', ParamStr(i))
end.

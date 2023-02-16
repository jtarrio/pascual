{ Accessing the command line arguments. }

program cmdline;

var
  i : integer;

begin
  { ParamStr(0) returns the program's name. }
  writeln('This program is: ', ParamStr(0));

  { ParamCount returns the number of arguments. }
  for i := 1 to ParamCount do
    { ParamStr(i) returns the i-th argument, 1-based. }
    writeln(i, ': ', ParamStr(i))
end.

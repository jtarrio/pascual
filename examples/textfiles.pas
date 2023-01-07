program textfiles;

var
  prog : text;

procedure outprog(var f : text);
var
  line : string;
begin
  while not eof(f) do
  begin
    readln(f, line);
    writeln(line)
  end;
  close(f)
end;

begin
  assign(prog, 'textfiles.pas');
  reset(prog);
  outprog(prog)
end.

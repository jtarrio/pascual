{ Example of reading and writing text files. }
program textfiles;

var infile, outfile : text;

procedure copyfile(var filein, fileout : text);
var
  line : string;
begin
  while not eof(filein) do
  begin
    readln(filein, line);
    writeln(fileout, line)
  end;
  close(filein);
  close(fileout)
end;

begin
  assign(infile, 'textfiles.pas');
  reset(infile);
  assign(outfile, '/tmp/textfiles-copy.pas');
  rewrite(outfile);
  copyfile(infile, outfile)
end.

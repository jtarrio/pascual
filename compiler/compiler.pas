program Pascual;

var Options : record
  SuppressWarnings : boolean;
end;

procedure CompileError(Msg : string);
forward;
procedure CompileWarning(Msg : string);
forward;

{$I typedefs.inc.pas}
{$I expression.forward.inc.pas}
{$I pseudofuns.forward.inc.pas}
{$I lexer.inc.pas}
{$I progdefs.inc.pas}
{$I codegen.defs.inc.pas}
{$I expression.inc.pas}
{$I parser.inc.pas}
{$I globals.inc.pas}
{$I codegen.inc.pas}
{$I pseudofuns.inc.pas}

procedure CompileError(Msg : string);
begin
  writeln(StdErr, Msg, LxWhereStr);
  halt(1)
end;

procedure CompileWarning(Msg : string);
begin
  if not Options.SuppressWarnings then
    writeln(StdErr, 'Warning: ', Msg, LxWhereStr)
end;

procedure Usage(Msg : string);
begin
  if Msg <> '' then
  begin
    writeln(Msg);
    writeln
  end;
  writeln('Usage:');
  writeln(ParamStr(0), ' input.pas [-o output.c] [-Wnone]');
  writeln;
  writeln('If you specify "-" as the input or output file, ',
          'stdin/stdout will be used.');
  writeln;
  writeln('Options:');
  writeln('   -Wnone    :- Suppress all warning messages.');
  halt(0)
end;

function ReplaceExtension(Str, Old, New : string) : string;
var 
  BaseLen, Pos : integer;
  Matches : boolean;
begin
  ReplaceExtension := '';
  BaseLen := Length(Str) - Length(Old);
  if BaseLen > 0 then
  begin
    Matches := true;
    for Pos := 1 to Length(Old) do
      Matches := Matches and (UpCase(Str[Pos + BaseLen]) = UpCase(Old[Pos]));
    if Matches then
      ReplaceExtension := Copy(Str, 1, BaseLen) + New
  end
end;

procedure ParseCmdline;
var 
  Pos : integer;
  InputFile, OutputFile : string;
  SuppressWarnings : boolean;
  Flag : (FlagNone, FlagOutput);
  Param : string;
begin
  InputFile := '';
  OutputFile := '';
  SuppressWarnings := false;
  Flag := FlagNone;

  for Pos := 1 to ParamCount do
  begin
    Param := ParamStr(Pos);
    if (Param[1] = '-') and (Param <> '-') then
    begin
      if Param = '-o' then Flag := FlagOutput
      else if Param = '-Wnone' then SuppressWarnings := true
      else if Param = '-h' then Usage('')
      else Usage('Unknown option: ' + Param)
    end
    else if Flag = FlagOutput then
    begin
      if OutputFile <> '' then Usage('Output file must be specified only once')
      else OutputFile := Param;
      Flag := FlagNone
    end
    else
    begin
      if InputFile <> '' then Usage('Input file must be specified only once')
      else InputFile := Param
    end
  end;

  if InputFile = '' then Usage('Input file must be specified');
  if OutputFile = '' then
  begin
    if InputFile = '-' then OutputFile := '-'
    else OutputFile := ReplaceExtension(InputFile, '.pas', '.c')
  end;
  if OutputFile = '' then Usage('Output file must be specified');

  if InputFile <> '-' then LxOpen(InputFile);
  if OutputFile <> '-' then CodegenSetOutput(OutputFile);
  Options.SuppressWarnings := SuppressWarnings
end;

procedure ClearState;
begin
  LxReset;
  CodegenReset;
end;

begin
  ClearState;
  ParseCmdline;
  StartGlobalScope;
  ParseProgram;
  CloseLocalScope;
  Close(Lexer.Input.Src);
  Close(Codegen.Output)
end.

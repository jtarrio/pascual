program dump_ast;

{$I ../types.inc.pas}
{$I ../utils.inc.pas}
{$I ../containers.inc.pas}
{$I ../bytebuffer.inc.pas}
{$I ../lexer/lexer.inc.pas}
{$I ../parser/errors.inc.pas}
{$I ../parser/ast.inc.pas}
{$I ../parser/parser.inc.pas}
{$I ../parser/ast_write.pas}

type 
    Options = record
        InputFileName: string;
        InputFile: text;
        OutputFileName: string;
        OutputFile: text;
        DisplayComments: boolean;
        DisplayNodeSource: boolean;
    end;

procedure Usage(Msg : string);
begin
    if Msg <> '' then
    begin
        writeln(Msg);
        writeln
    end;
    writeln('Usage:');
    writeln(ParamStr(0), ' input.pas [-o output.txt]');
    writeln;
    writeln('If you specify "-" as the input or output file, ',
            'stdin/stdout will be used.');
    writeln;
    writeln('Options:');
    writeln('   -c     : display comments.');
    writeln('   -s     : display every AST node''s source code.');
    halt(0)
end;

function OpenFile(name: string): text;
var f: text;
begin
    assign(f, name);
    reset(f);
    Result := f
end;

function ParseCmdline: Options;
var
    Pos : integer;
    InputFile, OutputFile : string;
    SuppressWarnings : boolean;
    Flag : (FlagNone, FlagOutput);
    Param : string;
    f : text;
begin
    Result.InputFileName := '';
    Result.OutputFileName := '';
    Result.DisplayComments := false;
    Result.DisplayNodeSource := false;

    for Pos := 1 to ParamCount do
    begin
        Param := ParamStr(Pos);
        if (Param[1] = '-') and (Param <> '-') then
        begin
            if Param = '-o' then Flag := FlagOutput
            else if Param = '-c' then Result.DisplayComments := true
            else if Param = '-s' then Result.DisplayNodeSource := true
            else if Param = '-h' then Usage('')
            else Usage('Unknown option: ' + Param)
        end
        else if Flag = FlagOutput then
        begin
            if Result.OutputFileName <> '' then Usage('Output file must be specified only once')
            else Result.OutputFileName := Param;
            Flag := FlagNone
        end
        else
        begin
            if Result.InputFileName <> '' then Usage('Input file must be specified only once')
            else Result.InputFileName := Param
        end
    end;

    if Result.InputFileName = '' then Usage('Input file must be specified')
    else if Result.InputFileName = '-' then Result.InputFile := input
    else Result.InputFile := OpenFile(Result.InputFileName);

    if Result.OutputFileName = '' then Result.OutputFile := output
    else if Result.OutputFileName = '-' then Result.OutputFile := output
    else Result.OutputFile := OpenFile(Result.OutputFileName);
end;

procedure ParseFile(const options: Options; var out_ast: TAst; var out_errors: TParseError);
var
    buffer : ByteBuffer;
    lexer : TLexer;
    parser : TParser;
begin
    buffer := ByteBuffer_FromTextFile(options.InputFile);
    lexer := TLexer_New(options.InputFileName, buffer);
    parser := TParser_New(lexer);
    out_errors := TParseError_New;
    out_ast := TParser_Parse_Program(parser, out_errors);
end;

procedure WriteOutput(const options: Options; const ast: TAst; var errors: TParseError);
var awo : TAstWriteOptions;
begin
    awo.ShowComments := options.DisplayComments;
    awo.ShowSource := options.DisplayNodeSource;
    if errors = nil then TAstWrite(options.OutputFile, ast, awo)
    else TParseError_DisplayList(errors, stderr)
end;

var
    opts: Options;
    ast: TAst;
    errors: TParseError;
begin
    opts := ParseCmdline;
    ParseFile(opts, ast, errors);
    WriteOutput(opts, ast, errors);
    TAst_Dispose(ast);
    TParseError_Dispose(errors);
end.
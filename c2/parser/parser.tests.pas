program parser_tests;

{$I ../types.inc.pas}
{$I ../utils.inc.pas}
{$I ../containers.inc.pas}
{$I ../bytebuffer.inc.pas}
{$I ../lexer/lexer.inc.pas}
{$I errors.inc.pas}
{$I ast.inc.pas}
{$I parser.inc.pas}
{$I ast_write.pas}
{$I ../tests.inc.pas}

procedure TestEmptyProgram;
var
    bb : ByteBuffer;
    lexer : TLexer;
    parser : TParser;
    ast : TAst;
    errors: TParseError;
begin
    bb := ByteBuffer_New(0);
    lexer := TLexer_New('testfile.pas', bb);
    parser := TParser_New(lexer);
    errors := TParseError_New;
    ast := TParser_Parse_Program(parser, errors);
    Assert(errors <> nil, 'expected non-empty error list');
    TAst_Dispose(ast)    
end;

procedure TestBasicProgram;
var
    bbb : ByteBufferBuilder;
    lexer : TLexer;
    parser : TParser;
    ast : TAst;
    errors: TParseError;
begin
    bbb := ByteBufferBuilder_New;
    ByteBufferBuilder_AddString(bbb, 'PROGRAM foo(input, output);'#10);
    ByteBufferBuilder_AddString(bbb, 'begin'#10);
    ByteBufferBuilder_AddString(bbb, 'end.'#10);
    lexer := TLexer_New('testfile.pas', ByteBufferBuilder_Build(bbb));
    parser := TParser_New(lexer);
    errors := TParseError_New;
    ast := TParser_Parse_Program(parser, errors);
    Assert(errors = nil, 'expected empty error list');
    TAstWrite(output, ast);
    TAst_Dispose(ast)    
end;

begin
    RunTest('TestEmptyProgram', @TestEmptyProgram);
    RunTest('TestBasicProgram', @TestBasicProgram);
end.

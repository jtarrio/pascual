{ Requires: ../containers.inc.pas }
{ Requires: ast.inc.pas }

const _TAst_Indent = '    ';

type TAstWriteOptions = record
    ShowComments : boolean;
    ShowSource : boolean;
end;

procedure _TAstWrite(f: text; ast: TAst; const indent: string; const options: TAstWriteOptions); forward;

procedure _TAstWrite__Location(f: text; ast: TAst; const Location: TBlock);
begin
    ByteBuffer_ToTextFile_Partial(ast^.Lexer^.Buffer, f, Location.Offset, Location.Size)
end;

procedure _TAstWrite__Content(f: text; ast: TAst);
begin
    _TAstWrite__Location(f, ast, ast^.Location)
end;

procedure _TAstWrite__Token(f: text; ast: TAst; const token: TToken);
begin
    _TAstWrite__Location(f, ast, token.Location)
end;

procedure _TAstWrite_ShowSource(f: text; const indent: string; ast: TAst);
begin
    write(f, indent, 'source: ');
    _TAstWrite__Content(f, ast);
    writeln(f)
end;

procedure _TAstWrite_Identifier(f: text; ast: TAst; const indent: string);
begin
    write(f, indent, 'identifier: ');
    _TAstWrite__Content(f, ast);
    writeln(f)
end;

procedure _TAstWrite_Program(f: text; ast: TAst; const indent: string; const options: TAstWriteOptions);
var Child : TAstList;
begin
    writeln(f, indent, 'program:');
    TList_First(ast^.Children, Child);
    while Child <> nil do
    begin
        _TAstWrite(f, Child^.Value, indent + _TAst_Indent, options);
        TList_Next(ast^.Children, Child)
    end
end;

procedure _TAstWrite_ProgramHeader(f: text; ast: TAst; const indent: string; const options: TAstWriteOptions);
var Child : TAstList;
begin
    writeln(f, indent, 'program_header:');
    if options.ShowSource then _TAstWrite_ShowSource(f, indent, ast);
    TList_First(ast^.Children, Child);
    while Child <> nil do
    begin
        _TAstWrite(f, Child^.Value, indent + _TAst_Indent, options);
        TList_Next(ast^.Children, Child);
    end
end;

procedure _TAstWrite_Comments(f: text; ast: TAst; const indent: string);
var Item : TCommentList;
begin
    TList_First(ast^.Comments, Item);
    while Item <> nil do
    begin
        write(f, indent, 'comment: ');
        _TAstWrite__Token(f, ast, Item^.value);
        writeln(f);
        TList_Next(ast^.Comments, Item)
    end
end;

procedure _TAstWrite(f: text; ast: TAst; const indent: string; const options: TAstWriteOptions);
begin
    if ast = nil then write(f, indent, 'nil')
    else
    begin
        if options.ShowComments then _TAstWrite_Comments(f, ast, indent);
        case ast^.AstType of
            AstIdentifier: _TAstWrite_Identifier(f, ast, indent);
            AstProgram: _TAstWrite_Program(f, ast, indent, options);
            AstProgramHeader: _TAstWrite_ProgramHeader(f, ast, indent, options);
        end
    end
end;

procedure TAstWrite(f:text; ast: TAst; const options: TAstWriteOptions);
begin
    _TAstWrite(f, ast, '', options)
end;

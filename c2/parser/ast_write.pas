{ Requires: ../containers.inc.pas }
{ Requires: ast.inc.pas }

const _TAst_Indent = '    ';

procedure _TAst_Write(f: text; ast: TAst; const indent: string); forward;

procedure _TAst_Write__Content(f: text; ast: TAst);
begin
    ByteBuffer_ToTextFile_Partial(ast^.Lexer^.Buffer, f, ast^.Location.Offset, ast^.Location.Size)
end;

procedure _TAst_Write__Token(f: text; ast: TAst; const token: TToken);
begin
    ByteBuffer_ToTextFile_Partial(ast^.Lexer^.Buffer, f, token.Location.Offset, token.Location.Size)
end;

procedure _TAst_Write_Identifier(f: text; ast: TAst; const indent: string);
begin
    write(indent);
    _TAst_Write__Content(f, ast);
    writeln
end;

procedure _TAst_Write_Program(f: text; ast: TAst; const indent: string);
var Child : TAstList;
begin
    writeln(f, indent, 'program:');
    TList_First(ast^.Children, Child);
    while Child <> nil do
    begin
        _TAst_Write(f, Child^.Value, indent + _TAst_Indent);
        TList_Next(ast^.Children, Child)
    end
end;

procedure _TAst_Write_ProgramHeader(f: text; ast: TAst; const indent: string);
var Child : TAstList;
begin
    writeln(f, indent, 'program_header:');
    TList_First(ast^.Children, Child);
    while Child <> nil do
    begin
        _TAst_Write(f, Child^.Value, indent + _TAst_Indent);
        TList_Next(ast^.Children, Child);
    end
end;

procedure _TAst_Write_Comments(f: text; ast: TAst; const indent: string);
var Item : TCommentList;
begin
    TList_First(ast^.Comments, Item);
    while Item <> nil do
    begin
        write(f, indent, 'comment: ');
        _TAst_Write__Token(f, ast, Item^.value);
        writeln;
        TList_Next(ast^.Comments, Item)
    end
end;

procedure _TAst_Write(f: text; ast: TAst; const indent: string);
begin
    if ast = nil then write(f, indent, 'nil')
    else
    begin
        _TAst_Write_Comments(f, ast, indent);
        case ast^.AstType of
            AstIdentifier: _TAst_Write_Identifier(f, ast, indent);
            AstProgram: _TAst_Write_Program(f, ast, indent);
            AstProgramHeader: _TAst_Write_ProgramHeader(f, ast, indent);
        end
    end
end;

procedure TAst_Write(f:text; ast: TAst);
begin
    _TAst_Write(f, ast, '')
end;

{ Requires ../types.inc.pas }
{ Requires ../containers.inc.pas }
{ Requires ../lexer/lexer.inc.pas }
{ Requires errors.inc.pas }
{ Requires ast.inc.pas }

type
    { A parser. }
    TParser = ^TParserObj;
    { The underlying object for the parser. }
    TParserObj = record
        Lexer : TLexer;
        Comments: TCommentList;
    end;

{ Creates a parser that reads from the given lexer.
  Ownership of the lexer passes to the parser. }
function TParser_New(Lexer: TLexer): TParser;
begin
    new(Result);
    Result^.Lexer := Lexer;
    TList_New(Result^.Comments)
end;

{ Releases the memory used by the given parser. }
procedure TParser_Dispose(var self: TParser);
begin
    TLexer_Dispose(self^.Lexer);
    Dispose(self);
    self := nil
end;

procedure _TParser_AddError(var self: TParser; var out_Errors: TParseError; const Message: string);
begin
    TParseError_Add(out_Errors, message, self^.Lexer, self^.Lexer^.Token.Location)
end;

procedure _TParser_AddComment(var self: TParser; const token: TToken);
var CommentList : TCommentList;
begin
    New(CommentList);
    CommentList^.Value := token;
    TList_Push_Back(self^.Comments, CommentList)
end;

procedure _TParser_FirstToken(var self: TParser; var out_Errors: TParseError);
begin
    while (self^.Lexer^.Token.Id in [TkComment, TkBlank]) do
    begin
        if self^.Lexer^.Token.Id = TkError then _TParser_AddError(self, out_Errors, self^.Lexer^.Error)
        else if self^.Lexer^.Token.Id = TkComment then _TParser_AddComment(self, self^.Lexer^.Token);
        TLexer_NextToken(self^.Lexer)
    end
end;

procedure _TParser_NextToken(var self: TParser; var out_Errors: TParseError);
begin
    repeat
        TLexer_NextToken(self^.Lexer);
        if self^.Lexer^.Token.Id = TkError then _TParser_AddError(self, out_Errors, self^.Lexer^.Error)
        else if self^.Lexer^.Token.Id = TkComment then _TParser_AddComment(self, self^.Lexer^.Token)
    until not (self^.Lexer^.Token.Id in [TkComment, TkBlank])
end;

function _TParser_NewAstNode(var self: TParser; AstType : TAstType): TAst;
begin
    Result := TAst_New(self^.Lexer^.Token, AstType);
    Result^.Comments := self^.Comments;
    TList_New(self^.Comments)
end;

procedure _TParser_FindSemicolon(var self: TParser; var out_Errors: TParseError);
begin
    repeat
        _TParser_NextToken(self, out_Errors)
    until self^.Lexer^.Token.Id in [TkSemicolon, TkEof, TkError]
end;

{ Parses a program header. }
function _TParser_Parse_Program_Header(self: TParser; var out_Errors: TParseError): TAst;
var Child : TAst;
begin
    if self^.Lexer^.Token.Id = TkProgram then
    begin
        Result := _TParser_NewAstNode(self, AstProgramHeader);
        _TParser_NextToken(self, out_Errors);
        if self^.Lexer^.Token.Id = TkIdentifier then
        begin
            Child := _TParser_NewAstNode(self, AstIdentifier);
            TAst_AddChild(Result, Child);
            _TParser_NextToken(self, out_Errors);
            if self^.Lexer^.Token.Id = TkLparen then
            begin
                repeat
                    _TParser_NextToken(self, out_Errors);
                    if self^.Lexer^.Token.Id = TkIdentifier then
                    begin
                        Child := _TParser_NewAstNode(self, AstIdentifier);
                        TAst_AddChild(Result, Child);
                        _TParser_NextToken(self, out_Errors)
                    end
                until self^.Lexer^.Token.Id <> TkComma;
                if self^.Lexer^.Token.Id = TkRparen then _TParser_NextToken(self, out_Errors);
            end;
        end;
        if self^.Lexer^.Token.Id <> TkSemicolon then
        begin
            _TParser_AddError(self, out_Errors, 'Expected semicolon');
            _TParser_FindSemicolon(self, out_Errors)
        end;
        TAst_SetEndLocation(Result, self^.Lexer^.Token.Location)
    end
    else
    begin
        Result := nil;
        _TParser_AddError(self, out_Errors, 'Expected ''PROGRAM''');
        _TParser_FindSemicolon(self, out_Errors)
    end
end;

{ Parses the input as a program.
  out_Errors: will receive a TList of error messages.
  Returns a parse tree. }
function TParser_Parse_Program(self: TParser; var out_Errors: TParseError): TAst;
var
    Header : TAst;
    Error : TParseError;
begin
    _TParser_FirstToken(self, out_Errors);
    Result := _TParser_NewAstNode(self, AstProgram);
    Header := _TParser_Parse_Program_Header(self, out_Errors);
    if Header <> nil then TAst_AddChild(Result, Header);
end;


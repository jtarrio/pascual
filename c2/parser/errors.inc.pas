{ Requires ../bytebuffer.inc.pas }
{ Requires ../utils.inc.pas }
{ Requires ../containers.inc.pas }

type
    { Content of a parsing error. }
    TParseError = ^TParseErrorObj;
    TParseErrorObj = record
        _ : TList;
        { The text of the error. }
        Message: string;
        { The source of the error. }
        Lexer: TLexer;
        { The location in the source and size of the items that caused the error. }
        Location: TBlock;
    end;

function TParseError_New: TParseError;
begin
    TList_New(Result)
end;

procedure TParseError_Dispose(var self: TParseError);
var Item : TParseError;
begin
    TList_Pop_Front(self, Item);
    while Item <> nil do
    begin
        Dispose(Item);
        TList_Pop_Front(self, Item);
    end
end;

{ Adds an error message to the given list. }
procedure TParseError_Add(var self : TParseError; const Message : string; const Lexer : TLexer; const Location : TBlock);
var Error : TParseError;
begin
    New(Error);
    Error^.Message := Message;
    Error^.Lexer := Lexer;
    Error^.Location := Location;
    TList_Push_Back(self, Error)
end;

{ Displays an error message. }
procedure TParseError_Display(const self : TParseError; f: text);
var
    StartOffset, EndOffset, CaretShift, CaretLen : integer;
    i : integer;
begin
    writeln(f, 'Parse error at ', self^.Lexer^.FileName, ':', self^.Location.Row, ':', self^.Location.Col, ': ', self^.Message)
end;

{ Displays a list of error messages. }
procedure TParseError_DisplayList(var self : TParseError; f: text);
var Error : TParseError;
begin
    TList_First(self, Error);
    while Error <> nil do
    begin
        TParseError_Display(Error, f);
        TList_Next(self, Error)
    end
end;

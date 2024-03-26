{ Requires ../types.inc.pas }
{ Requires ../utils.inc.pas }
{ Requires ../containers.inc.pas (for bytebuffer) }
{ Requires ../bytebuffer.inc.pas }

type
    { Token identifiers. }
    TTokenId = (TkUnknown, TkError, TkEof, TkBlank, TkComment, TkIdentifier, TkInteger, TkReal,
                TkString, TkPlus, TkMinus, TkAsterisk, TkSlash, TkEquals,
                TkLessthan, TkMorethan, TkLbracket, TkRbracket, TkDot, TkComma,
                TkColon, TkSemicolon, TkCaret, TkLparen, TkRparen, TkNotEquals,
                TkLessOrEquals, TkMoreOrEquals, TkAssign, TkRange, TkAt,
                TkAbsolute, TkAnd, TkArray, TkBegin, TkCase, TkConst, TkDiv,
                TkDo, TkDownto, TkElse, TkEnd, TkFile, TkFor, TkForward,
                TkFunction, TkGoto, TkIf, TkIn, TkLabel, TkMod, TkNil, TkNot,
                TkOf, TkOr, TkPacked, TkProcedure, TkProgram, TkRecord,
                TkRepeat, TkSet, TkShl, TkShr, TkThen, TkTo, TkType, TkUntil,
                TkVar, TkWhile, TkWith, TkXor);
    { A lexer. }
    TLexer = ^TLexerObj;
    { A token that was parsed from the lexer. }
    TToken = record
        { Token identifier. }
        Id: TTokenId;
        { Location and size of the token in the source code. }
        Location: TBlock;
        { Lexer that the token comes from. }
        Lexer: TLexer;
    end;
    { The underlying record for a lexer. }
    TLexerObj = record
        { The name of the file that is being read. }
        FileName: string;
        { The content of the file that is being read. }
        Buffer: ByteBuffer;
        { Current position in the file. }
        Pos: TPos;
        { Pointer to the current character in the buffer. }
        BufferPtr: ^char;
        { Pointer past the last character in the buffer. }
        EofPtr: ^char;
        { The last parsed token. }
        Token: TToken;
        { The message for the last parsing error. }
        Error: string;
    end;

{ Forward definitions. }
procedure TLexer_NextToken(self: TLexer); forward;

{ Creates a lexer that reads from the given ByteBuffer.
  FileName: the name of the file the data comes from.
  Buffer: the ByteBuffer to read from. Ownership passes to the lexer. }
function TLexer_New(FileName: string; Buffer: ByteBuffer): TLexer;
begin
    new(Result);
    Result^.FileName := FileName;
    Result^.Pos := TPos_Zero;
    Result^.Buffer := Buffer;
    Result^.BufferPtr := Result^.Buffer.Ptr;
    Result^.EofPtr := Result^.Buffer.Ptr + Result^.Buffer.Size;
    Result^.Token.Lexer := Result;
    TLexer_NextToken(Result)
end;

{ Frees the memory used by the lexer. }
procedure TLexer_Dispose(var self: TLexer);
begin
    ByteBuffer_Dispose(self^.Buffer);
    Dispose(self);
    self := nil
end;

{ Reads the next character, advancing the position. }
procedure _TLexer_NextChar(self: TLexer);
begin
    if self^.BufferPtr <> self^.EofPtr then
    begin
        case self^.BufferPtr^ of
            #10 : TPos_NewLine(self^.Pos);
            else TPos_Advance(self^.Pos)
        end;
        self^.BufferPtr := Succ(self^.BufferPtr)
    end
end;

{ Returns the next character from the buffer, if available.
  If unavailable, returns NUL. }
function _TLexer_PeekChar(self: TLexer): char;
var Ptr : ^char;
begin
    Ptr := Succ(self^.BufferPtr);
    if Ptr = self^.EofPtr then Result := #0
    else Result := Ptr^
end;

{ Returns the last read character.
  If we've reached the end of the buffer, returns NUL. }
function _TLexer_GetChar(self: TLexer): char;
begin
    if self^.BufferPtr = self^.EofPtr then Result := #0
    else Result := self^.BufferPtr^
end;

{ Returns whether we've reached the end of the buffer. }
function _TLexer_Eof(self: TLexer): boolean;
begin
    Result := self^.BufferPtr = self^.EofPtr
end;

{ Returns a variable containing the current position. }
function _TLexer_SavePos(self: TLexer): TPos;
begin
    Result := self^.Pos
end;

{ Sets the current position from the given variable. }
procedure _TLexer_RestorePos(self: TLexer; const Pos: TPos);
begin
    self^.Pos := Pos;
    self^.BufferPtr := self^.Buffer.Ptr + self^.Pos.Offset;
end;

{ Returns the text of a token as a string. }
function _TLexer_ReadToken(self: TLexer; const Token: TToken) : string;
begin
    Result := ByteBuffer_GetString(self^.Buffer, Token.Location.Offset, Token.Location.Size)
end;

{ Marks the starting position of a token of the given type. }
procedure _TLexer_StartToken(self: TLexer; Id: TTokenId);
begin
    self^.Token.Id := Id;
    self^.Token.Location := TBlock_Make(self^.Pos, 0);
end;

{ Marks the ending position of the last started token. }
procedure _TLexer_FinishToken(self: TLexer);
begin
    self^.Token.Location.Size := self^.Pos.Offset - self^.Token.Location.Offset
end;

{ A string containing the names of all the tokens. }
const _TLexer_NameStr : string =
    '()*+,-../:=;<=<>=@[]^absoluteofileandivarraybegintegerrorecordownto' +
    'blankcasetypecommentconstringotoforwardfunctionilabelseidentifierepeat' +
    'packedprocedurealprogramodshlshrunknownotuntilwhilewithendxor';
{ Starting position of each token's name within the string. }
const _TLexer_NamePos : array[TkUnknown..TkXor] of integer = (
    170, 53, 29, 68, 81, 123, 48, 151, 91, 4, 6, 3, 9, 11, 13, 16, 19, 20, 7, 5,
    10, 12, 21, 1, 2, 15, 13, 16, 10, 7, 18, 22, 35, 40, 45, 73, 88, 37, 62, 62,
    119, 193, 31, 100, 100, 107, 96, 128, 48, 116, 161, 114, 176, 30, 56, 138,
    144, 155, 57, 132, 75, 164, 167, 191, 66, 77, 179, 39, 184, 189, 196);
{ Length of each token's name. }
const _TLexer_NameLen : array[TkUnknown..TkXor] of integer = (
    7, 5, 3, 5, 7, 10, 7, 4, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 1, 8, 3, 5, 5, 4, 5, 3, 2, 6, 4, 3, 4, 3, 7, 8, 4, 2,
    2, 5, 3, 3, 3, 2, 2, 6, 9, 7, 6, 6, 3, 3, 3, 4, 2, 4, 5, 3, 5, 4, 3);

function TLexer_GetTokenName(id: TTokenId): string;
begin
    Result := Copy(_TLexer_NameStr, _TLexer_NamePos[id], _TLexer_NameLen[id]);
end;

function TLexer_GetIdentifierToken(Id: string): TTokenId;
var
    i : integer;
    Cur : TTokenId;
    Pos, Len : integer;
    Cp : string;
begin
    for i := 1 to Length(Id) do Id[i] := LowerCase(Id[i]);
    Result := TkBlank;
    Cur := TkAbsolute;
    repeat
        Pos := _TLexer_NamePos[Cur];
        Len := _TLexer_NameLen[Cur];
        if _TLexer_NameStr[Pos] > Id[1] then Result := TkUnknown
        else if (Len = Length(Id)) and (_TLexer_NameStr[Pos] = Id[1]) and (Copy(_TLexer_NameStr, Pos, Len) = Id) then Result := Cur
        else if Cur < TkXor then Cur := Succ(Cur)
        else Result := TkUnknown
    until Result <> TkBlank
end;

{ Parses a 'blank' token. }
procedure _TLexer_T_Blank(self: TLexer);
begin
    _TLexer_StartToken(self, TkBlank);
    while _TLexer_GetChar(self) in [' ', #9, #10, #13] do _TLexer_NextChar(self);
    _TLexer_FinishToken(self)
end;

{ Parses a 'number' token. }
procedure _TLexer_T_Number(self: TLexer);
var Pos : TPos;
begin
    _TLexer_StartToken(self, TkInteger);
    if _TLexer_GetChar(self) = '$' then
    begin
        repeat
            _TLexer_NextChar(self)
        until not (_TLexer_GetChar(self) in ['0'..'9', 'a'..'f', 'A'..'F'])
    end
    else
    begin
        while _TLexer_GetChar(self) in ['0'..'9'] do _TLexer_NextChar(self);
        if (_TLexer_GetChar(self) = '.') and (_TLexer_PeekChar(self) in ['0'..'9', 'e']) then
        begin
            self^.Token.Id := TkReal;
            repeat
                _TLexer_NextChar(self)
            until not (_TLexer_GetChar(self) in ['0'..'9'])
        end;
        if (_TLexer_GetChar(self) = 'e') and (_TLexer_PeekChar(self) in ['+', '-', '0'..'9']) then
        begin
            Pos := _TLexer_SavePos(self);
            _TLexer_NextChar(self);
            if (_TLexer_GetChar(self) in ['+', '-']) and (_TLexer_PeekChar(self) in ['0'..'9']) then
                _TLexer_NextChar(self);
            if _TLexer_GetChar(self) in ['0'..'9'] then
            begin
                repeat
                    _TLexer_NextChar(self)
                until not (_TLexer_GetChar(self) in ['0'..'9']);
                self^.Token.Id := TkReal
            end
            else
                _TLexer_RestorePos(self, Pos);
        end;
    end;
    _TLexer_FinishToken(self)
end;

{ Parses an 'identifier' token. }
procedure _TLexer_T_Identifier(self: TLexer);
var
    Id : string;
    TokenId : TTokenId;
begin
    Id := '';
    _TLexer_StartToken(self, TkIdentifier);
    while _TLexer_GetChar(self) in ['a'..'z', 'A'..'Z', '0'..'9', '_'] do
    begin
        Id := Id + _TLexer_GetChar(self);
        _TLexer_NextChar(self)
    end;
    _TLexer_FinishToken(self);
    TokenId := TLexer_GetIdentifierToken(Id);
    if TokenId <> TkUnknown then self^.Token.Id := TokenId;
end;

{ Parses a 'string' token. }
procedure _TLexer_T_String(self: TLexer);
var
    Chr : char;
    State : (None, QuotedStr, Hash, NumCharDec, NumCharHex, Caret, Done, Error);
begin
    State := None;
    _TLexer_StartToken(self, TkString);
    repeat
        Chr := _TLexer_GetChar(self);
        if State = None then
        begin
            if Chr = '''' then State := QuotedStr
            else if Chr = '#' then State := Hash
            else if Chr = '^' then State := Caret
            else State := Done;
        end
        else if State = Hash then
        begin
            if Chr = '$' then State := NumCharHex
            else if Chr in ['0'..'9'] then State := NumCharDec
            else State := Done
        end
        else if State = NumCharDec then
        begin
            if Chr = '''' then State := QuotedStr
            else if Chr = '#' then State := Hash
            else if Chr = '^' then State := Caret
            else if not (Chr in ['0'..'9']) then State := Done
        end
        else if State = NumCharHex then
        begin
            if Chr = '''' then State := QuotedStr
            else if Chr = '#' then State := Hash
            else if Chr = '^' then State := Caret
            else if not (Chr in ['0'..'9', 'a'..'f', 'A'..'F']) then State := Done
        end
        else if State = Caret then
        begin
            if (Chr in ['@'..'_']) or (Chr in ['a'..'z']) then State := None
            else State := Error
        end
        else if State = QuotedStr then
        begin
            if Chr = '''' then State := None
            else if Chr < ' ' then State := Error
        end;
        if (State <> Done) and (State <> Error) then
            _TLexer_NextChar(self)
    until (State = Done) or (State = Error);
    if State = Error then
    begin
        self^.Token.Id := TkError;
        if _TLexer_Eof(self) then
            self^.Error := 'EOF while reading string'
        else
            self^.Error := 'invalid character while reading string: ' + EscapeChar(Chr);
    end;
    _TLexer_FinishToken(self)
end;

{ Parses a single-character 'symbol' token. }
procedure _TLexer_T_Symbol(self: TLexer; Id: TTokenId);
begin
    _TLexer_StartToken(self, Id);
    _TLexer_NextChar(self);
    _TLexer_FinishToken(self)
end;

{ Parses a token starting with a '<' sign: either '<', '<=', or '<>'. }
procedure _TLexer_T_LessThan(self: TLexer);
begin
    _TLexer_StartToken(self, TkLessthan);
    _TLexer_NextChar(self);
    if _TLexer_GetChar(self) = '=' then
    begin
        self^.Token.Id := TkLessOrEquals;
        _TLexer_NextChar(self)
    end
    else if _TLexer_GetChar(self) = '>' then
    begin
        self^.Token.Id := TkNotEquals;
        _TLexer_NextChar(self)
    end;
    _TLexer_FinishToken(self)
end;

{ Parses a token starting with a '>' sign: either '>' or '>='. }
procedure _TLexer_T_MoreThan(self: TLexer);
begin
    _TLexer_StartToken(self, TkMorethan);
    _TLexer_NextChar(self);
    if _TLexer_GetChar(self) = '=' then
    begin
        self^.Token.Id := TkMoreOrEquals;
        _TLexer_NextChar(self)
    end;
    _TLexer_FinishToken(self)
end;

{ Parses a token starting with a '.' sign: either '.' or '..'. }
procedure _TLexer_T_Dot(self: TLexer);
begin
    _TLexer_StartToken(self, TkDot);
    _TLexer_NextChar(self);
    if _TLexer_GetChar(self) = '.' then
    begin
        self^.Token.Id := TkRange;
        _TLexer_NextChar(self)
    end;
    _TLexer_FinishToken(self)
end;

{ Parses a token starting with a ':' sign: either ':' or ':='. }
procedure _TLexer_T_Colon(self: TLexer);
begin
    _TLexer_StartToken(self, TkColon);
    _TLexer_NextChar(self);
    if _TLexer_GetChar(self) = '=' then
    begin
        self^.Token.Id := TkAssign;
        _TLexer_NextChar(self)
    end;
    _TLexer_FinishToken(self)
end;

{ Parses a token starting with a '(' sign: either '(' or a comment enclosed by '(* *)'. }
procedure _TLexer_T_LParen(self: TLexer);
var
    State : (Comment, Star, Done, Eof);
begin
    _TLexer_StartToken(self, TkLparen);
    _TLexer_NextChar(self);
    if _TLexer_GetChar(self) = '*' then
    begin
        self^.Token.Id := TkComment;
        _TLexer_NextChar(self);
        State := Comment;
        repeat
            if _TLexer_GetChar(self) = '*' then
                State := Star
            else if (State = Star) and (_TLexer_GetChar(self) = ')') then
                State := Done
            else if _TLexer_Eof(self) then
                State := Eof
            else
                State := Comment;
            _TLexer_NextChar(self)
        until (State = Done) or (State = Eof);
        if State = Eof then
        begin
            self^.Token.Id := TkError;
            self^.Error := 'EOF while reading comment'
        end
    end;
    _TLexer_FinishToken(self)
end;

{ Parses a comment starting with a '{' sign: either '{' or a comment enclosed by brackets. }
procedure _TLexer_T_Comment(self: TLexer);
var State : (Comment, Done, Eof);
begin
    _TLexer_StartToken(self, TkComment);
    State := Comment;
    repeat
        if _TLexer_GetChar(self) = '}' then State := Done
        else if _TLexer_Eof(self) then State := Eof;
        _TLexer_NextChar(self)
    until State <> Comment;
    if State = Eof then
    begin
        self^.Token.Id := TkError;
        self^.Error := 'EOF while reading comment'
    end;
    _TLexer_FinishToken(self)
end;

{ Parses EOF as an EOF token. }
procedure _TLexer_T_Eof(self: TLexer);
begin
    _TLexer_StartToken(self, TkEof)
end;

{ Parses an invalid character as an 'error' token. }
procedure _TLexer_T_InvalidChar(self: TLexer);
begin
    _TLexer_StartToken(self, TKError);
    self^.Error := 'Invalid character: ' + EscapeChar(_TLexer_GetChar(self));
    _TLexer_NextChar(self);
    _TLexer_FinishToken(self)
end;

{ Parses the next token. }
procedure TLexer_NextToken(self: TLexer);
var Chr : char;
begin
    Chr := _TLexer_GetChar(self);
    if _TLexer_Eof(self) then _TLexer_T_Eof(self)
    else if Chr in [' ', #9, #10, #13] then _TLexer_T_Blank(self)
    else if Chr in ['0'..'9', '$'] then _TLexer_T_Number(self)
    else if Chr in ['a'..'z', 'A'..'Z', '_'] then _TLexer_T_Identifier(self)
    else if Chr in ['''', '#'] then _TLexer_T_String(self)
    else case _TLexer_GetChar(self) of
        '+' : _TLexer_T_Symbol(self, TkPlus);
        '-' : _TLexer_T_Symbol(self, TkMinus);
        '*' : _TLexer_T_Symbol(self, TkAsterisk);
        '/' : _TLexer_T_Symbol(self, TkSlash);
        '=' : _TLexer_T_Symbol(self, TkEquals);
        '<' : _TLexer_T_LessThan(self);
        '>' : _TLexer_T_MoreThan(self);
        '[' : _TLexer_T_Symbol(self, TkLbracket);
        ']' : _TLexer_T_Symbol(self, TkRbracket);
        '.' : _TLexer_T_Dot(self);
        ',' : _TLexer_T_Symbol(self, TkComma);
        ':' : _TLexer_T_Colon(self);
        ';' : _TLexer_T_Symbol(self, TkSemicolon);
        '^' : _TLexer_T_Symbol(self, TkCaret);
        '@' : _TLexer_T_Symbol(self, TkAt);
        '(' : _TLexer_T_LParen(self);
        ')' : _TLexer_T_Symbol(self, TkRparen);
        '{' : _TLexer_T_Comment(self);
        else _TLexer_T_InvalidChar(self)
    end
end;

{ Returns a copy of the last parsed token. }
function TLexer_Token(self: TLexer): TToken;
begin
    Result := self^.Token
end;

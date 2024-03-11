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
        Block: TBlock;
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
            #9 : TPos_Tab(self^.Pos);
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
    Result := ByteBuffer_GetString(self^.Buffer, Token.Block.Offset, Token.Block.Size)
end;

{ Marks the starting position of a token of the given type. }
procedure _TLexer_StartToken(self: TLexer; Id: TTokenId);
begin
    self^.Token.Id := Id;
    self^.Token.Block := TBlock_Make(self^.Pos, 0);
end;

{ Marks the ending position of the last started token. }
procedure _TLexer_FinishToken(self: TLexer);
begin
    self^.Token.Block.Size := self^.Pos.Offset - self^.Token.Block.Offset
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

{ Checks if the given identifier corresponds to a reserved word.
  Returns the token id for the reserved word, or TkUnknown if there is no match. }
function _TLexer_T_FindReservedWord(const Id : string) : TTokenId;
{ Compact encoding: one digit with the length of the word, then the word. }
{ The words must be sorted so the search can be faster. }
const ReservedWords : string =
    '8ABSOLUTE3AND5ARRAY5BEGIN4CASE5CONST3DIV2DO6DOWNTO4ELSE3END4FILE3FOR7FORWARD8FUNCTION4GOTO2IF2IN5LABEL3MOD3NIL3NOT2OF2OR6PACKED9PROCEDURE7PROGRAM6RECORD6REPEAT3SET3SHL3SHR4THEN2TO4TYPE5UNTIL3VAR5WHILE4WITH3XOR';
var
    t : TTokenId;
    rw: string;
    rwi, rwl : integer;
    il : integer;
begin
    Result := TkUnknown;
    t := TkAbsolute;
    il := Length(Id);
    rwi := 1;
    while (rwi < Length(ReservedWords)) and (Result = TkUnknown) do
    begin
        if ReservedWords[rwi + 1] > Id[1] then
            rwi := Length(ReservedWords)
        else
        begin
            rwl := Ord(ReservedWords[rwi]) - Ord('0');
            if (rwl = il) and (ReservedWords[rwi + 1] = Id[1]) then
            begin
                rw := Copy(ReservedWords, rwi + 1, rwl);
                if rw = Id then Result := t;
            end;
            if t < TkXor then t := Succ(t);
            rwi := rwi + rwl + 1
        end
    end
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
        Id := Id + UpCase(_Tlexer_GetChar(self));
        _TLexer_NextChar(self)
    end;
    _TLexer_FinishToken(self);
    TokenId := _TLexer_T_FindReservedWord(Id);
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

{ Returns a copy of the last parsed token. }
function TLexer_Token(self: TLexer): TToken;
begin
    Result := self^.Token
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

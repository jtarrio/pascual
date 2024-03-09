{ Requires ../types.inc.pas }
{ Requires ../utils.inc.pas }
{ Requires ../bytebuffer.inc.pas }

type
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
    TLexer = ^TLexerObj;
    TToken = record
        Id: TTokenId;
        Pos: TPos;
        Size: integer;
        Ident: String;
        Lexer: TLexer;
    end;
    TLexerObj = record
        Name: string;
        Buffer: ByteBuffer;
        Pos: TPos;
        BufferPtr: ^char;
        EofPtr: ^char;
        LastChar: char;
        Token: TToken;
        Error: string;
    end;

function TLexer_New(name: string; buffer: ByteBuffer): TLexer;
begin
    new(Result);
    Result^.Name := name;
    TPos_Reset(Result^.Pos);
    Result^.Buffer := buffer;
    Result^.BufferPtr := Result^.Buffer.Ptr;
    Result^.EofPtr := Result^.Buffer.Ptr + Result^.Buffer.Size;
    Result^.Token.Id := TkUnknown;
    Result^.Token.Pos := Result^.Pos;
    Result^.Token.Size := 0;
    Result^.Token.Lexer := Result;
    if Result^.Buffer.Size = 0 then
        Result^.LastChar := #0
    else
        Result^.LastChar := Result^.BufferPtr^
end;

procedure TLexer_Dispose(var self: TLexer);
begin
    ByteBuffer_Dispose(self^.Buffer);
    Dispose(self);
    self := nil
end;

procedure _TLexer_NextChar(self: TLexer);
begin
    case self^.LastChar of
        #9 : TPos_Tab(self^.Pos);
        #10 : TPos_NewLine(self^.Pos);
        else TPos_Advance(self^.Pos)
    end;
    if self^.BufferPtr = self^.EofPtr then self^.LastChar := #0
    else
    begin
        self^.BufferPtr := Succ(self^.BufferPtr);
        self^.LastChar := self^.BufferPtr^
    end
end;

function _TLexer_PeekChar(self: TLexer): char;
var Ptr : ^char;
begin
    Ptr := Succ(self^.BufferPtr);
    if Ptr = self^.EofPtr then Result := #0
    else Result := Ptr^
end;

function _TLexer_GetChar(self: TLexer): char;
begin
    Result := self^.LastChar
end;

function _TLexer_Eof(self: TLexer): boolean;
begin
    Result := self^.BufferPtr = self^.EofPtr
end;

function _TLexer_SavePos(self: TLexer): TPos;
begin
    Result := self^.Pos
end;

procedure _TLexer_RestorePos(self: TLexer; const Pos: TPos);
begin
    self^.Pos := Pos;
    self^.BufferPtr := self^.Buffer.Ptr + self^.Pos.Offset;
end;

function _TLexer_ReadToken(self: TLexer; const Token: TToken) : string;
begin
    Result := ByteBuffer_GetString(self^.Buffer, Token.Pos.Offset, Token.Size)
end;

procedure _TLexer_MakeToken(self: TLexer; Id: TTokenId; Size: integer);
begin
    self^.Token.Id := Id;
    self^.Token.Pos := self^.Pos;
    self^.Token.Size := Size
end;

procedure _TLexer_FinishToken(self: TLexer);
begin
    self^.Token.Size := self^.Pos.Offset - self^.Token.Pos.Offset
end;

procedure _TLexer_T_Blank(self: TLexer);
begin
    _TLexer_MakeToken(self, TkBlank, 0);
    while _TLexer_GetChar(self) in [' ', #9, #10, #13] do _TLexer_NextChar(self);
    _TLexer_FinishToken(self)
end;

procedure _TLexer_T_Number(self: TLexer);
var Pos : TPos;
begin
    _TLexer_MakeToken(self, TkInteger, 0);
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

function _TLexer_T_FindKeyword(const Id : string) : TTokenId;
const Keywords : string =
    '8ABSOLUTE3AND5ARRAY5BEGIN4CASE5CONST3DIV2DO6DOWNTO4ELSE3END4FILE3FOR7FORWARD8FUNCTION4GOTO2IF2IN5LABEL3MOD3NIL3NOT2OF2OR6PACKED9PROCEDURE7PROGRAM6RECORD6REPEAT3SET3SHL3SHR4THEN2TO4TYPE5UNTIL3VAR5WHILE4WITH3XOR';
var
    t : TTokenId;
    kw: string;
    ki, kl : integer;
    il : integer;
begin
    Result := TkUnknown;
    t := TkAbsolute;
    il := Length(Id);
    ki := 1;
    while (ki < Length(Keywords)) and (Result = TkUnknown) do
    begin
        if Keywords[ki + 1] > Id[1] then
            ki := Length(Keywords)
        else
        begin
            kl := Ord(Keywords[ki]) - Ord('0');
            if (kl = il) and (Keywords[ki + 1] = Id[1]) then
            begin
                kw := Copy(Keywords, ki + 1, kl);
                if kw = Id then Result := t;
            end;
            if t < TkXor then t := Succ(t);
            ki := ki + kl + 1
        end
    end
end;

procedure _TLexer_T_Identifier(self: TLexer);
var
    Id : string;
    TokenId : TTokenId;
begin
    Id := '';
    _TLexer_MakeToken(self, TkIdentifier, 0);
    while _TLexer_GetChar(self) in ['a'..'z', 'A'..'Z', '0'..'9', '_'] do
    begin
        Id := Id + UpCase(_Tlexer_GetChar(self));
        _TLexer_NextChar(self)
    end;
    self^.Token.Ident := Id;
    _TLexer_FinishToken(self);
    TokenId := _TLexer_T_FindKeyword(Id);
    if TokenId <> TkUnknown then self^.Token.Id := TokenId;
end;

procedure _TLexer_T_String(self: TLexer);
var
    Chr : char;
    State : (None, QuotedStr, Hash, NumCharDec, NumCharHex, Caret, Done, Error);
begin
    State := None;
    _TLexer_MakeToken(self, TkString, 0);
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

procedure _TLexer_T_Symbol(self: TLexer; Id: TTokenId);
begin
    _TLexer_MakeToken(self, Id, 1);
    _TLexer_NextChar(self)
end;

procedure _TLexer_T_LessThan(self: TLexer);
begin
    _TLexer_MakeToken(self, TkLessthan, 1);
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

procedure _TLexer_T_MoreThan(self: TLexer);
begin
    _TLexer_MakeToken(self, TkMorethan, 1);
    _TLexer_NextChar(self);
    if _TLexer_GetChar(self) = '=' then
    begin
        self^.Token.Id := TkMoreOrEquals;
        _TLexer_NextChar(self)
    end;
    _TLexer_FinishToken(self)
end;

procedure _TLexer_T_Dot(self: TLexer);
begin
    _TLexer_MakeToken(self, TkDot, 1);
    _TLexer_NextChar(self);
    if _TLexer_GetChar(self) = '.' then
    begin
        self^.Token.Id := TkRange;
        _TLexer_NextChar(self)
    end;
    _TLexer_FinishToken(self)
end;

procedure _TLexer_T_Colon(self: TLexer);
begin
    _TLexer_MakeToken(self, TkColon, 1);
    _TLexer_NextChar(self);
    if _TLexer_GetChar(self) = '=' then
    begin
        self^.Token.Id := TkAssign;
        _TLexer_NextChar(self)
    end;
    _TLexer_FinishToken(self)
end;

procedure _TLexer_T_LParen(self: TLexer);
var
    State : (Comment, Star, Done, Eof);
begin
    _TLexer_MakeToken(self, TkLparen, 1);
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

procedure _TLexer_T_Comment(self: TLexer);
var State : (Comment, Done, Eof);
begin
    _TLexer_MakeToken(self, TkComment, 1);
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

function TLexer_Token(self: TLexer): TToken;
begin
    Result := self^.Token
end;

procedure TLexer_NextToken(self: TLexer);
var Chr : char;
begin
    Chr := _TLexer_GetChar(self);
    if _TLexer_Eof(self) then _TLexer_MakeToken(self, TkEof, 0)
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
        else
        begin
            _TLexer_MakeToken(self, TKError, 1);
            self^.Error := 'Invalid character: ' + EscapeChar(_TLexer_GetChar(self));
        end
    end
end;

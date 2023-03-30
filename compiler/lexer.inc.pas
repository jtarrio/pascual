type 
  TLxTokenId = (TkUnknown, TkEof, TkComment, TkIdentifier, TkInteger, TkReal,
                TkString, TkPlus, TkMinus, TkAsterisk, TkSlash, TkEquals,
                TkLessthan, TkMorethan, TkLbracket, TkRbracket, TkDot, TkComma,
                TkColon, TkSemicolon, TkCaret, TkLparen, TkRparen, TkNotEquals,
                TkLessOrEquals, TkMoreOrEquals, TkAssign, TkRange, TkAt, TkAnd,
                TkArray, TkBegin, TkCase, TkConst, TkDiv, TkDo, TkDownto,
                TkElse, TkEnd, TkFile, TkFor, TkForward, TkFunction, TkGoto,
                TkIf, TkIn, TkLabel, TkMod, TkNil, TkNot, TkOf, TkOr, TkPacked,
                TkProcedure, TkProgram, TkRecord, TkRepeat, TkSet, TkShl, TkShr,
                TkThen, TkTo, TkType, TkUntil, TkVar, TkWhile, TkWith, TkXor);
  TLxPos = record
    Row, Col : integer
  end;
  TLxToken = record
    Id : TLxTokenId;
    Value : string;
    Pos : TLxPos
  end;
  TLxInputFile = record
    Src : text;
    Name : string;
    Pos : TLxPos
  end;
  TLxIncludeStack = ^TLxIncludeStackElem;
  TLxIncludeStackElem = record
    Input : TLxInputFile;
    Prev : TLxIncludeStack
  end;

var 
  Lexer : record
    Line : string;
    Token : TLxToken;
    Input : TLxInputFile;
    IncludeStack : TLxIncludeStack
  end;

function LxTokenName(Id : TLxTokenId) : string;
var 
  Name : string;
begin
  Str(Id, Name);
  LxTokenName := Name
end;

function LxPosStr(Pos : TLxPos) : string;
begin
  LxPosStr := 'row ' + IntToStr(Pos.Row) + ' col ' + IntToStr(Pos.Col) +
              ' in ' + Lexer.Input.Name;
end;

function LxWhereStr : string;
begin
  LxWhereStr := ' near ' + LxPosStr(Lexer.Token.Pos)
end;

function LxTokenStr : string;
begin
  LxTokenStr := LxTokenName(Lexer.Token.Id) + ' [' + Lexer.Token.Value + ']'
end;

function LxIsAlpha(Chr : char) : boolean;
begin
  LxIsAlpha := Chr in ['a'..'z', 'A'..'Z']
end;

function LxIsDigit(Chr : char) : boolean;
begin
  LxIsDigit := Chr in ['0'..'9']
end;

function LxIsHexDigit(Chr : char) : boolean;
begin
  LxIsHexDigit := Chr in ['0'..'9', 'a'..'f', 'A'..'F']
end;

function LxIsAlphaNum(Chr : char) : boolean;
begin
  LxIsAlphaNum := LxIsAlpha(Chr) or LxIsDigit(Chr)
end;

function LxIsIdentifierFirst(Chr : char) : boolean;
begin
  LxIsIdentifierFirst := LxIsAlpha(Chr) or (Chr = '_')
end;

function LxIsIdentifierChar(Chr : char) : boolean;
begin
  LxIsIdentifierChar := LxIsAlphaNum(Chr) or (Chr = '_')
end;

function LxIsTokenWaiting : boolean;
begin
  repeat
    while (Length(Lexer.Line) = 0) and not Eof(Lexer.Input.Src) do
    begin
      Lexer.Input.Pos.Row := Lexer.Input.Pos.Row + 1;
      Lexer.Input.Pos.Col := 1;
      readln(Lexer.Input.Src, Lexer.Line)
    end;
    while (Length(Lexer.Line) > 0) and (Lexer.Line[1] = ' ') do
    begin
      Lexer.Input.Pos.Col := Lexer.Input.Pos.Col + 1;
      delete(Lexer.Line, 1, 1)
    end;
  until Eof(Lexer.Input.Src) or (Length(Lexer.Line) > 0);
  LxIsTokenWaiting := Length(Lexer.Line) > 0
end;

procedure LxGetSymbol(Id : TLxTokenId; Length : integer);
begin
  Lexer.Token.Id := Id;
  Lexer.Token.Value := copy(Lexer.Line, 1, Length);
  Lexer.Token.Pos := Lexer.Input.Pos;
  delete(Lexer.Line, 1, Length);
  Lexer.Input.Pos.Col := Lexer.Input.Pos.Col + Length
end;

procedure LxGetIdentifier;
const 
  NK = 39;
  Keywords : array[1..NK] of string = ('AND', 'ARRAY', 'BEGIN', 'CASE', 'CONST',
                                       'DIV', 'DO', 'DOWNTO', 'ELSE', 'END',
                                       'FILE', 'FOR', 'FORWARD', 'FUNCTION',
                                       'GOTO', 'IF', 'IN', 'LABEL', 'MOD',
                                       'NIL', 'NOT', 'OF', 'OR', 'PACKED',
                                       'PROCEDURE', 'PROGRAM', 'RECORD',
                                       'REPEAT', 'SET', 'SHL', 'SHR', 'THEN',
                                       'TO', 'TYPE', 'UNTIL', 'VAR', 'WHILE',
                                       'WITH', 'XOR' );
  Tokens : array[1..NK] of TLxTokenId = (TkAnd, TkArray, TkBegin, TkCase,
                                         TkConst, TkDiv, TkDo, TkDownto, TkElse,
                                         TkEnd, TkFile, TkFor, TkForward,
                                         TkFunction, TkGoto, TkIf, TkIn,
                                         TkLabel, TkMod, TkNil, TkNot, TkOf,
                                         TkOr, TkPacked, TkProcedure, TkProgram,
                                         TkRecord, TkRepeat, TkSet, TkShl,
                                         TkShr, TkThen, TkTo, TkType, TkUntil,
                                         TkVar, TkWhile, TkWith, TkXor);
var 
  Chr : char;
  Pos : integer;
  InToken : boolean;
begin
  Pos := 0;
  InToken := true;
  while (Pos < Length(Lexer.Line)) and InToken do
  begin
    Chr := Lexer.Line[Pos + 1];
    InToken := LxIsIdentifierChar(Chr);
    if InToken then Pos := Pos + 1
  end;
  LxGetSymbol(TkIdentifier, Pos);
  for Pos := 1 to Length(Lexer.Token.Value) do
    Lexer.Token.Value[Pos] := UpCase(Lexer.Token.Value[Pos]);

  Pos := 1;
  while (Pos <= NK) and (Lexer.Token.Id = TkIdentifier) do
  begin
    if Lexer.Token.Value = Keywords[Pos] then Lexer.Token.Id := Tokens[Pos];
    Pos := Pos + 1
  end
end;

procedure LxGetNumber;
var 
  AsInt : integer;
  AsReal : real;
  AsIntCode, AsRealCode : integer;
begin
  Val(Lexer.Line, AsInt, AsIntCode);
  Val(Lexer.Line, AsReal, AsRealCode);
  if AsIntCode = 0 then AsIntCode := Length(Lexer.Line) + 1;
  if AsRealCode = 0 then AsRealCode := Length(Lexer.Line) + 1;
  if AsRealCode > AsIntCode then LxGetSymbol(TkReal, AsRealCode - 1)
  else LxGetSymbol(TkInteger, AsIntCode - 1)
end;

procedure LxGetString;
var 
  Chr : char;
  Pos : integer;
  Last : integer;
  State : (None, QuotedStr, Hash, NumCharDec, NumCharHex, Caret, Done);
begin
  Pos := 0;
  State := None;
  repeat
    Pos := Pos + 1;
    Chr := Lexer.Line[Pos];
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
      else if LxIsDigit(Chr) then
      begin
        State := NumCharDec;
        Last := Pos
      end
      else State := Done
    end
    else if State = NumCharDec then
    begin
      if LxIsDigit(Chr) then Last := Pos
      else if Chr = '''' then State := QuotedStr
      else if Chr = '#' then State := Hash
      else if Chr = '^' then State := Caret
      else State := Done
    end
    else if State = NumCharHex then
    begin
      if LxIsHexDigit(Chr) then Last := Pos
      else if Chr = '''' then State := QuotedStr
      else if Chr = '#' then State := Hash
      else if Chr = '^' then State := Caret
      else State := Done
    end
    else if State = Caret then
    begin
      if (Chr in ['@'..'_']) or (Chr in ['a'..'z']) then
      begin
        Last := Pos;
        State := None
      end
      else State := Done
    end
    else if State = QuotedStr then
    begin
      Last := Pos;
      if Chr = '''' then State := None
    end
  until State = Done;
  LxGetSymbol(TkString, Last)
end;

procedure LxGetComment;
var 
  Done : boolean;
  DelimiterLength : integer;
  Comment : string;
begin
  Comment := '';
  Done := false;
  if Lexer.Line[1] = '{' then DelimiterLength := 1
  else DelimiterLength := 2;
  LxGetSymbol(TkComment, DelimiterLength);
  repeat
    while Lexer.Line = '' do
    begin
      Comment := Comment + ' ';
      readln(Lexer.Input.Src, Lexer.Line);
      Lexer.Input.Pos.Row := Lexer.Input.Pos.Row + 1;
      Lexer.Input.Pos.Col := 1
    end;
    if DelimiterLength = 1 then Done := Lexer.Line[1] = '}'
    else Done := (Lexer.Line[1] = '*') and (Lexer.Line[2] = ')');
    if not Done then
    begin
      Comment := Comment + Lexer.Line[1];
      delete(Lexer.Line, 1, 1);
      Lexer.Input.Pos.Col := Lexer.Input.Pos.Col + 1
    end
  until Done;
  delete(Lexer.Line, 1, DelimiterLength);
  Lexer.Input.Pos.Col := Lexer.Input.Pos.Col + DelimiterLength;
  Lexer.Token.Value := Comment
end;

procedure LxReadToken;
var 
  Chr : char;
  Pfx : string;
begin
  Lexer.Token.Value := '';
  Lexer.Token.Id := TkUnknown;

  if not LxIsTokenWaiting then
    Lexer.Token.Id := TkEof
  else
  begin
    Chr := Lexer.Line[1];
    if Length(Lexer.Line) >= 2 then Pfx := Lexer.Line[1] + Lexer.Line[2]
    else Pfx := '';

    if Pfx = '<>' then LxGetSymbol(TkNotEquals, 2)
    else if Pfx = '<=' then LxGetSymbol(TkLessOrEquals, 2)
    else if Pfx = '>=' then LxGetSymbol(TkMoreOrEquals, 2)
    else if Pfx = ':=' then LxGetSymbol(TkAssign, 2)
    else if Pfx = '..' then LxGetSymbol(TkRange, 2)
    else if Pfx = '(*' then LxGetComment
    else if LxIsIdentifierFirst(Chr) then LxGetIdentifier
    else if LxIsDigit(Chr) then LxGetNumber
    else case Chr of 
           '''' : LxGetString;
           '#' : LxGetString;
           '$' : LxGetNumber;
           '+' : LxGetSymbol(TkPlus, 1);
           '-' : LxGetSymbol(TkMinus, 1);
           '*' : LxGetSymbol(TkAsterisk, 1);
           '/' : LxGetSymbol(TkSlash, 1);
           '=' : LxGetSymbol(TkEquals, 1);
           '<' : LxGetSymbol(TkLessthan, 1);
           '>' : LxGetSymbol(TkMorethan, 1);
           '[' : LxGetSymbol(TkLbracket, 1);
           ']' : LxGetSymbol(TkRbracket, 1);
           '.' : LxGetSymbol(TkDot, 1);
           ',' : LxGetSymbol(TkComma, 1);
           ':' : LxGetSymbol(TkColon, 1);
           ';' : LxGetSymbol(TkSemicolon, 1);
           '^' : LxGetSymbol(TkCaret, 1);
           '@' : LxGetSymbol(TkAt, 1);
           '(' : LxGetSymbol(TkLparen, 1);
           ')' : LxGetSymbol(TkRparen, 1);
           '{' : LxGetComment;
           else
             CompileError('Could not parse [' + Lexer.Line + ']')
      end
  end
end;

procedure LxReset;
begin
  Lexer.Line := '';
  Lexer.Input.Src := Input;
  Lexer.Input.Name := '-';
  Lexer.Input.Pos.Row := 0;
  Lexer.Input.Pos.Col := 0;
  Lexer.IncludeStack := nil
end;

function _LxResolveFilename(const Current, New : string) : string;
var Slash : integer;
begin
  if (Current = '-') or (New[1] = '/') then Result := New
  else
  begin
    Slash := Length(Current);
    while (Slash > 0) and (Current[Slash] <> '/') do
      Slash := Slash - 1;
    if Slash = 0 then Result := New
    else Result := Copy(Current, 1, Slash) + New
  end
end;

procedure LxOpen(Filename : string);
begin
  Lexer.Input.Name := Filename;
  Assign(Lexer.Input.Src, Lexer.Input.Name);
  Reset(Lexer.Input.Src)
end;

procedure LxInclude(const Filename : string);
var NewStack : TLxIncludeStack;
begin
  new(NewStack);
  NewStack^.Input := Lexer.Input;
  NewStack^.Prev := Lexer.IncludeStack;
  Lexer.IncludeStack := NewStack;
  Lexer.Input.Pos.Row := 0;
  Lexer.Input.Pos.Col := 0;
  LxOpen(_LxResolveFilename(Lexer.Input.Name, Filename));
end;

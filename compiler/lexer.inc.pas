type 
  TLxInputFile = record
    Src : text;
    Name : string;
    Pos : TLxPos
  end;
var 
  Lexer : record
    Line : string;
    Token : TLxToken;
    Input : TLxInputFile;
    Prev : record
      Exists : boolean;
      Input : TLxInputFile
    end
  end;

function LxTokenName(Id : TLxTokenId) : string;
var 
  Name : string;
begin
  Str(Id, Name);
  LxTokenName := Name
end;

function LxPosStr(Pos : TLxPos) : string;
var 
  Row, Col : string;
begin
  Str(Pos.Row, Row);
  Str(Pos.Col, Col);
  LxPosStr := 'row ' + Row + ' col ' + Col + ' in ' + Lexer.Input.Name;
end;

function LxWhereStr : string;
begin
  LxWhereStr := ' near ' + LxPosStr(Lexer.Token.Pos)
end;

function LxTokenStr : string;
begin
  LxTokenStr := LxTokenName(Lexer.Token.Id) + ' [' + Lexer.Token.Value + ']'
end;

procedure CompileError(Msg : string);
begin
  writeln(StdErr, Msg, LxWhereStr);
  halt(1)
end;

procedure CompileWarning(Msg : string);
begin
  if not Options.SuppressWarnings then
    writeln(StdErr, 'Warning: ', Msg, LxWhereStr)
end;

function LxIsAlpha(Chr : char) : boolean;
begin
  LxIsAlpha := ((Chr >= 'a') and (Chr <= 'z'))
               or ((Chr >= 'A') and (Chr <= 'Z'))
end;

function LxIsDigit(Chr : char) : boolean;
begin
  LxIsDigit := (Chr >= '0') and (Chr <= '9')
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

  if Lexer.Token.Value = 'AND' then Lexer.Token.Id := TkAnd
  else if Lexer.Token.Value = 'ARRAY' then Lexer.Token.Id := TkArray
  else if Lexer.Token.Value = 'BEGIN' then Lexer.Token.Id := TkBegin
  else if Lexer.Token.Value = 'CASE' then Lexer.Token.Id := TkCase
  else if Lexer.Token.Value = 'CONST' then Lexer.Token.Id := TkConst
  else if Lexer.Token.Value = 'DIV' then Lexer.Token.Id := TkDiv
  else if Lexer.Token.Value = 'DO' then Lexer.Token.Id := TkDo
  else if Lexer.Token.Value = 'DOWNTO' then Lexer.Token.Id := TkDownto
  else if Lexer.Token.Value = 'ELSE' then Lexer.Token.Id := TkElse
  else if Lexer.Token.Value = 'END' then Lexer.Token.Id := TkEnd
  else if Lexer.Token.Value = 'FILE' then Lexer.Token.Id := TkFile
  else if Lexer.Token.Value = 'FOR' then Lexer.Token.Id := TkFor
  else if Lexer.Token.Value = 'FORWARD' then Lexer.Token.Id := TkForward
  else if Lexer.Token.Value = 'FUNCTION' then Lexer.Token.Id := TkFunction
  else if Lexer.Token.Value = 'GOTO' then Lexer.Token.Id := TkGoto
  else if Lexer.Token.Value = 'IF' then Lexer.Token.Id := TkIf
  else if Lexer.Token.Value = 'IN' then Lexer.Token.Id := TkIn
  else if Lexer.Token.Value = 'LABEL' then Lexer.Token.Id := TkLabel
  else if Lexer.Token.Value = 'MOD' then Lexer.Token.Id := TkMod
  else if Lexer.Token.Value = 'NIL' then Lexer.Token.Id := TkNil
  else if Lexer.Token.Value = 'NOT' then Lexer.Token.Id := TkNot
  else if Lexer.Token.Value = 'OF' then Lexer.Token.Id := TkOf
  else if Lexer.Token.Value = 'OR' then Lexer.Token.Id := TkOr
  else if Lexer.Token.Value = 'PACKED' then Lexer.Token.Id := TkPacked
  else if Lexer.Token.Value = 'PROCEDURE' then Lexer.Token.Id := TkProcedure
  else if Lexer.Token.Value = 'PROGRAM' then Lexer.Token.Id := TkProgram
  else if Lexer.Token.Value = 'RECORD' then Lexer.Token.Id := TkRecord
  else if Lexer.Token.Value = 'REPEAT' then Lexer.Token.Id := TkRepeat
  else if Lexer.Token.Value = 'SET' then Lexer.Token.Id := TkSet
  else if Lexer.Token.Value = 'THEN' then Lexer.Token.Id := TkThen
  else if Lexer.Token.Value = 'TO' then Lexer.Token.Id := TkTo
  else if Lexer.Token.Value = 'TYPE' then Lexer.Token.Id := TkType
  else if Lexer.Token.Value = 'UNTIL' then Lexer.Token.Id := TkUntil
  else if Lexer.Token.Value = 'VAR' then Lexer.Token.Id := TkVar
  else if Lexer.Token.Value = 'WHILE' then Lexer.Token.Id := TkWhile
  else if Lexer.Token.Value = 'WITH' then Lexer.Token.Id := TkWith
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
  Instring : boolean;
begin
  Pos := 1;
  Instring := True;
  while Instring do
  begin
    Pos := Pos + 1;
    Chr := Lexer.Line[Pos];
    if Chr = '''' then
    begin
      if (Length(Lexer.Line) > Pos + 1) and (Lexer.Line[Pos + 1] = '''') then
        Pos := Pos + 1
      else
        Instring := False;
    end
  end;
  LxGetSymbol(TkString, Pos)
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
           '(' : LxGetSymbol(TkLparen, 1);
           ')' : LxGetSymbol(TkRparen, 1);
           '{' : LxGetComment;
           else
             CompileError('Could not parse [' + Lexer.Line)
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
  Lexer.Prev.Exists := false;
end;

procedure LxOpen(Filename : string);
begin
  Lexer.Input.Name := Filename;
  Assign(Lexer.Input.Src, Lexer.Input.Name);
  Reset(Lexer.Input.Src)
end;

procedure LxInclude(Filename : string);
begin
  if Lexer.Prev.Exists then
    CompileError('Include files cannot be recursive');
  Lexer.Prev.Exists := true;
  Lexer.Prev.Input := Lexer.Input;
  Lexer.Input.Pos.Row := 0;
  Lexer.Input.Pos.Col := 0;
  LxOpen(Filename);
end;

program stage0(Input, Output, StdErr);

type 
  TLxTokenId = (TkUnknown, TkEof, TkComment, TkIdentifier, TkNumber, TkString,
                TkPlus, TkMinus, TkAsterisk, TkSlash, TkEquals, TkLessthan,
                TkMorethan, TkLbracket, TkRbracket, TkDot, TkComma, TkColon,
                TkSemicolon, TkCaret, TkLparen, TkRparen, TkNotEquals,
                TkLessOrEquals, TkMoreOrEquals, TkAssign, TkRange, TkAnd,
                TkArray, TkBegin, TkCase, TkConst, TkDiv, TkDo, TkDownto,
                TkElse, TkEnd, TkFile, TkFor, TkFunction, TkGoto, TkIf, TkIn,
                TkLabel, TkMod, TkNil, TkNot, TkOf, TkOr, TkPacked, TkProcedure,
                TkProgram, TkRecord, TkRepeat, TkSet, TkThen, TkTo, TkType,
                TkUntil, TkVar, TkWhile, TkWith);
  TLxToken = record
    Id : TLxTokenId;
    Value : string
  end;
var 
  LxLine : string;
  LxToken : TLxToken;

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

function LxIsTokenWaiting : boolean;
begin
  repeat
    while (Length(LxLine) = 0) and not Eof(Input) do
      readln(Input, LxLine);
    while (Length(LxLine) > 0) and (LxLine[1] = ' ') do
      delete(LxLine, 1, 1);
  until Eof(Input) or (Length(LxLine) > 0);
  LxIsTokenWaiting := Length(LxLine) > 0
end;

procedure LxGetSymbol(Id : TLxTokenId; Length : integer);
begin
  LxToken.Id := Id;
  LxToken.Value := copy(LxLine, 1, length);
  delete(LxLine, 1, length)
end;

procedure LxGetIdentifier;
const 
  Words : array[1..35] of string = ('AND', 'ARRAY', 'BEGIN', 'CASE', 'CONST',
                                    'DIV', 'DO', 'DOWNTO', 'ELSE', 'END',
                                    'FILE', 'FOR', 'FUNCTION', 'GOTO', 'IF',
                                    'IN', 'LABEL', 'MOD', 'NIL', 'NOT', 'OF',
                                    'OR', 'PACKED', 'PROCEDURE', 'PROGRAM',
                                    'RECORD', 'REPEAT', 'SET', 'THEN', 'TO',
                                    'TYPE', 'UNTIL', 'VAR', 'WHILE', 'WITH');
var 
  Chr : char;
  Pos : integer;
  InToken : boolean;
  WordId : TLxTokenId;
begin
  Pos := 0;
  InToken := true;
  while (Pos < Length(LxLine)) and InToken do
  begin
    Chr := LxLine[Pos + 1];
    InToken := LxIsAlphaNum(Chr);
    if InToken then Pos := Pos + 1
  end;
  LxGetSymbol(TkIdentifier, Pos);
  for Pos := 1 to Length(LxToken.Value) do
    LxToken.Value[Pos] := UpCase(LxToken.Value[Pos]);
  WordId := TkAnd;
  for Pos := 1 to 35 do
  begin
    if LxToken.Value = Words[Pos] then
      LxToken.Id := WordId;
    WordId := succ(WordId)
  end
end;

procedure LxGetNumber;
var 
  Chr : char;
  Pos : integer;
  InToken : boolean;
begin
  Pos := 0;
  InToken := true;
  while (Pos < Length(LxLine)) and InToken do
  begin
    Chr := LxLine[Pos + 1];
    InToken := LxIsDigit(Chr);
    if InToken then Pos := Pos + 1
  end;
  LxGetSymbol(TkNumber, Pos)
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
    Chr := LxLine[Pos];
    if Chr = '''' then
    begin
      if (Length(LxLine) > Pos + 1) and (LxLine[Pos + 1] = '''') then
        Pos := Pos + 1
      else
        Instring := False;
    end
  end;
  LxGetSymbol(TkString, Pos)
end;

procedure ReadToken;
const 
  Symbols : array[1..16] of string = ('+', '-', '*', '/', '=', '<', '>', '[',
                                      ']', '.', ',', ':', ';', '^', '(', ')');
var 
  Chr : char;
  Nxt : char;
  i : integer;
  SymbolId : TLxTokenId;
begin
  LxToken.Value := '';
  LxToken.Id := TkUnknown;

  if not LxIsTokenWaiting() then
    LxToken.Id := TkEof
  else
  begin
    Chr := LxLine[1];

    if LxIsAlpha(Chr) then
      LxGetIdentifier();
    if (LxToken.Id = TkUnknown) and LxIsDigit(Chr) then
      LxGetNumber();
    if (LxToken.Id = TkUnknown) and (Chr = '''') then
      LxGetString();
    if (LxToken.Id = TkUnknown) and (Length(LxLine) > 1) then
    begin
      Nxt := LxLine[2];
      if (LxToken.Id = TkUnknown) and (Chr = '<') and (Nxt = '>') then
        LxGetSymbol(TkNotEquals, 2);
      if (LxToken.Id = TkUnknown) and (Chr = '<') and (Nxt = '=') then
        LxGetSymbol(TkLessOrEquals, 2);
      if (LxToken.Id = TkUnknown) and (Chr = '>') and (Nxt = '=') then
        LxGetSymbol(TkMoreOrEquals, 2);
      if (LxToken.Id = TkUnknown) and (Chr = ':') and (Nxt = '=') then
        LxGetSymbol(TkAssign, 2);
      if (LxToken.Id = TkUnknown) and (Chr = '.') and (Nxt = '.') then
        LxGetSymbol(TkRange, 2)
    end;
    SymbolId := TkPlus;
    for i := 1 to 16 do
    begin
      if (LxToken.Id = TkUnknown) and (Chr = Symbols[i]) then
        LxGetSymbol(SymbolId, 1);
      SymbolId := succ(SymbolId)
    end
  end
end;

procedure WantToken(Id : TLxTokenId);
begin
  if LxToken.Id <> Id then
  begin
    writeln(StdErr, 'Wanted token ', Id, ', found ', LxToken.Id, ': ',
            LxToken.Value);
    halt(1)
  end
end;

procedure WantTokenAndRead(Id : TLxTokenId);
begin
  WantToken(Id);
  ReadToken()
end;

procedure WantToken2(Id1 : TLxTokenId; Id2 : TLxTokenId);
begin
  if (LxToken.Id <> Id1) and (LxToken.Id <> Id2) then
  begin
    writeln(StdErr, 'Wanted token ', Id1, ' or ', Id2, ', found ', LxToken.Id,
            ': ', LxToken.Value);
    halt(1)
  end
end;

procedure SkipToken(Id : TLxTokenId);
begin
  if LxToken.Id = Id then
    ReadToken()
end;

procedure PsProgramHeader;
begin
  WantTokenAndRead(TkProgram);
  WantToken(TkIdentifier);
  writeln(Output, '/* Program: ', LxToken.Value , ' */');
  ReadToken();
  if LxToken.Id = TkLparen then
  begin
    repeat
      ReadToken();
      WantToken2(TkIdentifier, TkRparen);
      SkipToken(TkIdentifier);
      WantToken2(TkComma, TkRparen)
    until LxToken.Id = TkRparen;
    SkipToken(TkRparen);
  end;
  WantTokenAndRead(TkSemicolon);
end;

procedure ParseProgram;
begin
  ReadToken();
  PsProgramHeader()
end;

begin
  ParseProgram();
end.

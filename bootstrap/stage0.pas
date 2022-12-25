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

procedure WantToken2(Id1 : TLxTokenId; Id2 : TLxTokenId);
begin
  if (LxToken.Id <> Id1) and (LxToken.Id <> Id2) then
  begin
    writeln(StdErr, 'Wanted token ', Id1, ' or ', Id2, ', found ', LxToken.Id,
            ': ', LxToken.Value);
    halt(1)
  end
end;

procedure WantTokenAndRead(Id : TLxTokenId);
begin
  WantToken(Id);
  ReadToken()
end;

procedure SkipToken(Id : TLxTokenId);
begin
  if LxToken.Id = Id then
    ReadToken()
end;

type 
  TPsEnumType = array[1..70] of string;
  TPsRecordField = record
    Name : string;
    FieldType : string
  end;
  TPsRecordType = array[1..16] of TPsRecordField;
  TPsType = record
    Name : string;
    EnumSize : integer;
    EnumValues : TPsEnumType;
    RecordSize : integer;
    RecordFields : TPsRecordType;
  end;

function PsTypeDenoter : TPsType;
var 
  Def : TPsType;
begin
  Def.Name := '';
  Def.EnumSize := 0;
  Def.RecordSize := 0;
  if LxToken.Id = TkIdentifier then
  begin
    Def.Name := LxToken.Value;
    ReadToken()
  end
  else if LxToken.Id = TkLparen then
  begin
    SkipToken(TkLparen);
    repeat
      WantToken(TkIdentifier);
      Def.EnumSize := Def.EnumSize + 1;
      Def.EnumValues[Def.EnumSize] := LxToken.Value;
      ReadToken();
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
    until LxToken.Id = TkRparen;
    SkipToken(TkRparen);
  end
  else if LxToken.Id = TkRecord then
  begin
    SkipToken(TkRecord);
    repeat
      WantToken(TkIdentifier);
      Def.RecordSize := Def.RecordSize + 1;
      Def.RecordFields[Def.RecordSize].Name := LxToken.Value;
      ReadToken();
      WantTokenAndRead(TkColon);
      WantToken(TkIdentifier);
      Def.RecordFields[Def.RecordSize].Fieldtype := LxToken.Value;
      ReadToken();
      WantToken2(TkSemicolon, TkEnd);
      SkipToken(TkSemicolon);
    until LxToken.Id = TkEnd;
    SkipToken(TkEnd);
  end
  else
  begin
    writeln(StdErr, 'Wanted type definition, found ', LxToken.Id, ': ',
            LxToken.Value);
    halt(1)
  end;
  PsTypeDenoter := Def;
end;

procedure OutNameAndType(Name : string; Def : TPsType);
var 
  Pos : integer;
  SubDef : TPsType;
begin
  if Def.EnumSize > 0 then
  begin
    write(Output, 'enum { ');
    for Pos := 1 to Def.EnumSize do
    begin
      if Pos > 1 then
        write(Output, ', ');
      write(Output, Def.EnumValues[Pos])
    end;
    write(Output, '} ', Name);
  end
  else if Def.RecordSize > 0 then
  begin
    SubDef.EnumSize := 0;
    SubDef.RecordSize := 0;
    writeln(Output, 'struct { ');
    for Pos := 1 to Def.RecordSize do
    begin
      write(Output, '  ');
      SubDef.Name := Def.RecordFields[Pos].FieldType;
      OutNameAndType(Def.RecordFields[Pos].Name, SubDef);
      writeln(Output, ';');
    end;
    write(Output, '} ', Name);
  end
  else
    write(Output, Def.Name, ' ', Name);
end;

procedure OutTypeDefinition(Name : string; Def : TPsType);
begin
  write(Output, 'typedef ');
  OutNameAndType(Name, Def);
  writeln(Output, ';');
end;

procedure PsTypeDefinitions;
var 
  Name : string;
  Def : TPsType;
begin
  WantTokenAndRead(TkType);
  repeat
    WantToken(TkIdentifier);
    Name := LxToken.Value;
    ReadToken();
    WantTokenAndRead(TkEquals);
    Def := PsTypeDenoter();
    WantTokenAndRead(TkSemicolon);
    OutTypeDefinition(Name, Def);
  until LxToken.Id <> TkIdentifier;
end;

procedure OutVarDefinition(Name : string; VarType : TPsType);
begin
  OutNameAndType(Name, VarType);
  writeln(Output, ';');
end;

procedure PsVarDefinitions;
var 
  Name : string;
  VarType : TPsType;
begin
  WantTokenAndRead(TkVar);
  repeat
    WantToken(TkIdentifier);
    Name := LxToken.Value;
    ReadToken();
    WantTokenAndRead(TkColon);
    VarType := PsTypeDenoter();
    WantTokenAndRead(TkSemicolon);
    OutVarDefinition(Name, VarType);
  until LxToken.Id <> TkIdentifier;
end;

procedure PsDefinitions;
var 
  Done : boolean;
begin
  Done := false;
  repeat
    if LxToken.Id = TkType then
      PsTypeDefinitions()
    else if LxToken.Id = TkVar then
           PsVarDefinitions()
    else
      Done := true;
  until Done;
end;

procedure OutProgramHeading(Name : string);
begin
  writeln(Output, '/* Program: ', Name, ' */');
end;

procedure PsProgramHeading;
begin
  WantTokenAndRead(TkProgram);
  WantToken(TkIdentifier);
  OutProgramHeading(LxToken.Value);
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

procedure PsProgramBlock;
begin
  PsDefinitions();
end;

procedure ParseProgram;
begin
  ReadToken();
  PsProgramHeading();
  PsProgramBlock();
  WantTokenAndRead(TkDot);
  WantToken(TkEof);
end;

begin
  ParseProgram();
end.

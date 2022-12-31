program compiler(Input, Output, StdErr);

type 
  TLxTokenId = (TkUnknown, TkEof, TkComment, TkIdentifier, TkNumber, TkString,
                TkPlus, TkMinus, TkAsterisk, TkSlash, TkEquals, TkLessthan,
                TkMorethan, TkLbracket, TkRbracket, TkDot, TkComma, TkColon,
                TkSemicolon, TkCaret, TkLparen, TkRparen, TkNotEquals,
                TkLessOrEquals, TkMoreOrEquals, TkAssign, TkRange, TkAnd,
                TkArray, TkBegin, TkCase, TkConst, TkDiv, TkDo, TkDownto,
                TkElse, TkEnd, TkFile, TkFor, TkForward, TkFunction, TkGoto,
                TkIf, TkIn, TkLabel, TkMod, TkNil, TkNot, TkOf, TkOr, TkPacked,
                TkProcedure, TkProgram, TkRecord, TkRepeat, TkSet, TkThen, TkTo,
                TkType, TkUntil, TkVar, TkWhile, TkWith);
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
    InToken := LxIsAlphaNum(Chr);
    if InToken then Pos := Pos + 1
  end;
  LxGetSymbol(TkIdentifier, Pos);
  for Pos := 1 to Length(LxToken.Value) do
    LxToken.Value[Pos] := UpCase(LxToken.Value[Pos]);

  if LxToken.Value = 'AND' then LxToken.Id := TkAnd
  else if LxToken.Value = 'ARRAY' then LxToken.Id := TkArray
  else if LxToken.Value = 'BEGIN' then LxToken.Id := TkBegin
  else if LxToken.Value = 'CASE' then LxToken.Id := TkCase
  else if LxToken.Value = 'CONST' then LxToken.Id := TkConst
  else if LxToken.Value = 'DIV' then LxToken.Id := TkDiv
  else if LxToken.Value = 'DO' then LxToken.Id := TkDo
  else if LxToken.Value = 'DOWNTO' then LxToken.Id := TkDownto
  else if LxToken.Value = 'ELSE' then LxToken.Id := TkElse
  else if LxToken.Value = 'END' then LxToken.Id := TkEnd
  else if LxToken.Value = 'FILE' then LxToken.Id := TkFile
  else if LxToken.Value = 'FOR' then LxToken.Id := TkFor
  else if LxToken.Value = 'FORWARD' then LxToken.Id := TkForward
  else if LxToken.Value = 'FUNCTION' then LxToken.Id := TkFunction
  else if LxToken.Value = 'GOTO' then LxToken.Id := TkGoto
  else if LxToken.Value = 'IF' then LxToken.Id := TkIf
  else if LxToken.Value = 'IN' then LxToken.Id := TkIn
  else if LxToken.Value = 'LABEL' then LxToken.Id := TkLabel
  else if LxToken.Value = 'MOD' then LxToken.Id := TkMod
  else if LxToken.Value = 'NIL' then LxToken.Id := TkNil
  else if LxToken.Value = 'NOT' then LxToken.Id := TkNot
  else if LxToken.Value = 'OF' then LxToken.Id := TkOf
  else if LxToken.Value = 'OR' then LxToken.Id := TkOr
  else if LxToken.Value = 'PACKED' then LxToken.Id := TkPacked
  else if LxToken.Value = 'PROCEDURE' then LxToken.Id := TkProcedure
  else if LxToken.Value = 'PROGRAM' then LxToken.Id := TkProgram
  else if LxToken.Value = 'RECORD' then LxToken.Id := TkRecord
  else if LxToken.Value = 'REPEAT' then LxToken.Id := TkRepeat
  else if LxToken.Value = 'SET' then LxToken.Id := TkSet
  else if LxToken.Value = 'THEN' then LxToken.Id := TkThen
  else if LxToken.Value = 'TO' then LxToken.Id := TkTo
  else if LxToken.Value = 'TYPE' then LxToken.Id := TkType
  else if LxToken.Value = 'UNTIL' then LxToken.Id := TkUntil
  else if LxToken.Value = 'VAR' then LxToken.Id := TkVar
  else if LxToken.Value = 'WHILE' then LxToken.Id := TkWhile
  else if LxToken.Value = 'WITH' then LxToken.Id := TkWith
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
var 
  Chr : char;
  Nxt : char;
begin
  LxToken.Value := '';
  LxToken.Id := TkUnknown;

  if not LxIsTokenWaiting() then
    LxToken.Id := TkEof
  else
  begin
    Chr := LxLine[1];

    if LxIsAlpha(Chr) then LxGetIdentifier();
    if (LxToken.Id = TkUnknown) and LxIsDigit(Chr) then LxGetNumber();
    if (LxToken.Id = TkUnknown) and (Chr = '''') then LxGetString();
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
    if (LxToken.Id = TkUnknown) and (Chr = '+') then LxGetSymbol(TkPlus, 1);
    if (LxToken.Id = TkUnknown) and (Chr = '-') then LxGetSymbol(TkMinus, 1);
    if (LxToken.Id = TkUnknown) and (Chr = '*') then LxGetSymbol(TkAsterisk, 1);
    if (LxToken.Id = TkUnknown) and (Chr = '/') then LxGetSymbol(TkSlash, 1);
    if (LxToken.Id = TkUnknown) and (Chr = '=') then LxGetSymbol(TkEquals, 1);
    if (LxToken.Id = TkUnknown) and (Chr = '<') then LxGetSymbol(TkLessthan, 1);
    if (LxToken.Id = TkUnknown) and (Chr = '>') then LxGetSymbol(TkMorethan, 1);
    if (LxToken.Id = TkUnknown) and (Chr = '[') then LxGetSymbol(TkLbracket, 1);
    if (LxToken.Id = TkUnknown) and (Chr = ']') then LxGetSymbol(TkRbracket, 1);
    if (LxToken.Id = TkUnknown) and (Chr = '.') then LxGetSymbol(TkDot, 1);
    if (LxToken.Id = TkUnknown) and (Chr = ',') then LxGetSymbol(TkComma, 1);
    if (LxToken.Id = TkUnknown) and (Chr = ':') then LxGetSymbol(TkColon, 1);
    if (LxToken.Id = TkUnknown) and (Chr = ';') then
      LxGetSymbol(TkSemicolon, 1);
    if (LxToken.Id = TkUnknown) and (Chr = '^') then LxGetSymbol(TkCaret, 1);
    if (LxToken.Id = TkUnknown) and (Chr = '(') then LxGetSymbol(TkLparen, 1);
    if (LxToken.Id = TkUnknown) and (Chr = ')') then LxGetSymbol(TkRparen, 1)
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
  TPsEnumValues = array[1..70] of string;
  TPsEnumType = record
    Size : integer;
    Values : TPsEnumValues
  end;
  TPsRecordField = record
    Name : string;
    Typ : string
  end;
  TPsRecordFields = array[1..16] of TPsRecordField;
  TPsRecordType = record
    Size : integer;
    Fields : TPsRecordFields
  end;
  TPsType = record
    Typ : string;
    Enum : TPsEnumType;
    Rec : TPsRecordType;
    IsRef : boolean;
  end;
  TPsNamedType = record
    Name : string;
    Typ : TPsType
  end;
  TPsProcedure = record
    Name : string;
    ArgCount : integer;
    Args : array[1..4] of TPsNamedType
  end;
  TPsFunction = record
    Name : string;
    ArgCount : integer;
    Args : array[1..4] of TPsNamedType;
    Ret : TPsType
  end;
  TPsScope = (Global, Local);
  TPsScopeSearch = (GlobalOnly, LocalOnly, AllScope);
  TPsCount = record
    Global : integer;
    Local : integer
  end;
  TPsDefs = record
    NumTypes : TPsCount;
    Types : array[1..32] of TPsNamedType;
    NumVars : TPsCount;
    Vars : array[1..32] of TPsNamedType;
    NumProcs : integer;
    Procs : array[1..64] of TPsProcedure;
    NumFuns : integer;
    Funs : array[1..64] of TPsFunction;
  end;
var 
  Defs : TPsDefs;

function TypeName(Typ : TPsType) : string;
var 
  Ret : string;
  Pos : integer;
begin
  if Typ.Enum.Size > 0 then
  begin
    Ret := '(';
    for Pos := 1 to Typ.Enum.Size do
    begin
      if Pos <> 1 then
        Ret := Ret + ',';
      Ret := Ret + Typ.Enum.Values[Pos]
    end;
    Ret := Ret + ')'
  end
  else if Typ.Rec.Size > 0 then
  begin
    Ret := 'record ';
    for Pos := 1 to Typ.Rec.Size do
    begin
      Ret := Ret + Typ.Rec.Fields[Pos].Name + ':' + Typ.Rec.Fields[Pos].Typ +
             '; '
    end;
    Ret := Ret + ' end'
  end
  else Ret := Typ.Typ;
  TypeName := Ret
end;

function EmptyType : TPsType;
var 
  Ret : TPsType;
begin
  Ret.Typ := '';
  Ret.Enum.Size := 0;
  Ret.Rec.Size := 0;
  EmptyType := Ret
end;

function SimpleType(Typ : string) : TPsType;
var 
  Ret : TPsType;
begin
  Ret := EmptyType();
  Ret.Typ := Typ;
  SimpleType := Ret
end;

function IsSameType(A : TPsType; B : TPsType) : boolean;
begin
  IsSameType := (A.Typ <> '') and (A.Typ = B.Typ)
end;

function IsEmptyType(Typ : TPsType) : boolean;
begin
  IsEmptyType := (Typ.Typ = '') and (Typ.Enum.Size = 0) and (Typ.Rec.Size = 0)
end;

function IntegerType : TPsType;
begin
  IntegerType := SimpleType('INTEGER')
end;

function IsIntegerType(Typ : TPsType) : boolean;
begin
  IsIntegerType := (Typ.Rec.Size = 0)
                   and ((Typ.Typ = 'INTEGER') or (Typ.Enum.Size > 0))
end;

function StringType : TPsType;
begin
  StringType := SimpleType('STRING')
end;

function CharType : TPsType;
begin
  CharType := SimpleType('CHAR')
end;

function IsStringType(Typ : TPsType) : boolean;
begin
  IsStringType := (Typ.Rec.Size = 0) and (Typ.Enum.Size = 0)
                  and (Typ.Typ = 'STRING')
end;

function IsCharType(Typ : TPsType) : boolean;
begin
  IsCharType := (Typ.Rec.Size = 0) and (Typ.Enum.Size = 0)
                and (Typ.Typ = 'CHAR')
end;

function IsStringyType(Typ : TPsType) : boolean;
begin
  IsStringyType := IsStringType(Typ) or IsCharType(Typ)
end;

function BooleanType : TPsType;
begin
  BooleanType := SimpleType('BOOLEAN')
end;

function IsBooleanType(Typ : TPsType) : boolean;
begin
  IsBooleanType := (Typ.Rec.Size = 0) and (Typ.Enum.Size = 0)
                   and (Typ.Typ = 'BOOLEAN')
end;

function TextType : TPsType;
begin
  TextType := SimpleType('TEXT')
end;

function IsTextType(Typ : TPsType) : boolean;
begin
  IsTextType := (Typ.Rec.Size = 0) and (Typ.Enum.Size = 0)
                and (Typ.Typ = 'TEXT')
end;

function IsPrimaryType(Typ : TPsType) : boolean;
begin
  IsPrimaryType := (Typ.Enum.Size > 0) or (Typ.Rec.Size > 0)
                   or (Typ.Typ = 'BOOLEAN') or (Typ.Typ = 'CHAR')
                   or (Typ.Typ = 'STRING') or (Typ.Typ = 'INTEGER')
                   or (Typ.Typ = 'TEXT')
end;

function FindType(Name : string; Scope : TPsScopeSearch) : TPsType;
var 
  Pos : integer;
  First : integer;
  Last : integer;
  Ret : TPsType;
begin
  Ret := EmptyType();
  First := 1;
  if Scope = LocalOnly then
    First := Defs.NumTypes.Global;
  Last := Defs.NumTypes.Global;
  if Scope <> GlobalOnly then
    Last := Last + Defs.NumTypes.Local;
  for Pos := First to Last do
  begin
    if Name = Defs.Types[Pos].Name then
      Ret := Defs.Types[Pos].Typ
  end;
  FindType := Ret
end;

procedure AddType(Typ : TPsNamedType; Scope : TPsScope);
var 
  Pos : integer;
begin
  if Scope = Global then
    Defs.NumTypes.Global := Defs.NumTypes.Global + 1
  else
    Defs.NumTypes.Local := Defs.NumTypes.Local + 1;
  Pos := Defs.NumTypes.Global + Defs.NumTypes.Local;
  if Pos > 32 then
  begin
    writeln(StdErr, 'Too many types');
    halt(1)
  end;
  Defs.Types[Pos] := Typ
end;

function FindVar(Name : string; Scope : TPsScopeSearch) : TPsType;
var 
  Pos : integer;
  First : integer;
  Last : integer;
  Ret : TPsType;
begin
  Ret := EmptyType();
  First := 1;
  if Scope = LocalOnly then
    First := Defs.NumVars.Global + 1;
  Last := Defs.NumVars.Global;
  if Scope <> GlobalOnly then
    Last := Last + Defs.NumVars.Local;
  for Pos := First to Last do
  begin
    if Name = Defs.Vars[Pos].Name then
      Ret := Defs.Vars[Pos].Typ
  end;
  FindVar := Ret
end;

function GetUltimateType(Typ : TPsType) : TPsType;
var 
  Name : string;
begin
  while not IsPrimaryType(Typ) and not IsEmptyType(Typ) do
  begin
    Name := Typ.Typ;
    Typ := FindType(Name, AllScope);
  end;
  GetUltimateType := Typ
end;

function ResolveType(Name : string) : TPsType;
begin
  ResolveType := GetUltimateType(SimpleType(Name))
end;

function ResolveVar(Name : string) : TPsType;
begin
  ResolveVar := GetUltimateType(FindVar(Name, AllScope))
end;

procedure AddVar(VarDef : TPsNamedType; Scope : TPsScope);
var 
  SearchScope : TPsScopeSearch;
  Pos : integer;
begin
  SearchScope := LocalOnly;
  if Scope = Global then
    SearchScope := AllScope;
  if not IsEmptyType(FindVar(VarDef.Name, SearchScope)) then
  begin
    writeln(StdErr, 'A variable named ', VarDef.Name, ' is already defined');
    halt(1)
  end;
  if Scope = Global then
    Defs.NumVars.Global := Defs.NumVars.Global + 1
  else
    Defs.NumVars.Local := Defs.NumVars.Local + 1;
  Pos := Defs.NumVars.Global + Defs.NumVars.Local;
  if Pos > 32 then
  begin
    writeln(StdErr, 'Too many vars');
    halt(1)
  end;
  Defs.Vars[Pos] := VarDef
end;

procedure AddProcArgsToLocalScope(Def : TPsProcedure);
var 
  Pos : integer;
begin
  for Pos := 1 to Def.ArgCount do
    AddVar(Def.Args[Pos], Local)
end;

procedure AddFuncArgsToLocalScope(Def : TPsFunction);
var 
  Pos : integer;
begin
  for Pos := 1 to Def.ArgCount do
    AddVar(Def.Args[Pos], Local)
end;

function EmptyProcedure : TPsProcedure;
var 
  Ret : TPsProcedure;
begin
  Ret.Name := '';
  Ret.ArgCount := 0;
  EmptyProcedure := Ret
end;

function IsEmptyProcedure(Fn : TPsProcedure) : boolean;
begin
  IsEmptyProcedure := (Fn.Name = '') and (Fn.ArgCount = 0)
end;

function FindProcedure(Name : string) : TPsProcedure;
var 
  Pos : integer;
  Ret : TPsProcedure;
begin
  Ret := EmptyProcedure();
  for Pos := 1 to Defs.NumProcs do
  begin
    if Defs.Procs[Pos].Name = Name then
      Ret := Defs.Procs[Pos]
  end;
  FindProcedure := Ret
end;

procedure AddProcedure(Proc : TPsProcedure);
begin
  Defs.NumProcs := Defs.NumProcs + 1;
  if Defs.NumProcs > 64 then
  begin
    writeln(StdErr, 'Too many functions');
    halt(1)
  end;
  Defs.Procs[Defs.NumProcs] := Proc
end;

function EmptyFunction : TPsFunction;
var 
  Ret : TPsFunction;
begin
  Ret.Name := '';
  Ret.ArgCount := 0;
  Ret.Ret := EmptyType();
  EmptyFunction := Ret
end;

function IsEmptyFunction(Fn : TPsFunction) : boolean;
begin
  IsEmptyFunction := (Fn.Name = '')
                     and (Fn.ArgCount = 0)
                     and IsEmptyType(Fn.Ret)
end;

function FindFunction(Name : string) : TPsFunction;
var 
  Pos : integer;
  Ret : TPsFunction;
begin
  Ret := EmptyFunction();
  for Pos := 1 to Defs.NumFuns do
  begin
    if Defs.Funs[Pos].Name = Name then
      Ret := Defs.Funs[Pos]
  end;
  FindFunction := Ret
end;

procedure AddFunction(Fun : TPsFunction);
begin
  Defs.NumFuns := Defs.NumFuns + 1;
  if Defs.NumFuns > 64 then
  begin
    writeln(StdErr, 'Too many functions');
    halt(1)
  end;
  Defs.Funs[Defs.NumFuns] := Fun
end;

function FindField(Typ : TPsType; Name : string) : TPsType;
var 
  Pos : integer;
  Ret : TPsType;
begin
  if Typ.Rec.Size = 0 then
  begin
    writeln(StdErr, 'Not a record: ', Name);
    halt(1)
  end;
  Ret := EmptyType();
  for Pos := 1 to Typ.Rec.Size do
    if Typ.Rec.Fields[Pos].Name = Name then
      Ret := ResolveType(Typ.Rec.Fields[Pos].Typ);
  if IsEmptyType(Ret) then
  begin
    writeln(StdErr, 'Field not found: ', Name);
    halt(1)
  end;
  FindField := Ret
end;

procedure StartGlobalScope;
var 
  Proc : TPsProcedure;
  Fun : TPsFunction;
  Def : TPsNamedType;
begin
  Proc.Name := 'DELETE';
  Proc.ArgCount := 3;
  Proc.Args[1].Name := 'STR';
  Proc.Args[1].Typ := StringType();
  Proc.Args[2].Name := 'FIRST';
  Proc.Args[2].Typ := IntegerType();
  Proc.Args[3].Name := 'NUM';
  Proc.Args[3].Typ := IntegerType();
  AddProcedure(Proc);

  Fun.Name := 'COPY';
  Fun.ArgCount := 3;
  Proc.Args[1].Name := 'STR';
  Proc.Args[1].Typ := StringType();
  Proc.Args[2].Name := 'FIRST';
  Proc.Args[2].Typ := IntegerType();
  Proc.Args[3].Name := 'NUM';
  Proc.Args[3].Typ := IntegerType();
  Fun.Ret := StringType();
  AddFunction(Fun);
  Fun.Name := 'EOF';
  Fun.ArgCount := 1;
  Fun.Args[1].Name := 'F';
  Fun.Args[1].Typ := TextType();
  Fun.Ret := BooleanType();
  AddFunction(Fun);
  Fun.Name := 'LENGTH';
  Fun.ArgCount := 1;
  Fun.Args[1].Name := 'STR';
  Fun.Args[1].Typ := StringType();
  Fun.Ret := IntegerType();
  AddFunction(Fun);

  Def.Name := 'FALSE';
  Def.Typ := BooleanType();
  AddVar(Def, Global);
  Def.Name := 'TRUE';
  Def.Typ := BooleanType();
  AddVar(Def, Global);
  Def.Name := 'INPUT';
  Def.Typ := TextType();
  AddVar(Def, Global);
  Def.Name := 'OUTPUT';
  Def.Typ := TextType();
  AddVar(Def, Global);
  Def.Name := 'STDERR';
  Def.Typ := TextType();
  AddVar(Def, Global);
end;

procedure ResetLocalScope;
begin
  Defs.NumTypes.Local := 0;
  Defs.NumVars.Local := 0
end;

function PsTypeDenoter : TPsType;
var 
  Def : TPsType;
begin
  Def.Typ := '';
  Def.Enum.Size := 0;
  Def.Rec.Size := 0;
  Def.IsRef := false;
  if LxToken.Id = TkIdentifier then
  begin
    Def.Typ := LxToken.Value;
    ReadToken()
  end
  else if LxToken.Id = TkLparen then
  begin
    SkipToken(TkLparen);
    repeat
      WantToken(TkIdentifier);
      Def.Enum.Size := Def.Enum.Size + 1;
      Def.Enum.Values[Def.Enum.Size] := LxToken.Value;
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
      Def.Rec.Size := Def.Rec.Size + 1;
      Def.Rec.Fields[Def.Rec.Size].Name := LxToken.Value;
      ReadToken();
      WantTokenAndRead(TkColon);
      WantToken(TkIdentifier);
      Def.Rec.Fields[Def.Rec.Size].Typ := LxToken.Value;
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
  if Def.IsRef then
    Name := '*' + Name;
  if Def.Enum.Size > 0 then
  begin
    write(Output, 'enum { ');
    for Pos := 1 to Def.Enum.Size do
    begin
      if Pos > 1 then
        write(Output, ', ');
      write(Output, Def.Enum.Values[Pos])
    end;
    write(Output, '} ', Name);
  end
  else if Def.Rec.Size > 0 then
  begin
    SubDef.Enum.Size := 0;
    SubDef.Rec.Size := 0;
    write(Output, 'struct { ');
    for Pos := 1 to Def.Rec.Size do
    begin
      SubDef.Typ := Def.Rec.Fields[Pos].Typ;
      OutNameAndType(Def.Rec.Fields[Pos].Name, SubDef);
      write(Output, '; ');
    end;
    write(Output, '} ', Name);
  end
  else if Def.Typ = 'BOOLEAN' then
         write(Output, 'PBoolean ', Name)
  else if Def.Typ = 'CHAR' then
         write(Output, 'char ', Name)
  else if Def.Typ = 'INTEGER' then
         write(Output, 'int ', Name)
  else
    write(Output, Def.Typ, ' ', Name);
end;

procedure OutVar(Def : TPsNamedType);
begin
  OutNameAndType(Def.Name, Def.Typ)
end;

procedure OutTypeDefinition(Def : TPsNamedType);
begin
  write(Output, 'typedef ');
  OutNameAndType(Def.Name, Def.Typ);
  writeln(Output, ';');
end;

procedure PsTypeDefinitions(Scope : TPsScope);
var 
  Def : TPsNamedType;
begin
  WantTokenAndRead(TkType);
  repeat
    WantToken(TkIdentifier);
    Def.Name := LxToken.Value;
    ReadToken();
    WantTokenAndRead(TkEquals);
    Def.Typ := PsTypeDenoter();
    WantTokenAndRead(TkSemicolon);
    OutTypeDefinition(Def);
    AddType(Def, Scope)
  until LxToken.Id <> TkIdentifier;
end;

procedure OutVarDefinition(Def : TPsNamedType);
begin
  OutNameAndType(Def.Name, Def.Typ);
  writeln(Output, ';');
end;

procedure PsVarDefinitions(Scope : TPsScope);
var 
  Def : TPsNamedType;
begin
  WantTokenAndRead(TkVar);
  repeat
    WantToken(TkIdentifier);
    Def.Name := LxToken.Value;
    ReadToken();
    WantTokenAndRead(TkColon);
    Def.Typ := PsTypeDenoter();
    WantTokenAndRead(TkSemicolon);
    OutVarDefinition(Def);
    AddVar(Def, Scope)
  until LxToken.Id <> TkIdentifier;
end;

procedure OutProcedurePrototype(Def : TPsProcedure);
var 
  Pos : integer;
begin
  write(Output, 'void ', Def.Name, '(');
  for Pos := 1 to Def.ArgCount do
  begin
    OutVar(def.Args[Pos]);
    if Pos <> Def.ArgCount then
      write(Output, ', ')
  end;
  write(Output, ')')
end;

procedure OutProcedureDeclaration(Def : TPsProcedure);
begin
  OutProcedurePrototype(Def);
  writeln(Output, ';')
end;

procedure OutProcedureDefinition(Def : TPsProcedure);
begin
  OutProcedurePrototype(Def);
  writeln(Output, ' {');
end;

procedure OutProcedureEnd(Def : TPsProcedure);
begin
  writeln(Output, '}')
end;

procedure OutFunctionPrototype(Def : TPsFunction);
var 
  Pos : integer;
begin
  OutNameAndType(Def.Name, Def.Ret);
  write(Output, '(');
  for Pos := 1 to Def.ArgCount do
  begin
    OutVar(def.Args[Pos]);
    if Pos <> Def.ArgCount then
      write(Output, ', ')
  end;
  write(Output, ')')
end;

procedure OutFunctionDeclaration(Def : TPsFunction);
begin
  OutFunctionPrototype(Def);
  writeln(Output, ';')
end;

procedure OutFunctionDefinition(Def : TPsFunction);
begin
  OutFunctionPrototype(Def);
  writeln(Output, ' {');
  OutNameAndType(Def.Name, Def.Ret);
  writeln(Output, ';')
end;

procedure OutFunctionEnd(Def : TPsFunction);
begin
  writeln(Output, 'return ', Def.Name, ';');
  writeln(Output, '}')
end;

procedure PsStatement;
forward;

procedure PsDefinitions(Scope : TPsScope);
forward;

procedure PsProcedureDefinition;
var 
  Def : TPsProcedure;
  IsRef : boolean;
begin
  WantTokenAndRead(TkProcedure);
  WantToken(TkIdentifier);
  Def.Name := LxToken.Value;
  Def.ArgCount := 0;
  ReadToken();
  WantToken2(TkLparen, TkSemicolon);
  if LxToken.Id = TkLparen then
  begin
    WantTokenAndRead(TkLparen);
    repeat
      Def.ArgCount := Def.ArgCount + 1;
      IsRef := LxToken.Id = TkVar;
      SkipToken(TkVar);
      WantToken(TkIdentifier);
      Def.Args[Def.ArgCount].Name := LxToken.Value;
      ReadToken();
      WantTokenAndRead(TkColon);
      Def.Args[Def.ArgCount].Typ := PsTypeDenoter();
      Def.Args[Def.ArgCount].Typ.IsRef := IsRef;
      WantToken2(TkSemicolon, TkRparen);
      SkipToken(TkSemicolon);
    until LxToken.Id = TkRparen;
    SkipToken(TkRparen)
  end;
  WantTokenAndRead(TkSemicolon);
  AddProcedure(Def);

  if LxToken.Id = TkForward then
  begin
    SkipToken(TkForward);
    WantTokenAndRead(TkSemicolon);
    OutProcedureDeclaration(Def);
  end
  else
  begin
    ResetLocalScope();
    AddProcArgsToLocalScope(Def);
    OutProcedureDefinition(Def);
    PsDefinitions(Local);
    WantTokenAndRead(TkBegin);
    while LxToken.Id <> TkEnd do
      PsStatement();
    WantTokenAndRead(TkEnd);
    WantTokenAndRead(TkSemicolon);
    OutProcedureEnd(Def);
  end
end;

procedure PsFunctionDefinition;
var 
  Def : TPsFunction;
  IsRef : boolean;
begin
  WantTokenAndRead(TkFunction);
  WantToken(TkIdentifier);
  Def.Name := LxToken.Value;
  Def.ArgCount := 0;
  ReadToken();
  WantToken2(TkLparen, TkColon);
  if LxToken.Id = TkLparen then
  begin
    WantTokenAndRead(TkLparen);
    repeat
      Def.ArgCount := Def.ArgCount + 1;
      IsRef := LxToken.Id = TkVar;
      SkipToken(TkVar);
      WantToken(TkIdentifier);
      Def.Args[Def.ArgCount].Name := LxToken.Value;
      ReadToken();
      WantTokenAndRead(TkColon);
      Def.Args[Def.ArgCount].Typ := PsTypeDenoter();
      Def.Args[Def.ArgCount].Typ.IsRef := IsRef;
      WantToken2(TkSemicolon, TkRparen);
      SkipToken(TkSemicolon);
    until LxToken.Id = TkRparen;
    SkipToken(TkRparen)
  end;
  WantTokenAndRead(TkColon);
  Def.Ret := PsTypeDenoter();
  WantTokenAndRead(TkSemicolon);
  AddFunction(Def);

  if LxToken.Id = TkForward then
  begin
    SkipToken(TkForward);
    WantTokenAndRead(TkSemicolon);
    OutFunctionDeclaration(Def);
  end
  else
  begin
    ResetLocalScope();
    AddFuncArgsToLocalScope(Def);
    OutFunctionDefinition(Def);
    PsDefinitions(Local);
    WantTokenAndRead(TkBegin);
    while LxToken.Id <> TkEnd do
      PsStatement();
    WantTokenAndRead(TkEnd);
    WantTokenAndRead(TkSemicolon);
    OutFunctionEnd(Def);
  end
end;

procedure PsDefinitions(Scope : TPsScope);
var 
  Done : boolean;
begin
  Done := false;
  repeat
    if LxToken.Id = TkType then
      PsTypeDefinitions(Scope)
    else if LxToken.Id = TkVar then
           PsVarDefinitions(Scope)
    else if LxToken.Id = TkProcedure then
           PsProcedureDefinition()
    else if LxToken.Id = TkFunction then
           PsFunctionDefinition()
    else
      Done := true;
  until Done;
end;

procedure OutProgramHeading(Name : string);
begin
  writeln(Output, '/* Program: ', Name, ' */');
  writeln(Output, '#include "runtime.h"')
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

procedure OutBegin;
begin
  writeln(Output, '{')
end;

procedure OutEnd;
begin
  writeln(Output, '}')
end;

type 
  TPsIdClass = (IdcVariable, IdcProcedure, IdcFunction,
                IdcRead, IdcReadln, IdcWrite, IdcWriteln);
  TPsIdentifier = record
    Cls : TPsIdClass;
    Proc : TPsProcedure;
    Fn : TPsFunction;
    Typ : TPsType;
    Name : string
  end;
  TPsExpression = record
    Value : string;
    Typ : TPsType;
  end;

function SetIdentifier(Name : string; Id : TPsIdentifier) : TPsIdentifier;
begin
  Id.Name := Name;
  if (Id.Cls = IdcVariable) and Id.Typ.IsRef then
    Id.Name := '(*' + Id.Name + ')';
  SetIdentifier := Id
end;

function AddField(Name : string; Id : TPsIdentifier) : TPsIdentifier;
begin
  Id.Name := Id.Name + '.' + Name;
  AddField := Id
end;

function SetStringIndex(Idx : TPsExpression; Id : TPsIdentifier)
: TPsIdentifier;
begin
  Id.Name := Id.Name + '.chr[' + Idx.Value + ']';
  Id.Typ := CharType();
  SetStringIndex := Id
end;

function CoerceType(Expr : TPsExpression; Typ : TPsType) : TPsExpression;
begin
  if IsCharType(Expr.Typ) and IsStringType(Typ) then
  begin
    Expr.Typ := StringType();
    Expr.Value := 'str_of(' + Expr.Value + ')';
  end
  else if not IsSameType(Expr.Typ, Typ) then
  begin
    writeln(StdErr, 'Cannot assign ', TypeName(Expr.Typ), ' to ',
    TypeName(Typ));
    halt(1)
  end;
  CoerceType := Expr
end;

function PsExpression : TPsExpression;
forward;

procedure OutIdentifier(Id : TPsIdentifier);
begin
  write(Output, Id.Name)
end;

function PsIdentifier : TPsIdentifier;
var 
  Name : string;
  Ident : TPsIdentifier;
  Proc : TPsProcedure;
  Fn : TPsFunction;
  Expr : TPsExpression;
begin
  Name := LxToken.Value;
  Proc := FindProcedure(Name);
  Fn := FindFunction(Name);
  ReadToken();
  if LxToken.Id = TkLparen then
  begin
    if Name = 'READ' then
      Ident.Cls := IdcRead
    else if Name = 'READLN' then
           Ident.Cls := IdcReadln
    else if Name = 'WRITE' then
           Ident.Cls := IdcWrite
    else if Name = 'WRITELN' then
           Ident.Cls := IdcWriteln
    else if not IsEmptyProcedure(Proc) then
    begin
      Ident.Cls := IdcProcedure;
      Ident := SetIdentifier(Name, Ident);
      Ident.Proc := Proc;
    end
    else if not IsEmptyFunction(Fn) then
    begin
      Ident.Cls := IdcFunction;
      Ident := SetIdentifier(Name, Ident);
      Ident.Fn := Fn;
    end
  end
  else
  begin
    Ident.Cls := IdcVariable;
    Ident.Typ := ResolveVar(Name);
    Ident := SetIdentifier(Name, Ident);
    if IsEmptyType(Ident.Typ) then
      Ident.Typ := Fn.Ret;
    if IsEmptyType(Ident.Typ) then
    begin
      writeln(StdErr, 'Unknown variable: ', Name);
      halt(1)
    end;
    while (LxToken.Id = TkDot) or (LxToken.Id = TkLbracket) do
    begin
      if LxToken.Id = TkDot then
      begin
        WantTokenAndRead(TkDot);
        Name := LxToken.Value;
        Ident := AddField(Name, Ident);
        Ident.Typ := FindField(Ident.Typ, Name);
      end
      else
      begin
        WantTokenAndRead(TkLbracket);
        Expr := PsExpression();
        WantToken(TkRbracket);
        if IsStringyType(Ident.Typ) then
          Ident := SetStringIndex(Expr, Ident)
        else
        begin
          writeln(StdErr, 'Specified index for variable ', Ident.Name,
                  ', which is not an array');
          halt(1)
        end
      end;
      ReadToken()
    end
  end;
  PsIdentifier := Ident
end;

function IsOpAdding(Tok : TLxToken) : boolean;
begin
  IsOpAdding := (Tok.Id = TkPlus) or (Tok.Id = TkMinus) or (Tok.Id = TkOr)
end;

function IsOpMultipying(Tok : TLxToken) : boolean;
begin
  IsOpMultipying := (Tok.Id = TkAsterisk) or (Tok.Id = TkSlash)
                    or (Tok.Id = TkDiv) or (Tok.Id = TkMod)
                    or (Tok.Id = TkAnd)
end;

function IsOpRelational(Tok : TLxToken) : boolean;
begin
  IsOpRelational := (Tok.Id = TkEquals) or (Tok.Id = TkNotEquals)
                    or (Tok.Id = TkLessthan) or (Tok.Id = TkMorethan)
                    or (Tok.Id = TkLessOrEquals) or (Tok.Id = TkMoreOrEquals)
                    or (Tok.Id = TkIn);
end;

function GenStringConstant(Value : string) : TPsExpression;
var 
  Expr : TPsExpression;
  Size : string;
  Pos : integer;
  InStr : boolean;
  LastQuote : boolean;
begin
  InStr := false;
  LastQuote := false;
  Expr.Value := '';
  for Pos := 1 to Length(Value) do
  begin
    if Value[Pos] = '''' then
    begin
      InStr := not InStr;
      if InStr and LastQuote then
        Expr.Value := Expr.Value + ''''
      else
        LastQuote := InStr
    end
    else
    begin
      LastQuote := false;
      Expr.Value := Expr.Value + Value[Pos];
    end
  end;
  if Length(Expr.Value) = 1 then
  begin
    Expr.Value := '''' + Expr.Value + '''';
    Expr.Typ := CharType()
  end
  else
  begin
    Str(Length(Expr.Value), Size);
    Expr.Value := 'str_make(' + Size + ', "' + Expr.Value + '")';
    Expr.Typ := StringType()
  end;
  GenStringConstant := Expr
end;

function GenNumberConstant(Value : string) : TPsExpression;
var 
  Expr : TPsExpression;
begin
  Expr.Typ := IntegerType();
  Expr.Value := Value;
  GenNumberConstant := Expr
end;

function IntegerBinaryExpression(Left : TPsExpression; Op : TLxTokenId;
                                 Right : TPsExpression) : TPsExpression;
var 
  Oper : string;
  Cmp : string;
  Expr : TPsExpression;
begin
  Oper := '';
  Cmp := '';
  if Op = TkPlus then Oper := '+'
  else if Op = TkMinus then Oper := '-'
  else if Op = TkAsterisk then Oper := '*'
  else if Op = TkSlash then Oper := '/'
  else if Op = TkDiv then Oper := '/'
  else if Op = TkMod then Oper := '%'
  else if Op = TkAnd then Oper := '&'
  else if Op = TkOr then Oper := '|'
  else if Op = TkEquals then Cmp := '=='
  else if Op = TkNotEquals then Cmp := '!='
  else if Op = TkLessthan then Cmp := '<'
  else if Op = TkMorethan then Cmp := '>'
  else if Op = TkLessOrEquals then Cmp := '<='
  else if Op = TkMoreOrEquals then Cmp := '>='
  else
  begin
    writeln(StdErr, 'Expected binary operator, found ', Op);
    halt(1)
  end;
  if Cmp = '' then
    Expr.Typ := IntegerType()
  else
    Expr.Typ := BooleanType();
  Expr.Value := Left.Value + ' ' + Oper + Cmp + ' ' + Right.Value;
  IntegerBinaryExpression := Expr
end;

function BooleanBinaryExpression(Left : TPsExpression; Op : TLxTokenId;
                                 Right : TPsExpression) : TPsExpression;
var 
  Oper : string;
  Expr : TPsExpression;
begin
  if Op = TkAnd then Oper := '&&'
  else if Op = TkOr then Oper := '||'
  else if Op = TkEquals then Oper := '=='
  else if Op = TkNotEquals then Oper := '!='
  else if Op = TkLessthan then Oper := '<'
  else if Op = TkMorethan then Oper := '>'
  else if Op = TkLessOrEquals then Oper := '<='
  else if Op = TkMoreOrEquals then Oper := '>='
  else
  begin
    writeln(StdErr, 'Expected binary operator, found ', Op);
    halt(1)
  end;
  Expr.Typ := BooleanType();
  Expr.Value := Left.Value + ' ' + Oper + ' ' + Right.Value;
  BooleanBinaryExpression := Expr
end;


function StringyBinaryExpression(Left : TPsExpression; Op : TLxTokenId;
                                 Right : TPsExpression) : TPsExpression;
var 
  FName : string;
  Cmp : string;
  Expr : TPsExpression;
begin
  FName := 'str_compare';
  Cmp := '';
  if Op = TkPlus then FName := 'str_concat'
  else if Op = TkEquals then Cmp := '=='
  else if Op = TkNotEquals then Cmp := '!='
  else if Op = TkLessthan then Cmp := '<'
  else if Op = TkMorethan then Cmp := '>'
  else if Op = TkLessOrEquals then Cmp := '<='
  else if Op = TkMoreOrEquals then Cmp := '>='
  else
  begin
    writeln(StdErr, 'Expected binary operator, found ', Op);
    halt(1)
  end;

  FName := FName + '_' + Left.Typ.Typ + '_' + Right.Typ.Typ;

  Expr.Typ := StringType();
  Expr.Value := FName + '(' + Left.Value + ', ' + Right.Value + ')';
  if Cmp <> '' then
  begin
    Expr.Typ := BooleanType();
    Expr.Value := '(' + Expr.Value + ' ' + Cmp + ' 0)'
  end;
  StringyBinaryExpression := Expr
end;

function BinaryExpression(Left : TPsExpression; Op : TLxTokenId; Right :
                          TPsExpression) : TPsExpression;
begin
  if IsBooleanType(Left.Typ) and IsBooleanType(Right.Typ) then
    BinaryExpression := BooleanBinaryExpression(Left, Op, Right)
  else if IsIntegerType(Left.Typ) and IsIntegerType(Right.Typ) then
         BinaryExpression := IntegerBinaryExpression(Left, Op, Right)
  else if IsStringyType(Left.Typ) and IsStringyType(Right.Typ) then
         BinaryExpression := StringyBinaryExpression(Left, Op, Right)
  else
  begin
    writeln(StdErr, 'Type mismatch for operator ', Op,
            ': ', TypeName(Left.Typ), ' and ', TypeName(Right.Typ));
    halt(1)
  end
end;

function UnaryExpression(Op : TLxTokenId; Expr : TPsExpression)
: TPsExpression;
begin
  if Op <> TkNot then
  begin
    writeln('Expected unary operator, found ', Op);
    halt(1)
  end
  else if not IsBooleanType(Expr.Typ) then
  begin
    writeln('Expected boolean expression, got ', TypeName(Expr.Typ));
    halt(1)
  end
  else
    Expr.Value := '!' + Expr.Value;
  UnaryExpression := Expr
end;

function GenFunctionCallStart(FnProc : TPsExpression) : TPsExpression;
begin
  FnProc.Value := FnProc.Value + '(';
  GenFunctionCallStart := FnProc
end;

function GenFunctionCallEnd(FnProc : TPsExpression) : TPsExpression;
begin
  FnProc.Value := FnProc.Value + ')';
  GenFunctionCallEnd := FnProc
end;

function GenFunctionCallArgument(FnProc : TPsExpression; Arg : TPsExpression;
                                 Def: TPsNamedType; First : boolean)
: TPsExpression;
begin
  if not First then
    FnProc.Value := FnProc.Value + ', ';
  if Def.Typ.IsRef then
    FnProc.Value := FnProc.Value + '&';
  FnProc.Value := FnProc.Value + CoerceType(Arg, Def.Typ).Value;
  GenFunctionCallArgument := FnProc
end;

function GenParens(Expr : TPsExpression) : TPsExpression;
begin
  Expr.Value := '(' + Expr.Value + ')';
  GenParens := Expr
end;

function PsFunctionCall(Id : TPsIdentifier) : TPsExpression;
var 
  Expr : TPsExpression;
  ArgNum : integer;
begin
  Expr.Typ := Id.Fn.Ret;
  Expr.Value := Id.Name;
  WantTokenAndRead(TkLparen);
  Expr := GenFunctionCallStart(Expr);
  for ArgNum := 1 to Id.Fn.ArgCount do
  begin
    Expr := GenFunctionCallArgument(Expr, PsExpression(),
            Id.Fn.Args[ArgNum], ArgNum = 1);
    if ArgNum < Id.Fn.ArgCount then
      WantTokenAndRead(TkComma)
  end;
  WantTokenAndRead(TkRparen);
  PsFunctionCall := GenFunctionCallEnd(Expr)
end;

procedure OutRead(Src : string; OutVar : TPsIdentifier);
begin
  if OutVar.Cls <> IdcVariable then
  begin
    writeln(StdErr, 'Expected variable for read argument, got ', OutVar.Name);
    halt(1)
  end;
  if not IsStringyType(OutVar.Typ) then
  begin
    writeln(StdErr, 'Invalid type for read argument ', OutVar.Name, ' got ',
            TypeName(OutVar.Typ));
    halt(1)
  end;
  writeln(Output, 'read_', TypeName(OutVar.Typ), '(', Src, ', &', OutVar.Name,
  ');')
end;

procedure OutReadln(Src : string);
begin
  writeln(Output, 'readln(', Src, ');')
end;

procedure PsRead(Id : TPsIdentifier);
var 
  Src : string;
  OutVar : TPsIdentifier;
begin
  Src := 'INPUT';
  WantTokenAndRead(TkLparen);
  OutVar := PsIdentifier();
  if IsTextType(OutVar.Typ) then
    Src := OutVar.Name
  else
    OutRead(Src, OutVar);
  WantToken2(TkComma, TkRparen);
  SkipToken(TkComma);
  while LxToken.Id <> TkRparen do
  begin
    WantToken(TkIdentifier);
    OutVar := PsIdentifier();
    OutRead(Src, OutVar);
    WantToken2(TkComma, TkRparen);
    SkipToken(TkComma)
  end;
  WantTokenAndRead(TkRparen);
  if Id.Cls = IdcReadln then
    OutReadln(Src)
end;

procedure OutWrite(Dst : string; Expr : TPsExpression);
begin
  writeln(Output, 'write_', TypeName(Expr.Typ), '(', Dst, ', ', Expr.Value,
  ');')
end;

procedure OutWriteln(Src : string);
begin
  writeln(Output, 'writeln(', Src, ');')
end;

procedure PsWrite(Id : TPsIdentifier);
var 
  Dst : string;
  Expr : TPsExpression;
begin
  Dst := 'OUTPUT';
  WantTokenAndRead(TkLparen);
  Expr := PsExpression();
  if IsTextType(Expr.Typ) then
    Dst := Expr.Value
  else
    OutWrite(Dst, Expr);
  WantToken2(TkComma, TkRparen);
  SkipToken(TkComma);
  while LxToken.Id <> TkRParen do
  begin
    OutWrite(Dst, PsExpression());
    WantToken2(TkComma, TkRParen);
    SkipToken(TkComma)
  end;
  WantTokenAndRead(TkRparen);
  if Id.Cls = IdcWriteln then
    OutWriteln(Dst)
end;

function GenVariable(Id : TPsIdentifier) : TPsExpression;
var 
  Expr : TPsExpression;
begin
  Expr.Typ := Id.Typ;
  Expr.Value := Id.Name;
  GenVariable := Expr
end;

function PsFactor : TPsExpression;
var 
  Expr : TPsExpression;
  Id : TPsIdentifier;
begin
  if LxToken.Id = TkString then
  begin
    Expr := GenStringConstant(LxToken.Value);
    ReadToken()
  end
  else if LxToken.Id = TkNumber then
  begin
    Expr := GenNumberConstant(LxToken.Value);
    ReadToken()
  end
  else if LxToken.Id = TkIdentifier then
  begin
    Id := PsIdentifier();
    if Id.Cls = IdcFunction then
      Expr := PsFunctionCall(Id)
    else if Id.Cls = IdcVariable then
           Expr := GenVariable(Id)
    else
    begin
      writeln(StdErr, 'Expected variable or function, got ', Id.Cls);
      halt(1)
    end
  end
  else if LxToken.Id = TkLparen then
  begin
    WantTokenAndRead(TkLparen);
    Expr := GenParens(PsExpression());
    WantTokenAndRead(TkRparen)
  end
  else if LxToken.Id = TkNot then
  begin
    WantTokenAndRead(TkNot);
    Expr := UnaryExpression(TkNot, PsFactor());
  end
  else
  begin
    writeln(StdErr, 'Invalid token in expression: ', LxToken.Id, ': ',
            LxToken.Value)
  end;
  PsFactor := Expr
end;

function PsTerm : TPsExpression;
var 
  Op : TLxTokenId;
  Expr : TPsExpression;
begin
  Expr := PsFactor();
  while IsOpMultipying(LxToken) do
  begin
    Op := LxToken.Id;
    ReadToken();
    Expr := BinaryExpression(Expr, Op, PsFactor())
  end;
  PsTerm := Expr
end;

function PsSimpleExpression : TPsExpression;
var 
  Op : TLxTokenId;
  Expr : TPsExpression;
begin
  Expr := PsTerm();
  while IsOpAdding(LxToken) do
  begin
    Op := LxToken.Id;
    ReadToken();
    Expr := BinaryExpression(Expr, Op, PsTerm())
  end;
  PsSimpleExpression := Expr
end;

function PsExpression : TPsExpression;
var 
  Op : TLxTokenId;
  Expr : TPsExpression;
begin
  Expr := PsSimpleExpression();
  while IsOpRelational(LxToken) do
  begin
    Op := LxToken.Id;
    ReadToken();
    Expr := BinaryExpression(Expr, Op, PsSimpleExpression())
  end;
  PsExpression := Expr
end;

procedure OutExpression(Expr : TPsExpression);
begin
  write(Output, Expr.Value)
end;

procedure OutAssign(Id : TPsIdentifier; Expr : TPsExpression);
begin
  OutIdentifier(Id);
  write(Output, ' = ');
  OutExpression(CoerceType(Expr, Id.Typ));
  writeln(Output, ';')
end;

procedure OutProcedureCallStart(Id : TPsIdentifier);
begin
  write(Output, Id.Name, '(')
end;

procedure OutProcedureCallEnd;
begin
  writeln(Output, ');')
end;

procedure OutProcedureCallArgument(Arg : TPsExpression; Def : TPsNamedType;
                                   First : boolean);
begin
  if not First then
    write(Output, ', ');
  if Def.Typ.IsRef then
    write(Output, '&');
  write(Output, CoerceType(Arg, Def.Typ).Value)
end;

procedure PsProcedureCall(Id : TPsIdentifier);
var 
  ArgNum : integer;
begin
  ArgNum := 1;
  WantTokenAndRead(TkLparen);
  OutProcedureCallStart(Id);
  for ArgNum := 1 to Id.Proc.ArgCount do
  begin
    OutProcedureCallArgument(PsExpression, Id.Proc.Args[ArgNum], ArgNum = 1);
    if ArgNum < Id.Proc.ArgCount then
      WantTokenAndRead(TkComma)
  end;
  WantTokenAndRead(TkRparen);
  OutProcedureCallEnd()
end;

procedure OutIf(Expr : TPsExpression);
begin
  write(Output, 'if (', Expr.Value, ') ')
end;

procedure OutElse;
begin
  write(Output, ' else ')
end;

procedure OutRepeatBegin;
begin
  writeln(Output, 'repeat {')
end;

procedure OutRepeatEnd(Expr : TPsExpression);
begin
  if not IsBooleanType(Expr.Typ) then
  begin
    writeln('Expected boolean expression, got ', TypeName(Expr.Typ));
    halt(1)
  end;
  writeln(Output, '} while (!(', Expr.Value, '));')
end;

procedure OutWhileBegin(Expr : TPsExpression);
begin
  if not IsBooleanType(Expr.Typ) then
  begin
    writeln('Expected boolean expression, got ', TypeName(Expr.Typ));
    halt(1)
  end;
  write(Output, 'while (', Expr.Value, ') ')
end;

procedure OutWhileEnd;
begin
end;

procedure PsStatement;
var 
  Id : TPsIdentifier;
begin
  if LxToken.Id = TkSemicolon then
    WantTokenAndRead(TkSemicolon)
  else if LxToken.Id = TkBegin then
  begin
    OutBegin();
    SkipToken(TkBegin);
    while LxToken.Id <> TkEnd do
    begin
      PsStatement();
    end;
    OutEnd();
    SkipToken(TkEnd);
  end
  else if LxToken.Id = TkIdentifier then
  begin
    Id := PsIdentifier();
    if (Id.Cls = IdcRead) or (Id.Cls = IdcReadln) then
      PsRead(Id)
    else if (Id.Cls = IdcWrite) or (Id.Cls = IdcWriteln) then
           PsWrite(Id)
    else if Id.Cls = IdcProcedure then
           PsProcedureCall(Id)
    else if LxToken.Id = TkAssign then
    begin
      WantTokenAndRead(TkAssign);
      OutAssign(Id, PsExpression());
    end
  end
  else if LxToken.Id = TkIf then
  begin
    WantTokenAndRead(TkIf);
    OutIf(PsExpression());
    WantTokenAndRead(TkThen);
    PsStatement();
    if LxToken.Id = TkElse then
    begin
      WantTokenAndRead(TkElse);
      OutElse();
      PsStatement();
    end
  end
  else if LxToken.Id = TkRepeat then
  begin
    WantTokenAndRead(TkRepeat);
    OutRepeatBegin();
    while LxToken.Id <> TkUntil do
      PsStatement();
    WantTokenAndRead(TkUntil);
    OutRepeatEnd(PsExpression());
  end
  else if LxToken.Id = TkWhile then
  begin
    WantTokenAndRead(TkWhile);
    OutWhileBegin(PsExpression);
    WantTokenAndRead(TkDo);
    PsStatement();
    OutWhileEnd()
  end
  else
    WantToken(TkUnknown);
end;

procedure OutProgramBegin;
begin
  writeln('int main() {');
  writeln('InitFiles();')
end;

procedure OutProgramEnd;
begin
  writeln('return 0;');
  writeln('}')
end;

procedure PsProgramBlock;
begin
  PsDefinitions(Global);
  ResetLocalScope();
  WantTokenAndRead(TkBegin);
  OutProgramBegin();
  while LxToken.Id <> TkEnd do
    PsStatement();
  OutProgramEnd();
  WantTokenAndRead(TkEnd)
end;

procedure ParseProgram;
begin
  StartGlobalScope();
  ReadToken();
  PsProgramHeading();
  PsProgramBlock();
  WantTokenAndRead(TkDot);
  WantToken(TkEof);
end;

begin
  ParseProgram();
end.

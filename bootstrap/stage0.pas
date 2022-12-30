program stage0(Input, Output, StdErr);

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
const 
  Words : array[1..36] of string = ('AND', 'ARRAY', 'BEGIN', 'CASE', 'CONST',
                                    'DIV', 'DO', 'DOWNTO', 'ELSE', 'END',
                                    'FILE', 'FOR', 'FORWARD', 'FUNCTION',
                                    'GOTO', 'IF', 'IN', 'LABEL', 'MOD', 'NIL',
                                    'NOT', 'OF', 'OR', 'PACKED', 'PROCEDURE',
                                    'PROGRAM', 'RECORD', 'REPEAT', 'SET',
                                    'THEN', 'TO', 'TYPE', 'UNTIL', 'VAR',
                                    'WHILE', 'WITH');
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
    Rec : TPsRecordType
  end;
  TPsNamedType = record
    Name : string;
    Typ : TPsType
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
    Types : array[1..16] of TPsNamedType;
    NumVars : TPsCount;
    Vars : array[1..16] of TPsNamedType;
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
        Ret := ',';
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

function IsStringyType(Typ : TPsType) : boolean;
begin
  IsStringyType := (Typ.Rec.Size = 0) and (Typ.Enum.Size = 0)
                   and ((Typ.Typ = 'STRING') or (Typ.Typ = 'CHAR'))
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

function IsPrimaryType(Typ : TPsType) : boolean;
begin
  IsPrimaryType := (Typ.Enum.Size > 0) or (Typ.Rec.Size > 0)
                   or (Typ.Typ = 'CHAR') or (Typ.Typ = 'STRING')
                   or (Typ.Typ = 'INTEGER') or (Typ.Typ = 'TEXT')
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
    Last := Defs.NumTypes.Local;
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
  if Scope = Local then
    Defs.NumTypes.Global := Defs.NumTypes.Global + 1
  else
    Defs.NumTypes.Local := Defs.NumTypes.Local + 1;
  Pos := Defs.NumTypes.Global + Defs.NumTypes.Local;
  if Pos > 16 then
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
    First := Defs.NumVars.Global;
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

function ResolveVar(Name : string) : TPsType;
var 
  Typ : TPsType;
begin
  Typ := FindVar(Name, AllScope);
  while not IsPrimaryType(Typ) do
  begin
    if IsEmptyType(Typ) then
    begin
      writeln(StdErr, 'Unknown variable: ', Name);
      halt(1)
    end;
    Name := Typ.Typ;
    Typ := FindType(Name, AllScope);
  end;
  ResolveVar := Typ
end;

procedure AddVar(VarDef : TPsNamedType; Scope : TPsScope);
var 
  Pos : integer;
begin
  if Scope = Global then
    Defs.NumVars.Global := Defs.NumVars.Global + 1
  else
    Defs.NumVars.Local := Defs.NumVars.Local + 1;
  Pos := Defs.NumVars.Global + Defs.NumVars.Local;
  if Pos > 16 then
  begin
    writeln(StdErr, 'Too many vars');
    halt(1)
  end;
  Defs.Vars[Pos] := VarDef
end;

procedure AddArgumentsToLocalScope(Def : TPsFunction);
var 
  Pos : integer;
begin
  for Pos := 1 to Def.ArgCount do
    AddVar(Def.Args[Pos], Global)
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
      Ret := FindType(Typ.Rec.Fields[Pos].Typ, AllScope);
  if IsEmptyType(Ret) then
  begin
    writeln(StdErr, 'Field not found: ', Name);
    halt(1)
  end;
  FindField := Ret
end;

procedure StartGlobalScope;
var 
  Fun : TPsFunction;
  Def : TPsNamedType;
begin
  Fun.Name := 'LENGTH';
  Fun.ArgCount := 1;
  Fun.Args[1].Name := 'STR';
  Fun.Args[1].Typ := StringType();
  Fun.Ret := IntegerType();
  AddFunction(Fun);
  Fun.Name := 'EOF';
  Fun.ArgCount := 1;
  Fun.Args[1].Name := 'F';
  Fun.Args[1].Typ := TextType();
  Fun.Ret := BooleanType();
  AddFunction(Fun);

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

procedure StartLocalScope;
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

procedure PsFunctionDefinition;
var 
  Def : TPsFunction;
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
      WantToken(TkIdentifier);
      Def.Args[Def.ArgCount].Name := LxToken.Value;
      ReadToken();
      WantTokenAndRead(TkColon);
      Def.Args[Def.ArgCount].Typ := PsTypeDenoter();
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
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
    StartLocalScope();
    AddArgumentsToLocalScope(Def);
    OutFunctionDefinition(Def);
    WantTokenAndRead(TkBegin);
    while LxToken.Id <> TkEnd do
      PsStatement();
    WantTokenAndRead(TkEnd);
    WantTokenAndRead(TkSemicolon);
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
    else if LxToken.Id = TkFunction then
           PsFunctionDefinition()
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

procedure OutBegin;
begin
  writeln(Output, '{')
end;

procedure OutEnd;
begin
  writeln(Output, '}')
end;

type 
  TPsIdClass = (IdcVariable, IdcFunction);
  TPsIdentifier = record
    Cls : TPsIdClass;
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
  SetIdentifier := Id
end;

function AddField(Name : string; Id : TPsIdentifier) : TPsIdentifier;
begin
  Id.Name := Id.Name + '.' + Name;
  AddField := Id
end;

procedure OutIdentifier(Id : TPsIdentifier);
begin
  write(Output, Id.Name)
end;

function PsIdentifier : TPsIdentifier;
var 
  Name : string;
  Ident : TPsIdentifier;
  Fn : TPsFunction;
begin
  Name := LxToken.Value;
  Fn := FindFunction(Name);
  if not IsEmptyFunction(Fn) then
  begin
    Ident.Cls := IdcFunction;
    Ident := SetIdentifier(Name, Ident);
    Ident.Fn := Fn;
    ReadToken()
  end
  else
  begin
    Ident.Cls := IdcVariable;
    Ident := SetIdentifier(Name, Ident);
    Ident.Typ := ResolveVar(Name);
    ReadToken();
    while (LxToken.Id = TkDot) do
    begin
      WantTokenAndRead(TkDot);
      Name := LxToken.Value;
      Ident := AddField(Name, Ident);
      Ident.Typ := FindField(Ident.Typ, Name);
      ReadToken()
    end
  end;
  PsIdentifier := Ident;
end;

function IsOpAdding(Tok : TLxToken) : boolean;
begin
  IsOpAdding := (Tok.Id = TkPlus) or (Tok.Id = TkMinus) or (Tok.Id = TkOr)
end;

function IsOpMultipying(Tok : TLxToken) : boolean;
begin
  IsOpMultipying := (Tok.Id = TkAsterisk) or (Tok.Id = TkSlash)
                    or (Tok.Id = TkDiv) or (Tok.Id = TkMod) or (Tok.Id = TkAnd)
end;

function IsOpRelational(Tok : TLxToken) : boolean;
begin
  IsOpRelational := (Tok.Id = TkEquals) or (Tok.Id = TkNotEquals)
                    or (Tok.Id = TkLessthan) or (Tok.Id = TkMorethan)
                    or (Tok.Id = TkLessOrEquals) or (Tok.Id = TkMoreOrEquals)
                    or (Tok.Id = TkIn);
end;

function PsExpression : TPsExpression
                        forward;

function GenStringConstant(Value : string) : TPsExpression;
var 
  Expr : TPsExpression;
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
    Expr.Value := 'str_make("' + Expr.Value + '")';
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

function IntegerBinaryExpression(Left : TPsExpression; Op : TLxTokenId; Right :
                                 TPsExpression) : TPsExpression;
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

function BooleanBinaryExpression(Left : TPsExpression; Op : TLxTokenId; Right :
                                 TPsExpression) : TPsExpression;
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


function StringyBinaryExpression(Left : TPsExpression; Op : TLxTokenId; Right :
                                 TPsExpression) : TPsExpression;
var 
  FName : string;
  Cmp : string;
  Expr : TPsExpression;
begin
  FName := 'str_compare';
  Cmp := '';
  if Op = TkPlus then FName := 'str_concat'
  else if Op = TkEquals then Cmp := '='
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
    writeln(StdErr, 'Type mismatch for operator ', Op, ': ', TypeName(Left.Typ),
    ' and ', TypeName(Right.Typ));
    halt(1)
  end
end;

function UnaryExpression(Op : TLxTokenId; Expr : TPsExpression) : TPsExpression;
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

function GenCallStart(FnProc : TPsExpression) : TPsExpression;
begin
  FnProc.Value := FnProc.Value + '(';
  GenCallStart := FnProc
end;

function GenCallEnd(FnProc : TPsExpression) : TPsExpression;
begin
  FnProc.Value := FnProc.Value + ')';
  GenCallEnd := FnProc
end;

function GenCallArgument(FnProc : TPsExpression; Arg : TPsExpression; First :
                         boolean)
: TPsExpression;
begin
  if not First then
    FnProc.Value := FnProc.Value + ', ';
  FnProc.Value := FnProc.Value + Arg.Value;
  GenCallArgument := FnProc
end;

function GenParens(Expr : TPsExpression) : TPsExpression;
begin
  Expr.Value := '(' + Expr.Value + ')';
  GenParens := Expr
end;

function PsCall(Id : TPsIdentifier) : TPsExpression;
var 
  Expr : TPsExpression;
  First : boolean;
begin
  Expr.Typ := Id.Fn.Ret;
  Expr.Value := Id.Name;
  First := true;
  WantTokenAndRead(TkLparen);
  Expr := GenCallStart(Expr);
  while LxToken.Id <> TkRparen do
  begin
    Expr := GenCallArgument(Expr, PsExpression(), First);
    First := false;
    WantToken2(TkComma, TkRparen);
    SkipToken(TkComma)
  end;
  WantTokenAndRead(TkRparen);
  PsCall := GenCallEnd(Expr)
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
      Expr := PsCall(Id)
    else
      Expr := GenVariable(Id)
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
  OutExpression(Expr);
  writeln(Output, ';')
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
  writeln(Output, 'while (!(', Expr.Value, '));')
end;

procedure OutWhileBegin(Expr : TPsExpression);
begin
  if not IsBooleanType(Expr.Typ) then
  begin
    writeln('Expected boolean expression, got ', TypeName(Expr.Typ));
    halt(1)
  end;
  writeln(Output, 'while (', Expr.Value, ') {')
end;

procedure OutWhileEnd;
begin
  writeln(Output, '}')
end;

procedure PsStatement;
var 
  Id : TPsIdentifier;
begin
  if LxToken.Id = TkBegin then
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
    if LxToken.Id = TkAssign then
    begin
      WantTokenAndRead(TkAssign);
      OutAssign(Id, PsExpression());
    end
  end
  else if LxToken.Id = TkRepeat then
  begin
    WantTokenAndRead(TkRepeat);
    OutRepeatBegin();
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

procedure PsProgramBlock;
begin
  PsDefinitions(Global);
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

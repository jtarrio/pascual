program compiler;

const 
  MaxTypes = 64;
  MaxEnums = 16;
  MaxEnumValues = 128;
  MaxRecords = 32;
  MaxRecordFields = 16;
  MaxArrays = 32;
  MaxConstants = 32;
  MaxVariables = 32;
  MaxFunctions = 256;
  MaxFunctionArguments = 4;

type 
  TLxTokenId = (TkUnknown, TkEof, TkComment, TkIdentifier, TkNumber, TkString,
                TkPlus, TkMinus, TkAsterisk, TkSlash, TkEquals, TkLessthan,
                TkMorethan, TkLbracket, TkRbracket, TkDot, TkComma, TkColon,
                TkSemicolon, TkCaret, TkLparen, TkRparen, TkNotEquals,
                TkLessOrEquals, TkMoreOrEquals, TkAssign, TkRange, TkAnd,
                TkFalse, TkTrue, TkArray, TkBegin, TkCase, TkConst, TkDiv, TkDo,
                TkDownto, TkElse, TkEnd, TkFile, TkFor, TkForward, TkFunction,
                TkGoto, TkIf, TkIn, TkLabel, TkMod, TkNil, TkNot, TkOf, TkOr,
                TkPacked, TkProcedure, TkProgram, TkRecord, TkRepeat, TkSet,
                TkThen, TkTo, TkType, TkUntil, TkVar, TkWhile, TkWith);
  TLxPos = record
    Row : integer;
    Col : integer
  end;
  TLxToken = record
    Id : TLxTokenId;
    Value : string;
    Pos : TLxPos
  end;
var 
  LxLine : string;
  LxToken : TLxToken;
  LxPos : TLxPos;
  LxInput : text;

function LxPosStr(Pos : TLxPos) : string;
var 
  Row : string;
  Col : string;
begin
  Str(Pos.Row, Row);
  Str(Pos.Col, Col);
  LxPosStr := 'row ' + Row + ' col ' + Col
end;

function LxWhereStr : string;
begin
  LxWhereStr := ' near ' + LxPosStr(LxToken.Pos)
end;

function LxTokenStr : string;
var 
  Id : string;
begin
  Str(LxToken.Id, Id);
  LxTokenStr := Id + ' [' + LxToken.Value + ']'
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

function LxIsTokenWaiting : boolean;
begin
  repeat
    while (Length(LxLine) = 0) and not Eof(LxInput) do
    begin
      LxPos.Row := LxPos.Row + 1;
      LxPos.Col := 1;
      readln(LxInput, LxLine)
    end;
    while (Length(LxLine) > 0) and (LxLine[1] = ' ') do
    begin
      LxPos.Col := LxPos.Col + 1;
      delete(LxLine, 1, 1)
    end;
  until Eof(LxInput) or (Length(LxLine) > 0);
  LxIsTokenWaiting := Length(LxLine) > 0
end;

procedure LxGetSymbol(Id : TLxTokenId; Length : integer);
begin
  LxToken.Id := Id;
  LxToken.Value := copy(LxLine, 1, Length);
  LxToken.Pos := LxPos;
  delete(LxLine, 1, Length);
  LxPos.Col := LxPos.Col + Length
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

procedure LxGetComment;
var 
  Chr : char;
  ShortComment : boolean;
  Delim : char;
  CanEnd : boolean;
begin
  Chr := LxLine[1];
  ShortComment := Chr = '{';
  if ShortComment then
  begin
    Delim := '}';
    LxGetSymbol(TkComment, 1);
  end
  else
  begin
    Delim := ')';
    LxGetSymbol(TkComment, 2);
  end;

  repeat
    if not LxIsTokenWaiting() then
    begin
      writeln(StdErr, 'End of file in comment', LxWhereStr());
      halt(1)
    end;
    CanEnd := ShortComment or (Chr = '*');
    Chr := LxLine[1];
    delete(LxLine, 1, 1);
    LxPos.Col := LxPos.Col + 1;
  until CanEnd and (Chr = Delim);
  LxToken.Value := ''
end;

procedure LxReadToken;
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
        LxGetSymbol(TkRange, 2);
      if (LxToken.Id = TkUnknown) and (Chr = '(') and (Nxt = '*') then
        LxGetComment()
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
    if (LxToken.Id = TkUnknown) and (Chr = ')') then LxGetSymbol(TkRparen, 1);
    if (LxToken.Id = TkUnknown) and (Chr = '{') then LxGetComment();
    if LxToken.Id = TkUnknown then
    begin
      writeln(StdErr, 'Could not parse [', LxLine, '] at ', LxPosStr(LxPos));
      halt(1)
    end
  end
end;

procedure ReadToken;
forward;

procedure WantToken(Id : TLxTokenId);
begin
  if LxToken.Id <> Id then
  begin
    writeln(StdErr, 'Wanted token ', Id, ', found ', LxTokenStr(),
    LxWhereStr());
    halt(1)
  end
end;

procedure WantToken2(Id1 : TLxTokenId; Id2 : TLxTokenId);
begin
  if (LxToken.Id <> Id1) and (LxToken.Id <> Id2) then
  begin
    writeln(StdErr, 'Wanted token ', Id1, ' or ', Id2, ', found ', LxTokenStr(),
    LxWhereStr());
    halt(1)
  end
end;

procedure WantTokenAndRead(Id : TLxTokenId);
begin
  WantToken(Id);
  ReadToken()
end;

function GetTokenValueAndRead(Id : TLxTokenId) : string;
begin
  WantToken(Id);
  GetTokenValueAndRead := LxToken.Value;
  ReadToken()
end;

procedure SkipToken(Id : TLxTokenId);
begin
  if LxToken.Id = Id then
    ReadToken()
end;

type 
  TPsEnumIndex = integer;
  TPsRecordIndex = integer;
  TPsArrayIndex = integer;
  TPsTypeIndex = integer;
  TPsTypeClass = (TtcBoolean, TtcInteger, TtcChar, TtcString, TtcText,
                  TtcEnum, TtcRecord, TtcArray);
  TPsType = record
    Name : string;
    Cls : TPsTypeClass;
    EnumIndex : TPsEnumIndex;
    RecordIndex : TPsRecordIndex;
    ArrayIndex : TPsArrayIndex;
    AliasFor : TPsTypeIndex;
  end;
  TPsEnumDef = record
    Size : integer;
    Values : array[1..MaxEnumValues] of string;
  end;
  TPsRecordField = record
    Name : string;
    TypeIndex : TPsTypeIndex
  end;
  TPsRecordDef = record
    Size : integer;
    Fields : array[1..MaxRecordFields] of TPsRecordField
  end;
  TPsArrayDef = record
    LowBound : string;
    HighBound : string;
    TypeIndex : TPsTypeIndex
  end;
  TPsConstantIndex = integer;
  TPsConstant = record
    Name : string;
    Replacement : TLxToken
  end;
  TPsVariableIndex = integer;
  TPsVariable = record
    Name : string;
    TypeIndex : TPsTypeIndex;
    IsReference : boolean;
    IsConstant : boolean
  end;
  TPsFunctionIndex = integer;
  TPsFunction = record
    Name : string;
    ArgCount : integer;
    Args : array[1..MaxFunctionArguments] of TPsVariable;
    ReturnTypeIndex : TPsTypeIndex;
    IsDeclaration : boolean;
  end;
  TPsScope = record
    NumTypes : integer;
    NumEnums : integer;
    NumRecords : integer;
    NumArrays : integer;
    NumConstants : integer;
    NumVariables : integer;
    NumFunctions : integer
  end;
  TPsDefs = record
    Scope : TPsScope;
    Types : array[1..MaxTypes] of TPsType;
    Enums : array[1..MaxEnums] of TPsEnumDef;
    Records : array[1..MaxRecords] of TPsRecordDef;
    Arrays : array[1..MaxArrays] of TPsArrayDef;
    Constants : array[1..MaxConstants] of TPsConstant;
    Variables : array[1..MaxVariables] of TPsVariable;
    Functions : array[1..MaxFunctions] of TPsFunction;
  end;
  TPsIdClass = (IdcVariable, IdcFunction,
                IdcRead, IdcReadln, IdcWrite, IdcWriteln, IdcStr);
  TPsIdentifier = record
    Name : string;
  end;
  TPsExpressionClass = (TecValue, TecFunction, TecStatement);
  TPsExpression = record
    Value : string;
    Cls : TPsExpressionClass;
    IsConstant : boolean;
    TypeIndex : TPsTypeIndex;
    FunctionIndex : TPsFunctionIndex
  end;
var 
  Defs : TPsDefs;
  PrimitiveTypes : record
    PtBoolean : TPsTypeIndex;
    PtInteger : TPsTypeIndex;
    PtChar : TPsTypeIndex;
    PtString : TPsTypeIndex;
    PtText : TPsTypeIndex
  end;
  GlobalScope : TPsScope;
  CpOutput : text;

procedure ClearDefs;
begin
  Defs.Scope.NumTypes := 0;
  Defs.Scope.NumEnums := 0;
  Defs.Scope.NumRecords := 0;
  Defs.Scope.NumArrays := 0;
  Defs.Scope.NumConstants := 0;
  Defs.Scope.NumVariables := 0;
  Defs.Scope.NumFunctions := 0;
end;

function GetCurrentScope: TPsScope;
begin
  GetCurrentScope := Defs.Scope
end;

procedure SetCurrentScope(Scope : TPsScope);
begin
  Defs.Scope := Scope
end;

function DeepTypeName(TypeIndex : TPsTypeIndex; UseOriginal : boolean) : string;
var 
  Typ : TPsType;
  Ret : string;
  Pos : integer;
begin
  repeat
    Typ := Defs.Types[TypeIndex];
    TypeIndex := Typ.AliasFor
  until not UseOriginal or (TypeIndex = 0);
  if Typ.Name <> '' then DeepTypeName := Typ.Name
  else if Typ.Cls = TtcEnum then
  begin
    Ret := '(';
    for Pos := 1 to Defs.Enums[Typ.EnumIndex].Size do
    begin
      if Pos <> 1 then
        Ret := Ret + ',';
      Ret := Ret + Defs.Enums[Typ.EnumIndex].Values[Pos]
    end;
    DeepTypeName := Ret + ')'
  end
  else if Typ.Cls = TtcRecord then
  begin
    Ret := 'record ';
    for Pos := 1 to Defs.Records[Typ.RecordIndex].Size do
    begin
      if Pos <> 1 then Ret := Ret + ',';
      Ret := Ret +
             DeepTypeName(Defs.Records[Typ.RecordIndex].Fields[Pos].TypeIndex,
             true);
      Ret := Ret + ':' + Defs.Records[Typ.RecordIndex].Fields[Pos].Name
    end;
    DeepTypeName := Ret + ' end'
  end
  else if Typ.Cls = TtcArray then
  begin
    Ret := 'array [' + Defs.Arrays[Typ.ArrayIndex].LowBound;
    Ret := Ret + '..' + Defs.Arrays[Typ.ArrayIndex].HighBound;
    Ret := Ret + '] of ' + DeepTypeName(Defs.Arrays[Typ.ArrayIndex].TypeIndex,
           true);
    DeepTypeName := Ret
  end
  else
  begin
    writeln(StdErr, 'Could not get name for type of class ', Typ.Cls, LxWhereStr
            ());
    halt(1)
  end
end;

function TypeName(TypeIndex : TPsTypeIndex) : string;
begin
  TypeName := DeepTypeName(TypeIndex, false)
end;

function EmptyType : TPsType;
var 
  Ret : TPsType;
begin
  Ret.Name := '';
  Ret.Cls := TtcBoolean;
  Ret.EnumIndex := 0;
  Ret.RecordIndex := 0;
  Ret.AliasFor := 0;
  EmptyType := Ret
end;

function TypeOfClass(Cls : TPsTypeClass) : TPsType;
var 
  Ret : TPsType;
begin
  Ret := EmptyType();
  Ret.Cls := Cls;
  TypeOfClass := Ret
end;

function TypeHasClass(TypeIndex : TPsTypeIndex; Cls : TPsTypeClass) : boolean;
begin
  TypeHasClass := Defs.Types[TypeIndex].Cls = Cls
end;

function IntegerType : TPsType;
begin
  IntegerType := TypeOfClass(TtcInteger)
end;

function IsIntegerType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsIntegerType := TypeHasClass(TypeIndex, TtcInteger)
end;

function StringType : TPsType;
begin
  StringType := TypeOfClass(TtcString)
end;

function CharType : TPsType;
begin
  CharType := TypeOfClass(TtcChar)
end;

function IsStringType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsStringType := TypeHasClass(TypeIndex, TtcString)
end;

function IsCharType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsCharType := TypeHasClass(TypeIndex, TtcChar)
end;

function IsStringyType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsStringyType := IsStringType(TypeIndex) or IsCharType(TypeIndex)
end;

function BooleanType : TPsType;
begin
  BooleanType := TypeOfClass(TtcBoolean)
end;

function IsBooleanType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsBooleanType := TypeHasClass(TypeIndex, TtcBoolean)
end;

function TextType : TPsType;
begin
  TextType := TypeOfClass(TtcText)
end;

function IsTextType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsTextType := TypeHasClass(TypeIndex, TtcText)
end;

function IsEnumType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsEnumType := TypeHasClass(TypeIndex, TtcEnum)
end;

function IsRecordType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsRecordType := TypeHasClass(TypeIndex, TtcRecord)
end;

function IsArrayType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsArrayType := TypeHasClass(TypeIndex, TtcArray)
end;

function IsSameType(AIndex : TPsTypeIndex; BIndex : TPsTypeIndex) : boolean;
var 
  A : TPsType;
  B : TPsType;
begin
  A := Defs.Types[AIndex];
  B := Defs.Types[BIndex];
  IsSameType := (A.Cls = B.Cls)
                and (A.EnumIndex = B.EnumIndex)
                and (A.RecordIndex = B.RecordIndex)
end;

function FindType(Name : string) : TPsTypeIndex;
var 
  Pos : integer;
  Ret : TPsTypeIndex;
begin
  Ret := 0;
  for Pos := 1 to Defs.Scope.NumTypes do
    if Name = Defs.Types[Pos].Name then Ret := Pos;
  FindType := Ret
end;

function AddType(Typ : TPsType; Scope : TPsScope) : TPsTypeIndex;
var 
  Pos : integer;
begin
  if Typ.Name <> '' then
  begin
    Pos := FindType(Typ.Name);
    if Pos > Scope.NumTypes then
    begin
      writeln(StdErr, 'Variable ', Typ.Name, ' already defined as ', TypeName(
              Pos), LxWhereStr());
      halt(1)
    end
  end;
  Pos := Defs.Scope.NumTypes + 1;
  if Pos > MaxTypes then
  begin
    writeln(StdErr, 'Too many types have been defined', LxWhereStr());
    halt(1)
  end;
  Defs.Types[Pos] := Typ;
  Defs.Scope.NumTypes := Pos;
  AddType := Pos;
end;

function FindTypeOfEnumValue(Name : string) : TPsTypeIndex;
var 
  Pos : TPsTypeIndex;
  EnumPos : integer;
begin
  FindTypeOfEnumValue := 0;
  for Pos := 1 to Defs.Scope.NumTypes do
    if Defs.Types[Pos].EnumIndex <> 0 then
      for EnumPos := 1 to Defs.Enums[Defs.Types[Pos].EnumIndex].Size do
        if Name = Defs.Enums[Defs.Types[Pos].EnumIndex].Values[EnumPos] then
          FindTypeOfEnumValue := Pos
end;

function AddEnum(Enum : TPsEnumDef) : TPsEnumIndex;
begin
  Defs.Scope.NumEnums := Defs.Scope.NumEnums + 1;
  if Defs.Scope.NumEnums > MaxEnums then
  begin
    writeln(StdErr, 'Too many enums have been defined', LxWhereStr());
    halt(1)
  end;
  Defs.Enums[Defs.Scope.NumEnums] := Enum;
  AddEnum := Defs.Scope.NumEnums
end;

function AddRecord(Rec : TPsRecordDef) : TPsRecordIndex;
begin
  Defs.Scope.NumRecords := Defs.Scope.NumRecords + 1;
  if Defs.Scope.NumRecords > MaxRecords then
  begin
    writeln(StdErr, 'Too many records have been defined', LxWhereStr());
    halt(1)
  end;
  Defs.Records[Defs.Scope.NumRecords] := Rec;
  AddRecord := Defs.Scope.NumRecords
end;

function AddArray(Arr : TPsArrayDef) : TPsArrayIndex;
begin
  Defs.Scope.NumArrays := Defs.Scope.NumArrays + 1;
  if Defs.Scope.NumArrays > MaxArrays then
  begin
    writeln(StdErr, 'Too many arrays have been defined', LxWhereStr());
    halt(1)
  end;
  Defs.Arrays[Defs.Scope.NumArrays] := Arr;
  AddArray := Defs.Scope.NumArrays
end;

function FindConstant(Name : string) : TPsConstantIndex;
var 
  Pos : integer;
  Ret : TPsConstantIndex;
begin
  Ret := 0;
  for Pos := 1 to Defs.Scope.NumConstants do
    if Name = Defs.Constants[Pos].Name then Ret := Pos;
  FindConstant := Ret
end;

function AddConstant(Constant : TPsConstant; Scope : TPsScope)
: TPsConstantIndex;
var 
  Pos : integer;
begin
  if Constant.Name <> '' then
  begin
    Pos := FindConstant(Constant.Name);
    if Pos > Scope.NumConstants then
    begin
      writeln(StdErr, 'Constant ', Constant.Name, ' already defined',
              LxWhereStr());
      halt(1)
    end
  end;
  Pos := Defs.Scope.NumConstants + 1;
  if Pos > MaxConstants then
  begin
    writeln(StdErr, 'Too many constants have been defined', LxWhereStr());
    halt(1)
  end;
  Defs.Constants[Pos] := Constant;
  Defs.Scope.NumConstants := Pos;
  AddConstant := Pos
end;

function FindVariable(Name : string) : TPsVariableIndex;
var 
  Pos : integer;
  Ret : TPsVariableIndex;
begin
  Ret := 0;
  for Pos := 1 to Defs.Scope.NumVariables do
    if Name = Defs.Variables[Pos].Name then Ret := Pos;
  FindVariable := Ret
end;

function AddVariable(VarDef : TPsVariable; Scope : TPsScope)
: TPsVariableIndex;
var 
  Pos : integer;
begin
  if VarDef.Name <> '' then
  begin
    Pos := FindVariable(VarDef.Name);
    if Pos > Scope.NumVariables then
    begin
      writeln(StdErr, 'Variable ', VarDef.Name, ' already defined as ',
              TypeName(Defs.Variables[Pos].TypeIndex), LxWhereStr());
      halt(1)
    end
  end;
  Pos := Defs.Scope.NumVariables + 1;
  if Pos > MaxVariables then
  begin
    writeln(StdErr, 'Too many variables have been defined', LxWhereStr());
    halt(1)
  end;
  Defs.Variables[Pos] := VarDef;
  Defs.Scope.NumVariables := Pos;
  AddVariable := Pos
end;

function EmptyFunction : TPsFunction;
var 
  Ret : TPsFunction;
begin
  Ret.Name := '';
  Ret.ArgCount := 0;
  Ret.ReturnTypeIndex := 0;
  Ret.IsDeclaration := false;
  EmptyFunction := Ret
end;

function IsEmptyFunction(Fn : TPsFunction) : boolean;
begin
  IsEmptyFunction := Fn.Name = ''
end;

function FindFunction(Name : string) : TPsFunctionIndex;
var 
  Pos : integer;
begin
  FindFunction := 0;
  for Pos := 1 to Defs.Scope.NumFunctions do
  begin
    if Defs.Functions[Pos].Name = Name then
      FindFunction := Pos
  end
end;

function IsSameFunctionDefinition(DeclIndex : TPsFunctionIndex; Fun :
                                  TPsFunction) : boolean;
var 
  Decl : TPsFunction;
  Same : boolean;
  Pos : integer;
begin
  Decl := Defs.Functions[DeclIndex];
  Same := IsSameType(Decl.ReturnTypeIndex, Fun.ReturnTypeIndex)
          and (Decl.ArgCount = Fun.ArgCount);
  for Pos := 1 to Decl.ArgCount do
    Same := Same
            and IsSameType(Decl.Args[Pos].TypeIndex, Fun.Args[Pos].TypeIndex)
            and (Decl.Args[Pos].IsReference = Fun.Args[Pos].IsReference);
  IsSameFunctionDefinition := Same
end;

function AddFunction(Fun : TPsFunction) : TPsFunctionIndex;
var 
  Pos : integer;
begin
  Pos := FindFunction(Fun.Name);
  if Pos <> 0 then
  begin
    if Defs.Functions[Pos].IsDeclaration then
    begin
      if not IsSameFunctionDefinition(Pos, Fun) then
      begin
        if Fun.ReturnTypeIndex = 0 then
          writeln(StdErr, 'Procedure ', Fun.Name,
                  ' incompatible with its forward declaration', LxWhereStr())
        else
          writeln(StdErr, 'Function ', Fun.Name,
                  ' incompatible with its forward declaration', LxWhereStr());
        halt(1)
      end
    end
    else
    begin
      if Fun.ReturnTypeIndex = 0 then
        writeln(StdErr, 'Procedure ', Fun.Name, ' already defined',
                LxWhereStr())
      else
        writeln(StdErr, 'Function ', Fun.Name, ' already defined',
                LxWhereStr());
      halt(1)
    end
  end
  else
  begin
    Pos := Defs.Scope.NumFunctions + 1;
    if Pos > MaxFunctions then
    begin
      writeln(StdErr, 'Too many functions have been defined', LxWhereStr());
      halt(1)
    end;
    Defs.Scope.NumFunctions := Pos
  end;
  Defs.Functions[Pos] := Fun;
  AddFunction := Pos
end;

function FindFieldType(TypeIndex : TPsTypeIndex; Name : string) : TPsTypeIndex;
var 
  Typ : TPsType;
  Rec : TPsRecordDef;
  Pos : integer;
begin
  Typ := Defs.Types[TypeIndex];
  if Typ.Cls <> TtcRecord then
  begin
    writeln(StdErr, 'Not a record: ', Typ.Name, LxWhereStr());
    halt(1)
  end;
  TypeIndex := 0;
  Rec := Defs.Records[Typ.RecordIndex];
  for Pos := 1 to Rec.Size do
    if Rec.Fields[Pos].Name = Name then
      TypeIndex := Rec.Fields[Pos].TypeIndex;
  if TypeIndex = 0 then
  begin
    writeln(StdErr, 'Field not found: ', Name, LxWhereStr());
    halt(1)
  end;
  FindFieldType := TypeIndex
end;

function MakeType(Name : string; Cls : TPsTypeClass) : TPsType;
var 
  Typ : TPsType;
begin
  Typ := EmptyType();
  Typ.Name := Name;
  Typ.Cls := Cls;
  MakeType := Typ
end;

function MakeConstant(Name : string; TokenId : TLxTokenId; TokenValue : string)
: TPsConstant;
var 
  Constant : TPsConstant;
begin
  Constant.Name := Name;
  Constant.Replacement.Id := TokenId;
  Constant.Replacement.Value := TokenValue;
  MakeConstant := Constant
end;

function MakeTypedConstant(Name : string; TypeIndex : TPsTypeIndex)
: TPsVariable;
var 
  VarDef : TPsVariable;
begin
  VarDef.Name := Name;
  VarDef.TypeIndex := TypeIndex;
  VarDef.IsReference := false;
  VarDef.IsConstant := true;
  MakeTypedConstant := VarDef
end;

function MakeVariable(Name : string; TypeIndex : TPsTypeIndex; IsRef : boolean)
: TPsVariable;
var 
  VarDef : TPsVariable;
begin
  VarDef.Name := Name;
  VarDef.TypeIndex := TypeIndex;
  VarDef.IsReference := IsRef;
  VarDef.IsConstant := false;
  MakeVariable := VarDef
end;

function IsVariableExpression(Expr : TPsExpression) : boolean;
begin
  IsVariableExpression := (Expr.Cls = TecValue) and (Expr.TypeIndex <> 0)
                          and not Expr.IsConstant
end;

procedure StartGlobalScope;
var 
  Fun : TPsFunction;
begin
  ClearDefs();
  GlobalScope := GetCurrentScope();

  PrimitiveTypes.PtBoolean := AddType(MakeType('BOOLEAN', TtcBoolean),
                              GlobalScope);
  PrimitiveTypes.PtInteger := AddType(MakeType('INTEGER', TtcInteger),
                              GlobalScope);
  PrimitiveTypes.PtChar := AddType(MakeType('CHAR', TtcChar), GlobalScope);
  PrimitiveTypes.PtString := AddType(MakeType('STRING', TtcString),
                             GlobalScope);
  PrimitiveTypes.PtText := AddType(MakeType('TEXT', TtcText), GlobalScope);

  AddConstant(MakeConstant('FALSE', TkFalse, 'FALSE'), GlobalScope);
  AddConstant(MakeConstant('TRUE', TkTrue, 'TRUE'), GlobalScope);

  AddVariable(MakeVariable('INPUT', PrimitiveTypes.PtText, false), GlobalScope);
  AddVariable(MakeVariable('OUTPUT', PrimitiveTypes.PtText, false),
  GlobalScope);
  AddVariable(MakeVariable('STDERR', PrimitiveTypes.PtText, false),
  GlobalScope);

  Fun.Name := 'ASSIGN';
  Fun.ArgCount := 2;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.Args[2] := MakeVariable('NAME', PrimitiveTypes.PtString, false);
  Fun.ReturnTypeIndex := 0;
  AddFunction(Fun);
  Fun.Name := 'CLOSE';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.ReturnTypeIndex := 0;
  AddFunction(Fun);
  Fun.Name := 'COPY';
  Fun.ArgCount := 3;
  Fun.Args[1] := MakeVariable('STR', PrimitiveTypes.PtString, false);
  Fun.Args[2] := MakeVariable('POS', PrimitiveTypes.PtInteger, false);
  Fun.Args[3] := MakeVariable('NUM', PrimitiveTypes.PtInteger, false);
  Fun.ReturnTypeIndex := PrimitiveTypes.PtString;
  AddFunction(Fun);
  Fun.Name := 'DELETE';
  Fun.ArgCount := 3;
  Fun.Args[1] := MakeVariable('STR', PrimitiveTypes.PtString, true);
  Fun.Args[2] := MakeVariable('POS', PrimitiveTypes.PtInteger, false);
  Fun.Args[3] := MakeVariable('NUM', PrimitiveTypes.PtInteger, false);
  Fun.ReturnTypeIndex := 0;
  AddFunction(Fun);
  Fun.Name := 'EOF';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.ReturnTypeIndex := PrimitiveTypes.PtBoolean;
  AddFunction(Fun);
  Fun.Name := 'HALT';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('CODE', PrimitiveTypes.PtInteger, false);
  Fun.ReturnTypeIndex := 0;
  AddFunction(Fun);
  Fun.Name := 'LENGTH';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('STR', PrimitiveTypes.PtString, false);
  Fun.ReturnTypeIndex := PrimitiveTypes.PtInteger;
  AddFunction(Fun);
  Fun.Name := 'PARAMCOUNT';
  Fun.ArgCount := 0;
  Fun.ReturnTypeIndex := PrimitiveTypes.PtInteger;
  AddFunction(Fun);
  Fun.Name := 'PARAMSTR';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('I', PrimitiveTypes.PtInteger, false);
  Fun.ReturnTypeIndex := PrimitiveTypes.PtString;
  AddFunction(Fun);
  Fun.Name := 'RESET';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.ReturnTypeIndex := 0;
  AddFunction(Fun);
  Fun.Name := 'UPCASE';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('CHR', PrimitiveTypes.PtChar, false);
  Fun.ReturnTypeIndex := PrimitiveTypes.PtChar;
  AddFunction(Fun);
end;

function PsTypeDenoter(Scope : TPsScope) : TPsTypeIndex;
var 
  TypeIndex : TPsTypeIndex;
  Typ : TPsType;
  Enum : TPsEnumDef;
  Rec : TPsRecordDef;
  Arr : TPsArrayDef;
begin
  TypeIndex := 0;
  if LxToken.Id = TkIdentifier then
  begin
    TypeIndex := FindType(LxToken.Value);
    if TypeIndex = 0 then
    begin
      writeln(StdErr, 'Unknown type: ', LxToken.Value, LxWhereStr());
      halt(1)
    end;
    ReadToken()
  end
  else if LxToken.Id = TkLparen then
  begin
    SkipToken(TkLparen);
    Enum.Size := 0;
    repeat
      Enum.Size := Enum.Size + 1;
      if Enum.Size > MaxEnumValues then
      begin
        writeln(StdErr, 'Too many values in enum', LxWhereStr());
        halt(1)
      end;
      Enum.Values[Enum.Size] := GetTokenValueAndRead(TkIdentifier);
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
    until LxToken.Id = TkRparen;
    Typ := EmptyType();
    Typ.Cls := TtcEnum;
    Typ.EnumIndex := AddEnum(Enum);
    TypeIndex := AddType(Typ, Scope);
    SkipToken(TkRparen)
  end
  else if LxToken.Id = TkRecord then
  begin
    SkipToken(TkRecord);
    Rec.Size := 0;
    repeat
      Rec.Size := Rec.Size + 1;
      if Rec.Size > MaxRecordFields then
      begin
        writeln(StdErr, 'Too many fields in record', LxWhereStr());
        halt(1)
      end;
      Rec.Fields[Rec.Size].Name := GetTokenValueAndRead(TkIdentifier);
      WantTokenAndRead(TkColon);
      Rec.Fields[Rec.Size].TypeIndex := PsTypeDenoter(Scope);
      WantToken2(TkSemicolon, TkEnd);
      SkipToken(TkSemicolon);
    until LxToken.Id = TkEnd;
    Typ := TypeOfClass(TtcRecord);
    Typ.RecordIndex := AddRecord(Rec);
    TypeIndex := AddType(Typ, Scope);
    SkipToken(TkEnd)
  end
  else if LxToken.Id = TkArray then
  begin
    SkipToken(TkArray);
    WantTokenAndRead(TkLbracket);
    Arr.LowBound := GetTokenValueAndRead(TkNumber);
    WantTokenAndRead(TkRange);
    Arr.HighBound := GetTokenValueAndRead(TkNumber);
    WantTokenAndRead(TkRbracket);
    WantTokenAndRead(TkOf);
    Arr.TypeIndex := PsTypeDenoter(Scope);
    Typ := TypeOfClass(TtcArray);
    Typ.ArrayIndex := AddArray(Arr);
    TypeIndex := AddType(Typ, Scope)
  end
  else
  begin
    writeln(StdErr, 'Wanted type definition, found ', LxTokenStr(),
    LxWhereStr());
    halt(1)
  end;
  PsTypeDenoter := TypeIndex;
end;

procedure OutBegin;
begin
  writeln(CpOutput, '{')
end;

procedure OutEnd;
begin
  writeln(CpOutput, '}')
end;

procedure OutEnumValues(Pos : TPsEnumIndex);
var 
  PosInEnum : integer;
begin
  write(CpOutput, 'const char* EnumValues', Pos, '[] = { ');
  for PosInEnum := 1 to Defs.Enums[Pos].Size do
  begin
    if PosInEnum <> 1 then write(CpOutput, ', ');
    write(CpOutput, '"', Defs.Enums[Pos].Values[PosInEnum], '"')
  end;
  writeln(CpOutput, ' };')
end;

procedure OutEnumValuesInScope(Scope : TPsScope);
var 
  Pos : TPsEnumIndex;
begin
  for Pos := Scope.NumEnums + 1 to Defs.Scope.NumEnums do
    OutEnumValues(Pos)
end;

function OutVariableName(Name : string; IsReference : boolean) : string;
begin
  if IsReference then OutVariableName := '*' + Name
  else OutVariableName := Name
end;

function OutReturnVariableName(Name : string) : string;
begin
  OutReturnVariableName := 'return_' + Name
end;

procedure OutNameAndType(Name : string; TypeIndex : TPsTypeIndex);
var 
  Typ : TPsType;
  Enum : TPsEnumDef;
  Rec : TPsRecordDef;
  Arr : TPsArrayDef;
  Pos : integer;
begin
  if TypeIndex <> 0 then
    Typ := Defs.Types[TypeIndex];
  if TypeIndex = 0 then write(CpOutput, 'void ', Name)
  else if (Typ.AliasFor <> 0) and (Typ.Name <> '') then write(CpOutput, Typ.Name,
                                                              ' ', Name)
  else if Typ.Cls = TtcBoolean then write(CpOutput, 'PBoolean ', Name)
  else if Typ.Cls = TtcInteger then write(CpOutput, 'int ', Name)
  else if Typ.Cls = TtcChar then write(CpOutput, 'char ', Name)
  else if Typ.Cls = TtcString then write(CpOutput, 'STRING ', Name)
  else if Typ.Cls = TtcText then write(CpOutput, 'PFile ', Name)
  else if Typ.Cls = TtcEnum then
  begin
    Enum := Defs.Enums[Typ.EnumIndex];
    write(CpOutput, 'enum { ');
    for Pos := 1 to Enum.Size do
    begin
      if Pos > 1 then
        write(CpOutput, ', ');
      write(CpOutput, Enum.Values[Pos])
    end;
    write(CpOutput, '} ', Name)
  end
  else if Typ.Cls = TtcRecord then
  begin
    Rec := Defs.Records[Typ.RecordIndex];
    write(CpOutput, 'struct { ');
    for Pos := 1 to Rec.Size do
    begin
      OutNameAndType(Rec.Fields[Pos].Name, Rec.Fields[Pos].TypeIndex);
      write(CpOutput, '; ')
    end;
    write(CpOutput, '} ', Name)
  end
  else if Typ.Cls = TtcArray then
  begin
    Arr := Defs.Arrays[Typ.ArrayIndex];
    OutNameAndType(Name, Arr.TypeIndex);
    write(CpOutput, '[1 + ', Arr.HighBound, ' - ', Arr.LowBound, ']')
  end
  else
  begin
    writeln(StdErr, 'Error writing name and type: ', Name, ', ',
            TypeName(TypeIndex), LxWhereStr());
    halt(1)
  end
end;

procedure OutTypeDefinition(TypeIndex : TPsTypeIndex);
var 
  Name : string;
begin
  Name := Defs.Types[TypeIndex].Name;
  if Defs.Types[TypeIndex].AliasFor = 0 then
  begin
    writeln(StdErr, 'Type ', Name, ' is not an alias', LxWhereStr());
    halt(1)
  end;
  write(CpOutput, 'typedef ');
  OutNameAndType(Name, Defs.Types[TypeIndex].AliasFor);
  writeln(CpOutput, ';');
end;

procedure PsTypeDefinitions(Scope : TPsScope);
var 
  Name : string;
  TypeIndex : TPsTypeIndex;
  NewType : TPsType;
  PreviousScope : TPsScope;
begin
  PreviousScope := GetCurrentScope();
  WantTokenAndRead(TkType);
  repeat
    Name := GetTokenValueAndRead(TkIdentifier);
    WantTokenAndRead(TkEquals);
    TypeIndex := PsTypeDenoter(Scope);
    NewType := Defs.Types[TypeIndex];
    NewType.Name := Name;
    NewType.AliasFor := TypeIndex;
    TypeIndex := AddType(NewType, Scope);
    WantTokenAndRead(TkSemicolon);
    OutTypeDefinition(TypeIndex);
  until LxToken.Id <> TkIdentifier;
  OutEnumValuesInScope(PreviousScope)
end;

procedure PsConstant(Name : string; Scope : TPsScope);
var 
  Constant : TPsConstant;
begin
  WantTokenAndRead(TkEquals);
  Constant.Name := Name;
  if (LxToken.Id = TkFalse) or (LxToken.Id = TkTrue) or
     (LxToken.Id = TkNumber) or (LxToken.Id = TkString) then
    Constant.Replacement := LxToken
  else
  begin
    writeln(Stderr, 'Expected constant value, found ', LxTokenStr(),
    LxWhereStr());
    halt(1)
  end;
  AddConstant(Constant, Scope);
  ReadToken();
end;

function GenBooleanConstant(Value : boolean) : TPsExpression;
var 
  Expr : TPsExpression;
begin
  Expr.TypeIndex := PrimitiveTypes.PtBoolean;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  if Value then
    Expr.Value := '1'
  else
    Expr.Value := '0';
  GenBooleanConstant := Expr
end;

function GenStringConstant(Value : string) : TPsExpression;
var 
  Expr : TPsExpression;
  Size : string;
  Pos : integer;
  InStr : boolean;
  LastQuote : boolean;
  Len : integer;
begin
  InStr := false;
  LastQuote := false;
  Expr.Value := '';
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  Len := 0;
  for Pos := 1 to Length(Value) do
  begin
    if Value[Pos] = '''' then
    begin
      InStr := not InStr;
      if InStr and LastQuote then
      begin
        Expr.Value := Expr.Value + '\''';
        Len := Len + 1
      end
      else
        LastQuote := not InStr
    end
    else
    begin
      LastQuote := false;
      if Value[Pos] = '"' then Expr.Value := Expr.Value + '\"'
      else if Value[Pos] = '\' then Expr.Value := Expr.Value + '\\'
      else Expr.Value := Expr.Value + Value[Pos];
      Len := Len + 1
    end
  end;
  if Len = 1 then
  begin
    Expr.Value := '''' + Expr.Value + '''';
    Expr.TypeIndex := PrimitiveTypes.PtChar;
  end
  else
  begin
    Str(Len, Size);
    Expr.Value := 'str_make(' + Size + ', "' + Expr.Value + '")';
    Expr.TypeIndex := PrimitiveTypes.PtString;
  end;
  GenStringConstant := Expr
end;

function GenNumberConstant(Value : string) : TPsExpression;
var 
  Expr : TPsExpression;
begin
  Expr.TypeIndex := PrimitiveTypes.PtInteger;
  Expr.Value := Value;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  GenNumberConstant := Expr
end;

procedure OutConstantValue(Value : string);
begin
  write(CpOutput, Value)
end;

procedure OutConstantArrayBegin;
begin
  write(CpOutput, '{ ')
end;

procedure OutConstantArraySeparator;
begin
  write(CpOutput, ', ')
end;

procedure OutConstantArrayEnd;
begin
  write(CpOutput, ' }')
end;

procedure PsConstantValue(TypeIndex : TPsTypeIndex);
var 
  Expr : TPsExpression;
begin
  if IsBooleanType(TypeIndex) then
  begin
    WantToken2(TkFalse, TkTrue);
    Expr := GenBooleanConstant(LxToken.Id = TkTrue);
    ReadToken();
    OutConstantValue(Expr.Value)
  end
  else if IsIntegerType(TypeIndex) then
  begin
    Expr := GenNumberConstant(GetTokenValueAndRead(TkNumber));
    OutConstantValue(Expr.Value)
  end
  else if IsCharType(TypeIndex) then
  begin
    Expr := GenStringConstant(GetTokenValueAndRead(TkString));
    if not IsCharType(Expr.TypeIndex) then
    begin
      writeln(StdErr, 'Expected char constant, got ',
              TypeName(Expr.TypeIndex), LxWhereStr());
      halt(1)
    end;
    OutConstantValue(Expr.Value)
  end
  else if IsStringType(TypeIndex) then
  begin
    Expr := GenStringConstant(GetTokenValueAndRead(TkString));
    OutConstantValue(Expr.Value)
  end
  else if IsArrayType(TypeIndex) then
  begin
    WantTokenAndRead(TkLparen);
    TypeIndex := Defs.Arrays[Defs.Types[TypeIndex].ArrayIndex].TypeIndex;
    OutConstantArrayBegin();
    while LxToken.Id <> TkRparen do
    begin
      PsConstantValue(TypeIndex);
      WantToken2(TkComma, TkRparen);
      if LxToken.Id = TkComma then OutConstantArraySeparator();
      SkipToken(TkComma)
    end;
    OutConstantArrayEnd();
    WantTokenAndRead(TkRparen);
  end
  else
  begin
    writeln(StdErr, 'Invalid type for constant: ', TypeName(TypeIndex),
    LxWhereStr());
    halt(1)
  end
end;

procedure OutVariableDeclaration(VarDef : TPsVariable);
begin
  OutNameAndType(OutVariableName(VarDef.Name, VarDef.IsReference),
  VarDef.TypeIndex)
end;

procedure OutVariableDefinition(VarIndex : TPsVariableIndex);
begin
  if Defs.Variables[VarIndex].IsConstant then
    write(CpOutput, 'const ');
  OutVariableDeclaration(Defs.Variables[VarIndex]);
  writeln(CpOutput, ';');
end;

procedure OutConstantDefinitionBegin(VarIndex : TPsVariableIndex);
begin
  write(CpOutput, 'const ');
  OutVariableDeclaration(Defs.Variables[VarIndex]);
  write(CpOutput, ' = ');
end;

procedure OutConstantDefinitionEnd;
begin
  writeln(CpOutput, ';')
end;

procedure PsTypedConstant(Name : string; Scope : TPsScope);
var 
  TypeIndex : TPsTypeIndex;
begin
  WantTokenAndRead(TkColon);
  TypeIndex := PsTypeDenoter(Scope);
  WantTokenAndRead(TkEquals);
  OutConstantDefinitionBegin(AddVariable(MakeTypedConstant(Name, TypeIndex),
  Scope));
  PsConstantValue(TypeIndex);
  OutConstantDefinitionEnd()
end;

procedure PsConstDefinitions(Scope : TPsScope);
var 
  Name : string;
begin
  WantTokenAndRead(TkConst);
  repeat
    Name := GetTokenValueAndRead(TkIdentifier);
    WantToken2(TkEquals, TkColon);
    if LxToken.Id = TkEquals then
      PsConstant(Name, Scope)
    else
      PsTypedConstant(Name, Scope);
    WantTokenAndRead(TkSemicolon)
  until LxToken.Id <> TkIdentifier;
end;

procedure PsVarDefinitions(Scope : TPsScope);
var 
  Name : string;
  TypeIndex : TPsTypeIndex;
  PreviousScope : TPsScope;
begin
  PreviousScope := GetCurrentScope();
  WantTokenAndRead(TkVar);
  repeat
    Name := GetTokenValueAndRead(TkIdentifier);
    WantTokenAndRead(TkColon);
    TypeIndex := PsTypeDenoter(Scope);
    WantTokenAndRead(TkSemicolon);
    OutVariableDefinition(AddVariable(MakeVariable(Name, TypeIndex, false),
    Scope));
  until LxToken.Id <> TkIdentifier;
  OutEnumValuesInScope(PreviousScope)
end;

procedure OutFunctionPrototype(Def : TPsFunction);
var 
  Pos : integer;
begin
  OutNameAndType(Def.Name, Def.ReturnTypeIndex);
  write(CpOutput, '(');
  for Pos := 1 to Def.ArgCount do
  begin
    OutVariableDeclaration(def.Args[Pos]);
    if Pos <> Def.ArgCount then
      write(CpOutput, ', ')
  end;
  write(CpOutput, ')')
end;

procedure OutFunctionDeclaration(FnIndex : TPsFunctionIndex);
begin
  OutFunctionPrototype(Defs.Functions[FnIndex]);
  writeln(CpOutput, ';')
end;

procedure OutFunctionDefinition(FnIndex : TPsFunctionIndex);
var 
  Fun : TPsFunction;
begin
  Fun := Defs.Functions[FnIndex];
  OutFunctionPrototype(Fun);
  writeln(CpOutput, ' {');
  if Fun.ReturnTypeIndex <> 0 then
  begin
    OutNameAndType(OutReturnVariableName(Fun.Name), Fun.ReturnTypeIndex);
    writeln(CpOutput, ';')
  end
end;

procedure OutFunctionEnd(FnIndex : TPsFunctionIndex);
begin
  if Defs.Functions[FnIndex].ReturnTypeIndex <> 0 then
    writeln(CpOutput, 'return ',
            OutReturnVariableName(Defs.Functions[FnIndex].Name), ';');
  writeln(CpOutput, '}')
end;

procedure PsStatement;
forward;

procedure PsDefinitions(Scope : TPsScope);
forward;

procedure AddFuncArgsToScope(FnIndex : TPsFunctionIndex; Scope : TPsScope);
var 
  Pos : integer;
begin
  for Pos := 1 to Defs.Functions[FnIndex].ArgCount do
    AddVariable(Defs.Functions[FnIndex].Args[Pos], Scope)
end;

procedure PsFunctionBody(FnIndex : TPsFunctionIndex);
var 
  PreviousScope : TPsScope;
begin
  if LxToken.Id = TkForward then
  begin
    SkipToken(TkForward);
    WantTokenAndRead(TkSemicolon);
    Defs.Functions[FnIndex].IsDeclaration := true;
    OutFunctionDeclaration(FnIndex);
  end
  else
  begin
    PreviousScope := GetCurrentScope();
    AddFuncArgsToScope(FnIndex, PreviousScope);
    OutFunctionDefinition(FnIndex);
    OutEnumValuesInScope(PreviousScope);
    PsDefinitions(PreviousScope);
    WantTokenAndRead(TkBegin);
    while LxToken.Id <> TkEnd do
    begin
      PsStatement();
      WantToken2(TkSemicolon, TkEnd);
      SkipToken(TkSemicolon)
    end;
    WantTokenAndRead(TkEnd);
    WantTokenAndRead(TkSemicolon);
    OutFunctionEnd(FnIndex);
    SetCurrentScope(PreviousScope);
  end
end;

procedure PsFunctionDefinition;
var 
  IsProcedure : boolean;
  Def : TPsFunction;
begin
  WantToken2(TkFunction, TkProcedure);
  IsProcedure := LxToken.Id = TkProcedure;
  ReadToken();
  Def.Name := GetTokenValueAndRead(TkIdentifier);
  Def.ArgCount := 0;
  if IsProcedure then WantToken2(TkLparen, TkSemicolon)
  else WantToken2(TkLparen, TkColon);
  if LxToken.Id = TkLparen then
  begin
    WantTokenAndRead(TkLparen);
    repeat
      Def.ArgCount := Def.ArgCount + 1;
      if Def.ArgCount > MaxFunctionArguments then
      begin
        writeln(StdErr, 'Too many arguments declared for function ', Def.Name,
                LxWhereStr());
        halt(1)
      end;
      Def.Args[Def.ArgCount].IsReference := LxToken.Id = TkVar;
      SkipToken(TkVar);
      Def.Args[Def.ArgCount].Name := GetTokenValueAndRead(TkIdentifier);
      WantTokenAndRead(TkColon);
      Def.Args[Def.ArgCount].TypeIndex := PsTypeDenoter(GlobalScope);
      WantToken2(TkSemicolon, TkRparen);
      SkipToken(TkSemicolon);
    until LxToken.Id = TkRparen;
    SkipToken(TkRparen)
  end;
  if IsProcedure then
    Def.ReturnTypeIndex := 0
  else
  begin
    WantTokenAndRead(TkColon);
    Def.ReturnTypeIndex := PsTypeDenoter(GlobalScope);
  end;
  WantTokenAndRead(TkSemicolon);
  PsFunctionBody(AddFunction(Def));
end;

procedure PsDefinitions(Scope : TPsScope);
var 
  Done : boolean;
begin
  Done := false;
  repeat
    if LxToken.Id = TkType then
      PsTypeDefinitions(Scope)
    else if LxToken.Id = TkConst then PsConstDefinitions(Scope)
    else if LxToken.Id = TkVar then PsVarDefinitions(Scope)
    else if (LxToken.Id = TkProcedure) or (LxToken.Id = TkFunction) then
           PsFunctionDefinition()
    else
      Done := true;
  until Done;
end;

procedure OutProgramHeading(Name : string);
begin
  writeln(CpOutput, '/* Program: ', Name, ' */');
  writeln(CpOutput, '#include "pascual.h"')
end;

procedure PsProgramHeading;
begin
  WantTokenAndRead(TkProgram);
  OutProgramHeading(GetTokenValueAndRead(TkIdentifier));
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

function PsExpression : TPsExpression;
forward;

function PsIdentifier : TPsIdentifier;
var 
  Ident : TPsIdentifier;
begin
  Ident.Name := GetTokenValueAndRead(TkIdentifier);
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

function ShortTypeName(TypeIndex : TPsTypeIndex) : char;
begin
  if IsBooleanType(TypeIndex) then ShortTypeName := 'b'
  else if IsIntegerType(TypeIndex) then ShortTypeName := 'i'
  else if IsCharType(TypeIndex) then ShortTypeName := 'c'
  else if IsStringType(TypeIndex) then ShortTypeName := 's'
  else if IsEnumType(TypeIndex) then ShortTypeName := 'e'
  else
    writeln(StdErr, 'No short type name exists for ', TypeName(TypeIndex),
    LxWhereStr())
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
    writeln(StdErr, 'Expected binary operator, found ', Op, LxWhereStr());
    halt(1)
  end;
  if Cmp = '' then
    Expr.TypeIndex := PrimitiveTypes.PtInteger
  else
    Expr.TypeIndex := PrimitiveTypes.PtBoolean;
  Expr.Value := Left.Value + ' ' + Oper + Cmp + ' ' + Right.Value;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
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
    writeln(StdErr, 'Expected binary operator, found ', Op, LxWhereStr());
    halt(1)
  end;
  Expr.TypeIndex := PrimitiveTypes.PtBoolean;
  Expr.Value := Left.Value + ' ' + Oper + ' ' + Right.Value;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  BooleanBinaryExpression := Expr
end;


function StringyBinaryExpression(Left : TPsExpression; Op : TLxTokenId;
                                 Right : TPsExpression) : TPsExpression;
var 
  FName : string;
  Cmp : string;
  Expr : TPsExpression;
begin
  FName := 'cmp';
  Cmp := '';
  if Op = TkPlus then FName := 'cat'
  else if Op = TkEquals then Cmp := '=='
  else if Op = TkNotEquals then Cmp := '!='
  else if Op = TkLessthan then Cmp := '<'
  else if Op = TkMorethan then Cmp := '>'
  else if Op = TkLessOrEquals then Cmp := '<='
  else if Op = TkMoreOrEquals then Cmp := '>='
  else
  begin
    writeln(StdErr, 'Expected binary operator, found ', Op, LxWhereStr());
    halt(1)
  end;

  FName := FName + '_' + ShortTypeName(Left.TypeIndex) +
           ShortTypeName(Right.TypeIndex);

  Expr.TypeIndex := PrimitiveTypes.PtString;
  Expr.Value := FName + '(' + Left.Value + ', ' + Right.Value + ')';
  if Cmp <> '' then
  begin
    Expr.TypeIndex := PrimitiveTypes.PtBoolean;
    Expr.Value := Expr.Value + ' ' + Cmp + ' 0'
  end;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  StringyBinaryExpression := Expr
end;

function EnumBinaryExpression(Left : TPsExpression; Op : TLxTokenId;
                              Right : TPsExpression) : TPsExpression;
var 
  Cmp : string;
  Expr : TPsExpression;
begin
  Cmp := '';
  if Op = TkEquals then Cmp := '=='
  else if Op = TkNotEquals then Cmp := '!='
  else if Op = TkLessthan then Cmp := '<'
  else if Op = TkMorethan then Cmp := '>'
  else if Op = TkLessOrEquals then Cmp := '<='
  else if Op = TkMoreOrEquals then Cmp := '>='
  else
  begin
    writeln(StdErr, 'Expected binary operator, found ', Op, LxWhereStr());
    halt(1)
  end;
  Expr.TypeIndex := PrimitiveTypes.PtBoolean;
  Expr.Value := Left.Value + ' ' + Cmp + ' ' + Right.Value;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  EnumBinaryExpression := Expr
end;

function BinaryExpression(Left : TPsExpression; Op : TLxTokenId; Right :
                          TPsExpression) : TPsExpression;
begin
  if IsBooleanType(Left.TypeIndex) and IsBooleanType(Right.TypeIndex) then
    BinaryExpression := BooleanBinaryExpression(Left, Op, Right)
  else if IsIntegerType(Left.TypeIndex) and IsIntegerType(Right.TypeIndex)
         then
         BinaryExpression := IntegerBinaryExpression(Left, Op, Right)
  else if IsStringyType(Left.TypeIndex) and IsStringyType(Right.TypeIndex)
         then
         BinaryExpression := StringyBinaryExpression(Left, Op, Right)
  else if IsEnumType(Left.TypeIndex)
          and IsSameType(Left.TypeIndex, Right.TypeIndex) then
         BinaryExpression := EnumBinaryExpression(Left, Op, Right)
  else
  begin
    writeln(StdErr, 'Type mismatch for operator ', Op,
            ': ', TypeName(Left.TypeIndex), ' and ', TypeName(Right.TypeIndex)
    ,
    LxWhereStr());
    halt(1)
  end
end;

function UnaryExpression(Op : TLxTokenId; Expr : TPsExpression)
: TPsExpression;
begin
  if Op <> TkNot then
  begin
    writeln(StdErr, 'Expected unary operator, found ', Op, LxWhereStr());
    halt(1)
  end
  else if not IsBooleanType(Expr.TypeIndex) then
  begin
    writeln(StdErr, 'Expected boolean expression, got ',
            TypeName(Expr.TypeIndex), LxWhereStr());
    halt(1)
  end
  else
    Expr.Value := '!' + Expr.Value;
  Expr.IsConstant := true;
  UnaryExpression := Expr
end;

function CoerceType(Expr : TPsExpression; TypeIndex : TPsTypeIndex)
: TPsExpression;
begin
  if Expr.Cls <> TecValue then
  begin
    writeln(StdErr, 'Cannot assign function to ', TypeName(TypeIndex),
    LxWhereStr());
    halt(1)
  end;
  if IsCharType(Expr.TypeIndex) and IsStringType(TypeIndex) then
  begin
    Expr.TypeIndex := PrimitiveTypes.PtString;
    Expr.Value := 'str_of(' + Expr.Value + ')';
  end
  else if not IsSameType(Expr.TypeIndex, TypeIndex) then
  begin
    writeln(StdErr, 'Cannot assign ', TypeName(Expr.TypeIndex), ' to ',
    TypeName(TypeIndex), LxWhereStr());
    halt(1)
  end;
  CoerceType := Expr
end;

function GenFunctionCallStart(Fn : string) : string;
begin
  GenFunctionCallStart := Fn + '('
end;

function GenFunctionCallEnd(Fn : string) : string;
begin
  GenFunctionCallEnd := Fn + ')'
end;

function GenFunctionCallArgument(Fn : string; Expr : TPsExpression;
                                 IsReference : boolean;
                                 ArgNum : integer) : string;
begin
  if ArgNum <> 1 then Fn := Fn + ', ';
  if IsReference then Fn := Fn + '&' + Expr.Value
  else Fn := Fn + Expr.Value;
  GenFunctionCallArgument := Fn
end;

function GenParens(Expr : TPsExpression) : TPsExpression;
begin
  Expr.Value := '(' + Expr.Value + ')';
  GenParens := Expr
end;

function PsFunctionCall(Fn : TPsExpression) : TPsExpression;
var 
  Fun : TPsFunction;
  Expr : TPsExpression;
  ArgNum : integer;
begin
  if Fn.Cls <> TecFunction then
  begin
    writeln(StdErr, 'Not a function', LxWhereStr());
    halt(1)
  end;
  Fun := Defs.Functions[Fn.FunctionIndex];
  Fn.Value := GenFunctionCallStart(Fn.Value);
  WantTokenAndRead(TkLparen);
  for ArgNum := 1 to Fun.ArgCount do
  begin
    if ArgNum <> 1 then WantTokenAndRead(TkComma);
    Expr := CoerceType(PsExpression(), Fun.Args[ArgNum].TypeIndex);
    Fn.Value := GenFunctionCallArgument(Fn.Value, Expr,
                Fun.Args[ArgNum].IsReference, ArgNum)
  end;
  WantTokenAndRead(TkRparen);
  Fn.Value := GenFunctionCallEnd(Fn.Value);
  Fn.Cls := TecValue;
  Fn.TypeIndex := Fun.ReturnTypeIndex;
  Fn.IsConstant := true;
  PsFunctionCall := Fn
end;

procedure OutRead(Src : string; OutVar : TPsExpression);
begin
  writeln(CpOutput, 'read_', ShortTypeName(OutVar.TypeIndex), '(', Src,
  ', &', OutVar.Value, ');')
end;

procedure OutReadln(Src : string);
begin
  writeln(CpOutput, 'readln(', Src, ');')
end;

procedure PsRead(Id : TPsIdentifier);
var 
  Src : string;
  LineFeed : boolean;
  OutVar : TPsExpression;
begin
  LineFeed := Id.Name = 'READLN';
  OutBegin();
  Src := 'INPUT';
  WantTokenAndRead(TkLparen);
  if LxToken.Id <> TkRparen then
  begin
    OutVar := PsExpression();
    if IsVariableExpression(OutVar) and IsTextType(OutVar.TypeIndex) then
      Src := OutVar.Value
    else
    begin
      if not IsVariableExpression(OutVar)
         or not IsStringyType(OutVar.TypeIndex) then
      begin
        writeln(StdErr, 'Invalid expression for read argument', LxWhereStr());
        halt(1)
      end;
      OutRead(Src, OutVar);
    end;
    WantToken2(TkComma, TkRparen);
    SkipToken(TkComma);
    while LxToken.Id <> TkRparen do
    begin
      OutVar := PsExpression();
      if not IsVariableExpression(OutVar)
         or not IsStringyType(OutVar.TypeIndex) then
      begin
        writeln(StdErr, 'Invalid expression for read argument', LxWhereStr());
        halt(1)
      end;
      OutRead(Src, OutVar);
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma)
    end;
  end;
  WantTokenAndRead(TkRparen);
  if LineFeed then
    OutReadln(Src);
  OutEnd()
end;

procedure OutWrite(Dst : string; Expr : TPsExpression);
begin
  if Defs.Types[Expr.TypeIndex].Cls = TtcEnum then
    writeln(CpOutput, 'write_e(', Dst, ', ', Expr.Value, ', EnumValues',
            Defs.Types[Expr.TypeIndex].EnumIndex, ');')
  else
    writeln(CpOutput, 'write_', ShortTypeName(Expr.TypeIndex),
    '(', Dst, ', ', Expr.Value, ');')
end;

procedure OutWriteln(Src : string);
begin
  writeln(CpOutput, 'writeln(', Src, ');')
end;

procedure PsWrite(Id : TPsIdentifier);
var 
  Dst : string;
  LineFeed : boolean;
  Expr : TPsExpression;
begin
  LineFeed := Id.Name = 'WRITELN';
  OutBegin();
  Dst := 'OUTPUT';
  WantTokenAndRead(TkLparen);
  if LxToken.Id <> TkRparen then
  begin
    Expr := PsExpression();
    if IsVariableExpression(Expr) and IsTextType(Expr.TypeIndex) then
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
  end;
  WantTokenAndRead(TkRparen);
  if LineFeed then
    OutWriteln(Dst);
  OutEnd()
end;

procedure OutStr(Dst : string; Expr : TPsExpression);
begin
  if Defs.Types[Expr.TypeIndex].Cls = TtcEnum then
    writeln(CpOutput, Dst, ' = to_str_e(', Expr.Value, ', EnumValues',
            Defs.Types[Expr.TypeIndex].EnumIndex, ');')
  else
    writeln(CpOutput, Dst, ' = to_str_', ShortTypeName(Expr.TypeIndex),
    '(', Expr.Value, ');')
end;

procedure PsStr;
var 
  Expr : TPsExpression;
  Dest : TPsExpression;
begin
  WantTokenAndRead(TkLparen);
  Expr := PsExpression();
  WantTokenAndRead(TkComma);
  Dest := PsExpression();
  if not IsVariableExpression(Dest) or not IsStringType(Dest.TypeIndex) then
  begin
    writeln(StdErr, 'Destination argument is not a string variable',
            LxWhereStr());
    halt(1)
  end;
  WantTokenAndRead(TkRparen);
  OutStr(Dest.Value, Expr)
end;

procedure SetStringIndex(var Str : TPsExpression; Idx : TPsExpression);
begin
  Str.Value := Str.Value + '.chr[' + Idx.Value + ']';
end;

procedure SetArrayIndex(var Arr : TPsExpression; Idx : TPsExpression);
begin
  Arr.Value := Arr.Value + '[(' + Idx.Value + ') - '
               + Defs.Arrays[Defs.Types[Arr.TypeIndex].ArrayIndex].LowBound
               + ']'
end;

function PsArrayAccess(Arr : TPsExpression) : TPsExpression;
var 
  Idx : TPsExpression;
begin
  WantTokenAndRead(TkLbracket);
  Idx := PsExpression();
  WantTokenAndRead(TkRbracket);
  if (Idx.Cls <> TecValue) or not IsIntegerType(Idx.TypeIndex) then
  begin
    writeln(StdErr, 'Subscript must be an integer', LxWhereStr());
    halt(1)
  end;
  if (Arr.Cls = TecValue) and IsStringType(Arr.TypeIndex) then
  begin
    SetStringIndex(Arr, Idx);
    Arr.TypeIndex := PrimitiveTypes.PtChar
  end
  else if (Arr.Cls = TecValue) and IsArrayType(Arr.TypeIndex) then
  begin
    SetArrayIndex(Arr, Idx);
    Arr.TypeIndex := Defs.Arrays[Defs.Types[Arr.TypeIndex].ArrayIndex].TypeIndex
  end
  else
  begin
    writeln(StdErr, 'Not a string or array', LxWhereStr());
    halt(1)
  end;
  PsArrayAccess := Arr
end;

procedure SetFieldAccess(var Rec : TPsExpression; Fld : string);
begin
  if Rec.Value[1] = '*' then Rec.Value := '(' + Rec.Value + ')';
  Rec.Value := Rec.Value + '.' + Fld
end;

function PsFieldAccess(Rec : TPsExpression) : TPsExpression;
var 
  Fld : TPsIdentifier;
  FldType : TPsTypeIndex;
begin
  if (Rec.Cls <> TecValue) or (Defs.Types[Rec.TypeIndex].Cls <> TtcRecord) then
  begin
    writeln(StdErr, 'Not a record', LxWhereStr());
    halt(1)
  end;
  WantTokenAndRead(TkDot);
  Fld := PsIdentifier();
  FldType := FindFieldType(Rec.TypeIndex, Fld.Name);
  if FldType = 0 then
  begin
    writeln(StdErr, 'Field ', Fld.Name, ' not found in record', LxWhereStr());
    halt(1)
  end;
  SetFieldAccess(Rec, Fld.Name);
  Rec.TypeIndex := FldType;
  PsFieldAccess := Rec
end;

function PsVariableOrFunctionCall : TPsExpression;
var 
  Id : TPsIdentifier;
  VarIndex : TPsVariableIndex;
  FnIndex : TPsFunctionIndex;
  EnumTypeIndex : TPsTypeIndex;
  Expr : TPsExpression;
  Done : boolean;
begin
  Done := false;
  Expr.Value := '';
  Expr.IsConstant := false;
  Id := PsIdentifier();
  VarIndex := FindVariable(Id.Name);
  FnIndex := FindFunction(Id.Name);
  EnumTypeIndex := FindTypeOfEnumValue(Id.Name);
  if VarIndex <> 0 then
  begin
    if Defs.Variables[VarIndex].IsReference then
      Expr.Value := '*' + Id.Name
    else
      Expr.Value := Id.Name;
    Expr.Cls := TecValue;
    Expr.TypeIndex := Defs.Variables[VarIndex].TypeIndex;
  end
  else if FnIndex <> 0 then
  begin
    Expr.Value := Id.Name;
    Expr.Cls := TecFunction;
    Expr.FunctionIndex := FnIndex
  end
  else if EnumTypeIndex <> 0 then
  begin
    Expr.Value := Id.Name;
    Expr.Cls := TecValue;
    Expr.TypeIndex := EnumTypeIndex;
    Expr.IsConstant := true
  end
  else
  begin
    Expr.Cls := TecStatement;
    Expr.TypeIndex := 0;
    if (Id.Name = 'READ') or (Id.Name = 'READLN') then PsRead(Id)
    else if (Id.Name = 'WRITE') or (Id.Name = 'WRITELN') then PsWrite(Id)
    else if Id.Name = 'STR' then PsStr()
    else
    begin
      writeln(StdErr, 'Unknown variable or function: ', Id.Name,
              LxWhereStr());
      halt(1)
    end;
  end;
  repeat
    if LxToken.Id = TkDot then Expr := PsFieldAccess(Expr)
    else if LxToken.Id = TkLbracket then Expr := PsArrayAccess(Expr)
    else if LxToken.Id = TkLparen then Expr := PsFunctionCall(Expr)
    else Done := true
  until Done;
  PsVariableOrFunctionCall := Expr
end;

function PsFactor : TPsExpression;
var 
  Expr : TPsExpression;
begin
  if (LxToken.Id = TkFalse) or (LxToken.Id = TkTrue) then
  begin
    Expr := GenBooleanConstant(LxToken.Id = TkTrue);
    ReadToken()
  end
  else if LxToken.Id = TkString then
         Expr := GenStringConstant(GetTokenValueAndRead(TkString))
  else if LxToken.Id = TkNumber then
         Expr := GenNumberConstant(GetTokenValueAndRead(TkNumber))
  else if LxToken.Id = TkIdentifier then Expr := PsVariableOrFunctionCall()
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
    writeln(StdErr, 'Invalid token in expression: ', LxTokenStr(),
    LxWhereStr());
    halt(1)
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
  write(CpOutput, Expr.Value)
end;

procedure OutAssign(Lhs : TPsExpression; Rhs : TPsExpression);
begin
  writeln(CpOutput, Lhs.Value, ' = ', Rhs.Value, ';')
end;

procedure OutAssignReturnValue(Lhs : TPsExpression; Rhs : TPsExpression);
begin
  writeln(CpOutput, 'return_', Defs.Functions[Lhs.FunctionIndex].Name, ' = ',
          Rhs.Value, ';')
end;

procedure PsAssign(Lhs : TPsExpression; Rhs : TPsExpression);
begin
  if Lhs.IsConstant then
  begin
    writeln(StdErr, 'Cannot assign to a constant value', LxWhereStr());
    halt(1)
  end;
  if Lhs.Cls = TecFunction then
    OutAssignReturnValue(Lhs, CoerceType(Rhs,
                         Defs.Functions[Lhs.FunctionIndex].ReturnTypeIndex))
  else if Lhs.Cls = TecValue then
         OutAssign(Lhs, CoerceType(Rhs, Lhs.TypeIndex))
  else
  begin
    writeln(StdErr, 'Cannot assign to result of statement', LxWhereStr());
    halt(1)
  end
end;

procedure OutIf(Expr : TPsExpression);
begin
  write(CpOutput, 'if (', Expr.Value, ') ')
end;

procedure OutElse;
begin
  write(CpOutput, ' else ')
end;

procedure OutRepeatBegin;
begin
  writeln(CpOutput, 'do {')
end;

procedure OutRepeatEnd(Expr : TPsExpression);
begin
  if not IsBooleanType(Expr.TypeIndex) then
  begin
    writeln(StdErr, 'Expected boolean expression, got ',
            TypeName(Expr.TypeIndex), LxWhereStr());
    halt(1)
  end;
  writeln(CpOutput, '} while (!(', Expr.Value, '));')
end;

procedure OutWhileBegin(Expr : TPsExpression);
begin
  if not IsBooleanType(Expr.TypeIndex) then
  begin
    writeln(StdErr, 'Expected boolean expression, got ',
            TypeName(Expr.TypeIndex), LxWhereStr());
    halt(1)
  end;
  write(CpOutput, 'while (', Expr.Value, ') ')
end;

procedure OutWhileEnd;
begin
end;

procedure OutForBegin(Iter : TPsExpression; FirstExpr : TPsExpression;
                      LastExpr : TPsExpression; Ascending : boolean);
var 
  First : TPsVariable;
  Last : TPsVariable;
begin
  First := MakeVariable('first', Iter.TypeIndex, false);
  Last := MakeVariable('last', Iter.TypeIndex, false);
  writeln(CpOutput, '{');
  OutVariableDeclaration(First);
  writeln(CpOutput, ' = ', FirstExpr.Value, ';');
  OutVariableDeclaration(Last);
  writeln(CpOutput, ' = ', LastExpr.Value, ';');
  write(CpOutput, 'if (first ');
  if Ascending then
    write(CpOutput, '<=')
  else
    write(CpOutput, '=>');
  writeln(CpOutput, ' last) {');
  writeln(CpOutput, Iter.Value, ' = first;');
  writeln(CpOutput, 'while (1) {');
end;

procedure OutForEnd(Iter : TPsExpression; Ascending : boolean);
begin
  writeln(CpOutput, 'if (', Iter.Value, ' == last) break;');
  if Ascending then
    writeln(CpOutput, '++', Iter.Value, ';')
  else
    write(CpOutput, '--', Iter.Value, ';');
  writeln(CpOutput, '}');
  writeln(CpOutput, '}');
  writeln(CpOutput, '}');
end;

procedure OutProcedureCall(Expr : TPsExpression);
begin
  writeln(CpOutput, Expr.Value, ';')
end;

procedure OutEmptyStatement;
begin
  writeln(CpOutput, ';')
end;

procedure PsStatementSequence;
begin
  OutBegin();
  SkipToken(TkBegin);
  while LxToken.Id <> TkEnd do
  begin
    PsStatement();
    WantToken2(TkSemicolon, TkEnd);
    SkipToken(TkSemicolon);
  end;
  OutEnd();
  SkipToken(TkEnd)
end;

procedure PsIdentifierStatement;
var 
  Lhs : TPsExpression;
begin
  Lhs := PsExpression();
  if LxToken.Id = TkAssign then
  begin
    WantTokenAndRead(TkAssign);
    PsAssign(Lhs, PsExpression());
  end
  else if Lhs.Cls <> TecStatement then
         OutProcedureCall(Lhs)
end;

procedure PsIfStatement;
begin
  WantTokenAndRead(TkIf);
  OutIf(PsExpression());
  WantTokenAndRead(TkThen);
  if LxToken.Id = TkElse then
    OutEmptyStatement()
  else
    PsStatement();
  if LxToken.Id = TkElse then
  begin
    WantTokenAndRead(TkElse);
    OutElse();
    PsStatement();
  end
end;

procedure PsRepeatStatement;
begin
  WantTokenAndRead(TkRepeat);
  OutRepeatBegin();
  while LxToken.Id <> TkUntil do
  begin
    PsStatement();
    WantToken2(TkSemicolon, TkUntil);
    SkipToken(TkSemicolon)
  end;
  WantTokenAndRead(TkUntil);
  OutRepeatEnd(PsExpression());
end;

procedure PsWhileStatement;
begin
  WantTokenAndRead(TkWhile);
  OutWhileBegin(PsExpression());
  WantTokenAndRead(TkDo);
  PsStatement();
  OutWhileEnd()
end;

procedure PsForStatement;
var 
  Iter : TPsExpression;
  First : TPsExpression;
  Last  : TPsExpression;
  Ascending : boolean;
begin
  WantTokenAndRead(TkFor);
  Iter := PsExpression();
  if not IsVariableExpression(Iter) then
  begin
    writeln(StdErr, 'Expected variable', LxWhereStr());
    halt(1)
  end;
  WantTokenAndRead(TkAssign);
  First := PsExpression();
  WantToken2(TkTo, TkDownto);
  Ascending := LxToken.Id = TkTo;
  ReadToken();
  Last := PsExpression();
  WantTokenAndRead(TkDo);
  OutForBegin(Iter, First, Last, Ascending);
  PsStatement();
  OutForEnd(Iter, Ascending)
end;

procedure PsStatement;
begin
  if LxToken.Id = TkSemicolon then OutEmptyStatement()
  else if LxToken.Id = TkBegin then PsStatementSequence()
  else if LxToken.Id = TkIdentifier then PsIdentifierStatement()
  else if LxToken.Id = TkIf then PsIfStatement()
  else if LxToken.Id = TkRepeat then PsRepeatStatement()
  else if LxToken.Id = TkWhile then PsWhileStatement()
  else if LxToken.Id = TkFor then PsForStatement()
  else
  begin
    writeln(StdErr, 'Unexpected token ', LxTokenStr(), LxWhereStr());
    halt(1)
  end
end;

procedure OutProgramBegin;
begin
  writeln(CpOutput, 'void pascual_main() {');
end;

procedure OutProgramEnd;
begin
  writeln(CpOutput, '}')
end;

procedure PsProgramBlock;
begin
  PsDefinitions(GlobalScope);
  WantTokenAndRead(TkBegin);
  OutProgramBegin();
  while LxToken.Id <> TkEnd do
  begin
    PsStatement();
    WantToken2(TkSemicolon, TkEnd);
    SkipToken(TkSemicolon)
  end;
  OutProgramEnd();
  WantTokenAndRead(TkEnd)
end;

procedure ParseProgram;
begin
  LxPos.Row := 0;
  LxPos.Col := 0;
  StartGlobalScope();
  ReadToken();
  PsProgramHeading();
  PsProgramBlock();
  WantTokenAndRead(TkDot);
  WantToken(TkEof);
end;

procedure ReadToken;
var 
  ConstIndex : TPsConstantIndex;
  TokenPos : TLxPos;
begin
  repeat
    LxReadToken();
    if LxToken.Id = TkIdentifier then
    begin
      ConstIndex := FindConstant(LxToken.Value);
      if ConstIndex <> 0 then
      begin
        TokenPos := LxToken.Pos;
        LxToken := Defs.Constants[ConstIndex].Replacement;
        LxToken.Pos := TokenPos
      end
    end
  until LxToken.Id <> TkComment
end;

begin
  LxInput := Input;
  CpOutput := Output;
  ParseProgram();
  Close(LxInput);
  Close(CpOutput)
end.

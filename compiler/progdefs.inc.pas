const 
  MaxNames = 1;
  MaxTypes = 128;
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
  TPsEnumIndex = integer;
  TPsRecordIndex = integer;
  TPsArrayIndex = integer;
  TPsTypeIndex = integer;
  TPsTypeClass = (TtcBoolean, TtcInteger, TtcChar, TtcString, TtcText,
                  TtcEnum, TtcRecord, TtcArray, TtcPointer);
  TPsType = record
    Name : string;
    Cls : TPsTypeClass;
    EnumIndex : TPsEnumIndex;
    RecordIndex : TPsRecordIndex;
    ArrayIndex : TPsArrayIndex;
    PointedTypeIndex : TPsTypeIndex;
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
  TPsNameIndex = integer;
  TPsNameClass = (TncType, TncVariable, TncEnumValue, TncFunction,
                  TncSpecialFunction);
  TPsSpecialFunction = (TsfRead, TsfReadln, TsfWrite, TsfWriteln, TsfStr,
                        TsfNew, TsfDispose);
  TPsName = record
    Name : string;
    Cls : TPsNameClass;
    TypeIndex : TPsTypeIndex;
    VariableIndex : TPsVariableIndex;
    FunctionIndex : TPsFunctionIndex;
    SpecialFunction : TPsSpecialFunction;
  end;
  TPsScope = record
    NumNames : integer;
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
    Names : array[1..MaxNames] of TPsName;
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
  else if Typ.Cls = TtcPointer then
         DeepTypeName := '^' + DeepTypeName(Typ.PointedTypeIndex, true)
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
  Ret := EmptyType;
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

function PointerType(TypeIndex : TPsTypeIndex) : TPsType;
var 
  Typ : TPsType;
begin
  Typ := TypeOfClass(TtcPointer);
  Typ.PointedTypeIndex := TypeIndex;
  PointerType := Typ
end;

function IsPointerType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsPointerType := TypeHasClass(TypeIndex, TtcPointer)
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
      writeln(StdErr, 'Variable ', Typ.Name, ' already defined as ',
              TypeName(Pos), LxWhereStr);
      halt(1)
    end
  end;
  Pos := Defs.Scope.NumTypes + 1;
  if Pos > MaxTypes then
  begin
    writeln(StdErr, 'Too many types have been defined', LxWhereStr);
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
    writeln(StdErr, 'Too many enums have been defined', LxWhereStr);
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
    writeln(StdErr, 'Too many records have been defined', LxWhereStr);
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
    writeln(StdErr, 'Too many arrays have been defined', LxWhereStr);
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
              LxWhereStr);
      halt(1)
    end
  end;
  Pos := Defs.Scope.NumConstants + 1;
  if Pos > MaxConstants then
  begin
    writeln(StdErr, 'Too many constants have been defined', LxWhereStr);
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
              TypeName(Defs.Variables[Pos].TypeIndex), LxWhereStr);
      halt(1)
    end
  end;
  Pos := Defs.Scope.NumVariables + 1;
  if Pos > MaxVariables then
  begin
    writeln(StdErr, 'Too many variables have been defined', LxWhereStr);
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
                  ' incompatible with its forward declaration', LxWhereStr)
        else
          writeln(StdErr, 'Function ', Fun.Name,
                  ' incompatible with its forward declaration', LxWhereStr);
        halt(1)
      end
    end
    else
    begin
      if Fun.ReturnTypeIndex = 0 then
        writeln(StdErr, 'Procedure ', Fun.Name, ' already defined', LxWhereStr)
      else
        writeln(StdErr, 'Function ', Fun.Name, ' already defined', LxWhereStr);
      halt(1)
    end
  end
  else
  begin
    Pos := Defs.Scope.NumFunctions + 1;
    if Pos > MaxFunctions then
    begin
      writeln(StdErr, 'Too many functions have been defined', LxWhereStr);
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
    writeln(StdErr, 'Not a record: ', Typ.Name, LxWhereStr);
    halt(1)
  end;
  TypeIndex := 0;
  Rec := Defs.Records[Typ.RecordIndex];
  for Pos := 1 to Rec.Size do
    if Rec.Fields[Pos].Name = Name then
      TypeIndex := Rec.Fields[Pos].TypeIndex;
  if TypeIndex = 0 then
  begin
    writeln(StdErr, 'Field not found: ', Name, LxWhereStr);
    halt(1)
  end;
  FindFieldType := TypeIndex
end;

function MakeType(Name : string; Cls : TPsTypeClass) : TPsType;
var 
  Typ : TPsType;
begin
  Typ := EmptyType;
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

function FindName(Name : string) : TPsNameIndex;
var 
  NameDef : TPsName;
  VarIndex : TPsVariableIndex;
  FnIndex : TPsFunctionIndex;
  TypeIndex : TPsTypeIndex;
  EnumTypeIndex : TPsTypeIndex;
  Pos : integer;
begin
  NameDef.Name := Name;
  VarIndex := FindVariable(Name);
  FnIndex := FindFunction(Name);
  TypeIndex := FindType(Name);
  EnumTypeIndex := FindTypeOfEnumValue(Name);
  if TypeIndex <> 0 then
  begin
    NameDef.Cls := TncType;
    NameDef.TypeIndex := TypeIndex;
  end
  else if VarIndex <> 0 then
  begin
    NameDef.Cls := TncVariable;
    NameDef.VariableIndex := VarIndex
  end
  else if FnIndex <> 0 then
  begin
    NameDef.Cls := TncFunction;
    NameDef.FunctionIndex := FnIndex
  end
  else if EnumTypeIndex <> 0 then
  begin
    NameDef.Cls := TncEnumValue;
    NameDef.TypeIndex := EnumTypeIndex
  end
  else if Name = 'READ' then
  begin
    NameDef.Cls := TncSpecialFunction;
    NameDef.SpecialFunction := TsfRead
  end
  else if Name = 'READLN' then
  begin
    NameDef.Cls := TncSpecialFunction;
    NameDef.SpecialFunction := TsfReadln
  end
  else if Name = 'WRITE' then
  begin
    NameDef.Cls := TncSpecialFunction;
    NameDef.SpecialFunction := TsfWrite
  end
  else if Name = 'WRITELN' then
  begin
    NameDef.Cls := TncSpecialFunction;
    NameDef.SpecialFunction := TsfWriteln
  end
  else if Name = 'STR' then
  begin
    NameDef.Cls := TncSpecialFunction;
    NameDef.SpecialFunction := TsfStr
  end
  else if Name = 'NEW' then
  begin
    NameDef.Cls := TncSpecialFunction;
    NameDef.SpecialFunction := TsfNew
  end
  else if Name = 'DISPOSE' then
  begin
    NameDef.Cls := TncSpecialFunction;
    NameDef.SpecialFunction := TsfDispose
  end
  else
  begin
    writeln(StdErr, 'Unknown identifier: ', Name, LxWhereStr);
    halt(1)
  end;
  Defs.Names[1] := NameDef;
  FindName := 1
end;

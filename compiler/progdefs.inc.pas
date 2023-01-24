const 
  MaxNames = 1024;
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
                  TtcEnum, TtcRecord, TtcArray, TtcPointer, TtcNil,
                  TtcPlaceholder);
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
    PtNil : TPsTypeIndex;
    PtBoolean : TPsTypeIndex;
    PtInteger : TPsTypeIndex;
    PtChar : TPsTypeIndex;
    PtString : TPsTypeIndex;
    PtText : TPsTypeIndex
  end;
  GlobalScope : TPsScope;

procedure ClearDefs;
begin
  Defs.Scope.NumNames := 0;
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

function FindName(Name : string; Required : boolean) : TPsNameIndex;
var 
  Pos : TPsNameIndex;
  Ret : TPsNameIndex;
begin
  Ret := 0;
  for Pos := 1 to Defs.Scope.NumNames do
    if Name = Defs.Names[Pos].Name then Ret := Pos;
  if Required and (Ret = 0) then
  begin
    writeln(StdErr, 'Unknown identifier: ', Name, LxWhereStr);
    halt(1)
  end;
  FindName := Ret
end;

function AddName(Def : TPsName; Scope : TPsScope) : TPsNameIndex;
var 
  Pos : TPsNameIndex;
begin
  Pos := FindName(Def.Name, false);
  if Pos > Scope.NumNames then
  begin
    writeln(StdErr, 'Identifier ', Def.Name, ' already defined', LxWhereStr);
    halt(1)
  end;
  Pos := Defs.Scope.NumNames + 1;
  if Pos > MaxNames then
  begin
    writeln(StdErr, 'Too many identifiers have been defined', LxWhereStr);
    halt(1)
  end;
  Defs.Names[Pos] := Def;
  Defs.Scope.NumNames := Pos;
  AddName := Pos
end;

function MakeName(Name : string; Cls : TPsNameClass; Idx : integer) : TPsName;
var 
  Def : TPsName;
begin
  Def.Name := Name;
  Def.Cls := Cls;
  if Cls = TncType then Def.TypeIndex := Idx
  else if Cls = TncVariable then Def.VariableIndex := Idx
  else if Cls = TncEnumValue then Def.TypeIndex := Idx
  else if Cls = TncFunction then Def.FunctionIndex := Idx
  else
  begin
    writeln(StdErr, 'Cannot use MakeName for special functions', LxWhereStr);
    halt(1)
  end;
  MakeName := Def
end;

function MakeSpecialFunctionName(Name : string;
                                 Fn : TPsSpecialFunction) : TPsName;
var 
  Def : TPsName;
begin
  Def.Name := Name;
  Def.Cls := TncSpecialFunction;
  Def.SpecialFunction := Fn;
  MakeSpecialFunctionName := Def
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
  Ret.ArrayIndex := 0;
  Ret.PointedTypeIndex := 0;
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

function NilType : TPsType;
begin
  NilType := TypeOfClass(TtcNil)
end;

function IsNilType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsNilType := TypeHasClass(TypeIndex, TtcNil)
end;

function IsPointeryType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsPointeryType := IsPointerType(TypeIndex) or IsNilType(TypeIndex)
end;

function PlaceholderType : TPsType;
begin
  PlaceholderType := TypeOfClass(TtcPlaceholder)
end;

function IsPlaceholderType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsPlaceholderType := TypeHasClass(TypeIndex, TtcPlaceholder)
end;

function IsOrdinalType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsOrdinalType := IsBooleanType(TypeIndex)
                   or IsIntegerType(TypeIndex)
                   or IsCharType(TypeIndex)
                   or IsEnumType(TypeIndex)
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
                and (A.ArrayIndex = B.ArrayIndex)
                and (A.PointedTypeIndex = B.PointedTypeIndex)
end;

function ArePointersCompatible(AIndex : TPsTypeIndex; BIndex : TPsTypeIndex)
: boolean;
begin
  ArePointersCompatible := IsPointeryType(AIndex) and IsPointeryType(BIndex) and
                           (IsNilType(AIndex)
                           or IsNilType(BIndex)
                           or IsSameType(AIndex, BIndex))
end;

function AddType(Typ : TPsType; Scope : TPsScope) : TPsTypeIndex;
var 
  Pos : integer;
  EnumPos : integer;
begin
  if Typ.Name = '' then Pos := 0
  else Pos := FindName(Typ.Name, false);
  if Pos <= Scope.NumNames then
  begin
    Pos := Defs.Scope.NumTypes + 1;
    if Pos > MaxTypes then
    begin
      writeln(StdErr, 'Too many types have been defined', LxWhereStr);
      halt(1)
    end;
    Defs.Scope.NumTypes := Pos;
    if Typ.Name <> '' then
      AddName(MakeName(Typ.Name, TncType, Pos), Scope);
  end
  else if (Defs.Names[Pos].Cls = TncType)
          and IsPlaceholderType(Defs.Names[Pos].TypeIndex) then
         Pos := Defs.Names[Pos].TypeIndex
  else
  begin
    writeln(StdErr, 'Identifier ', Typ.Name, ' already defined',
            LxWhereStr);
    halt(1)
  end;
  Defs.Types[Pos] := Typ;
  AddType := Pos;
  if (Typ.Cls = TtcEnum) and (Typ.AliasFor = 0) then
    for EnumPos := 1 to Defs.Enums[Typ.EnumIndex].Size do
      AddName(MakeName(Defs.Enums[Typ.EnumIndex].Values[EnumPos],
              TncEnumValue, Pos), Scope)
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

function AddVariable(VarDef : TPsVariable; Scope : TPsScope)
: TPsVariableIndex;
var 
  Pos : integer;
begin
  if VarDef.Name <> '' then
  begin
    Pos := FindName(VarDef.Name, false);
    if Pos > Scope.NumNames then
    begin
      writeln(StdErr, 'Identifier ', VarDef.Name, ' already defined',
              LxWhereStr);
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
  AddVariable := Pos;
  if VarDef.Name <> '' then
    AddName(MakeName(VarDef.Name, TncVariable, Pos), Scope)
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

function HasForwardDeclaration(Name : string) : boolean;
var 
  Pos : integer;
begin
  Pos := FindName(Name, false);
  HasForwardDeclaration := (Pos <> 0)
                           and (Defs.Names[Pos].Cls = TncFunction)
                           and (Defs.Functions[Defs.Names[Pos].FunctionIndex].
                           IsDeclaration)
end;

function AddFunction(Fun : TPsFunction) : TPsFunctionIndex;
var 
  Pos : integer;
  IsNew : boolean;
begin
  Pos := FindName(Fun.Name, false);
  if Pos <> 0 then
  begin
    if (Defs.Names[Pos].Cls <> TncFunction) or Fun.IsDeclaration then
    begin
      writeln(StdErr, 'Identifier ', Fun.Name, ' already defined', LxWhereStr);
      halt(1)
    end;
    IsNew := false;
    Pos := Defs.Names[Pos].FunctionIndex;
    if Defs.Functions[Pos].IsDeclaration then
    begin
      if (Fun.ArgCount = 0) and (Fun.ReturnTypeIndex = 0) then
      begin
        Fun := Defs.Functions[Pos];
        Fun.IsDeclaration := false
      end
      else if not IsSameFunctionDefinition(Pos, Fun) then
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
    IsNew := true;
    Pos := Defs.Scope.NumFunctions + 1;
    if Pos > MaxFunctions then
    begin
      writeln(StdErr, 'Too many functions have been defined', LxWhereStr);
      halt(1)
    end;
    Defs.Scope.NumFunctions := Pos
  end;
  Defs.Functions[Pos] := Fun;
  AddFunction := Pos;
  if IsNew and (Fun.Name <> '') then
    AddName(MakeName(Fun.Name, TncFunction, Pos), GlobalScope)
end;

procedure AddSpecialFunction(Name : string; Fn : TPsSpecialFunction);
var 
  Def : TPsName;
begin
  Def.Name := Name;
  Def.Cls := TncSpecialFunction;
  Def.SpecialFunction := Fn;
  AddName(Def, GlobalScope)
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

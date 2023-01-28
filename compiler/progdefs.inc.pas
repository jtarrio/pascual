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
  MaxWithVars = 8;

type 
  TPsTypeIndex = integer;
  TPsEnumIndex = integer;
  TPsRecordIndex = integer;
  TPsArrayIndex = integer;
  TPsConstantIndex = integer;
  TPsVariableIndex = integer;
  TPsFunctionIndex = integer;
  TPsExpressionClass = (TecValue, TecFunction, TecStatement);
  TPsExpression = record
    Value : string;
    IsConstant : boolean;
    case Cls : TPsExpressionClass of 
      TecValue : (TypeIndex : TPsTypeIndex);
      TecFunction : (FunctionIndex : TPsFunctionIndex)
  end;
  TPsExpressionFnArgs = record
    Size : integer;
    Args : array[1..MaxFunctionArguments] of TPsExpression
  end;
  TPsTypeClass = (TtcBoolean, TtcInteger, TtcChar, TtcString, TtcText,
                  TtcEnum, TtcRecord, TtcArray, TtcPointer, TtcNil,
                  TtcPointerUnknown);
  TPsType = record
    Name : string;
    AliasFor : TPsTypeIndex;
    case Cls : TPsTypeClass of 
      TtcEnum : (EnumIndex : TPsEnumIndex);
      TtcRecord : (RecordIndex : TPsRecordIndex);
      TtcArray : (ArrayIndex : TPsArrayIndex);
      TtcPointer : (PointedTypeIndex : TPsTypeIndex);
      TtcPointerUnknown : (TargetName : ^string);
  end;
  TPsEnumDef = record
    Size : integer;
    Values : array[1..MaxEnumValues] of string;
    HasBeenDefined : boolean
  end;
  TPsRecordField = record
    Name : string;
    TypeIndex : TPsTypeIndex
  end;
  TPsRecordDef = record
    Size : integer;
    Fields : array[1..MaxRecordFields] of TPsRecordField;
    NumVariants : integer;
    VariantBounds : array[1..MaxRecordFields] of integer;
    HasBeenDefined : boolean
  end;
  TPsArrayDef = record
    LowBound, HighBound : string;
    TypeIndex : TPsTypeIndex
  end;
  TPsConstant = record
    Name : string;
    Replacement : TLxToken
  end;
  TPsVariable = record
    Name : string;
    TypeIndex : TPsTypeIndex;
    IsReference : boolean;
    IsConstant : boolean
  end;
  TPsFunction = record
    Name : string;
    ArgCount : integer;
    Args : array[1..MaxFunctionArguments] of TPsVariable;
    ReturnTypeIndex : TPsTypeIndex;
    IsDeclaration : boolean;
  end;
  TPsWithVarIndex = integer;
  TPsWithVar = record
    VariableIndex : TPsVariableIndex
  end;
  TPsNameIndex = integer;
  TPsNameClass = (TncType, TncVariable, TncEnumValue, TncFunction,
                  TncSpecialFunction);
  TPsSpecialFunction = (TsfRead, TsfReadln, TsfWrite, TsfWriteln, TsfStr,
                        TsfNew, TsfDispose);
  TPsName = record
    Name : string;
    case Cls : TPsNameClass of 
      TncType : (TypeIndex : TPsTypeIndex);
      TncVariable : (VariableIndex : TPsVariableIndex);
      TncEnumValue : (EnumTypeIndex : TPsTypeIndex;
                      Ordinal : integer);
      TncFunction : (FunctionIndex : TPsFunctionIndex);
      TncSpecialFunction : (SpecialFunction : TPsSpecialFunction)
  end;
  TPsDefBounds = record
    Names,
    Types,
    Enums,
    Records,
    Arrays,
    Constants,
    Variables,
    Functions,
    WithVars : integer
  end;
  TPsScopeStack = ^TPsScopeElement;
  TPsScopeElement = record
    Base : TPsDefBounds;
    Prev : TPsScopeStack;
    IsTemporary : boolean
  end;
  TPsDefs = record
    Bounds : TPsDefBounds;
    ScopeStack : TPsScopeStack;
    ScopeBase : TPsDefBounds;
    Names : array[1..MaxNames] of TPsName;
    Types : array[1..MaxTypes] of TPsType;
    Enums : array[1..MaxEnums] of TPsEnumDef;
    Records : array[1..MaxRecords] of TPsRecordDef;
    Arrays : array[1..MaxArrays] of TPsArrayDef;
    Constants : array[1..MaxConstants] of TPsConstant;
    Variables : array[1..MaxVariables] of TPsVariable;
    Functions : array[1..MaxFunctions] of TPsFunction;
    WithVars : array[1..MaxWithVars] of TPsWithVar;
  end;
  TPsIdentifier = record
    Name : string;
  end;
var 
  Defs : TPsDefs;
  PrimitiveTypes : record
    PtNil, PtBoolean, PtInteger, PtChar, PtString, PtText : TPsTypeIndex
  end;

function ClearBounds : TPsDefBounds;
var 
  Ret : TPsDefBounds;
begin
  Ret.Names := 0;
  Ret.Types := 0;
  Ret.Enums := 0;
  Ret.Records := 0;
  Ret.Arrays := 0;
  Ret.Constants := 0;
  Ret.Variables := 0;
  Ret.Functions := 0;
  Ret.WithVars := 0;
  ClearBounds := Ret
end;

procedure InitDefs;
begin
  Defs.Bounds := ClearBounds;
  Defs.ScopeStack := nil;
  Defs.ScopeBase := ClearBounds;
end;

procedure _StartScope(Temporary : boolean);
var 
  Prev : TPsScopeStack;
begin
  Prev := Defs.ScopeStack;
  new(Defs.ScopeStack);
  Defs.ScopeStack^.Prev := Prev;
  Defs.ScopeStack^.Base := Defs.ScopeBase;
  Defs.ScopeStack^.IsTemporary := Temporary;
  Defs.ScopeBase := Defs.Bounds
end;

procedure _CloseScope(Temporary : boolean);
var 
  Prev : TPsScopeStack;
  WasTemporary : boolean;
begin
  repeat
    if Defs.ScopeStack = nil then
      CompileError('Internal error: Already in global scope');
    Defs.Bounds := Defs.ScopeBase;
    Defs.ScopeBase := Defs.ScopeStack^.Base;
    WasTemporary := Defs.ScopeStack^.IsTemporary;
    Prev := Defs.ScopeStack^.Prev;
    dispose(Defs.ScopeStack);
    Defs.ScopeStack := Prev;
  until Temporary or not WasTemporary
end;

procedure StartLocalScope;
begin
  _StartScope({Temporary=}false)
end;

procedure CloseLocalScope;
begin
  _CloseScope({Temporary=}false)
end;

procedure StartTemporaryScope;
begin
  _StartScope({Temporary=}true)
end;

procedure CloseTemporaryScope;
begin
  _CloseScope({Temporary=}true)
end;

function _FindNameFromBase(Name : string; Required : boolean;
                           Base : TPsNameIndex) : TPsNameIndex;
var 
  Pos, Ret : TPsNameIndex;
begin
  Ret := 0;
  Pos := Defs.Bounds.Names;
  while (Pos > Base) and (Ret = 0) do
  begin
    if Name = Defs.Names[Pos].Name then Ret := Pos;
    Pos := Pos - 1
  end;
  if Required and (Ret = 0) then CompileError('Unknown identifier: ' + Name);
  _FindNameFromBase := Ret
end;

function _CheckNameClass(NameIndex : TPsNameIndex; Cls : TPsNameClass)
: TPSNameIndex;
var 
  Name : string;
begin
  if (NameIndex <> 0) and (Defs.Names[NameIndex].Cls <> Cls) then
    with Defs.Names[NameIndex] do
      case Cls of 
        TncType : CompileError('Not a type: ' + Name);
        TncVariable : CompileError('Not a variable: ' + Name);
        TncEnumValue : CompileError('Not an enumeration value: ' + Name);
        TncFunction : CompileError('Not a procedure or function: ' + Name);
        TncSpecialFunction : CompileError('Not a procedure or function: ' + Name
                             );
        else CompileError('Internal error: name class mismatch')
      end;
  _CheckNameClass := NameIndex
end;

function FindNameInLocalScope(Name : string; Required : boolean) : TPsNameIndex;
begin
  FindNameInLocalScope := _FindNameFromBase(Name, Required,
                          Defs.ScopeBase.Names)
end;

function FindNameOfClassInLocalScope(Name : string; Cls : TPsNameClass;
                                     Required : boolean) : TPsNameIndex;
begin
  FindNameOfClassInLocalScope := _CheckNameClass(
                                 FindNameInLocalScope(Name, Required), Cls)
end;

function FindName(Name : string; Required : boolean) : TPsNameIndex;
begin
  FindName := _FindNameFromBase(Name, Required, 0)
end;

function FindNameOfClass(Name : string; Cls : TPsNameClass; Required : boolean)
: TPsNameIndex;
begin
  FindNameOfClass := _CheckNameClass(FindName(Name, Required), Cls)
end;

function AddName(Def : TPsName) : TPsNameIndex;
var 
  Pos : TPsNameIndex;
begin
  Pos := FindNameInLocalScope(Def.Name, {Required=}false);
  if Pos <> 0 then CompileError('Identifier ' + Def.Name + ' already defined');
  Pos := Defs.Bounds.Names + 1;
  if Pos > MaxNames then CompileError('Too many identifiers have been defined');
  Defs.Names[Pos] := Def;
  Defs.Bounds.Names := Pos;
  AddName := Pos
end;

function MakeName(Name : string; Cls : TPsNameClass; Idx : integer) : TPsName;
var 
  Def : TPsName;
begin
  Def.Name := Name;
  Def.Cls := Cls;
  case Cls of 
    TncType : Def.TypeIndex := Idx;
    TncVariable : Def.VariableIndex := Idx;
    TncFunction : Def.FunctionIndex := Idx;
    else
      CompileError('Cannot use MakeName for special functions or enums')
  end;
  MakeName := Def
end;

function MakeOrdinalName(Name : string; Ordinal : integer; Cls : TPsNameClass;
                         Idx : integer) : TPsName;
var 
  Def : TPsName;
begin
  Def.Name := Name;
  Def.Cls := Cls;
  if Cls = TncEnumValue then
  begin
    Def.EnumTypeIndex := Idx;
    Def.Ordinal := Ordinal
  end
  else CompileError('Can only use MakeOrdinalName for enums');
  MakeOrdinalName := Def
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
    Str(Typ.Cls, Ret);
    CompileError('Could not get name for type of class ' + Ret)
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
  Ret.AliasFor := 0;
  EmptyType := Ret
end;

function CopyType(var Typ : TPsType) : TPsType;
var 
  NewTyp : TPsType;
begin
  NewTyp := Typ;
  if NewTyp.Cls = TtcPointerUnknown then
  begin
    new(NewTyp.TargetName);
    NewTyp.TargetName^ := Typ.TargetName^
  end;
  CopyType := NewTyp
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

function PointerUnknownType(TargetName : string) : TPsType;
var 
  Typ : TPsType;
begin
  Typ := TypeOfClass(TtcPointerUnknown);
  new(Typ.TargetName);
  Typ.TargetName^ := TargetName;
  PointerUnknownType := Typ
end;

function IsPointerUnknownType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsPointerUnknownType := TypeHasClass(TypeIndex, TtcPointerUnknown)
end;

function IsOrdinalType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsOrdinalType := IsBooleanType(TypeIndex)
                   or IsIntegerType(TypeIndex)
                   or IsCharType(TypeIndex)
                   or IsEnumType(TypeIndex)
end;

function IsSimpleType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsSimpleType := IsOrdinalType(TypeIndex) or IsStringType(TypeIndex)
end;

function IsSameType(AIndex, BIndex : TPsTypeIndex) : boolean;
var 
  A, B : TPsType;
begin
  A := Defs.Types[AIndex];
  B := Defs.Types[BIndex];
  IsSameType := (A.Cls = B.Cls)
                and (A.EnumIndex = B.EnumIndex)
                and (A.RecordIndex = B.RecordIndex)
                and (A.ArrayIndex = B.ArrayIndex)
                and (A.PointedTypeIndex = B.PointedTypeIndex)
end;

function ArePointersCompatible(AIndex, BIndex : TPsTypeIndex) : boolean;
begin
  ArePointersCompatible := IsPointeryType(AIndex) and IsPointeryType(BIndex) and
                           (IsNilType(AIndex)
                           or IsNilType(BIndex)
                           or IsSameType(AIndex, BIndex))
end;

function AddType(Typ : TPsType) : TPsTypeIndex;
var 
  Pos, EnumPos : integer;
begin
  if Typ.Name = '' then Pos := 0
  else Pos := FindNameInLocalScope(Typ.Name, {Required=}false);

  if Pos <> 0 then
    CompileError('Identifier ' + Typ.Name + ' already defined')
  else
  begin
    Pos := Defs.Bounds.Types + 1;
    if Pos > MaxTypes then CompileError('Too many types have been defined');
    Defs.Bounds.Types := Pos;
    if Typ.Name <> '' then AddName(MakeName(Typ.Name, TncType, Pos));
  end;

  Defs.Types[Pos] := Typ;
  AddType := Pos;
  if (Typ.Cls = TtcEnum) and (Typ.AliasFor = 0) then
    with Defs.Enums[Typ.EnumIndex] do
      for EnumPos := 1 to Size do
        AddName(MakeOrdinalName(Values[EnumPos], EnumPos, TncEnumValue, Pos))
end;

function AddEnum(Enum : TPsEnumDef) : TPsEnumIndex;
begin
  Defs.Bounds.Enums := Defs.Bounds.Enums + 1;
  if Defs.Bounds.Enums > MaxEnums then
    CompileError('Too many enums have been defined');
  Defs.Enums[Defs.Bounds.Enums] := Enum;
  AddEnum := Defs.Bounds.Enums
end;

function AddRecord(Rec : TPsRecordDef) : TPsRecordIndex;
begin
  Defs.Bounds.Records := Defs.Bounds.Records + 1;
  if Defs.Bounds.Records > MaxRecords then
    CompileError('Too many records have been defined');
  Defs.Records[Defs.Bounds.Records] := Rec;
  AddRecord := Defs.Bounds.Records
end;

function AddArray(Arr : TPsArrayDef) : TPsArrayIndex;
begin
  Defs.Bounds.Arrays := Defs.Bounds.Arrays + 1;
  if Defs.Bounds.Arrays > MaxArrays then
    CompileError('Too many arrays have been defined');
  Defs.Arrays[Defs.Bounds.Arrays] := Arr;
  AddArray := Defs.Bounds.Arrays
end;

function FindConstant(Name : string) : TPsConstantIndex;
var 
  Pos : integer;
  Ret : TPsConstantIndex;
begin
  Ret := 0;
  Pos := Defs.Bounds.Constants;
  while (Pos >= 1) and (Ret = 0) do
  begin
    if Name = Defs.Constants[Pos].Name then Ret := Pos;
    Pos := Pos - 1
  end;
  FindConstant := Ret
end;

function AddConstant(Constant : TPsConstant) : TPsConstantIndex;
var 
  Pos : integer;
begin
  if Constant.Name = '' then CompileError('Constant has no name');
  Pos := FindConstant(Constant.Name);
  if Pos > 0 then
    CompileError('Constant ' + Constant.Name + ' already defined');
  Pos := Defs.Bounds.Constants + 1;
  if Pos > MaxConstants then
    CompileError('Too many constants have been defined');
  Defs.Constants[Pos] := Constant;
  Defs.Bounds.Constants := Pos;
  AddConstant := Pos
end;

function AddVariable(VarDef : TPsVariable)
: TPsVariableIndex;
var 
  Pos : integer;
begin
  if VarDef.Name = '' then CompileError('Variable has no name');
  Pos := FindNameInLocalScope(VarDef.Name, {Required=}false);
  if Pos <> 0 then
    CompileError('Identifier ' + VarDef.Name + ' already defined');
  Pos := Defs.Bounds.Variables + 1;
  if Pos > MaxVariables then
    CompileError('Too many variables have been defined');
  Defs.Variables[Pos] := VarDef;
  Defs.Bounds.Variables := Pos;
  AddVariable := Pos;
  AddName(MakeName(VarDef.Name, TncVariable, Pos))
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
  Pos := FindNameOfClassInLocalScope(Name, TncFunction, {Required=}false);
  HasForwardDeclaration := (Pos <> 0)
                           and (Defs.Functions[Defs.Names[Pos].FunctionIndex].
                           IsDeclaration)
end;

function AddFunction(Fun : TPsFunction) : TPsFunctionIndex;
var 
  Pos : integer;
  IsNew : boolean;
begin
  Pos := FindNameInLocalScope(Fun.Name, {Required=}false);
  if Pos = 0 then
  begin
    IsNew := true;
    Pos := Defs.Bounds.Functions + 1;
    if Pos > MaxFunctions then
      CompileError('Too many functions have been defined');
    Defs.Bounds.Functions := Pos
  end
  else
  begin
    if (Defs.Names[Pos].Cls <> TncFunction) or Fun.IsDeclaration then
      CompileError('Identifier ' + Fun.Name + ' already defined');
    IsNew := false;
    Pos := Defs.Names[Pos].FunctionIndex;
    if not Defs.Functions[Pos].IsDeclaration then
    begin
      if Fun.ReturnTypeIndex = 0 then
        CompileError('Procedure ' + Fun.Name + ' already defined')
      else
        CompileError('Function ' + Fun.Name + ' already defined')
    end
    else
    begin
      if (Fun.ArgCount = 0) and (Fun.ReturnTypeIndex = 0) then
      begin
        Fun := Defs.Functions[Pos];
        Fun.IsDeclaration := false
      end
      else if not IsSameFunctionDefinition(Pos, Fun) then
      begin
        if Fun.ReturnTypeIndex = 0 then
          CompileError('Procedure ' + Fun.Name +
                       ' incompatible with its forward declaration')
        else
          CompileError('Function ' + Fun.Name +
                       ' incompatible with its forward declaration')
      end
    end
  end;
  Defs.Functions[Pos] := Fun;
  AddFunction := Pos;
  if IsNew and (Fun.Name <> '') then
    AddName(MakeName(Fun.Name, TncFunction, Pos))
end;

procedure AddSpecialFunction(Name : string; Fn : TPsSpecialFunction);
var 
  Def : TPsName;
begin
  Def.Name := Name;
  Def.Cls := TncSpecialFunction;
  Def.SpecialFunction := Fn;
  AddName(Def)
end;

function FindFieldType(TypeIndex : TPsTypeIndex; Name : string;
                       Required : boolean) : TPsTypeIndex;
var 
  Pos : integer;
  Ret : integer;
begin
  if Defs.Types[TypeIndex].Cls <> TtcRecord then
    CompileError('Not a record: ' + Defs.Types[TypeIndex].Name);
  with Defs.Records[Defs.Types[TypeIndex].RecordIndex] do
  begin
    Ret := 0;
    Pos := Size;
    while (Pos >= 1) and (Ret = 0) do
    begin
      if Name = Fields[Pos].Name then Ret := Fields[Pos].TypeIndex;
      Pos := Pos - 1
    end;
  end;
  if Required and (Ret = 0) then CompileError('Field not found: ' + Name);
  FindFieldType := Ret
end;

function FindWithVar(Name : string) : TPsWithVarIndex;
var 
  Ret : TPsWithVarIndex;
  Pos : TPsWithVarIndex;
  TypeIndex : TPsTypeIndex;
begin
  Ret := 0;
  Pos := Defs.Bounds.WithVars;
  while (Pos >= 1) and (Ret = 0) do
  begin
    TypeIndex := Defs.Variables[Defs.WithVars[Pos].VariableIndex].TypeIndex;
    if FindFieldType(TypeIndex, Name, false) <> 0 then
      Ret := Pos;
    Pos := Pos - 1
  end;
  FindWithVar := Ret
end;

function AddWithVar(Base : TPsExpression) : TPsVariableIndex;
var 
  TmpVarNum : string;
  TmpVar : TPsVariable;
  TmpVarIndex : TPsVariableIndex;
begin
  if (Base.Cls <> TecValue) or not IsRecordType(Base.TypeIndex) then
    CompileError('''With'' variable is not a record');
  Defs.Bounds.WithVars := Defs.Bounds.WithVars + 1;
  if Defs.Bounds.WithVars > MaxWithVars then
    CompileError('Too many nesting levels for ''with''');

  Str(Defs.Bounds.WithVars, TmpVarNum);
  TmpVar.Name := '_with_' + TmpVarNum;
  TmpVar.TypeIndex := Base.TypeIndex;
  TmpVar.IsConstant := Base.IsConstant;
  TmpVar.IsReference := true;
  TmpVarIndex := AddVariable(TmpVar);
  Defs.WithVars[Defs.Bounds.WithVars].VariableIndex := TmpVarIndex;
  AddWithVar := TmpVarIndex
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

const 
  MaxEnumValues = 128;
  MaxRecordFields = 16;
  MaxFunctionArguments = 4;

type 
  TPsTypeIndex = ^TPsType;
  TPsEnumIndex = ^TPsEnumDef;
  TPsRecordIndex = ^TPsRecordDef;
  TPsArrayIndex = ^TPsArrayDef;
  TPsConstantIndex = ^TPsConstant;
  TPsVariableIndex = ^TPsVariable;
  TPsFunctionIndex = ^TPsFunction;
  TPsWithVarIndex = ^TPsWithVar;
  TPsNameIndex = ^TPsName;

  TExpression = ^TExpressionObj;
  TExImmediateClass = (XicNil, XicBoolean, XicInteger, XicChar, XicString,
                       XicEnum);
  TExImmediate = record
    case Cls : TExImmediateClass of 
      XicBoolean : (BooleanValue : boolean);
      XicInteger : (IntegerValue : integer);
      XicChar : (CharValue : char);
      XicString : (StringValue : string);
      XicEnum : (EnumOrdinal : integer)
  end;
  TExToString = record
    Parent : TExpression
  end;
  TExVariableAccess = record
    VariableIndex : TPsVariableIndex
  end;
  TExFieldAccess = record
    Parent : TExpression;
    FieldNumber : integer
  end;
  TExArrayAccess = record
    Parent : TExpression;
    Subscript : TExpression
  end;
  TExPointerAccess = record
    Parent : TExpression
  end;
  TExStringChar = record
    Parent : TExpression;
    Subscript : TExpression
  end;
  TExFunctionRef = record
    FunctionIndex : TPsFunctionIndex
  end;
  TExFunctionArgs = record
    Size : integer;
    Values : array[1..MaxFunctionArguments] of TExpression
  end;
  TExFunctionCall = record
    FunctionRef : TExpression;
    Args : TExFunctionArgs
  end;
  TExUnaryOp = record
    Parent : TExpression;
    Op : TLxTokenId
  end;
  TExBinaryOp = record
    Left : TExpression;
    Right : TExpression;
    Op : TLxTokenId
  end;

  TExpressionClass = (XcNothing, XcImmediate, XcToString,
                      XcVariableAccess, XcFieldAccess,
                      XcArrayAccess, XcPointerAccess, XcStringChar,
                      XcFunctionRef, XcFunctionCall, XcUnaryOp,
                      XcBinaryOp);
  TExpressionObj = record
    TypeIndex : TPsTypeIndex;
    IsConstant : boolean;
    IsAssignable : boolean;
    case Cls : TExpressionClass of 
      XcImmediate : (ImmediateEx : TExImmediate);
      XcToString : (ToStringEx : TExToString);
      XcVariableAccess : (VariableEx : TExVariableAccess);
      XcFieldAccess : (FieldEx : TExFieldAccess);
      XcArrayAccess : (ArrayEx : TExArrayAccess);
      XcPointerAccess : (PointerEx : TExPointerAccess);
      XcStringChar : (StringCharEx : TExStringChar);
      XcFunctionRef : (FunctionEx : TExFunctionRef);
      XcFunctionCall : (CallEx : TExFunctionCall);
      XcUnaryOp : (UnaryEx : TExUnaryOp);
      XcBinaryOp : (BinaryEx : TExBinaryOp);
  end;

  TPsIdentifier = record
    Name : string;
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
    Id : integer;
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
    Id : integer;
    HasBeenDefined : boolean
  end;
  TPsArrayDef = record
    LowBound, HighBound : TExpression;
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
  TPsWithVar = record
    VariableIndex : TPsVariableIndex
  end;
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

  TPsDefPtr = ^TPsDefEntry;
  TPsDefClass = (TdcName, TdcType, TdcEnum, TdcRecord, TdcArray, TdcConstant,
                 TdcVariable, TdcFunction, TdcWithVar, TdcScopeBoundary);
  TPsDefEntry = record
    Prev : TPsDefPtr;
    Next : TPsDefPtr;
    case Cls : TPsDefClass of 
      TdcName : (NameIndex : TPsNameIndex);
      TdcType : (TypeIndex : TPsTypeIndex);
      TdcEnum : (EnumIndex : TPsEnumIndex);
      TdcRecord : (RecordIndex : TPsRecordIndex);
      TdcArray : (ArrayIndex : TPsArrayIndex);
      TdcConstant : (ConstantIndex : TPsConstantIndex);
      TdcVariable : (VariableIndex : TPsVariableIndex);
      TdcFunction : (FunctionIndex : TPsFunctionIndex);
      TdcWithVar : (WithVarIndex : TPsWithVarIndex);
      TdcScopeBoundary : (TemporaryScope : boolean)
  end;

  TPsDefs = record
    Latest : TPsDefPtr;
    Counter : integer;
  end;
var 
  Defs : TPsDefs;
  PrimitiveTypes : record
    PtNil, PtBoolean, PtInteger, PtChar, PtString, PtText : TPsTypeIndex
  end;

function DefCounter : integer;
begin
  Defs.Counter := Defs.Counter + 1;
  DefCounter := Defs.Counter
end;

procedure InitDefs;
begin
  Defs.Latest := nil;
  Defs.Counter := 0
end;

function _NewDef(Cls : TPsDefClass) : TPsDefPtr;
var 
  Def : TPsDefPtr;
begin
  new(Def);
  Def^.Prev := nil;
  Def^.Next := nil;
  Def^.Cls := Cls;
  case Cls of 
    TdcName : new(Def^.NameIndex);
    TdcType : new(Def^.TypeIndex);
    TdcEnum : new(Def^.EnumIndex);
    TdcRecord : new(Def^.RecordIndex);
    TdcArray : new(Def^.ArrayIndex);
    TdcConstant : new(Def^.ConstantIndex);
    TdcVariable : new(Def^.VariableIndex);
    TdcFunction : new(Def^.FunctionIndex);
    TdcWithVar : new(Def^.WithVarIndex);
    TdcScopeBoundary : Def^.TemporaryScope := false;
  end;
  _NewDef := Def
end;

procedure _DisposeDef(Def : TPsDefPtr);
begin
  case Def^.Cls of 
    TdcName : dispose(Def^.NameIndex);
    TdcType : dispose(Def^.TypeIndex);
    TdcEnum : dispose(Def^.EnumIndex);
    TdcRecord : dispose(Def^.RecordIndex);
    TdcArray : dispose(Def^.ArrayIndex);
    TdcConstant : dispose(Def^.ConstantIndex);
    TdcVariable : dispose(Def^.VariableIndex);
    TdcFunction : dispose(Def^.FunctionIndex);
    TdcWithVar : dispose(Def^.WithVarIndex)
  end;
  dispose(Def)
end;

function _AddDef(Cls : TPsDefClass) : TPsDefPtr;
var 
  Def : TPsDefPtr;
begin
  Def := _NewDef(Cls);
  Def^.Prev := Defs.Latest;
  if Defs.Latest <> nil then Defs.Latest^.Next := Def;
  Defs.Latest := Def;
  _AddDef := Def;
end;

function _DeleteDef(var DeletedDef : TPsDefEntry) : boolean;
begin
  if Defs.Latest = nil then _DeleteDef := false
  else
  begin
    DeletedDef := Defs.Latest^;
    _DisposeDef(Defs.Latest);
    Defs.Latest := DeletedDef.Prev;
    _DeleteDef := true
  end
end;

procedure _StartScope(Temporary : boolean);
var 
  Def : TPsDefPtr;
begin
  Def := _AddDef(TdcScopeBoundary);
  Def^.TemporaryScope := Temporary
end;

procedure _CloseScope(Temporary : boolean);
var 
  DeletedDef : TPsDefEntry;
  Deleted : boolean;
begin
  repeat
    Deleted := _DeleteDef(DeletedDef)
  until not Deleted
        or ((DeletedDef.Cls = TdcScopeBoundary)
        and (Temporary or not DeletedDef.TemporaryScope));
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

function _FindName(Name : string; Required : boolean;
                   FromLocalScope : boolean) : TPsNameIndex;
var 
  Def : TPsDefPtr;
  Ret : TPsNameIndex;
begin
  Ret := nil;
  Def := Defs.Latest;
  while (Ret = nil)
        and (Def <> nil)
        and (not FromLocalScope or (Def^.Cls <> TdcScopeBoundary)) do
  begin
    if (Def^.Cls = TdcName) and (Name = Def^.NameIndex^.Name) then
      Ret := Def^.NameIndex;
    Def := Def^.Prev
  end;
  if Required and (Ret = nil) then CompileError('Unknown identifier: ' + Name);
  _FindName := Ret
end;

function _CheckNameClass(NameIndex : TPsNameIndex; Cls : TPsNameClass)
: TPSNameIndex;
begin
  if (NameIndex <> nil) and (NameIndex^.Cls <> Cls) then
    case NameIndex^.Cls of 
      TncType : CompileError('Not a type: ' + NameIndex^.Name);
      TncVariable : CompileError('Not a variable: ' + NameIndex^.Name);
      TncEnumValue : CompileError('Not an enumeration value: ' +
                                  NameIndex^.Name);
      TncFunction : CompileError('Not a procedure or function: ' +
                                 NameIndex^.Name);
      TncSpecialFunction : CompileError('Not a procedure or function: ' +
                                        NameIndex^.Name);
      else CompileError('Internal error: name class mismatch')
    end;
  _CheckNameClass := NameIndex
end;

function FindNameInLocalScope(Name : string; Required : boolean) : TPsNameIndex;
begin
  FindNameInLocalScope := _FindName(Name, Required, {FromLocalScope=}true)
end;

function FindNameOfClassInLocalScope(Name : string; Cls : TPsNameClass;
                                     Required : boolean) : TPsNameIndex;
begin
  FindNameOfClassInLocalScope := _CheckNameClass(
                                 FindNameInLocalScope(Name, Required), Cls)
end;

function FindName(Name : string; Required : boolean) : TPsNameIndex;
begin
  FindName := _FindName(Name, Required, {FromLocalScope=}false)
end;

function FindNameOfClass(Name : string; Cls : TPsNameClass; Required : boolean)
: TPsNameIndex;
begin
  FindNameOfClass := _CheckNameClass(FindName(Name, Required), Cls)
end;

function _AddName(Name : string; Cls : TPsNameClass) : TPsNameIndex;
var 
  Pos : TPsNameIndex;
begin
  if FindNameInLocalScope(Name, {Required=}false) <> nil then
    CompileError('Identifier ' + Name + ' already defined');
  Pos := _AddDef(TdcName)^.NameIndex;
  Pos^.Name := Name;
  Pos^.Cls := Cls;
  _AddName := Pos
end;

function AddTypeName(Name : string; Idx : TPsTypeIndex) : TPsNameIndex;
var 
  Def : TPsNameIndex;
begin
  Def := _AddName(Name, TncType);
  Def^.TypeIndex := Idx;
  AddTypeName := Def
end;

function AddVariableName(Name : string; Idx : TPsVariableIndex) : TPsNameIndex;
var 
  Def : TPsNameIndex;
begin
  Def := _AddName(Name, TncVariable);
  Def^.VariableIndex := Idx;
  AddVariableName := Def
end;

function AddFunctionName(Name : string; Idx : TPsFunctionIndex) : TPsNameIndex;
var 
  Def : TPsNameIndex;
begin
  Def := _AddName(Name, TncFunction);
  Def^.FunctionIndex := Idx;
  AddFunctionName := Def
end;

function AddEnumValueName(Ordinal : integer;
                          TypeIdx : TPsTypeIndex) : TPsNameIndex;
var Def : TPsNameIndex;
begin
  Def := _AddName(TypeIdx^.EnumIndex^.Values[Ordinal], TncEnumValue);
  Def^.EnumTypeIndex := TypeIdx;
  Def^.Ordinal := Ordinal;
  AddEnumValueName := Def
end;

function AddSpecialFunctionName(Name : string; Fn : TPsSpecialFunction)
: TPsNameIndex;
var Def : TPsNameIndex;
begin
  Def := _AddName(Name, TncSpecialFunction);
  Def^.SpecialFunction := Fn;
  AddSpecialFunctionName := Def
end;

function DeepTypeName(TypeIndex : TPsTypeIndex; UseOriginal : boolean) : string;
var 
  Typ : TPsType;
  Ret : string;
  Pos : integer;
begin
  repeat
    Typ := TypeIndex^;
    TypeIndex := Typ.AliasFor
  until not UseOriginal or (TypeIndex = nil);
  if Typ.Name <> '' then DeepTypeName := Typ.Name
  else if Typ.Cls = TtcEnum then
  begin
    Ret := '(';
    for Pos := 1 to Typ.EnumIndex^.Size do
    begin
      if Pos <> 1 then
        Ret := Ret + ',';
      Ret := Ret + Typ.EnumIndex^.Values[Pos]
    end;
    DeepTypeName := Ret + ')'
  end
  else if Typ.Cls = TtcRecord then
  begin
    Ret := 'record ';
    for Pos := 1 to Typ.RecordIndex^.Size do
    begin
      if Pos <> 1 then Ret := Ret + ',';
      Ret := Ret + DeepTypeName(Typ.RecordIndex^.Fields[Pos].TypeIndex, true);
      Ret := Ret + ':' + Typ.RecordIndex^.Fields[Pos].Name
    end;
    DeepTypeName := Ret + ' end'
  end
  else if Typ.Cls = TtcArray then
  begin
    Ret := 'array [...] of ' + DeepTypeName(Typ.ArrayIndex^.TypeIndex, true);
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
  if TypeIndex = nil then TypeName := '(none)'
  else TypeName := DeepTypeName(TypeIndex, false)
end;

function EmptyType : TPsType;
var 
  Ret : TPsType;
begin
  Ret.Name := '';
  Ret.Cls := TtcBoolean;
  Ret.AliasFor := nil;
  EmptyType := Ret
end;

function CopyType(TypeIndex : TPsTypeIndex) : TPsType;
var 
  NewTyp : TPsType;
begin
  NewTyp := TypeIndex^;
  if NewTyp.Cls = TtcPointerUnknown then
  begin
    new(NewTyp.TargetName);
    NewTyp.TargetName^ := TypeIndex^.TargetName^
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

function IntegerType : TPsType;
begin
  IntegerType := TypeOfClass(TtcInteger)
end;

function IsIntegerType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsIntegerType := TypeIndex^.Cls = TtcInteger
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
  IsStringType := TypeIndex^.Cls = TtcString
end;

function IsCharType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsCharType := TypeIndex^.Cls = TtcChar
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
  IsBooleanType := TypeIndex^.Cls = TtcBoolean
end;

function TextType : TPsType;
begin
  TextType := TypeOfClass(TtcText)
end;

function IsTextType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsTextType := TypeIndex^.Cls = TtcText
end;

function IsEnumType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsEnumType := TypeIndex^.Cls = TtcEnum
end;

function IsRecordType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsRecordType := TypeIndex^.Cls = TtcRecord
end;

function IsArrayType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsArrayType := TypeIndex^.Cls = TtcArray
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
  IsPointerType := TypeIndex^.Cls = TtcPointer
end;

function NilType : TPsType;
begin
  NilType := TypeOfClass(TtcNil)
end;

function IsNilType(TypeIndex : TPsTypeIndex) : boolean;
begin
  IsNilType := TypeIndex^.Cls = TtcNil
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
  IsPointerUnknownType := TypeIndex^.Cls = TtcPointerUnknown
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

function IsSameType(A, B : TPsTypeIndex) : boolean;
begin
  if (A = nil) or (B = nil) then IsSameType := A = B
  else
  begin
    while A^.AliasFor <> nil do
      A := A^.AliasFor;
    while B^.AliasFor <> nil do
      B := B^.AliasFor;
    IsSameType := (A = B)
                  or (IsPointerType(A) and IsPointerType(B)
                  and IsSameType(A^.PointedTypeIndex, B^.PointedTypeIndex))
  end
end;

function ArePointersCompatible(A, B : TPsTypeIndex) : boolean;
begin
  ArePointersCompatible := IsPointeryType(A) and IsPointeryType(B) and
                           (IsNilType(A) or IsNilType(B)
                           or IsSameType(A, B))
end;

function AddType(Typ : TPsType) : TPsTypeIndex;
var 
  TypeIndex : TPsTypeIndex;
  EnumPos : integer;
begin
  TypeIndex := _AddDef(TdcType)^.TypeIndex;
  TypeIndex^ := Typ;
  AddType := TypeIndex;

  if Typ.Name <> '' then
  begin
    if FindNameInLocalScope(Typ.Name, {Required=}false) <> nil then
      CompileError('Identifier ' + Typ.Name + ' already defined');
    AddTypeName(Typ.Name, TypeIndex)
  end;

  if (Typ.Cls = TtcEnum) and (Typ.AliasFor = nil) then
    for EnumPos := 1 to Typ.EnumIndex^.Size do
      AddEnumValueName(EnumPos, TypeIndex)
end;

function AddEnum(Enum : TPsEnumDef) : TPsEnumIndex;
var 
  EnumIndex : TPsEnumIndex;
begin
  EnumIndex := _AddDef(TdcEnum)^.EnumIndex;
  EnumIndex^ := Enum;
  EnumIndex^.Id := DefCounter;
  AddEnum := EnumIndex
end;

function AddRecord(Rec : TPsRecordDef) : TPsRecordIndex;
var 
  RecordIndex : TPsRecordIndex;
begin
  RecordIndex := _AddDef(TdcRecord)^.RecordIndex;
  RecordIndex^ := Rec;
  RecordIndex^.Id := DefCounter;
  AddRecord := RecordIndex
end;

function AddArray(Arr : TPsArrayDef) : TPsArrayIndex;
var 
  ArrayIndex : TPsArrayIndex;
begin
  ArrayIndex := _AddDef(TdcArray)^.ArrayIndex;
  ArrayIndex^ := Arr;
  AddArray := ArrayIndex
end;

function FindConstant(Name : string) : TPsConstantIndex;
var 
  Def : TPsDefPtr;
  Ret : TPsConstantIndex;
begin
  Ret := nil;
  Def := Defs.Latest;
  while (Ret = nil) and (Def <> nil) do
  begin
    if (Def^.Cls = TdcConstant) and (Name = Def^.ConstantIndex^.Name) then
      Ret := Def^.ConstantIndex;
    Def := Def^.Prev
  end;
  FindConstant := Ret
end;

function AddConstant(Constant : TPsConstant) : TPsConstantIndex;
var 
  ConstantIndex : TPsConstantIndex;
begin
  if FindConstant(Constant.Name) <> nil then
    CompileError('Constant ' + Constant.Name + ' already defined');
  ConstantIndex := _AddDef(TdcConstant)^.ConstantIndex;
  ConstantIndex^ := Constant;
  AddConstant := ConstantIndex
end;

function AddVariable(VarDef : TPsVariable) : TPsVariableIndex;
var 
  VariableIndex : TPsVariableIndex;
begin
  if FindNameInLocalScope(VarDef.Name, {Required=}false) <> nil then
    CompileError('Identifier ' + VarDef.Name + ' already defined');
  VariableIndex := _AddDef(TdcVariable)^.VariableIndex;
  AddVariableName(VarDef.Name, VariableIndex);
  VariableIndex^ := VarDef;
  AddVariable := VariableIndex;
end;

function EmptyFunction : TPsFunction;
var 
  Ret : TPsFunction;
begin
  Ret.Name := '';
  Ret.ArgCount := 0;
  Ret.ReturnTypeIndex := nil;
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
  Decl := DeclIndex^;
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
  NameIndex : TPsNameIndex;
begin
  NameIndex := FindNameOfClassInLocalScope(Name, TncFunction, {Required=}false);
  HasForwardDeclaration := (NameIndex <> nil)
                           and (NameIndex^.FunctionIndex^.IsDeclaration)
end;

function AddFunction(Fun : TPsFunction) : TPsFunctionIndex;
var 
  NameIndex : TPsNameIndex;
  FnIndex : TPsFunctionIndex;
  IsProcedure : boolean;
begin
  IsProcedure := Fun.ReturnTypeIndex = nil;
  NameIndex := FindNameInLocalScope(Fun.Name, {Required=}false);
  if NameIndex = nil then
  begin
    FnIndex := _AddDef(TdcFunction)^.FunctionIndex;
    FnIndex^ := Fun;
    AddFunctionName(Fun.Name, FnIndex)
  end
  else
  begin
    if (NameIndex^.Cls <> TncFunction) or Fun.IsDeclaration then
      CompileError('Identifier ' + Fun.Name + ' already defined');
    FnIndex := NameIndex^.FunctionIndex;
    if FnIndex^.IsDeclaration then
    begin
      if ((Fun.ArgCount = 0) and (Fun.ReturnTypeIndex = nil))
         or IsSameFunctionDefinition(FnIndex, Fun) then
        FnIndex^.IsDeclaration := false
      else
      begin
        if IsProcedure then
          CompileError('Procedure ' + Fun.Name +
                       ' incompatible with its forward declaration')
        else
          CompileError('Function ' + Fun.Name +
                       ' incompatible with its forward declaration')
      end
    end
    else
    begin
      if IsProcedure then
        CompileError('Procedure ' + Fun.Name + ' already defined')
      else
        CompileError('Function ' + Fun.Name + ' already defined')
    end
  end;
  AddFunction := FnIndex;
end;

function FindField(TypeIndex : TPsTypeIndex; Name : string; Required : boolean)
: integer;
var 
  Pos : integer;
  Ret : integer;
begin
  if TypeIndex^.Cls <> TtcRecord then
    CompileError('Not a record: ' + TypeIndex^.Name);
  with TypeIndex^.RecordIndex^ do
  begin
    Ret := 0;
    Pos := Size;
    while (Pos >= 1) and (Ret = 0) do
    begin
      if Name = Fields[Pos].Name then Ret := Pos;
      Pos := Pos - 1
    end;
  end;
  if Required and (Ret = 0) then CompileError('Field not found: ' + Name);
  FindField := Ret
end;

function FindFieldType(TypeIndex : TPsTypeIndex; Name : string;
                       Required : boolean) : TPsTypeIndex;
var 
  Pos : integer;
begin
  Pos := FindField(TypeIndex, Name, Required);
  if Pos = 0 then FindFieldType := nil
  else FindFieldType := TypeIndex^.RecordIndex^.Fields[Pos].TypeIndex
end;

function FindWithVar(Name : string) : TPsWithVarIndex;
var 
  Ret : TPsWithVarIndex;
  Def : TPsDefPtr;
  TypeIndex : TPsTypeIndex;
begin
  Ret := nil;
  Def := Defs.Latest;
  while (Ret = nil) and (Def <> nil) and (Def^.Cls <> TdcScopeBoundary) do
  begin
    if Def^.Cls = TdcWithVar then
    begin
      TypeIndex := Def^.WithVarIndex^.VariableIndex^.TypeIndex;
      if FindFieldType(TypeIndex, Name, false) <> nil then
        Ret := Def^.WithVarIndex;
    end;
    Def := Def^.Prev
  end;
  FindWithVar := Ret
end;

function AddWithVar(Base : TExpression) : TPsVariableIndex;
var 
  TmpVarNum : string;
  TmpVar : TPsVariable;
  TmpVarIndex : TPsVariableIndex;
  WithVarIndex : TPsWithVarIndex;
begin
  if not IsRecordType(Base^.TypeIndex) then
    CompileError('''With'' variable is not a record');

  Str(DefCounter, TmpVarNum);
  TmpVar.Name := 'with' + TmpVarNum;
  TmpVar.TypeIndex := Base^.TypeIndex;
  TmpVar.IsConstant := Base^.IsConstant;
  TmpVar.IsReference := true;
  TmpVarIndex := AddVariable(TmpVar);
  WithVarIndex := _AddDef(TdcWithVar)^.WithVarIndex;
  WithVarIndex^.VariableIndex := TmpVarIndex;
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

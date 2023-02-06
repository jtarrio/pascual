var 
  Defs : TPsDefs;
  PrimitiveTypes : record
    PtNil, PtBoolean, PtInteger, PtChar, PtString, PtText : TPsTypePtr
  end;

function DefCounter : integer;
begin
  Defs.Counter := Defs.Counter + 1;
  DefCounter := Defs.Counter
end;

procedure InitDefs;
begin
  Defs.Latest := nil;
  Defs.CurrentFn := nil;
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
    TdcName : new(Def^.NamePtr);
    TdcType : new(Def^.TypePtr);
    TdcEnum : new(Def^.EnumPtr);
    TdcRange : new(Def^.RangePtr);
    TdcRecord : new(Def^.RecPtr);
    TdcArray : new(Def^.ArrayPtr);
    TdcConstant : new(Def^.ConstPtr);
    TdcVariable : new(Def^.VarPtr);
    TdcFunction : new(Def^.FnPtr);
    TdcWithVar : new(Def^.WithVarPtr);
    TdcScopeBoundary :
                       begin
                         Def^.TemporaryScope := false;
                         Def^.CurrentFn := nil
                       end
  end;
  _NewDef := Def
end;

procedure _DisposeDef(Def : TPsDefPtr);
begin
  case Def^.Cls of 
    TdcName : dispose(Def^.NamePtr);
    TdcType : dispose(Def^.TypePtr);
    TdcEnum : dispose(Def^.EnumPtr);
    TdcRange:
              begin
                DisposeExpr(Def^.RangePtr^.First);
                DisposeExpr(Def^.RangePtr^.Last);
                dispose(Def^.RangePtr)
              end;
    TdcRecord : dispose(Def^.RecPtr);
    TdcArray : dispose(Def^.ArrayPtr);
    TdcConstant : dispose(Def^.ConstPtr);
    TdcVariable : dispose(Def^.VarPtr);
    TdcFunction : dispose(Def^.FnPtr);
    TdcWithVar : dispose(Def^.WithVarPtr)
  end;
  dispose(Def)
end;

function TypeName(TypePtr : TPsTypePtr) : string;
forward;

procedure _CheckUnusedVariables(Def : TPsDefPtr);
var Where : string;
begin
  if Defs.CurrentFn = nil then
    Where := ' in program'
  else if Defs.CurrentFn^.ReturnTypePtr = nil then
         Where := ' in procedure ' + Defs.CurrentFn^.Name
  else
    Where := ' in function ' + Defs.CurrentFn^.Name;

  case Def^.Cls of 
    TdcVariable:
                 if not Def^.VarPtr^.WasUsed then
                   writeln(StdErr, 'Warning: variable ', Def^.VarPtr^.Name,
                           ' was not used', Where)
                 else if not Def^.VarPtr^.WasInitialized then
                        writeln(StdErr, 'Warning: variable ', Def^.VarPtr^.Name,
                                ' was not initialized', Where);
    TdcFunction: if not Def^.FnPtr^.WasUsed then
                 begin
                   if Def^.FnPtr^.ReturnTypePtr = nil then
                     writeln(StdErr, 'Warning: procedure ', Def^.FnPtr^.Name,
                             ' was not used')
                   else writeln(StdErr, 'Warning: function ', Def^.FnPtr^.Name,
                                ' was not used')
                 end;
    TdcType: if (Def^.TypePtr^.Name <> '') and not Def^.TypePtr^.WasUsed then
               writeln(StdErr, 'Warning: type ', TypeName(Def^.TypePtr),
               ' was not used')
  end
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
    _CheckUnusedVariables(Defs.Latest);
    DeletedDef := Defs.Latest^;
    _DisposeDef(Defs.Latest);
    Defs.Latest := DeletedDef.Prev;
    _DeleteDef := true
  end
end;

procedure _StartScope(Temporary : boolean; NewFunction : TPsFnPtr);
var 
  Def : TPsDefPtr;
begin
  Def := _AddDef(TdcScopeBoundary);
  Def^.TemporaryScope := Temporary;
  Def^.CurrentFn := Defs.CurrentFn;
  if not Temporary then
    Defs.CurrentFn := NewFunction
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
  Defs.CurrentFn := DeletedDef.CurrentFn
end;

procedure StartLocalScope(NewFunction : TPsFnPtr);
begin
  _StartScope({Temporary=}false, NewFunction)
end;

procedure CloseLocalScope;
begin
  _CloseScope({Temporary=}false)
end;

procedure StartTemporaryScope;
begin
  _StartScope({Temporary=}true, nil)
end;

procedure CloseTemporaryScope;
begin
  _CloseScope({Temporary=}true)
end;

function _FindName(Name : string; Required : boolean;
                   FromLocalScope : boolean) : TPsNamePtr;
var 
  Def : TPsDefPtr;
  Ret : TPsNamePtr;
begin
  Ret := nil;
  Def := Defs.Latest;
  while (Ret = nil)
        and (Def <> nil)
        and (not FromLocalScope or (Def^.Cls <> TdcScopeBoundary)) do
  begin
    if (Def^.Cls = TdcName) and (Name = Def^.NamePtr^.Name) then
      Ret := Def^.NamePtr;
    Def := Def^.Prev
  end;
  if Required and (Ret = nil) then CompileError('Unknown identifier: ' + Name);
  _FindName := Ret
end;

function _CheckNameClass(NamePtr : TPsNamePtr; Cls : TPsNameClass) : TPSNamePtr;
begin
  if (NamePtr <> nil) and (NamePtr^.Cls <> Cls) then
    case Cls of 
      TncType : CompileError('Not a type: ' + NamePtr^.Name);
      TncVariable : CompileError('Not a variable: ' + NamePtr^.Name);
      TncEnumVal : CompileError('Not an enumeration value: ' + NamePtr^.Name);
      TncFunction : CompileError('Not a procedure or function: ' +
                                 NamePtr^.Name);
      TncPseudoFn : CompileError('Not a procedure or function: ' +
                                 NamePtr^.Name);
      else CompileError('Internal error: name class mismatch')
    end;
  _CheckNameClass := NamePtr
end;

function FindNameInLocalScope(Name : string; Required : boolean) : TPsNamePtr;
begin
  FindNameInLocalScope := _FindName(Name, Required, {FromLocalScope=}true)
end;

function FindNameOfClassInLocalScope(Name : string; Cls : TPsNameClass;
                                     Required : boolean) : TPsNamePtr;
begin
  FindNameOfClassInLocalScope := _CheckNameClass(
                                 FindNameInLocalScope(Name, Required), Cls)
end;

function FindName(Name : string; Required : boolean) : TPsNamePtr;
begin
  FindName := _FindName(Name, Required, {FromLocalScope=}false)
end;

function FindNameOfClass(Name : string; Cls : TPsNameClass; Required : boolean)
: TPsNamePtr;
begin
  FindNameOfClass := _CheckNameClass(FindName(Name, Required), Cls)
end;

function _AddName(Name : string; Cls : TPsNameClass) : TPsNamePtr;
var 
  Pos : TPsNamePtr;
begin
  if FindNameInLocalScope(Name, {Required=}false) <> nil then
    CompileError('Identifier ' + Name + ' already defined');
  Pos := _AddDef(TdcName)^.NamePtr;
  Pos^.Name := Name;
  Pos^.Cls := Cls;
  _AddName := Pos
end;

function AddTypeName(Name : string; Idx : TPsTypePtr) : TPsNamePtr;
var 
  Def : TPsNamePtr;
begin
  Def := _AddName(Name, TncType);
  Def^.TypePtr := Idx;
  AddTypeName := Def
end;

function AddVariableName(Name : string; Idx : TPsVarPtr) : TPsNamePtr;
var 
  Def : TPsNamePtr;
begin
  Def := _AddName(Name, TncVariable);
  Def^.VarPtr := Idx;
  AddVariableName := Def
end;

function AddConstantName(Name : string; Idx : TPsConstPtr) : TPsNamePtr;
var 
  Def : TPsNamePtr;
begin
  Def := _AddName(Name, TncConstant);
  Def^.ConstPtr := Idx;
  Result := Def
end;

function AddFunctionName(Name : string; Idx : TPsFnPtr) : TPsNamePtr;
var 
  Def : TPsNamePtr;
begin
  Def := _AddName(Name, TncFunction);
  Def^.FnPtr := Idx;
  AddFunctionName := Def
end;

function AddEnumValName(Ordinal : integer;
                        TypeIdx : TPsTypePtr) : TPsNamePtr;
var Def : TPsNamePtr;
begin
  Def := _AddName(TypeIdx^.EnumPtr^.Values[Ordinal], TncEnumVal);
  Def^.EnumTypePtr := TypeIdx;
  Def^.Ordinal := Ordinal;
  AddEnumValName := Def
end;

function AddPseudoFn(Name : string; Fn : TPsPseudoFn) : TPsNamePtr;
var Def : TPsNamePtr;
begin
  Def := _AddName(Name, TncPseudoFn);
  Def^.PseudoFn := Fn;
  AddPseudoFn := Def
end;

function DeepTypeName(TypePtr : TPsTypePtr; UseOriginal : boolean) : string;
var 
  Typ : TPsType;
  Ret : string;
  Pos : integer;
begin
  repeat
    Typ := TypePtr^;
    TypePtr := Typ.AliasFor
  until not UseOriginal or (TypePtr = nil);
  if Typ.Name <> '' then DeepTypeName := Typ.Name
  else if Typ.Cls = TtcEnum then
  begin
    Ret := '(';
    for Pos := 0 to Typ.EnumPtr^.Size - 1 do
    begin
      if Pos <> 0 then
        Ret := Ret + ',';
      Ret := Ret + Typ.EnumPtr^.Values[Pos]
    end;
    DeepTypeName := Ret + ')'
  end
  else if Typ.Cls = TtcRange then
         DeepTypeName := DescribeExpr(Typ.RangePtr^.First, 1) +
                         '..' + DescribeExpr(Typ.RangePtr^.Last, 1)
  else if Typ.Cls = TtcRecord then
  begin
    Ret := 'record ';
    for Pos := 1 to Typ.RecPtr^.Size do
    begin
      if Pos <> 1 then Ret := Ret + ',';
      Ret := Ret + DeepTypeName(Typ.RecPtr^.Fields[Pos].TypePtr, true);
      Ret := Ret + ':' + Typ.RecPtr^.Fields[Pos].Name
    end;
    DeepTypeName := Ret + ' end'
  end
  else if Typ.Cls = TtcArray then
  begin
    Ret := 'array [' + DeepTypeName(Typ.ArrayPtr^.IndexTypePtr, false) +
           '] of ' + DeepTypeName(Typ.ArrayPtr^.ValueTypePtr, false);
    DeepTypeName := Ret
  end
  else if Typ.Cls = TtcPointer then
         DeepTypeName := '^' + DeepTypeName(Typ.PointedTypePtr, true)
  else
  begin
    Str(Typ.Cls, Ret);
    CompileError('Could not get name for type of class ' + Ret)
  end
end;

function TypeName(TypePtr : TPsTypePtr) : string;
begin
  if TypePtr = nil then TypeName := '(none)'
  else TypeName := DeepTypeName(TypePtr, false)
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

function CopyType(TypePtr : TPsTypePtr) : TPsType;
var 
  NewTyp : TPsType;
begin
  NewTyp := TypePtr^;
  if NewTyp.Cls = TtcPointerUnknown then
  begin
    new(NewTyp.TargetName);
    NewTyp.TargetName^ := TypePtr^.TargetName^
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

function IsIntegerType(TypePtr : TPsTypePtr) : boolean;
begin
  IsIntegerType := (TypePtr <> nil) and (TypePtr^.Cls = TtcInteger)
end;

function StringType : TPsType;
begin
  StringType := TypeOfClass(TtcString)
end;

function CharType : TPsType;
begin
  CharType := TypeOfClass(TtcChar)
end;

function IsStringType(TypePtr : TPsTypePtr) : boolean;
begin
  IsStringType := (TypePtr <> nil) and (TypePtr^.Cls = TtcString)
end;

function IsCharType(TypePtr : TPsTypePtr) : boolean;
begin
  IsCharType := (TypePtr <> nil) and (TypePtr^.Cls = TtcChar)
end;

function IsStringyType(TypePtr : TPsTypePtr) : boolean;
begin
  IsStringyType := IsStringType(TypePtr) or IsCharType(TypePtr)
end;

function BooleanType : TPsType;
begin
  BooleanType := TypeOfClass(TtcBoolean)
end;

function IsBooleanType(TypePtr : TPsTypePtr) : boolean;
begin
  IsBooleanType := (TypePtr <> nil) and (TypePtr^.Cls = TtcBoolean)
end;

function TextType : TPsType;
begin
  TextType := TypeOfClass(TtcText)
end;

function IsTextType(TypePtr : TPsTypePtr) : boolean;
begin
  IsTextType := (TypePtr <> nil) and (TypePtr^.Cls = TtcText)
end;

function IsEnumType(TypePtr : TPsTypePtr) : boolean;
begin
  IsEnumType := (TypePtr <> nil) and (TypePtr^.Cls = TtcEnum)
end;

function IsRangeType(TypePtr : TPsTypePtr) : boolean;
begin
  Result := (TypePtr <> nil) and (TypePtr^.Cls = TtcRange)
end;

function IsRecordType(TypePtr : TPsTypePtr) : boolean;
begin
  IsRecordType := (TypePtr <> nil) and (TypePtr^.Cls = TtcRecord)
end;

function IsArrayType(TypePtr : TPsTypePtr) : boolean;
begin
  IsArrayType := (TypePtr <> nil) and (TypePtr^.Cls = TtcArray)
end;

function PointerType(TypePtr : TPsTypePtr) : TPsType;
var 
  Typ : TPsType;
begin
  Typ := TypeOfClass(TtcPointer);
  Typ.PointedTypePtr := TypePtr;
  PointerType := Typ
end;

function IsPointerType(TypePtr : TPsTypePtr) : boolean;
begin
  IsPointerType := (TypePtr <> nil) and (TypePtr^.Cls = TtcPointer)
end;

function NilType : TPsType;
begin
  NilType := TypeOfClass(TtcNil)
end;

function IsNilType(TypePtr : TPsTypePtr) : boolean;
begin
  IsNilType := (TypePtr <> nil) and (TypePtr^.Cls = TtcNil)
end;

function IsPointeryType(TypePtr : TPsTypePtr) : boolean;
begin
  IsPointeryType := IsPointerType(TypePtr) or IsNilType(TypePtr)
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

function IsPointerUnknownType(TypePtr : TPsTypePtr) : boolean;
begin
  IsPointerUnknownType := (TypePtr <> nil)
                          and (TypePtr^.Cls = TtcPointerUnknown)
end;

function IsOrdinalType(TypePtr : TPsTypePtr) : boolean;
begin
  IsOrdinalType := IsBooleanType(TypePtr)
                   or IsIntegerType(TypePtr)
                   or IsCharType(TypePtr)
                   or IsEnumType(TypePtr)
                   or IsRangeType(TypePtr)
end;

function IsBoundedType(TypePtr : TPsTypePtr) : boolean;
begin
  Result := IsBooleanType(TypePtr)
            or IsCharType(TypePtr)
            or IsEnumType(TypePtr)
            or IsRangeType(TypePtr)
end;

function IsSimpleType(TypePtr : TPsTypePtr) : boolean;
begin
  IsSimpleType := IsOrdinalType(TypePtr) or IsStringType(TypePtr)
end;

function IsSameType(A, B : TPsTypePtr) : boolean;
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
                  and IsSameType(A^.PointedTypePtr, B^.PointedTypePtr))
  end
end;

function ArePointersCompatible(A, B : TPsTypePtr) : boolean;
begin
  ArePointersCompatible := IsPointeryType(A) and IsPointeryType(B) and
                           (IsNilType(A) or IsNilType(B)
                           or IsSameType(A, B))
end;

function AddType(Typ : TPsType) : TPsTypePtr;
var 
  TypePtr : TPsTypePtr;
  EnumPos : integer;
begin
  TypePtr := _AddDef(TdcType)^.TypePtr;
  TypePtr^ := Typ;
  AddType := TypePtr;

  if Typ.Name <> '' then
  begin
    if FindNameInLocalScope(Typ.Name, {Required=}false) <> nil then
      CompileError('Identifier ' + Typ.Name + ' already defined');
    AddTypeName(Typ.Name, TypePtr)
  end;

  if (Typ.Cls = TtcEnum) and (Typ.AliasFor = nil) then
    for EnumPos := 0 to Typ.EnumPtr^.Size - 1 do
      AddEnumValName(EnumPos, TypePtr)
end;

function AddEnum(Enum : TPsEnumDef) : TPsEnumPtr;
var 
  EnumPtr : TPsEnumPtr;
begin
  EnumPtr := _AddDef(TdcEnum)^.EnumPtr;
  EnumPtr^ := Enum;
  EnumPtr^.Id := DefCounter;
  AddEnum := EnumPtr
end;

function AddRange(Range : TPsRangeDef) : TPsRangePtr;
begin
  Result := _AddDef(TdcRange)^.RangePtr;
  Result^ := Range
end;

function AddRecord(Rec : TPsRecordDef) : TPsRecPtr;
var 
  RecPtr : TPsRecPtr;
begin
  RecPtr := _AddDef(TdcRecord)^.RecPtr;
  RecPtr^ := Rec;
  RecPtr^.Id := DefCounter;
  AddRecord := RecPtr
end;

function AddArray(Arr : TPsArrayDef) : TPsArrayPtr;
var 
  ArrayPtr : TPsArrayPtr;
begin
  ArrayPtr := _AddDef(TdcArray)^.ArrayPtr;
  ArrayPtr^ := Arr;
  AddArray := ArrayPtr
end;

function AddConstant(Constant : TPsConstant) : TPsConstPtr;
var 
  ConstPtr : TPsConstPtr;
begin
  if FindNameInLocalScope(Constant.Name, {Required=}false) <> nil then
    CompileError('Identifier ' + Constant.Name + ' already defined');
  ConstPtr := _AddDef(TdcConstant)^.ConstPtr;
  AddConstantName(Constant.Name, ConstPtr);
  ConstPtr^ := Constant;
  AddConstant := ConstPtr
end;

function AddVariable(VarDef : TPsVariable) : TPsVarPtr;
var 
  VarPtr : TPsVarPtr;
begin
  if FindNameInLocalScope(VarDef.Name, {Required=}false) <> nil then
    CompileError('Identifier ' + VarDef.Name + ' already defined');
  VarPtr := _AddDef(TdcVariable)^.VarPtr;
  AddVariableName(VarDef.Name, VarPtr);
  VarPtr^ := VarDef;
  AddVariable := VarPtr;
end;

function EmptyFunction : TPsFunction;
begin
  Result.Name := '';
  Result.ArgCount := 0;
  Result.ReturnTypePtr := nil;
  Result.IsDeclaration := false;
  Result.WasUsed := false
end;

function IsEmptyFunction(Fn : TPsFunction) : boolean;
begin
  IsEmptyFunction := Fn.Name = ''
end;

function IsSameFunctionDefinition(DeclPtr : TPsFnPtr;
                                  Fun : TPsFunction) : boolean;
var 
  Decl : TPsFunction;
  Same : boolean;
  Pos : integer;
begin
  Decl := DeclPtr^;
  Same := IsSameType(Decl.ReturnTypePtr, Fun.ReturnTypePtr)
          and (Decl.ArgCount = Fun.ArgCount);
  for Pos := 1 to Decl.ArgCount do
    Same := Same
            and IsSameType(Decl.Args[Pos].TypePtr, Fun.Args[Pos].TypePtr)
            and (Decl.Args[Pos].IsReference = Fun.Args[Pos].IsReference);
  IsSameFunctionDefinition := Same
end;

function HasForwardDeclaration(Name : string) : boolean;
var 
  NamePtr : TPsNamePtr;
begin
  NamePtr := FindNameOfClassInLocalScope(Name, TncFunction, {Required=}false);
  HasForwardDeclaration := (NamePtr <> nil)
                           and (NamePtr^.FnPtr^.IsDeclaration)
end;

function AddFunction(Fun : TPsFunction) : TPsFnPtr;
var 
  NamePtr : TPsNamePtr;
  FnPtr : TPsFnPtr;
  IsProcedure : boolean;
begin
  IsProcedure := Fun.ReturnTypePtr = nil;
  NamePtr := FindNameInLocalScope(Fun.Name, {Required=}false);
  if NamePtr = nil then
  begin
    FnPtr := _AddDef(TdcFunction)^.FnPtr;
    FnPtr^ := Fun;
    AddFunctionName(Fun.Name, FnPtr)
  end
  else
  begin
    if (NamePtr^.Cls <> TncFunction) or Fun.IsDeclaration then
      CompileError('Identifier ' + Fun.Name + ' already defined');
    FnPtr := NamePtr^.FnPtr;
    if FnPtr^.IsDeclaration then
    begin
      if ((Fun.ArgCount = 0) and (Fun.ReturnTypePtr = nil))
         or IsSameFunctionDefinition(FnPtr, Fun) then
        FnPtr^.IsDeclaration := false
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
  AddFunction := FnPtr;
end;

function FindField(TypePtr : TPsTypePtr; Name : string; Required : boolean)
: integer;
var 
  Pos : integer;
  Ret : integer;
begin
  if TypePtr^.Cls <> TtcRecord then
    CompileError('Not a record: ' + TypeName(TypePtr));
  with TypePtr^.RecPtr^ do
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

function FindFieldType(TypePtr : TPsTypePtr; Name : string;
                       Required : boolean) : TPsTypePtr;
var 
  Pos : integer;
begin
  Pos := FindField(TypePtr, Name, Required);
  if Pos = 0 then FindFieldType := nil
  else FindFieldType := TypePtr^.RecPtr^.Fields[Pos].TypePtr
end;

function FindWithVar(Name : string) : TPsWithVarPtr;
var 
  Ret : TPsWithVarPtr;
  Def : TPsDefPtr;
  TypePtr : TPsTypePtr;
begin
  Ret := nil;
  Def := Defs.Latest;
  while (Ret = nil) and (Def <> nil) and (Def^.Cls <> TdcScopeBoundary) do
  begin
    if Def^.Cls = TdcWithVar then
    begin
      TypePtr := Def^.WithVarPtr^.VarPtr^.TypePtr;
      if FindFieldType(TypePtr, Name, false) <> nil then
        Ret := Def^.WithVarPtr;
    end;
    Def := Def^.Prev
  end;
  FindWithVar := Ret
end;

function AddWithVar(Base : TExpression) : TPsVarPtr;
var 
  TmpVarNum : string;
  TmpVar : TPsVariable;
  TmpVarPtr : TPsVarPtr;
  WithVarPtr : TPsWithVarPtr;
begin
  if not IsRecordType(Base^.TypePtr) then
    CompileError('''With'' variable is not a record');

  Str(DefCounter, TmpVarNum);
  TmpVar.Name := 'with' + TmpVarNum;
  TmpVar.TypePtr := Base^.TypePtr;
  TmpVar.IsConstant := Base^.IsConstant;
  TmpVar.IsReference := true;
  TmpVarPtr := AddVariable(TmpVar);
  WithVarPtr := _AddDef(TdcWithVar)^.WithVarPtr;
  WithVarPtr^.VarPtr := TmpVarPtr;
  AddWithVar := TmpVarPtr
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

function MakeConstant(Name : string; Value : TExpression)
: TPsConstant;
var 
  Constant : TPsConstant;
begin
  Constant.Name := Name;
  Constant.Value := Value;
  MakeConstant := Constant
end;

function MakeTypedConstant(Name : string; TypePtr : TPsTypePtr)
: TPsVariable;
var 
  VarDef : TPsVariable;
begin
  VarDef.Name := Name;
  VarDef.TypePtr := TypePtr;
  VarDef.IsReference := false;
  VarDef.IsConstant := true;
  MakeTypedConstant := VarDef
end;

function MakeVariable(Name : string; TypePtr : TPsTypePtr; IsRef : boolean)
: TPsVariable;
var 
  VarDef : TPsVariable;
begin
  VarDef.Name := Name;
  VarDef.TypePtr := TypePtr;
  VarDef.IsReference := IsRef;
  VarDef.IsConstant := false;
  VarDef.WasInitialized := false;
  VarDef.WasUsed := false;
  MakeVariable := VarDef
end;

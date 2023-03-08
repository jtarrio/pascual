var 
  Defs : TPsDefs;
  PrimitiveTypes : record
    PtNil, PtBoolean, PtInteger, PtReal, PtChar, PtString, PtText : TPsTypePtr
  end;

function DefCounter(CounterType : TPsCounterType) : integer;
begin
  case CounterType of 
    TctEnum:
             begin
               Defs.Counters.EnumCtr := Defs.Counters.EnumCtr + 1;
               Result := Defs.Counters.EnumCtr
             end;
    TctRecord:
               begin
                 Defs.Counters.RecordCtr := Defs.Counters.RecordCtr + 1;
                 Result := Defs.Counters.RecordCtr
               end;
    TctTmpVar:
               begin
                 Defs.Counters.TmpVarCtr := Defs.Counters.TmpVarCtr + 1;
                 Result := Defs.Counters.TmpVarCtr
               end;
  end
end;

procedure InitDefs;
begin
  Defs.Latest := nil;
  Defs.Counters.EnumCtr := 0;
  Defs.Counters.RecordCtr := 0;
  Defs.Counters.TmpVarCtr := 0;
  Defs.CurrentFn := nil;
end;

function NewEnum(Enum : TPsEnumDef) : TPsEnumPtr;
begin
  new(Result);
  Result^ := Enum;
  Result^.Id := DefCounter(TctEnum);
  Result^.RefCount := 1
end;

procedure DisposeEnum(Ptr : TPsEnumPtr);
begin
  Ptr^.RefCount := Ptr^.RefCount - 1;
  if Ptr^.RefCount = 0 then dispose(Ptr)
end;

function NewRecord(Rec : TPsRecordDef) : TPsRecPtr;
begin
  new(Result);
  Result^ := Rec;
  Result^.Id := DefCounter(TctRecord);
  Result^.RefCount := 1
end;

procedure DisposeRecord(Ptr : TPsRecPtr);
begin
  Ptr^.RefCount := Ptr^.RefCount - 1;
  if Ptr^.RefCount = 0 then dispose(Ptr)
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

procedure _DisposeType(var TypePtr : TPsTypePtr);
begin
  if TypePtr^.Cls = TtcEnum then DisposeEnum(TypePtr^.EnumPtr)
  else if TypePtr^.Cls = TtcRecord then DisposeRecord(TypePtr^.RecPtr);
  dispose(TypePtr);
end;

procedure _DisposeDef(Def : TPsDefPtr);
begin
  case Def^.Cls of 
    TdcName : dispose(Def^.NamePtr);
    TdcType : _DisposeType(Def^.TypePtr);
    TdcConstant : dispose(Def^.ConstPtr);
    TdcVariable : dispose(Def^.VarPtr);
    TdcFunction : dispose(Def^.FnPtr);
    TdcWithVar : dispose(Def^.WithVarPtr)
  end;
  dispose(Def)
end;

function TypeName(TypePtr : TPsTypePtr) : string;
forward;

procedure _CheckUnusedSymbols(Def : TPsDefPtr);
var Where : string;
begin
  if Defs.CurrentFn = nil then
    Where := ''
  else if Defs.CurrentFn^.ReturnTypePtr = nil then
         Where := ' in procedure ' + Defs.CurrentFn^.Name
  else
    Where := ' in function ' + Defs.CurrentFn^.Name;

  case Def^.Cls of 
    TdcVariable:
                 if not Def^.VarPtr^.WasUsed then
                 begin
                   if Def^.VarPtr^.IsConstant then
                     CompileWarning('Constant ' + Def^.VarPtr^.Name +
                                    ' was not used' + Where)
                   else
                     CompileWarning('Variable ' + Def^.VarPtr^.Name +
                                    ' was not used' + Where)
                 end
                 else if not Def^.VarPtr^.WasInitialized then
                        CompileWarning('Variable ' + Def^.VarPtr^.Name +
                                       ' was not initialized' + Where);
    TdcFunction: if not Def^.FnPtr^.WasUsed then
                 begin
                   if Def^.FnPtr^.ReturnTypePtr = nil then
                     CompileWarning('Procedure ' + Def^.FnPtr^.Name +
                                    ' was not used')
                   else CompileWarning('Function ' + Def^.FnPtr^.Name +
                                       ' was not used')
                 end;
    TdcType: if (Def^.TypePtr^.Name <> '') and not Def^.TypePtr^.WasUsed then
               CompileWarning('Type ' + TypeName(Def^.TypePtr) +
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
    _CheckUnusedSymbols(Defs.Latest);
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
  Def^.Counters := Defs.Counters;
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
  Defs.CurrentFn := DeletedDef.CurrentFn;
  Defs.Counters := DeletedDef.Counters
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
      else InternalError('Name class mismatch for ' + NamePtr^.Name)
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

function EmptyType : TPsType;
var 
  Ret : TPsType;
begin
  Ret.Name := '';
  Ret.Cls := TtcBoolean;
  Ret.AliasFor := nil;
  Ret.WasUsed := false;
  EmptyType := Ret
end;

function CopyType(TypePtr : TPsTypePtr) : TPsType;
begin
  Result := TypePtr^;
  if Result.Cls = TtcPointerUnknown then
  begin
    new(Result.TargetName);
    Result.TargetName^ := TypePtr^.TargetName^
  end
  else if Result.Cls = TtcEnum then
         Result.EnumPtr^.RefCount := Result.EnumPtr^.RefCount + 1
  else if Result.Cls = TtcRecord then
         Result.RecPtr^.RefCount := Result.RecPtr^.RefCount + 1
end;

function TypeOfClass(Cls : TPsTypeClass) : TPsType;
var 
  Ret : TPsType;
begin
  Ret := EmptyType;
  Ret.Cls := Cls;
  TypeOfClass := Ret
end;

function IsIntegerType(TypePtr : TPsTypePtr) : boolean;
begin
  IsIntegerType := (TypePtr <> nil) and (TypePtr^.Cls = TtcInteger)
end;

function IsRealType(TypePtr : TPsTypePtr) : boolean;
begin
  Result := (TypePtr <> nil) and (TypePtr^.Cls = TtcReal)
end;

function IsNumericType(TypePtr : TPsTypePtr) : boolean;
begin
  Result := IsIntegerType(TypePtr) or IsRealType(TypePtr)
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

function IsBooleanType(TypePtr : TPsTypePtr) : boolean;
begin
  IsBooleanType := (TypePtr <> nil) and (TypePtr^.Cls = TtcBoolean)
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

function GetFundamentalType(TypePtr : TPsTypePtr) : TPsTypePtr;
begin
  while IsRangeType(TypePtr) do
    TypePtr := TypePtr^.RangeDef.BaseTypePtr;
  Result := TypePtr
end;

function IsSetType(TypePtr : TPsTypePtr) : boolean;
begin
  Result := (TypePtr <> nil) and (TypePtr^.Cls = TtcSet)
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

function GetTypeLowBound(TypePtr : TPsTypePtr) : integer;
begin
  case TypePtr^.Cls of 
    TtcBoolean : Result := 0;
    TtcChar : Result := 0;
    TtcEnum : Result := 0;
    TtcRange : Result := TypePtr^.RangeDef.First;
    else CompileError('Type is not bounded: ' + TypeName(TypePtr))
  end
end;

function GetTypeHighBound(TypePtr : TPsTypePtr) : integer;
begin
  case TypePtr^.Cls of 
    TtcBoolean : Result := 1;
    TtcChar : Result := 255;
    TtcEnum : Result := TypePtr^.EnumPtr^.Size - 1;
    TtcRange : Result := TypePtr^.RangeDef.Last;
    else CompileError('Type is not bounded: ' + TypeName(TypePtr))
  end
end;

function GetBoundedTypeSize(TypePtr : TPsTypePtr) : integer;
begin
  Result := GetTypeHighBound(TypePtr) - GetTypeLowBound(TypePtr) + 1
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
                  or (IsRangeType(A) and IsRangeType(B)
                  and IsSameType(GetFundamentalType(A), GetFundamentalType(B))
                  and (GetTypeLowBound(A) = GetTypeLowBound(B))
                  and (GetTypeHighBound(A) = GetTypeHighBound(B))
                  or (IsSetType(A) and IsSetType(B)
                  and IsSameType(A^.ElementTypePtr, B^.ElementTypePtr)))
  end
end;

function ArePointersCompatible(A, B : TPsTypePtr) : boolean;
begin
  ArePointersCompatible := IsPointeryType(A) and IsPointeryType(B) and
                           (IsNilType(A) or IsNilType(B)
                           or IsSameType(A, B))
end;

function UnparseChar(Chr : char) : string;
var 
  ChNum : string;
begin
  if Chr = '''' then Result := ''''''''''
  else if Chr < ' ' then
  begin
    Str(Ord(Chr), ChNum);
    Result := '#' + ChNum
  end
  else Result := '''' + Chr + ''''
end;

function UnparseString(St : string) : string;
var 
  Pos : integer;
  ChNum : string;
  Quoted : boolean;
begin
  Quoted := false;
  Result := '';
  for Pos := 1 to Length(St) do
  begin
    if St[Pos] < ' ' then
    begin
      if Quoted then
      begin
        Quoted := false;
        Result := Result + ''''
      end;
      Str(Ord(St[Pos]), ChNum);
      Result := Result + '#' + ChNum
    end
    else
    begin
      if not Quoted then
      begin
        Quoted := true;
        Result := Result + ''''
      end;
      if St[Pos] = '''' then Result := Result + ''''''
      else Result := Result + St[Pos]
    end
  end;
  if Quoted then Result := Result + '''';
  if Result = '' then Result := ''''''
end;

function _AntiOrdinal(Ordinal : integer; TypePtr : TPsTypePtr) : string;
begin
  Result := ExDescribe(ExGetAntiOrdinal(Ordinal, TypePtr));
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
  begin
    Result := _AntiOrdinal(Typ.RangeDef.First, Typ.RangeDef.BaseTypePtr) +
              '..' + _AntiOrdinal(Typ.RangeDef.Last, Typ.RangeDef.BaseTypePtr)
  end
  else if Typ.Cls = TtcSet then
  begin
    if Typ.ElementTypePtr = nil then Result := 'SET OF []'
    else Result := 'SET OF ' + DeepTypeName(Typ.ElementTypePtr, false)
  end
  else if Typ.Cls = TtcRecord then
  begin
    Ret := 'RECORD ';
    for Pos := 1 to Typ.RecPtr^.Size do
    begin
      if Pos <> 1 then Ret := Ret + ',';
      Ret := Ret + DeepTypeName(Typ.RecPtr^.Fields[Pos].TypePtr, true);
      Ret := Ret + ':' + Typ.RecPtr^.Fields[Pos].Name
    end;
    DeepTypeName := Ret + ' END'
  end
  else if Typ.Cls = TtcArray then
  begin
    Ret := 'ARRAY [' + DeepTypeName(Typ.ArrayDef.IndexTypePtr, false) +
           '] OF ' + DeepTypeName(Typ.ArrayDef.ValueTypePtr, false);
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
  Result.ExternalName := '';
  Result.ArgCount := 0;
  Result.ReturnTypePtr := nil;
  Result.IsDeclaration := false;
  Result.WasUsed := false
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

  Str(DefCounter(TctTmpVar), TmpVarNum);
  TmpVar.Name := 'with' + TmpVarNum;
  TmpVar.TypePtr := Base^.TypePtr;
  TmpVar.IsConstant := false;
  TmpVar.IsReference := Base^.IsAssignable;
  TmpVar.WasInitialized := true;
  TmpVar.WasUsed := true;
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
  VarDef.WasInitialized := true;
  MakeTypedConstant := VarDef
end;

function MakeVariable(Name : string; TypePtr : TPsTypePtr) : TPsVariable;
begin
  Result.Name := Name;
  Result.TypePtr := TypePtr;
  Result.IsReference := false;
  Result.IsConstant := false;
  Result.WasInitialized := false;
  Result.WasUsed := false
end;

function AddTmpVariable(Prefix : string; TypePtr : TPsTypePtr) : TPsVarPtr;
var VarNum : string;
begin
  Str(DefCounter(TctTmpVar), VarNum);
  Result := AddVariable(MakeVariable(Prefix + VarNum, TypePtr))
end;

function _MakeArg(Name : string; TypePtr : TPsTypePtr;
                  IsRef, IsConst : boolean) : TPsVariable;
begin
  Result.Name := Name;
  Result.TypePtr := TypePtr;
  Result.IsReference := IsRef or IsConst;
  Result.IsConstant := IsConst;
  Result.WasInitialized := false;
  Result.WasUsed := false
end;

function MakeArg(Name : string; TypePtr : TPsTypePtr) : TPsVariable;
begin
  MakeArg := _MakeArg(Name, TypePtr, false, false)
end;

function MakeVarArg(Name : string; TypePtr : TPsTypePtr) : TPsVariable;
begin
  MakeVarArg := _MakeArg(Name, TypePtr, true, false)
end;

function MakeConstArg(Name : string; TypePtr : TPsTypePtr) : TPsVariable;
begin
  MakeConstArg := _MakeArg(Name, TypePtr, false, true)
end;

function MakeProcedure0(Name : string) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ArgCount := 0
end;

function MakeProcedure1(Name : string; Arg : TPsVariable) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ArgCount := 1;
  Result.Args[1] := Arg
end;

function MakeProcedure2(Name : string; Arg1, Arg2 : TPsVariable) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ArgCount := 2;
  Result.Args[1] := Arg1;
  Result.Args[2] := Arg2
end;

function MakeProcedure3(Name : string;
                        Arg1, Arg2, Arg3 : TPsVariable) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ArgCount := 3;
  Result.Args[1] := Arg1;
  Result.Args[2] := Arg2;
  Result.Args[3] := Arg3
end;

function MakeFunction0(Name : string; RetTypePtr : TPsTypePtr) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ReturnTypePtr := RetTypePtr
end;

function MakeFunction1(Name : string; RetTypePtr : TPsTypePtr;
                       Arg : TPsVariable) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ReturnTypePtr := RetTypePtr;
  Result.ArgCount := 1;
  Result.Args[1] := Arg
end;

function MakeFunction2(Name : string; RetTypePtr : TPsTypePtr;
                       Arg1, Arg2 : TPsVariable) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ReturnTypePtr := RetTypePtr;
  Result.ArgCount := 2;
  Result.Args[1] := Arg1;
  Result.Args[2] := Arg2
end;

function MakeFunction3(Name : string; RetTypePtr : TPsTypePtr;
                       Arg1, Arg2, Arg3 : TPsVariable) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ReturnTypePtr := RetTypePtr;
  Result.ArgCount := 3;
  Result.Args[1] := Arg1;
  Result.Args[2] := Arg2;
  Result.Args[3] := Arg3
end;

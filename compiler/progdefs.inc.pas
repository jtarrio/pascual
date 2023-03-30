var 
  Defs : TPsDefs;
  PrimitiveTypes : record
    PtNil, PtBoolean, PtInteger, PtReal, PtChar, PtString, PtText,
    PtEmptySet, PtRawPtr : TPsTypePtr
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

function NewEnum(const Enum : TPsEnumDef) : TPsEnumPtr;
begin
  new(Result);
  Result^ := Enum;
  Result^.Id := DefCounter(TctEnum);
  Result^.RefCount := 1;
  Result^.HasBeenDefined := false;
  Result^.ValuesHaveBeenOutput := false
end;

procedure DisposeEnum(Ptr : TPsEnumPtr);
begin
  Ptr^.RefCount := Ptr^.RefCount - 1;
  if Ptr^.RefCount = 0 then dispose(Ptr)
end;

function NewRecord(const Rec : TPsRecordDef) : TPsRecPtr;
begin
  new(Result);
  Result^ := Rec;
  Result^.Id := DefCounter(TctRecord);
  Result^.RefCount := 1;
  Result^.HasBeenDefined := false
end;

procedure DisposeRecord(Ptr : TPsRecPtr);
begin
  Ptr^.RefCount := Ptr^.RefCount - 1;
  if Ptr^.RefCount = 0 then dispose(Ptr)
end;

function NewFnDef : TPsFnDefPtr;
begin
  new(Result);
  Result^.RefCount := 1
end;

procedure DisposeFnDef(Ptr : TPsFnDefPtr);
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
    TdcPseudoFn : new(Def^.PseudoFnPtr);
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
  else if TypePtr^.Cls = TtcRecord then DisposeRecord(TypePtr^.RecPtr)
  else if TypePtr^.Cls = TtcFunction then DisposeFnDef(TypePtr^.FnDefPtr);
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
    TdcPseudoFn : dispose(Def^.PseudoFnPtr);
    TdcWithVar : dispose(Def^.WithVarPtr)
  end;
  dispose(Def)
end;

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

function _FindName(const Name : string; Required : boolean;
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

function FindNameInLocalScope(const Name : string;
                              Required : boolean) : TPsNamePtr;
begin
  FindNameInLocalScope := _FindName(Name, Required, {FromLocalScope=}true)
end;

function FindNameOfClassInLocalScope(const Name : string; Cls : TPsNameClass;
                                     Required : boolean) : TPsNamePtr;
begin
  FindNameOfClassInLocalScope := _CheckNameClass(
                                 FindNameInLocalScope(Name, Required), Cls)
end;

function FindName(const Name : string; Required : boolean) : TPsNamePtr;
begin
  FindName := _FindName(Name, Required, {FromLocalScope=}false)
end;

function FindNameOfClass(const Name : string; Cls : TPsNameClass;
                         Required : boolean)
: TPsNamePtr;
begin
  FindNameOfClass := _CheckNameClass(FindName(Name, Required), Cls)
end;

function _AddName(const Name : string; Cls : TPsNameClass) : TPsNamePtr;
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

function AddTypeName(const Name : string; Idx : TPsTypePtr) : TPsNamePtr;
begin
  Result := _AddName(Name, TncType);
  Result^.TypePtr := Idx
end;

function AddVariableName(const Name : string; Idx : TPsVarPtr) : TPsNamePtr;
var 
  Def : TPsNamePtr;
begin
  Def := _AddName(Name, TncVariable);
  Def^.VarPtr := Idx;
  AddVariableName := Def
end;

function AddConstantName(const Name : string; Idx : TPsConstPtr) : TPsNamePtr;
var 
  Def : TPsNamePtr;
begin
  Def := _AddName(Name, TncConstant);
  Def^.ConstPtr := Idx;
  Result := Def
end;

function AddFunctionName(const Name : string; Idx : TPsFnPtr) : TPsNamePtr;
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

function AddPseudoFn(const Name : string;
                     Parse : TPsPseudoFnParser) : TPsPseudoFnPtr;
var Def : TPsNamePtr;
begin
  Def := _AddName(Name, TncPseudoFn);
  Def^.PseudoFnPtr := _AddDef(TdcPseudoFn)^.PseudoFnPtr;
  Def^.PseudoFnPtr^.Name := Name;
  Def^.PseudoFnPtr^.ParseFn := Parse;
  Result := Def^.PseudoFnPtr
end;

function CopyType(TypePtr : TPsTypePtr) : TPsType;
begin
  Result := TypePtr^;
  if Result.Cls = TtcPointerForward then
  begin
    new(Result.TargetName);
    Result.TargetName^ := TypePtr^.TargetName^
  end
  else if Result.Cls = TtcEnum then
         Result.EnumPtr^.RefCount := Result.EnumPtr^.RefCount + 1
  else if Result.Cls = TtcRecord then
         Result.RecPtr^.RefCount := Result.RecPtr^.RefCount + 1
  else if Result.Cls = TtcFunction then
         Result.FnDefPtr^.RefCount := Result.FnDefPtr^.RefCount + 1
end;

function GetFundamentalType(TypePtr : TPsTypePtr) : TPsTypePtr;
begin
  if (TypePtr <> nil) and (TypePtr^.Cls = TtcRange) then
    Result := TypePtr^.RangeDef.BaseTypePtr
  else
    Result := TypePtr
end;

function _TypeHasClass(TypePtr : TPsTypePtr; Cls : TPsTypeClass) : boolean;
begin
  Result := (TypePtr <> nil) and (GetFundamentalType(TypePtr)^.Cls = Cls)
end;

function IsIntegerType(TypePtr : TPsTypePtr) : boolean;
begin
  IsIntegerType := _TypeHasClass(TypePtr, TtcInteger)
end;

function IsRealType(TypePtr : TPsTypePtr) : boolean;
begin
  Result := _TypeHasClass(TypePtr, TtcReal)
end;

function IsNumericType(TypePtr : TPsTypePtr) : boolean;
begin
  Result := IsIntegerType(TypePtr) or IsRealType(TypePtr)
end;

function IsStringType(TypePtr : TPsTypePtr) : boolean;
begin
  IsStringType := _TypeHasClass(TypePtr, TtcString)
end;

function IsCharType(TypePtr : TPsTypePtr) : boolean;
begin
  IsCharType := _TypeHasClass(TypePtr, TtcChar)
end;

function IsStringyType(TypePtr : TPsTypePtr) : boolean;
begin
  IsStringyType := IsStringType(TypePtr) or IsCharType(TypePtr)
end;

function IsBooleanType(TypePtr : TPsTypePtr) : boolean;
begin
  IsBooleanType := _TypeHasClass(TypePtr, TtcBoolean)
end;

function IsTextType(TypePtr : TPsTypePtr) : boolean;
begin
  IsTextType := _TypeHasClass(TypePtr, TtcText)
end;

function IsEnumType(TypePtr : TPsTypePtr) : boolean;
begin
  IsEnumType := _TypeHasClass(TypePtr, TtcEnum)
end;

function IsRangeType(TypePtr : TPsTypePtr) : boolean;
begin
  Result := (TypePtr <> nil) and (TypePtr^.Cls = TtcRange)
end;

function IsSetType(TypePtr : TPsTypePtr) : boolean;
begin
  Result := _TypeHasClass(TypePtr, TtcSet)
end;

function IsRecordType(TypePtr : TPsTypePtr) : boolean;
begin
  IsRecordType := _TypeHasClass(TypePtr, TtcRecord)
end;

function IsArrayType(TypePtr : TPsTypePtr) : boolean;
begin
  IsArrayType := _TypeHasClass(TypePtr, TtcArray)
end;

function IsPointerType(TypePtr : TPsTypePtr) : boolean;
begin
  IsPointerType := _TypeHasClass(TypePtr, TtcPointer)
end;

function IsNilType(TypePtr : TPsTypePtr) : boolean;
begin
  IsNilType := _TypeHasClass(TypePtr, TtcNil)
end;

function IsPointeryType(TypePtr : TPsTypePtr) : boolean;
begin
  IsPointeryType := IsPointerType(TypePtr) or IsNilType(TypePtr)
end;

function IsPointerForwardType(TypePtr : TPsTypePtr) : boolean;
begin
  IsPointerForwardType := _TypeHasClass(TypePtr, TtcPointerForward)
end;

function IsRawPtrType(TypePtr : TPsTypePtr) : boolean;
begin
  IsRawPtrType := _TypeHasClass(TypePtr, TtcRawPtr)
end;

function IsFunctionType(TypePtr : TPsTypePtr) : boolean;
begin
  IsFunctionType := _TypeHasClass(TypePtr, TtcFunction)
end;

function IsFunctionyType(TypePtr : TPsTypePtr) : boolean;
begin
  Result := IsFunctionType(TypePtr) or IsNilType(TypePtr)
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
    else ErrorForType('Expected bounded type', TypePtr)
  end
end;

function GetTypeHighBound(TypePtr : TPsTypePtr) : integer;
begin
  case TypePtr^.Cls of 
    TtcBoolean : Result := 1;
    TtcChar : Result := 255;
    TtcEnum : Result := TypePtr^.EnumPtr^.Size - 1;
    TtcRange : Result := TypePtr^.RangeDef.Last;
    else ErrorForType('Expected bounded type', TypePtr)
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
                  and (GetTypeHighBound(A) = GetTypeHighBound(B)))
                  or (IsSetType(A) and IsSetType(B)
                  and IsSameType(A^.ElementTypePtr, B^.ElementTypePtr))
  end
end;

function IsFundamentallySameType(A, B : TPsTypePtr) : boolean;
begin
  Result := IsSameType(GetFundamentalType(A), GetFundamentalType(B))
end;

function ArePointersCompatible(A, B : TPsTypePtr) : boolean;
begin
  ArePointersCompatible := IsPointeryType(A) and IsPointeryType(B) and
                           (IsNilType(A) or IsNilType(B)
                           or IsSameType(A, B))
end;

function UnparseChar(Chr : char) : string;
begin
  if Chr = '''' then Result := ''''''''''
  else if Chr < ' ' then Result := '#' + IntToStr(Ord(Chr))
  else Result := '''' + Chr + ''''
end;

function UnparseString(const St : string) : string;
var 
  Pos : integer;
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
      Result := Result + '#' + IntToStr(Ord(St[Pos]))
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
  Pos : integer;
begin
  repeat
    Typ := TypePtr^;
    TypePtr := Typ.AliasFor
  until not UseOriginal or (TypePtr = nil);
  if Typ.Name <> '' then Result := Typ.Name
  else if Typ.Cls = TtcEnum then
  begin
    Result := '(';
    for Pos := 0 to Typ.EnumPtr^.Size - 1 do
    begin
      if Pos <> 0 then
        Result := Result + ',';
      Result := Result + Typ.EnumPtr^.Values[Pos]
    end;
    Result := Result + ')'
  end
  else if Typ.Cls = TtcRange then
         Result := _AntiOrdinal(Typ.RangeDef.First, Typ.RangeDef.BaseTypePtr) +
                   '..' + _AntiOrdinal(Typ.RangeDef.Last, Typ.RangeDef.
                   BaseTypePtr)
  else if Typ.Cls = TtcSet then
  begin
    if Typ.ElementTypePtr = nil then Result := 'SET OF []'
    else Result := 'SET OF ' + DeepTypeName(Typ.ElementTypePtr, false)
  end
  else if Typ.Cls = TtcRecord then
  begin
    Result := 'RECORD ';
    for Pos := 1 to Typ.RecPtr^.Size do
    begin
      if Pos <> 1 then Result := Result + ',';
      Result := Result + DeepTypeName(Typ.RecPtr^.Fields[Pos].TypePtr, false) +
                ':' + Typ.RecPtr^.Fields[Pos].Name
    end;
    Result := Result + ' END'
  end
  else if Typ.Cls = TtcArray then
         Result := 'ARRAY [' + DeepTypeName(Typ.ArrayDef.IndexTypePtr, false) +
                   '] OF ' + DeepTypeName(Typ.ArrayDef.ValueTypePtr, false)
  else if Typ.Cls = TtcPointer then
         Result := '^' + DeepTypeName(Typ.PointedTypePtr, UseOriginal)
  else if Typ.Cls = TtcFunction then
         with Typ.FnDefPtr^ do
  begin
    if ReturnTypePtr = nil then Result := 'PROCEDURE'
    else Result := 'FUNCTION';
    if Args.Count > 0 then
    begin
      Result := Result + '(';
      for Pos := 1 to Args.Count do
      begin
        if Pos <> 1 then Result := Result + '; ';
        if Args.Defs[Pos].IsConstant then Result := Result + 'CONST '
        else if Args.Defs[Pos].IsReference then Result := Result + 'VAR ';
        Result := Result + Args.Defs[Pos].Name;
        Result := Result + ' : ' + DeepTypeName(Args.Defs[Pos].TypePtr, false)
      end;
      Result := Result + ')';
    end;
    if ReturnTypePtr <> nil then
      Result := Result + ' : ' + DeepTypeName(ReturnTypePtr, false)
  end
  else
  begin
    Str(Typ.Cls, Result);
    CompileError('Could not get name for type of class ' + Result)
  end
end;

function TypeName(TypePtr : TPsTypePtr) : string;
begin
  if TypePtr = nil then TypeName := '(none)'
  else TypeName := DeepTypeName(TypePtr, false)
end;

function AddConstant(const Constant : TPsConstant) : TPsConstPtr;
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

function AddVariable(const VarDef : TPsVariable) : TPsVarPtr;
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
  Result.Args.Count := 0;
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
          and (Decl.Args.Count = Fun.Args.Count);
  for Pos := 1 to Decl.Args.Count do
    Same := Same
            and IsSameType(Decl.Args.Defs[Pos].TypePtr,
            Fun.Args.Defs[Pos].TypePtr)
            and (Decl.Args.Defs[Pos].IsReference =
            Fun.Args.Defs[Pos].IsReference);
  IsSameFunctionDefinition := Same
end;

function HasForwardDeclaration(const Name : string) : boolean;
var 
  NamePtr : TPsNamePtr;
begin
  NamePtr := FindNameOfClassInLocalScope(Name, TncFunction, {Required=}false);
  HasForwardDeclaration := (NamePtr <> nil)
                           and (NamePtr^.FnPtr^.IsDeclaration)
end;

function AddFunction(const Fun : TPsFunction) : TPsFnPtr;
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
      if ((Fun.Args.Count = 0) and (Fun.ReturnTypePtr = nil))
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

function FindField(TypePtr : TPsTypePtr; const Name : string;
                   Required : boolean)
: integer;
var 
  Pos : integer;
  Ret : integer;
begin
  EnsureRecordType(TypePtr);
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

function FindFieldType(TypePtr : TPsTypePtr; const Name : string;
                       Required : boolean) : TPsTypePtr;
var 
  Pos : integer;
begin
  Pos := FindField(TypePtr, Name, Required);
  if Pos = 0 then FindFieldType := nil
  else FindFieldType := TypePtr^.RecPtr^.Fields[Pos].TypePtr
end;

function FindWithVar(const Name : string) : TPsWithVarPtr;
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
  TmpVar : TPsVariable;
  TmpVarPtr : TPsVarPtr;
  WithVarPtr : TPsWithVarPtr;
begin
  EnsureRecordExpr(Base);

  TmpVar.Name := 'with' + IntToStr(DefCounter(TctTmpVar));
  TmpVar.TypePtr := Base^.TypePtr;
  TmpVar.IsConstant := false;
  TmpVar.IsReference := Base^.IsAddressable;
  TmpVar.WasInitialized := true;
  TmpVar.WasUsed := true;
  TmpVarPtr := AddVariable(TmpVar);
  WithVarPtr := _AddDef(TdcWithVar)^.WithVarPtr;
  WithVarPtr^.VarPtr := TmpVarPtr;
  AddWithVar := TmpVarPtr
end;

function MakeConstant(const Name : string; Value : TExpression)
: TPsConstant;
var 
  Constant : TPsConstant;
begin
  Constant.Name := Name;
  Constant.Value := Value;
  MakeConstant := Constant
end;

function MakeTypedConstant(const Name : string; TypePtr : TPsTypePtr)
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

function MakeVariable(const Name : string; TypePtr : TPsTypePtr) : TPsVariable;
begin
  Result.Name := Name;
  Result.TypePtr := TypePtr;
  Result.IsReference := false;
  Result.IsConstant := false;
  Result.WasInitialized := false;
  Result.WasUsed := false
end;

function AddTmpVariable(const Prefix : string;
                        TypePtr : TPsTypePtr) : TPsVarPtr;
begin
  Result := AddVariable(MakeVariable(Prefix + IntToStr(DefCounter(TctTmpVar)),
            TypePtr))
end;

function _MakeArg(const Name : string; TypePtr : TPsTypePtr;
                  IsRef, IsConst : boolean) : TPsVariable;
begin
  Result.Name := Name;
  Result.TypePtr := TypePtr;
  Result.IsReference := IsRef or IsConst;
  Result.IsConstant := IsConst;
  Result.WasInitialized := false;
  Result.WasUsed := false
end;

function MakeArg(const Name : string; TypePtr : TPsTypePtr) : TPsVariable;
begin
  MakeArg := _MakeArg(Name, TypePtr, false, false)
end;

function MakeVarArg(const Name : string; TypePtr : TPsTypePtr) : TPsVariable;
begin
  MakeVarArg := _MakeArg(Name, TypePtr, true, false)
end;

function MakeConstArg(const Name : string; TypePtr : TPsTypePtr) : TPsVariable;
begin
  MakeConstArg := _MakeArg(Name, TypePtr, false, true)
end;

function MakeProcedure0(const Name : string) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.Args.Count := 0
end;

function MakeProcedure1(const Name : string; Arg : TPsVariable) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.Args.Count := 1;
  Result.Args.Defs[1] := Arg
end;

function MakeProcedure2(const Name : string;
                        Arg1, Arg2 : TPsVariable) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.Args.Count := 2;
  Result.Args.Defs[1] := Arg1;
  Result.Args.Defs[2] := Arg2
end;

function MakeProcedure3(const Name : string;
                        Arg1, Arg2, Arg3 : TPsVariable) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.Args.Count := 3;
  Result.Args.Defs[1] := Arg1;
  Result.Args.Defs[2] := Arg2;
  Result.Args.Defs[3] := Arg3
end;

function MakeFunction0(const Name : string;
                       RetTypePtr : TPsTypePtr) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ReturnTypePtr := RetTypePtr
end;

function MakeFunction1(const Name : string; RetTypePtr : TPsTypePtr;
                       Arg : TPsVariable) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ReturnTypePtr := RetTypePtr;
  Result.Args.Count := 1;
  Result.Args.Defs[1] := Arg
end;

function MakeFunction2(const Name : string; RetTypePtr : TPsTypePtr;
                       Arg1, Arg2 : TPsVariable) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ReturnTypePtr := RetTypePtr;
  Result.Args.Count := 2;
  Result.Args.Defs[1] := Arg1;
  Result.Args.Defs[2] := Arg2
end;

function MakeFunction3(const Name : string; RetTypePtr : TPsTypePtr;
                       Arg1, Arg2, Arg3 : TPsVariable) : TPsFunction;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ReturnTypePtr := RetTypePtr;
  Result.Args.Count := 3;
  Result.Args.Defs[1] := Arg1;
  Result.Args.Defs[2] := Arg2;
  Result.Args.Defs[3] := Arg3
end;

function _UnaliasType(TypePtr : TPsTypePtr) : TPsTypePtr;
begin
  Result := TypePtr;
  while Result^.AliasFor <> nil do
    Result := Result^.AliasFor
end;

function _NewType(Cls : TPsTypeClass) : TPsTypePtr;
begin
  Result := _AddDef(TdcType)^.TypePtr;
  Result^.Name := '';
  Result^.Cls := Cls;
  Result^.AliasFor := nil;
  Result^.WasUsed := false
end;

function MakeBaseType(const Name : String; Cls : TPsTypeClass) : TPsTypePtr;
begin
  Result := _NewType(Cls);
  Result^.Name := Name;
  AddTypeName(Name, Result)
end;

function MakeEnumType(const Enum : TPsEnumDef) : TPsTypePtr;
var Pos : integer;
begin
  Result := _NewType(TtcEnum);
  Result^.EnumPtr := NewEnum(Enum);
  { It's hard to detect when an enumerated type was used }
  Result^.WasUsed := true;
  for Pos := 0 to Result^.EnumPtr^.Size - 1 do
    AddEnumValName(Pos, Result)
end;

function MakeRecordType(const Rec : TPsRecordDef) : TPsTypePtr;
begin
  Result := _NewType(TtcRecord);
  Result^.RecPtr := NewRecord(Rec)
end;

function MakeArrayType(IndexType, ValueType : TPsTypePtr) : TPsTypePtr;
var 
  Def : TPsDefPtr;
begin
  Result := nil;
  Def := Defs.Latest;
  while (Def <> nil) and (Result = nil) do
  begin
    if (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcArray)
       and IsSameType(Def^.TypePtr^.ArrayDef.IndexTypePtr, IndexType)
       and IsSameType(Def^.TypePtr^.ArrayDef.ValueTypePtr, ValueType) then
      Result := _UnaliasType(Def^.TypePtr);
    Def := Def^.Prev
  end;
  if Result = nil then
  begin
    if not IsBoundedType(IndexType) then
      ErrorForType('Array indices must belong to a bounded ordinal type',
                   IndexType);
    Result := _NewType(TtcArray);
    Result^.ArrayDef.IndexTypePtr := IndexType;
    Result^.ArrayDef.ValueTypePtr := ValueType
  end
end;

function MakeRangeType(TypePtr : TPsTypePtr;
                       First, Last : integer) : TPsTypePtr;
var 
  Def : TPsDefPtr;
begin
  Result := nil;
  Def := Defs.Latest;
  while (Def <> nil) and (Result = nil) do
  begin
    if (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcRange)
       and IsSameType(Def^.TypePtr^.RangeDef.BaseTypePtr, TypePtr)
       and (First = Def^.TypePtr^.RangeDef.First)
       and (Last = Def^.TypePtr^.RangeDef.Last) then
      Result := _UnaliasType(Def^.TypePtr);
    Def := Def^.Prev
  end;
  if Result = nil then
  begin
    if First > Last then
      CompileError('The bounds of a subrange must be in ascending order');
    Result := _NewType(TtcRange);
    Result^.RangeDef.First := First;
    Result^.RangeDef.Last := Last;
    Result^.RangeDef.BaseTypePtr := GetFundamentalType(TypePtr)
  end
end;

function MakePointerType(TypePtr : TPsTypePtr) : TPsTypePtr;
var 
  Def : TPsDefPtr;
begin
  Result := nil;
  Def := Defs.Latest;
  while (Def <> nil) and (Result = nil) do
  begin
    if (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcPointer)
       and IsSameType(Def^.TypePtr^.PointedTypePtr, TypePtr) then
      Result := _UnaliasType(Def^.TypePtr);
    Def := Def^.Prev
  end;
  if Result = nil then
  begin
    Result := _NewType(TtcPointer);
    Result^.PointedTypePtr := TypePtr
  end
end;

function MakePointerForwardType(const TargetName : string) : TPsTypePtr;
var 
  Def : TPsDefPtr;
begin
  Result := nil;
  Def := Defs.Latest;
  while (Def <> nil) and (Result = nil) do
  begin
    if (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcPointerForward)
       and (Def^.TypePtr^.TargetName^ = TargetName) then
      Result := _UnaliasType(Def^.TypePtr);
    Def := Def^.Prev
  end;
  if Result = nil then
  begin
    Result := _NewType(TtcPointerForward);
    new(Result^.TargetName);
    Result^.TargetName^ := TargetName
  end
end;

function MakeSetType(TypePtr : TPsTypePtr) : TPsTypePtr;
var 
  Def : TPsDefPtr;
begin
  Result := nil;
  Def := Defs.Latest;
  while (Def <> nil) and (Result = nil) do
  begin
    if (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcSet)
       and IsSameType(Def^.TypePtr^.ElementTypePtr, TypePtr) then
      Result := _UnaliasType(Def^.TypePtr);
    Def := Def^.Prev
  end;
  if Result = nil then
  begin
    Result := _NewType(TtcSet);
    Result^.ElementTypePtr := TypePtr
  end
end;

function AreSameArgs(const A, B : TPsFnArgs) : boolean;
var Pos : integer;
begin
  Result := A.Count = B.Count;
  if Result then
    for Pos := 1 to A.Count do
      Result := Result and (A.Defs[Pos].IsReference = B.Defs[Pos].IsReference)
                and (A.Defs[Pos].IsConstant = B.Defs[Pos].IsConstant)
                and IsSameType(A.Defs[Pos].TypePtr, B.Defs[Pos].TypePtr)
end;

function MakeFunctionType(const Args : TPsFnArgs;
                          ReturnTypePtr : TPsTypePtr) : TPsTypePtr;
var Def : TPsDefPtr;
begin
  Result := nil;
  Def := Defs.Latest;
  while (Def <> nil) and (Result = nil) do
  begin
    if (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcFunction)
       and IsSameType(Def^.TypePtr^.FnDefPtr^.ReturnTypePtr, ReturnTypePtr)
       and AreSameArgs(Def^.TypePtr^.FnDefPtr^.Args, Args) then
      Result := _UnaliasType(Def^.TypePtr);
    Def := Def^.Prev
  end;
  if Result = nil then
  begin
    Result := _NewType(TtcFunction);
    Result^.FnDefPtr := NewFnDef;
    Result^.FnDefPtr^.ReturnTypePtr := ReturnTypePtr;
    Result^.FnDefPtr^.Args := Args
  end
end;

function MakeAliasType(const Name : string; TypePtr : TPsTypePtr) : TPsTypePtr;
begin
  TypePtr := _UnaliasType(TypePtr);
  Result := _NewType(TypePtr^.Cls);
  Result^ := CopyType(TypePtr);
  Result^.Name := Name;
  Result^.AliasFor := TypePtr;
  AddTypeName(Name, Result)
end;

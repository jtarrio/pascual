var 
  GlobalDefinitions : TSScope;
  CurrentScope : TSScope;
  PrimitiveTypes : record
    PtNil, PtBoolean, PtInteger, PtReal, PtChar, PtString, PtText, PtFile,
    PtEmptySet, PtUntypedPtr : TPsTypePtr
  end;

function DefCounter(CounterType : TSCounterType) : integer;
var Ctr : ^integer;
begin
  case CounterType of 
    TctEnum: Ctr := @CurrentScope^.Counters.EnumCtr;
    TctRecord: Ctr := @CurrentScope^.Counters.RecordCtr;
    TctTmpVar: Ctr := @CurrentScope^.Counters.TmpVarCtr;
  end;
  Ctr^ := Ctr^ + 1;
  Result := Ctr^
end;

procedure PushGlobalDefs(Defs : TSScope);
begin
  Defs^.Parent := nil;
  Defs^.LatestDef := nil;
  Defs^.Counters.EnumCtr := 0;
  Defs^.Counters.RecordCtr := 0;
  Defs^.Counters.TmpVarCtr := 0;
  Defs^.CurrentFn := nil;
  CurrentScope := Defs
end;

function NewEnum(const Enum : TPsEnumDef) : TPsEnumPtr;
begin
  new(Result);
  Result^ := Enum;
  Result^.Id := DefCounter(TctEnum);
  Result^.HasBeenDefined := false;
  Result^.ValuesHaveBeenOutput := false
end;

function NewRecord(const Rec : TPsRecordDef) : TPsRecPtr;
begin
  new(Result);
  Result^ := Rec;
  Result^.Id := DefCounter(TctRecord);
  Result^.HasBeenDefined := false
end;

function NewFnDef : TPsFnDefPtr;
begin
  new(Result)
end;

function _HasUnusedPrefix(const Name : string) : boolean;
begin
  Result := (Length(Name) > 7)
            and (Name[1] = 'U') and (Name[2] = 'N') and (Name[3] = 'U')
            and (Name[4] = 'S') and (Name[5] = 'E') and (Name[6] = 'D')
            and (Name[7] = '_')
end;

procedure _CheckUnusedSymbols(Def : TSDefinition);
var Where : string;
begin
  if CurrentScope^.CurrentFn = nil then
    Where := ''
  else if CurrentScope^.CurrentFn^.ReturnTypePtr = nil then
         Where := ' in procedure ' + CurrentScope^.CurrentFn^.Name
  else
    Where := ' in function ' + CurrentScope^.CurrentFn^.Name;

  case Def^.Cls of 
    TdcVariable:
                 if not Def^.VarPtr^.WasUsed
                    and not _HasUnusedPrefix(Def^.VarPtr^.Name) then
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
    TdcSubroutine: if not Def^.SrPtr^.WasUsed then
                 begin
                   if Def^.SrPtr^.ReturnTypePtr = nil then
                     CompileWarning('Procedure ' + Def^.SrPtr^.Name +
                                    ' was not used')
                   else CompileWarning('Function ' + Def^.SrPtr^.Name +
                                       ' was not used')
                 end;
    TdcType: if (Def^.TypePtr^.Name <> '') and not Def^.TypePtr^.WasUsed then
               CompileWarning('Type ' + TypeName(Def^.TypePtr) +
               ' was not used')
  end
end;

function _AddDef(Cls : TSDefClass) : TSDefinition;
begin
  new(Result);
  Result^.Cls := Cls;
  case Cls of 
    TdcName : new(Result^.NamePtr);
    TdcType : new(Result^.TypePtr);
    TdcConstant : new(Result^.ConstPtr);
    TdcVariable : new(Result^.VarPtr);
    TdcSubroutine : new(Result^.SrPtr);
    TdcPseudoFn : new(Result^.PseudoFnPtr);
    TdcWithVar : new(Result^.WithVarPtr);
  end;
  Stack_Push(CurrentScope^.LatestDef, Result)
end;

procedure StartLocalScope(Defs : TSScope; NewFunction : TPsSubrPtr);
begin
  Defs^.Parent := CurrentScope;
  Defs^.LatestDef := nil;
  Defs^.Counters := CurrentScope^.Counters;
  Defs^.CurrentFn := NewFunction;
  CurrentScope := Defs
end;

procedure CloseLocalScope;
var Def : TSDefinition;
begin
  Def := CurrentScope^.LatestDef;
  while Def <> nil do
  begin
    _CheckUnusedSymbols(Def);
    Def := Def^.Older
  end;
  CurrentScope := CurrentScope^.Parent;
  if CurrentScope = nil then InternalError('Closed the global scope')
end;

function _FindDef(var FoundDef : TSDefinition;
                  Predicate : TStackPredicate;
                  var Context {:Any};
                  FromLocalScope : boolean) : boolean;
var 
  DefSet : TSScope;
  Found : boolean;
begin
  DefSet := CurrentScope;
  repeat
    Found := Stack_Find(DefSet^.LatestDef, FoundDef, Predicate, Context);
    if not Found and not FromLocalScope then DefSet := DefSet^.Parent;
  until Found or FromLocalScope or (DefSet = nil);
  Result := Found
end;

function _DefIsName(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  Name : string absolute Ctx;
begin
  Result := (Def^.Cls = TdcName) and (Name = Def^.NamePtr^.Name)
end;

function _FindName(Name : string; Required : boolean;
                   FromLocalScope : boolean) : TPsNamePtr;
var 
  Def : TSDefinition;
begin
  if _FindDef(Def, @_DefIsName, Name, FromLocalScope) then
    Result := Def^.NamePtr
  else if Required then CompileError('Unknown identifier: ' + Name)
  else Result := nil
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

function AddFunctionName(const Name : string; Idx : TPsSubrPtr) : TPsNamePtr;
var 
  Def : TPsNamePtr;
begin
  Def := _AddName(Name, TncFunction);
  Def^.SrPtr := Idx;
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
end;

function GetFundamentalType(TypePtr : TPsTypePtr) : TPsTypePtr;
begin
  if (TypePtr <> nil) and (TypePtr^.Cls = TtcRange) then
    Result := TypePtr^.RangeDef.BaseTypePtr
  else
    Result := TypePtr
end;

function IsUntyped(TypePtr : TPstypePtr) : boolean;
begin
  Result := TypePtr = nil
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

function IsFileType(TypePtr : TPsTypePtr) : boolean;
begin
  Result := _TypeHasClass(TypePtr, TtcFile)
end;

function IsTextType(TypePtr : TPsTypePtr) : boolean;
begin
  IsTextType := IsFileType(TypePtr) and (TypePtr^.FileDef.Cls = TfcText)
end;

function IsGenericFileType(TypePtr : TPsTypePtr) : boolean;
begin
  Result := IsFileType(TypePtr) and (TypePtr^.FileDef.Cls = TfcNone)
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

function IsUntypedPtrType(TypePtr : TPsTypePtr) : boolean;
begin
  Result := IsPointerType(TypePtr) and (TypePtr^.PointedTypePtr = nil)
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

function AreFunctionsCompatible(A, B : TPsTypePtr) : boolean;
begin
  AreFunctionsCompatible := IsFunctionyType(A) and IsFunctionyType(B)
                            and (IsNilType(A) or IsNilType(B)
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
var Pos : integer;
begin
  if UseOriginal and (TypePtr <> nil) then
    while TypePtr^.AliasFor <> nil do
      TypePtr := TypePtr^.AliasFor;
  if TypePtr = nil then Result := 'untyped'
  else if TypePtr^.Name <> '' then Result := TypePtr^.Name
  else if TypePtr^.Cls = TtcFile then
  begin
    if TypePtr^.FileDef.Cls = TfcNone then Result := 'FILE'
    else if TypePtr^.FileDef.Cls = TfcText then Result := 'TEXT'
    else Result := 'FILE OF ' + DeepTypeName(TypePtr^.FileDef.TypePtr, false)
  end
  else if TypePtr^.Cls = TtcEnum then
  begin
    Result := '(';
    for Pos := 0 to TypePtr^.EnumPtr^.Size - 1 do
    begin
      if Pos <> 0 then
        Result := Result + ',';
      Result := Result + TypePtr^.EnumPtr^.Values[Pos]
    end;
    Result := Result + ')'
  end
  else if TypePtr^.Cls = TtcRange then
         Result := _AntiOrdinal(TypePtr^.RangeDef.First,
                   TypePtr^.RangeDef.BaseTypePtr) +
                   '..' + _AntiOrdinal(TypePtr^.RangeDef.Last,
                   TypePtr^.RangeDef.BaseTypePtr)
  else if TypePtr^.Cls = TtcSet then
  begin
    if TypePtr^.ElementTypePtr = nil then Result := 'SET OF []'
    else Result := 'SET OF ' + DeepTypeName(TypePtr^.ElementTypePtr, false)
  end
  else if TypePtr^.Cls = TtcRecord then
  begin
    Result := 'RECORD ';
    for Pos := 1 to TypePtr^.RecPtr^.Size do
    begin
      if Pos <> 1 then Result := Result + '; ';
      Result := Result +
                DeepTypeName(TypePtr^.RecPtr^.Fields[Pos].TypePtr, false) +
                ':' + TypePtr^.RecPtr^.Fields[Pos].Name
    end;
    Result := Result + ' END'
  end
  else if TypePtr^.Cls = TtcArray then
         Result := 'ARRAY [' +
                   DeepTypeName(TypePtr^.ArrayDef.IndexTypePtr, false) +
                   '] OF ' + DeepTypeName(TypePtr^.ArrayDef.ValueTypePtr, false)
  else if TypePtr^.Cls = TtcPointer then
  begin
    if TypePtr^.PointedTypePtr = nil then Result := 'POINTER'
    else Result := '^' + DeepTypeName(TypePtr^.PointedTypePtr, UseOriginal)
  end
  else if TypePtr^.Cls = TtcPointerForward then
         Result := '^' + TypePtr^.TargetName^
  else if TypePtr^.Cls = TtcFunction then
         with TypePtr^.FnDefPtr^ do
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
    Str(TypePtr^.Cls, Result);
    CompileError('Could not get name for type of class ' + Result)
  end
end;

function TypeName(TypePtr : TPsTypePtr) : string;
begin
  Result := DeepTypeName(TypePtr, false)
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

function EmptyFunction : TPsSubroutine;
begin
  Result.Name := '';
  Result.ExternalName := '';
  Result.Args.Count := 0;
  Result.ReturnTypePtr := nil;
  Result.IsDeclaration := false;
  Result.WasUsed := false
end;

function IsSameFunctionDefinition(DeclPtr : TPsSubrPtr;
                                  Fun : TPsSubroutine) : boolean;
var 
  Decl : TPsSubroutine;
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
                           and (NamePtr^.SrPtr^.IsDeclaration)
end;

function AddFunction(const Fun : TPsSubroutine) : TPsSubrPtr;
var 
  NamePtr : TPsNamePtr;
  SrPtr : TPsSubrPtr;
  IsProcedure : boolean;
begin
  IsProcedure := Fun.ReturnTypePtr = nil;
  NamePtr := FindNameInLocalScope(Fun.Name, {Required=}false);
  if NamePtr = nil then
  begin
    SrPtr := _AddDef(TdcSubroutine)^.SrPtr;
    SrPtr^ := Fun;
    AddFunctionName(Fun.Name, SrPtr)
  end
  else
  begin
    if (NamePtr^.Cls <> TncFunction) or Fun.IsDeclaration then
      CompileError('Identifier ' + Fun.Name + ' already defined');
    SrPtr := NamePtr^.SrPtr;
    if SrPtr^.IsDeclaration then
    begin
      if ((Fun.Args.Count = 0) and (Fun.ReturnTypePtr = nil))
         or IsSameFunctionDefinition(SrPtr, Fun) then
        SrPtr^.IsDeclaration := false
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
  AddFunction := SrPtr;
end;

function FindField(TypePtr : TPsTypePtr; const Name : string;
                   Required : boolean) : integer;
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

function _DefIsWithVar(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  Name : string absolute Ctx;
begin
  Result := (Def^.Cls = TdcWithVar)
            and (FindFieldType(Def^.WithVarPtr^.VarPtr^.TypePtr,
            Name, False) <> nil)
end;

function FindWithVar(Name : string) : TPsWithVarPtr;
var Def : TSDefinition;
begin
  if _FindDef(Def, @_DefIsWithVar, Name, {FromLocalScope=}true) then
    Result := Def^.WithVarPtr
  else Result := nil
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
  TmpVar.IsAliasFor := ExCopy(Base);
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
begin
  Result.Name := Name;
  Result.TypePtr := TypePtr;
  Result.IsReference := false;
  Result.IsConstant := true;
  Result.WasInitialized := true;
  Result.WasUsed := false;
  Result.IsAliasFor := nil
end;

function MakeVariable(const Name : string; TypePtr : TPsTypePtr) : TPsVariable;
begin
  Result.Name := Name;
  Result.TypePtr := TypePtr;
  Result.IsReference := false;
  Result.IsConstant := false;
  Result.WasInitialized := false;
  Result.WasUsed := false;
  Result.IsAliasFor := nil
end;

function MakeReference(const Name : string; TypePtr : TPsTypePtr) : TPsVariable;
begin
  Result.Name := Name;
  Result.TypePtr := TypePtr;
  Result.IsReference := true;
  Result.IsConstant := false;
  Result.WasInitialized := true;
  Result.WasUsed := false;
  Result.IsAliasFor := nil
end;

function AddAliasVariable(const Prefix : string;
                          TypePtr : TPsTypePtr;
                          Expr : TExpression) : TPsVarPtr;
begin
  Result := AddVariable(MakeVariable(Prefix + IntToStr(DefCounter(TctTmpVar)),
            TypePtr));
  Result^.IsAliasFor := ExCopy(Expr)
end;

function _MakeArg(const Name : string; TypePtr : TPsTypePtr;
                  IsRef, IsConst : boolean) : TPsVariable;
begin
  Result.Name := Name;
  Result.TypePtr := TypePtr;
  Result.IsReference := IsRef or IsConst;
  Result.IsConstant := IsConst;
  Result.WasInitialized := false;
  Result.WasUsed := false;
  Result.IsAliasFor := nil
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

function MakeProcedure0(const Name : string) : TPsSubroutine;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.Args.Count := 0
end;

function MakeProcedure1(const Name : string; Arg : TPsVariable) : TPsSubroutine;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.Args.Count := 1;
  Result.Args.Defs[1] := Arg
end;

function MakeProcedure2(const Name : string;
                        Arg1, Arg2 : TPsVariable) : TPsSubroutine;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.Args.Count := 2;
  Result.Args.Defs[1] := Arg1;
  Result.Args.Defs[2] := Arg2
end;

function MakeProcedure3(const Name : string;
                        Arg1, Arg2, Arg3 : TPsVariable) : TPsSubroutine;
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
                       RetTypePtr : TPsTypePtr) : TPsSubroutine;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ReturnTypePtr := RetTypePtr
end;

function MakeFunction1(const Name : string; RetTypePtr : TPsTypePtr;
                       Arg : TPsVariable) : TPsSubroutine;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ReturnTypePtr := RetTypePtr;
  Result.Args.Count := 1;
  Result.Args.Defs[1] := Arg
end;

function MakeFunction2(const Name : string; RetTypePtr : TPsTypePtr;
                       Arg1, Arg2 : TPsVariable) : TPsSubroutine;
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
                       Arg1, Arg2, Arg3 : TPsVariable) : TPsSubroutine;
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

function _DefIsFileType(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  Wanted : TPsFileTypeDef absolute Ctx;
begin
  Result := (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcFile)
            and (Def^.TypePtr^.FileDef.Cls = Wanted.Cls)
            and ((Wanted.Cls <> TfcBinary)
            or IsSameType(Def^.TypePtr^.FileDef.TypePtr, Wanted.TypePtr))
end;

function _MakeFileType(Cls : TPsFileClass; TypePtr : TPsTypePtr) : TPsTypePtr;
var 
  FileDef : TPsFileTypeDef;
  Def : TSDefinition;
begin
  { TODO check that the type is appropriate }
  FileDef.Cls := Cls;
  FileDef.TypePtr := TypePtr;
  if _FindDef(Def, @_DefIsFileType, FileDef, {FromLocalScope=}false) then
    Result := _UnaliasType(Def^.TypePtr)
  else
  begin
    Result := _NewType(TtcFile);
    Result^.FileDef := FileDef
  end
end;

function MakeGenericFileType : TPsTypePtr;
begin
  Result := _MakeFileType(TfcNone, nil)
end;

function MakeTextType : TPsTypePtr;
begin
  Result := _MakeFileType(TfcText, nil)
end;

function MakeFileType(TypePtr : TPsTypePtr) : TPsTypePtr;
begin
  Result := _MakeFileType(TfcBinary, TypePtr)
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

function _DefIsArrayType(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  Wanted : TPsArrayTypeDef absolute Ctx;
begin
  Result := (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcArray)
            and IsSameType(Def^.TypePtr^.ArrayDef.IndexTypePtr,
            Wanted.IndexTypePtr)
            and IsSameType(Def^.TypePtr^.ArrayDef.ValueTypePtr,
            Wanted.ValueTypePtr)
end;

function MakeArrayType(IndexType, ValueType : TPsTypePtr) : TPsTypePtr;
var 
  ArrayDef : TPsArrayTypeDef;
  Def : TSDefinition;
begin
  if not IsBoundedType(IndexType) then
    ErrorForType('Array indices must belong to a bounded ordinal type',
                 IndexType);
  ArrayDef.IndexTypePtr := IndexType;
  ArrayDef.ValueTypePtr := ValueType;
  if _FindDef(Def, @_DefIsArrayType, ArrayDef, {FromLocalScope=}false) then
    Result := _UnaliasType(Def^.TypePtr)
  else
  begin
    Result := _NewType(TtcArray);
    Result^.ArrayDef := ArrayDef
  end
end;

function _DefIsRangeType(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  Wanted : TPsRangeTypeDef absolute Ctx;
begin
  Result := (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcRange)
            and IsSameType(Def^.TypePtr^.RangeDef.BaseTypePtr,
            Wanted.BaseTypePtr)
            and (Def^.TypePtr^.RangeDef.First = Wanted.First)
            and (Def^.TypePtr^.RangeDef.Last = Wanted.Last)
end;

function MakeRangeType(TypePtr : TPsTypePtr;
                       First, Last : integer) : TPsTypePtr;
var 
  RangeDef : TPsRangeTypeDef;
  Def : TSDefinition;
begin
  if First > Last then
    CompileError('The bounds of a subrange must be in ascending order');
  RangeDef.BaseTypePtr := GetFundamentalType(TypePtr);
  RangeDef.First := First;
  RangeDef.Last := Last;
  if _FindDef(Def, @_DefIsRangeType, RangeDef, {FromLocalScope=}false) then
    Result := _UnaliasType(Def^.TypePtr)
  else
  begin
    Result := _NewType(TtcRange);
    Result^.RangeDef := RangeDef
  end
end;

function _DefIsPointerType(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  TypePtr : TPsTypePtr absolute Ctx;
begin
  Result := (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcPointer)
            and IsSameType(Def^.TypePtr^.PointedTypePtr, TypePtr)
end;

function MakePointerType(TypePtr : TPsTypePtr) : TPsTypePtr;
var 
  Def : TSDefinition;
begin
  if _FindDef(Def, @_DefIsPointerType, TypePtr, {FromLocalScope=}false) then
    Result := _UnaliasType(Def^.TypePtr)
  else
  begin
    Result := _NewType(TtcPointer);
    Result^.PointedTypePtr := TypePtr
  end
end;

function _DefIsPointerForwardType(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  TargetName : string absolute Ctx;
begin
  Result := (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcPointerForward)
            and (Def^.TypePtr^.TargetName^ = TargetName)
end;

function MakePointerForwardType(TargetName : string) : TPsTypePtr;
var 
  Def : TSDefinition;
begin
  if _FindDef(Def, @_DefIsPointerForwardType,
     TargetName, {FromLocalScope=}false) then
    Result := _UnaliasType(Def^.TypePtr)
  else
  begin
    Result := _NewType(TtcPointerForward);
    new(Result^.TargetName);
    Result^.TargetName^ := TargetName
  end
end;

function _DefIsSetType(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  TypePtr : TPsTypePtr absolute Ctx;
begin
  Result := (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcSet)
            and IsSameType(Def^.TypePtr^.ElementTypePtr, TypePtr)
end;

function MakeSetType(TypePtr : TPsTypePtr) : TPsTypePtr;
var 
  Def : TSDefinition;
begin
  if _FindDef(Def, @_DefIsSetType, TypePtr, {FromLocalScope=}false) then
    Result := _UnaliasType(Def^.TypePtr)
  else
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

function _DefIsFunctionType(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  FnDef : TPsFnDef absolute Ctx;
begin
  Result := (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcFunction)
            and IsSameType(Def^.TypePtr^.FnDefPtr^.ReturnTypePtr,
            FnDef.ReturnTypePtr)
            and AreSameArgs(Def^.TypePtr^.FnDefPtr^.Args, FnDef.Args)
end;

function MakeFunctionType(const Args : TPsFnArgs;
                          ReturnTypePtr : TPsTypePtr) : TPsTypePtr;
var 
  Def : TSDefinition;
  FnDef : TPsFnDef;
begin
  FnDef.ReturnTypePtr := ReturnTypePtr;
  FnDef.Args := Args;
  if _FindDef(Def, @_DefIsFunctionType, FnDef, {FromLocalScope=}false) then
    Result := _UnaliasType(Def^.TypePtr)
  else
  begin
    Result := _NewType(TtcFunction);
    Result^.FnDefPtr := NewFnDef;
    Result^.FnDefPtr^.ReturnTypePtr := FnDef.ReturnTypePtr;
    Result^.FnDefPtr^.Args := FnDef.Args
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

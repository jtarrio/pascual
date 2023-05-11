var 
  GlobalDefinitions : TSScope;
  CurrentScope : TSScope;
  PrimitiveTypes : record
    PtNil, PtBoolean, PtInteger, PtReal, PtChar, PtString, PtText, PtFile,
    PtEmptySet, PtUntypedPtr : TSDType
  end;

function DefCounter(CounterType : TSCounterType) : integer;
var Ctr : ^integer;
begin
  case CounterType of 
    SctEnum: Ctr := @CurrentScope^.Counters.EnumCtr;
    SctRecord: Ctr := @CurrentScope^.Counters.RecordCtr;
    SctTmpVar: Ctr := @CurrentScope^.Counters.TmpVarCtr;
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

function NewEnum(const Enum : TSDTEnumDef) : TSDTEnum;
begin
  new(Result);
  Result^ := Enum;
  Result^.Id := DefCounter(SctEnum);
  Result^.HasBeenDefined := false;
  Result^.ValuesHaveBeenOutput := false
end;

function NewRecord(const Rec : TSDTRecordDef) : TSDTRecord;
begin
  new(Result);
  Result^ := Rec;
  Result^.Id := DefCounter(SctRecord);
  Result^.HasBeenDefined := false
end;

function NewFnDef : TSDTSubroutine;
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
    SdcVariable:
                 if not Def^.VarDef.WasUsed
                    and not _HasUnusedPrefix(Def^.VarDef.Name) then
                 begin
                   if Def^.VarDef.IsConstant then
                     CompileWarning('Constant ' + Def^.VarDef.Name +
                                    ' was not used' + Where)
                   else
                     CompileWarning('Variable ' + Def^.VarDef.Name +
                                    ' was not used' + Where)
                 end
                 else if not Def^.VarDef.WasInitialized then
                        CompileWarning('Variable ' + Def^.VarDef.Name +
                                       ' was not initialized' + Where);
    SdcSubroutine: if not Def^.SrDef.WasUsed then
                   begin
                     if Def^.SrDef.ReturnTypePtr = nil then
                       CompileWarning('Procedure ' + Def^.SrDef.Name +
                                      ' was not used')
                     else CompileWarning('Function ' + Def^.SrDef.Name +
                                         ' was not used')
                   end;
    SdcType: if (Def^.TypeDef.Name <> '') and not Def^.TypeDef.WasUsed then
               CompileWarning('Type ' + TypeName(@Def^.TypeDef) +
               ' was not used')
  end
end;

function _AddDef(Cls : TSDefClass) : TSDefinition;
begin
  new(Result);
  Result^.Cls := Cls;
  Stack_Push(CurrentScope^.LatestDef, Result)
end;

procedure StartLocalScope(Defs : TSScope; NewFunction : TSDSubroutine);
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
  Result := (Def^.Cls = SdcName) and (Name = Def^.NameDef.Name)
end;

function _FindName(Name : string; Required : boolean;
                   FromLocalScope : boolean) : TSDName;
var 
  Def : TSDefinition;
begin
  if _FindDef(Def, @_DefIsName, Name, FromLocalScope) then
    Result := @Def^.NameDef
  else if Required then CompileError('Unknown identifier: ' + Name)
  else Result := nil
end;

function _CheckNameClass(NamePtr : TSDName; Cls : TSDNameClass) : TSDName;
begin
  if (NamePtr <> nil) and (NamePtr^.Cls <> Cls) then
    case Cls of 
      SdncType : CompileError('Not a type: ' + NamePtr^.Name);
      SdncVariable : CompileError('Not a variable: ' + NamePtr^.Name);
      SdncEnumVal : CompileError('Not an enumeration value: ' + NamePtr^.Name);
      SdncSubroutine : CompileError('Not a procedure or function: ' +
                                    NamePtr^.Name);
      SdncPsfn : CompileError('Not a procedure or function: ' +
                              NamePtr^.Name);
      else InternalError('Name class mismatch for ' + NamePtr^.Name)
    end;
  _CheckNameClass := NamePtr
end;

function FindNameInLocalScope(const Name : string;
                              Required : boolean) : TSDName;
begin
  FindNameInLocalScope := _FindName(Name, Required, {FromLocalScope=}true)
end;

function FindNameOfClassInLocalScope(const Name : string; Cls : TSDNameClass;
                                     Required : boolean) : TSDName;
begin
  FindNameOfClassInLocalScope := _CheckNameClass(
                                 FindNameInLocalScope(Name, Required), Cls)
end;

function FindName(const Name : string; Required : boolean) : TSDName;
begin
  FindName := _FindName(Name, Required, {FromLocalScope=}false)
end;

function FindNameOfClass(const Name : string; Cls : TSDNameClass;
                         Required : boolean)
: TSDName;
begin
  FindNameOfClass := _CheckNameClass(FindName(Name, Required), Cls)
end;

function _AddName(const Name : string; Cls : TSDNameClass) : TSDName;
var Def : TSDefinition;
begin
  if FindNameInLocalScope(Name, {Required=}false) <> nil then
    CompileError('Identifier ' + Name + ' already defined');
  Def := _AddDef(SdcName);
  Result := @Def^.NameDef;
  Result^.Name := Name;
  Result^.Cls := Cls
end;

function AddTypeName(const Name : string; Idx : TSDType) : TSDName;
begin
  Result := _AddName(Name, SdncType);
  Result^.TypePtr := Idx
end;

function AddVariableName(const Name : string; Idx : TSDVariable) : TSDName;
var 
  Def : TSDName;
begin
  Def := _AddName(Name, SdncVariable);
  Def^.VarPtr := Idx;
  AddVariableName := Def
end;

function AddConstantName(const Name : string; Idx : TSDConstant) : TSDName;
var 
  Def : TSDName;
begin
  Def := _AddName(Name, SdncConstant);
  Def^.ConstPtr := Idx;
  Result := Def
end;

function AddFunctionName(const Name : string; Idx : TSDSubroutine) : TSDName;
var 
  Def : TSDName;
begin
  Def := _AddName(Name, SdncSubroutine);
  Def^.SrPtr := Idx;
  AddFunctionName := Def
end;

function AddEnumValName(Ordinal : integer;
                        TypeIdx : TSDType) : TSDName;
var Def : TSDName;
begin
  Def := _AddName(TypeIdx^.EnumPtr^.Values[Ordinal], SdncEnumVal);
  Def^.EnumTypePtr := TypeIdx;
  Def^.Ordinal := Ordinal;
  AddEnumValName := Def
end;

function AddPsfn(const Name : string;
                 Parse : TSDPsfnParser) : TSDPsfn;
var 
  NameDef : TSDName;
  Def : TSDefinition;
begin
  Def := _AddDef(SdcPsfn);
  Def^.PsfnDef.Name := Name;
  Def^.PsfnDef.ParseFn := Parse;
  NameDef := _AddName(Name, SdncPsfn);
  NameDef^.PsfnPtr := @Def^.PsfnDef;
  Result := NameDef^.PsfnPtr
end;

function CopyType(TypePtr : TSDType) : TSDTypeDef;
begin
  Result := TypePtr^;
  if Result.Cls = SdtcPointerForward then
  begin
    new(Result.TargetName);
    Result.TargetName^ := TypePtr^.TargetName^
  end
end;

function GetFundamentalType(TypePtr : TSDType) : TSDType;
begin
  if (TypePtr <> nil) and (TypePtr^.Cls = SdtcRange) then
    Result := TypePtr^.RangeDef.BaseTypePtr
  else
    Result := TypePtr
end;

function IsUntyped(TypePtr : TSDType) : boolean;
begin
  Result := TypePtr = nil
end;

function _TypeHasClass(TypePtr : TSDType; Cls : TSDTypeClass) : boolean;
begin
  Result := (TypePtr <> nil) and (GetFundamentalType(TypePtr)^.Cls = Cls)
end;

function IsIntegerType(TypePtr : TSDType) : boolean;
begin
  IsIntegerType := _TypeHasClass(TypePtr, SdtcInteger)
end;

function IsRealType(TypePtr : TSDType) : boolean;
begin
  Result := _TypeHasClass(TypePtr, SdtcReal)
end;

function IsNumericType(TypePtr : TSDType) : boolean;
begin
  Result := IsIntegerType(TypePtr) or IsRealType(TypePtr)
end;

function IsStringType(TypePtr : TSDType) : boolean;
begin
  IsStringType := _TypeHasClass(TypePtr, SdtcString)
end;

function IsCharType(TypePtr : TSDType) : boolean;
begin
  IsCharType := _TypeHasClass(TypePtr, SdtcChar)
end;

function IsStringyType(TypePtr : TSDType) : boolean;
begin
  IsStringyType := IsStringType(TypePtr) or IsCharType(TypePtr)
end;

function IsBooleanType(TypePtr : TSDType) : boolean;
begin
  IsBooleanType := _TypeHasClass(TypePtr, SdtcBoolean)
end;

function IsFileType(TypePtr : TSDType) : boolean;
begin
  Result := _TypeHasClass(TypePtr, SdtcFile)
end;

function IsTextType(TypePtr : TSDType) : boolean;
begin
  IsTextType := IsFileType(TypePtr) and (TypePtr^.FileDef.Cls = SdtfcText)
end;

function IsGenericFileType(TypePtr : TSDType) : boolean;
begin
  Result := IsFileType(TypePtr) and (TypePtr^.FileDef.Cls = SdtfcNone)
end;

function IsEnumType(TypePtr : TSDType) : boolean;
begin
  IsEnumType := _TypeHasClass(TypePtr, SdtcEnum)
end;

function IsRangeType(TypePtr : TSDType) : boolean;
begin
  Result := (TypePtr <> nil) and (TypePtr^.Cls = SdtcRange)
end;

function IsSetType(TypePtr : TSDType) : boolean;
begin
  Result := _TypeHasClass(TypePtr, SdtcSet)
end;

function IsRecordType(TypePtr : TSDType) : boolean;
begin
  IsRecordType := _TypeHasClass(TypePtr, SdtcRecord)
end;

function IsArrayType(TypePtr : TSDType) : boolean;
begin
  IsArrayType := _TypeHasClass(TypePtr, SdtcArray)
end;

function IsPointerType(TypePtr : TSDType) : boolean;
begin
  IsPointerType := _TypeHasClass(TypePtr, SdtcPointer)
end;

function IsNilType(TypePtr : TSDType) : boolean;
begin
  IsNilType := _TypeHasClass(TypePtr, SdtcNil)
end;

function IsPointeryType(TypePtr : TSDType) : boolean;
begin
  IsPointeryType := IsPointerType(TypePtr) or IsNilType(TypePtr)
end;

function IsPointerForwardType(TypePtr : TSDType) : boolean;
begin
  IsPointerForwardType := _TypeHasClass(TypePtr, SdtcPointerForward)
end;

function IsUntypedPtrType(TypePtr : TSDType) : boolean;
begin
  Result := IsPointerType(TypePtr) and (TypePtr^.PointedTypePtr = nil)
end;

function IsFunctionType(TypePtr : TSDType) : boolean;
begin
  IsFunctionType := _TypeHasClass(TypePtr, SdtcFunction)
end;

function IsFunctionyType(TypePtr : TSDType) : boolean;
begin
  Result := IsFunctionType(TypePtr) or IsNilType(TypePtr)
end;

function IsOrdinalType(TypePtr : TSDType) : boolean;
begin
  IsOrdinalType := IsBooleanType(TypePtr)
                   or IsIntegerType(TypePtr)
                   or IsCharType(TypePtr)
                   or IsEnumType(TypePtr)
                   or IsRangeType(TypePtr)
end;

function IsBoundedType(TypePtr : TSDType) : boolean;
begin
  Result := IsBooleanType(TypePtr)
            or IsCharType(TypePtr)
            or IsEnumType(TypePtr)
            or IsRangeType(TypePtr)
end;

function GetTypeLowBound(TypePtr : TSDType) : integer;
begin
  case TypePtr^.Cls of 
    SdtcBoolean : Result := 0;
    SdtcChar : Result := 0;
    SdtcEnum : Result := 0;
    SdtcRange : Result := TypePtr^.RangeDef.First;
    else ErrorForType('Expected bounded type', TypePtr)
  end
end;

function GetTypeHighBound(TypePtr : TSDType) : integer;
begin
  case TypePtr^.Cls of 
    SdtcBoolean : Result := 1;
    SdtcChar : Result := 255;
    SdtcEnum : Result := TypePtr^.EnumPtr^.Size - 1;
    SdtcRange : Result := TypePtr^.RangeDef.Last;
    else ErrorForType('Expected bounded type', TypePtr)
  end
end;

function GetBoundedTypeSize(TypePtr : TSDType) : integer;
begin
  Result := GetTypeHighBound(TypePtr) - GetTypeLowBound(TypePtr) + 1
end;

function IsSameType(A, B : TSDType) : boolean;
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

function IsFundamentallySameType(A, B : TSDType) : boolean;
begin
  Result := IsSameType(GetFundamentalType(A), GetFundamentalType(B))
end;

function ArePointersCompatible(A, B : TSDType) : boolean;
begin
  ArePointersCompatible := IsPointeryType(A) and IsPointeryType(B) and
                           (IsNilType(A) or IsNilType(B)
                           or IsSameType(A, B))
end;

function AreFunctionsCompatible(A, B : TSDType) : boolean;
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

function _AntiOrdinal(Ordinal : integer; TypePtr : TSDType) : string;
begin
  Result := ExDescribe(ExGetAntiOrdinal(Ordinal, TypePtr));
end;

function DeepTypeName(TypePtr : TSDType; UseOriginal : boolean) : string;
var Pos : integer;
begin
  if UseOriginal and (TypePtr <> nil) then
    while TypePtr^.AliasFor <> nil do
      TypePtr := TypePtr^.AliasFor;
  if TypePtr = nil then Result := 'untyped'
  else if TypePtr^.Name <> '' then Result := TypePtr^.Name
  else if TypePtr^.Cls = SdtcFile then
  begin
    if TypePtr^.FileDef.Cls = SdtfcNone then Result := 'FILE'
    else if TypePtr^.FileDef.Cls = SdtfcText then Result := 'TEXT'
    else Result := 'FILE OF ' + DeepTypeName(TypePtr^.FileDef.TypePtr, false)
  end
  else if TypePtr^.Cls = SdtcEnum then
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
  else if TypePtr^.Cls = SdtcRange then
         Result := _AntiOrdinal(TypePtr^.RangeDef.First,
                   TypePtr^.RangeDef.BaseTypePtr) +
                   '..' + _AntiOrdinal(TypePtr^.RangeDef.Last,
                   TypePtr^.RangeDef.BaseTypePtr)
  else if TypePtr^.Cls = SdtcSet then
  begin
    if TypePtr^.ElementTypePtr = nil then Result := 'SET OF []'
    else Result := 'SET OF ' + DeepTypeName(TypePtr^.ElementTypePtr, false)
  end
  else if TypePtr^.Cls = SdtcRecord then
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
  else if TypePtr^.Cls = SdtcArray then
         Result := 'ARRAY [' +
                   DeepTypeName(TypePtr^.ArrayDef.IndexTypePtr, false) +
                   '] OF ' + DeepTypeName(TypePtr^.ArrayDef.ValueTypePtr, false)
  else if TypePtr^.Cls = SdtcPointer then
  begin
    if TypePtr^.PointedTypePtr = nil then Result := 'POINTER'
    else Result := '^' + DeepTypeName(TypePtr^.PointedTypePtr, UseOriginal)
  end
  else if TypePtr^.Cls = SdtcPointerForward then
         Result := '^' + TypePtr^.TargetName^
  else if TypePtr^.Cls = SdtcFunction then
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

function TypeName(TypePtr : TSDType) : string;
begin
  Result := DeepTypeName(TypePtr, false)
end;

function AddConstant(const Constant : TSDConstantDef) : TSDConstant;
var Def : TSDefinition;
begin
  if FindNameInLocalScope(Constant.Name, {Required=}false) <> nil then
    CompileError('Identifier ' + Constant.Name + ' already defined');
  Def := _AddDef(SdcConstant);
  Result := @Def^.ConstDef;
  AddConstantName(Constant.Name, Result);
  Result^ := Constant
end;

function AddVariable(const VarDef : TSDVariableDef) : TSDVariable;
var Def : TSDefinition;
begin
  if FindNameInLocalScope(VarDef.Name, {Required=}false) <> nil then
    CompileError('Identifier ' + VarDef.Name + ' already defined');
  Def := _AddDef(SdcVariable);
  Result := @Def^.VarDef;
  AddVariableName(VarDef.Name, Result);
  Result^ := VarDef
end;

function EmptyFunction : TSDSubroutineDef;
begin
  Result.Name := '';
  Result.ExternalName := '';
  Result.Args.Count := 0;
  Result.ReturnTypePtr := nil;
  Result.IsDeclaration := false;
  Result.HadDeclaration := false;
  Result.WasUsed := false
end;

function IsSameFunctionDefinition(DeclPtr : TSDSubroutine;
                                  Fun : TSDSubroutineDef) : boolean;
var 
  Decl : TSDSubroutineDef;
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
  NamePtr : TSDName;
begin
  NamePtr := FindNameOfClassInLocalScope(Name,
             SdncSubroutine, {Required=}false);
  HasForwardDeclaration := (NamePtr <> nil)
                           and (NamePtr^.SrPtr^.IsDeclaration)
end;

function AddFunction(const Fun : TSDSubroutineDef) : TSDSubroutine;
var 
  Def : TSDefinition;
  NamePtr : TSDName;
  IsProcedure : boolean;
begin
  IsProcedure := Fun.ReturnTypePtr = nil;
  NamePtr := FindNameInLocalScope(Fun.Name, {Required=}false);
  if NamePtr = nil then
  begin
    Def := _AddDef(SdcSubroutine);
    Result := @Def^.SrDef;
    Result^ := Fun;
    AddFunctionName(Fun.Name, Result)
  end
  else
  begin
    if (NamePtr^.Cls <> SdncSubroutine) or Fun.IsDeclaration then
      CompileError('Identifier ' + Fun.Name + ' already defined');
    Result := NamePtr^.SrPtr;
    if Result^.IsDeclaration then
    begin
      if ((Fun.Args.Count = 0) and (Fun.ReturnTypePtr = nil))
         or IsSameFunctionDefinition(Result, Fun) then
      begin
        Result^.IsDeclaration := false;
        Result^.HadDeclaration := true
      end
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
  end
end;

function FindField(TypePtr : TSDType; const Name : string;
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

function FindFieldType(TypePtr : TSDType; const Name : string;
                       Required : boolean) : TSDType;
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
  Result := (Def^.Cls = SdcWithVar)
            and Def^.WithVarDef.IsActive
            and (FindFieldType(Def^.WithVarDef.VarPtr^.TypePtr,
            Name, False) <> nil)
end;

function FindWithVar(Name : string) : TSDWithVar;
var Def : TSDefinition;
begin
  if _FindDef(Def, @_DefIsWithVar, Name, {FromLocalScope=}true) then
    Result := @Def^.WithVarDef
  else Result := nil
end;

function AddWithVar(Base : TSExpression) : TSDWithVar;
var 
  Def : TSDefinition;
  TmpVar : TSDVariableDef;
  TmpVarPtr : TSDVariable;
begin
  EnsureRecordExpr(Base);

  TmpVar.Name := 'with' + IntToStr(DefCounter(SctTmpVar));
  TmpVar.TypePtr := Base^.TypePtr;
  TmpVar.IsConstant := false;
  TmpVar.ConstantValue := nil;
  TmpVar.IsReference := Base^.IsAddressable;
  TmpVar.Location := nil;
  TmpVar.IsArgument := false;
  TmpVar.WasInitialized := true;
  TmpVar.WasUsed := true;
  TmpVar.IsAliasFor := ExCopy(Base);
  TmpVarPtr := AddVariable(TmpVar);
  Def := _AddDef(SdcWithVar);
  Result := @Def^.WithVarDef;
  Result^.VarPtr := TmpVarPtr;
  Result^.IsActive := true
end;

function MakeConstant(const Name : string; Value : TSExpression)
: TSDConstantDef;
var 
  Constant : TSDConstantDef;
begin
  Constant.Name := Name;
  Constant.Value := Value;
  MakeConstant := Constant
end;

function MakeTypedConstant(const Name : string; TypePtr : TSDType;
                           Value : TSExpression) : TSDVariableDef;
begin
  Result.Name := Name;
  Result.TypePtr := TypePtr;
  Result.IsReference := false;
  Result.Location := nil;
  Result.IsConstant := true;
  Result.ConstantValue := Value;
  Result.IsArgument := false;
  Result.WasInitialized := true;
  Result.WasUsed := false;
  Result.IsAliasFor := nil
end;

function MakeVariable(const Name : string; TypePtr : TSDType) : TSDVariableDef;
begin
  Result.Name := Name;
  Result.TypePtr := TypePtr;
  Result.IsReference := false;
  Result.Location := nil;
  Result.IsConstant := false;
  Result.ConstantValue := nil;
  Result.IsArgument := false;
  Result.WasInitialized := false;
  Result.WasUsed := false;
  Result.IsAliasFor := nil
end;

function MakeAbsolute(const Name : string; TypePtr : TSDType;
                      Location : TSExpression) : TSDVariableDef;
begin
  Result.Name := Name;
  Result.TypePtr := TypePtr;
  Result.IsReference := true;
  Result.Location := Location;
  Result.IsConstant := false;
  Result.ConstantValue := nil;
  Result.IsArgument := false;
  Result.WasInitialized := true;
  Result.WasUsed := false;
  Result.IsAliasFor := nil
end;

function MakeFromArg(const Arg : TSDSubroutineArg) : TSDVariableDef;
begin
  Result.Name := Arg.Name;
  Result.TypePtr := Arg.TypePtr;
  Result.IsReference := Arg.IsReference;
  Result.Location := nil;
  Result.IsConstant := Arg.IsConstant;
  Result.ConstantValue := nil;
  Result.IsArgument := true;
  Result.WasInitialized := true;
  Result.WasUsed := false;
  Result.IsAliasFor := nil;
end;

function AddAliasVariable(const Prefix : string;
                          TypePtr : TSDType;
                          Expr : TSExpression) : TSDVariable;
begin
  Result := AddVariable(MakeVariable(Prefix + IntToStr(DefCounter(SctTmpVar)),
            TypePtr));
  Result^.IsAliasFor := ExCopy(Expr)
end;

function _MakeArg(const Name : string; TypePtr : TSDType;
                  IsRef, IsConst : boolean) : TSDSubroutineArg;
begin
  Result.Name := Name;
  Result.TypePtr := TypePtr;
  Result.IsReference := IsRef or IsConst;
  Result.IsConstant := IsConst
end;

function MakeArg(const Name : string; TypePtr : TSDType) : TSDSubroutineArg;
begin
  MakeArg := _MakeArg(Name, TypePtr, false, false)
end;

function MakeVarArg(const Name : string; TypePtr : TSDType) : TSDSubroutineArg;
begin
  MakeVarArg := _MakeArg(Name, TypePtr, true, false)
end;

function MakeConstArg(const Name : string;
                      TypePtr : TSDType) : TSDSubroutineArg;
begin
  MakeConstArg := _MakeArg(Name, TypePtr, false, true)
end;

function MakeProcedure0(const Name : string) : TSDSubroutineDef;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.Args.Count := 0
end;

function MakeProcedure1(const Name : string;
                        Arg : TSDSubroutineArg) : TSDSubroutineDef;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.Args.Count := 1;
  Result.Args.Defs[1] := Arg
end;

function MakeProcedure2(const Name : string;
                        Arg1, Arg2 : TSDSubroutineArg) : TSDSubroutineDef;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.Args.Count := 2;
  Result.Args.Defs[1] := Arg1;
  Result.Args.Defs[2] := Arg2
end;

function MakeProcedure3(const Name : string;
                        Arg1, Arg2, Arg3 : TSDSubroutineArg) : TSDSubroutineDef;
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
                       RetTypePtr : TSDType) : TSDSubroutineDef;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ReturnTypePtr := RetTypePtr
end;

function MakeFunction1(const Name : string; RetTypePtr : TSDType;
                       Arg : TSDSubroutineArg) : TSDSubroutineDef;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ReturnTypePtr := RetTypePtr;
  Result.Args.Count := 1;
  Result.Args.Defs[1] := Arg
end;

function MakeFunction2(const Name : string; RetTypePtr : TSDType;
                       Arg1, Arg2 : TSDSubroutineArg) : TSDSubroutineDef;
begin
  Result := EmptyFunction;
  Result.Name := Name;
  Result.ExternalName := Name;
  Result.ReturnTypePtr := RetTypePtr;
  Result.Args.Count := 2;
  Result.Args.Defs[1] := Arg1;
  Result.Args.Defs[2] := Arg2
end;

function MakeFunction3(const Name : string; RetTypePtr : TSDType;
                       Arg1, Arg2, Arg3 : TSDSubroutineArg) : TSDSubroutineDef;
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

function _UnaliasType(TypePtr : TSDType) : TSDType;
begin
  Result := TypePtr;
  while Result^.AliasFor <> nil do
    Result := Result^.AliasFor
end;

function _NewType(Cls : TSDTypeClass) : TSDType;
var Def : TSDefinition;
begin
  Def := _AddDef(SdcType);
  Result := @Def^.TypeDef;
  Result^.Name := '';
  Result^.Cls := Cls;
  Result^.AliasFor := nil;
  Result^.WasUsed := false
end;

function MakeBaseType(const Name : String; Cls : TSDTypeClass) : TSDType;
begin
  Result := _NewType(Cls);
  Result^.Name := Name;
  AddTypeName(Name, Result)
end;

function _DefIsFileType(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  Wanted : TSDTFile absolute Ctx;
begin
  Result := (Def^.Cls = SdcType) and (Def^.TypeDef.Cls = SdtcFile)
            and (Def^.TypeDef.FileDef.Cls = Wanted.Cls)
            and ((Wanted.Cls <> SdtfcBinary)
            or IsSameType(Def^.TypeDef.FileDef.TypePtr, Wanted.TypePtr))
end;

function _MakeFileType(Cls : TSDTFileClass; TypePtr : TSDType) : TSDType;
var 
  FileDef : TSDTFile;
  Def : TSDefinition;
begin
  { TODO check that the type is appropriate }
  FileDef.Cls := Cls;
  FileDef.TypePtr := TypePtr;
  if _FindDef(Def, @_DefIsFileType, FileDef, {FromLocalScope=}false) then
    Result := _UnaliasType(@Def^.TypeDef)
  else
  begin
    Result := _NewType(SdtcFile);
    Result^.FileDef := FileDef
  end
end;

function MakeGenericFileType : TSDType;
begin
  Result := _MakeFileType(SdtfcNone, nil)
end;

function MakeTextType : TSDType;
begin
  Result := _MakeFileType(SdtfcText, nil)
end;

function MakeFileType(TypePtr : TSDType) : TSDType;
begin
  Result := _MakeFileType(SdtfcBinary, TypePtr)
end;

function MakeEnumType(const Enum : TSDTEnumDef) : TSDType;
var Pos : integer;
begin
  Result := _NewType(SdtcEnum);
  Result^.EnumPtr := NewEnum(Enum);
  { It's hard to detect when an enumerated type was used }
  Result^.WasUsed := true;
  for Pos := 0 to Result^.EnumPtr^.Size - 1 do
    AddEnumValName(Pos, Result)
end;

function MakeRecordType(const Rec : TSDTRecordDef) : TSDType;
begin
  Result := _NewType(SdtcRecord);
  Result^.RecPtr := NewRecord(Rec)
end;

function _DefIsArrayType(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  Wanted : TSDTArray absolute Ctx;
begin
  Result := (Def^.Cls = SdcType) and (Def^.TypeDef.Cls = SdtcArray)
            and IsSameType(Def^.TypeDef.ArrayDef.IndexTypePtr,
            Wanted.IndexTypePtr)
            and IsSameType(Def^.TypeDef.ArrayDef.ValueTypePtr,
            Wanted.ValueTypePtr)
end;

function MakeArrayType(IndexType, ValueType : TSDType) : TSDType;
var 
  ArrayDef : TSDTArray;
  Def : TSDefinition;
begin
  if not IsBoundedType(IndexType) then
    ErrorForType('Array indices must belong to a bounded ordinal type',
                 IndexType);
  ArrayDef.IndexTypePtr := IndexType;
  ArrayDef.ValueTypePtr := ValueType;
  if _FindDef(Def, @_DefIsArrayType, ArrayDef, {FromLocalScope=}false) then
    Result := _UnaliasType(@Def^.TypeDef)
  else
  begin
    Result := _NewType(SdtcArray);
    Result^.ArrayDef := ArrayDef
  end
end;

function _DefIsRangeType(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  Wanted : TSDTRange absolute Ctx;
begin
  Result := (Def^.Cls = SdcType) and (Def^.TypeDef.Cls = SdtcRange)
            and IsSameType(Def^.TypeDef.RangeDef.BaseTypePtr,
            Wanted.BaseTypePtr)
            and (Def^.TypeDef.RangeDef.First = Wanted.First)
            and (Def^.TypeDef.RangeDef.Last = Wanted.Last)
end;

function MakeRangeType(TypePtr : TSDType;
                       First, Last : integer) : TSDType;
var 
  RangeDef : TSDTRange;
  Def : TSDefinition;
begin
  if First > Last then
    CompileError('The bounds of a subrange must be in ascending order');
  RangeDef.BaseTypePtr := GetFundamentalType(TypePtr);
  RangeDef.First := First;
  RangeDef.Last := Last;
  if _FindDef(Def, @_DefIsRangeType, RangeDef, {FromLocalScope=}false) then
    Result := _UnaliasType(@Def^.TypeDef)
  else
  begin
    Result := _NewType(SdtcRange);
    Result^.RangeDef := RangeDef
  end
end;

function _DefIsPointerType(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  TypePtr : TSDType absolute Ctx;
begin
  Result := (Def^.Cls = SdcType) and (Def^.TypeDef.Cls = SdtcPointer)
            and IsSameType(Def^.TypeDef.PointedTypePtr, TypePtr)
end;

function MakePointerType(TypePtr : TSDType) : TSDType;
var 
  Def : TSDefinition;
begin
  if _FindDef(Def, @_DefIsPointerType, TypePtr, {FromLocalScope=}false) then
    Result := _UnaliasType(@Def^.TypeDef)
  else
  begin
    Result := _NewType(SdtcPointer);
    Result^.PointedTypePtr := TypePtr
  end
end;

function _DefIsPointerForwardType(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  TargetName : string absolute Ctx;
begin
  Result := (Def^.Cls = SdcType) and (Def^.TypeDef.Cls = SdtcPointerForward)
            and (Def^.TypeDef.TargetName^ = TargetName)
end;

function MakePointerForwardType(TargetName : string) : TSDType;
var 
  Def : TSDefinition;
begin
  if _FindDef(Def, @_DefIsPointerForwardType,
     TargetName, {FromLocalScope=}false) then
    Result := _UnaliasType(@Def^.TypeDef)
  else
  begin
    Result := _NewType(SdtcPointerForward);
    new(Result^.TargetName);
    Result^.TargetName^ := TargetName
  end
end;

function _DefIsSetType(var Item; var Ctx) : boolean;
var 
  Def : TSDefinition absolute Item;
  TypePtr : TSDType absolute Ctx;
begin
  Result := (Def^.Cls = SdcType) and (Def^.TypeDef.Cls = SdtcSet)
            and IsSameType(Def^.TypeDef.ElementTypePtr, TypePtr)
end;

function MakeSetType(TypePtr : TSDType) : TSDType;
var 
  Def : TSDefinition;
begin
  if _FindDef(Def, @_DefIsSetType, TypePtr, {FromLocalScope=}false) then
    Result := _UnaliasType(@Def^.TypeDef)
  else
  begin
    Result := _NewType(SdtcSet);
    Result^.ElementTypePtr := TypePtr
  end
end;

function AreSameArgs(const A, B : TSDSubroutineArgs) : boolean;
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
  FnDef : TSDTSubroutineDef absolute Ctx;
begin
  Result := (Def^.Cls = SdcType) and (Def^.TypeDef.Cls = SdtcFunction)
            and IsSameType(Def^.TypeDef.FnDefPtr^.ReturnTypePtr,
            FnDef.ReturnTypePtr)
            and AreSameArgs(Def^.TypeDef.FnDefPtr^.Args, FnDef.Args)
end;

function MakeFunctionType(const Args : TSDSubroutineArgs;
                          ReturnTypePtr : TSDType) : TSDType;
var 
  Def : TSDefinition;
  FnDef : TSDTSubroutineDef;
begin
  FnDef.ReturnTypePtr := ReturnTypePtr;
  FnDef.Args := Args;
  if _FindDef(Def, @_DefIsFunctionType, FnDef, {FromLocalScope=}false) then
    Result := _UnaliasType(@Def^.TypeDef)
  else
  begin
    Result := _NewType(SdtcFunction);
    Result^.FnDefPtr := NewFnDef;
    Result^.FnDefPtr^.ReturnTypePtr := FnDef.ReturnTypePtr;
    Result^.FnDefPtr^.Args := FnDef.Args
  end
end;

function MakeAliasType(const Name : string; TypePtr : TSDType) : TSDType;
begin
  TypePtr := _UnaliasType(TypePtr);
  Result := _NewType(TypePtr^.Cls);
  Result^ := CopyType(TypePtr);
  Result^.Name := Name;
  Result^.AliasFor := TypePtr;
  AddTypeName(Name, Result)
end;

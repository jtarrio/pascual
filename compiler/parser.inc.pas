type 
  TPsIdentifier = record
    Name : string;
  end;

procedure ReadToken;
forward;

function PsTypeDenoter : TSDType;
forward;

function PsExpression : TSExpression;
forward;

function PsVariable : TSExpression;
forward;

function PsStatement : TSStatement;
forward;

procedure PsDefinitions;
forward;

procedure WantToken(Id : TLxTokenId);
begin
  if Lexer.Token.Id <> Id then
    CompileError('Wanted token ' + LxTokenName(Id) + ', found ' + LxTokenStr)
end;

procedure WantToken2(Id1, Id2 : TLxTokenId);
begin
  if not (Lexer.Token.Id in [Id1, Id2]) then
    CompileError('Wanted token ' + LxTokenName(Id1) + ' or ' +
    LxTokenName(Id2) + ', found ' + LxTokenStr)
end;

procedure WantToken3(Id1, Id2, Id3 : TLxTokenId);
begin
  if not (Lexer.Token.Id in [Id1, Id2, Id3]) then
    CompileError('Wanted token ' + LxTokenName(Id1) + ', ' + LxTokenName(Id2) +
    ', or ' + LxTokenName(Id3) + ', found ' + LxTokenStr)
end;

procedure WantToken4(Id1, Id2, Id3, Id4 : TLxTokenId);
begin
  if not (Lexer.Token.Id in [Id1, Id2, Id3, Id4]) then
    CompileError('Wanted token ' + LxTokenName(Id1) + ', ' + LxTokenName(Id2) +
    ', ' + LxTokenName(Id3) +', or ' + LxTokenName(Id4) + ', found ' +
    LxTokenStr)
end;

procedure WantTokenAndRead(Id : TLxTokenId);
begin
  WantToken(Id);
  ReadToken
end;

function GetTokenValueAndRead(Id : TLxTokenId) : string;
begin
  WantToken(Id);
  GetTokenValueAndRead := Lexer.Token.Value;
  ReadToken
end;

procedure SkipToken(Id : TLxTokenId);
begin
  if Lexer.Token.Id = Id then ReadToken
end;

function PsTypeIdentifier : TSDType;
begin
  WantToken(TkIdentifier);
  Result := FindNameOfClass(Lexer.Token.Value,
            SdncType, {Required=}true)^.TypePtr;
  if Result <> nil then Result^.WasUsed := true;
  ReadToken
end;

function PsIdentifier : TPsIdentifier;
begin
  Result.Name := GetTokenValueAndRead(TkIdentifier);
end;

function PsImmediate : TSExpression;
var Expr : TSExpression;
begin
  Expr := PsExpression;
  if not ExIsImmediate(Expr) then
    ErrorForExpr('Expected an immediate expression', Expr);
  PsImmediate := Expr
end;

function PsEnumeratedType : TSDType;
var 
  Enum : TSDTEnumDef;
  Pos : integer;
begin
  WantTokenAndRead(TkLparen);
  Enum.Size := 0;
  repeat
    Enum.Size := Enum.Size + 1;
    if Enum.Size > MaxEnumVals then
      CompileError('Too many values in enum');
    Enum.Values[Enum.Size - 1] := GetTokenValueAndRead(TkIdentifier);
    WantToken2(TkComma, TkRparen);
    SkipToken(TkComma);
  until Lexer.Token.Id = TkRparen;
  WantTokenAndRead(TkRparen);
  Result := MakeEnumType(Enum);
  for Pos := 0 to Result^.EnumPtr^.Size - 1 do
    AddConstant(MakeConstant(Result^.EnumPtr^.Values[Pos],
                ExEnumConstant(Pos, Result)))

end;

procedure PsRecordField(var Rec : TSDTRecordDef; Delimiter : TLxTokenId);
var 
  Name : string;
  LastField, Field : integer;
  TypePtr : TSDType;
begin
  LastField := Rec.Size;
  repeat
    Name := GetTokenValueAndRead(TkIdentifier);
    for Field := 1 to Rec.Size do
    begin
      if Rec.Fields[Field].Name = Name then
        CompileError('A field named ' + Name + ' has already been defined')
    end;
    Rec.Size := Rec.Size + 1;
    if Rec.Size > MaxRecordFields then
      CompileError('Too many fields in record');
    Rec.Fields[Rec.Size].Name := Name;
    WantToken2(TkComma, TkColon);
    SkipToken(TkComma)
  until Lexer.Token.Id = TkColon;
  WantTokenAndRead(TkColon);
  TypePtr := PsTypeDenoter;
  for Field := LastField + 1 to Rec.Size do
    Rec.Fields[Field].TypePtr := TypePtr;
  WantToken2(TkSemicolon, Delimiter);
  SkipToken(TkSemicolon);
end;

procedure PsRecordVariants(var Rec : TSDTRecordDef);
var 
  Tag : TPsIdentifier;
  TagType : TSDType;
  CaseLabel : TSExpression;
begin
  WantTokenAndRead(TkCase);
  Tag := PsIdentifier;
  WantToken2(TkColon, TkOf);
  if Lexer.Token.Id = TkColon then
  begin
    ReadToken;
    TagType := PsTypeIdentifier;
    Rec.Size := Rec.Size + 1;
    Rec.Fields[Rec.Size].Name := Tag.Name;
    Rec.Fields[Rec.Size].TypePtr := TagType;
  end
  else
    TagType := FindNameOfClass(Tag.Name, SdncType, {Required=}true)^.TypePtr;
  EnsureOrdinalType(TagType);
  WantTokenAndRead(TkOf);
  repeat
    Rec.NumVariants := Rec.NumVariants + 1;
    Rec.VariantBounds[Rec.NumVariants] := Rec.Size + 1;
    repeat
      CaseLabel := ExCoerce(PsImmediate, TagType);
      ExDispose(CaseLabel);
      WantToken2(TkComma, TkColon);
      SkipToken(TkComma)
    until Lexer.Token.Id = TkColon;
    WantTokenAndRead(TkColon);
    WantTokenAndRead(TkLParen);
    while Lexer.Token.Id <> TkRparen do
      PsRecordField(Rec, TkRParen);
    WantTokenAndRead(TkRParen);
    WantToken2(TkSemicolon, TkEnd);
    SkipToken(TkSemicolon)
  until Lexer.Token.Id = TkEnd;
end;

function PsRecordType(IsPacked : boolean) : TSDType;
var Rec : TSDTRecordDef;
begin
  WantTokenAndRead(TkRecord);
  Rec.Size := 0;
  Rec.NumVariants := 0;
  Rec.IsPacked := IsPacked;
  while (Lexer.Token.Id <> TkCase) and (Lexer.Token.Id <> TkEnd) do
    PsRecordField(Rec, TkEnd);
  if Lexer.Token.Id = TkCase then
    PsRecordVariants(Rec);
  WantTokenAndRead(TkEnd);
  Result := MakeRecordType(Rec)
end;

procedure PsArguments(var Args : TSDSubroutineArgs);
var 
  IsConst : boolean;
  IsVar : boolean;
  LastArg, Arg : integer;
  TypePtr : TSDType;
begin
  WantTokenAndRead(TkLparen);
  Args.Count := 0;
  repeat
    IsConst := Lexer.Token.Id = TkConst;
    IsVar := Lexer.Token.Id = TkVar;
    if IsConst then WantTokenAndRead(TkConst);
    if IsVar then WantTokenAndRead(TkVar);
    LastArg := Args.Count;
    repeat
      Args.Count := Args.Count + 1;
      if Args.Count > MaxSubroutineArgs then
        CompileError('Too many arguments declared for subroutine');
      Args.Defs[Args.Count].Name := GetTokenValueAndRead(TkIdentifier);
      Args.Defs[Args.Count].IsConstant := IsConst;
      Args.Defs[Args.Count].IsReference := IsVar or IsConst;
      WantToken4(TkComma, TkColon, TkSemicolon, TkRparen);
      SkipToken(TkComma)
    until Lexer.Token.Id in [TkColon, TkSemicolon, TkRparen];
    if Lexer.Token.Id = TkColon then
    begin
      SkipToken(TkColon);
      TypePtr := PsTypeIdentifier;
    end
    else if IsVar then TypePtr := nil
    else CompileError('Untyped parameters must be pass-by-reference');
    for Arg := LastArg + 1 to Args.Count do
      Args.Defs[Arg].TypePtr := TypePtr;
    WantToken2(TkSemicolon, TkRparen);
    SkipToken(TkSemicolon);
  until Lexer.Token.Id = TkRparen;
  SkipToken(TkRparen)
end;

function PsResultType : TSDType;
begin
  PsResultType := PsTypeIdentifier
end;

function PsProcedureType : TSDType;
var Args : TSDSubroutineArgs;
begin
  WantTokenAndRead(TkProcedure);
  if Lexer.Token.Id = TkLParen then PsArguments(Args);
  Result := MakeFunctionType(Args, nil)
end;

function PsFunctionType : TSDType;
var 
  Args : TSDSubroutineArgs;
  ReturnType : TSDType;
begin
  WantTokenAndRead(TkFunction);
  WantToken2(TkLParen, TkColon);
  if Lexer.Token.Id = TkLParen then PsArguments(Args);
  WantTokenAndRead(TkColon);
  ReturnType := PsResultType;
  Result := MakeFunctionType(Args, ReturnType)
end;

function _PsArrayTypeInternal : TSDType;
var IndexType, ValueType : TSDType;
begin
  IndexType := PsTypeDenoter;
  WantToken2(TkComma, TkRbracket);
  if Lexer.Token.Id = TkComma then
  begin
    WantTokenAndRead(TkComma);
    ValueType := _PsArrayTypeInternal
  end
  else
  begin
    WantTokenAndRead(TkRbracket);
    WantTokenAndRead(TkOf);
    ValueType := PsTypeDenoter
  end;
  Result := MakeArrayType(IndexType, ValueType)
end;

function PsArrayType : TSDType;
begin
  WantTokenAndRead(TkArray);
  WantTokenAndRead(TkLbracket);
  Result := _PsArrayTypeInternal
end;

function PsPointerType : TSDType;
var NamePtr : TSDName;
begin
  WantTokenAndRead(TkCaret);
  WantToken(TkIdentifier);
  NamePtr := FindNameOfClass(Lexer.Token.Value, SdncType, {Required=}false);
  if NamePtr = nil then Result := MakePointerForwardType(Lexer.Token.Value)
  else
  begin
    Result := MakePointerType(NamePtr^.TypePtr);
    NamePtr^.TypePtr^.WasUsed := true
  end;
  ReadToken
end;

function PsRangeType : TSDType;
var 
  First, Last : TSExpression;
begin
  First := PsImmediate;
  WantTokenAndRead(TkRange);
  Last := PsImmediate;
  EnsureOrdinalExpr(First);
  if not IsSameType(First^.TypePtr, Last^.TypePtr) then
    ErrorForExpr('Expected ' + TypeName(First^.TypePtr), Last);

  Result := MakeRangeType(First^.TypePtr,
            ExGetOrdinal(First), ExGetOrdinal(Last));

  ExDispose(First);
  ExDispose(Last)
end;

function PsSetType : TSDType;
var ElementTypePtr : TSDType;
begin
  WantTokenAndRead(TkSet);
  WantTokenAndRead(TkOf);
  ElementTypePtr := PsTypeDenoter;
  if not IsBoundedType(ElementTypePtr) then
    ErrorForType('Set element types must be bounded ordinal types',
                 ElementTypePtr);
  if GetBoundedTypeSize(ElementTypePtr) > 256 then
    ErrorForType('Set element types may not contain more than 256 values',
                 ElementTypePtr);
  Result := MakeSetType(ElementTypePtr)
end;

function PsFileType : TSDType;
begin
  WantTokenAndRead(TkFile);
  WantTokenAndRead(TkOf);
  Result := MakeFileType(PsTypeIdentifier)
end;

function PsTypeDenoter;
var 
  Idx : TSDName;
  IsPacked : boolean;
begin
  Result := nil;
  IsPacked := Lexer.Token.Id = TkPacked;
  SkipToken(TkPacked);

  if Lexer.Token.Id = TkLparen then Result := PsEnumeratedType
  else if Lexer.Token.Id = TkFile then Result := PsFileType
  else if Lexer.Token.Id = TkSet then Result := PsSetType
  else if Lexer.Token.Id = TkRecord then Result := PsRecordType(IsPacked)
  else if Lexer.Token.Id = TkArray then Result := PsArrayType
  else if Lexer.Token.Id = TkCaret then Result := PsPointerType
  else if Lexer.Token.Id = TkProcedure then Result := PsProcedureType
  else if Lexer.Token.Id = TkFunction then Result := PsFunctionType
  else if Lexer.Token.Id = TkIdentifier then
  begin
    Idx := FindName(Lexer.Token.Value, {Required=}false);
    if Idx = nil then
    else if Idx^.Cls = SdncType then Result := PsTypeIdentifier
    else if Idx^.Cls = SdncConstant then
           Result := PsRangeType
  end
  else if Lexer.Token.Id in [TkInteger, TkString, TkMinus, TkPlus] then
         Result := PsRangeType;
  if Result = nil then
    CompileError('Expected type denoter, found ' + LxTokenStr);
end;

procedure _ResolvePointerForward(TypePtr : TSDType);
var 
  TargetPtr : TSDType;
begin
  if IsPointerForwardType(TypePtr) then
  begin
    TargetPtr := FindNameOfClass(TypePtr^.TargetName^,
                 SdncType, {Required=}true)^.TypePtr;
    dispose(TypePtr^.TargetName);
    TypePtr^.Cls := SdtcPointer;
    TypePtr^.PointedTypePtr := TargetPtr;
    TargetPtr^.WasUsed := true
  end
end;

procedure _ResolvePointerForwards(Checkpoint : TSDefinition);
var 
  Def : TSDefinition;
begin
  if Checkpoint = nil then Stack_GetOldest(CurrentScope^.LatestDef, Def)
  else Def := Checkpoint^.Newer;
  while Def <> nil do
  begin
    if Def^.Cls = SdcType then _ResolvePointerForward(@Def^.TypeDef);
    Def := Def^.Newer
  end
end;

procedure PsTypeDefinitions;
var 
  Name : string;
  TypePtr : TSDType;
  Checkpoint : TSDefinition;
begin
  Checkpoint := CurrentScope^.LatestDef;
  WantTokenAndRead(TkType);
  repeat
    Name := GetTokenValueAndRead(TkIdentifier);
    WantTokenAndRead(TkEquals);
    TypePtr := PsTypeDenoter;
    WantTokenAndRead(TkSemicolon);
    MakeAliasType(Name, TypePtr)
  until Lexer.Token.Id <> TkIdentifier;
  _ResolvePointerForwards(Checkpoint)
end;

procedure PsConstant(const Name : string);
var Constant : TSDConstantDef;
begin
  WantTokenAndRead(TkEquals);
  Constant.Name := Name;
  Constant.Value := PsImmediate;
  AddConstant(Constant);
end;

function PsConstantValue(TypePtr : TSDType) : TSExpression;
forward;

function PsConstantArray(TypePtr : TSDType) : TSExpression;
var 
  ConstSize, WantedSize : integer;
  ArrayElems, NewElem : TSEArrayElem;
  AddPoint : TListAddPoint;
begin
  ArrayElems := nil;
  AddPoint := List_GetAddPoint(ArrayElems);
  WantTokenAndRead(TkLparen);
  ConstSize := 0;
  while Lexer.Token.Id <> TkRparen do
  begin
    ConstSize := ConstSize + 1;
    new(NewElem);
    NewElem^.Value := PsConstantValue(TypePtr^.ArrayDef.ValueTypePtr);
    List_Add(AddPoint, NewElem);
    WantToken2(TkComma, TkRparen);
    SkipToken(TkComma)
  end;
  WantTokenAndRead(TkRparen);
  WantedSize := GetBoundedTypeSize(TypePtr^.ArrayDef.IndexTypePtr);
  if ConstSize <> WantedSize then
    CompileError('Array constant has size ' + IntToStr(ConstSize) +
    ' instead of ' + IntToStr(WantedSize) + ' for ' + TypeName(TypePtr));
  Result := ExArrayValue(TypePtr, ArrayElems)
end;

function PsConstantRecord(TypePtr : TSDType) : TSExpression;
var 
  FieldId : TPsIdentifier;
  FieldType : TSDType;
  RecordFields, NewField : TSERecordField;
  AddPoint : TListAddPoint;
begin
  RecordFields := nil;
  AddPoint := List_GetAddPoint(RecordFields);
  WantTokenAndRead(TkLparen);
  while Lexer.Token.Id <> TkRparen do
  begin
    new(NewField);
    FieldId := PsIdentifier;
    WantTokenAndRead(TkColon);
    NewField^.Ordinal := FindField(TypePtr, FieldId.Name, {Required=}true);
    FieldType := TypePtr^.RecPtr^.Fields[NewField^.Ordinal].TypePtr;
    NewField^.Value := PsConstantValue(FieldType);
    List_Add(AddPoint, NewField);
    WantToken2(TkSemicolon, TkRparen);
    SkipToken(TkSemicolon)
  end;
  WantTokenAndRead(TkRparen);
  Result := ExRecordValue(TypePtr, RecordFields)
end;

function PsConstantValue(TypePtr : TSDType) : TSExpression;
begin
  if IsArrayType(TypePtr) then Result := PsConstantArray(TypePtr)
  else if IsRecordType(TypePtr) then Result := PsConstantRecord(TypePtr)
  else Result := ExCoerce(PsImmediate, TypePtr)
end;

procedure PsTypedConstant(const Name : string);
var 
  TypePtr : TSDType;
begin
  WantTokenAndRead(TkColon);
  TypePtr := PsTypeDenoter;
  WantTokenAndRead(TkEquals);
  AddVariable(MakeTypedConstant(Name, TypePtr, PsConstantValue(TypePtr)))
end;

procedure PsConstDefinitions;
var 
  Name : string;
begin
  WantTokenAndRead(TkConst);
  repeat
    Name := GetTokenValueAndRead(TkIdentifier);
    WantToken2(TkEquals, TkColon);
    if Lexer.Token.Id = TkEquals then
      PsConstant(Name)
    else
      PsTypedConstant(Name);
    WantTokenAndRead(TkSemicolon)
  until Lexer.Token.Id <> TkIdentifier
end;

procedure PsVarDefinitions;
const 
  MaxVarNames = 8;
var 
  NumNames : integer;
  Names : array[1..MaxVarNames] of string;
  TypePtr : TSDType;
  Location : TSExpression;
begin
  WantTokenAndRead(TkVar);
  repeat
    NumNames := 0;
    repeat
      NumNames := NumNames + 1;
      if NumNames > MaxVarNames then
        CompileError('Too many names in variable definition');
      Names[NumNames] := GetTokenValueAndRead(TkIdentifier);
      WantToken2(TkComma, TkColon);
      SkipToken(TkComma)
    until Lexer.Token.Id = TkColon;
    WantTokenAndRead(TkColon);
    TypePtr := PsTypeDenoter;
    if Lexer.Token.Id = TkAbsolute then
    begin
      WantTokenAndRead(TkAbsolute);
      Location := PsVariable;
      EnsureAddressableExpr(Location)
    end
    else Location := nil;
    WantTokenAndRead(TkSemicolon);
    for NumNames := 1 to NumNames do
    begin
      if Location = nil then
        AddVariable(MakeVariable(Names[NumNames], TypePtr))
      else
        AddVariable(MakeAbsolute(Names[NumNames], TypePtr, Location))
    end
  until Lexer.Token.Id <> TkIdentifier
end;

function PsBody : TSSSequence;
var 
  AddPoint : TListAddPoint;
  NewStatement : TSSSequence;
begin
  Result := nil;
  AddPoint := List_GetAddPoint(Result);
  WantTokenAndRead(TkBegin);
  while Lexer.Token.Id <> TkEnd do
  begin
    new(NewStatement);
    NewStatement^.Statement := PsStatement;
    List_Add(AddPoint, NewStatement);
    WantToken2(TkSemicolon, TkEnd);
    SkipToken(TkSemicolon)
  end;
  WantTokenAndRead(TkEnd);
end;

procedure PsFunctionBody(SrPtr : TSDSubroutine);
var 
  Pos : integer;
  ResultPtr : TSDVariable;
begin
  StartLocalScope(@SrPtr^.Scope, SrPtr);
  for Pos := 1 to SrPtr^.Args.Count do
    AddVariable(MakeFromArg(SrPtr^.Args.Defs[Pos]));
  if SrPtr^.ReturnTypePtr <> nil then
  begin
    ResultPtr := AddVariable(MakeVariable('RESULT', SrPtr^.ReturnTypePtr));
    ResultPtr^.WasUsed := true
  end;
  PsDefinitions;
  SrPtr^.Body := PsBody;
  WantTokenAndRead(TkSemicolon);
  CloseLocalScope
end;

procedure PsProcedureDefinition;
var 
  Def : TSDSubroutineDef;
begin
  Def := EmptyFunction();
  WantTokenAndRead(TkProcedure);
  Def.Name := GetTokenValueAndRead(TkIdentifier);
  Def.ExternalName := Def.Name;
  WantToken2(TkLparen, TkSemicolon);
  if Lexer.Token.Id = TkLparen then PsArguments(Def.Args);
  WantTokenAndRead(TkSemicolon);
  if Lexer.Token.Id = TkForward then
  begin
    SkipToken(TkForward);
    WantTokenAndRead(TkSemicolon);
    Def.IsDeclaration := true;
    AddFunction(Def)
  end
  else
    PsFunctionBody(AddFunction(Def));
end;

procedure PsFunctionDefinition;
var 
  Def : TSDSubroutineDef;
begin
  Def := EmptyFunction();
  WantTokenAndRead(TkFunction);
  Def.Name := GetTokenValueAndRead(TkIdentifier);
  Def.ExternalName := Def.Name;
  if (Lexer.Token.Id = TkSemicolon) and HasForwardDeclaration(Def.Name) then
    Def.ReturnTypePtr := nil
  else
  begin
    WantToken2(TkLparen, TkColon);
    if Lexer.Token.Id = TkLparen then PsArguments(Def.Args);
    WantTokenAndRead(TkColon);
    Def.ReturnTypePtr := PsResultType;
  end;
  WantTokenAndRead(TkSemicolon);
  if Lexer.Token.Id = TkForward then
  begin
    SkipToken(TkForward);
    WantTokenAndRead(TkSemicolon);
    Def.IsDeclaration := true;
    AddFunction(Def)
  end
  else
    PsFunctionBody(AddFunction(Def));
end;

procedure PsDefinitions;
var 
  Done : boolean;
begin
  Done := false;
  repeat
    if Lexer.Token.Id = TkType then PsTypeDefinitions
    else if Lexer.Token.Id = TkConst then PsConstDefinitions
    else if Lexer.Token.Id = TkVar then PsVarDefinitions
    else if Lexer.Token.Id = TkProcedure then PsProcedureDefinition
    else if Lexer.Token.Id = TkFunction then PsFunctionDefinition
    else
      Done := true;
  until Done;
end;

function PsProgramHeading : string;
begin
  WantTokenAndRead(TkProgram);
  Result := GetTokenValueAndRead(TkIdentifier);
  if Lexer.Token.Id = TkLparen then
  begin
    repeat
      ReadToken;
      WantToken2(TkIdentifier, TkRparen);
      SkipToken(TkIdentifier);
      WantToken2(TkComma, TkRparen)
    until Lexer.Token.Id = TkRparen;
    SkipToken(TkRparen);
  end;
  WantTokenAndRead(TkSemicolon);
end;

function PsProgram : TSProgram;
begin
  new(Result);
  Result^.Name := PsProgramHeading;
  StartLocalScope(@Result^.Scope, nil);
  PsDefinitions;
  Result^.Body := PsBody;
  CloseLocalScope
end;

function PsPointerDeref(Ptr : TSExpression) : TSExpression;
begin
  if Ptr^.Cls = SecVariable then Ptr^.VarPtr^.WasUsed := true;
  WantTokenAndRead(TkCaret);
  PsPointerDeref := ExPointerAccess(Ptr)
end;

function PsFunctionArgs : TSEFunctionArgs;
begin
  Result.Size := 0;
  if Lexer.Token.Id = TkLParen then
  begin
    WantTokenAndRead(TkLParen);
    while Lexer.Token.Id <> TkRParen do
    begin
      Result.Size := Result.Size + 1;
      Result.Values[Result.Size] := PsExpression;
      WantToken2(TkComma, TkRParen);
      SkipToken(TkComma)
    end;
    WantTokenAndRead(TkRparen)
  end
end;

function PsFunctionCall(Fn : TSExpression) : TSExpression;
begin
  if (Fn^.Cls = SecFnRef) and (Fn^.FnPtr <> CurrentScope^.CurrentFn) then
    Fn^.FnPtr^.WasUsed := true
  else if Fn^.Cls = SecVariable then Fn^.VarPtr^.WasUsed := true;

  if Fn^.Cls = SecFnRef then
    Result := ExFunctionCall(Fn, PsFunctionArgs)
  else if IsFunctionType(Fn^.TypePtr) then
         Result := ExFunctionCall(Fn, PsFunctionArgs)
  else if Fn^.Cls = SecPsfnRef then
         Result := Fn^.PsfnPtr^.ParseFn(Fn)
end;

function PsArrayAccess(Arr : TSExpression) : TSExpression;
var 
  Idx : TSExpression;
begin
  if Arr^.Cls = SecVariable then Arr^.VarPtr^.WasUsed := true;
  WantTokenAndRead(TkLbracket);
  repeat
    Idx := PsExpression;
    if IsStringyType(Arr^.TypePtr) then Arr := ExStringChar(Arr, Idx)
    else Arr := ExArrayAccess(Arr, Idx);
    WantToken2(TkComma, TkRbracket);
    if (Lexer.Token.Id = TkComma) and not IsArrayType(Arr^.TypePtr)
       and not IsStringyType(Arr^.TypePtr) then
      ErrorForExpr('Expected an array or a string', Arr);
    SkipToken(TkComma)
  until Lexer.Token.Id = TkRbracket;
  WantTokenAndRead(TkRbracket);
  Result := Arr
end;

function PsFieldAccess(Rec : TSExpression) : TSExpression;
var 
  Fld : TPsIdentifier;
begin
  if Rec^.Cls = SecVariable then Rec^.VarPtr^.WasUsed := true;
  WantTokenAndRead(TkDot);
  Fld := PsIdentifier;
  PsFieldAccess := ExFieldAccess(Rec,
                   FindField(Rec^.TypePtr, Fld.Name, {Required=}true))
end;

function _PsVariableInternal(ForStatement : boolean;
                             CallFns : boolean) : TSExpression;
var 
  Id : TPsIdentifier;
  WithVarPtr : TSDWithVar;
  Found : TSDName;
  Expr : TSExpression;
  Done : boolean;
begin
  Id := PsIdentifier;
  WithVarPtr := FindWithVar(Id.Name);
  Found := FindName(Id.Name, {Required=}false);
  if WithVarPtr <> nil then
  begin
    Expr := ExVariable(WithVarPtr^.VarPtr);
    Expr := ExFieldAccess(Expr,
            FindField(Expr^.TypePtr, Id.Name, {Required=}true))
  end
  else if Found = nil then CompileError('Unknown identifier: ' + Id.Name)
  else if Found^.Cls = SdncVariable then Expr := ExVariable(Found^.VarPtr)
  else if Found^.Cls = SdncConstant then Expr := ExCopy(Found^.ConstPtr^.Value)
  else if Found^.Cls = SdncSubroutine then Expr := ExFnRef(Found^.SrPtr)
  else if Found^.Cls = SdncPsfn then Expr := ExPsfn(Found^.PsfnPtr)
  else CompileError('Invalid identifier: ' + Id.Name);

  Done := ForStatement and (Expr^.Cls = SecFnRef)
          and (Expr^.FnPtr = CurrentScope^.CurrentFn)
          and (Lexer.Token.Id = TkAssign);
  while not Done do
  begin
    if CallFns and (Expr^.Cls in [SecFnRef, SecPsfnRef]) then
      Expr := PsFunctionCall(Expr)
    else if CallFns and IsFunctionType(Expr^.TypePtr)
            and (Lexer.Token.Id = TkLparen) then
           Expr := PsFunctionCall(Expr)
    else if Lexer.Token.Id = TkDot then Expr := PsFieldAccess(Expr)
    else if Lexer.Token.Id = TkLbracket then Expr := PsArrayAccess(Expr)
    else if Lexer.Token.Id = TkCaret then Expr := PsPointerDeref(Expr)
    else Done := true
  end;

  if (Expr^.Cls = SecVariable) and not ForStatement then
    Expr^.VarPtr^.WasUsed := true
  else if (Expr^.Cls = SecFnRef)
          and (Expr^.FnPtr <> CurrentScope^.CurrentFn) then
         Expr^.FnPtr^.WasUsed := true;

  Result := Expr
end;

function PsVariable : TSExpression;
begin
  Result := _PsVariableInternal({ForStatement=}false, {CallFns=}true)
end;

function PsVariableOrFunction : TSExpression;
begin
  Result := _PsVariableInternal({ForStatement=}false, {CallFns=}false)
end;

function PsVariableForStatement : TSExpression;
begin
  Result := _PsVariableInternal({ForStatement=}true, {CallFns=}true)
end;

function IsOpAdding(Tok : TLxToken) : boolean;
begin
  IsOpAdding := Tok.Id in [TkPlus, TkMinus, TkOr, TkXor]
end;

function IsOpMultiplying(Tok : TLxToken) : boolean;
begin
  IsOpMultiplying := Tok.Id in [TkAsterisk, TkSlash, TkDiv, TkMod, TkAnd,
                     TkShl, TkShr]
end;

function IsOpRelational(Tok : TLxToken) : boolean;
begin
  IsOpRelational := Tok.Id in [TkEquals, TkNotEquals, TkLessthan, TkMorethan,
                    TkLessOrEquals, TkMoreOrEquals, TkIn]
end;

function ParseString(const Pstr : string) : string;
var 
  State : (None, QuotedStr, Hash, NumCharDec, NumCharHex, NumCharReady,
           Caret, Error, Done);
  Pos : integer;
  Ch : char;
  ChNum : integer;
begin
  Result := '';
  State := None;
  Pos := 1;
  while Pos <= Length(Pstr) do
  begin
    Ch := Pstr[Pos];
    if State = None then
    begin
      Pos := Pos + 1;
      if Ch = '''' then
      begin
        State := QuotedStr;
        if (Pos > 2) and (Pstr[Pos - 2] = '''') then Result := Result + ''''
      end
      else if Ch = '#' then State := Hash
      else if Ch = '^' then State := Caret
      else State := Error
    end
    else if State = QuotedStr then
    begin
      Pos := Pos + 1;
      if Ch = '''' then State := None
      else Result := Result + Ch
    end
    else if State = Hash then
    begin
      ChNum := 0;
      if Ch in LxCharsDigits then State := NumCharDec
      else if Ch = '$' then
      begin
        State := NumCharHex;
        Pos := Pos + 1
      end
    end
    else if State = NumCharDec then
    begin
      if Ch in LxCharsDigits then
      begin
        Pos := Pos + 1;
        ChNum := ChNum * 10 + Ord(Ch) - 48
      end
      else State := NumCharReady
    end
    else if State = NumCharHex then
    begin
      if Ch in LxCharsHex then
      begin
        Pos := Pos + 1;
        if Ch <= '9' then ChNum := ChNum * 16 + Ord(Ch) - 48
        else if Ch <= 'F' then ChNum := ChNum * 16 + Ord(Ch) - 55
        else if Ch <= 'f' then ChNum := ChNum * 16 + Ord(Ch) - 87
      end
      else State := NumCharReady
    end
    else if State = NumCharReady then
    begin
      Result := Result + Chr(ChNum);
      State := None
    end
    else if State = Caret then
    begin
      Pos := Pos + 1;
      State := None;
      if Ch in ['@'..'_'] then
        Result := Result + Chr(Ord(Ch) - 64)
      else if Ch in ['a'..'z'] then
             Result := Result + Chr(Ord(Ch) - 96)
      else State := Error
    end
    else if State = Error then
           CompileError('Invalid character in string: ' + Pstr)
  end;
  if State in [QuotedStr, Caret] then
    CompileError('String is not terminated: ' + Pstr);
  if State in [NumCharDec, NumCharHex] then
    Result := Result + Chr(ChNum);
end;

function ParseInt(const Pstr : string) : integer;
var Code : integer;
begin
  Val(Pstr, Result, Code);
  if Code <> 0 then CompileError('Could not parse integer: ' + Pstr)
end;

function ParseReal(const Pstr : string) : real;
var Code : integer;
begin
  Val(Pstr, Result, Code);
  if Code <> 0 then CompileError('Could not parse real number: ' + Pstr)
end;

function PsSetConstructor : TSExpression;
var First, Last : TSExpression;
begin
  Result := ExSet;
  WantTokenAndRead(TkLbracket);
  while Lexer.Token.Id <> TkRbracket do
  begin
    First := PsExpression;
    EnsureOrdinalExpr(First);
    if Lexer.Token.Id = TkRange then
    begin
      WantTokenAndRead(TkRange);
      Last := PsExpression;
      if not IsSameType(First^.TypePtr, Last^.TypePtr) then
        ErrorForExpr('Set element range bounds must belong' +
                     ' to the same type', Last)
    end
    else
      Last := nil;
    Result := ExSetAddRange(Result, First, Last);
    WantToken2(TkComma, TkRbracket);
    SkipToken(TkComma)
  end;
  WantTokenAndRead(TkRbracket)
end;

function PsFactor : TSExpression;
var 
  Expr : TSExpression;
  Str : string;
begin
  if Lexer.Token.Id = TkCaret then LxGetStringFromCaret;
  if Lexer.Token.Id = TkNil then
  begin
    Expr := ExNil;
    ReadToken
  end
  else if Lexer.Token.Id = TkString then
  begin
    Str := ParseString(GetTokenValueAndRead(TkString));
    if Length(Str) = 1 then Expr := ExCharConstant(Str[1])
    else Expr := ExStringConstant(Str)
  end
  else if Lexer.Token.Id = TkInteger then
         Expr := ExIntegerConstant(ParseInt(GetTokenValueAndRead(TkInteger)))
  else if Lexer.Token.Id = TkReal then
         Expr := ExRealConstant(ParseReal(GetTokenValueAndRead(TkReal)))
  else if Lexer.Token.Id = TkIdentifier then Expr := PsVariable
  else if Lexer.Token.Id = TkLbracket then Expr := PsSetConstructor
  else if Lexer.Token.Id = TkLparen then
  begin
    WantTokenAndRead(TkLparen);
    Expr := PsExpression;
    WantTokenAndRead(TkRparen)
  end
  else if Lexer.Token.Id = TkNot then
  begin
    WantTokenAndRead(TkNot);
    Expr := ExOpNot(PsFactor);
  end
  else if Lexer.Token.Id = TkAt then
  begin
    WantTokenAndRead(TkAt);
    Expr := ExAddressOf(PsVariableOrFunction)
  end
  else
    CompileError('Invalid token in expression: ' + LxTokenStr);
  PsFactor := Expr
end;

function PsBinaryOp(Left, Right : TSExpression;
                    Op : TLxTokenId) : TSExpression;
begin
  case Op of 
    TkPlus: Result := ExOpAdd(Left, Right);
    TkMinus: Result := ExOpSub(Left, Right);
    TkAsterisk: Result := ExOpMul(Left, Right);
    TkSlash: Result := ExOpDivReal(Left, Right);
    TkDiv: Result := ExOpDivInt(Left, Right);
    TkMod: Result := ExOpMod(Left, Right);
    TkAnd: Result := ExOpAnd(Left, Right);
    TkOr: Result := ExOpOr(Left, Right);
    TkXor: Result := ExOpXor(Left, Right);
    TkShl: Result := ExOpShl(Left, Right);
    TkShr: Result := ExOpShr(Left, Right);
    TkIn: Result := ExOpIn(Left, Right);
    TkEquals: Result := ExOpEq(Left, Right);
    TkNotEquals: Result := ExOpNe(Left, Right);
    TkLessthan: Result := ExOpLt(Left, Right);
    TkMorethan: Result := ExOpGt(Left, Right);
    TkLessOrEquals: Result := ExOpLtEq(Left, Right);
    TkMoreOrEquals: Result := ExOpGtEq(Left, Right);
    else CompileError('Expected an operator, got ' + LxTokenName(Op))
  end;
end;

function PsTerm : TSExpression;
var 
  Op : TLxTokenId;
  Expr : TSExpression;
begin
  Expr := PsFactor;
  while IsOpMultiplying(Lexer.Token) do
  begin
    Op := Lexer.Token.Id;
    ReadToken;
    Expr := PsBinaryOp(Expr, PsFactor, Op)
  end;
  PsTerm := Expr
end;

function PsSimpleExpression : TSExpression;
var 
  Negative : boolean;
  Op : TLxTokenId;
  Expr : TSExpression;
begin
  Negative := Lexer.Token.Id = TkMinus;
  if Negative then ReadToken
  else SkipToken(TkPlus);
  Expr := PsTerm;
  if Negative then Expr := ExOpNeg(Expr);
  while IsOpAdding(Lexer.Token) do
  begin
    Op := Lexer.Token.Id;
    ReadToken;
    Expr := PsBinaryOp(Expr, PsTerm, Op)
  end;
  PsSimpleExpression := Expr
end;

function PsExpression;
var 
  Op : TLxTokenId;
  Expr : TSExpression;
begin
  Expr := PsSimpleExpression;
  while IsOpRelational(Lexer.Token) do
  begin
    Op := Lexer.Token.Id;
    ReadToken;
    Expr := PsBinaryOp(Expr, PsSimpleExpression, Op)
  end;
  PsExpression := Expr
end;

function PsAssign(Lhs, Rhs : TSExpression) : TSStatement;
var ResultVarPtr : TSDVariable;
begin
  if Lhs^.Cls = SecFnRef then
  begin
    if Lhs^.FnPtr <> CurrentScope^.CurrentFn then
      ErrorForExpr('Cannot assign a value to a function', Lhs);
    ResultVarPtr := FindNameOfClass('RESULT',
                    SdncVariable, {Required=}true)^.VarPtr;
    ExDispose(Lhs);
    Lhs := ExVariable(ResultVarPtr);
  end;

  Rhs := ExCoerce(Rhs, Lhs^.TypePtr);
  if not Lhs^.IsAssignable then
  begin
    if Lhs^.IsFunctionResult then
      ErrorForExpr('Cannot assign to the result of a function', Lhs)
    else
      ErrorForExpr('Cannot assign to a constant', Lhs);
  end;
  ExMarkInitialized(Lhs);
  Result := StAssign(Lhs, Rhs)
end;

function PsStatementSequence : TSStatement;
begin
  Result := StSequence;
  Result^.Sequence := PsBody
end;

function PsProcCallStatement(Lhs : TSExpression) : TSStatement;
var 
  Stmt : TSStatement;
begin
  if Lhs^.Cls = SecWithTmpVar then
  begin
    Stmt := PsProcCallStatement(Lhs^.TmpVarChild);
    Result := StWith(ExVariable(Lhs^.TmpVar^.VarPtr), Lhs^.TmpVarValue, Stmt);
  end
  else if Lhs^.IsStatement then Result := StProcCall(Lhs)
  else if Lexer.Token.Id = TkEquals then
         CompileError('Invalid statement' +
                      ' (maybe you wrote ''='' instead of '':=''?)')
  else CompileError('Invalid statement')
end;

function PsIdentifierStatement : TSStatement;
var Lhs : TSExpression;
begin
  Lhs := PsVariableForStatement;
  if Lexer.Token.Id = TkAssign then
  begin
    WantTokenAndRead(TkAssign);
    Result := PsAssign(Lhs, PsExpression);
  end
  else
  begin
    if IsFunctionType(Lhs^.TypePtr) then Lhs := PsFunctionCall(Lhs);
    Result := PsProcCallStatement(Lhs)
  end
end;

function PsIfStatement : TSStatement;
var 
  Cond : TSExpression;
  IfThen, IfElse : TSStatement;
begin
  IfThen := nil;
  IfElse := nil;
  WantTokenAndRead(TkIf);
  Cond := ExCoerce(PsExpression, PrimitiveTypes.PtBoolean);
  WantTokenAndRead(TkThen);
  if Lexer.Token.Id <> TkElse then IfThen := PsStatement;
  if Lexer.Token.Id = TkElse then
  begin
    WantTokenAndRead(TkElse);
    IfElse := PsStatement;
  end;
  Result := StIf(Cond, IfThen, IfElse)
end;

function PsCaseStatement : TSStatement;
var 
  CaseSelector : TSExpression;
  CaseTypePtr : TSDType;
  CaseLabel : TSExpression;
  Stmt : TSStatement;
  CaseList, CaseEntry : TSSCase;
  AddPoint : TListAddPoint;
  SeqEntry : TSSSequence;
  SeqAddPoint : TListAddPoint;
begin
  CaseList := nil;
  AddPoint := List_GetAddPoint(CaseList);
  WantTokenAndRead(TkCase);
  CaseSelector := PsExpression;
  CaseTypePtr := CaseSelector^.TypePtr;
  EnsureOrdinalExpr(CaseSelector);
  WantTokenAndRead(TkOf);
  repeat
    CaseLabel := ExCoerce(PsImmediate, CaseTypePtr);
    WantTokenAndRead(TkColon);
    Stmt := PsStatement;
    CaseEntry := StCaseEntry(CaseLabel, Stmt);
    List_Add(AddPoint, CaseEntry);
    WantToken3(TkSemicolon, TkElse, TkEnd);
    SkipToken(TkSemicolon);
  until Lexer.Token.Id in [TkElse, TkEnd];
  if Lexer.Token.Id = TkElse then
  begin
    ReadToken;
    Stmt := StSequence;
    SeqAddPoint := List_GetAddPoint(Stmt^.Sequence);
    while Lexer.Token.Id <> TkEnd do
    begin
      SeqEntry := StSequenceEntry(PsStatement);
      List_Add(SeqAddPoint, SeqEntry);
      WantToken2(TkSemicolon, TkEnd);
      SkipToken(TkSemicolon)
    end;
    CaseEntry := StCaseEntry(nil, Stmt);
    List_Add(AddPoint, CaseEntry)
  end;
  WantTokenAndRead(TkEnd);
  Result := StCase(CaseSelector, CaseList)
end;

function PsRepeatStatement : TSStatement;
var 
  Cond : TSExpression;
  Sequence, SeqEntry : TSSSequence;
  AddPoint : TListAddPoint;
begin
  Sequence := nil;
  AddPoint := List_GetAddPoint(Sequence);
  WantTokenAndRead(TkRepeat);
  while Lexer.Token.Id <> TkUntil do
  begin
    SeqEntry := StSequenceEntry(PsStatement);
    List_Add(AddPoint, SeqEntry);
    WantToken2(TkSemicolon, TkUntil);
    SkipToken(TkSemicolon)
  end;
  WantTokenAndRead(TkUntil);
  Cond := ExCoerce(PsExpression, PrimitiveTypes.PtBoolean);
  Result := StRepeat(Cond, Sequence)
end;

function PsWhileStatement : TSStatement;
var 
  Cond : TSExpression;
  Stmt : TSStatement;
begin
  WantTokenAndRead(TkWhile);
  Cond := ExCoerce(PsExpression, PrimitiveTypes.PtBoolean);
  WantTokenAndRead(TkDo);
  Stmt := PsStatement;
  Result := StWhile(Cond, Stmt)
end;

function PsForStatement : TSStatement;
var 
  Iter, First, Last : TSExpression;
  Ascending : boolean;
  Stmt : TSStatement;
begin
  WantTokenAndRead(TkFor);
  Iter := PsExpression;
  if Iter^.IsFunctionResult then
    ErrorForExpr('Iterator must not be the result of a function', Iter);
  EnsureAssignableExpr(Iter);
  EnsureOrdinalExpr(Iter);
  if Iter^.Cls = SecVariable then
  begin
    Iter^.VarPtr^.WasInitialized := true;
    Iter^.VarPtr^.WasUsed := true
  end;
  WantTokenAndRead(TkAssign);
  First := ExCoerce(PsExpression, Iter^.TypePtr);
  WantToken2(TkTo, TkDownto);
  Ascending := Lexer.Token.Id = TkTo;
  ReadToken;
  Last := ExCoerce(PsExpression, Iter^.TypePtr);
  WantTokenAndRead(TkDo);
  Stmt := PsStatement;
  Result := StFor(Iter, First, Last, Ascending, Stmt)
end;

function _PsWithStatementInner : TSStatement;
var 
  Base : TSExpression;
  WithVarPtr : TSDWithVar;
  Stmt : TSStatement;
begin
  ReadToken;
  Base := PsExpression;
  WithVarPtr := AddWithVar(Base);
  WantToken2(TkComma, TkDo);
  if Lexer.Token.Id = TkComma then Stmt := _PsWithStatementInner
  else
  begin
    WantTokenAndRead(TkDo);
    Stmt := PsStatement
  end;
  Result := StWith(ExVariable(WithVarPtr^.VarPtr), Base, Stmt);
  WithVarPtr^.IsActive := false
end;


function PsWithStatement : TSStatement;
begin
  WantToken(TkWith);
  Result := _PsWithStatementInner
end;

function PsEmptyStatement : TSStatement;
begin
  Result := StEmpty
end;

function PsStatement : TSStatement;
begin
  if Lexer.Token.Id = TkSemicolon then Result := PsEmptyStatement
  else if Lexer.Token.Id = TkBegin then Result := PsStatementSequence
  else if Lexer.Token.Id = TkIdentifier then Result := PsIdentifierStatement
  else if Lexer.Token.Id = TkIf then Result := PsIfStatement
  else if Lexer.Token.Id = TkRepeat then Result := PsRepeatStatement
  else if Lexer.Token.Id = TkWhile then Result := PsWhileStatement
  else if Lexer.Token.Id = TkFor then Result := PsForStatement
  else if Lexer.Token.Id = TkWith then Result := PsWithStatement
  else if Lexer.Token.Id = TkCase then Result := PsCaseStatement
  else
    CompileError('Unexpected token ' + LxTokenStr)
end;

procedure ExecuteDirective(const Dir : string);
begin
  if (length(Dir) > 3) and (Dir[2] = 'I') and (Dir[3] = ' ') then
    LxInclude(Copy(Dir, 4, 255))
  else if (length(Dir) = 3) and (Dir[2] = 'R') and (Dir[3] in ['-', '+']) then
         Options.CheckBounds := Dir[3] = '+'
  else if (length(Dir) = 3) and (Dir[2] = 'I') and (Dir[3] in ['-', '+']) then
         Options.CheckIoResult := Dir[3] = '+'
end;

procedure ReadToken;
var 
  Stop : boolean;
  PrevStack : TLxIncludeStack;
begin
  repeat
    LxReadToken;
    Stop := Lexer.Token.Id <> TkComment;
    if Lexer.Token.Id = TkComment then
      if (Length(Lexer.Token.Value) >= 2) and (Lexer.Token.Value[1] = '$') then
        ExecuteDirective(Lexer.Token.Value);
    if (Lexer.Token.Id = TkEof) and (Lexer.IncludeStack <> nil) then
    begin
      Lexer.Input := Lexer.IncludeStack^.Input;
      PrevStack := Lexer.IncludeStack;
      Lexer.IncludeStack := PrevStack^.Prev;
      dispose(PrevStack);
      Stop := false
    end
  until Stop;
end;

function ParseProgram : TSProgram;
begin
  ReadToken;
  Result := PsProgram;
  WantTokenAndRead(TkDot);
  WantToken(TkEof);
end;

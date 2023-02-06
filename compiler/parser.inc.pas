procedure ReadToken;
forward;

procedure WantToken(Id : TLxTokenId);
begin
  if Lexer.Token.Id <> Id then
    CompileError('Wanted token ' + LxTokenName(Id) + ', found ' + LxTokenStr)
end;

procedure WantToken2(Id1, Id2 : TLxTokenId);
begin
  if (Lexer.Token.Id <> Id1) and (Lexer.Token.Id <> Id2) then
    CompileError('Wanted token ' + LxTokenName(Id1) + ' or ' +
    LxTokenName(Id2) + ', found ' + LxTokenStr)
end;

procedure WantToken3(Id1, Id2, Id3 : TLxTokenId);
begin
  if (Lexer.Token.Id <> Id1)
     and (Lexer.Token.Id <> Id2) and (Lexer.Token.Id <> Id3) then
    CompileError('Wanted token ' + LxTokenName(Id1) + ', ' + LxTokenName(Id2) +
    ', or ' + LxTokenName(Id3) + ', found ' + LxTokenStr)
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

function PsTypeDenoter : TPsTypePtr;
forward;

function PsTypeIdentifier : TPsTypePtr;
begin
  WantToken(TkIdentifier);
  Result := FindNameOfClass(Lexer.Token.Value,
                      TncType, {Required=}true)^.TypePtr;
  Result^.WasUsed := true;
  ReadToken
end;

function PsIdentifier : TPsIdentifier;
var 
  Ident : TPsIdentifier;
begin
  Ident.Name := GetTokenValueAndRead(TkIdentifier);
  PsIdentifier := Ident
end;

type 
  ExpressionType = (Lhs, Rhs);

function PsExpression : TExpression;
forward;

function PsEnumeratedType : TPsTypePtr;
var 
  Typ : TPsType;
  Enum : TPsEnumDef;
begin
  WantTokenAndRead(TkLparen);
  Enum.Size := 0;
  Enum.HasBeenDefined := false;
  repeat
    Enum.Size := Enum.Size + 1;
    if Enum.Size > MaxEnumVals then
      CompileError('Too many values in enum');
    Enum.Values[Enum.Size - 1] := GetTokenValueAndRead(TkIdentifier);
    WantToken2(TkComma, TkRparen);
    SkipToken(TkComma);
  until Lexer.Token.Id = TkRparen;
  Typ := EmptyType;
  Typ.Cls := TtcEnum;
  Typ.EnumPtr := AddEnum(Enum);
  PsEnumeratedType := AddType(Typ);
  SkipToken(TkRparen)
end;

procedure PsRecordField(var Rec : TPsRecordDef; Delimiter : TLxTokenId);
var 
  Name : string;
  LastField, Field : integer;
  TypePtr : TPsTypePtr;
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

procedure PsRecordVariants(var Rec : TPsRecordDef);
var 
  Tag : TPsIdentifier;
  TagType : TPsTypePtr;
  CaseLabel : TExpression;
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
    TagType := FindNameOfClass(Tag.Name, TncType, {Required=}true)^.TypePtr;
  if not IsOrdinalType(TagType) then
    CompileError('The index of the case statement is not ordinal');
  WantTokenAndRead(TkOf);
  repeat
    Rec.NumVariants := Rec.NumVariants + 1;
    Rec.VariantBounds[Rec.NumVariants] := Rec.Size + 1;
    repeat
      CaseLabel := ExCoerce(PsExpression, TagType);
      if not CaseLabel^.IsConstant then
        CompileError('The label of the case statement is not constant');
      DisposeExpr(CaseLabel);
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

function PsRecordType : TPsTypePtr;
var 
  Typ : TPsType;
  Rec : TPsRecordDef;
begin
  WantTokenAndRead(TkRecord);
  Rec.Size := 0;
  Rec.NumVariants := 0;
  Rec.HasBeenDefined := false;
  while (Lexer.Token.Id <> TkCase) and (Lexer.Token.Id <> TkEnd) do
    PsRecordField(Rec, TkEnd);
  if Lexer.Token.Id = TkCase then
    PsRecordVariants(Rec);
  WantTokenAndRead(TkEnd);
  Typ := TypeOfClass(TtcRecord);
  Typ.RecPtr := AddRecord(Rec);
  PsRecordType := AddType(Typ);
end;

function PsImmediate : TExpression;
var Expr : TExpression;
begin
  Expr := PsExpression;
  if Expr^.cls <> XcImmediate then
    CompileError('Expected an immediate expression');
  PsImmediate := Expr
end;

function PsArrayType : TPsTypePtr;
var 
  Typ : TPsType;
  Arr : TPsArrayDef;
begin
  WantTokenAndRead(TkArray);
  WantTokenAndRead(TkLbracket);
  Arr.IndexTypePtr := PsTypeDenoter;
  if not IsBoundedType(Arr.IndexTypePtr) then
    CompileError('Array indices must belong to a bounded ordinal type');
  WantTokenAndRead(TkRbracket);
  WantTokenAndRead(TkOf);
  Arr.ValueTypePtr := PsTypeDenoter;
  Typ := TypeOfClass(TtcArray);
  Typ.ArrayPtr := AddArray(Arr);
  PsArrayType := AddType(Typ)
end;

function PsPointerType : TPsTypePtr;
var 
  Typ : TPsType;
  NamePtr : TPsNamePtr;
begin
  WantTokenAndRead(TkCaret);
  WantToken(TkIdentifier);
  NamePtr := FindNameOfClass(Lexer.Token.Value, TncType, {Required=}false);
  if NamePtr = nil then Typ := PointerUnknownType(Lexer.Token.Value)
  else Typ := PointerType(NamePtr^.TypePtr);
  Typ.WasUsed := true;
  ReadToken;
  PsPointerType := AddType(Typ)
end;

function PsCompareExprs(A, B : TExpression; Op : TLxTokenId) : boolean;
var CmpExpr : TExpression;
begin
  CmpExpr := ExBinaryOp(CopyExpr(A), CopyExpr(B), Op);
  if CmpExpr^.Cls <> XcImmediate then
    CompileError('Internal error: the compared expressions are not immediate');
  Result := CmpExpr^.Immediate.BooleanVal;
  DisposeExpr(CmpExpr)
end;

function PsRangeType : TPsTypePtr;
var 
  Typ : TPsType;
  Range : TPsRangeDef;
begin
  Range.First := PsImmediate;
  WantTokenAndRead(TkRange);
  Range.Last := PsImmediate;
  Range.BaseTypePtr := Range.First^.TypePtr;
  if not IsSameType(Range.First^.TypePtr, Range.Last^.TypePtr) then
    CompileError('The bounds of a subrange must belong to the same type');
  if not IsOrdinalType(Range.BaseTypePtr) then
    CompileError('The bounds of a subrange must belong to an ordinal type');
  if not PsCompareExprs(Range.First, Range.Last, TkLessOrEquals) then
    CompileError('The bounds of a subrange must be in ascending order');
  Typ := TypeOfClass(TtcRange);
  Typ.RangePtr := AddRange(Range);
  Result := AddType(Typ)
end;

function PsTypeDenoter;
var Idx : TPsNamePtr;
begin
  Result := nil;
  if Lexer.Token.Id = TkLparen then Result := PsEnumeratedType
  else if Lexer.Token.Id = TkRecord then Result := PsRecordType
  else if Lexer.Token.Id = TkArray then Result := PsArrayType
  else if Lexer.Token.Id = TkCaret then Result := PsPointerType
  else if Lexer.Token.Id = TkIdentifier then
  begin
    Idx := FindName(Lexer.Token.Value, {Required=}false);
    if Idx = nil then
    else if Idx^.Cls = TncType then Result := PsTypeIdentifier
    else if (Idx^.Cls = TncConstant) or (Idx^.Cls = TncEnumVal) then
           Result := PsRangeType
  end
  else if (Lexer.Token.Id = TkNumber) or (Lexer.Token.Id = TkString)
          or (Lexer.Token.Id = TkMinus) or (Lexer.Token.Id = TkPlus) then
         Result := PsRangeType;
  if Result = nil then
    CompileError('Expected type denoter, found ' + LxTokenStr);
end;

procedure _ResolvePointerUnknown(TypePtr : TPsTypePtr);
var 
  TargetPtr : TPsTypePtr;
begin
  if TypePtr^.Cls = TtcPointerUnknown then
  begin
    TargetPtr := FindNameOfClass(TypePtr^.TargetName^,
                 TncType, {Required=}true)^.TypePtr;
    dispose(TypePtr^.TargetName);
    TypePtr^.Cls := TtcPointer;
    TypePtr^.PointedTypePtr := TargetPtr;
    TargetPtr^.WasUsed := true
  end
end;

procedure PsTypeDefinitions;
var 
  Name : string;
  TypePtr : TPsTypePtr;
  NewType : TPsType;
  Checkpoint : TPsDefPtr;
begin
  Checkpoint := Defs.Latest;
  WantTokenAndRead(TkType);
  repeat
    Name := GetTokenValueAndRead(TkIdentifier);
    WantTokenAndRead(TkEquals);
    TypePtr := PsTypeDenoter;
    NewType := CopyType(TypePtr);
    NewType.Name := Name;
    NewType.AliasFor := TypePtr;
    TypePtr := AddType(NewType);
    WantTokenAndRead(TkSemicolon)
  until Lexer.Token.Id <> TkIdentifier;
  OutTypeDefinitionsFromCheckpoint(Checkpoint);
  OutEnumValuesFromCheckpoint(Checkpoint)
end;

procedure PsConstant(Name : string);
var Constant : TPsConstant;
begin
  WantTokenAndRead(TkEquals);
  Constant.Name := Name;
  Constant.Value := PsImmediate;
  AddConstant(Constant);
end;

procedure PsConstantValue(TypePtr : TPsTypePtr);
var 
  Expr : TExpression;
begin
  if IsArrayType(TypePtr) then
  begin
    WantTokenAndRead(TkLparen);
    TypePtr := TypePtr^.ArrayPtr^.ValueTypePtr;
    OutConstantArrayBegin;
    while Lexer.Token.Id <> TkRparen do
    begin
      PsConstantValue(TypePtr);
      WantToken2(TkComma, TkRparen);
      if Lexer.Token.Id = TkComma then OutConstantArraySeparator;
      SkipToken(TkComma)
    end;
    OutConstantArrayEnd;
    WantTokenAndRead(TkRparen);
  end
  else
  begin
    Expr := ExCoerce(PsImmediate, TypePtr);
    OutExpression(Expr);
    DisposeExpr(Expr)
  end
end;

procedure PsTypedConstant(Name : string);
var 
  TypePtr : TPsTypePtr;
begin
  WantTokenAndRead(TkColon);
  TypePtr := PsTypeDenoter;
  WantTokenAndRead(TkEquals);
  OutConstantDefinitionBegin(AddVariable(MakeTypedConstant(Name, TypePtr)));
  PsConstantValue(TypePtr);
  OutConstantDefinitionEnd
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
  until Lexer.Token.Id <> TkIdentifier;
end;

procedure PsVarDefinitions;
const 
  MaxVarNames = 8;
var 
  NumNames : integer;
  Names : array[1..MaxVarNames] of string;
  TypePtr : TPsTypePtr;
  Checkpoint : TPsDefPtr;
begin
  Checkpoint := Defs.Latest;
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
    WantTokenAndRead(TkSemicolon);
    for NumNames := 1 to NumNames do
      OutVariableDefinition(AddVariable(MakeVariable(Names[NumNames],
                            TypePtr, false)));
  until Lexer.Token.Id <> TkIdentifier;
  OutEnumValuesFromCheckpoint(Checkpoint)
end;

procedure PsStatement;
forward;

procedure PsDefinitions;
forward;

procedure PsFunctionBody(FnPtr : TPsFnPtr);
var 
  Pos : integer;
  Checkpoint : TPsDefPtr;
begin
  StartLocalScope(FnPtr);
  Checkpoint := Defs.Latest;
  for Pos := 1 to FnPtr^.ArgCount do
    AddVariable(FnPtr^.Args[Pos]);
  OutFunctionDefinition(FnPtr);
  OutEnumValuesFromCheckpoint(Checkpoint);
  if FnPtr^.ReturnTypePtr <> nil then
    OutVariableDefinition(AddVariable(MakeVariable('RESULT',
                          FnPtr^.ReturnTypePtr, false)));
  PsDefinitions;
  WantTokenAndRead(TkBegin);
  while Lexer.Token.Id <> TkEnd do
  begin
    PsStatement;
    WantToken2(TkSemicolon, TkEnd);
    SkipToken(TkSemicolon)
  end;
  WantTokenAndRead(TkEnd);
  WantTokenAndRead(TkSemicolon);
  OutFunctionEnd(FnPtr);
  CloseLocalScope
end;

procedure PsArguments(var Def : TPsFunction);
var 
  IsReference : boolean;
  LastArg, Arg : integer;
  TypePtr : TPsTypePtr;
begin
  WantTokenAndRead(TkLparen);
  Def.ArgCount := 0;
  repeat
    IsReference := Lexer.Token.Id = TkVar;
    SkipToken(TkVar);
    LastArg := Def.ArgCount;
    repeat
      Def.ArgCount := Def.ArgCount + 1;
      if Def.ArgCount > MaxFnArgs then
        CompileError('Too many arguments declared for function ' + Def.Name);
      Def.Args[Def.ArgCount].Name := GetTokenValueAndRead(TkIdentifier);
      Def.Args[Def.ArgCount].IsReference := IsReference;
      Def.Args[Def.ArgCount].IsConstant := false;
      Def.Args[Def.ArgCount].WasInitialized := true;
      WantToken2(TkColon, TkComma);
      SkipToken(TkComma)
    until Lexer.Token.Id = TkColon;
    SkipToken(TkColon);
    TypePtr := PsTypeIdentifier;
    for Arg := LastArg + 1 to Def.ArgCount do
      Def.Args[Arg].TypePtr := TypePtr;
    WantToken2(TkSemicolon, TkRparen);
    SkipToken(TkSemicolon);
  until Lexer.Token.Id = TkRparen;
  SkipToken(TkRparen)
end;

procedure PsProcedureDefinition;
var 
  Def : TPsFunction;
begin
  Def := EmptyFunction();
  WantTokenAndRead(TkProcedure);
  Def.Name := GetTokenValueAndRead(TkIdentifier);
  WantToken2(TkLparen, TkSemicolon);
  if Lexer.Token.Id = TkLparen then PsArguments(Def);
  WantTokenAndRead(TkSemicolon);
  if Lexer.Token.Id = TkForward then
  begin
    SkipToken(TkForward);
    WantTokenAndRead(TkSemicolon);
    Def.IsDeclaration := true;
    OutFunctionDeclaration(AddFunction(Def))
  end
  else
    PsFunctionBody(AddFunction(Def));
end;

function PsResultType : TPsTypePtr;
begin
  PsResultType := PsTypeIdentifier
end;

procedure PsFunctionDefinition;
var 
  Def : TPsFunction;
begin
  Def := EmptyFunction();
  WantTokenAndRead(TkFunction);
  Def.Name := GetTokenValueAndRead(TkIdentifier);
  if (Lexer.Token.Id = TkSemicolon) and HasForwardDeclaration(Def.Name) then
    Def.ReturnTypePtr := nil
  else
  begin
    WantToken2(TkLparen, TkColon);
    if Lexer.Token.Id = TkLparen then PsArguments(Def);
    WantTokenAndRead(TkColon);
    Def.ReturnTypePtr := PsResultType;
  end;
  WantTokenAndRead(TkSemicolon);
  if Lexer.Token.Id = TkForward then
  begin
    SkipToken(TkForward);
    WantTokenAndRead(TkSemicolon);
    Def.IsDeclaration := true;
    OutFunctionDeclaration(AddFunction(Def))
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

procedure PsProgramHeading;
begin
  WantTokenAndRead(TkProgram);
  OutProgramHeading(GetTokenValueAndRead(TkIdentifier));
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

function PsPointerDeref(Ptr : TExpression) : TExpression;
begin
  WantTokenAndRead(TkCaret);
  PsPointerDeref := ExPointerAccess(Ptr)
end;

function PsRead(FnExpr : TExpression) : TExpression;
var 
  Expr : TExpression;
  First : boolean;
  OutVar : TExpression;
  ReadArg : ^TExReadArgs;
begin
  Expr := ExPseudoFnCall(FnExpr);
  Expr^.PseudoFnCall.Arg1 := ExVariable(FindNameOfClass('INPUT',
                             TncVariable, {Required=}true)^.VarPtr);
  ReadArg := nil;
  if Lexer.Token.Id = TkLparen then
  begin
    First := true;
    WantTokenAndRead(TkLparen);
    while Lexer.Token.Id <> TkRparen do
    begin
      OutVar := PsExpression;
      if First and OutVar^.IsAssignable and IsTextType(OutVar^.TypePtr) then
      begin
        DisposeExpr(Expr^.PseudoFnCall.Arg1);
        Expr^.PseudoFnCall.Arg1 := OutVar
      end
      else
      begin
        if not OutVar^.IsAssignable
           or not IsStringyType(OutVar^.TypePtr) then
          CompileError('Invalid expression for read argument');
        if ReadArg = nil then
        begin
          new(Expr^.PseudoFnCall.ReadArgs);
          ReadArg := Expr^.PseudoFnCall.ReadArgs
        end
        else
        begin
          new(ReadArg^.Next);
          ReadArg := ReadArg^.Next;
        end;
        ReadArg^.Next := nil;
        ReadArg^.Arg := OutVar;
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      First := false
    end;
    WantTokenAndRead(TkRparen)
  end;
  PsRead := Expr
end;

function PsWrite(FnExpr : TExpression) : TExpression;
var 
  Expr : TExpression;
  First : boolean;
  OutExpr : TExpression;
  WriteArg : ^TExWriteArgs;
begin
  Expr := ExPseudoFnCall(FnExpr);
  Expr^.PseudoFnCall.Arg1 := ExVariable(FindNameOfClass('OUTPUT',
                             TncVariable, {Required=}true)^.VarPtr);
  WriteArg := nil;
  if Lexer.Token.Id = TkLparen then
  begin
    First := true;
    WantTokenAndRead(TkLparen);
    while Lexer.Token.Id <> TkRparen do
    begin
      OutExpr := PsExpression;
      if First and OutExpr^.IsAssignable and IsTextType(OutExpr^.TypePtr) then
      begin
        DisposeExpr(Expr^.PseudoFnCall.Arg1);
        Expr^.PseudoFnCall.Arg1 := OutExpr
      end
      else
      begin
        if WriteArg = nil then
        begin
          new(Expr^.PseudoFnCall.WriteArgs);
          WriteArg := Expr^.PseudoFnCall.WriteArgs
        end
        else
        begin
          new(WriteArg^.Next);
          WriteArg := WriteArg^.Next;
        end;
        WriteArg^.Next := nil;
        WriteArg^.Arg := OutExpr;
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      First := false
    end;
    WantTokenAndRead(TkRparen)
  end;
  PsWrite := Expr
end;

function PsStr(FnExpr : TExpression) : TExpression;
var 
  Expr, Src, Dest : TExpression;
begin
  WantTokenAndRead(TkLparen);
  Src := PsExpression;
  WantTokenAndRead(TkComma);
  Dest := PsExpression;
  if not Dest^.IsAssignable or not IsStringType(Dest^.TypePtr) then
    CompileError('Destination argument is not a string variable');
  WantTokenAndRead(TkRparen);
  Expr := ExPseudoFnCallBinary(FnExpr, Src, Dest);
  PsStr := Expr
end;

function PsOrdPrecSucc(FnExpr : TExpression) : TExpression;
var 
  Expr, Value : TExpression;
begin
  WantTokenAndRead(TkLparen);
  Value := PsExpression;
  WantTokenAndRead(TkRparen);
  if not IsOrdinalType(Value^.TypePtr) then
    CompileError('Argument does not have an ordinal type');
  Expr := ExPseudoFnCallUnary(FnExpr, Value);
  Result := Expr
end;

function PsNewDispose(FnExpr : TExpression) : TExpression;
var 
  Expr, Ptr : TExpression;
begin
  WantTokenAndRead(TkLparen);
  Ptr := PsExpression;
  WantTokenAndRead(TkRparen);
  if not Ptr^.IsAssignable or not IsPointerType(Ptr^.TypePtr) then
    CompileError('Argument is not a pointer');
  Expr := ExPseudoFnCallUnary(FnExpr, Ptr);
  Result := Expr
end;

function PsFunctionCall(Fn : TExpression) : TExpression;
var 
  Args : TExFunctionArgs;
begin
  if Fn^.Cls = XcFnRef then
  begin
    Args.Size := 0;
    if Lexer.Token.Id = TkLParen then
    begin
      WantTokenAndRead(TkLParen);
      while Lexer.Token.Id <> TkRParen do
      begin
        Args.Size := Args.Size + 1;
        Args.Values[Args.Size] := PsExpression;
        WantToken2(TkComma, TkRParen);
        SkipToken(TkComma)
      end;
      WantTokenAndRead(TkRparen)
    end;
    PsFunctionCall := ExFunctionCall(Fn, Args)
  end
  else if Fn^.Cls = XcPseudoFnRef then
  begin
    case Fn^.PseudoFn of 
      TpfDispose : PsFunctionCall := PsNewDispose(Fn);
      TpfNew : PsFunctionCall := PsNewDispose(Fn);
      TpfOrd : PsFunctionCall := PsOrdPrecSucc(Fn);
      TpfPred : PsFunctionCall := PsOrdPrecSucc(Fn);
      TpfRead : PsFunctionCall := PsRead(Fn);
      TpfReadln : PsFunctionCall := PsRead(Fn);
      TpfStr : PsFunctionCall := PsStr(Fn);
      TpfSucc : PsFunctionCall := PsOrdPrecSucc(Fn);
      TpfWrite : PsFunctionCall := PsWrite(Fn);
      TpfWriteln : PsFunctionCall := PsWrite(Fn);
      else CompileError('Internal error: unimplemented special function')
    end;
  end;
end;

function PsArrayAccess(Arr : TExpression) : TExpression;
var 
  Idx : TExpression;
begin
  WantTokenAndRead(TkLbracket);
  Idx := PsExpression;
  WantTokenAndRead(TkRbracket);
  if IsStringyType(Arr^.TypePtr) then
    PsArrayAccess := ExStringChar(Arr, Idx)
  else
    PsArrayAccess := ExArrayAccess(Arr, Idx)
end;

function PsFieldAccess(Rec : TExpression) : TExpression;
var 
  Fld : TPsIdentifier;
begin
  WantTokenAndRead(TkDot);
  Fld := PsIdentifier;
  PsFieldAccess := ExFieldAccess(Rec,
                   FindField(Rec^.TypePtr, Fld.Name, {Required=}true))
end;

function PsVariable : TExpression;
var 
  Id : TPsIdentifier;
  WithVarPtr : TPsWithVarPtr;
  Found : TPsName;
  Expr : TExpression;
begin
  Id := PsIdentifier;
  WithVarPtr := FindWithVar(Id.Name);
  if WithVarPtr <> nil then
  begin
    Expr := ExVariable(WithVarPtr^.VarPtr);
    Expr := ExFieldAccess(Expr,
            FindField(Expr^.TypePtr, Id.Name, {Required=}true))
  end
  else
  begin
    Found := FindName(Id.Name, {Required=}true)^;
    if Found.Cls = TncVariable then
      Expr := ExVariable(Found.VarPtr)
    else if Found.Cls = TncConstant then
           Expr := CopyExpr(Found.ConstPtr^.Value)
    else if Found.Cls = TncFunction then
           Expr := ExFnRef(Found.FnPtr)
    else if Found.Cls = TncEnumVal then
           Expr := ExEnumConstant(Found.Ordinal, Found.EnumTypePtr)
    else if Found.Cls = TncPseudoFn then
           Expr := ExPseudoFn(Found.PseudoFn)
    else
      CompileError('Invalid identifier: ' + Id.Name)
  end;
  PsVariable := Expr
end;

function PsVariableOrFunctionExtension(Expr : TExpression) : TExpression;
var 
  Done : boolean;
begin
  Done := false;
  repeat
    if (Expr^.Cls = XcFnRef) or (Expr^.Cls = XcPseudoFnRef) then
      Expr := PsFunctionCall(Expr)
    else if Lexer.Token.Id = TkDot then Expr := PsFieldAccess(Expr)
    else if Lexer.Token.Id = TkLbracket then Expr := PsArrayAccess(Expr)
    else if Lexer.Token.Id = TkCaret then Expr := PsPointerDeref(Expr)
    else Done := true
  until Done;
  PsVariableOrFunctionExtension := Expr
end;

function IsOpAdding(Tok : TLxToken) : boolean;
begin
  IsOpAdding := (Tok.Id = TkPlus) or (Tok.Id = TkMinus) or (Tok.Id = TkOr)
end;

function IsOpMultiplying(Tok : TLxToken) : boolean;
begin
  IsOpMultiplying := (Tok.Id = TkAsterisk) or (Tok.Id = TkSlash)
                     or (Tok.Id = TkDiv) or (Tok.Id = TkMod)
                     or (Tok.Id = TkAnd)
end;

function IsOpRelational(Tok : TLxToken) : boolean;
begin
  IsOpRelational := (Tok.Id = TkEquals) or (Tok.Id = TkNotEquals)
                    or (Tok.Id = TkLessthan) or (Tok.Id = TkMorethan)
                    or (Tok.Id = TkLessOrEquals) or (Tok.Id = TkMoreOrEquals)
                    or (Tok.Id = TkIn);
end;

function ParseString(Pstr : string) : string;
var 
  InStr : boolean;
  Pos : integer;
  Chr : char;
  Str : string;
begin
  Str := '';
  InStr := false;
  for Pos := 1 to Length(Pstr) do
  begin
    Chr := Pstr[Pos];
    if Chr = '''' then
    begin
      InStr := not InStr;
      if InStr and (Pos > 1) and (Pstr[Pos - 1] = '''') then
        Str := Str + ''''
    end
    else if InStr then Str := Str + Chr
  end;
  ParseString := Str
end;

function ParseInt(Pstr : string) : integer;
var 
  Ret : integer;
  Pos : integer;
begin
  Ret := 0;
  for Pos := 1 to Length(Pstr) do
    Ret := Ret * 10 + Ord(Pstr[Pos]) - 48;
  ParseInt := Ret
end;

function PsFactor : TExpression;
var 
  Expr : TExpression;
  Str : string;
begin
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
  else if Lexer.Token.Id = TkNumber then
         Expr := ExIntegerConstant(ParseInt(GetTokenValueAndRead(TkNumber)))
  else if Lexer.Token.Id = TkIdentifier then
         Expr := PsVariableOrFunctionExtension(PsVariable)
  else if Lexer.Token.Id = TkLparen then
  begin
    WantTokenAndRead(TkLparen);
    Expr := PsExpression;
    WantTokenAndRead(TkRparen)
  end
  else if Lexer.Token.Id = TkNot then
  begin
    WantTokenAndRead(TkNot);
    Expr := ExUnaryOp(PsFactor, TkNot);
  end
  else
    CompileError('Invalid token in expression: ' + LxTokenStr);
  PsFactor := Expr
end;

function PsTerm : TExpression;
var 
  Op : TLxTokenId;
  Expr : TExpression;
begin
  Expr := PsFactor;
  while IsOpMultiplying(Lexer.Token) do
  begin
    Op := Lexer.Token.Id;
    ReadToken;
    Expr := ExBinaryOp(Expr, PsFactor, Op)
  end;
  PsTerm := Expr
end;

function PsSimpleExpression : TExpression;
var 
  Negative : boolean;
  Op : TLxTokenId;
  Expr : TExpression;
begin
  Negative := Lexer.Token.Id = TkMinus;
  if Negative then ReadToken;
  Expr := PsTerm;
  if Negative then Expr := ExUnaryOp(Expr, TkMinus);
  while IsOpAdding(Lexer.Token) do
  begin
    Op := Lexer.Token.Id;
    ReadToken;
    Expr := ExBinaryOp(Expr, PsTerm, Op)
  end;
  PsSimpleExpression := Expr
end;

function PsExpression;
var 
  Op : TLxTokenId;
  Expr : TExpression;
begin
  Expr := PsSimpleExpression;
  while IsOpRelational(Lexer.Token) do
  begin
    Op := Lexer.Token.Id;
    ReadToken;
    Expr := ExBinaryOp(Expr, PsSimpleExpression, Op)
  end;
  PsExpression := Expr
end;

procedure PsAssign(Lhs, Rhs : TExpression);
var ResultVarPtr : TPsVarPtr;
begin
  if Lhs^.Cls = XcFnRef then
  begin
    if Lhs^.FnPtr <> Defs.CurrentFn then
      CompileError('Cannot assign a value to a function');
    ResultVarPtr := FindNameOfClass('RESULT',
                    TncVariable, {Required=}true)^.VarPtr;
    DisposeExpr(Lhs);
    Lhs := ExVariable(ResultVarPtr);
  end;

  Rhs := ExCoerce(Rhs, Lhs^.TypePtr);
  if not Lhs^.IsAssignable or Lhs^.IsConstant then
  begin
    if Lhs^.IsFunctionResult then
      CompileError('Cannot assign to the result of a function')
    else
      CompileError('Cannot assign to a constant value');
  end;
  ExMarkInitialized(Lhs);
  OutAssign(Lhs, Rhs);
  DisposeExpr(Lhs);
  DisposeExpr(Rhs)
end;

procedure PsStatementSequence;
begin
  OutSequenceBegin;
  SkipToken(TkBegin);
  while Lexer.Token.Id <> TkEnd do
  begin
    PsStatement;
    WantToken2(TkSemicolon, TkEnd);
    SkipToken(TkSemicolon);
  end;
  OutSequenceEnd;
  SkipToken(TkEnd)
end;

procedure PsIdentifierStatement;
var 
  Lhs : TExpression;
begin
  Lhs := PsVariable;
  if (Lhs^.Cls <> XcFnRef)
     or (Lhs^.FnPtr <> Defs.CurrentFn)
     or (Lexer.Token.Id <> TkAssign) then
    Lhs := PsVariableOrFunctionExtension(Lhs);

  if Lexer.Token.Id = TkAssign then
  begin
    WantTokenAndRead(TkAssign);
    PsAssign(Lhs, PsExpression);
  end
  else
  begin
    if Lhs^.Cls = XcFnCall then
    begin
      OutProcedureCall(Lhs);
      DisposeExpr(Lhs)
    end
    else if Lhs^.Cls = XcPseudoFnCall then
    begin
      OutPseudoProcCall(Lhs);
      DisposeExpr(Lhs)
    end
    else if Lhs^.Cls = XcBinaryOp then
           CompileError('Invalid statement' +
                        ' (maybe you wrote ''='' instead of '':=''?)')
    else
      CompileError('Invalid statement')
  end
end;

procedure PsIfStatement;
var Cond : TExpression;
begin
  WantTokenAndRead(TkIf);
  Cond := ExCoerce(PsExpression, PrimitiveTypes.PtBoolean);
  OutIf(Cond);
  DisposeExpr(Cond);
  WantTokenAndRead(TkThen);
  if Lexer.Token.Id = TkElse then OutEmptyStatement
  else PsStatement;
  if Lexer.Token.Id = TkElse then
  begin
    WantTokenAndRead(TkElse);
    OutElse;
    PsStatement;
  end;
  OutIfEnd;
end;

procedure PsCaseStatement;
var 
  CasePtr : TExpression;
  CaseTypePtr : TPsTypePtr;
  CaseLabel : TExpression;
begin
  WantTokenAndRead(TkCase);
  CasePtr := PsExpression;
  CaseTypePtr := CasePtr^.TypePtr;
  if not IsOrdinalType(CaseTypePtr) then
    CompileError('The index of the case statement is not ordinal');
  OutCaseBegin(CasePtr);
  DisposeExpr(CasePtr);
  WantTokenAndRead(TkOf);
  repeat
    CaseLabel := ExCoerce(PsExpression, CaseTypePtr);
    if not CaseLabel^.IsConstant then
      CompileError('The label of the case statement is not constant');
    WantTokenAndRead(TkColon);
    OutCaseStatementBegin(CaseLabel);
    DisposeExpr(CaseLabel);
    PsStatement;
    OutCaseStatementEnd;
    WantToken3(TkSemicolon, TkElse, TkEnd);
    SkipToken(TkSemicolon);
  until (Lexer.Token.Id = TkElse) or (Lexer.Token.Id = TkEnd);
  OutCaseElseBegin;
  if Lexer.Token.Id = TkElse then
    repeat
      ReadToken;
      PsStatement;
      WantToken2(TkSemicolon, TkEnd);
    until Lexer.Token.Id = TkEnd;
  OutCaseElseEnd;
  OutCaseEnd;
  WantTokenAndRead(TkEnd);
end;

procedure PsRepeatStatement;
var Cond : TExpression;
begin
  WantTokenAndRead(TkRepeat);
  OutRepeatBegin;
  while Lexer.Token.Id <> TkUntil do
  begin
    PsStatement;
    WantToken2(TkSemicolon, TkUntil);
    SkipToken(TkSemicolon)
  end;
  WantTokenAndRead(TkUntil);
  Cond := ExCoerce(PsExpression, PrimitiveTypes.PtBoolean);
  OutRepeatEnd(Cond);
  DisposeExpr(Cond)
end;

procedure PsWhileStatement;
var Cond : TExpression;
begin
  WantTokenAndRead(TkWhile);
  Cond := ExCoerce(PsExpression, PrimitiveTypes.PtBoolean);
  OutWhileBegin(Cond);
  DisposeExpr(Cond);
  WantTokenAndRead(TkDo);
  PsStatement;
  OutWhileEnd
end;

procedure PsForStatement;
var 
  Iter, First, Last : TExpression;
  Ascending : boolean;
begin
  WantTokenAndRead(TkFor);
  Iter := PsExpression;
  if not Iter^.IsAssignable then
    CompileError('Iterator variable must be assignable');
  if Iter^.IsConstant then
    CompileError('Iterator must not be a constant');
  if Iter^.IsFunctionResult then
    CompileError('Iterator must not be the result of a function');
  if not IsOrdinalType(Iter^.TypePtr) then
    CompileError('Type of iterator is not ordinal: ' + TypeName(Iter^.TypePtr));
  if Iter^.Cls = XcVariable then
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
  OutForBegin(Iter, First, Last, Ascending);
  PsStatement;
  OutForEnd(Iter, Ascending);
  DisposeExpr(Iter);
  DisposeExpr(First);
  DisposeExpr(Last)
end;

procedure PsWithStatement;
var 
  Base : TExpression;
  VarPtr : TPsVarPtr;
begin
  WantToken(TkWith);
  StartTemporaryScope;
  OutSequenceBegin;
  repeat
    ReadToken;
    Base := PsExpression;
    VarPtr := AddWithVar(Base);
    OutAssignToReference(VarPtr, Base);
    DisposeExpr(Base);
    WantToken2(TkComma, TkDo)
  until Lexer.Token.Id = TkDo;
  WantTokenAndRead(TkDo);
  PsStatement;
  OutSequenceEnd;
  CloseTemporaryScope
end;

procedure PsStatement;
begin
  if Lexer.Token.Id = TkSemicolon then OutEmptyStatement
  else if Lexer.Token.Id = TkBegin then PsStatementSequence
  else if Lexer.Token.Id = TkIdentifier then PsIdentifierStatement
  else if Lexer.Token.Id = TkIf then PsIfStatement
  else if Lexer.Token.Id = TkCase then PsCaseStatement
  else if Lexer.Token.Id = TkRepeat then PsRepeatStatement
  else if Lexer.Token.Id = TkWhile then PsWhileStatement
  else if Lexer.Token.Id = TkFor then PsForStatement
  else if Lexer.Token.Id = TkWith then PsWithStatement
  else
    CompileError('Unexpected token ' + LxTokenStr)
end;

procedure PsProgramBlock;
begin
  PsDefinitions;
  WantTokenAndRead(TkBegin);
  OutProgramBegin;
  while Lexer.Token.Id <> TkEnd do
  begin
    PsStatement;
    WantToken2(TkSemicolon, TkEnd);
    SkipToken(TkSemicolon)
  end;
  OutProgramEnd;
  WantTokenAndRead(TkEnd)
end;

procedure ExecuteDirective(Dir : string);
begin
  if (length(Dir) > 3) and (Dir[2] = 'I') and (Dir[3] = ' ') then
    LxInclude(Copy(Dir, 4, 255))
  else if (length(Dir) = 3) and (Dir[2] = 'R') and
          ((Dir[3] = '-') or (Dir[3] = '+')) then
         CodegenSetCheckBounds(Dir[3] = '+')
end;

procedure ReadToken;
var Stop : boolean;
begin
  repeat
    LxReadToken;
    Stop := Lexer.Token.Id <> TkComment;
    if Lexer.Token.Id = TkComment then
      if (Length(Lexer.Token.Value) >= 2) and (Lexer.Token.Value[1] = '$') then
        ExecuteDirective(Lexer.Token.Value);
    if (Lexer.Token.Id = TkEof) and Lexer.Prev.Exists then
    begin
      Lexer.Input := Lexer.Prev.Input;
      Lexer.Prev.Exists := false;
      Stop := false
    end
  until Stop;
end;

procedure ParseProgram;
begin
  ReadToken;
  PsProgramHeading;
  PsProgramBlock;
  WantTokenAndRead(TkDot);
  WantToken(TkEof);
end;

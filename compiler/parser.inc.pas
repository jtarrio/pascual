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
  { It's hard to detect when an enumerated type was used, so we give up. }
  Typ.WasUsed := true;
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
      if not ExIsImmediate(CaseLabel) then
        CompileError('The label of the case statement is not immediate');
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
  if not ExIsImmediate(Expr) then
    CompileError('Expected an immediate expression');
  PsImmediate := Expr
end;

function PsArrayType : TPsTypePtr;
var 
  Typ : TPsType;
  Arr : TPsArrayDef;
  TypePtr : TPsTypePtr;
  ArrPtr : TPsArrayPtr;
begin
  Arr.IndexTypePtr := nil;
  Arr.ValueTypePtr := nil;
  ArrPtr := AddArray(Arr);
  Typ := TypeOfClass(TtcArray);
  Typ.ArrayPtr := ArrPtr;
  TypePtr := AddType(Typ);
  Result := TypePtr;

  WantTokenAndRead(TkArray);
  WantTokenAndRead(TkLbracket);
  repeat
    ArrPtr^.IndexTypePtr := PsTypeDenoter;
    ArrPtr^.IndexTypePtr^.WasUsed := true;
    if not IsBoundedType(ArrPtr^.IndexTypePtr) then
      CompileError('Array indices must belong to a bounded ordinal type');
    WantToken2(TkComma, TkRbracket);
    if Lexer.Token.Id = TkComma then
    begin
      TypePtr := AddType(Typ);
      ArrPtr^.ValueTypePtr := TypePtr;
      ArrPtr^.ValuetypePtr^.WasUsed := true;
      ArrPtr := AddArray(Arr);
      TypePtr^.ArrayPtr := ArrPtr
    end;
    SkipToken(TkComma)
  until Lexer.Token.Id = TkRbracket;
  WantTokenAndRead(TkRbracket);
  WantTokenAndRead(TkOf);
  ArrPtr^.ValueTypePtr := PsTypeDenoter
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
  else
  begin
    Typ := PointerType(NamePtr^.TypePtr);
    NamePtr^.TypePtr^.WasUsed := true
  end;
  ReadToken;
  PsPointerType := AddType(Typ)
end;

function PsRangeType : TPsTypePtr;
var 
  First, Last : TExpression;
  Typ : TPsType;
  Range : TPsRangeDef;
begin
  First := PsImmediate;
  WantTokenAndRead(TkRange);
  Last := PsImmediate;
  if not IsSameType(First^.TypePtr, Last^.TypePtr) then
    CompileError('The bounds of a subrange must belong to the same type');
  if not IsOrdinalType(First^.TypePtr) then
    CompileError('The bounds of a subrange must belong to an ordinal type');
  Range.First := ExGetOrdinal(First);
  Range.Last := ExGetOrdinal(Last);
  Range.BaseTypePtr := First^.TypePtr;
  DisposeExpr(First);
  DisposeExpr(Last);
  if Range.First > Range.Last then
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
  else if (Lexer.Token.Id = TkInteger) or (Lexer.Token.Id = TkString)
          or (Lexer.Token.Id = TkMinus) or (Lexer.Token.Id = TkPlus) then
         Result := PsRangeType;
  if Result = nil then
    CompileError('Expected type denoter, found ' + LxTokenStr);
end;

procedure _ResolvePointerUnknown(TypePtr : TPsTypePtr);
var 
  TargetPtr : TPsTypePtr;
begin
  if IsPointerUnknownType(TypePtr) then
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
  Checkpoint : TPsDefPtr;
begin
  Checkpoint := Defs.Latest;
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
  OutEnumValuesFromCheckpoint(Checkpoint)
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
                            TypePtr)));
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
  ResultPtr : TPsVarPtr;
begin
  StartLocalScope(FnPtr);
  Checkpoint := Defs.Latest;
  for Pos := 1 to FnPtr^.ArgCount do
    AddVariable(FnPtr^.Args[Pos]);
  OutFunctionDefinition(FnPtr);
  OutEnumValuesFromCheckpoint(Checkpoint);
  if FnPtr^.ReturnTypePtr <> nil then
  begin
    ResultPtr := AddVariable(MakeVariable('RESULT', FnPtr^.ReturnTypePtr));
    ResultPtr^.WasUsed := true;
    OutVariableDefinition(ResultPtr);
  end;
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
  IsConst : boolean;
  IsVar : boolean;
  LastArg, Arg : integer;
  TypePtr : TPsTypePtr;
begin
  WantTokenAndRead(TkLparen);
  Def.ArgCount := 0;
  repeat
    IsConst := Lexer.Token.Id = TkConst;
    IsVar := Lexer.Token.Id = TkVar;
    if IsConst then WantTokenAndRead(TkConst);
    if IsVar then WantTokenAndRead(TkVar);
    LastArg := Def.ArgCount;
    repeat
      Def.ArgCount := Def.ArgCount + 1;
      if Def.ArgCount > MaxFnArgs then
        CompileError('Too many arguments declared for function ' + Def.Name);
      Def.Args[Def.ArgCount].Name := GetTokenValueAndRead(TkIdentifier);
      Def.Args[Def.ArgCount].IsConstant := IsConst;
      Def.Args[Def.ArgCount].IsReference := IsVar or IsConst;
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
  else if Fn^.Cls = XcPseudoFnRef then PsFunctionCall := Pf_Parse(Fn)
end;

function PsArrayAccess(Arr : TExpression) : TExpression;
var 
  Idx : TExpression;
begin
  WantTokenAndRead(TkLbracket);
  repeat
    Idx := PsExpression;
    if IsStringyType(Arr^.TypePtr) then Arr := ExStringChar(Arr, Idx)
    else Arr := ExArrayAccess(Arr, Idx);
    WantToken2(TkComma, TkRbracket);
    if (Lexer.Token.Id = TkComma) and not IsArrayType(Arr^.TypePtr)
       and not IsStringyType(Arr^.TypePtr) then
      CompileError('Array element is not an array or string: ' +
                   DescribeExpr(Arr, 10) +
      '; it has type ' + TypeName(Arr^.TypePtr));
    SkipToken(TkComma)
  until Lexer.Token.Id = TkRbracket;
  WantTokenAndRead(TkRbracket);
  Result := Arr
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
  if (Expr^.Cls = XcVariable)
     and ((Lexer.Token.Id = TkDot)
     or (Lexer.Token.Id = TkLbracket)
     or (Lexer.Token.Id = TkCaret)) then
    Expr^.VarPtr^.WasUsed := true;
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
      if LxIsDigit(Ch) then State := NumCharDec
      else if Ch = '$' then
      begin
        State := NumCharHex;
        Pos := Pos + 1
      end
    end
    else if State = NumCharDec then
    begin
      if LxIsDigit(Ch) then
      begin
        Pos := Pos + 1;
        ChNum := ChNum * 10 + Ord(Ch) - 48
      end
      else State := NumCharReady
    end
    else if State = NumCharHex then
    begin
      if LxIsHexDigit(Ch) then
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
      if (Ch >= '@') and (Ch <= '_') then
        Result := Result + Chr(Ord(Ch) - 64)
      else if (Ch >= 'a') and (Ch <= 'z') then
             Result := Result + Chr(Ord(Ch) - 96)
      else State := Error
    end
    else if State = Error then
           CompileError('Invalid character in string: ' + Pstr)
  end;
  if (State = QuotedStr) or (State = Caret) then
    CompileError('String is not terminated: ' + Pstr);
  if (State = NumCharDec) or (State = NumCharHex) then
    Result := Result + Chr(ChNum);
end;

function ParseInt(Pstr : string) : integer;
var Code : integer;
begin
  Val(Pstr, Result, Code);
  if Code <> 0 then CompileError('Could not parse integer: ' + Pstr)
end;

function ParseReal(Pstr : string) : real;
var Code : integer;
begin
  Val(Pstr, Result, Code);
  if Code <> 0 then CompileError('Could not parse real number: ' + Pstr)
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
  else if Lexer.Token.Id = TkInteger then
         Expr := ExIntegerConstant(ParseInt(GetTokenValueAndRead(TkInteger)))
  else if Lexer.Token.Id = TkReal then
         Expr := ExRealConstant(ParseReal(GetTokenValueAndRead(TkReal)))
  else if Lexer.Token.Id = TkIdentifier then
  begin
    Expr := PsVariable;
    if Expr^.Cls = XcVariable then Expr^.VarPtr^.WasUsed := true;
    Expr := PsVariableOrFunctionExtension(Expr)
  end
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
  if Negative then ReadToken
  else SkipToken(TkPlus);
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
  if not Lhs^.IsAssignable then
  begin
    if Lhs^.IsFunctionResult then
      CompileError('Cannot assign to the result of a function: ' +
                   DescribeExpr(Lhs, 5))
    else
      CompileError('Cannot assign to a constant:' + DescribeExpr(Lhs, 5));
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
  OrigLhs : TExpression;
  UsesTmpVars : boolean;
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
    OrigLhs := Lhs;
    UsesTmpVars := false;
    while Lhs^.Cls = XcSetTmpVar do
    begin
      if not UsesTmpVars then
      begin
        UsesTmpVars := true;
        StartTemporaryScope;
        OutSequenceBegin
      end;
      OutDeclareAndAssign(Lhs^.TmpVar^.VarPtr, Lhs^.TmpVarValue);
      Lhs := Lhs^.TmpVarChild
    end;
    if Lhs^.Cls = XcFnCall then OutProcedureCall(Lhs)
    else if Lhs^.Cls = XcPseudoFnCall then OutPseudoProcCall(Lhs)
    else if Lhs^.Cls = XcBinaryOp then
           CompileError('Invalid statement' +
                        ' (maybe you wrote ''='' instead of '':=''?)')
    else CompileError('Invalid statement');
    DisposeExpr(OrigLhs);
    if UsesTmpVars then
    begin
      OutSequenceEnd;
      CloseTemporaryScope
    end
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
    if not ExIsImmediate(CaseLabel) then
      CompileError('The label of the case statement is not immediate');
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
    OutDeclareAndAssign(VarPtr, Base);
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

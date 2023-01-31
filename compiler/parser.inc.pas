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
    ', or ' + LxTokenName(Id3) + ' found ' + LxTokenStr)
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

function PsTypeDenoter : TPsTypeIndex;
forward;

function PsTypeIdentifier : TPsTypeIndex;
begin
  WantToken(TkIdentifier);
  PsTypeIdentifier := FindNameOfClass(Lexer.Token.Value,
                      TncType, {Required=}true)^.TypeIndex;
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

function PsEnumeratedType : TPsTypeIndex;
var 
  Typ : TPsType;
  Enum : TPsEnumDef;
begin
  WantTokenAndRead(TkLparen);
  Enum.Size := 0;
  Enum.HasBeenDefined := false;
  repeat
    Enum.Size := Enum.Size + 1;
    if Enum.Size > MaxEnumValues then
      CompileError('Too many values in enum');
    Enum.Values[Enum.Size] := GetTokenValueAndRead(TkIdentifier);
    WantToken2(TkComma, TkRparen);
    SkipToken(TkComma);
  until Lexer.Token.Id = TkRparen;
  Typ := EmptyType;
  Typ.Cls := TtcEnum;
  Typ.EnumIndex := AddEnum(Enum);
  PsEnumeratedType := AddType(Typ);
  SkipToken(TkRparen)
end;

procedure PsRecordField(var Rec : TPsRecordDef; Delimiter : TLxTokenId);
var 
  Name : string;
  LastField, Field : integer;
  TypeIndex : TPsTypeIndex;
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
  TypeIndex := PsTypeDenoter;
  for Field := LastField + 1 to Rec.Size do
    Rec.Fields[Field].TypeIndex := TypeIndex;
  WantToken2(TkSemicolon, Delimiter);
  SkipToken(TkSemicolon);
end;

procedure PsRecordVariants(var Rec : TPsRecordDef);
var 
  Tag : TPsIdentifier;
  TagType : TPsTypeIndex;
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
    Rec.Fields[Rec.Size].TypeIndex := TagType;
  end
  else
    TagType := FindNameOfClass(Tag.Name, TncType, {Required=}true)^.TypeIndex;
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

function PsRecordType : TPsTypeIndex;
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
  Typ.RecordIndex := AddRecord(Rec);
  PsRecordType := AddType(Typ);
end;

function PsImmediate : TExpression;
var Expr : TExpression;
begin
  Expr := PsExpression;
  if Expr^.cls <> XcImmediate then
    CompileError('Expected an immediate value');
  PsImmediate := Expr
end;

function PsArrayType : TPsTypeIndex;
var 
  Typ : TPsType;
  Arr : TPsArrayDef;
begin
  WantTokenAndRead(TkArray);
  WantTokenAndRead(TkLbracket);
  Arr.LowBound := PsImmediate;
  WantTokenAndRead(TkRange);
  Arr.HighBound := PsImmediate;
  WantTokenAndRead(TkRbracket);
  WantTokenAndRead(TkOf);
  Arr.TypeIndex := PsTypeDenoter;
  Typ := TypeOfClass(TtcArray);
  Typ.ArrayIndex := AddArray(Arr);
  PsArrayType := AddType(Typ)
end;

function PsPointerType : TPsTypeIndex;
var 
  Typ : TPsType;
  NameIndex : TPsNameIndex;
begin
  WantTokenAndRead(TkCaret);
  WantToken(TkIdentifier);
  NameIndex := FindNameOfClass(Lexer.Token.Value, TncType, {Required=}false);
  if NameIndex = nil then Typ := PointerUnknownType(Lexer.Token.Value)
  else Typ := PointerType(NameIndex^.TypeIndex);
  ReadToken;
  PsPointerType := AddType(Typ)
end;

function PsTypeDenoter;
var 
  TypeIndex : TPsTypeIndex;
begin
  TypeIndex := nil;
  if Lexer.Token.Id = TkIdentifier then TypeIndex := PsTypeIdentifier
  else if Lexer.Token.Id = TkLparen then TypeIndex := PsEnumeratedType
  else if Lexer.Token.Id = TkRecord then TypeIndex := PsRecordType
  else if Lexer.Token.Id = TkArray then TypeIndex := PsArrayType
  else if Lexer.Token.Id = TkCaret then TypeIndex := PsPointerType
  else
    CompileError('Wanted type definition, found ' + LxTokenStr);
  PsTypeDenoter := TypeIndex;
end;

procedure _ResolvePointerUnknown(TypeIndex : TPsTypeIndex);
var 
  TargetIndex : TPsTypeIndex;
begin
  if TypeIndex^.Cls = TtcPointerUnknown then
  begin
    TargetIndex := FindNameOfClass(TypeIndex^.TargetName^,
                   TncType, {Required=}true)^.TypeIndex;
    dispose(TypeIndex^.TargetName);
    TypeIndex^.Cls := TtcPointer;
    TypeIndex^.PointedTypeIndex := TargetIndex
  end
end;

procedure PsTypeDefinitions;
var 
  Name : string;
  TypeIndex : TPsTypeIndex;
  NewType : TPsType;
  Checkpoint : TPsDefPtr;
begin
  Checkpoint := Defs.Latest;
  WantTokenAndRead(TkType);
  repeat
    Name := GetTokenValueAndRead(TkIdentifier);
    WantTokenAndRead(TkEquals);
    TypeIndex := PsTypeDenoter;
    NewType := CopyType(TypeIndex);
    NewType.Name := Name;
    NewType.AliasFor := TypeIndex;
    TypeIndex := AddType(NewType);
    WantTokenAndRead(TkSemicolon)
  until Lexer.Token.Id <> TkIdentifier;
  OutTypeDefinitionsFromCheckpoint(Checkpoint);
  OutEnumValuesFromCheckpoint(Checkpoint)
end;

procedure PsConstant(Name : string);
var 
  Constant : TPsConstant;
begin
  WantTokenAndRead(TkEquals);
  Constant.Name := Name;
  if (Lexer.Token.Id = TkFalse) or (Lexer.Token.Id = TkTrue) or
     (Lexer.Token.Id = TkNumber) or (Lexer.Token.Id = TkString) then
    Constant.Replacement := Lexer.Token
  else
    CompileError('Expected constant value, found ' + LxTokenStr);
  AddConstant(Constant);
  ReadToken;
end;

procedure PsConstantValue(TypeIndex : TPsTypeIndex);
var 
  Expr : TExpression;
begin
  if IsArrayType(TypeIndex) then
  begin
    WantTokenAndRead(TkLparen);
    TypeIndex := TypeIndex^.ArrayIndex^.TypeIndex;
    OutConstantArrayBegin;
    while Lexer.Token.Id <> TkRparen do
    begin
      PsConstantValue(TypeIndex);
      WantToken2(TkComma, TkRparen);
      if Lexer.Token.Id = TkComma then OutConstantArraySeparator;
      SkipToken(TkComma)
    end;
    OutConstantArrayEnd;
    WantTokenAndRead(TkRparen);
  end
  else
  begin
    Expr := ExCoerce(PsImmediate, TypeIndex);
    OutExpression(Expr);
    DisposeExpr(Expr)
  end
end;

procedure PsTypedConstant(Name : string);
var 
  TypeIndex : TPsTypeIndex;
begin
  WantTokenAndRead(TkColon);
  TypeIndex := PsTypeDenoter;
  WantTokenAndRead(TkEquals);
  OutConstantDefinitionBegin(AddVariable(MakeTypedConstant(Name, TypeIndex)));
  PsConstantValue(TypeIndex);
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
  TypeIndex : TPsTypeIndex;
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
    TypeIndex := PsTypeDenoter;
    WantTokenAndRead(TkSemicolon);
    for NumNames := 1 to NumNames do
      OutVariableDefinition(AddVariable(MakeVariable(Names[NumNames],
                            TypeIndex, false)));
  until Lexer.Token.Id <> TkIdentifier;
  OutEnumValuesFromCheckpoint(Checkpoint)
end;

procedure PsStatement;
forward;

procedure PsDefinitions;
forward;

procedure PsFunctionBody(FnIndex : TPsFunctionIndex);
var 
  Pos : integer;
  Checkpoint : TPsDefPtr;
begin
  StartLocalScope;
  Checkpoint := Defs.Latest;
  for Pos := 1 to FnIndex^.ArgCount do
    AddVariable(FnIndex^.Args[Pos]);
  OutFunctionDefinition(FnIndex);
  OutEnumValuesFromCheckpoint(Checkpoint);
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
  OutFunctionEnd(FnIndex);
  CloseLocalScope
end;

procedure PsArguments(var Def : TPsFunction);
var 
  IsReference : boolean;
  LastArg, Arg : integer;
  TypeIndex : TPsTypeIndex;
begin
  WantTokenAndRead(TkLparen);
  Def.ArgCount := 0;
  repeat
    IsReference := Lexer.Token.Id = TkVar;
    SkipToken(TkVar);
    LastArg := Def.ArgCount;
    repeat
      Def.ArgCount := Def.ArgCount + 1;
      if Def.ArgCount > MaxFunctionArguments then
        CompileError('Too many arguments declared for function ' + Def.Name);
      Def.Args[Def.ArgCount].Name := GetTokenValueAndRead(TkIdentifier);
      Def.Args[Def.ArgCount].IsReference := IsReference;
      Def.Args[Def.ArgCount].IsConstant := false;
      WantToken2(TkColon, TkComma);
      SkipToken(TkComma)
    until Lexer.Token.Id = TkColon;
    SkipToken(TkColon);
    TypeIndex := PsTypeIdentifier;
    for Arg := LastArg + 1 to Def.ArgCount do
      Def.Args[Arg].TypeIndex := TypeIndex;
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

function PsResultType : TPsTypeIndex;
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
    Def.ReturnTypeIndex := nil
  else
  begin
    WantToken2(TkLparen, TkColon);
    if Lexer.Token.Id = TkLparen then PsArguments(Def);
    WantTokenAndRead(TkColon);
    Def.ReturnTypeIndex := PsResultType;
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

function PsFunctionCall(Fn : TExpression) : TExpression;
var 
  Args : TExFunctionArgs;
begin
  WantTokenAndRead(TkLParen);
  Args.Size := 0;
  while Lexer.Token.Id <> TkRParen do
  begin
    Args.Size := Args.Size + 1;
    Args.Values[Args.Size] := PsExpression;
    WantToken2(TkComma, TkRParen);
    SkipToken(TkComma)
  end;
  WantTokenAndRead(TkRparen);
  PsFunctionCall := ExFunctionCall(Fn, Args)
end;

function PsPointerDeref(Ptr : TExpression) : TExpression;
begin
  WantTokenAndRead(TkCaret);
  PsPointerDeref := ExPointerAccess(Ptr)
end;

procedure PsRead(Fn : TPsSpecialFunction);
var 
  Src : TExpression;
  LineFeed : boolean;
  OutVar : TExpression;
begin
  LineFeed := Fn = TsfReadln;
  Src := ExVariable(FindNameOfClass('INPUT',
         TncVariable, {Required=}true)^.VariableIndex);
  if Lexer.Token.Id <> TkLparen then
  begin
    if LineFeed then OutReadln(Src);
  end
  else
  begin
    OutBegin;
    WantTokenAndRead(TkLparen);
    if Lexer.Token.Id <> TkRparen then
    begin
      OutVar := PsExpression;
      if OutVar^.IsAssignable and IsTextType(OutVar^.TypeIndex) then
      begin
        DisposeExpr(Src);
        Src := OutVar
      end
      else
      begin
        if not OutVar^.IsAssignable
           or not IsStringyType(OutVar^.TypeIndex) then
          CompileError('Invalid expression for read argument');
        OutRead(Src, OutVar);
        DisposeExpr(OutVar);
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      while Lexer.Token.Id <> TkRparen do
      begin
        OutVar := PsExpression;
        if not OutVar^.IsAssignable
           or not IsStringyType(OutVar^.TypeIndex) then
          CompileError('Invalid expression for read argument');
        OutRead(Src, OutVar);
        DisposeExpr(OutVar);
        WantToken2(TkComma, TkRparen);
        SkipToken(TkComma)
      end;
    end;
    WantTokenAndRead(TkRparen);
    if LineFeed then
      OutReadln(Src);
    DisposeExpr(Src);
    OutEnd;
  end
end;

procedure PsWrite(Fn : TPsSpecialFunction);
var 
  Dst : TExpression;
  LineFeed : boolean;
  Expr : TExpression;
begin
  LineFeed := Fn = TsfWriteln;
  Dst := ExVariable(FindNameOfClass('OUTPUT',
         TncVariable, {Required=}true)^.VariableIndex);
  if Lexer.Token.Id <> TkLparen then
  begin
    if LineFeed then OutWriteln(Dst);
  end
  else
  begin
    OutBegin;
    WantTokenAndRead(TkLparen);
    if Lexer.Token.Id <> TkRparen then
    begin
      Expr := PsExpression;
      if Expr^.IsAssignable and IsTextType(Expr^.TypeIndex) then
      begin
        DisposeExpr(Dst);
        Dst := Expr
      end
      else
      begin
        OutWrite(Dst, Expr);
        DisposeExpr(Expr);
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      while Lexer.Token.Id <> TkRParen do
      begin
        Expr := PsExpression;
        OutWrite(Dst, Expr);
        DisposeExpr(Expr);
        WantToken2(TkComma, TkRParen);
        SkipToken(TkComma)
      end;
    end;
    WantTokenAndRead(TkRparen);
    if LineFeed then
      OutWriteln(Dst);
    DisposeExpr(Dst);
    OutEnd
  end
end;

procedure PsStr;
var 
  Expr, Dest : TExpression;
begin
  WantTokenAndRead(TkLparen);
  Expr := PsExpression;
  WantTokenAndRead(TkComma);
  Dest := PsExpression;
  if not Dest^.IsAssignable or not IsStringType(Dest^.TypeIndex) then
    CompileError('Destination argument is not a string variable');
  WantTokenAndRead(TkRparen);
  OutStr(Dest, Expr);
  DisposeExpr(Dest);
  DisposeExpr(Expr)
end;

procedure PsNew;
var 
  Dest : TExpression;
begin
  WantTokenAndRead(TkLparen);
  Dest := PsExpression;
  WantTokenAndRead(TkRparen);
  if not Dest^.IsAssignable or not IsPointerType(Dest^.TypeIndex) then
    CompileError('Argument is not a pointer');
  OutNew(Dest);
  DisposeExpr(Dest)
end;

procedure PsDispose;
var 
  Dest : TExpression;
begin
  WantTokenAndRead(TkLparen);
  Dest := PsExpression;
  WantTokenAndRead(TkRparen);
  if not Dest^.IsAssignable or not IsPointerType(Dest^.TypeIndex) then
    CompileError('Argument is not a pointer');
  OutDispose(Dest)
end;

function PsArrayAccess(Arr : TExpression) : TExpression;
var 
  Idx : TExpression;
begin
  WantTokenAndRead(TkLbracket);
  Idx := PsExpression;
  WantTokenAndRead(TkRbracket);
  Arr := ExEnsureEvaluation(Arr);
  if IsStringyType(Arr^.TypeIndex) then
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
                   FindField(Rec^.TypeIndex, Fld.Name, {Required=}true))
end;

function PsVariableOrFunctionCall : TExpression;
var 
  Id : TPsIdentifier;
  WithVarIndex : TPsWithVarIndex;
  Found : TPsName;
  Expr : TExpression;
  Done : boolean;
begin
  Done := false;
  Id := PsIdentifier;
  WithVarIndex := FindWithVar(Id.Name);
  if WithVarIndex <> nil then
  begin
    Expr := ExVariable(WithVarIndex^.VariableIndex);
    Expr := ExFieldAccess(Expr,
            FindField(Expr^.TypeIndex, Id.Name, {Required=}true))
  end
  else
  begin
    Found := FindName(Id.Name, {Required=}true)^;
    if Found.Cls = TncVariable then
      Expr := ExVariable(Found.VariableIndex)
    else if Found.Cls = TncFunction then
           Expr := ExFunctionRef(Found.FunctionIndex)
    else if Found.Cls = TncEnumValue then
           Expr := ExEnumConstant(Found.Ordinal, Found.EnumTypeIndex)
    else if Found.Cls = TncSpecialFunction then
    begin
      Expr := ExNothing;
      if (Found.SpecialFunction = TsfRead)
         or (Found.SpecialFunction = TsfReadln) then
        PsRead(Found.SpecialFunction)
      else if (Found.SpecialFunction = TsfWrite)
              or (Found.SpecialFunction = TsfWriteln) then
             PsWrite(Found.SpecialFunction)
      else if Found.SpecialFunction = TsfStr then PsStr
      else if Found.SpecialFunction = TsfNew then PsNew
      else if Found.SpecialFunction = TsfDispose then PsDispose
    end
    else
      CompileError('Invalid identifier: ' + Id.Name)
  end;
  repeat
    if Lexer.Token.Id = TkDot then Expr := PsFieldAccess(Expr)
    else if Lexer.Token.Id = TkLbracket then Expr := PsArrayAccess(Expr)
    else if Lexer.Token.Id = TkLparen then Expr := PsFunctionCall(Expr)
    else if Lexer.Token.Id = TkCaret then Expr := PsPointerDeref(Expr)
    else Done := true
  until Done;
  PsVariableOrFunctionCall := Expr
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
  else if (Lexer.Token.Id = TkFalse) or (Lexer.Token.Id = TkTrue) then
  begin
    Expr := ExBooleanConstant(Lexer.Token.Id = TkTrue);
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
  else if Lexer.Token.Id = TkIdentifier then Expr := PsVariableOrFunctionCall
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
begin
  if Lhs^.Cls = XcFunctionRef then
    Rhs := ExCoerce(Rhs, Lhs^.FunctionEx.FunctionIndex^.ReturnTypeIndex)
  else
    Rhs := ExCoerce(Rhs, Lhs^.TypeIndex);
  if Lhs^.Cls = XcFunctionRef then
    OutAssignReturnValue(Lhs, Rhs)
  else
  begin
    if not Lhs^.IsAssignable or Lhs^.IsConstant then
    begin
      if Lhs^.IsFunctionResult then
        CompileError('Cannot assign to the result of a function')
      else
        CompileError('Cannot assign to a constant value');
    end;
    OutAssign(Lhs, Rhs)
  end;
  DisposeExpr(Lhs);
  DisposeExpr(Rhs)
end;

procedure PsStatementSequence;
begin
  OutBegin;
  SkipToken(TkBegin);
  while Lexer.Token.Id <> TkEnd do
  begin
    PsStatement;
    WantToken2(TkSemicolon, TkEnd);
    SkipToken(TkSemicolon);
  end;
  OutEnd;
  SkipToken(TkEnd)
end;

procedure PsIdentifierStatement;
var 
  Lhs : TExpression;
begin
  Lhs := PsExpression;
  if Lexer.Token.Id = TkAssign then
  begin
    WantTokenAndRead(TkAssign);
    PsAssign(Lhs, ExEnsureEvaluation(PsExpression));
  end
  else
  begin
    Lhs := ExEnsureEvaluation(Lhs);
    if Lhs^.Cls = XcFunctionCall then
    begin
      OutProcedureCall(Lhs);
      DisposeExpr(Lhs)
    end
    else if Lhs^.Cls <> XcNothing then
           if Lhs^.Cls = XcBinaryOp then
             CompileError('Invalid statement' +
                          ' (maybe you wrote ''='' instead of '':=''?')
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
  end
end;

procedure PsCaseStatement;
var 
  CaseIndex : TExpression;
  CaseTypeIndex : TPsTypeIndex;
  CaseLabel : TExpression;
begin
  WantTokenAndRead(TkCase);
  CaseIndex := PsExpression;
  CaseTypeIndex := CaseIndex^.TypeIndex;
  if not IsOrdinalType(CaseTypeIndex) then
    CompileError('The index of the case statement is not ordinal');
  OutCaseBegin(CaseIndex);
  DisposeExpr(CaseIndex);
  WantTokenAndRead(TkOf);
  repeat
    CaseLabel := ExCoerce(PsExpression, CaseTypeIndex);
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
  Iter := ExEnsureEvaluation(PsExpression);
  if not Iter^.IsAssignable then
    CompileError('Iterator variable must be assignable');
  if Iter^.IsConstant then
    CompileError('Iterator must not be a constant');
  if Iter^.IsFunctionResult then
    CompileError('Iterator must not be the result of a function');
  if not IsOrdinalType(Iter^.TypeIndex) then
    CompileError('Type of iterator is not ordinal: ' +
                 TypeName(Iter^.TypeIndex));
  WantTokenAndRead(TkAssign);
  First := ExEnsureEvaluation(PsExpression);
  WantToken2(TkTo, TkDownto);
  Ascending := Lexer.Token.Id = TkTo;
  ReadToken;
  Last := ExEnsureEvaluation(PsExpression);
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
  VarIndex : TPsVariableIndex;
begin
  WantToken(TkWith);
  StartTemporaryScope;
  OutBegin;
  repeat
    ReadToken;
    Base := PsExpression;
    VarIndex := AddWithVar(Base);
    OutAssignToReference(VarIndex, Base);
    DisposeExpr(Base);
    WantToken2(TkComma, TkDo)
  until Lexer.Token.Id = TkDo;
  WantTokenAndRead(TkDo);
  PsStatement;
  OutEnd;
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
end;

procedure ReadToken;
var 
  ConstIndex : TPsConstantIndex;
  TokenPos : TLxPos;
  Stop : boolean;
begin
  repeat
    LxReadToken;
    Stop := Lexer.Token.Id <> TkComment;
    if Lexer.Token.Id = TkIdentifier then
    begin
      ConstIndex := FindConstant(Lexer.Token.Value);
      if ConstIndex <> nil then
      begin
        TokenPos := Lexer.Token.Pos;
        Lexer.Token := ConstIndex^.Replacement;
        Lexer.Token.Pos := TokenPos
      end
    end;
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

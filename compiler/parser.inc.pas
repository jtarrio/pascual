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

function PsFindType(Name : string) : TPsTypeIndex;
var 
  Found : TPsName;
begin
  Found := Defs.Names[FindName(Name, {Required=}true)];
  if Found.Cls <> TncType then
    CompileError('Not a type: ' + Found.Name);
  PsFindType := Found.TypeIndex;
end;

function PsTypeIdentifier : TPsTypeIndex;
begin
  WantToken(TkIdentifier);
  PsTypeIdentifier := PsFindType(Lexer.Token.Value);
  ReadToken
end;

function PsIdentifier : TPsIdentifier;
var 
  Ident : TPsIdentifier;
begin
  Ident.Name := GetTokenValueAndRead(TkIdentifier);
  PsIdentifier := Ident
end;

function PsExpression : TPsExpression;
forward;

function PsEnumeratedType : TPsTypeIndex;
var 
  Typ : TPsType;
  Enum : TPsEnumDef;
begin
  WantTokenAndRead(TkLparen);
  Enum.Size := 0;
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
  CaseLabel : TPsExpression;
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
    TagType := PsFindType(Tag.Name);
  if not IsOrdinalType(TagType) then
    CompileError('The index of the case statement is not ordinal');
  WantTokenAndRead(TkOf);
  repeat
    Rec.NumVariants := Rec.NumVariants + 1;
    Rec.VariantBounds[Rec.NumVariants] := Rec.Size + 1;
    repeat
      CaseLabel := ExprCoerce(PsExpression, TagType);
      if not CaseLabel.IsConstant then
        CompileError('The label of the case statement is not constant');
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
  while (Lexer.Token.Id <> TkCase) and (Lexer.Token.Id <> TkEnd) do
    PsRecordField(Rec, TkEnd);
  if Lexer.Token.Id = TkCase then
    PsRecordVariants(Rec);
  WantTokenAndRead(TkEnd);
  Typ := TypeOfClass(TtcRecord);
  Typ.RecordIndex := AddRecord(Rec);
  PsRecordType := AddType(Typ);
end;

function PsArrayType : TPsTypeIndex;
var 
  Typ : TPsType;
  Arr : TPsArrayDef;
begin
  WantTokenAndRead(TkArray);
  WantTokenAndRead(TkLbracket);
  Arr.LowBound := GetTokenValueAndRead(TkNumber);
  WantTokenAndRead(TkRange);
  Arr.HighBound := GetTokenValueAndRead(TkNumber);
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
  TypeIndex : TPsTypeIndex;
  NameIndex : TPsNameIndex;
begin
  WantTokenAndRead(TkCaret);
  WantToken(TkIdentifier);
  NameIndex := FindName(Lexer.Token.Value, {Required=}false);
  if NameIndex = 0 then
  begin
    Typ := PlaceholderType;
    Typ.Name := Lexer.Token.Value;
    TypeIndex := AddType(Typ)
  end
  else if Defs.Names[NameIndex].Cls = TncType then
         TypeIndex := Defs.Names[NameIndex].TypeIndex
  else
    CompileError('Not a type: ' + Lexer.Token.Value);
  ReadToken;
  Typ := PointerType(TypeIndex);
  PsPointerType := AddType(Typ)
end;

function PsTypeDenoter;
var 
  TypeIndex : TPsTypeIndex;
begin
  TypeIndex := 0;
  if Lexer.Token.Id = TkIdentifier then TypeIndex := PsTypeIdentifier
  else if Lexer.Token.Id = TkLparen then TypeIndex := PsEnumeratedType
  else if Lexer.Token.Id = TkRecord then TypeIndex := PsRecordType
  else if Lexer.Token.Id = TkArray then TypeIndex := PsArrayType
  else if Lexer.Token.Id = TkCaret then TypeIndex := PsPointerType
  else
    CompileError('Wanted type definition, found ' + LxTokenStr);
  PsTypeDenoter := TypeIndex;
end;

procedure PsTypeDefinitions;
var 
  Name : string;
  TypeIndex : TPsTypeIndex;
  NewType : TPsType;
  TypeBase : TPsTypeIndex;
  EnumBase : TPsEnumIndex;
begin
  TypeBase := Defs.Bounds.Types;
  EnumBase := Defs.Bounds.Enums;
  WantTokenAndRead(TkType);
  repeat
    Name := GetTokenValueAndRead(TkIdentifier);
    WantTokenAndRead(TkEquals);
    TypeIndex := PsTypeDenoter;
    NewType := Defs.Types[TypeIndex];
    NewType.Name := Name;
    NewType.AliasFor := TypeIndex;
    TypeIndex := AddType(NewType);
    WantTokenAndRead(TkSemicolon)
  until Lexer.Token.Id <> TkIdentifier;
  for TypeIndex := TypeBase + 1 to Defs.Bounds.Types do
  begin
    if IsPlaceholderType(TypeIndex) then
      CompileError('Type has been mentioned but not defined: ' +
                   TypeName(TypeIndex));
    if Defs.Types[TypeIndex].AliasFor <> 0 then
      OutTypeDefinition(TypeIndex)
  end;
  OutEnumValuesFromBase(EnumBase)
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
  Expr : TPsExpression;
begin
  if IsBooleanType(TypeIndex) then
  begin
    WantToken2(TkFalse, TkTrue);
    Expr := ExprMakeBooleanConstant(Lexer.Token.Id = TkTrue);
    ReadToken;
    OutConstantValue(Expr.Value)
  end
  else if IsIntegerType(TypeIndex) then
  begin
    Expr := ExprMakeNumberConstant(GetTokenValueAndRead(TkNumber));
    OutConstantValue(Expr.Value)
  end
  else if IsCharType(TypeIndex) then
  begin
    Expr := ExprMakeStringConstant(GetTokenValueAndRead(TkString));
    if not IsCharType(Expr.TypeIndex) then
      CompileError('Expected char constant, got ' + TypeName(Expr.TypeIndex));
    OutConstantValue(Expr.Value)
  end
  else if IsStringType(TypeIndex) then
  begin
    Expr := ExprMakeStringConstant(GetTokenValueAndRead(TkString));
    OutConstantValue(Expr.Value)
  end
  else if IsArrayType(TypeIndex) then
  begin
    WantTokenAndRead(TkLparen);
    TypeIndex := Defs.Arrays[Defs.Types[TypeIndex].ArrayIndex].TypeIndex;
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
    CompileError('Invalid type for constant: ' + TypeName(TypeIndex))
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
  EnumBase : TPsEnumIndex;
begin
  EnumBase := Defs.Bounds.Enums;
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
  OutEnumValuesFromBase(EnumBase)
end;

procedure PsStatement;
forward;

procedure PsDefinitions;
forward;

procedure PsFunctionBody(FnIndex : TPsFunctionIndex);
var 
  Pos : integer;
begin
  StartLocalScope;
  for Pos := 1 to Defs.Functions[FnIndex].ArgCount do
    AddVariable(Defs.Functions[FnIndex].Args[Pos]);
  OutFunctionDefinition(FnIndex);
  OutEnumValuesFromBase(Defs.ScopeBase.Enums);
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
    Def.ReturnTypeIndex := 0
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

function PsFunctionCall(Fn : TPsExpression) : TPsExpression;
var 
  Fun : TPsFunction;
  Expr : TPsExpression;
  ArgNum : integer;
begin
  if Fn.Cls <> TecFunction then CompileError('Not a function');
  Fun := Defs.Functions[Fn.FunctionIndex];
  Fn.Value := GenFunctionCallStart(Fn.Value);
  WantTokenAndRead(TkLparen);
  for ArgNum := 1 to Fun.ArgCount do
  begin
    if ArgNum <> 1 then WantTokenAndRead(TkComma);
    Expr := ExprCoerce(PsExpression, Fun.Args[ArgNum].TypeIndex);
    Fn.Value := GenFunctionCallArgument(Fn.Value, Expr,
                Fun.Args[ArgNum].IsReference, ArgNum)
  end;
  WantTokenAndRead(TkRparen);
  Fn.Value := GenFunctionCallEnd(Fn.Value);
  Fn.Cls := TecValue;
  Fn.TypeIndex := Fun.ReturnTypeIndex;
  Fn.IsConstant := true;
  PsFunctionCall := Fn
end;

function PsPointerDeref(Ptr : TPsExpression) : TPsExpression;
begin
  Ptr := ExprEvaluate(Ptr);
  if (Ptr.Cls <> TecValue) or not IsPointerType(Ptr.TypeIndex) then
    CompileError('Not a pointer');
  WantTokenAndRead(TkCaret);
  Ptr.Value := '*(' + Ptr.Value + ')';
  Ptr.TypeIndex := Defs.Types[Ptr.TypeIndex].PointedTypeIndex;
  PsPointerDeref := Ptr
end;

procedure PsRead(Fn : TPsSpecialFunction);
var 
  Src : string;
  LineFeed : boolean;
  OutVar : TPsExpression;
begin
  LineFeed := Fn = TsfReadln;
  Src := 'INPUT';
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
      if ExprIsVariable(OutVar) and IsTextType(OutVar.TypeIndex) then
        Src := OutVar.Value
      else
      begin
        if not ExprIsVariable(OutVar)
           or not IsStringyType(OutVar.TypeIndex) then
          CompileError('Invalid expression for read argument');
        OutRead(Src, OutVar);
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      while Lexer.Token.Id <> TkRparen do
      begin
        OutVar := PsExpression;
        if not ExprIsVariable(OutVar)
           or not IsStringyType(OutVar.TypeIndex) then
          CompileError('Invalid expression for read argument');
        OutRead(Src, OutVar);
        WantToken2(TkComma, TkRparen);
        SkipToken(TkComma)
      end;
    end;
    WantTokenAndRead(TkRparen);
    if LineFeed then
      OutReadln(Src);
    OutEnd
  end
end;

procedure PsWrite(Fn : TPsSpecialFunction);
var 
  Dst : string;
  LineFeed : boolean;
  Expr : TPsExpression;
begin
  LineFeed := Fn = TsfWriteln;
  Dst := 'OUTPUT';
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
      if ExprIsVariable(Expr) and IsTextType(Expr.TypeIndex) then
        Dst := Expr.Value
      else
        OutWrite(Dst, Expr);
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      while Lexer.Token.Id <> TkRParen do
      begin
        OutWrite(Dst, PsExpression);
        WantToken2(TkComma, TkRParen);
        SkipToken(TkComma)
      end;
    end;
    WantTokenAndRead(TkRparen);
    if LineFeed then
      OutWriteln(Dst);
    OutEnd
  end
end;

procedure PsStr;
var 
  Expr, Dest : TPsExpression;
begin
  WantTokenAndRead(TkLparen);
  Expr := PsExpression;
  WantTokenAndRead(TkComma);
  Dest := PsExpression;
  if not ExprIsVariable(Dest) or not IsStringType(Dest.TypeIndex) then
    CompileError('Destination argument is not a string variable');
  WantTokenAndRead(TkRparen);
  OutStr(Dest.Value, Expr)
end;

procedure PsNew;
var 
  Dest : TPsExpression;
begin
  WantTokenAndRead(TkLparen);
  Dest := PsExpression;
  WantTokenAndRead(TkRparen);
  if not ExprIsVariable(Dest) or not IsPointerType(Dest.TypeIndex) then
    CompileError('Argument is not a pointer');
  OutNew(Dest)
end;

procedure PsDispose;
var 
  Dest : TPsExpression;
begin
  WantTokenAndRead(TkLparen);
  Dest := PsExpression;
  WantTokenAndRead(TkRparen);
  if not ExprIsVariable(Dest) or not IsPointerType(Dest.TypeIndex) then
    CompileError('Argument is not a pointer');
  OutDispose(Dest)
end;

function PsArrayAccess(Arr : TPsExpression) : TPsExpression;
var 
  Idx : TPsExpression;
begin
  WantTokenAndRead(TkLbracket);
  Idx := PsExpression;
  WantTokenAndRead(TkRbracket);
  if (Idx.Cls <> TecValue) or not IsIntegerType(Idx.TypeIndex) then
    CompileError('Subscript must be an integer');
  if (Arr.Cls = TecValue) and IsStringType(Arr.TypeIndex) then
  begin
    SetStringIndex(Arr, Idx);
    Arr.TypeIndex := PrimitiveTypes.PtChar
  end
  else if (Arr.Cls = TecValue) and IsArrayType(Arr.TypeIndex) then
  begin
    SetArrayIndex(Arr, Idx);
    Arr.TypeIndex := Defs.Arrays[Defs.Types[Arr.TypeIndex].ArrayIndex].TypeIndex
  end
  else
    CompileError('Not a string or array');
  PsArrayAccess := Arr
end;

function PsFieldAccess(Rec : TPsExpression) : TPsExpression;
var 
  Fld : TPsIdentifier;
  FldType : TPsTypeIndex;
begin
  if (Rec.Cls <> TecValue) or (Defs.Types[Rec.TypeIndex].Cls <> TtcRecord) then
    CompileError('Not a record');
  WantTokenAndRead(TkDot);
  Fld := PsIdentifier;
  FldType := FindFieldType(Rec.TypeIndex, Fld.Name, true);
  if FldType = 0 then
    CompileError('Field ' + Fld.Name + ' not found in record');
  SetFieldAccess(Rec, Fld.Name);
  Rec.TypeIndex := FldType;
  PsFieldAccess := Rec
end;

function PsVariableOrFunctionCall : TPsExpression;
var 
  Id : TPsIdentifier;
  WithVarIndex : TPsWithVarIndex;
  Found : TPsName;
  Expr : TPsExpression;
  Done : boolean;
begin
  Done := false;
  Expr.Value := '';
  Expr.IsConstant := false;
  Id := PsIdentifier;
  WithVarIndex := FindWithVar(Id.Name);
  if WithVarIndex <> 0 then
  begin
    Expr := Defs.WithVars[WithVarIndex].Expr;
    SetFieldAccess(Expr, Id.Name);
    Expr.TypeIndex := FindFieldType(Expr.TypeIndex, Id.Name, true)
  end
  else
  begin
    Found := Defs.Names[FindName(Id.Name, {Required=}true)];
    if Found.Cls = TncVariable then
    begin
      if Defs.Variables[Found.VariableIndex].IsReference then
        Expr.Value := '*' + Id.Name
      else
        Expr.Value := Id.Name;
      Expr.Cls := TecValue;
      Expr.TypeIndex := Defs.Variables[Found.VariableIndex].TypeIndex;
    end
    else if Found.Cls = TncFunction then
    begin
      Expr.Value := Id.Name;
      Expr.Cls := TecFunction;
      Expr.FunctionIndex := Found.FunctionIndex
    end
    else if Found.Cls = TncEnumValue then
    begin
      Expr.Value := Id.Name;
      Expr.Cls := TecValue;
      Expr.TypeIndex := Found.EnumTypeIndex;
      Expr.IsConstant := true
    end
    else if Found.Cls = TncSpecialFunction then
    begin
      Expr.Cls := TecStatement;
      Expr.TypeIndex := 0;
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

function PsFactor : TPsExpression;
var 
  Expr : TPsExpression;
begin
  if Lexer.Token.Id = TkNil then
  begin
    Expr := ExprMakeNilConstant;
    ReadToken
  end
  else if (Lexer.Token.Id = TkFalse) or (Lexer.Token.Id = TkTrue) then
  begin
    Expr := ExprMakeBooleanConstant(Lexer.Token.Id = TkTrue);
    ReadToken
  end
  else if Lexer.Token.Id = TkString then
         Expr := ExprMakeStringConstant(GetTokenValueAndRead(TkString))
  else if Lexer.Token.Id = TkNumber then
         Expr := ExprMakeNumberConstant(GetTokenValueAndRead(TkNumber))
  else if Lexer.Token.Id = TkIdentifier then Expr := PsVariableOrFunctionCall
  else if Lexer.Token.Id = TkLparen then
  begin
    WantTokenAndRead(TkLparen);
    Expr := GenParens(PsExpression);
    WantTokenAndRead(TkRparen)
  end
  else if Lexer.Token.Id = TkNot then
  begin
    WantTokenAndRead(TkNot);
    Expr := ExprUnaryOp(TkNot, PsFactor);
  end
  else
    CompileError('Invalid token in expression: ' + LxTokenStr);
  PsFactor := Expr
end;

function PsTerm : TPsExpression;
var 
  Op : TLxTokenId;
  Expr : TPsExpression;
begin
  Expr := PsFactor;
  while IsOpMultiplying(Lexer.Token) do
  begin
    Op := Lexer.Token.Id;
    ReadToken;
    Expr := ExprBinaryOp(Expr, Op, PsFactor)
  end;
  PsTerm := Expr
end;

function PsSimpleExpression : TPsExpression;
var 
  Negative : boolean;
  Op : TLxTokenId;
  Expr : TPsExpression;
begin
  Negative := Lexer.Token.Id = TkMinus;
  if Negative then ReadToken;
  Expr := PsTerm;
  if Negative then Expr := ExprUnaryOp(TkMinus, Expr);
  while IsOpAdding(Lexer.Token) do
  begin
    Op := Lexer.Token.Id;
    ReadToken;
    Expr := ExprBinaryOp(Expr, Op, PsTerm)
  end;
  PsSimpleExpression := Expr
end;

function PsExpression : TPsExpression;
var 
  Op : TLxTokenId;
  Expr : TPsExpression;
begin
  Expr := PsSimpleExpression;
  while IsOpRelational(Lexer.Token) do
  begin
    Op := Lexer.Token.Id;
    ReadToken;
    Expr := ExprBinaryOp(Expr, Op, PsSimpleExpression)
  end;
  PsExpression := Expr
end;

procedure PsAssign(Lhs, Rhs : TPsExpression);
begin
  if Lhs.IsConstant then
    CompileError('Cannot assign to a constant value');
  if Lhs.Cls = TecFunction then
    OutAssignReturnValue(Lhs, ExprCoerce(Rhs,
                         Defs.Functions[Lhs.FunctionIndex].ReturnTypeIndex))
  else if Lhs.Cls = TecValue then
         OutAssign(Lhs, ExprCoerce(Rhs, Lhs.TypeIndex))
  else
    CompileError('Cannot assign to result of statement')
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
  Lhs : TPsExpression;
begin
  Lhs := PsExpression;
  if Lexer.Token.Id = TkAssign then
  begin
    WantTokenAndRead(TkAssign);
    PsAssign(Lhs, PsExpression);
  end
  else if Lhs.Cls <> TecStatement then
  begin
    if Lhs.Cls = TecFunction then Lhs := ExprEvaluate(Lhs);
    OutProcedureCall(Lhs)
  end
end;

procedure PsIfStatement;
begin
  WantTokenAndRead(TkIf);
  OutIf(ExprCoerce(PsExpression, PrimitiveTypes.PtBoolean));
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
  CaseIndex : TPsExpression;
  CaseLabel : TPsExpression;
begin
  WantTokenAndRead(TkCase);
  CaseIndex := PsExpression;
  if not IsOrdinalType(CaseIndex.TypeIndex) then
    CompileError('The index of the case statement is not ordinal');
  OutCaseBegin(CaseIndex);
  WantTokenAndRead(TkOf);
  repeat
    CaseLabel := ExprCoerce(PsExpression, CaseIndex.TypeIndex);
    if not CaseLabel.IsConstant then
      CompileError('The label of the case statement is not constant');
    WantTokenAndRead(TkColon);
    OutCaseStatementBegin(CaseLabel);
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
  OutRepeatEnd(ExprCoerce(PsExpression, PrimitiveTypes.PtBoolean));
end;

procedure PsWhileStatement;
begin
  WantTokenAndRead(TkWhile);
  OutWhileBegin(ExprCoerce(PsExpression, PrimitiveTypes.PtBoolean));
  WantTokenAndRead(TkDo);
  PsStatement;
  OutWhileEnd
end;

procedure PsForStatement;
var 
  Iter, First, Last : TPsExpression;
  Ascending : boolean;
begin
  WantTokenAndRead(TkFor);
  Iter := PsExpression;
  if not ExprIsVariable(Iter) then
    CompileError('Expected variable');
  if not IsOrdinalType(Iter.TypeIndex) then
    CompileError('Type of iterator is not ordinal: ' +
                 TypeName(Iter.TypeIndex));
  WantTokenAndRead(TkAssign);
  First := ExprEvaluate(PsExpression);
  WantToken2(TkTo, TkDownto);
  Ascending := Lexer.Token.Id = TkTo;
  ReadToken;
  Last := ExprEvaluate(PsExpression);
  WantTokenAndRead(TkDo);
  OutForBegin(Iter, First, Last, Ascending);
  PsStatement;
  OutForEnd(Iter, Ascending)
end;

procedure PsWithStatement;
var 
  WithVarsBase : TPsWithVarIndex;
begin
  WantToken(TkWith);
  WithVarsBase := Defs.Bounds.WithVars;
  repeat
    ReadToken;
    AddWithVar(PsExpression);
    WantToken2(TkComma, TkDo)
  until Lexer.Token.Id = TkDo;
  WantTokenAndRead(TkDo);
  PsStatement;
  Defs.Bounds.WithVars := WithVarsBase
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
      if ConstIndex <> 0 then
      begin
        TokenPos := Lexer.Token.Pos;
        Lexer.Token := Defs.Constants[ConstIndex].Replacement;
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

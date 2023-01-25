procedure ReadToken;
forward;

procedure WantToken(Id : TLxTokenId);
begin
  if Lexer.Token.Id <> Id then
  begin
    writeln(StdErr, 'Wanted token ', Id, ', found ', LxTokenStr, LxWhereStr);
    halt(1)
  end
end;

procedure WantToken2(Id1 : TLxTokenId; Id2 : TLxTokenId);
begin
  if (Lexer.Token.Id <> Id1) and (Lexer.Token.Id <> Id2) then
  begin
    writeln(StdErr, 'Wanted token ', Id1, ' or ', Id2, ', found ', LxTokenStr,
            LxWhereStr);
    halt(1)
  end
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

function PsTypeDenoter(Scope : TPsScope) : TPsTypeIndex;
forward;

function PsTypeIdentifier : TPsTypeIndex;
var 
  Found : TPsName;
begin
  WantToken(TkIdentifier);
  Found := Defs.Names[FindName(Lexer.Token.Value, true)];
  if Found.Cls <> TncType then
  begin
    writeln(StdErr, 'Not a type: ', Found.Name, LxWhereStr);
    halt(1)
  end;
  PsTypeIdentifier := Found.TypeIndex;
  ReadToken
end;

function PsEnumeratedType(Scope : TPsScope) : TPsTypeIndex;
var 
  Typ : TPsType;
  Enum : TPsEnumDef;
begin
  WantTokenAndRead(TkLparen);
  Enum.Size := 0;
  repeat
    Enum.Size := Enum.Size + 1;
    if Enum.Size > MaxEnumValues then
    begin
      writeln(StdErr, 'Too many values in enum', LxWhereStr);
      halt(1)
    end;
    Enum.Values[Enum.Size] := GetTokenValueAndRead(TkIdentifier);
    WantToken2(TkComma, TkRparen);
    SkipToken(TkComma);
  until Lexer.Token.Id = TkRparen;
  Typ := EmptyType;
  Typ.Cls := TtcEnum;
  Typ.EnumIndex := AddEnum(Enum);
  PsEnumeratedType := AddType(Typ, Scope);
  SkipToken(TkRparen)
end;

function PsRecordType(Scope : TPsScope) : TPsTypeIndex;
var 
  Typ : TPsType;
  LastField, Field : integer;
  TypeIndex : TPsTypeIndex;
  Rec : TPsRecordDef;
begin
  WantTokenAndRead(TkRecord);
  Rec.Size := 0;
  repeat
    LastField := Rec.Size;
    repeat
      Rec.Size := Rec.Size + 1;
      if Rec.Size > MaxRecordFields then
      begin
        writeln(StdErr, 'Too many fields in record', LxWhereStr);
        halt(1)
      end;
      Rec.Fields[Rec.Size].Name := GetTokenValueAndRead(TkIdentifier);
      WantToken2(TkComma, TkColon);
      SkipToken(TkComma)
    until Lexer.Token.Id = TkColon;
    WantTokenAndRead(TkColon);
    TypeIndex := PsTypeDenoter(Scope);
    for Field := LastField + 1 to Rec.Size do
      Rec.Fields[Field].TypeIndex := TypeIndex;
    WantToken2(TkSemicolon, TkEnd);
    SkipToken(TkSemicolon);
  until Lexer.Token.Id = TkEnd;
  Typ := TypeOfClass(TtcRecord);
  Typ.RecordIndex := AddRecord(Rec);
  PsRecordType := AddType(Typ, Scope);
  SkipToken(TkEnd)
end;

function PsArrayType(Scope : TPsScope) : TPsTypeIndex;
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
  Arr.TypeIndex := PsTypeDenoter(Scope);
  Typ := TypeOfClass(TtcArray);
  Typ.ArrayIndex := AddArray(Arr);
  PsArrayType := AddType(Typ, Scope)
end;

function PsPointerType(Scope : TPsScope) : TPsTypeIndex;
var 
  Typ : TPsType;
  TypeIndex : TPsTypeIndex;
  NameIndex : TPsNameIndex;
begin
  WantTokenAndRead(TkCaret);
  WantToken(TkIdentifier);
  NameIndex := FindName(Lexer.Token.Value, false);
  if NameIndex = 0 then
  begin
    Typ := PlaceholderType;
    Typ.Name := Lexer.Token.Value;
    TypeIndex := AddType(Typ, Scope)
  end
  else if Defs.Names[NameIndex].Cls = TncType then
         TypeIndex := Defs.Names[NameIndex].TypeIndex
  else
  begin
    writeln(StdErr, 'Not a type: ', Lexer.Token.Value, LxWhereStr);
    halt(1)
  end;
  ReadToken;
  Typ := PointerType(TypeIndex);
  PsPointerType := AddType(Typ, Scope)
end;

function PsTypeDenoter;
var 
  TypeIndex : TPsTypeIndex;
begin
  TypeIndex := 0;
  if Lexer.Token.Id = TkIdentifier then TypeIndex := PsTypeIdentifier
  else if Lexer.Token.Id = TkLparen then TypeIndex := PsEnumeratedType(Scope)
  else if Lexer.Token.Id = TkRecord then TypeIndex := PsRecordType(Scope)
  else if Lexer.Token.Id = TkArray then TypeIndex := PsArrayType(Scope)
  else if Lexer.Token.Id = TkCaret then TypeIndex := PsPointerType(Scope)
  else
  begin
    writeln(StdErr, 'Wanted type definition, found ', LxTokenStr, LxWhereStr);
    halt(1)
  end;
  PsTypeDenoter := TypeIndex;
end;

procedure PsTypeDefinitions(Scope : TPsScope);
var 
  Name : string;
  TypeIndex : TPsTypeIndex;
  NewType : TPsType;
  PreviousScope : TPsScope;
begin
  PreviousScope := GetCurrentScope;
  WantTokenAndRead(TkType);
  repeat
    Name := GetTokenValueAndRead(TkIdentifier);
    WantTokenAndRead(TkEquals);
    TypeIndex := PsTypeDenoter(Scope);
    NewType := Defs.Types[TypeIndex];
    NewType.Name := Name;
    NewType.AliasFor := TypeIndex;
    TypeIndex := AddType(NewType, Scope);
    WantTokenAndRead(TkSemicolon)
  until Lexer.Token.Id <> TkIdentifier;
  for TypeIndex := PreviousScope.NumTypes + 1 to Defs.Scope.NumTypes do
  begin
    if IsPlaceholderType(TypeIndex) then
    begin
      writeln(StdErr, 'Type has been mentioned but not defined: ',
              TypeName(TypeIndex), LxWhereStr);
      halt(1)
    end;
    if Defs.Types[TypeIndex].AliasFor <> 0 then
      OutTypeDefinition(TypeIndex)
  end;
  OutEnumValuesInScope(PreviousScope)
end;

procedure PsConstant(Name : string; Scope : TPsScope);
var 
  Constant : TPsConstant;
begin
  WantTokenAndRead(TkEquals);
  Constant.Name := Name;
  if (Lexer.Token.Id = TkFalse) or (Lexer.Token.Id = TkTrue) or
     (Lexer.Token.Id = TkNumber) or (Lexer.Token.Id = TkString) then
    Constant.Replacement := Lexer.Token
  else
  begin
    writeln(Stderr, 'Expected constant value, found ', LxTokenStr, LxWhereStr);
    halt(1)
  end;
  AddConstant(Constant, Scope);
  ReadToken;
end;

procedure PsConstantValue(TypeIndex : TPsTypeIndex);
var 
  Expr : TPsExpression;
begin
  if IsBooleanType(TypeIndex) then
  begin
    WantToken2(TkFalse, TkTrue);
    Expr := GenBooleanConstant(Lexer.Token.Id = TkTrue);
    ReadToken;
    OutConstantValue(Expr.Value)
  end
  else if IsIntegerType(TypeIndex) then
  begin
    Expr := GenNumberConstant(GetTokenValueAndRead(TkNumber));
    OutConstantValue(Expr.Value)
  end
  else if IsCharType(TypeIndex) then
  begin
    Expr := GenStringConstant(GetTokenValueAndRead(TkString));
    if not IsCharType(Expr.TypeIndex) then
    begin
      writeln(StdErr, 'Expected char constant, got ',
              TypeName(Expr.TypeIndex), LxWhereStr);
      halt(1)
    end;
    OutConstantValue(Expr.Value)
  end
  else if IsStringType(TypeIndex) then
  begin
    Expr := GenStringConstant(GetTokenValueAndRead(TkString));
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
  begin
    writeln(StdErr, 'Invalid type for constant: ', TypeName(TypeIndex),
    LxWhereStr);
    halt(1)
  end
end;

procedure PsTypedConstant(Name : string; Scope : TPsScope);
var 
  TypeIndex : TPsTypeIndex;
begin
  WantTokenAndRead(TkColon);
  TypeIndex := PsTypeDenoter(Scope);
  WantTokenAndRead(TkEquals);
  OutConstantDefinitionBegin(AddVariable(MakeTypedConstant(Name, TypeIndex),
  Scope));
  PsConstantValue(TypeIndex);
  OutConstantDefinitionEnd
end;

procedure PsConstDefinitions(Scope : TPsScope);
var 
  Name : string;
begin
  WantTokenAndRead(TkConst);
  repeat
    Name := GetTokenValueAndRead(TkIdentifier);
    WantToken2(TkEquals, TkColon);
    if Lexer.Token.Id = TkEquals then
      PsConstant(Name, Scope)
    else
      PsTypedConstant(Name, Scope);
    WantTokenAndRead(TkSemicolon)
  until Lexer.Token.Id <> TkIdentifier;
end;

procedure PsVarDefinitions(Scope : TPsScope);
const 
  MaxVarNames = 8;
var 
  NumNames : integer;
  Names : array[1..MaxVarNames] of string;
  TypeIndex : TPsTypeIndex;
  PreviousScope : TPsScope;
begin
  PreviousScope := GetCurrentScope;
  WantTokenAndRead(TkVar);
  repeat
    NumNames := 0;
    repeat
      NumNames := NumNames + 1;
      if NumNames > MaxVarNames then
      begin
        writeln(StdErr, 'Too many names in variable definition', LxWhereStr);
        halt(1)
      end;
      Names[NumNames] := GetTokenValueAndRead(TkIdentifier);
      WantToken2(TkComma, TkColon);
      SkipToken(TkComma)
    until Lexer.Token.Id = TkColon;
    WantTokenAndRead(TkColon);
    TypeIndex := PsTypeDenoter(Scope);
    WantTokenAndRead(TkSemicolon);
    for NumNames := 1 to NumNames do
      OutVariableDefinition(AddVariable(MakeVariable(Names[NumNames],
                            TypeIndex, false), Scope));
  until Lexer.Token.Id <> TkIdentifier;
  OutEnumValuesInScope(PreviousScope)
end;

procedure PsStatement;
forward;

procedure PsDefinitions(Scope : TPsScope);
forward;

procedure PsFunctionBody(FnIndex : TPsFunctionIndex);
var 
  PreviousScope : TPsScope;
  Pos : integer;
begin
  PreviousScope := GetCurrentScope;
  for Pos := 1 to Defs.Functions[FnIndex].ArgCount do
    AddVariable(Defs.Functions[FnIndex].Args[Pos], PreviousScope);
  OutFunctionDefinition(FnIndex);
  OutEnumValuesInScope(PreviousScope);
  PsDefinitions(PreviousScope);
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
  SetCurrentScope(PreviousScope);
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
      begin
        writeln(StdErr, 'Too many arguments declared for function ', Def.Name,
                LxWhereStr);
        halt(1)
      end;
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
    if Lexer.Token.Id = TkType then PsTypeDefinitions(Scope)
    else if Lexer.Token.Id = TkConst then PsConstDefinitions(Scope)
    else if Lexer.Token.Id = TkVar then PsVarDefinitions(Scope)
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

function PsExpression : TPsExpression;
forward;

function PsIdentifier : TPsIdentifier;
var 
  Ident : TPsIdentifier;
begin
  Ident.Name := GetTokenValueAndRead(TkIdentifier);
  PsIdentifier := Ident
end;

function PsFunctionCall(Fn : TPsExpression) : TPsExpression;
var 
  Fun : TPsFunction;
  Expr : TPsExpression;
  ArgNum : integer;
begin
  if Fn.Cls <> TecFunction then
  begin
    writeln(StdErr, 'Not a function', LxWhereStr);
    halt(1)
  end;
  Fun := Defs.Functions[Fn.FunctionIndex];
  Fn.Value := GenFunctionCallStart(Fn.Value);
  WantTokenAndRead(TkLparen);
  for ArgNum := 1 to Fun.ArgCount do
  begin
    if ArgNum <> 1 then WantTokenAndRead(TkComma);
    Expr := CoerceType(PsExpression, Fun.Args[ArgNum].TypeIndex);
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
  Ptr := Evaluate(Ptr);
  if (Ptr.Cls <> TecValue) or not IsPointerType(Ptr.TypeIndex) then
  begin
    writeln(StdErr, 'Not a pointer', LxWhereStr);
    halt(1)
  end;
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
      if IsVariableExpression(OutVar) and IsTextType(OutVar.TypeIndex) then
        Src := OutVar.Value
      else
      begin
        if not IsVariableExpression(OutVar)
           or not IsStringyType(OutVar.TypeIndex) then
        begin
          writeln(StdErr, 'Invalid expression for read argument', LxWhereStr);
          halt(1)
        end;
        OutRead(Src, OutVar);
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      while Lexer.Token.Id <> TkRparen do
      begin
        OutVar := PsExpression;
        if not IsVariableExpression(OutVar)
           or not IsStringyType(OutVar.TypeIndex) then
        begin
          writeln(StdErr, 'Invalid expression for read argument', LxWhereStr);
          halt(1)
        end;
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
      if IsVariableExpression(Expr) and IsTextType(Expr.TypeIndex) then
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
  if not IsVariableExpression(Dest) or not IsStringType(Dest.TypeIndex) then
  begin
    writeln(StdErr, 'Destination argument is not a string variable',
            LxWhereStr);
    halt(1)
  end;
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
  if not IsVariableExpression(Dest) or not IsPointerType(Dest.TypeIndex) then
  begin
    writeln(StdErr, 'Argument is not a pointer', LxWhereStr);
    halt(1)
  end;
  OutNew(Dest)
end;

procedure PsDispose;
var 
  Dest : TPsExpression;
begin
  WantTokenAndRead(TkLparen);
  Dest := PsExpression;
  WantTokenAndRead(TkRparen);
  if not IsVariableExpression(Dest) or not IsPointerType(Dest.TypeIndex) then
  begin
    writeln(StdErr, 'Argument is not a pointer', LxWhereStr);
    halt(1)
  end;
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
  begin
    writeln(StdErr, 'Subscript must be an integer', LxWhereStr);
    halt(1)
  end;
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
  begin
    writeln(StdErr, 'Not a string or array', LxWhereStr);
    halt(1)
  end;
  PsArrayAccess := Arr
end;

function PsFieldAccess(Rec : TPsExpression) : TPsExpression;
var 
  Fld : TPsIdentifier;
  FldType : TPsTypeIndex;
begin
  if (Rec.Cls <> TecValue) or (Defs.Types[Rec.TypeIndex].Cls <> TtcRecord) then
  begin
    writeln(StdErr, 'Not a record', LxWhereStr);
    halt(1)
  end;
  WantTokenAndRead(TkDot);
  Fld := PsIdentifier;
  FldType := FindFieldType(Rec.TypeIndex, Fld.Name);
  if FldType = 0 then
  begin
    writeln(StdErr, 'Field ', Fld.Name, ' not found in record', LxWhereStr);
    halt(1)
  end;
  SetFieldAccess(Rec, Fld.Name);
  Rec.TypeIndex := FldType;
  PsFieldAccess := Rec
end;

function PsVariableOrFunctionCall : TPsExpression;
var 
  Id : TPsIdentifier;
  Found : TPsName;
  Expr : TPsExpression;
  Done : boolean;
begin
  Done := false;
  Expr.Value := '';
  Expr.IsConstant := false;
  Id := PsIdentifier;
  Found := Defs.Names[FindName(Id.Name, true)];
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
    Expr.TypeIndex := Found.TypeIndex;
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
  begin
    writeln(StdErr, 'Invalid identifier: ', Id.Name, LxWhereStr);
    halt(1)
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
    Expr := GenNilConstant;
    ReadToken
  end
  else if (Lexer.Token.Id = TkFalse) or (Lexer.Token.Id = TkTrue) then
  begin
    Expr := GenBooleanConstant(Lexer.Token.Id = TkTrue);
    ReadToken
  end
  else if Lexer.Token.Id = TkString then
         Expr := GenStringConstant(GetTokenValueAndRead(TkString))
  else if Lexer.Token.Id = TkNumber then
         Expr := GenNumberConstant(GetTokenValueAndRead(TkNumber))
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
    Expr := UnaryExpression(TkNot, PsFactor);
  end
  else
  begin
    writeln(StdErr, 'Invalid token in expression: ', LxTokenStr, LxWhereStr);
    halt(1)
  end;
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
    Expr := BinaryExpression(Expr, Op, PsFactor)
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
  if Negative then Expr := UnaryExpression(TkMinus, Expr);
  while IsOpAdding(Lexer.Token) do
  begin
    Op := Lexer.Token.Id;
    ReadToken;
    Expr := BinaryExpression(Expr, Op, PsTerm)
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
    Expr := BinaryExpression(Expr, Op, PsSimpleExpression)
  end;
  PsExpression := Expr
end;

procedure PsAssign(Lhs, Rhs : TPsExpression);
begin
  if Lhs.IsConstant then
  begin
    writeln(StdErr, 'Cannot assign to a constant value', LxWhereStr);
    halt(1)
  end;
  if Lhs.Cls = TecFunction then
    OutAssignReturnValue(Lhs, CoerceType(Rhs,
                         Defs.Functions[Lhs.FunctionIndex].ReturnTypeIndex))
  else if Lhs.Cls = TecValue then
         OutAssign(Lhs, CoerceType(Rhs, Lhs.TypeIndex))
  else
  begin
    writeln(StdErr, 'Cannot assign to result of statement', LxWhereStr);
    halt(1)
  end
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
    if Lhs.Cls = TecFunction then Lhs := Evaluate(Lhs);
    OutProcedureCall(Lhs)
  end
end;

procedure PsIfStatement;
begin
  WantTokenAndRead(TkIf);
  OutIf(CoerceType(PsExpression, PrimitiveTypes.PtBoolean));
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
  begin
    writeln(StdErr, 'The index of the case statement is not ordinal', LxWhereStr);
    halt(1)
  end;
  OutCaseBegin(CaseIndex);
  WantTokenAndRead(TkOf);
  repeat
    CaseLabel := CoerceType(PsExpression, CaseIndex.TypeIndex);
    if not CaseLabel.IsConstant then
    begin
      writeln(StdErr, 'The label of the case statement is not constant', LxWhereStr);
      halt(1)
    end;
    WantTokenAndRead(TkColon);
    OutCaseStatementBegin(CaseLabel);
    PsStatement;
    OutCaseStatementEnd;
    WantToken2(TkSemicolon, TkEnd);
    SkipToken(TkSemicolon);
  until Lexer.Token.Id = TkEnd;
  WantTokenAndRead(TkEnd);
  OutCaseEnd;
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
  OutRepeatEnd(CoerceType(PsExpression, PrimitiveTypes.PtBoolean));
end;

procedure PsWhileStatement;
begin
  WantTokenAndRead(TkWhile);
  OutWhileBegin(CoerceType(PsExpression, PrimitiveTypes.PtBoolean));
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
  if not IsVariableExpression(Iter) then
  begin
    writeln(StdErr, 'Expected variable', LxWhereStr);
    halt(1)
  end;
  if not IsOrdinalType(Iter.TypeIndex) then
  begin
    writeln(StdErr, 'Type of iterator is not ordinal: ',
            TypeName(Iter.TypeIndex), LxWhereStr);
    halt(1)
  end;
  WantTokenAndRead(TkAssign);
  First := Evaluate(PsExpression);
  WantToken2(TkTo, TkDownto);
  Ascending := Lexer.Token.Id = TkTo;
  ReadToken;
  Last := Evaluate(PsExpression);
  WantTokenAndRead(TkDo);
  OutForBegin(Iter, First, Last, Ascending);
  PsStatement;
  OutForEnd(Iter, Ascending)
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
  else
  begin
    writeln(StdErr, 'Unexpected token ', LxTokenStr, LxWhereStr);
    halt(1)
  end
end;

procedure PsProgramBlock;
begin
  PsDefinitions(GlobalScope);
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

function _ModStrings_Concat_Parse(FnExpr : TExpression) : TExpression;
var Operand : TExpression;
begin
  ExDispose(FnExpr);
  Result := nil;
  WantTokenAndRead(TkLparen);
  repeat
    Operand := PSExpression;
    EnsureStringyExpr(Operand);
    if Result = nil then Result := Operand
    else Result := ExBinaryOp(Result, Operand, TkPlus);
    WantToken2(TkComma, TkRparen);
    SkipToken(TkComma)
  until Lexer.Token.Id = TkRparen;
  WantTokenAndRead(TkRparen)
end;

function _ModStrings_Str_Parse(FnExpr : TExpression) : TExpression;
var 
  Src, Dest : TExpression;
  Width, Prec : TExpression;
begin
  Width := nil;
  Prec := nil;
  WantTokenAndRead(TkLparen);
  Src := ExOutrange(PsExpression);
  if Lexer.Token.Id = TkColon then
  begin
    WantTokenAndRead(TkColon);
    Width := PsExpression;
    EnsureIntegerExpr(Width);
    if IsRealType(Src^.TypePtr) and (Lexer.Token.Id = TkColon) then
    begin
      WantTokenAndRead(TkColon);
      Prec := PsExpression;
      EnsureIntegerExpr(Prec)
    end
  end;
  WantTokenAndRead(TkComma);
  Dest := PsExpression;
  WantTokenAndRead(TkRparen);
  EnsureAssignableExpr(Dest);
  EnsureStringExpr(Dest);
  if not IsBooleanType(Src^.TypePtr) and not IsIntegerType(Src^.TypePtr)
     and not IsRealType(Src^.TypePtr) and not IsEnumType(Src^.TypePtr) then
    ErrorForExpr('Invalid type for source of STR', Src);
  ExMarkInitialized(Dest);
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := Src;
  Result^.PseudoFnCall.Arg2 := Dest;
  Result^.PseudoFnCall.Arg3 := Width;
  Result^.PseudoFnCall.Arg4 := Prec
end;

function _ModStrings_Str_Describe(Expr : TExpression) : string;
begin
  with Expr^.PseudoFnCall do
    if Arg3 = nil then
      Result := 'STR(' + ExDescribe(Arg1) + ', ' + ExDescribe(Arg2) + ')'
    else if Arg4 = nil then
           Result := 'STR(' + ExDescribe(Arg1) + ':' +
                     ExDescribe(Arg3) + ', ' +
                     ExDescribe(Arg2) + ')'
    else Result := 'STR(' + ExDescribe(Arg1) + ':' +
                   ExDescribe(Arg3) + ':' +
                   ExDescribe(Arg4) + ', ' +
                   ExDescribe(Arg2) + ')'
end;

function _ModStrings_Val_Parse(FnExpr : TExpression) : TExpression;
var 
  Src, Dest, Code : TExpression;
begin
  WantTokenAndRead(TkLparen);
  Src := ExOutrange(PsExpression);
  WantTokenAndRead(TkComma);
  Dest := PsExpression;
  WantTokenAndRead(TkComma);
  Code := PsExpression;
  WantTokenAndRead(TkRparen);
  EnsureStringExpr(Src);
  EnsureAssignableExpr(Dest);
  if not IsBooleanType(Dest^.TypePtr) and not IsIntegerType(Dest^.TypePtr)
     and not IsRealType(Dest^.TypePtr) and not IsEnumType(Dest^.TypePtr) then
    ErrorForExpr('Invalid type for target of VAL', Dest);
  EnsureAssignableExpr(Code);
  EnsureIntegerExpr(Code);
  ExMarkInitialized(Dest);
  ExMarkInitialized(Code);
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := Src;
  Result^.PseudoFnCall.Arg2 := Dest;
  Result^.PseudoFnCall.Arg3 := Code;
end;

procedure RegisterGlobals_Strings;
begin
  { Character and String subroutines }
  AddPseudoFn('CONCAT', @_ModStrings_Concat_Parse, @Pf_Indef_Describe);
  PseudoFuns.Str := AddPseudoFn('STR', @_ModStrings_Str_Parse, 
                    @_ModStrings_Str_Describe);
  PseudoFuns.Val := AddPseudoFn('VAL', @_ModStrings_Val_Parse, 
                    @Pf_Ternary_Describe);
  AddFunction(MakeFunction1('CHR', PrimitiveTypes.PtChar,
              MakeArg('POS', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction3('COPY', PrimitiveTypes.PtString,
              MakeConstArg('STR', PrimitiveTypes.PtString),
              MakeArg('POS', PrimitiveTypes.PtInteger),
              MakeArg('NUM', PrimitiveTypes.PtInteger)));
  AddFunction(MakeProcedure3('DELETE',
              MakeVarArg('STR', PrimitiveTypes.PtString),
              MakeArg('POS', PrimitiveTypes.PtInteger),
              MakeArg('NUM', PrimitiveTypes.PtInteger)));
  AddFunction(MakeProcedure3('INSERT',
              MakeConstArg('INS', PrimitiveTypes.PtString),
              MakeVarArg('TARGET', PrimitiveTypes.PtString),
              MakeArg('POS', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction1('LENGTH', PrimitiveTypes.PtInteger,
              MakeConstArg('STR', PrimitiveTypes.PtString)));
  AddFunction(MakeFunction1('LOWERCASE', PrimitiveTypes.PtChar,
              MakeArg('CHR', PrimitiveTypes.PtChar)));
  AddFunction(MakeFunction2('POS', PrimitiveTypes.PtInteger,
              MakeConstArg('NEEDLE', PrimitiveTypes.PtString),
              MakeConstArg('HAYSTACK', PrimitiveTypes.PtString)));
  AddFunction(MakeFunction1('UPCASE', PrimitiveTypes.PtChar,
              MakeArg('CHR', PrimitiveTypes.PtChar)));
end;

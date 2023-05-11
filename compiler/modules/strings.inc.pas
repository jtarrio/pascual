function _ModStrings_Concat_Parse(FnExpr : TSExpression) : TSExpression;
var Operand : TSExpression;
begin
  ExDispose(FnExpr);
  Result := nil;
  WantTokenAndRead(TkLparen);
  repeat
    Operand := PSExpression;
    EnsureStringyExpr(Operand);
    if Result = nil then Result := Operand
    else Result := ExOpAdd(Result, Operand);
    WantToken2(TkComma, TkRparen);
    SkipToken(TkComma)
  until Lexer.Token.Id = TkRparen;
  WantTokenAndRead(TkRparen)
end;

function _ModStrings_Str_Parse(FnExpr : TSExpression) : TSExpression;
var 
  Src : TSEWriteArg;
  Dest : TSExpression;
begin
  WantTokenAndRead(TkLparen);
  Src := Pf_WriteArg_Parse;
  WantTokenAndRead(TkComma);
  Dest := PsExpression;
  WantTokenAndRead(TkRparen);
  EnsureAssignableExpr(Dest);
  EnsureStringExpr(Dest);
  if not IsBooleanType(Src.Arg^.TypePtr)
     and not IsIntegerType(Src.Arg^.TypePtr)
     and not IsRealType(Src.Arg^.TypePtr)
     and not IsEnumType(Src.Arg^.TypePtr) then
    ErrorForExpr('Invalid type for source of STR', Src.Arg);
  ExMarkInitialized(Dest);
  ExDispose(FnExpr);
  if IsEnumType(Src.Arg^.TypePtr) then
      Src.Arg^.TypePtr^.EnumPtr^.NeedValueArray := true;
  Result := ExConvertToStr(Src.Arg, Src.Width, Src.Prec, Dest)
end;

function _ModStrings_Val_Parse(FnExpr : TSExpression) : TSExpression;
var 
  Src, Dest, Code : TSExpression;
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
  ExDispose(FnExpr);
  if IsEnumType(Dest^.TypePtr) then
      Dest^.TypePtr^.EnumPtr^.NeedValueArray := true;
  Result := ExConvertToVal(Src, Dest, Code)
end;

procedure RegisterGlobals_Strings;
begin
  { Character and String subroutines }
  AddPsfn('CONCAT', @_ModStrings_Concat_Parse);
  AddPsfn('STR', @_ModStrings_Str_Parse);
  AddPsfn('VAL', @_ModStrings_Val_Parse);
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

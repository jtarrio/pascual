function _Pf_Unary_Parse : TExpression;
begin
  WantTokenAndRead(TkLparen);
  Result := PsExpression;
  WantTokenAndRead(TkRparen)
end;

function _Pf_Fun_Overload(const NamePrefix : string;
                          TypePtr : TPsTypePtr) : string;
begin
  if IsBooleanType(TypePtr) then Result := NamePrefix + '_b'
  else if IsIntegerType(TypePtr) then Result := NamePrefix + '_i'
  else if IsRealType(TypePtr) then Result := NamePrefix + '_r'
  else if IsCharType(TypePtr) then Result := NamePrefix + '_c'
  else if IsStringType(TypePtr) then Result := NamePrefix + '_s'
  else CompileError('Type ' + TypeName(TypePtr) + ' is not representable for ' +
    'overload of ' + NamePrefix)
end;

function Pf_Overload_Parse(FnExpr : TExpression;
                            NamePrefix : string) : TExpression;
var 
  Arg : TExpression;
  Args : TExFunctionArgs;
  FnPtr : TPsFnPtr;
begin
  ExDispose(FnExpr);
  Arg := nil;
  if Lexer.Token.Id = TkLparen then
  begin
    WantTokenAndRead(TkLparen);
    if Lexer.Token.Id <> TkRparen then Arg := PsExpression;
    WantTokenAndRead(TkRparen);
  end;
  if Arg = nil then
  begin
    FnPtr := FindNameOfClass(NamePrefix + '_n', TncFunction, {Required=}true)^.
             FnPtr;
    Args.Size := 0;
    Result := ExFunctionCall(ExFnRef(FnPtr), Args);
  end
  else
  begin
    FnPtr := FindNameOfClass(_Pf_Fun_Overload(NamePrefix, Arg^.TypePtr),
             TncFunction, {Required=}true)^.FnPtr;
    Args.Size := 1;
    Args.Values[1] := Arg;
    Result := ExFunctionCall(ExFnRef(FnPtr), Args);
  end
end;

function PfDispose_Parse(FnExpr : TExpression) : TExpression;
var
  Ptr : TExpression;
  FnPtr : TPsFnPtr;
  Args : TExFunctionArgs;
begin
  Ptr := _Pf_Unary_Parse;
  EnsureAssignableExpr(Ptr);
  EnsurePointerExpr(Ptr);
  ExMarkInitialized(Ptr);

  ExDispose(FnExpr);
  Args.Size := 1;
  Args.Values[1] := Ptr;
  FnPtr := FindNameOfClass('Dispose', TncFunction, {Required=}true)^.FnPtr;
  Result := ExFunctionCall(ExFnRef(FnPtr), Args)
end;

function PfNew_Parse(FnExpr : TExpression) : TExpression;
var
  Ptr : TExpression;
  FnPtr : TPsFnPtr;
  Args : TExFunctionArgs;
begin
  Ptr := _Pf_Unary_Parse;
  EnsureAssignableExpr(Ptr);
  EnsurePointerExpr(Ptr);
  ExMarkInitialized(Ptr);

  ExDispose(FnExpr);
  Args.Size := 2;
  Args.Values[1] := Ptr;
  Args.Values[2] := ExSizeof(Ptr^.TypePtr^.PointedTypePtr);
  FnPtr := FindNameOfClass('New', TncFunction, {Required=}true)^.FnPtr;
  Result := ExFunctionCall(ExFnRef(FnPtr), Args)
end;

function PfOrd_Parse(FnExpr : TExpression) : TExpression;
begin
  ExDispose(FnExpr);
  Result := ExOpOrd(_Pf_Unary_Parse)
end;

function PfPred_Parse(FnExpr : TExpression) : TExpression;
begin
  ExDispose(FnExpr);
  Result := ExOpPred(_Pf_Unary_Parse)
end;

function PfRandom_Parse(FnExpr : TExpression) : TExpression;
begin
  Result := Pf_Overload_Parse(FnExpr, 'RANDOM')
end;

function PfSizeof_Parse(FnExpr : TExpression) : TExpression;
var 
  Id : TPsIdentifier;
  Found : TPsName;
begin
  ExDispose(FnExpr);
  WantTokenAndRead(TkLparen);
  Id := PsIdentifier;
  WantTokenAndRead(TkRparen);
  Found := FindName(Id.Name, {Required=}true)^;
  if Found.Cls = TncVariable then
    Result := ExSizeof(Found.VarPtr^.TypePtr)
  else if Found.Cls = TncType then
    Result := ExSizeof(Found.TypePtr)
  else
    CompileError('Expected a variable or a type identifier; got ' + Id.Name);
end;

function PfSucc_Parse(FnExpr : TExpression) : TExpression;
begin
  ExDispose(FnExpr);
  Result := ExOpSucc(_Pf_Unary_Parse)
end;

function Pf_Indef_Describe(Expr : TExpression) : string;
begin
  with Expr^.PseudoFnCall do
    Result := PseudoFnPtr^.Name + '(...)'
end;

function _Pf_Unary_Parse : TExpression;
begin
  WantTokenAndRead(TkLparen);
  Result := PsExpression;
  WantTokenAndRead(TkRparen)
end;

function _Pf_Fun_Overload(const NamePrefix : string;
                          TypePtr : TPsTypePtr) : string;
begin
  TypePtr := GetFundamentalType(TypePtr);
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
var Ptr : TExpression;
begin
  Ptr := _Pf_Unary_Parse;
  EnsureAssignableExpr(Ptr);
  EnsurePointerExpr(Ptr);
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := Ptr
end;

function PfNew_Parse(FnExpr : TExpression) : TExpression;
var Ptr : TExpression;
begin
  Ptr := _Pf_Unary_Parse;
  EnsureAssignableExpr(Ptr);
  EnsurePointerExpr(Ptr);
  ExMarkInitialized(Ptr);
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := Ptr
end;

function PfOrd_Parse(FnExpr : TExpression) : TExpression;
begin
  ExDispose(FnExpr);
  Result := PfOrd(_Pf_Unary_Parse)
end;

function PfOrd(Arg : TExpression) : TExpression;
begin
  EnsureOrdinalExpr(Arg);
  if ExIsImmediate(Arg) then
  begin
    with Arg^.Immediate do
      case Cls of 
        XicBoolean : if BooleanVal then Result := ExIntegerConstant(1)
                     else Result := ExIntegerConstant(0);
        XicInteger: Result := ExIntegerConstant(IntegerVal);
        XicChar: Result := ExIntegerConstant(Ord(CharVal));
        XicEnum: Result := ExIntegerConstant(EnumOrdinal);
        else ErrorForExpr('Expected an ordinal', Arg)
      end;
    ExDispose(Arg)
  end
  else
  begin
    Result := ExPseudoFnCall(ExPseudoFn(PseudoFuns.Ord));
    Result^.PseudoFnCall.Arg1 := Arg;
    Result^.TypePtr := PrimitiveTypes.PtInteger
  end
end;

function PfPred_Parse(FnExpr : TExpression) : TExpression;
begin
  ExDispose(FnExpr);
  Result := PfPred(_Pf_Unary_Parse)
end;

function PfPred(Arg : TExpression) : TExpression;
var OutOfBounds : boolean;
begin
  EnsureOrdinalExpr(Arg);
  if ExIsImmediate(Arg) then
  begin
    OutOfBounds := false;
    with Arg^.Immediate do
      case Cls of 
        XicBoolean : if BooleanVal then Result := ExBooleanConstant(false)
                     else OutOfBounds := true;
        XicInteger: Result := ExIntegerConstant(IntegerVal - 1);
        XicChar: if Ord(CharVal) > 0 then
                   Result := ExCharConstant(Pred(CharVal))
                 else OutOfBounds := true;
        XicEnum: if EnumOrdinal > 0 then
                   Result := ExEnumConstant(Pred(EnumOrdinal), Arg^.TypePtr)
                 else OutOfBounds := true;
        else ErrorForExpr('Expected an ordinal', Arg)
      end;
    if OutOfBounds then
      ErrorForExpr('Predecessor for argument would be out of bounds', Arg);
    ExDispose(Arg)
  end
  else
  begin
    Result := ExPseudoFnCall(ExPseudoFn(PseudoFuns.Pred));
    Result^.PseudoFnCall.Arg1 := Arg;
    Result^.TypePtr := Arg^.TypePtr
  end
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
  WantTokenAndRead(TkLparen);
  Id := PsIdentifier;
  WantTokenAndRead(TkRparen);
  Result := ExPseudoFnCall(FnExpr);
  Result^.TypePtr := PrimitiveTypes.PtInteger;
  Found := FindName(Id.Name, {Required=}true)^;
  if Found.Cls = TncVariable then
    Result^.PseudoFnCall.Arg1 := ExVariable(Found.VarPtr)
  else if Found.Cls = TncType then
         Result^.PseudoFnCall.TypeArg := Found.TypePtr
  else
    CompileError('Expected a variable or a type identifier; got ' + Id.Name);
end;

function PfSucc_Parse(FnExpr : TExpression) : TExpression;
begin
  ExDispose(FnExpr);
  Result := PfSucc(_Pf_Unary_Parse)
end;

function PfSucc(Arg : TExpression) : TExpression;
var OutOfBounds : boolean;
begin
  EnsureOrdinalExpr(Arg);
  if ExIsImmediate(Arg) then
  begin
    OutOfBounds := false;
    with Arg^.Immediate do
      case Cls of 
        XicBoolean : if not BooleanVal then Result := ExBooleanConstant(true)
                     else OutOfBounds := true;
        XicInteger: Result := ExIntegerConstant(IntegerVal + 1);
        XicChar: if Ord(CharVal) < 255 then
                   Result := ExCharConstant(Succ(CharVal))
                 else OutOfBounds := true;
        XicEnum: if EnumOrdinal < EnumPtr^.Size - 1 then
                   Result := ExEnumConstant(Succ(EnumOrdinal), Arg^.TypePtr)
                 else OutOfBounds := true;
        else ErrorForExpr('Expected an ordinal', Arg)
      end;
    if OutOfBounds then
      ErrorForExpr('Successor for argument would be out of bounds', Arg);
    ExDispose(Arg)
  end
  else
  begin
    Result := ExPseudoFnCall(ExPseudoFn(PseudoFuns.Succ));
    Result^.PseudoFnCall.Arg1 := Arg;
    Result^.TypePtr := Arg^.TypePtr
  end
end;

function Pf_Unary_Describe(Expr : TExpression) : string;
begin
  with Expr^.PseudoFnCall do
    Result := PseudoFnPtr^.Name + '(' + ExDescribe(Arg1) + ')'
end;

function Pf_Ternary_Describe(Expr : TExpression) : string;
begin
  with Expr^.PseudoFnCall do
    Result := PseudoFnPtr^.Name + '(' + ExDescribe(Arg1) + ', ' +
              ExDescribe(Arg2) + ', ' + ExDescribe(Arg3) + ')'
end;

function Pf_Indef_Describe(Expr : TExpression) : string;
begin
  with Expr^.PseudoFnCall do
    Result := PseudoFnPtr^.Name + '(...)'
end;

function PfSizeof_Describe(Expr : TExpression) : string;
begin
  with Expr^.PseudoFnCall do
    if Arg1 <> nil then
      Result := 'SIZEOF(' + ExDescribe(Arg1) + ')'
    else
      Result := 'SIZEOF(' + TypeName(TypeArg) + ')'
end;

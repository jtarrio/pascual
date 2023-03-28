function ExOpOrd(Expr : TExpression) : TExpression;
begin
  EnsureOrdinalExpr(Expr);
  if ExIsImmediate(Expr) then
  begin
    with Expr^.Immediate do
      case Cls of 
        XicBoolean : if BooleanVal then Result := ExIntegerConstant(1)
                     else Result := ExIntegerConstant(0);
        XicInteger: Result := ExIntegerConstant(IntegerVal);
        XicChar: Result := ExIntegerConstant(Ord(CharVal));
        XicEnum: Result := ExIntegerConstant(EnumOrdinal);
      end;
    ExDispose(Expr)
  end
  else
    Result := _ExOp_MakeUnary(Expr, XoOrd, PrimitiveTypes.PtInteger)
end;

function ExOpPred(Expr : TExpression) : TExpression;
var OutOfBounds : boolean;
begin
  EnsureOrdinalExpr(Expr);
  if ExIsImmediate(Expr) then
  begin
    OutOfBounds := false;
    with Expr^.Immediate do
      case Cls of 
        XicBoolean : if BooleanVal then BooleanVal := false
                     else OutOfBounds := true;
        XicInteger: IntegerVal := Pred(IntegerVal);
        XicChar: if Ord(CharVal) > 0 then CharVal := Pred(CharVal)
                 else OutOfBounds := true;
        XicEnum: if EnumOrdinal > 0 then EnumOrdinal := Pred(EnumOrdinal)
                 else OutOfBounds := true;
      end;
    if OutOfBounds then
      ErrorForExpr('Predecessor for argument would be out of bounds', Expr);
    Result := Expr
  end
  else
    Result := _ExOp_MakeUnary(Expr, XoPred, Expr^.TypePtr)
end;

function ExOpSucc(Expr : TExpression) : TExpression;
var OutOfBounds : boolean;
begin
  EnsureOrdinalExpr(Expr);
  if ExIsImmediate(Expr) then
  begin
    OutOfBounds := false;
    with Expr^.Immediate do
      case Cls of 
        XicBoolean : if not BooleanVal then BooleanVal := true
                     else OutOfBounds := true;
        XicInteger: IntegerVal := Succ(IntegerVal);
        XicChar: if Ord(CharVal) < 255 then
                   CharVal := Succ(CharVal)
                 else OutOfBounds := true;
        XicEnum: if EnumOrdinal < EnumPtr^.Size - 1 then
                   EnumOrdinal := Succ(EnumOrdinal)
                 else OutOfBounds := true;
      end;
    if OutOfBounds then
      ErrorForExpr('Successor for argument would be out of bounds', Expr);
    Result := Expr
  end
  else
    Result := _ExOp_MakeUnary(Expr, XoSucc, Expr^.TypePtr)
end;

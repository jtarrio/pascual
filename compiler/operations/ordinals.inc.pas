function ExOpOrd(Expr : TSExpression) : TSExpression;
begin
  EnsureOrdinalExpr(Expr);
  if ExIsImmediate(Expr) then
  begin
    with Expr^.Immediate do
      case Cls of 
        SeicBoolean : if BooleanVal then Result := ExIntegerConstant(1)
                     else Result := ExIntegerConstant(0);
        SeicInteger: Result := ExIntegerConstant(IntegerVal);
        SeicChar: Result := ExIntegerConstant(Ord(CharVal));
        SeicEnum: Result := ExIntegerConstant(EnumOrdinal);
      end;
    ExDispose(Expr)
  end
  else
    Result := _ExOp_MakeUnary(Expr, SeoOrd, PrimitiveTypes.PtInteger)
end;

function ExOpPred(Expr : TSExpression) : TSExpression;
var OutOfBounds : boolean;
begin
  EnsureOrdinalOrPointerExpr(Expr);
  if ExIsImmediate(Expr) then
  begin
    OutOfBounds := false;
    with Expr^.Immediate do
      case Cls of 
        SeicBoolean : if BooleanVal then BooleanVal := false
                     else OutOfBounds := true;
        SeicInteger: IntegerVal := Pred(IntegerVal);
        SeicChar: if Ord(CharVal) > 0 then CharVal := Pred(CharVal)
                 else OutOfBounds := true;
        SeicEnum: if EnumOrdinal > 0 then EnumOrdinal := Pred(EnumOrdinal)
                 else OutOfBounds := true;
      end;
    if OutOfBounds then
      ErrorForExpr('Predecessor for argument would be out of bounds', Expr);
    Result := Expr
  end
  else
    Result := _ExOp_MakeUnary(Expr, SeoPred, Expr^.TypePtr)
end;

function ExOpSucc(Expr : TSExpression) : TSExpression;
var OutOfBounds : boolean;
begin
  EnsureOrdinalOrPointerExpr(Expr);
  if ExIsImmediate(Expr) then
  begin
    OutOfBounds := false;
    with Expr^.Immediate do
      case Cls of 
        SeicBoolean : if not BooleanVal then BooleanVal := true
                     else OutOfBounds := true;
        SeicInteger: IntegerVal := Succ(IntegerVal);
        SeicChar: if Ord(CharVal) < 255 then
                   CharVal := Succ(CharVal)
                 else OutOfBounds := true;
        SeicEnum: if EnumOrdinal < EnumPtr^.Size - 1 then
                   EnumOrdinal := Succ(EnumOrdinal)
                 else OutOfBounds := true;
      end;
    if OutOfBounds then
      ErrorForExpr('Successor for argument would be out of bounds', Expr);
    Result := Expr
  end
  else
    Result := _ExOp_MakeUnary(Expr, SeoSucc, Expr^.TypePtr)
end;

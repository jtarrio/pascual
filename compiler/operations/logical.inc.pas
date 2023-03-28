function ExOpNot(Expr : TExpression) : TExpression;
forward;
function ExOpAnd(Left, Right : TExpression) : TExpression;
forward;
function ExOpOr(Left, Right : TExpression) : TExpression;
forward;
function ExOpXor(Left, Right : TExpression) : TExpression;
forward;

function _ExOpAnd_Booleans(Left, Right : TExpression) : TExpression;
begin
  if ExIsImmediate(Left) and (Left^.Immediate.BooleanVal = false) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Left) and (Left^.Immediate.BooleanVal = true) then
  begin
    ExDispose(Left);
    Result := Right
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.BooleanVal := Left^.Immediate.BooleanVal and
                                    Right^.Immediate.BooleanVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoAnd, PrimitiveTypes.PtBoolean)
end;

function _ExOpOr_Booleans(Left, Right : TExpression) : TExpression;
begin
  if ExIsImmediate(Left) and (Left^.Immediate.BooleanVal = true) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Left) and (Left^.Immediate.BooleanVal = false) then
  begin
    ExDispose(Left);
    Result := Right
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.BooleanVal := Left^.Immediate.BooleanVal or
                                    Right^.Immediate.BooleanVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoOr, PrimitiveTypes.PtBoolean)
end;

function _ExOpXor_Booleans(Left, Right : TExpression) : TExpression;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.BooleanVal := Left^.Immediate.BooleanVal xor
                                    Right^.Immediate.BooleanVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoXor, PrimitiveTypes.PtBoolean)
end;

function _ExOpAnd_Integers(Left, Right : TExpression) : TExpression;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal and
                                    Right^.Immediate.IntegerVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoAnd, PrimitiveTypes.PtInteger)
end;

function _ExOpOr_Integers(Left, Right : TExpression) : TExpression;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal or
                                    Right^.Immediate.IntegerVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoOr, PrimitiveTypes.PtInteger)
end;

function _ExOpXor_Integers(Left, Right : TExpression) : TExpression;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal xor
                                    Right^.Immediate.IntegerVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoXor, PrimitiveTypes.PtInteger)
end;

function ExOpNot(Expr : TExpression) : TExpression;
begin
  if not IsBooleanType(Expr^.TypePtr) and not IsIntegerType(Expr^.TypePtr) then
    ErrorForExpr('Expected a boolean or an integer', Expr);
  if ExIsImmediate(Expr) then
  begin
    with Expr^.Immediate do
      if Cls = XicBoolean then BooleanVal := not BooleanVal
      else IntegerVal := not IntegerVal;
    Result := Expr
  end
  else
    Result := _ExOp_MakeUnary(Expr, XoNot, Expr^.TypePtr)
end;

function ExOpAnd(Left, Right : TExpression) : TExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpAnd_Booleans(Left, Right)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpAnd_Integers(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoAdd)
end;

function ExOpOr(Left, Right : TExpression) : TExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpOr_Booleans(Left, Right)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpOr_Integers(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoOr)
end;

function ExOpXor(Left, Right : TExpression) : TExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpXor_Booleans(Left, Right)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpXor_Integers(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoXor)
end;

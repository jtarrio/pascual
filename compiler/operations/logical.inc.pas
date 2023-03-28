function ExOpNot(Expr : TExpression) : TExpression;
forward;
function ExOpAnd(Left, Right : TExpression) : TExpression;
forward;
function ExOpOr(Left, Right : TExpression) : TExpression;
forward;
function ExOpXor(Left, Right : TExpression) : TExpression;
forward;

function _ExOpNot_Boolean(Expr : TExpression) : TExpression;
begin
  Result := Expr;
  if ExIsImmediate(Expr) then
    Result^.Immediate.BooleanVal := not Expr^.Immediate.BooleanVal
  else if (Expr^.Cls = XcUnaryOp) and (Expr^.Unary.Op = XoNot) then
  begin
    Result := ExCopy(Expr^.Unary.Parent);
    ExDispose(Expr)
  end
  else if (Expr^.Cls = XcBinaryOp) and (Expr^.Binary.Op = XoEq) then
         Result^.Binary.Op := XoNe
  else if (Expr^.Cls = XcBinaryOp) and (Expr^.Binary.Op = XoNe) then
         Result^.Binary.Op := XoEq
  else if (Expr^.Cls = XcBinaryOp) and (Expr^.Binary.Op = XoLt) then
         Result^.Binary.Op := XoGtEq
  else if (Expr^.Cls = XcBinaryOp) and (Expr^.Binary.Op = XoGt) then
         Result^.Binary.Op := XoLtEq
  else if (Expr^.Cls = XcBinaryOp) and (Expr^.Binary.Op = XoLtEq)
          and not IsSetType(Expr^.Binary.Left^.TypePtr)
          and not IsSetType(Expr^.Binary.Right^.TypePtr) then
         Result^.Binary.Op := XoGt
  else if (Expr^.Cls = XcBinaryOp) and (Expr^.Binary.Op = XoGtEq)
          and not IsSetType(Expr^.Binary.Left^.TypePtr)
          and not IsSetType(Expr^.Binary.Right^.TypePtr) then
         Result^.Binary.Op := XoLt
  else if (Expr^.Cls = XcBinaryOp) and (Expr^.Binary.Op = XoAnd)
          and IsBooleanType(Expr^.TypePtr) then
  begin
    Result := ExOpOr(ExOpNot(ExCopy(Expr^.Binary.Left)),
              ExOpNot(ExCopy(Expr^.Binary.Right)));
    ExDispose(Expr)
  end
  else if (Expr^.Cls = XcBinaryOp) and (Expr^.Binary.Op = XoOr)
          and IsBooleanType(Expr^.TypePtr) then
  begin
    Result := ExOpAnd(ExOpNot(ExCopy(Expr^.Binary.Left)),
              ExOpNot(ExCopy(Expr^.Binary.Right)));
    ExDispose(Expr)
  end
  else Result := _ExOp_MakeUnary(Expr, XoNot, Expr^.TypePtr)
end;

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

function _ExOpNot_Integer(Expr : TExpression) : TExpression;
begin
  if ExIsImmediate(Expr) then
  begin
    Expr^.Immediate.IntegerVal := not Expr^.Immediate.IntegerVal;
    Result := Expr
  end
  else Result := _ExOp_MakeUnary(Expr, XoNot, Expr^.TypePtr)
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
  if IsBooleanType(Expr^.TypePtr) then
    Result := _ExOpNot_Boolean(Expr)
  else if IsIntegerType(Expr^.TypePtr) then
         Result := _ExOpNot_Integer(Expr)
  else ErrorInvalidOperator(Expr, XoNot)
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

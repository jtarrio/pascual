function _ExOpShl_Integers(Left, Right : TExpression) : TExpression;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal shl
                                    Right^.Immediate.IntegerVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoShl, PrimitiveTypes.PtInteger)
end;

function _ExOpShr_Integers(Left, Right : TExpression) : TExpression;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal shr
                                    Right^.Immediate.IntegerVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoShr, PrimitiveTypes.PtInteger)
end;

function ExOpShl(Left, Right : TExpression) : TExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpShl_Integers(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoShl)
end;

function ExOpShr(Left, Right : TExpression) : TExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpShr_Integers(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoShr)
end;

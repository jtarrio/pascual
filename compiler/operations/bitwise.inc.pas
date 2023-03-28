function _ExOpBitwise_Integers(Left, Right : TExpression;
                               Op : TExOperator) : TExpression;
begin
  if ExIsImmediate(Right) and (Right^.Immediate.IntegerVal = 0) then
  begin
    ExDispose(Right);
    Result := Left;
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    case Op of 
      XoShl:
             Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal shl
                                             Right^.Immediate.IntegerVal;
      XoShr:
             Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal shr
                                             Right^.Immediate.IntegerVal;
    end;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtInteger)
end;

function ExOpShl(Left, Right : TExpression) : TExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
    Result := _ExOpBitwise_Integers(Left, Right, XoShl)
  else ErrorInvalidOperator2(Left, Right, XoShl)
end;

function ExOpShr(Left, Right : TExpression) : TExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
    Result := _ExOpBitwise_Integers(Left, Right, XoShr)
  else ErrorInvalidOperator2(Left, Right, XoShr)
end;

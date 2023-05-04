function _ExOpBitwise_Integers(Left, Right : TSExpression;
                               Op : TSEOperator) : TSExpression;
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
      SeoShl:
             Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal shl
                                             Right^.Immediate.IntegerVal;
      SeoShr:
             Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal shr
                                             Right^.Immediate.IntegerVal;
    end;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtInteger)
end;

function ExOpShl(Left, Right : TSExpression) : TSExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
    Result := _ExOpBitwise_Integers(Left, Right, SeoShl)
  else ErrorInvalidOperator2(Left, Right, SeoShl)
end;

function ExOpShr(Left, Right : TSExpression) : TSExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
    Result := _ExOpBitwise_Integers(Left, Right, SeoShr)
  else ErrorInvalidOperator2(Left, Right, SeoShr)
end;

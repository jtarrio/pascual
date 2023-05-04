function _ExOpNot_Boolean(Expr : TSExpression) : TSExpression;
begin
  Result := Expr;
  if ExIsImmediate(Expr) then
    Result^.Immediate.BooleanVal := not Expr^.Immediate.BooleanVal
  else if (Expr^.Cls = SecUnaryOp) and (Expr^.Unary.Op = SeoNot) then
  begin
    Result := ExCopy(Expr^.Unary.Parent);
    ExDispose(Expr)
  end
  else if (Expr^.Cls = SecBinaryOp) and (Expr^.Binary.Op = SeoEq) then
         Result^.Binary.Op := SeoNe
  else if (Expr^.Cls = SecBinaryOp) and (Expr^.Binary.Op = SeoNe) then
         Result^.Binary.Op := SeoEq
  else if (Expr^.Cls = SecBinaryOp) and (Expr^.Binary.Op = SeoLt) then
         Result^.Binary.Op := SeoGtEq
  else if (Expr^.Cls = SecBinaryOp) and (Expr^.Binary.Op = SeoGt) then
         Result^.Binary.Op := SeoLtEq
  else if (Expr^.Cls = SecBinaryOp) and (Expr^.Binary.Op = SeoLtEq)
          and not IsSetType(Expr^.Binary.Left^.TypePtr)
          and not IsSetType(Expr^.Binary.Right^.TypePtr) then
         Result^.Binary.Op := SeoGt
  else if (Expr^.Cls = SecBinaryOp) and (Expr^.Binary.Op = SeoGtEq)
          and not IsSetType(Expr^.Binary.Left^.TypePtr)
          and not IsSetType(Expr^.Binary.Right^.TypePtr) then
         Result^.Binary.Op := SeoLt
  else if (Expr^.Cls = SecBinaryOp) and (Expr^.Binary.Op = SeoAnd)
          and IsBooleanType(Expr^.TypePtr) then
  begin
    Result := ExOpOr(ExOpNot(ExCopy(Expr^.Binary.Left)),
              ExOpNot(ExCopy(Expr^.Binary.Right)));
    ExDispose(Expr)
  end
  else if (Expr^.Cls = SecBinaryOp) and (Expr^.Binary.Op = SeoOr)
          and IsBooleanType(Expr^.TypePtr) then
  begin
    Result := ExOpAnd(ExOpNot(ExCopy(Expr^.Binary.Left)),
              ExOpNot(ExCopy(Expr^.Binary.Right)));
    ExDispose(Expr)
  end
  else Result := _ExOp_MakeUnary(Expr, SeoNot, Expr^.TypePtr)
end;

function _ExOpLogical_Booleans(Left, Right : TSExpression;
                               Op : TSEOperator) : TSExpression;
var 
  HasShortcut : boolean;
  Shortcut : boolean;
begin
  HasShortcut := false;
  case Op of 
    SeoAnd:
           begin
             HasShortcut := true;
             Shortcut := false;
           end;
    SeoOr:
          begin
            HasShortcut := true;
            Shortcut := true;
          end;
  end;
  if HasShortcut and ExIsImmediate(Left) then
  begin
    if Left^.Immediate.BooleanVal = Shortcut then
    begin
      ExDispose(Right);
      Result := Left
    end
    else
    begin
      ExDispose(Left);
      Result := Right
    end
  end
  else if HasShortcut and ExIsImmediate(Right)
          and (Right^.Immediate.BooleanVal <> Shortcut) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if (Op = SeoXor) and ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.BooleanVal := Left^.Immediate.BooleanVal xor
                                    Right^.Immediate.BooleanVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtBoolean)
end;

function _ExOpNot_Integer(Expr : TSExpression) : TSExpression;
begin
  if ExIsImmediate(Expr) then
  begin
    Expr^.Immediate.IntegerVal := not Expr^.Immediate.IntegerVal;
    Result := Expr
  end
  else Result := _ExOp_MakeUnary(Expr, SeoNot, Expr^.TypePtr)
end;

function _ExOpLogical_Integers(Left, Right : TSExpression;
                               Op : TSEOperator) : TSExpression;
var Lt, Rt, Ret : integer;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Lt := Left^.Immediate.IntegerVal;
    Rt := Right^.Immediate.IntegerVal;
    case Op of 
      SeoAnd: Ret := Lt and Rt;
      SeoOr: Ret := Lt or Rt;
      SeoXor: Ret := Lt xor Rt;
    end;
    Result := Left;
    Result^.Immediate.IntegerVal := Ret;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtInteger)
end;

function ExOpNot(Expr : TSExpression) : TSExpression;
begin
  if IsBooleanType(Expr^.TypePtr) then
    Result := _ExOpNot_Boolean(Expr)
  else if IsIntegerType(Expr^.TypePtr) then
         Result := _ExOpNot_Integer(Expr)
  else ErrorInvalidOperator(Expr, SeoNot)
end;

function ExOpAnd(Left, Right : TSExpression) : TSExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpLogical_Booleans(Left, Right, SeoAnd)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpLogical_Integers(Left, Right, SeoAnd)
  else ErrorInvalidOperator2(Left, Right, SeoAnd)
end;

function ExOpOr(Left, Right : TSExpression) : TSExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpLogical_Booleans(Left, Right, SeoOr)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpLogical_Integers(Left, Right, SeoOr)
  else ErrorInvalidOperator2(Left, Right, SeoOr)
end;

function ExOpXor(Left, Right : TSExpression) : TSExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpLogical_Booleans(Left, Right, SeoXor)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpLogical_Integers(Left, Right, SeoXor)
  else ErrorInvalidOperator2(Left, Right, SeoXor)
end;

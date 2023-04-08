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

function _ExOpLogical_Booleans(Left, Right : TExpression;
                               Op : TExOperator) : TExpression;
var 
  HasShortcut : boolean;
  Shortcut : boolean;
begin
  HasShortcut := false;
  case Op of 
    XoAnd:
           begin
             HasShortcut := true;
             Shortcut := false;
           end;
    XoOr:
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
  else if (Op = XoXor) and ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.BooleanVal := Left^.Immediate.BooleanVal xor
                                    Right^.Immediate.BooleanVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtBoolean)
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

function _ExOpLogical_Integers(Left, Right : TExpression;
                               Op : TExOperator) : TExpression;
var Lt, Rt, Ret : integer;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Lt := Left^.Immediate.IntegerVal;
    Rt := Right^.Immediate.IntegerVal;
    case Op of 
      XoAnd: Ret := Lt and Rt;
      XoOr: Ret := Lt or Rt;
      XoXor: Ret := Lt xor Rt;
    end;
    Result := Left;
    Result^.Immediate.IntegerVal := Ret;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtInteger)
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
    Result := _ExOpLogical_Booleans(Left, Right, XoAnd)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpLogical_Integers(Left, Right, XoAnd)
  else ErrorInvalidOperator2(Left, Right, XoAnd)
end;

function ExOpOr(Left, Right : TExpression) : TExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpLogical_Booleans(Left, Right, XoOr)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpLogical_Integers(Left, Right, XoOr)
  else ErrorInvalidOperator2(Left, Right, XoOr)
end;

function ExOpXor(Left, Right : TExpression) : TExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpLogical_Booleans(Left, Right, XoXor)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpLogical_Integers(Left, Right, XoXor)
  else ErrorInvalidOperator2(Left, Right, XoXor)
end;

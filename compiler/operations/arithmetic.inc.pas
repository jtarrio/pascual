function ExOpNeg(Expr : TExpression) : TExpression;
forward;
function ExOpAdd(Left, Right : TExpression) : TExpression;
forward;
function ExOpSub(Left, Right : TExpression) : TExpression;
forward;
function ExOpMul(Left, Right : TExpression) : TExpression;
forward;

function _ExOpAdd_Integers(Left, Right : TExpression) : TExpression;
begin
  if ExIsImmediate(Left) and (Left^.Immediate.IntegerVal = 0) then
  begin
    ExDispose(Left);
    Result := Right
  end
  else if ExIsImmediate(Right) and (Right^.Immediate.IntegerVal = 0) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal +
                                    Right^.Immediate.IntegerVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoAdd, PrimitiveTypes.PtInteger)
end;

function _ExOpSub_Integers(Left, Right : TExpression) : TExpression;
begin
  if ExIsImmediate(Left) and (Left^.Immediate.IntegerVal = 0) then
  begin
    ExDispose(Left);
    Result := ExOpNeg(Right)
  end
  else if ExIsImmediate(Right) and (Right^.Immediate.IntegerVal = 0) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal -
                                    Right^.Immediate.IntegerVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoSub, PrimitiveTypes.PtInteger)
end;

function _ExOpMul_Integers(Left, Right : TExpression) : TExpression;
begin
  if ExIsImmediate(Left) and (Left^.Immediate.IntegerVal = 1) then
  begin
    ExDispose(Left);
    Result := Right
  end
  else if ExIsImmediate(Left) and (Left^.Immediate.IntegerVal = -1) then
  begin
    ExDispose(Left);
    Result := ExOpNeg(Right)
  end
  else if ExIsImmediate(Right) and (Right^.Immediate.IntegerVal = 1) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Right) and (Right^.Immediate.IntegerVal = -1) then
  begin
    ExDispose(Right);
    Result := ExOpNeg(Left)
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal *
                                    Right^.Immediate.IntegerVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoMul, PrimitiveTypes.PtInteger)
end;

function _ExOpDiv_Integers(Left, Right : TExpression) : TExpression;
begin
  if ExIsImmediate(Right) and (Right^.Immediate.IntegerVal = 1) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Right) and (Right^.Immediate.IntegerVal = -1) then
  begin
    ExDispose(Right);
    Result := ExOpNeg(Left)
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal div
                                    Right^.Immediate.IntegerVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoDivInt, PrimitiveTypes.PtInteger)
end;

function _ExOpMod_Integers(Left, Right : TExpression) : TExpression;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.IntegerVal := Left^.Immediate.IntegerVal mod
                                    Right^.Immediate.IntegerVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoMod, PrimitiveTypes.PtInteger)
end;

function _ExOpAdd_Numbers(Left, Right : TExpression) : TExpression;
begin
  Left := ExCoerce(Left, PrimitiveTypes.PtReal);
  Right := ExCoerce(Right, PrimitiveTypes.PtReal);
  if ExIsImmediate(Left) and (Left^.Immediate.RealVal = 0.0) then
  begin
    ExDispose(Left);
    Result := Right
  end
  else if ExIsImmediate(Right) and (Right^.Immediate.RealVal = 0.0) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.RealVal := Left^.Immediate.RealVal +
                                 Right^.Immediate.RealVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoAdd, PrimitiveTypes.PtReal)
end;

function _ExOpSub_Numbers(Left, Right : TExpression) : TExpression;
begin
  Left := ExCoerce(Left, PrimitiveTypes.PtReal);
  Right := ExCoerce(Right, PrimitiveTypes.PtReal);
  if ExIsImmediate(Left) and (Left^.Immediate.RealVal = 0.0) then
  begin
    ExDispose(Left);
    Result := ExOpNeg(Right)
  end
  else if ExIsImmediate(Right) and (Right^.Immediate.RealVal = 0.0) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.RealVal := Left^.Immediate.RealVal -
                                 Right^.Immediate.RealVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoSub, PrimitiveTypes.PtReal)
end;

function _ExOpMul_Numbers(Left, Right : TExpression) : TExpression;
begin
  Left := ExCoerce(Left, PrimitiveTypes.PtReal);
  Right := ExCoerce(Right, PrimitiveTypes.PtReal);
  if ExIsImmediate(Left) and (Left^.Immediate.RealVal = 1.0) then
  begin
    ExDispose(Left);
    Result := Right
  end
  else if ExIsImmediate(Left) and (Left^.Immediate.RealVal = -1.0) then
  begin
    ExDispose(Left);
    Result := ExOpNeg(Right)
  end
  else if ExIsImmediate(Right) and (Right^.Immediate.RealVal = 1.0) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Right) and (Right^.Immediate.RealVal = -1.0) then
  begin
    ExDispose(Right);
    Result := ExOpNeg(Left)
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.RealVal := Left^.Immediate.RealVal *
                                 Right^.Immediate.RealVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoMul, PrimitiveTypes.PtReal)
end;

function _ExOpDiv_Numbers(Left, Right : TExpression) : TExpression;
begin
  Left := ExCoerce(Left, PrimitiveTypes.PtReal);
  Right := ExCoerce(Right, PrimitiveTypes.PtReal);
  if ExIsImmediate(Right) and (Right^.Immediate.RealVal = 1.0) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Right) and (Right^.Immediate.RealVal = -1.0) then
  begin
    ExDispose(Right);
    Result := ExOpNeg(Left)
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Result := Left;
    Result^.Immediate.RealVal := Left^.Immediate.RealVal /
                                 Right^.Immediate.RealVal;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoDivReal, PrimitiveTypes.PtReal)
end;

function _ExOpAdd_Strings(Left, Right : TExpression) : TExpression;
var Lt, Rt : string;
begin
  if ExIsImmediateOfClass(Left, XicString)
     and (Left^.Immediate.StringVal = '') then
  begin
    ExDispose(Left);
    Result := Right
  end
  else if ExIsImmediateOfClass(Right, XicString)
          and (Right^.Immediate.StringVal = '') then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    if Left^.Immediate.Cls = XicChar then Lt := Left^.Immediate.CharVal
    else Lt := Left^.Immediate.StringVal;
    if Right^.Immediate.Cls = XicChar then Rt := Right^.Immediate.CharVal
    else Rt := Right^.Immediate.StringVal;
    Result := Left;
    Result^.Immediate.StringVal := Lt + Rt;
    Result^.Immediate.Cls := XicString;
    Result^.TypePtr := PrimitiveTypes.PtString;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoAdd, PrimitiveTypes.PtString)
end;

function _ExOpUnion_Sets(Left, Right : TExpression) : TExpression;
var NewBds, OldBds : TExSetImmBounds;
begin
  ExSetCoerceToCommon(Left, Right);
  if ExIsImmediate(Left) and (Left^.SetBounds = nil) then
  begin
    ExDispose(Left);
    Result := Right
  end
  else if ExIsImmediate(Right) and (Right^.SetBounds = nil) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    NewBds := nil;
    OldBds := Left^.Immediate.SetBounds;
    while OldBds <> nil do
    begin
      NewBds := ExSetAddBounds(NewBds, OldBds^.First, OldBds^.Last);
      OldBds := OldBds^.Next
    end;
    OldBds := Right^.Immediate.SetBounds;
    while OldBds <> nil do
    begin
      NewBds := ExSetAddBounds(NewBds, OldBds^.First, OldBds^.Last);
      OldBds := OldBds^.Next
    end;
    Result := ExSetConstant(NewBds, Left^.TypePtr);
    ExDispose(Left);
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoAdd, Left^.TypePtr)
end;

function _ExOpDifference_Sets(Left, Right : TExpression) : TExpression;
var LtBds, RtBds, NewBds : TExSetImmBounds;
begin
  ExSetCoerceToCommon(Left, Right);
  if ExIsImmediate(Left) and (Left^.SetBounds = nil) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Right) and (Right^.SetBounds = nil) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    LtBds := Left^.Immediate.SetBounds;
    RtBds := Right^.Immediate.SetBounds;
    NewBds := nil;
    while (LtBds <> nil) and (RtBds <> nil) do
    begin
      if RtBds^.Last < LtBds^.First then RtBds := RtBds^.Next
      else if (RtBds^.First <= LtBds^.First)
              and (RtBds^.Last < LtBds^.Last) then
             LtBds^.First := RtBds^.Last + 1
      else if (RtBds^.First <= LtBds^.First)
              and (RtBds^.Last >= LtBds^.Last) then
             LtBds := LtBds^.Next
      else if (RtBds^.First > LtBds^.First)
              and (RtBds^.Last < LtBds^.Last) then
      begin
        NewBds := ExSetAddBounds(NewBds, LtBds^.First, RtBds^.First - 1);
        LtBds^.First := RtBds^.Last + 1
      end
      else if (RtBds^.First < LtBds^.Last)
              and (RtBds^.Last >= LtBds^.Last) then
      begin
        NewBds := ExSetAddBounds(NewBds, LtBds^.First, RtBds^.First - 1);
        LtBds := LtBds^.Next
      end
      else
      begin
        NewBds := ExSetAddBounds(NewBds, LtBds^.First, LtBds^.Last);
        LtBds := LtBds^.Next
      end
    end;
    while LtBds <> nil do
    begin
      NewBds := ExSetAddBounds(NewBds, LtBds^.First, LtBds^.Last);
      LtBds := LtBds^.Next
    end;
    Result := ExSetConstant(NewBds, Left^.TypePtr);
    ExDispose(Left);
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoSub, Left^.TypePtr)
end;

function _ExOpIntersection_Sets(Left, Right : TExpression) : TExpression;
var LtBds, RtBds, NewBds : TExSetImmBounds;
begin
  ExSetCoerceToCommon(Left, Right);
  if ExIsImmediate(Left) and (Left^.SetBounds = nil) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if ExIsImmediate(Right) and (Right^.SetBounds = nil) then
  begin
    ExDispose(Left);
    Result := Right
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    LtBds := Left^.Immediate.SetBounds;
    RtBds := Right^.Immediate.SetBounds;
    NewBds := nil;
    while (LtBds <> nil) and (RtBds <> nil) do
    begin
      if RtBds^.Last < LtBds^.First then RtBds := RtBds^.Next
      else if (RtBds^.First <= LtBds^.First)
              and (RtBds^.Last < LtBds^.Last) then
      begin
        NewBds := ExSetAddBounds(NewBds, LtBds^.First, RtBds^.Last);
        RtBds := RtBds^.Next
      end
      else if (RtBds^.First <= LtBds^.First)
              and (RtBds^.Last >= LtBds^.Last) then
      begin
        NewBds := ExSetAddBounds(NewBds, LtBds^.First, LtBds^.Last);
        LtBds := LtBds^.Next
      end
      else if (RtBds^.First > LtBds^.First)
              and (RtBds^.Last < LtBds^.Last) then
      begin
        NewBds := ExSetAddBounds(NewBds, RtBds^.First, RtBds^.Last);
        RtBds := RtBds^.Next
      end
      else if (RtBds^.First < LtBds^.Last)
              and (RtBds^.Last >= LtBds^.Last) then
      begin
        NewBds := ExSetAddBounds(NewBds, RtBds^.First, LtBds^.Last);
        LtBds := LtBds^.Next
      end
      else LtBds := LtBds^.Next
    end;
    Result := ExSetConstant(NewBds, Left^.TypePtr);
    ExDispose(Left);
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoMul, Left^.TypePtr)
end;

function ExOpNeg(Expr : TExpression) : TExpression;
begin
  EnsureNumericExpr(Expr);
  if ExIsImmediate(Expr) then
  begin
    with Expr^.Immediate do
      if Cls = XicInteger then IntegerVal := -IntegerVal
      else RealVal := -RealVal;
    Result := Expr
  end
  else
    Result := _ExOp_MakeUnary(Expr, XoNeg, Expr^.TypePtr)
end;

function ExOpAdd(Left, Right : TExpression) : TExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
    Result := _ExOpAdd_Integers(Left, Right)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpAdd_Numbers(Left, Right)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpAdd_Strings(Left, Right)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := _ExOpUnion_Sets(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoAdd)
end;

function ExOpSub(Left, Right : TExpression) : TExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
    Result := _ExOpSub_Integers(Left, Right)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpSub_Numbers(Left, Right)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := _ExOpDifference_Sets(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoSub)
end;

function ExOpMul(Left, Right : TExpression) : TExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
    Result := _ExOpMul_Integers(Left, Right)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpMul_Numbers(Left, Right)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := _ExOpIntersection_Sets(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoMul)
end;

function ExOpDivReal(Left, Right : TExpression) : TExpression;
begin
  if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
    Result := _ExOpDiv_Numbers(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoDivReal)
end;

function ExOpDivInt(Left, Right : TExpression) : TExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
    Result := _ExOpDiv_Integers(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoDivInt)
end;

function ExOpMod(Left, Right : TExpression) : TExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
    Result := _ExOpMod_Integers(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoMod)
end;

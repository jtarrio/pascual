function _ExOpArithmetic_Integers(Left, Right : TExpression;
                                  Op : TExOperator) : TExpression;
var 
  HasLtNull, HasRtNull, HasLtNeg, HasRtNeg : boolean;
  LtNull, RtNull, LtNeg, RtNeg : integer;
  Lt, Rt, Ret : integer;
begin
  HasLtNull := false;
  HasRtNull := false;
  HasLtNeg := false;
  HasRtNeg := false;
  LtNull := 0;
  RtNull := 0;
  LtNeg := 0;
  RtNeg := 0;
  case Op of 
    XoAdd:
           begin
             HasLtNull := true;
             HasRtNull := true;
           end;
    XoSub:
           begin
             HasLtNeg := true;
             HasRtNull := true;
           end;
    XoMul:
           begin
             HasLtNull := true;
             LtNull := 1;
             HasRtNull := true;
             RtNull := 1;
             HasLtNeg := true;
             LtNeg := -1;
             HasRtNeg := true;
             RtNeg := -1;
           end;
    XoDivInt:
              begin
                HasRtNull := true;
                RtNull := 1;
                HasRtNeg := true;
                RtNeg := -1;
              end;
  end;
  if HasLtNull and ExIsImmediate(Left)
     and (Left^.Immediate.IntegerVal = LtNull) then
  begin
    ExDispose(Left);
    Result := Right
  end
  else if HasRtNull and ExIsImmediate(Right)
          and (Right^.Immediate.IntegerVal = RtNull) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if HasLtNeg and ExIsImmediate(Left)
          and (Left^.Immediate.IntegerVal = LtNeg) then
  begin
    ExDispose(Left);
    Result := ExOpNeg(Right)
  end
  else if HasRtNeg and ExIsImmediate(Right)
          and (Right^.Immediate.IntegerVal = RtNeg) then
  begin
    ExDispose(Right);
    Result := ExOpNeg(Left)
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Lt := Left^.Immediate.IntegerVal;
    Rt := Right^.Immediate.IntegerVal;
    case Op of 
      XoAdd: Ret := Lt + Rt;
      XoSub: Ret := Lt - Rt;
      XoMul: Ret := Lt * Rt;
      XoDivInt: Ret := Lt div Rt;
      XoMod: Ret := Lt mod Rt;
    end;
    Result := Left;
    Result^.Immediate.IntegerVal := Ret;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtInteger)
end;

function _ExOpArithmetic_Numbers(Left, Right : TExpression;
                                 Op : TExOperator) : TExpression;
var 
  HasLtNull, HasRtNull, HasLtNeg, HasRtNeg : boolean;
  LtNull, RtNull, LtNeg, RtNeg : real;
  Lt, Rt, Ret : real;
begin
  HasLtNull := false;
  HasRtNull := false;
  HasLtNeg := false;
  HasRtNeg := false;
  LtNull := 0;
  RtNull := 0;
  LtNeg := 0;
  RtNeg := 0;
  case Op of 
    XoAdd:
           begin
             HasLtNull := true;
             HasRtNull := true;
           end;
    XoSub:
           begin
             HasLtNeg := true;
             HasRtNull := true;
           end;
    XoMul:
           begin
             HasLtNull := true;
             LtNull := 1;
             HasRtNull := true;
             RtNull := 1;
             HasRtNeg := true;
             RtNeg := -1;
           end;
    XoDivReal:
               begin
                 HasRtNull := true;
                 RtNull := 1;
                 HasRtNeg := true;
                 RtNeg := -1;
               end;
  end;
  Left := ExCoerce(Left, PrimitiveTypes.PtReal);
  Right := ExCoerce(Right, PrimitiveTypes.PtReal);
  if HasLtNull and ExIsImmediate(Left)
     and (Left^.Immediate.RealVal = LtNull) then
  begin
    ExDispose(Left);
    Result := Right
  end
  else if HasRtNull and ExIsImmediate(Right)
          and (Right^.Immediate.RealVal = RtNull) then
  begin
    ExDispose(Right);
    Result := Left
  end
  else if HasLtNeg and ExIsImmediate(Left)
          and (Left^.Immediate.RealVal = LtNeg) then
  begin
    ExDispose(Left);
    Result := ExOpNeg(Right)
  end
  else if HasRtNeg and ExIsImmediate(Right)
          and (Right^.Immediate.RealVal = RtNeg) then
  begin
    ExDispose(Right);
    Result := ExOpNeg(Left)
  end
  else if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Lt := Left^.Immediate.RealVal;
    Rt := Right^.Immediate.RealVal;
    case Op of 
      XoAdd: Ret := Lt + Rt;
      XoSub: Ret := Lt - Rt;
      XoMul: Ret := Lt * Rt;
      XoDivReal: Ret := Lt / Rt;
    end;
    Result := Left;
    Result^.Immediate.RealVal := Ret;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtReal)
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
    Result := _ExOpArithmetic_Integers(Left, Right, XoAdd)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpArithmetic_Numbers(Left, Right, XoAdd)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpAdd_Strings(Left, Right)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := _ExOpUnion_Sets(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoAdd)
end;

function ExOpSub(Left, Right : TExpression) : TExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
    Result := _ExOpArithmetic_Integers(Left, Right, XoSub)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpArithmetic_Numbers(Left, Right, XoSub)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := _ExOpDifference_Sets(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoSub)
end;

function ExOpMul(Left, Right : TExpression) : TExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
    Result := _ExOpArithmetic_Integers(Left, Right, XoMul)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpArithmetic_Numbers(Left, Right, XoMul)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := _ExOpIntersection_Sets(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoMul)
end;

function ExOpDivReal(Left, Right : TExpression) : TExpression;
begin
  if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr)
     and (IsRealType(Left^.TypePtr) or IsRealType(Right^.TypePtr)) then
    Result := _ExOpArithmetic_Numbers(Left, Right, XoDivReal)
  else ErrorInvalidOperator2(Left, Right, XoDivReal)
end;

function ExOpDivInt(Left, Right : TExpression) : TExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
    Result := _ExOpArithmetic_Integers(Left, Right, XoDivInt)
  else ErrorInvalidOperator2(Left, Right, XoDivInt)
end;

function ExOpMod(Left, Right : TExpression) : TExpression;
begin
  if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
    Result := _ExOpArithmetic_Integers(Left, Right, XoMod)
  else ErrorInvalidOperator2(Left, Right, XoMod)
end;

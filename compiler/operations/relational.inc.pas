function _ExOpRelational_Booleans(Left, Right : TExpression;
                               Op : TExOperator) : TExpression;
var Lt, Rt, Ret : boolean;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Lt := Left^.Immediate.BooleanVal;
    Rt := Right^.Immediate.BooleanVal;
    case Op of 
      XoEq: Ret := Lt = Rt;
      XoNe: Ret := Lt <> Rt;
      XoLt: Ret := Lt < Rt;
      XoGt: Ret := Lt > Rt;
      XoLtEq: Ret := Lt <= Rt;
      XoGtEq: Ret := Lt >= Rt;
    end;
    Result := Left;
    Result^.Immediate.BooleanVal := Ret;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtBoolean)
end;

function _ExOpRelational_Integers(Left, Right : TExpression;
                               Op : TExOperator) : TExpression;
var 
  Lt, Rt : integer;
  Ret : boolean;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Lt := Left^.Immediate.IntegerVal;
    Rt := Right^.Immediate.IntegerVal;
    case Op of 
      XoEq: Ret := Lt = Rt;
      XoNe: Ret := Lt <> Rt;
      XoLt: Ret := Lt < Rt;
      XoGt: Ret := Lt > Rt;
      XoLtEq: Ret := Lt <= Rt;
      XoGtEq: Ret := Lt >= Rt;
    end;
    Result := Left;
    Result^.Immediate.BooleanVal := Ret;
    Result^.Immediate.Cls := XicBoolean;
    Result^.TypePtr := PrimitiveTypes.PtBoolean;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtBoolean)
end;

function _ExOpRelational_Numbers(Left, Right : TExpression;
                              Op : TExOperator) : TExpression;
var 
  Lt, Rt : real;
  Ret : boolean;
begin
  Left := ExCoerce(Left, PrimitiveTypes.PtReal);
  Right := ExCoerce(Right, PrimitiveTypes.PtReal);
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Lt := Left^.Immediate.RealVal;
    Rt := Right^.Immediate.RealVal;
    case Op of 
      XoEq: Ret := Lt = Rt;
      XoNe: Ret := Lt <> Rt;
      XoLt: Ret := Lt < Rt;
      XoGt: Ret := Lt > Rt;
      XoLtEq: Ret := Lt <= Rt;
      XoGtEq: Ret := Lt >= Rt;
    end;
    Result := Left;
    Result^.Immediate.BooleanVal := Ret;
    Result^.Immediate.Cls := XicBoolean;
    Result^.TypePtr := PrimitiveTypes.PtBoolean;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtBoolean)
end;

function _ExOpRelational_Strings(Left, Right : TExpression;
                              Op : TExOperator) : TExpression;
var 
  Lt, Rt : string;
  Ret : boolean;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    if Left^.Immediate.Cls = XicChar then Lt := Left^.Immediate.CharVal
    else Lt := Left^.Immediate.StringVal;
    if Right^.Immediate.Cls = XicChar then Rt := Right^.Immediate.CharVal
    else Rt := Right^.Immediate.StringVal;
    case Op of 
      XoEq: Ret := Lt = Rt;
      XoNe: Ret := Lt <> Rt;
      XoLt: Ret := Lt < Rt;
      XoGt: Ret := Lt > Rt;
      XoLtEq: Ret := Lt <= Rt;
      XoGtEq: Ret := Lt >= Rt;
    end;
    Result := Left;
    Result^.Immediate.BooleanVal := Ret;
    Result^.Immediate.Cls := XicBoolean;
    Result^.TypePtr := PrimitiveTypes.PtBoolean;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtBoolean)
end;

function _ExOpRelational_Enums(Left, Right : TExpression;
                            Op : TExOperator) : TExpression;
var 
  Lt, Rt : integer;
  Ret : boolean;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Lt := Left^.Immediate.EnumOrdinal;
    Rt := Right^.Immediate.EnumOrdinal;
    case Op of 
      XoEq: Ret := Lt = Rt;
      XoNe: Ret := Lt <> Rt;
      XoLt: Ret := Lt < Rt;
      XoGt: Ret := Lt > Rt;
      XoLtEq: Ret := Lt <= Rt;
      XoGtEq: Ret := Lt >= Rt;
    end;
    Result := Left;
    Result^.Immediate.BooleanVal := Ret;
    Result^.Immediate.Cls := XicBoolean;
    Result^.TypePtr := PrimitiveTypes.PtBoolean;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtBoolean)
end;

function _ExOpEq_Sets(Left, Right : TExpression) : TExpression;
var 
  LtBds, RtBds : TExSetImmBounds;
  Equals : boolean;
begin
  ExSetCoerceToCommon(Left, Right);
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    LtBds := Left^.Immediate.SetBounds;
    RtBds := Right^.Immediate.SetBounds;
    Equals := true;
    while (LtBds <> nil) and (RtBds <> nil) and Equals do
    begin
      Equals := (RtBds^.First = LtBds^.First) and (RtBds^.Last = LtBds^.Last);
      LtBds := LtBds^.Next;
      RtBds := RtBds^.Next
    end;
    Equals := Equals and (LtBds = nil) and (RtBds = nil);
    Result := ExBooleanConstant(Equals);
    ExDispose(Left);
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoEq, PrimitiveTypes.PtBoolean)
end;

function _ExOpGtEq_Sets(Left, Right : TExpression) : TExpression;
var 
  LtBds, RtBds : TExSetImmBounds;
  IsGtEq : boolean;
begin
  ExSetCoerceToCommon(Left, Right);
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    LtBds := Left^.Immediate.SetBounds;
    RtBds := Right^.Immediate.SetBounds;
    IsGtEq := true;
    while (LtBds <> nil) and (RtBds <> nil) and IsGtEq do
    begin
      if (LtBds^.Last < RtBds^.First) then
        LtBds := LtBds^.Next
      else if (LtBds^.First <= RtBds^.First)
              and (LtBds^.Last >= RtBds^.Last) then
             RtBds := RtBds^.Next
      else
        IsGtEq := false
    end;
    IsGtEq := IsGtEq and (RtBds = nil);
    Result := ExBooleanConstant(IsGtEq);
    ExDispose(Left);
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, XoGtEq, PrimitiveTypes.PtBoolean)
end;

function _ExOpIn_Impl(Needle, Haystack : TExpression) : TExpression;
var 
  ElemType : TPsTypePtr;
  ImmSet : TExpression;
  ExprSet : TExpression;
  ImmBounds : TExSetImmBounds;
  ExprBounds : TExSetExprBounds;
  Cond : TExpression;
  TmpVar : TPsVarPtr;
  Wanted : TExpression;
begin
  ElemType := Haystack^.TypePtr^.ElementTypePtr;
  if ElemType = nil then ElemType := Needle^.TypePtr
  else Needle := ExCoerce(Needle, ElemType);
  if Needle^.IsFunctionResult then
  begin
    TmpVar := AddTmpVariable('elem', ElemType);
    Wanted := ExVariable(TmpVar)
  end
  else
  begin
    TmpVar := nil;
    Wanted := Needle;
  end;
  Result := ExBooleanConstant(false);
  if ExIsImmediate(Haystack) then
  begin
    ImmSet := Haystack;
    ExprSet := nil
  end
  else
  begin
    ImmSet := Haystack^.SetBase;
    ExprSet := Haystack
  end;
  if ImmSet <> nil then
  begin
    ImmBounds := ImmSet^.Immediate.SetBounds;
    while ImmBounds <> nil do
    begin
      if ImmBounds^.First = ImmBounds^.Last then
        Cond := ExOpEq(ExCopy(Wanted),
                ExGetAntiOrdinal(ImmBounds^.First, ElemType))
      else
        Cond := ExOpAnd(
                ExOpLtEq(ExGetAntiOrdinal(ImmBounds^.First, ElemType),
                ExCopy(Wanted)),
                ExOpLtEq(ExCopy(Wanted),
                ExGetAntiOrdinal(ImmBounds^.Last, ElemType)));
      Result := ExOpOr(Result, Cond);
      ImmBounds := ImmBounds^.Next
    end
  end;
  if ExprSet <> nil then
  begin
    ExprBounds := ExprSet^.SetBounds;
    while ExprBounds <> nil do
    begin
      if ExprBounds^.Last = nil then
        Cond := ExOpEq(ExCopy(Wanted), ExCopy(ExprBounds^.First))
      else
        Cond := ExOpAnd(
                ExOpLtEq(ExCopy(ExprBounds^.First),
                ExCopy(Wanted)),
                ExOpLtEq(ExCopy(Wanted),
                ExCopy(ExprBounds^.Last)));
      Result := ExOpOr(Result, Cond);
      ExprBounds := ExprBounds^.Next
    end
  end;
  if TmpVar <> nil then
  begin
    Result := ExWithTmpVar(Wanted, Needle, Result);
  end
  else ExDispose(Needle);
  ExDispose(Haystack)
end;

function _ExOpEq_Pointers(Left, Right : TExpression) : TExpression;
begin
  Result := _ExOp_MakeBinary(Left, Right, XoEq, PrimitiveTypes.PtBoolean)
end;

function _ExOpNe_Pointers(Left, Right : TExpression) : TExpression;
begin
  Result := _ExOp_MakeBinary(Left, Right, XoNe, PrimitiveTypes.PtBoolean)
end;

function ExOpIn(Left, Right : TExpression) : TExpression;
begin
  if IsSetType(Right^.TypePtr) then
  begin
    if ExIsImmediate(Right) or (Right^.Cls = XcSet) then
      Result := _ExOpIn_Impl(Left, Right)
    else Result := _ExOp_MakeBinary(Left, Right, XoIn, PrimitiveTypes.PtBoolean)
  end
  else ErrorInvalidOperator2(Left, Right, XoXor)
end;

function ExOpEq(Left, Right : TExpression) : TExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpRelational_Booleans(Left, Right, XoEq)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpRelational_Integers(Left, Right, XoEq)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpRelational_Numbers(Left, Right, XoEq)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpRelational_Strings(Left, Right, XoEq)
  else if IsEnumType(Left^.TypePtr)
          and IsSameType(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpRelational_Enums(Left, Right, XoEq)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := _ExOpEq_Sets(Left, Right)
  else if ArePointersCompatible(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpEq_Pointers(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoEq)
end;

function ExOpNe(Left, Right : TExpression) : TExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpRelational_Booleans(Left, Right, XoNe)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpRelational_Integers(Left, Right, XoNe)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpRelational_Numbers(Left, Right, XoNe)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpRelational_Strings(Left, Right, XoNe)
  else if IsEnumType(Left^.TypePtr)
          and IsSameType(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpRelational_Enums(Left, Right, XoNe)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := ExOpNot(_ExOpEq_Sets(Left, Right))
  else if ArePointersCompatible(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpNe_Pointers(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoNe)
end;

function ExOpLt(Left, Right : TExpression) : TExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpRelational_Booleans(Left, Right, XoLt)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpRelational_Integers(Left, Right, XoLt)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpRelational_Numbers(Left, Right, XoLt)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpRelational_Strings(Left, Right, XoLt)
  else if IsEnumType(Left^.TypePtr)
          and IsSameType(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpRelational_Enums(Left, Right, XoLt)
  else ErrorInvalidOperator2(Left, Right, XoLt)
end;

function ExOpGt(Left, Right : TExpression) : TExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpRelational_Booleans(Left, Right, XoGt)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpRelational_Integers(Left, Right, XoGt)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpRelational_Numbers(Left, Right, XoGt)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpRelational_Strings(Left, Right, XoGt)
  else if IsEnumType(Left^.TypePtr)
          and IsSameType(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpRelational_Enums(Left, Right, XoGt)
  else ErrorInvalidOperator2(Left, Right, XoGt)
end;

function ExOpLtEq(Left, Right : TExpression) : TExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpRelational_Booleans(Left, Right, XoLtEq)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpRelational_Integers(Left, Right, XoLtEq)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpRelational_Numbers(Left, Right, XoLtEq)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpRelational_Strings(Left, Right, XoLtEq)
  else if IsEnumType(Left^.TypePtr)
          and IsSameType(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpRelational_Enums(Left, Right, XoLtEq)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := _ExOpGtEq_Sets(Right, Left)
  else ErrorInvalidOperator2(Left, Right, XoLtEq)
end;

function ExOpGtEq(Left, Right : TExpression) : TExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpRelational_Booleans(Left, Right, XoGtEq)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpRelational_Integers(Left, Right, XoGtEq)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpRelational_Numbers(Left, Right, XoGtEq)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpRelational_Strings(Left, Right, XoGtEq)
  else if IsEnumType(Left^.TypePtr)
          and IsSameType(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpRelational_Enums(Left, Right, XoGtEq)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := _ExOpGtEq_Sets(Left, Right)
  else ErrorInvalidOperator2(Left, Right, XoGtEq)
end;

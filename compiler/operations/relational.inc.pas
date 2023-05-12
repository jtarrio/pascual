function _ExOpRelational_Booleans(Left, Right : TSExpression;
                                  Op : TSEOperator) : TSExpression;
var Lt, Rt, Ret : boolean;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Lt := Left^.Immediate.BooleanVal;
    Rt := Right^.Immediate.BooleanVal;
    case Op of 
      SeoEq: Ret := Lt = Rt;
      SeoNe: Ret := Lt <> Rt;
      SeoLt: Ret := Lt < Rt;
      SeoGt: Ret := Lt > Rt;
      SeoLtEq: Ret := Lt <= Rt;
      SeoGtEq: Ret := Lt >= Rt;
    end;
    Result := Left;
    Result^.Immediate.BooleanVal := Ret;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtBoolean)
end;

function _ExOpRelational_Integers(Left, Right : TSExpression;
                                  Op : TSEOperator) : TSExpression;
var 
  Lt, Rt : integer;
  Ret : boolean;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Lt := Left^.Immediate.IntegerVal;
    Rt := Right^.Immediate.IntegerVal;
    case Op of 
      SeoEq: Ret := Lt = Rt;
      SeoNe: Ret := Lt <> Rt;
      SeoLt: Ret := Lt < Rt;
      SeoGt: Ret := Lt > Rt;
      SeoLtEq: Ret := Lt <= Rt;
      SeoGtEq: Ret := Lt >= Rt;
    end;
    Result := Left;
    Result^.Immediate.BooleanVal := Ret;
    Result^.Immediate.Cls := SeicBoolean;
    Result^.TypePtr := PrimitiveTypes.PtBoolean;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtBoolean)
end;

function _ExOpRelational_Numbers(Left, Right : TSExpression;
                                 Op : TSEOperator) : TSExpression;
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
      SeoEq: Ret := Lt = Rt;
      SeoNe: Ret := Lt <> Rt;
      SeoLt: Ret := Lt < Rt;
      SeoGt: Ret := Lt > Rt;
      SeoLtEq: Ret := Lt <= Rt;
      SeoGtEq: Ret := Lt >= Rt;
    end;
    Result := Left;
    Result^.Immediate.BooleanVal := Ret;
    Result^.Immediate.Cls := SeicBoolean;
    Result^.TypePtr := PrimitiveTypes.PtBoolean;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtBoolean)
end;

function _ExOpRelational_Strings(Left, Right : TSExpression;
                                 Op : TSEOperator) : TSExpression;
var 
  Lt, Rt : string;
  Ret : boolean;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    if Left^.Immediate.Cls = SeicChar then Lt := Left^.Immediate.CharVal
    else Lt := Left^.Immediate.StringVal;
    if Right^.Immediate.Cls = SeicChar then Rt := Right^.Immediate.CharVal
    else Rt := Right^.Immediate.StringVal;
    case Op of 
      SeoEq: Ret := Lt = Rt;
      SeoNe: Ret := Lt <> Rt;
      SeoLt: Ret := Lt < Rt;
      SeoGt: Ret := Lt > Rt;
      SeoLtEq: Ret := Lt <= Rt;
      SeoGtEq: Ret := Lt >= Rt;
    end;
    Result := Left;
    Result^.Immediate.BooleanVal := Ret;
    Result^.Immediate.Cls := SeicBoolean;
    Result^.TypePtr := PrimitiveTypes.PtBoolean;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtBoolean)
end;

function _ExOpRelational_Enums(Left, Right : TSExpression;
                               Op : TSEOperator) : TSExpression;
var 
  Lt, Rt : integer;
  Ret : boolean;
begin
  if ExIsImmediate(Left) and ExIsImmediate(Right) then
  begin
    Lt := Left^.Immediate.EnumOrdinal;
    Rt := Right^.Immediate.EnumOrdinal;
    case Op of 
      SeoEq: Ret := Lt = Rt;
      SeoNe: Ret := Lt <> Rt;
      SeoLt: Ret := Lt < Rt;
      SeoGt: Ret := Lt > Rt;
      SeoLtEq: Ret := Lt <= Rt;
      SeoGtEq: Ret := Lt >= Rt;
    end;
    Result := Left;
    Result^.Immediate.BooleanVal := Ret;
    Result^.Immediate.Cls := SeicBoolean;
    Result^.TypePtr := PrimitiveTypes.PtBoolean;
    ExDispose(Right)
  end
  else
    Result := _ExOp_MakeBinary(Left, Right, Op, PrimitiveTypes.PtBoolean)
end;

function _ExOpEq_Sets(Left, Right : TSExpression) : TSExpression;
var 
  LtBds, RtBds : TSESetImmBounds;
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
    Result := _ExOp_MakeBinary(Left, Right, SeoEq, PrimitiveTypes.PtBoolean)
end;

function _ExOpGtEq_Sets(Left, Right : TSExpression) : TSExpression;
var 
  LtBds, RtBds : TSESetImmBounds;
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
    Result := _ExOp_MakeBinary(Left, Right, SeoGtEq, PrimitiveTypes.PtBoolean)
end;

function _ExOpIn_Impl(Needle, Haystack : TSExpression) : TSExpression;
var 
  ElemType : TSDType;
  ImmSet : TSExpression;
  ExprSet : TSExpression;
  ImmBounds : TSESetImmBounds;
  ExprBounds : TSESetExprBounds;
  Cond : TSExpression;
  TmpVar : TSDVariable;
  Wanted : TSExpression;
begin
  ElemType := Haystack^.TypePtr^.ElementTypePtr;
  if ElemType = nil then ElemType := Needle^.TypePtr
  else Needle := ExCoerce(Needle, ElemType);
  if Needle^.IsFunctionResult then
  begin
    TmpVar := AddAliasVariable(ElemType, Needle);
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

function _ExOpEq_Pointers(Left, Right : TSExpression) : TSExpression;
begin
  Result := _ExOp_MakeBinary(Left, Right, SeoEq, PrimitiveTypes.PtBoolean)
end;

function _ExOpNe_Pointers(Left, Right : TSExpression) : TSExpression;
begin
  Result := _ExOp_MakeBinary(Left, Right, SeoNe, PrimitiveTypes.PtBoolean)
end;

function _ExOpEq_Functions(Left, Right : TSExpression) : TSExpression;
begin
  Result := _ExOp_MakeBinary(Left, Right, SeoEq, PrimitiveTypes.PtBoolean)
end;

function _ExOpNe_Functions(Left, Right : TSExpression) : TSExpression;
begin
  Result := _ExOp_MakeBinary(Left, Right, SeoNe, PrimitiveTypes.PtBoolean)
end;

function ExOpIn(Left, Right : TSExpression) : TSExpression;
begin
  if IsSetType(Right^.TypePtr) then
  begin
    if ExIsImmediate(Right) or (Right^.Cls = SecSetValue) then
      Result := _ExOpIn_Impl(Left, Right)
    else Result := _ExOp_MakeBinary(Left, Right, SeoIn,
                   PrimitiveTypes.PtBoolean)
  end
  else ErrorInvalidOperator2(Left, Right, SeoXor)
end;

function ExOpEq(Left, Right : TSExpression) : TSExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpRelational_Booleans(Left, Right, SeoEq)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpRelational_Integers(Left, Right, SeoEq)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpRelational_Numbers(Left, Right, SeoEq)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpRelational_Strings(Left, Right, SeoEq)
  else if IsEnumType(Left^.TypePtr)
          and IsSameType(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpRelational_Enums(Left, Right, SeoEq)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := _ExOpEq_Sets(Left, Right)
  else if ArePointersCompatible(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpEq_Pointers(Left, Right)
  else if AreFunctionsCompatible(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpEq_Functions(Left, Right)
  else ErrorInvalidOperator2(Left, Right, SeoEq)
end;

function ExOpNe(Left, Right : TSExpression) : TSExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpRelational_Booleans(Left, Right, SeoNe)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpRelational_Integers(Left, Right, SeoNe)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpRelational_Numbers(Left, Right, SeoNe)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpRelational_Strings(Left, Right, SeoNe)
  else if IsEnumType(Left^.TypePtr)
          and IsSameType(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpRelational_Enums(Left, Right, SeoNe)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := ExOpNot(_ExOpEq_Sets(Left, Right))
  else if ArePointersCompatible(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpNe_Pointers(Left, Right)
  else if AreFunctionsCompatible(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpNe_Functions(Left, Right)
  else ErrorInvalidOperator2(Left, Right, SeoNe)
end;

function ExOpLt(Left, Right : TSExpression) : TSExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpRelational_Booleans(Left, Right, SeoLt)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpRelational_Integers(Left, Right, SeoLt)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpRelational_Numbers(Left, Right, SeoLt)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpRelational_Strings(Left, Right, SeoLt)
  else if IsEnumType(Left^.TypePtr)
          and IsSameType(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpRelational_Enums(Left, Right, SeoLt)
  else ErrorInvalidOperator2(Left, Right, SeoLt)
end;

function ExOpGt(Left, Right : TSExpression) : TSExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpRelational_Booleans(Left, Right, SeoGt)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpRelational_Integers(Left, Right, SeoGt)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpRelational_Numbers(Left, Right, SeoGt)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpRelational_Strings(Left, Right, SeoGt)
  else if IsEnumType(Left^.TypePtr)
          and IsSameType(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpRelational_Enums(Left, Right, SeoGt)
  else ErrorInvalidOperator2(Left, Right, SeoGt)
end;

function ExOpLtEq(Left, Right : TSExpression) : TSExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpRelational_Booleans(Left, Right, SeoLtEq)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpRelational_Integers(Left, Right, SeoLtEq)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpRelational_Numbers(Left, Right, SeoLtEq)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpRelational_Strings(Left, Right, SeoLtEq)
  else if IsEnumType(Left^.TypePtr)
          and IsSameType(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpRelational_Enums(Left, Right, SeoLtEq)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := _ExOpGtEq_Sets(Right, Left)
  else ErrorInvalidOperator2(Left, Right, SeoLtEq)
end;

function ExOpGtEq(Left, Right : TSExpression) : TSExpression;
begin
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    Result := _ExOpRelational_Booleans(Left, Right, SeoGtEq)
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
         Result := _ExOpRelational_Integers(Left, Right, SeoGtEq)
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
         Result := _ExOpRelational_Numbers(Left, Right, SeoGtEq)
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
         Result := _ExOpRelational_Strings(Left, Right, SeoGtEq)
  else if IsEnumType(Left^.TypePtr)
          and IsSameType(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExOpRelational_Enums(Left, Right, SeoGtEq)
  else if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
         Result := _ExOpGtEq_Sets(Left, Right)
  else ErrorInvalidOperator2(Left, Right, SeoGtEq)
end;

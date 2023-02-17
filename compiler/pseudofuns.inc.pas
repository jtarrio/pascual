function _Pf_Unary_Parse : TExpression;
begin
  WantTokenAndRead(TkLparen);
  Result := PsExpression;
  WantTokenAndRead(TkRparen)
end;

function PfConcat_Parse(FnExpr : TExpression) : TExpression;
var Operand : TExpression;
begin
  DisposeExpr(FnExpr);
  Result := nil;
  WantTokenAndRead(TkLparen);
  repeat
    Operand := PSExpression;
    if not IsStringyType(Operand^.TypePtr) then
      CompileError('Argument for CONCAT is not a string');
    if Result = nil then Result := Operand
    else Result := ExBinaryOp(Result, Operand, TkPlus);
    WantToken2(TkComma, TkRparen);
    SkipToken(TkComma)
  until Lexer.Token.Id = TkRparen;
  WantTokenAndRead(TkRparen)
end;

function PfDispose_Parse(FnExpr : TExpression) : TExpression;
var Ptr : TExpression;
begin
  Ptr := _Pf_Unary_Parse;
  if not Ptr^.IsAssignable or not IsPointerType(Ptr^.TypePtr) then
    CompileError('Argument for DISPOSE is not a pointer');
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := Ptr
end;

function PfNew_Parse(FnExpr : TExpression) : TExpression;
var Ptr : TExpression;
begin
  Ptr := _Pf_Unary_Parse;
  if not Ptr^.IsAssignable or not IsPointerType(Ptr^.TypePtr) then
    CompileError('Argument for NEW is not a pointer');
  ExMarkInitialized(Ptr);
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := Ptr
end;

function PfOrd_Parse(FnExpr : TExpression) : TExpression;
begin
  DisposeExpr(FnExpr);
  Result := PfOrd(_Pf_Unary_Parse)
end;

function PfOrd(Arg : TExpression) : TExpression;
var Imm : TExImmediate;
begin
  if not IsOrdinalType(Arg^.TypePtr) then
    CompileError('Argument for ORD does not have an ordinal type');
  if ExIsImmediate(Arg) then
  begin
    Imm := Arg^.Immediate;
    DisposeExpr(Arg);
    case Imm.Cls of 
      XicBoolean : if Imm.BooleanVal then Result := ExIntegerConstant(1)
                   else Result := ExIntegerConstant(0);
      XicInteger: Result := ExIntegerConstant(Imm.IntegerVal);
      XicChar: Result := ExIntegerConstant(Ord(Imm.CharVal));
      XicEnum: Result := ExIntegerConstant(Imm.EnumOrdinal);
      else CompileError('Invalid type for ORD')
    end
  end
  else
  begin
    Result := ExPseudoFnCall(ExPseudoFn(TpfOrd));
    Result^.PseudoFnCall.Arg1 := Arg;
    Result^.TypePtr := PrimitiveTypes.PtInteger
  end
end;

function PfPred_Parse(FnExpr : TExpression) : TExpression;
begin
  DisposeExpr(FnExpr);
  Result := PfPred(_Pf_Unary_Parse)
end;

function PfPred(Arg : TExpression) : TExpression;
var 
  Imm : TExImmediate;
  OutOfBounds : boolean;
begin
  if not IsOrdinalType(Arg^.TypePtr) then
    CompileError('Argument for PRED does not have an ordinal type');
  if ExIsImmediate(Arg) then
  begin
    Imm := Arg^.Immediate;
    OutOfBounds := false;
    case Imm.Cls of 
      XicBoolean : if Imm.BooleanVal then Result := ExBooleanConstant(false)
                   else OutOfBounds := true;
      XicInteger: Result := ExIntegerConstant(Imm.IntegerVal - 1);
      XicChar: if Ord(Imm.CharVal) > 0 then
                 Result := ExCharConstant(Pred(Imm.CharVal));
      XicEnum: if Imm.EnumOrdinal > 0 then
                 Result := ExEnumConstant(Pred(Imm.EnumOrdinal), Arg^.TypePtr);
      else CompileError('Invalid type for PRED')
    end;
    DisposeExpr(Arg);
    if OutOfBounds then CompileError('Out of bounds in PRED')
  end
  else
  begin
    Result := ExPseudoFnCall(ExPseudoFn(TpfPred));
    Result^.PseudoFnCall.Arg1 := Arg;
    Result^.TypePtr := Arg^.TypePtr
  end
end;

function PfRead_Parse(FnExpr : TExpression) : TExpression;
var 
  First : boolean;
  OutVar : TExpression;
  ReadArg : ^TExReadArgs;
begin
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := ExVariable(FindNameOfClass('INPUT',
                               TncVariable, {Required=}true)^.VarPtr);
  ReadArg := nil;
  if Lexer.Token.Id = TkLparen then
  begin
    First := true;
    WantTokenAndRead(TkLparen);
    while Lexer.Token.Id <> TkRparen do
    begin
      OutVar := PsExpression;
      if First and OutVar^.IsAssignable and IsTextType(OutVar^.TypePtr) then
      begin
        DisposeExpr(Result^.PseudoFnCall.Arg1);
        Result^.PseudoFnCall.Arg1 := OutVar
      end
      else
      begin
        if not OutVar^.IsAssignable
           or not IsStringyType(OutVar^.TypePtr) then
          CompileError('Invalid argument for READ');
        if ReadArg = nil then
        begin
          new(Result^.PseudoFnCall.ReadArgs);
          ReadArg := Result^.PseudoFnCall.ReadArgs
        end
        else
        begin
          new(ReadArg^.Next);
          ReadArg := ReadArg^.Next;
        end;
        ReadArg^.Next := nil;
        ReadArg^.Arg := OutVar;
        ExMarkInitialized(OutVar)
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      First := false
    end;
    WantTokenAndRead(TkRparen)
  end
end;

function PfStr_Parse(FnExpr : TExpression) : TExpression;
var 
  Src, Dest : TExpression;
begin
  WantTokenAndRead(TkLparen);
  Src := ExOutrange(PsExpression);
  WantTokenAndRead(TkComma);
  Dest := PsExpression;
  WantTokenAndRead(TkRparen);
  if not Dest^.IsAssignable or not IsStringType(Dest^.TypePtr) then
    CompileError('Second argument for STR is not a string variable');
  if not IsBooleanType(Src^.TypePtr) and not IsIntegerType(Src^.TypePtr)
     and not IsRealType(Src^.TypePtr) and not IsEnumType(Src^.TypePtr) then
    CompileError('First argument for STR has an invalid type: ' +
                 TypeName(Src^.TypePtr));
  ExMarkInitialized(Dest);
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := Src;
  Result^.PseudoFnCall.Arg2 := Dest
end;

function PfSucc_Parse(FnExpr : TExpression) : TExpression;
begin
  DisposeExpr(FnExpr);
  Result := PfSucc(_Pf_Unary_Parse)
end;

function PfSucc(Arg : TExpression) : TExpression;
var 
  Imm : TExImmediate;
  OutOfBounds : boolean;
begin
  if not IsOrdinalType(Arg^.TypePtr) then
    CompileError('Argument for SUCC does not have an ordinal type');
  if ExIsImmediate(Arg) then
  begin
    Imm := Arg^.Immediate;
    OutOfBounds := false;
    case Imm.Cls of 
      XicBoolean : if not Imm.BooleanVal then Result := ExBooleanConstant(true)
                   else OutOfBounds := true;
      XicInteger: Result := ExIntegerConstant(Imm.IntegerVal + 1);
      XicChar: if Ord(Imm.CharVal) < 255 then
                 Result := ExCharConstant(Succ(Imm.CharVal));
      XicEnum: if Imm.EnumOrdinal < Imm.EnumPtr^.Size - 1 then
                 Result := ExEnumConstant(Succ(Imm.EnumOrdinal), Arg^.TypePtr);
      else CompileError('Invalid type for SUCC')
    end;
    DisposeExpr(Arg);
    if OutOfBounds then CompileError('Out of bounds in SUCC')
  end
  else
  begin
    Result := ExPseudoFnCall(ExPseudoFn(TpfSucc));
    Result^.PseudoFnCall.Arg1 := Arg;
    Result^.TypePtr := Arg^.TypePtr
  end
end;

function PfVal_Parse(FnExpr : TExpression) : TExpression;
var 
  Src, Dest, Code : TExpression;
begin
  WantTokenAndRead(TkLparen);
  Src := ExOutrange(PsExpression);
  WantTokenAndRead(TkComma);
  Dest := PsExpression;
  WantTokenAndRead(TkComma);
  Code := PsExpression;
  WantTokenAndRead(TkRparen);
  if not IsStringType(Src^.TypePtr) then
    CompileError('First argument for VAL is not a string variable');
  if not Dest^.IsAssignable then
    CompileError('Second argument for VAL is not a variable');
  if not IsBooleanType(Dest^.TypePtr) and not IsIntegerType(Dest^.TypePtr)
     and not IsRealType(Dest^.TypePtr) and not IsEnumType(Dest^.TypePtr) then
    CompileError('Second argument for VAL has an invalid type: ' +
                 TypeName(Dest^.TypePtr));
  if not Code^.IsAssignable or not IsIntegerType(Code^.TypePtr) then
    CompileError('Third argument for VAL is not an integer variable');
  ExMarkInitialized(Dest);
  ExMarkInitialized(Code);
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := Src;
  Result^.PseudoFnCall.Arg2 := Dest;
  Result^.PseudoFnCall.Arg3 := Code;
end;


function PfWrite_Parse(FnExpr : TExpression) : TExpression;
var 
  First : boolean;
  OutExpr : TExpression;
  WriteArg : ^TExWriteArgs;
begin
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := ExVariable(FindNameOfClass('OUTPUT',
                               TncVariable, {Required=}true)^.VarPtr);
  WriteArg := nil;
  if Lexer.Token.Id = TkLparen then
  begin
    First := true;
    WantTokenAndRead(TkLparen);
    while Lexer.Token.Id <> TkRparen do
    begin
      OutExpr := PsExpression;
      if First and OutExpr^.IsAssignable and IsTextType(OutExpr^.TypePtr) then
      begin
        DisposeExpr(Result^.PseudoFnCall.Arg1);
        Result^.PseudoFnCall.Arg1 := OutExpr
      end
      else
      begin
        if WriteArg = nil then
        begin
          new(Result^.PseudoFnCall.WriteArgs);
          WriteArg := Result^.PseudoFnCall.WriteArgs
        end
        else
        begin
          new(WriteArg^.Next);
          WriteArg := WriteArg^.Next;
        end;
        WriteArg^.Next := nil;
        WriteArg^.Arg := OutExpr;
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      First := false
    end;
    WantTokenAndRead(TkRparen)
  end
end;

function Pf_Parse(Fn : TExpression) : TExpression;
begin
  case Fn^.PseudoFn of 
    TpfConcat : Result := PfConcat_Parse(Fn);
    TpfDispose : Result := PfDispose_Parse(Fn);
    TpfNew : Result := PfNew_Parse(Fn);
    TpfOrd : Result := PfOrd_Parse(Fn);
    TpfPred : Result := PfPred_Parse(Fn);
    TpfRead : Result := PfRead_Parse(Fn);
    TpfReadln : Result := PfRead_Parse(Fn);
    TpfStr : Result := PfStr_Parse(Fn);
    TpfSucc : Result := PfSucc_Parse(Fn);
    TpfVal : Result := PfVal_Parse(Fn);
    TpfWrite : Result := PfWrite_Parse(Fn);
    TpfWriteln : Result := PfWrite_Parse(Fn);
    else InternalError('Unimplemented special function ' + DescribeExpr(Fn, 5))
  end
end;

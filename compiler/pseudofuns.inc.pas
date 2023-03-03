function _Pf_Unary_Parse : TExpression;
begin
  WantTokenAndRead(TkLparen);
  Result := PsExpression;
  WantTokenAndRead(TkRparen)
end;

function _Pf_Fun_Overload(NamePrefix : string; TypePtr : TPsTypePtr) : string;
begin
  TypePtr := GetFundamentalType(TypePtr);
  if IsBooleanType(TypePtr) then Result := NamePrefix + '_b'
  else if IsIntegerType(TypePtr) then Result := NamePrefix + '_i'
  else if IsRealType(TypePtr) then Result := NamePrefix + '_r'
  else if IsCharType(TypePtr) then Result := NamePrefix + '_c'
  else if IsStringType(TypePtr) then Result := NamePrefix + '_s'
  else CompileError('Type ' + TypeName(TypePtr) + ' is not representable for ' +
    'overload of ' + NamePrefix)
end;

function _Pf_Overload_Parse(FnExpr : TExpression;
                            NamePrefix : string) : TExpression;
var 
  Arg : TExpression;
  Args : TExFunctionArgs;
  FnPtr : TPsFnPtr;
begin
  DisposeExpr(FnExpr);
  Arg := nil;
  if Lexer.Token.Id = TkLparen then
  begin
    WantTokenAndRead(TkLparen);
    if Lexer.Token.Id <> TkRparen then Arg := PsExpression;
    WantTokenAndRead(TkRparen);
  end;
  if Arg = nil then
  begin
    FnPtr := FindNameOfClass(NamePrefix + '_n', TncFunction, {Required=}true)^.
             FnPtr;
    Args.Size := 0;
    Result := ExFunctionCall(ExFnRef(FnPtr), Args);
  end
  else
  begin
    FnPtr := FindNameOfClass(_Pf_Fun_Overload(NamePrefix, Arg^.TypePtr),
             TncFunction, {Required=}true)^.FnPtr;
    Args.Size := 1;
    Args.Values[1] := Arg;
    Result := ExFunctionCall(ExFnRef(FnPtr), Args);
  end
end;

function PfAbs_Parse(FnExpr : TExpression) : TExpression;
begin
  Result := _Pf_Overload_Parse(FnExpr, 'ABS')
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

function PfRandom_Parse(FnExpr : TExpression) : TExpression;
begin
  Result := _Pf_Overload_Parse(FnExpr, 'RANDOM')
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
        if not OutVar^.IsAssignable then
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

function PfSqr_Parse(FnExpr : TExpression) : TExpression;
begin
  Result := _Pf_Overload_Parse(FnExpr, 'SQR')
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
    TpfAbs : Result := PfAbs_Parse(Fn);
    TpfConcat : Result := PfConcat_Parse(Fn);
    TpfDispose : Result := PfDispose_Parse(Fn);
    TpfNew : Result := PfNew_Parse(Fn);
    TpfOrd : Result := PfOrd_Parse(Fn);
    TpfPred : Result := PfPred_Parse(Fn);
    TpfRandom : Result := PfRandom_Parse(Fn);
    TpfRead : Result := PfRead_Parse(Fn);
    TpfReadln : Result := PfRead_Parse(Fn);
    TpfSqr : Result := PfSqr_Parse(Fn);
    TpfStr : Result := PfStr_Parse(Fn);
    TpfSucc : Result := PfSucc_Parse(Fn);
    TpfVal : Result := PfVal_Parse(Fn);
    TpfWrite : Result := PfWrite_Parse(Fn);
    TpfWriteln : Result := PfWrite_Parse(Fn);
    else InternalError('Unimplemented special function ' + DescribeExpr(Fn, 5))
  end
end;

function Pf_DescribeName(Fn : TExpression) : string;
begin
  case Fn^.PseudoFn of 
    TpfAbs: Result := 'ABS';
    TpfConcat: Result := 'CONCAT';
    TpfDispose: Result := 'DISPOSE';
    TpfNew: Result := 'NEW';
    TpfOrd: Result := 'ORD';
    TpfPred: Result := 'PRED';
    TpfRandom : Result := 'RANDOM';
    TpfRead: Result := 'READ';
    TpfReadln: Result := 'READLN';
    TpfSqr: Result := 'SQR';
    TpfStr: Result := 'STR';
    TpfSucc: Result := 'SUCC';
    TpfVal: Result := 'VAL';
    TpfWrite: Result := 'WRITE';
    TpfWriteln: Result := 'WRITELN';
    else InternalError('Cannot describe pseudofun')
  end
end;

function Pf_DescribeCall(Expr : TExpression; Levels : integer) : string;
begin
  with Expr^.PseudoFnCall do
    case PseudoFn of 
      TpfDispose: Result := 'DISPOSE(' + DescribeExpr(Arg1, Levels - 1) + ')';
      TpfNew: Result := 'NEW(' + DescribeExpr(Arg1, Levels - 1) + ')';
      TpfOrd: Result := 'ORD(' + DescribeExpr(Arg1, Levels - 1) + ')';
      TpfPred: Result := 'PRED(' + DescribeExpr(Arg1, Levels - 1) + ')';
      TpfRead: Result := 'READ(...)';
      TpfReadln: Result := 'READLN(...)';
      TpfStr: Result := 'STR(' + DescribeExpr(Arg1, Levels - 1) + ', ' +
                        DescribeExpr(Arg2, Levels - 1) + ')';
      TpfSucc: Result := 'SUCC(' + DescribeExpr(Arg1, Levels - 1) + ')';
      TpfVal: Result := 'VAL(' + DescribeExpr(Arg1, Levels - 1) + ', ' +
                        DescribeExpr(Arg2, Levels - 1) + ', ' +
                        DescribeExpr(Arg3, Levels - 1) + ')';
      TpfWrite: Result := 'WRITE(...)';
      TpfWriteln: Result := 'WRITELN(...)';
      else InternalError('Cannot describe pseudofun')
    end
end;

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
  ExDispose(FnExpr);
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
  ExDispose(FnExpr);
  Result := nil;
  WantTokenAndRead(TkLparen);
  repeat
    Operand := PSExpression;
    EnsureStringyExpr(Operand);
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
  EnsureAssignableExpr(Ptr);
  EnsurePointerExpr(Ptr);
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := Ptr
end;

function PfNew_Parse(FnExpr : TExpression) : TExpression;
var Ptr : TExpression;
begin
  Ptr := _Pf_Unary_Parse;
  EnsureAssignableExpr(Ptr);
  EnsurePointerExpr(Ptr);
  ExMarkInitialized(Ptr);
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := Ptr
end;

function PfOrd_Parse(FnExpr : TExpression) : TExpression;
begin
  ExDispose(FnExpr);
  Result := PfOrd(_Pf_Unary_Parse)
end;

function PfOrd(Arg : TExpression) : TExpression;
begin
  EnsureOrdinalExpr(Arg);
  if ExIsImmediate(Arg) then
  begin
    with Arg^.Immediate do
      case Cls of 
        XicBoolean : if BooleanVal then Result := ExIntegerConstant(1)
                     else Result := ExIntegerConstant(0);
        XicInteger: Result := ExIntegerConstant(IntegerVal);
        XicChar: Result := ExIntegerConstant(Ord(CharVal));
        XicEnum: Result := ExIntegerConstant(EnumOrdinal);
        else ErrorForExpr('Expected an ordinal', Arg)
      end;
    ExDispose(Arg)
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
  ExDispose(FnExpr);
  Result := PfPred(_Pf_Unary_Parse)
end;

function PfPred(Arg : TExpression) : TExpression;
var OutOfBounds : boolean;
begin
  EnsureOrdinalExpr(Arg);
  if ExIsImmediate(Arg) then
  begin
    OutOfBounds := false;
    with Arg^.Immediate do
      case Cls of 
        XicBoolean : if BooleanVal then Result := ExBooleanConstant(false)
                     else OutOfBounds := true;
        XicInteger: Result := ExIntegerConstant(IntegerVal - 1);
        XicChar: if Ord(CharVal) > 0 then
                   Result := ExCharConstant(Pred(CharVal))
                 else OutOfBounds := true;
        XicEnum: if EnumOrdinal > 0 then
                   Result := ExEnumConstant(Pred(EnumOrdinal), Arg^.TypePtr)
                 else OutOfBounds := true;
        else ErrorForExpr('Expected an ordinal', Arg)
      end;
    if OutOfBounds then
      ErrorForExpr('Predecessor for argument would be out of bounds', Arg);
    ExDispose(Arg)
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
        ExDispose(Result^.PseudoFnCall.Arg1);
        Result^.PseudoFnCall.Arg1 := OutVar
      end
      else
      begin
        EnsureAssignableExpr(OutVar);
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
  Width, Prec : TExpression;
begin
  Width := nil;
  Prec := nil;
  WantTokenAndRead(TkLparen);
  Src := ExOutrange(PsExpression);
  if Lexer.Token.Id = TkColon then
  begin
    WantTokenAndRead(TkColon);
    Width := PsExpression;
    EnsureIntegerExpr(Width);
    if IsRealType(Src^.TypePtr) and (Lexer.Token.Id = TkColon) then
    begin
      WantTokenAndRead(TkColon);
      Prec := PsExpression;
      EnsureIntegerExpr(Prec)
    end
  end;
  WantTokenAndRead(TkComma);
  Dest := PsExpression;
  WantTokenAndRead(TkRparen);
  EnsureAssignableExpr(Dest);
  EnsureStringExpr(Dest);
  if not IsBooleanType(Src^.TypePtr) and not IsIntegerType(Src^.TypePtr)
     and not IsRealType(Src^.TypePtr) and not IsEnumType(Src^.TypePtr) then
    ErrorForExpr('Invalid type for source of STR', Src);
  ExMarkInitialized(Dest);
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := Src;
  Result^.PseudoFnCall.Arg2 := Dest;
  Result^.PseudoFnCall.Arg3 := Width;
  Result^.PseudoFnCall.Arg4 := Prec
end;

function PfSucc_Parse(FnExpr : TExpression) : TExpression;
begin
  ExDispose(FnExpr);
  Result := PfSucc(_Pf_Unary_Parse)
end;

function PfSucc(Arg : TExpression) : TExpression;
var OutOfBounds : boolean;
begin
  EnsureOrdinalExpr(Arg);
  if ExIsImmediate(Arg) then
  begin
    OutOfBounds := false;
    with Arg^.Immediate do
      case Cls of 
        XicBoolean : if not BooleanVal then Result := ExBooleanConstant(true)
                     else OutOfBounds := true;
        XicInteger: Result := ExIntegerConstant(IntegerVal + 1);
        XicChar: if Ord(CharVal) < 255 then
                   Result := ExCharConstant(Succ(CharVal))
                 else OutOfBounds := true;
        XicEnum: if EnumOrdinal < EnumPtr^.Size - 1 then
                   Result := ExEnumConstant(Succ(EnumOrdinal), Arg^.TypePtr)
                 else OutOfBounds := true;
        else ErrorForExpr('Expected an ordinal', Arg)
      end;
    if OutOfBounds then
      ErrorForExpr('Successor for argument would be out of bounds', Arg);
    ExDispose(Arg)
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
  EnsureStringExpr(Src);
  EnsureAssignableExpr(Dest);
  if not IsBooleanType(Dest^.TypePtr) and not IsIntegerType(Dest^.TypePtr)
     and not IsRealType(Dest^.TypePtr) and not IsEnumType(Dest^.TypePtr) then
    ErrorForExpr('Invalid type for target of VAL', Dest);
  EnsureAssignableExpr(Code);
  EnsureIntegerExpr(Code);
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
        ExDispose(Result^.PseudoFnCall.Arg1);
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
        WriteArg^.Arg := OutExpr;
        WriteArg^.Width := nil;
        WriteArg^.Prec := nil;
        WriteArg^.Next := nil;
        if Lexer.Token.Id = TkColon then
        begin
          WantTokenAndRead(TkColon);
          WriteArg^.Width := PsExpression;
          EnsureIntegerExpr(WriteArg^.Width);
          if IsRealType(WriteArg^.Arg^.TypePtr)
             and (Lexer.Token.Id = TkColon) then
          begin
            WantTokenAndRead(TkColon);
            WriteArg^.Prec := PsExpression;
            EnsureIntegerExpr(WriteArg^.Prec);
          end
        end
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
    else InternalError('Unimplemented special function ' + ExDescribe(Fn))
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

function Pf_DescribeCall(Expr : TExpression) : string;
begin
  with Expr^.PseudoFnCall do
    case PseudoFn of 
      TpfDispose: Result := 'DISPOSE(' + ExDescribe(Arg1) + ')';
      TpfNew: Result := 'NEW(' + ExDescribe(Arg1) + ')';
      TpfOrd: Result := 'ORD(' + ExDescribe(Arg1) + ')';
      TpfPred: Result := 'PRED(' + ExDescribe(Arg1) + ')';
      TpfRead: Result := 'READ(...)';
      TpfReadln: Result := 'READLN(...)';
      TpfStr:
              if Arg3 = nil then
                Result := 'STR(' + ExDescribe(Arg1) + ', ' +
                          ExDescribe(Arg2) + ')'
              else if Arg4 = nil then
                     Result := 'STR(' + ExDescribe(Arg1) + ':' +
                               ExDescribe(Arg3) + ', ' +
                               ExDescribe(Arg2) + ')'
              else Result := 'STR(' + ExDescribe(Arg1) + ':' +
                             ExDescribe(Arg3) + ':' +
                             ExDescribe(Arg4) + ', ' +
                             ExDescribe(Arg2) + ')';
      TpfSucc: Result := 'SUCC(' + ExDescribe(Arg1) + ')';
      TpfVal: Result := 'VAL(' + ExDescribe(Arg1) + ', ' +
                        ExDescribe(Arg2) + ', ' +
                        ExDescribe(Arg3) + ')';
      TpfWrite: Result := 'WRITE(...)';
      TpfWriteln: Result := 'WRITELN(...)';
      else InternalError('Cannot describe pseudofun')
    end
end;

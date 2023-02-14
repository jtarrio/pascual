function _NewExpr(Cls : TExpressionClass) : TExpression;
var Expr : TExpression;
begin
  new(Expr);
  Expr^.Cls := Cls;
  Expr^.IsAssignable := false;
  Expr^.IsFunctionResult := false;
  _NewExpr := Expr
end;

procedure _DisposePseudoCallExpr(var Call : TExPseudoFnCall);
var 
  ReadArg, NextReadArg : ^TExReadArgs;
  WriteArg, NextWriteArg : ^TExWriteArgs;
begin
  if Call.Arg1 <> nil then DisposeExpr(Call.Arg1);
  if Call.Arg2 <> nil then DisposeExpr(Call.Arg2);
  if Call.Arg3 <> nil then DisposeExpr(Call.Arg3);
  if (Call.PseudoFn = TpfWrite) or (Call.PseudoFn = TpfWriteln) then
  begin
    ReadArg := Call.ReadArgs;
    while ReadArg <> nil do
    begin
      NextReadArg := ReadArg^.Next;
      DisposeExpr(ReadArg^.Arg);
      dispose(ReadArg);
      ReadArg := NextReadArg
    end
  end
  else if (Call.PseudoFn = TpfRead) or (Call.PseudoFn = TpfReadln) then
  begin
    WriteArg := Call.WriteArgs;
    while WriteArg <> nil do
    begin
      NextWriteArg := WriteArg^.Next;
      DisposeExpr(WriteArg^.Arg);
      dispose(WriteArg);
      WriteArg := NextWriteArg
    end
  end
end;

procedure DisposeExpr;
var Pos : integer;
begin
  case Expr^.Cls of 
    XcToString : DisposeExpr(Expr^.ToStrParent);
    XcToReal : DisposeExpr(Expr^.ToRealParent);
    XcSetTmpVar :
                  begin
                    DisposeExpr(Expr^.TmpVar);
                    DisposeExpr(Expr^.TmpVarValue);
                    DisposeExpr(Expr^.TmpVarChild);
                  end;
    XcSubrange : DisposeExpr(Expr^.SubrangeParent);
    XcField : DisposeExpr(Expr^.RecExpr);
    XcArray :
              begin
                DisposeExpr(Expr^.ArrayExpr);
                DisposeExpr(Expr^.ArrayIndex);
              end;
    XcPointer : DisposeExpr(Expr^.PointerExpr);
    XcStringChar :
                   begin
                     DisposeExpr(Expr^.StringExpr);
                     DisposeExpr(Expr^.StringIndex);
                   end;
    XcFnCall :
               begin
                 DisposeExpr(Expr^.FnExpr);
                 for Pos := 1 to Expr^.CallArgs.Size do
                   DisposeExpr(Expr^.CallArgs.Values[Pos]);
               end;
    XcPseudoFnCall : _DisposePseudoCallExpr(Expr^.PseudoFnCall);
    XcUnaryOp : DisposeExpr(Expr^.Unary.Parent);
    XcBinaryOp :
                 begin
                   DisposeExpr(Expr^.Binary.Left);
                   DisposeExpr(Expr^.Binary.Right);
                 end;
  end;
  dispose(Expr);
end;

procedure _CopyPseudoCallExpr(var Call, Copy : TExPseudoFnCall);
var 
  ReadArg, NextReadArg, CopyReadArg : ^TExReadArgs;
  WriteArg, NextWriteArg, CopyWriteArg : ^TExWriteArgs;
begin
  Copy.PseudoFn := Call.PseudoFn;
  if Call.Arg1 <> nil then Copy.Arg1 := CopyExpr(Call.Arg1);
  if Call.Arg2 <> nil then Copy.Arg2 := CopyExpr(Call.Arg2);
  if Call.Arg3 <> nil then Copy.Arg3 := CopyExpr(Call.Arg3);
  if (Call.PseudoFn = TpfWrite) or (Call.PseudoFn = TpfWriteln) then
  begin
    ReadArg := Call.ReadArgs;
    CopyReadArg := nil;
    while ReadArg <> nil do
    begin
      NextReadArg := ReadArg^.Next;
      if CopyReadArg = nil then
      begin
        new(CopyReadArg);
        Copy.ReadArgs := CopyReadArg
      end
      else
      begin
        new(CopyReadArg^.Next);
        CopyReadArg := CopyReadArg^.Next;
      end;
      CopyReadArg^.Next := nil;
      CopyReadArg^.Arg := CopyExpr(ReadArg^.Arg);
      ReadArg := NextReadArg
    end
  end
  else if (Call.PseudoFn = TpfRead) or (Call.PseudoFn = TpfReadln) then
  begin
    WriteArg := Call.WriteArgs;
    CopyWriteArg := nil;
    while WriteArg <> nil do
    begin
      NextWriteArg := WriteArg^.Next;
      if CopyWriteArg = nil then
      begin
        new(CopyWriteArg);
        Copy.WriteArgs := CopyWriteArg
      end
      else
      begin
        new(CopyWriteArg^.Next);
        CopyWriteArg := CopyWriteArg^.Next;
      end;
      CopyWriteArg^.Next := nil;
      CopyWriteArg^.Arg := CopyExpr(WriteArg^.Arg);
      WriteArg := NextWriteArg
    end
  end
end;

function CopyExpr;
var 
  Copy : TExpression;
  Pos : integer;
begin
  Copy := _NewExpr(Expr^.Cls);
  Copy^.TypePtr := Expr^.TypePtr;
  Copy^.IsAssignable := Expr^.IsAssignable;
  Copy^.IsFunctionResult := Expr^.IsFunctionResult;
  case Expr^.Cls of 
    XcImmediate: Copy^.Immediate := Expr^.Immediate;
    XcToString: Copy^.ToStrParent := CopyExpr(Expr^.ToStrParent);
    XcToReal: Copy^.ToRealParent := CopyExpr(Expr^.ToRealParent);
    XcSetTmpVar :
                  begin
                    Copy^.TmpVar := CopyExpr(Expr^.TmpVar);
                    Copy^.TmpVarValue := CopyExpr(Expr^.TmpVarValue);
                    Copy^.TmpVarChild := CopyExpr(Expr^.TmpVarChild);
                  end;
    XcSubrange : Copy^.SubrangeParent := CopyExpr(Expr^.SubrangeParent);
    XcVariable: Copy^.VarPtr := Expr^.VarPtr;
    XcField:
             begin
               Copy^.RecExpr := CopyExpr(Expr^.RecExpr);
               Copy^.RecFieldNum := Expr^.RecFieldNum
             end;
    XcArray:
             begin
               Copy^.ArrayExpr := CopyExpr(Expr^.ArrayExpr);
               Copy^.ArrayIndex := CopyExpr(Expr^.ArrayIndex)
             end;
    XcPointer: Copy^.PointerExpr := CopyExpr(Expr^.PointerExpr);
    XcStringChar:
                  begin
                    Copy^.StringExpr := CopyExpr(Expr^.StringExpr);
                    Copy^.StringIndex := CopyExpr(Expr^.StringIndex)
                  end;
    XcFnRef: Copy^.FnPtr := Expr^.FnPtr;
    XcFnCall:
              begin
                Copy^.FnExpr := CopyExpr(Expr^.FnExpr);
                Copy^.CallArgs.Size := Expr^.CallArgs.Size;
                for Pos := 1 to Expr^.CallArgs.Size do
                  Copy^.CallArgs.Values[Pos] := CopyExpr(Expr^.CallArgs
                                                .Values[Pos])
              end;
    XcPseudoFnRef: Copy^.PseudoFn := Expr^.PseudoFn;
    XcPseudoFnCall: _CopyPseudoCallExpr(Expr^.PseudoFnCall,
                                        Copy^.PseudoFnCall);
    XcUnaryOp:
               begin
                 Copy^.Unary.Parent := CopyExpr(Expr^.Unary.Parent);
                 Copy^.Unary.Op := Expr^.Unary.Op
               end;
    XcBinaryOp:
                begin
                  Copy^.Binary.Left := CopyExpr(Expr^.Binary.Left);
                  Copy^.Binary.Right := CopyExpr(Expr^.Binary.Right);
                  Copy^.Binary.Op := Expr^.Binary.Op
                end;
    else CompileError('Internal error: cannot copy expression: ' +
                      DescribeExpr(Expr, 2))
  end;
  CopyExpr := Copy
end;

function _UnparseChar(Chr : char) : string;
var 
  ChNum : string;
begin
  if Chr = '''' then Result := ''''''''''
  else if Chr < ' ' then
  begin
    Str(Ord(Chr), ChNum);
    Result := '#' + ChNum
  end
  else Result := '''' + Chr + ''''
end;

function _UnparseString(St : string) : string;
var 
  Pos : integer;
  ChNum : string;
  Quoted : boolean;
begin
  Quoted := false;
  Result := '';
  for Pos := 1 to Length(St) do
  begin
    if St[Pos] < ' ' then
    begin
      if Quoted then
      begin
        Quoted := false;
        Result := Result + ''''
      end;
      Str(Ord(St[Pos]), ChNum);
      Result := Result + '#' + ChNum
    end
    else
    begin
      if not Quoted then
      begin
        Quoted := true;
        Result := Result + ''''
      end;
      if St[Pos] = '''' then Result := Result + ''''''
      else Result := Result + St[Pos]
    end
  end;
  if Quoted then Result := Result + '''';
  if Result = '' then Result := ''''''
end;

function _DescribeImmediatepr(Expr : TExpression) : string;
begin
  with Expr^.Immediate do
    case Cls of 
      XicNil: Result := 'nil';
      XicBoolean: Str(BooleanVal, Result);
      XicInteger: Str(IntegerVal, Result);
      XicReal: Str(RealVal, Result);
      XicChar: Result := _UnparseChar(CharVal);
      XicString: Result := _UnparseString(StringVal);
      XicEnum: Result := EnumPtr^.Values[EnumOrdinal];
      else CompileError('Internal error: cannot describe immediate value')
    end
end;

function _DescribePseudoFnExpr(Expr : TExpression) : string;
begin
  case Expr^.PseudoFn of 
    TpfConcat: Result := 'CONCAT';
    TpfDispose: Result := 'DISPOSE';
    TpfNew: Result := 'NEW';
    TpfPred: Result := 'PRED';
    TpfRead: Result := 'READ';
    TpfReadln: Result := 'READLN';
    TpfStr: Result := 'STR';
    TpfSucc: Result := 'SUCC';
    TpfVal: Result := 'VAL';
    TpfWrite: Result := 'WRITE';
    TpfWriteln: Result := 'WRITELN';
    else CompileError('Internal error: cannot describe pseudofun')
  end
end;

function _DescribePseudoCallExpr(Expr : TExpression; Levels : integer) : string;
begin
  with Expr^.PseudoFnCall do
    case PseudoFn of 
      TpfDispose: Result := 'DISPOSE(' + DescribeExpr(Arg1, Levels - 1) + ')';
      TpfNew: Result := 'NEW(' + DescribeExpr(Arg1, Levels - 1) + ')';
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
      else CompileError('Internal error: cannot describe pseudofun')
    end
end;

function _ExprPrecedence(Expr : TExpression) : integer;
begin
  case Expr^.Cls of 
    XcImmediate: Result := 0;
    XcToString: Result := _ExprPrecedence(Expr^.ToStrParent);
    XcToReal: Result := _ExprPrecedence(Expr^.ToRealParent);
    XcSetTmpVar: Result := _ExprPrecedence(Expr^.TmpVarChild);
    XcSubrange: Result := _ExprPrecedence(Expr^.SubrangeParent);
    XcVariable: Result := 0;
    XcField: Result := 1;
    XcArray: Result := 1;
    XcPointer: Result := 1;
    XcStringChar: Result := 1;
    XcFnRef: Result := 0;
    XcFnCall: Result := 1;
    XcPseudoFnRef: Result := 0;
    XcPseudoFnCall: Result := 1;
    XcUnaryOp: case Expr^.Unary.Op of 
                 TkMinus: Result := 4;
                 TkNot: Result := 2;
               end;
    XcBinaryOp: case Expr^.Binary.Op of 
                  TkPlus: Result := 4;
                  TkMinus: Result := 4;
                  TkAsterisk: Result := 3;
                  TkDiv: Result := 3;
                  TkAnd: Result := 3;
                  TkOr: Result := 4;
                  TkEquals: Result := 5;
                  TkNotEquals: Result := 5;
                  TkLessthan: Result := 5;
                  TkMorethan: Result := 5;
                  TkLessOrEquals: Result := 5;
                  TkMoreOrEquals: Result := 5;
                end;
  end;
end;

function _DescribeUnaryOpExpr(Expr : TExpression; Levels : integer) : string;
var UseParens : boolean;
begin
  case Expr^.Unary.Op of 
    TkMinus: Result := '-';
    TkNot: Result := 'not ';
    else CompileError('Internal error: cannot describe unary operation')
  end;
  UseParens := _ExprPrecedence(Expr) < _ExprPrecedence(Expr^.Unary.Parent);
  if UseParens then Result := Result + '(';
  Result := Result + DescribeExpr(Expr^.Unary.Parent, Levels);
  if UseParens then Result := Result + ')';
end;

function _DescribeBinaryOpExpr(Expr : TExpression; Levels : integer) : string;
var UseParens : boolean;
begin
  UseParens := _ExprPrecedence(Expr) < _ExprPrecedence(Expr^.Binary.Left);
  if UseParens then Result := '('
  else Result := '';
  Result := Result + DescribeExpr(Expr^.Binary.Left, Levels - 1);
  if UseParens then Result := Result + ')';
  case Expr^.Binary.Op of 
    TkPlus: Result := Result + ' + ';
    TkMinus: Result := Result + ' - ';
    TkAsterisk: Result := Result + ' * ';
    TkSlash: Result := Result + ' / ';
    TkDiv: Result := Result + ' div ';
    TkAnd: Result := Result + ' and ';
    TkOr: Result := Result + ' or ';
    TkEquals: Result := Result + ' = ';
    TkNotEquals: Result := Result + ' <> ';
    TkLessthan: Result := Result + ' < ';
    TkMorethan: Result := Result + ' > ';
    TkLessOrEquals: Result := Result + ' <= ';
    TkMoreOrEquals: Result := Result + ' >= ';
    else CompileError('Internal error: cannot describe binary operation')
  end;
  UseParens := _ExprPrecedence(Expr) < _ExprPrecedence(Expr^.Binary.Right);
  if UseParens then Result := Result + '(';
  Result := Result + DescribeExpr(Expr^.Binary.Right, Levels - 1);
  if UseParens then Result := Result + ')';
end;

function _DescribeSetTmpVar(Expr : TExpression; Levels : integer) : string;
begin
  Result := '{with ';
  while Expr^.Cls = XcSetTmpVar do
  begin
    Result := Result + DescribeExpr(Expr^.TmpVar, Levels) + ':=' +
              DescribeExpr(Expr^.TmpVarValue, Levels - 1);
    Expr := Expr^.TmpVarChild;
    if Expr^.Cls = XcSetTmpVar then Result := Result + ', '
  end;
  Result := Result + '} ' + DescribeExpr(Expr, Levels - 1)
end;

function DescribeExpr;
var 
  Pos : integer;
begin
  if Levels < 1 then Result := '(...)'
  else
    case Expr^.Cls of 
      XcImmediate: Result := _DescribeImmediatepr(Expr);
      XcToString: Result := DescribeExpr(Expr^.ToStrParent, Levels);
      XcToReal: Result := DescribeExpr(Expr^.ToRealParent, Levels);
      XcSetTmpVar: Result := _DescribeSetTmpVar(Expr, Levels);
      XcSubrange: Result := DescribeExpr(Expr^.ToStrParent, Levels);
      XcVariable: Result := Expr^.VarPtr^.Name;
      XcField: Result := DescribeExpr(Expr^.RecExpr, Levels) + '.' +
                         Expr^.RecExpr^.TypePtr^.RecPtr^
                         .Fields[Expr^.RecFieldNum].Name;
      XcArray: Result := DescribeExpr(Expr^.ArrayExpr, Levels) +
                         '[' + DescribeExpr(Expr^.ArrayIndex, Levels - 1) + ']';
      XcPointer: Result := DescribeExpr(Expr^.PointerExpr, Levels) + '^';
      XcStringChar: Result := DescribeExpr(Expr^.StringExpr, Levels) + '[' +
                              DescribeExpr(Expr^.StringIndex, Levels - 1) + ']';
      XcFnRef: Result := Expr^.FnPtr^.Name;
      XcFnCall:
                begin
                  Result := DescribeExpr(Expr^.FnExpr, Levels) + '(';
                  if Levels < 2 then
                    Result := Result + '...'
                  else
                    for Pos := 1 to Expr^.CallArgs.Size do
                  begin
                    if Pos <> 1 then Result := Result + ', ';
                    Result := Result +
                              DescribeExpr(Expr^.CallArgs.Values[Pos],
                              Levels - 1)
                  end;
                  Result := Result + ')'
                end;
      XcPseudoFnRef: Result := _DescribePseudoFnExpr(Expr);
      XcPseudoFnCall: Result := _DescribePseudoCallExpr(Expr, Levels);
      XcUnaryOp: Result := _DescribeUnaryOpExpr(Expr, Levels);
      XcBinaryOp: Result := _DescribeBinaryOpExpr(Expr, Levels);
      else CompileError('Internal error: cannot describe expression')
    end
end;

function ExCoerce(Expr : TExpression; TypePtr : TPsTypePtr) : TExpression;
forward;

function _ExImmediate(Cls : TExImmediateClass) : TExpression;
var Expr : TExpression;
begin
  Expr := _NewExpr(XcImmediate);
  Expr^.Immediate.Cls := Cls;
  _ExImmediate := Expr
end;

function ExNil : TExpression;
begin
  Result := _ExImmediate(XicNil);
  Result^.TypePtr := PrimitiveTypes.PtNil
end;

function ExBooleanConstant(Value : boolean) : TExpression;
begin
  Result := _ExImmediate(XicBoolean);
  Result^.Immediate.BooleanVal := Value;
  Result^.TypePtr := PrimitiveTypes.PtBoolean
end;

function ExIntegerConstant(Value : integer) : TExpression;
begin
  Result := _ExImmediate(XicInteger);
  Result^.Immediate.IntegerVal := Value;
  Result^.TypePtr := PrimitiveTypes.PtInteger
end;

function ExRealConstant(Value : real) : TExpression;
begin
  Result := _ExImmediate(XicReal);
  Result^.Immediate.RealVal := Value;
  Result^.TypePtr := PrimitiveTypes.PtReal
end;

function ExCharConstant(Value : char) : TExpression;
begin
  Result := _ExImmediate(XicChar);
  Result^.Immediate.CharVal := Value;
  Result^.TypePtr := PrimitiveTypes.PtChar
end;

function ExStringConstant(Value : string) : TExpression;
begin
  Result := _ExImmediate(XicString);
  Result^.Immediate.StringVal := Value;
  Result^.TypePtr := PrimitiveTypes.PtString
end;

function ExEnumConstant(Ordinal : integer; TypePtr : TPsTypePtr) : TExpression;
begin
  if not IsEnumType(TypePtr) then
    CompileError('Not an enumeration type: ' + TypeName(TypePtr));
  if (Ordinal < 0) or (Ordinal > TypePtr^.EnumPtr^.Size - 1) then
    CompileError('Invalid value for ' + TypeName(TypePtr));
  Result := _ExImmediate(XicEnum);
  Result^.Immediate.EnumOrdinal := Ordinal;
  Result^.Immediate.EnumPtr := TypePtr^.EnumPtr;
  Result^.TypePtr := TypePtr
end;

function ExToString(Parent : TExpression) : TExpression;
var Str : string;
begin
  if IsCharType(Parent^.TypePtr) then
  begin
    if Parent^.Cls = XcImmediate then
    begin
      Str := Parent^.Immediate.CharVal;
      Parent^.Immediate.Cls := XicString;
      Parent^.Immediate.StringVal := Str;
      Result := Parent
    end
    else
    begin
      Result := _NewExpr(XcToString);
      Result^.ToStrParent := Parent;
      Result^.TypePtr := PrimitiveTypes.PtString;
      Result^.IsAssignable := false;
      Result^.IsFunctionResult := Parent^.IsFunctionResult
    end
  end
  else if IsStringType(Parent^.TypePtr) then Result := Parent
  else
    CompileError('Cannot convert value of type ' + TypeName(Parent^.TypePtr) +
    ' to string: ' + DescribeExpr(Parent, 10))
end;

function ExToReal(Parent : TExpression) : TExpression;
var Value : real;
begin
  if Parent^.Cls = XcImmediate then
  begin
    Value := Parent^.Immediate.IntegerVal;
    Parent^.Immediate.Cls := XicReal;
    Parent^.Immediate.RealVal := Value;
    Parent^.TypePtr := PrimitiveTypes.PtReal;
    Result := Parent
  end
  else
  begin
    Result := _NewExpr(XcToReal);
    Result^.ToRealParent := Parent;
    Result^.TypePtr := PrimitiveTypes.PtReal;
    Result^.IsAssignable := false;
    Result^.IsFunctionResult := Parent^.IsFunctionResult
  end;
end;

function ExSetTmpVar(TmpVar, Value, Child : TExpression) : TExpression;
begin
  Result := _NewExpr(XcSetTmpVar);
  Result^.TmpVar := TmpVar;
  Result^.TmpVarValue := Value;
  Result^.TmpVarChild := Child;
  Result^.TypePtr := Child^.TypePtr;
  Result^.IsAssignable := true;
  Result^.IsFunctionResult := false;
  TmpVar^.VarPtr^.WasInitialized := true;
  TmpVar^.VarPtr^.WasUsed := true
end;

function ExSubrange(Parent : TExpression; TypePtr : TPsTypePtr)
: TExpression;
forward;

function ExOutrange(Expr : TExpression) : TExpression;
var TmpExpr : TExpression;
begin
  while Expr^.Cls = XcSubrange do
  begin
    TmpExpr := CopyExpr(Expr^.SubrangeParent);
    DisposeExpr(Expr);
    Expr := TmpExpr;
  end;
  while IsRangeType(Expr^.TypePtr) do
    Expr^.TypePtr := Expr^.TypePtr^.RangePtr^.BaseTypePtr;
  Result := Expr
end;

function ExVariable(VarPtr : TPsVarPtr) : TExpression;
begin
  Result := _NewExpr(XcVariable);
  Result^.VarPtr := VarPtr;
  Result^.TypePtr := VarPtr^.TypePtr;
  Result^.IsAssignable := not VarPtr^.IsConstant
end;

function ExFieldAccess(Parent : TExpression; FieldNum : integer)
: TExpression;
begin
  if not IsRecordType(Parent^.TypePtr) then
    CompileError('Cannot access field of non-record value of type ' +
                 TypeName(Parent^.TypePtr));
  if (FieldNum < 1)
     or (FieldNum > Parent^.TypePtr^.RecPtr^.Size) then
    CompileError('Invalid field for type ' + TypeName(Parent^.TypePtr));
  Result := _NewExpr(XcField);
  Result^.RecExpr := Parent;
  Result^.RecFieldNum := FieldNum;
  Result^.TypePtr := Parent^.TypePtr^.RecPtr^
                     .Fields[FieldNum].TypePtr;
  Result^.IsAssignable := Parent^.IsAssignable;
  Result^.IsFunctionResult := Parent^.IsFunctionResult
end;

function ExArrayAccess(Parent, Subscript : TExpression) : TExpression;
begin
  if not IsArrayType(Parent^.TypePtr) then
    CompileError('Cannot access subscript of non-array value of type ' +
                 TypeName(Parent^.TypePtr) + ': ' + DescribeExpr(Parent, 10));
  Result := _NewExpr(XcArray);
  Result^.ArrayExpr := Parent;
  Result^.ArrayIndex := ExCoerce(Subscript,
                        Parent^.TypePtr^.ArrayPtr^.IndexTypePtr);
  Result^.TypePtr := Parent^.TypePtr^.ArrayPtr^.ValueTypePtr;
  Result^.IsAssignable := Parent^.IsAssignable;
  Result^.IsFunctionResult := Parent^.IsFunctionResult
end;

function ExPointerAccess(Parent : TExpression) : TExpression;
begin
  if not IsPointerType(Parent^.TypePtr) then
    CompileError('Cannot dereference non-pointer value of type ' +
                 TypeName(Parent^.TypePtr) + ': ' + DescribeExpr(Parent, 10));
  Result := _NewExpr(XcPointer);
  Result^.PointerExpr := Parent;
  Result^.TypePtr := Parent^.TypePtr^.PointedTypePtr;
  Result^.IsAssignable := true;
  Result^.IsFunctionResult := Parent^.IsFunctionResult
end;

function ExStringChar(Parent, Subscript : TExpression) : TExpression;
begin
  if not IsStringyType(Parent^.TypePtr) then
    CompileError('Cannot access subscript of non-string value of type ' +
                 TypeName(Parent^.TypePtr) + ': ' + DescribeExpr(Parent, 10));
  if not IsIntegerType(Subscript^.TypePtr) then
    CompileError('Invalid type for subscript of string: ' +
                 TypeName(Subscript^.TypePtr) +
    ': ' + DescribeExpr(Subscript, 10));
  Result := _NewExpr(XcStringChar);
  Result^.ArrayExpr := ExToString(Parent);
  Result^.ArrayIndex := Subscript;
  Result^.TypePtr := PrimitiveTypes.PtChar;
  Result^.IsAssignable := Parent^.IsAssignable;
  Result^.IsFunctionResult := Parent^.IsFunctionResult
end;

function ExFnRef(FnPtr : TPsFnPtr) : TExpression;
begin
  Result := _NewExpr(XcFnRef);
  Result^.FnPtr := FnPtr;
  Result^.TypePtr := nil
end;

function ExFunctionCall(FnExpr : TExpression; var Args : TExFunctionArgs)
: TExpression;
var 
  Pos : integer;
  FnCall : TExpression;
  TmpVarNum : string;
begin
  if FnExpr^.Cls <> XcFnRef then
    CompileError('Cannot call non-function');
  if Args.Size <> FnExpr^.FnPtr^.ArgCount then
    CompileError('Wrong number of arguments in call to ' + FnExpr^.FnPtr^.Name);
  FnExpr^.FnPtr^.WasUsed := true;
  FnCall := _NewExpr(XcFnCall);
  FnCall^.FnExpr := FnExpr;
  FnCall^.CallArgs.Size := Args.Size;
  FnCall^.TypePtr := FnExpr^.FnPtr^.ReturnTypePtr;
  FnCall^.IsAssignable := false;
  FnCall^.IsFunctionResult := true;
  Result := FnCall;
  for Pos := 1 to Args.Size do
  begin
    FnCall^.CallArgs.Values[Pos] := ExCoerce(Args.Values[Pos],
                                    FnExpr^.FnPtr^.Args[Pos].TypePtr);
    if FnExpr^.FnPtr^.Args[Pos].IsReference then
    begin
      if not FnCall^.CallArgs.Values[Pos]^.IsAssignable then
      begin
        if FnExpr^.FnPtr^.Args[Pos].IsConstant then
        begin
          Str(DefCounter(TctTmpVar), TmpVarNum);
          Result := ExSetTmpVar(ExVariable(AddVariable(MakeVariable(
                    'tmp' + TmpVarNum,
                    FnExpr^.FnPtr^.Args[Pos].TypePtr))),
                    FnCall^.CallArgs.Values[Pos], Result);
          FnCall^.CallArgs.Values[Pos] := CopyExpr(Result^.TmpVar)
        end
        else
          CompileError('Pass-by-reference argument must be assignable: ' +
                       DescribeExpr(FnCall^.CallArgs.Values[Pos], 10))
      end
      else if not FnExpr^.FnPtr^.Args[Pos].IsConstant then
             ExMarkInitialized(FnCall^.CallArgs.Values[Pos])
    end
  end
end;

function ExPseudoFn(SpecialFn : TPsPseudoFn) : TExpression;
begin
  Result := _NewExpr(XcPseudoFnRef);
  Result^.PseudoFn := SpecialFn
end;

function ExPseudoFnCall(Expr : TExpression) : TExpression;
var Fn : TPsPseudoFn;
begin
  if Expr^.Cls <> XcPseudoFnRef then
    CompileError('Internal error: Expected a pseudofunction, got ' +
                 DescribeExpr(Expr, 10));
  Fn := Expr^.PseudoFn;
  Expr^.Cls := XcPseudoFnCall;
  Expr^.PseudoFnCall.PseudoFn := Fn;
  Expr^.PseudoFnCall.Arg1 := nil;
  Expr^.PseudoFnCall.Arg2 := nil;
  Expr^.PseudoFnCall.Arg3 := nil;
  Expr^.PseudoFnCall.ReadArgs := nil;
  Expr^.PseudoFnCall.WriteArgs := nil;
  Result := Expr
end;

function _ExUnOpImm(Parent : TExpression; Op : TLxTokenId) : TExpression;
forward;
function _ExUnOpCmp(Parent : TExpression; Op : TLxTokenId) : TExpression;
forward;
function ExUnaryOp(Parent : TExpression; Op : TLxTokenId) : TExpression;
begin
  if (Op = TkMinus) or (Op = TkPlus) then
  begin
    if not IsNumericType(Parent^.TypePtr) then
      CompileError('Invalid type for ' + LxTokenName(Op) + ': ' +
      TypeName(Parent^.TypePtr))
  end
  else if Op = TkNot then
  begin
    if not IsBooleanType(Parent^.TypePtr)
       and not IsIntegerType(Parent^.TypePtr) then
      CompileError('Invalid type for ' + LxTokenName(Op) + ': ' +
      TypeName(Parent^.TypePtr))
  end
  else CompileError('Invalid unary operator: ' + LxTokenName(Op));

  if Parent^.Cls = XcImmediate then ExUnaryOp := _ExUnOpImm(Parent, Op)
  else ExUnaryOp := _ExUnOpCmp(Parent, Op)
end;

function _ExUnOpImm;
begin
  if (Op = TkMinus) and (Parent^.Immediate.Cls = XicInteger) then
    Parent^.Immediate.IntegerVal := -Parent^.Immediate.IntegerVal
  else if (Op = TkPlus) and (Parent^.Immediate.Cls = XicInteger) then
    { do nothing }
  else if (Op = TkMinus) and (Parent^.Immediate.Cls = XicReal) then
         Parent^.Immediate.RealVal := -Parent^.Immediate.RealVal
  else if (Op = TkPlus) and (Parent^.Immediate.Cls = XicReal) then
    { do nothing }
  else if (Op = TkNot) and (Parent^.Immediate.Cls = XicBoolean) then
         Parent^.Immediate.BooleanVal := not Parent^.Immediate.BooleanVal
  else if (Op = TkNot) and (Parent^.Immediate.Cls = XicInteger) then
         Parent^.Immediate.IntegerVal := not Parent^.Immediate.IntegerVal
  else CompileError('Internal error: invalid immediate unary operation');
  _ExUnOpImm := Parent
end;

function _ExUnOpCmp;
begin
  Result := _NewExpr(XcUnaryOp);
  Result^.Unary.Parent := Parent;
  Result^.Unary.Op := Op;
  Result^.TypePtr := Parent^.TypePtr;
  Result^.IsAssignable := false;
  Result^.IsFunctionResult := Parent^.IsFunctionResult
end;

function _ExBinOpBoolImm(Left, Right : TExpression;
                         Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpIntImm(Left, Right : TExpression;
                        Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpNumImm(Left, Right : TExpression;
                        Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpStrImm(Left, Right : TExpression;
                        Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpEnumImm(Left, Right : TExpression;
                         Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpBoolCmp(Left, Right : TExpression;
                         Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpIntCmp(Left, Right : TExpression;
                        Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpNumCmp(Left, Right : TExpression;
                        Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpStrCmp(Left, Right : TExpression;
                        Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpEnumCmp(Left, Right : TExpression;
                         Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpPtrCmp(Left, Right : TExpression;
                        Op : TLxTokenId) : TExpression;
forward;
function ExBinaryOp(Left, Right : TExpression;
                    Op : TLxTokenId) : TExpression;
var 
  Immediate : boolean;
begin
  Left := ExOutrange(Left);
  Right := ExOutrange(Right);

  Immediate := (Left^.Cls = XcImmediate) and (Right^.Cls = XcImmediate);
  if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
  begin
    if Immediate then Result := _ExBinOpBoolImm(Left, Right, Op)
    else Result := _ExBinOpBoolCmp(Left, Right, Op)
  end
  else if IsIntegerType(Left^.TypePtr) and IsIntegerType(Right^.TypePtr) then
  begin
    if Immediate then Result := _ExBinOpIntImm(Left, Right, Op)
    else Result := _ExBinOpIntCmp(Left, Right, Op)
  end
  else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
  begin
    if Immediate then Result := _ExBinOpNumImm(Left, Right, Op)
    else Result := _ExBinOpNumCmp(Left, Right, Op)
  end
  else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
  begin
    if Immediate then Result := _ExBinOpStrImm(Left, Right, Op)
    else Result := _ExBinOpStrCmp(Left, Right, Op)
  end
  else if IsEnumType(Left^.TypePtr)
          and IsSameType(Left^.TypePtr, Right^.TypePtr) then
  begin
    if Immediate then Result := _ExBinOpEnumImm(Left, Right, Op)
    else Result := _ExBinOpEnumCmp(Left, Right, Op)
  end
  else if ArePointersCompatible(Left^.TypePtr, Right^.TypePtr) then
         Result := _ExBinOpPtrCmp(Left, Right, Op)
  else
    CompileError('Type mismatch for operator ' + LxTokenName(Op) + ': ' +
    TypeName(Left^.TypePtr) + ' and ' + TypeName(Right^.TypePtr))
end;

function _ExBinOpBoolImm;
var Lt, Rt : boolean;
begin
  Lt := Left^.Immediate.BooleanVal;
  Rt := Right^.Immediate.BooleanVal;
  DisposeExpr(Right);
  case Op of 
    TkAnd : Lt := Lt and Rt;
    TkOr : Lt := Lt or Rt;
    TkEquals : Lt := Lt = Rt;
    TkNotEquals : Lt := Lt <> Rt;
    TkLessthan : Lt := Lt < Rt;
    TkMorethan : Lt := Lt > Rt;
    TkLessOrEquals : Lt := Lt <= Rt;
    TkMoreOrEquals : Lt := Lt >= Rt;
    else CompileError('Invalid boolean operator: ' + LxTokenName(Op))
  end;
  Left^.Immediate.BooleanVal := Lt;
  Left^.TypePtr := PrimitiveTypes.PtBoolean;
  Left^.IsAssignable := false;
  Result := Left
end;

function _ExBinOpIntImm;
var 
  Lt, Rt : integer;
  Bo : boolean;
begin
  Lt := Left^.Immediate.IntegerVal;
  Rt := Right^.Immediate.IntegerVal;
  DisposeExpr(Right);
  case Op of 
    TkPlus : Lt := Lt + Rt;
    TkMinus : Lt := Lt - Rt;
    TkAsterisk : Lt := Lt * Rt;
    TkDiv : Lt := Lt div Rt;
    TkMod : Lt := Lt mod Rt;
    TkAnd : Lt := Lt and Rt;
    TkOr : Lt := Lt or Rt;
    else
    begin
      Left^.Immediate.Cls := XicBoolean;
      case Op of 
        TkEquals : Bo := Lt = Rt;
        TkNotEquals : Bo := Lt <> Rt;
        TkLessthan : Bo := Lt < Rt;
        TkMorethan : Bo := Lt > Rt;
        TkLessOrEquals : Bo := Lt <= Rt;
        TkMoreOrEquals : Bo := Lt >= Rt;
        else CompileError('Invalid integer operator: ' + LxTokenName(Op))
      end
    end
  end;
  if Left^.Immediate.Cls = XicInteger then
  begin
    Left^.Immediate.IntegerVal := Lt;
    Left^.TypePtr := PrimitiveTypes.PtInteger
  end
  else
  begin
    Left^.Immediate.BooleanVal := Bo;
    Left^.TypePtr := PrimitiveTypes.PtBoolean
  end;
  Left^.IsAssignable := false;
  Result := Left
end;

function _ExBinOpNumImm;
var 
  Lt, Rt : real;
  Bo : boolean;
begin
  Left := ExCoerce(Left, PrimitiveTypes.PtReal);
  Lt := Left^.Immediate.RealVal;
  Right := ExCoerce(Right, PrimitiveTypes.PtReal);
  Rt := Right^.Immediate.RealVal;
  DisposeExpr(Right);
  case Op of 
    TkPlus : Lt := Lt + Rt;
    TkMinus : Lt := Lt - Rt;
    TkAsterisk : Lt := Lt * Rt;
    TkSlash : Lt := Lt / Rt;
    else
    begin
      Left^.Immediate.Cls := XicBoolean;
      case Op of 
        TkEquals : Bo := Lt = Rt;
        TkNotEquals : Bo := Lt <> Rt;
        TkLessthan : Bo := Lt < Rt;
        TkMorethan : Bo := Lt > Rt;
        TkLessOrEquals : Bo := Lt <= Rt;
        TkMoreOrEquals : Bo := Lt >= Rt;
        else CompileError('Invalid real operator: ' + LxTokenName(Op))
      end
    end
  end;
  if Left^.Immediate.Cls = XicReal then
  begin
    Left^.Immediate.RealVal := Lt;
    Left^.TypePtr := PrimitiveTypes.PtReal
  end
  else
  begin
    Left^.Immediate.BooleanVal := Bo;
    Left^.TypePtr := PrimitiveTypes.PtBoolean
  end;
  Left^.IsAssignable := false;
  Result := Left
end;

function _ExBinOpStrImm;
var 
  Lt, Rt : string;
  Bo : boolean;
begin
  if Left^.Immediate.Cls = XicChar then Lt := Left^.Immediate.CharVal
  else Lt := Left^.Immediate.StringVal;
  if Right^.Immediate.Cls = XicChar then Rt := Right^.Immediate.CharVal
  else Rt := Right^.Immediate.StringVal;
  DisposeExpr(Right);
  if Op = TkPlus then
  begin
    Left^.Immediate.Cls := XicString;
    Lt := Lt + Rt;
  end
  else
  begin
    Left^.Immediate.Cls := XicBoolean;
    case Op of 
      TkEquals : Bo := Lt = Rt;
      TkNotEquals : Bo := Lt <> Rt;
      TkLessthan : Bo := Lt < Rt;
      TkMorethan : Bo := Lt > Rt;
      TkLessOrEquals : Bo := Lt <= Rt;
      TkMoreOrEquals : Bo := Lt >= Rt;
      else CompileError('Invalid string operator: ' + LxTokenName(Op))
    end;
  end;
  if Left^.Immediate.Cls = XicString then
  begin
    Left^.Immediate.StringVal := Lt;
    Left^.TypePtr := PrimitiveTypes.PtString
  end
  else
  begin
    Left^.Immediate.BooleanVal := Bo;
    Left^.TypePtr := PrimitiveTypes.PtBoolean
  end;
  Left^.IsAssignable := false;
  Result := Left
end;

function _ExBinOpEnumImm;
var 
  Lt, Rt : integer;
  Bo : boolean;
begin
  Lt := Left^.Immediate.EnumOrdinal;
  Rt := Right^.Immediate.EnumOrdinal;
  DisposeExpr(Right);
  case Op of 
    TkEquals : Bo := Lt = Rt;
    TkNotEquals : Bo := Lt <> Rt;
    TkLessthan : Bo := Lt < Rt;
    TkMorethan : Bo := Lt > Rt;
    TkLessOrEquals : Bo := Lt <= Rt;
    TkMoreOrEquals : Bo := Lt >= Rt;
    else CompileError('Invalid integer operator: ' + LxTokenName(Op))
  end;
  Left^.Immediate.Cls := XicBoolean;
  Left^.Immediate.BooleanVal := Bo;
  Left^.TypePtr := PrimitiveTypes.PtBoolean;
  Left^.IsAssignable := false;
  Result := Left
end;

function _ExBinOpBoolCmp;
begin
  if (Op = TkAnd) or (Op = TkOr) or (Op = TkEquals) or (Op = TkNotEquals)
     or (Op = TkLessthan) or (Op = TkMorethan) or (Op = TkLessOrEquals)
     or (Op = TkMoreOrEquals) then
  begin
    Result := _NewExpr(XcBinaryOp);
    Result^.Binary.Left := Left;
    Result^.Binary.Right := Right;
    Result^.Binary.Op := Op;
    Result^.TypePtr := PrimitiveTypes.PtBoolean;
    Result^.IsAssignable := false;
    Result^.IsFunctionResult := Left^.IsFunctionResult
                                or Right^.IsFunctionResult
  end
  else CompileError('Invalid boolean operator: ' + LxTokenName(Op))
end;

function _ExBinOpIntCmp;
begin
  Result := _NewExpr(XcBinaryOp);
  Result^.Binary.Left := Left;
  Result^.Binary.Right := Right;
  Result^.Binary.Op := Op;
  Result^.IsAssignable := false;
  Result^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
  if (Op = TkPlus) or (Op = TkMinus) or (Op = TkAsterisk) or (Op = TkDiv)
     or (Op = TkMod) or (Op = TkAnd) or (Op = TkOr) then
    Result^.TypePtr := PrimitiveTypes.PtInteger
  else if (Op = TkEquals) or (Op = TkNotEquals)
          or (Op = TkLessthan) or (Op = TkMorethan) or (Op = TkLessOrEquals)
          or (Op = TkMoreOrEquals) then
         Result^.TypePtr := PrimitiveTypes.PtBoolean
  else CompileError('Invalid integer operator: ' + LxTokenName(Op))
end;

function _ExBinOpNumCmp;
begin
  Result := _NewExpr(XcBinaryOp);
  Result^.Binary.Left := ExCoerce(Left, PrimitiveTypes.PtReal);
  Result^.Binary.Right := ExCoerce(Right, PrimitiveTypes.PtReal);
  Result^.Binary.Op := Op;
  Result^.IsAssignable := false;
  Result^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
  if (Op = TkPlus) or (Op = TkMinus) or (Op = TkAsterisk) or (Op = TkSlash) then
    Result^.TypePtr := PrimitiveTypes.PtReal
  else if (Op = TkEquals) or (Op = TkNotEquals)
          or (Op = TkLessthan) or (Op = TkMorethan) or (Op = TkLessOrEquals)
          or (Op = TkMoreOrEquals) then
         Result^.TypePtr := PrimitiveTypes.PtBoolean
  else CompileError('Invalid real operator: ' + LxTokenName(Op))
end;

function _ExBinOpStrCmp;
begin
  Result := _NewExpr(XcBinaryOp);
  Result^.Binary.Left := Left;
  Result^.Binary.Right := Right;
  Result^.Binary.Op := Op;
  Result^.IsAssignable := false;
  Result^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
  if Op = TkPlus then
    Result^.TypePtr := PrimitiveTypes.PtString
  else if (Op = TkEquals) or (Op = TkNotEquals)
          or (Op = TkLessthan) or (Op = TkMorethan) or (Op = TkLessOrEquals)
          or (Op = TkMoreOrEquals) then
         Result^.TypePtr := PrimitiveTypes.PtBoolean
  else CompileError('Invalid string operator: ' + LxTokenName(Op))
end;

function _ExBinOpEnumCmp;
begin
  Result := _NewExpr(XcBinaryOp);
  Result^.Binary.Left := Left;
  Result^.Binary.Right := Right;
  Result^.Binary.Op := Op;
  Result^.IsAssignable := false;
  Result^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
  if (Op = TkEquals) or (Op = TkNotEquals)
     or (Op = TkLessthan) or (Op = TkMorethan) or (Op = TkLessOrEquals)
     or (Op = TkMoreOrEquals) then
    Result^.TypePtr := PrimitiveTypes.PtBoolean
  else CompileError('Invalid enum operator: ' + LxTokenName(Op))
end;

function _ExBinOpPtrCmp;
begin
  Result := _NewExpr(XcBinaryOp);
  Result^.Binary.Left := Left;
  Result^.Binary.Right := Right;
  Result^.Binary.Op := Op;
  Result^.IsAssignable := false;
  Result^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
  if (Op = TkEquals) or (Op = TkNotEquals) then
    Result^.TypePtr := PrimitiveTypes.PtBoolean
  else CompileError('Invalid string operator: ' + LxTokenName(Op))
end;

function _ExBelongsToRange(Expr, First, Last : TExpression) : boolean;
var CmpExpr : TExpression;
begin
  CmpExpr := ExBinaryOp(
             ExBinaryOp(CopyExpr(First), CopyExpr(Expr), TkLessOrEquals),
             ExBinaryOp(CopyExpr(Expr), CopyExpr(Last), TkLessOrEquals),
             TkAnd);
  if CmpExpr^.Cls <> XcImmediate then
    CompileError('Internal error: could not calculate if an immediate value ' +
                 'belongs to a range');
  Result := CmpExpr^.Immediate.BooleanVal;
  DisposeExpr(CmpExpr)
end;

function ExSubrange;
begin
  if Parent^.Cls = XcImmediate then
  begin
    if not _ExBelongsToRange(Parent, TypePtr^.RangePtr^.First,
       TypePtr^.RangePtr^.Last) then
      CompileError('Value ' + DescribeExpr(Parent, 2) +
      ' out of bounds for ' + TypeName(TypePtr));
    Parent^.TypePtr := TypePtr;
    Result := Parent
  end
  else
  begin
    Result := _NewExpr(XcSubrange);
    Result^.SubrangeParent := Parent;
    Result^.TypePtr := TypePtr
  end
end;

function ExRerange(Expr : TExpression; TypePtr : TPsTypePtr) : TExpression;
begin
  if _ExBelongsToRange(Expr^.TypePtr^.RangePtr^.First,
     TypePtr^.RangePtr^.First,
     TypePtr^.RangePtr^.Last)
     and _ExBelongsToRange(Expr^.TypePtr^.RangePtr^.Last,
     TypePtr^.RangePtr^.First,
     TypePtr^.RangePtr^.Last) then
  begin
    Expr^.TypePtr := TypePtr;
    Result := Expr
  end
  else
    Result := ExSubrange(ExOutrange(Expr), TypePtr)
end;

function ExCoerce;
begin
  if IsRangeType(Expr^.TypePtr)
     and IsSameType(TypePtr, Expr^.TypePtr^.RangePtr^.BaseTypePtr) then
    ExCoerce := ExOutrange(Expr)
  else if IsRangeType(TypePtr)
          and IsSameType(TypePtr^.RangePtr^.BaseTypePtr, Expr^.TypePtr) then
         ExCoerce := ExSubrange(Expr, TypePtr)
  else if IsRangeType(Expr^.TypePtr) and IsRangeType(TypePtr)
          and IsSameType(Expr^.TypePtr^.RangePtr^.BaseTypePtr,
          TypePtr^.RangePtr^.BaseTypePtr) then
         ExCoerce := ExRerange(Expr, TypePtr)
  else if IsCharType(Expr^.TypePtr) and IsStringType(TypePtr) then
         ExCoerce := ExToString(Expr)
  else if IsIntegerType(Expr^.TypePtr) and IsRealType(TypePtr) then
         ExCoerce := ExToReal(Expr)
  else if IsSameType(Expr^.TypePtr, TypePtr) then
         ExCoerce := Expr
  else if IsNilType(Expr^.TypePtr) and IsPointeryType(TypePtr) then
         ExCoerce := Expr
  else
    CompileError('Type mismatch: expected ' + TypeName(TypePtr) +
    ', got ' + TypeName(Expr^.TypePtr))
end;

procedure ExMarkInitialized(Lhs : TExpression);
var IsTerminal : boolean;
begin
  IsTerminal := false;
  while not IsTerminal do
    case Lhs^.Cls of 
      XcField: Lhs := Lhs^.RecExpr;
      XcArray: Lhs := Lhs^.ArrayExpr;
      XcStringChar: Lhs := Lhs^.StringExpr;
      else IsTerminal := true
    end;
  if Lhs^.Cls = XcVariable then Lhs^.VarPtr^.WasInitialized := true
end;

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

procedure _DisposeImmediate(var Imm : TExImmediate);
var Bounds : TExSetBounds;
begin
  if Imm.Cls = XicSet then
  begin
    while Imm.SetBounds <> nil do
    begin
      Bounds := Imm.SetBounds^.Next;
      Dispose(Imm.SetBounds);
      Imm.SetBounds := Bounds
    end
  end
end;

procedure DisposeExpr;
var Pos : integer;
begin
  case Expr^.Cls of 
    XcImmediate : _DisposeImmediate(Expr^.Immediate);
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

function _CopyImmediate(const Imm : TExImmediate) : TExImmediate;
var Src, Dst : TExSetBounds;
begin
  Result := Imm;
  if Imm.Cls = XicSet then
  begin
    Src := Imm.SetBounds;
    New(Result.SetBounds);
    Dst := Result.SetBounds;
    while Src <> nil do
    begin
      Dst^ := Src^;
      Src := Src^.Next;
      if Src <> nil then
      begin
        New(Dst^.Next);
        Dst := Dst^.Next
      end
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
    XcImmediate: Copy^.Immediate := _CopyImmediate(Expr^.Immediate);
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
    else InternalError('Cannot copy expression: ' + DescribeExpr(Expr, 2))
  end;
  CopyExpr := Copy
end;

function _DescribeSet(Bounds : TExSetBounds;
                      SetOfTypePtr : TPsTypePtr) : string;
begin
  Result := '[';
  while Bounds <> nil do
  begin
    Result := Result +
              DescribeExpr(ExGetAntiOrdinal(Bounds^.First, SetOfTypePtr), 100);
    if Bounds^.First <> Bounds^.Last then
      Result := Result + '..' +
                DescribeExpr(ExGetAntiOrdinal(Bounds^.Last, SetOfTypePtr), 100);
    Bounds := Bounds^.Next;
    if Bounds <> nil then Result := Result + ', '
  end;
  Result := Result + ']'
end;

function _DescribeImmediate(Expr : TExpression) : string;
begin
  with Expr^.Immediate do
    case Cls of 
      XicNil: Result := 'nil';
      XicBoolean: Str(BooleanVal, Result);
      XicInteger: Str(IntegerVal, Result);
      XicReal: Str(RealVal, Result);
      XicChar: Result := UnparseChar(CharVal);
      XicString: Result := UnparseString(StringVal);
      XicEnum: Result := EnumPtr^.Values[EnumOrdinal];
      XicSet: Result := _DescribeSet(SetBounds, SetOfTypePtr);
      else InternalError('Cannot describe immediate value')
    end
end;

function _DescribePseudoFnExpr(Expr : TExpression) : string;
begin
  case Expr^.PseudoFn of 
    TpfConcat: Result := 'CONCAT';
    TpfDispose: Result := 'DISPOSE';
    TpfNew: Result := 'NEW';
    TpfOrd: Result := 'ORD';
    TpfPred: Result := 'PRED';
    TpfRead: Result := 'READ';
    TpfReadln: Result := 'READLN';
    TpfStr: Result := 'STR';
    TpfSucc: Result := 'SUCC';
    TpfVal: Result := 'VAL';
    TpfWrite: Result := 'WRITE';
    TpfWriteln: Result := 'WRITELN';
    else InternalError('Cannot describe pseudofun')
  end
end;

function _DescribePseudoCallExpr(Expr : TExpression; Levels : integer) : string;
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
    else InternalError('Cannot describe unary operation')
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
    else InternalError('Cannot describe binary operation')
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
      XcImmediate: Result := _DescribeImmediate(Expr);
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
      else InternalError('Cannot describe expression')
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

function ExSetConstant(Bounds : TExSetBounds;
                       TypePtr : TPsTypePtr) : TExpression;
var ElementType : TPsTypePtr;
begin
  ElementType := TypePtr^.SetDef.ElementTypePtr;
  if (ElementType <> nil) and not IsOrdinalType(ElementType) then
    CompileError('Not an ordinal type: ' + TypeName(ElementType));
  Result := _ExImmediate(XicSet);
  Result^.Immediate.SetBounds := Bounds;
  Result^.Immediate.SetOfTypePtr := ElementType;
  Result^.TypePtr := TypePtr
end;

function ExSetAddBounds(Bounds : TExSetBounds;
                        First, Last : integer) : TExSetBounds;
var 
  Prev, This, NewBounds : TExSetBounds;
  Done : boolean;
begin
  if First > Last then
    CompileError('Set bounds must appear in ascending order');
  Result := Bounds;
  Done := false;
  Prev := nil;
  This := Bounds;
  repeat
    {         nil  or         pppppp }
    { nnnnnnn          nnnnnn        }
    if (This = nil) or (Last + 1 < This^.First) then
    begin
      new(NewBounds);
      NewBounds^.First := First;
      NewBounds^.Last := Last;
      NewBounds^.Next := This;
      if Prev = nil then Result := NewBounds
      else Prev^.Next := NewBounds;
      Done := true
    end
    {      ppppp  or      ppppp  or     ppppp }
    {  nnnn           nnnnnn         nnnnnnnn }
    else if (First < This^.First) and (Last <= This^.Last) then
    begin
      This^.First := First;
      Done := true
    end
    { pppppppp  or  pppppppp  or  pppppppp  or  pppppppp }
    { nnnn              nnnn        nnnn        nnnnnnnn }
    else if (First >= This^.First) and (Last <= This^.Last) then
    begin
      Done := true
    end
    {    ppppppp   or   pppppp   }
    {  nnnnnnnnnn       nnnnnnnn }
    else if (First <= This^.First) and (Last > This^.Last) then
    begin
      NewBounds := This^.Next;
      Dispose(This);
      This := NewBounds;
      if Prev = nil then Result := NewBounds
      else Prev^.Next := NewBounds;
      Done := false
    end
    { pppppp     or  ppppp       }
    {   nnnnnnn           nnnnnn }
    else if (First > This^.First) and (First <= This^.Last + 1)
            and (Last > This^.Last) then
    begin
      First := This^.First;
      NewBounds := This^.Next;
      Dispose(This);
      This := NewBounds;
      if Prev = nil then Result := NewBounds
      else Prev^.Next := NewBounds;
      Done := false
    end
    { ppppppp       }
    {         nnnnn }
    else
    begin
      Prev := This;
      This := This^.Next;
      Done := false
    end
  until Done;
end;

function ExIsImmediate(Expr : TExpression) : boolean;
begin
  Result := Expr^.Cls = XcImmediate
end;

function ExIsImmediateOfClass(Expr : TExpression;
                              Cls : TExImmediateClass) : boolean;
begin
  Result := (Expr^.Cls = XcImmediate) and (Expr^.Immediate.Cls = Cls)
end;

function ExToString(Parent : TExpression) : TExpression;
var Str : string;
begin
  if IsCharType(Parent^.TypePtr) then
  begin
    if ExIsImmediate(Parent) then
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
  if ExIsImmediate(Parent) then
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
  Expr^.TypePtr := GetFundamentalType(Expr^.TypePtr);
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
                        Parent^.TypePtr^.ArrayDef.IndexTypePtr);
  Result^.TypePtr := Parent^.TypePtr^.ArrayDef.ValueTypePtr;
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
          Result := ExSetTmpVar(ExVariable(AddTmpVariable(
                    'tmp', FnExpr^.FnPtr^.Args[Pos].TypePtr)),
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
    InternalError('Expected a pseudofunction, got ' + DescribeExpr(Expr, 10));
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

procedure _ExSetCoerceToCommon(Left, Right : TExpression);
var LeftType, RightType : TPsTypePtr;
begin
  LeftType := Left^.TypePtr;
  RightType := Right^.TypePtr;
  if LeftType^.SetDef.ElementTypePtr = nil then
    LeftType^.SetDef.ElementTypePtr := RightType^.SetDef.ElementTypePtr
  else if RightType^.SetDef.ElementTypePtr = nil then
         RightType^.SetDef.ElementTypePtr := LeftType^.SetDef.ElementTypePtr
  else if not IsSameType(Left^.Immediate.SetOfTypePtr,
          Right^.Immediate.SetOfTypePtr) then
         CompileError('Type mismatch: cannot combine ' +
                      TypeName(Left^.TypePtr) + ' with ' +
         TypeName(Right^.TypePtr))
  else
  begin
    LeftType^.SetDef.ElementTypePtr := Left^.Immediate.SetOfTypePtr;
    RightType^.SetDef.ElementTypePtr := Right^.Immediate.SetOfTypePtr
  end
end;

function _ExSetUnion(Left, Right : TExpression) : TExpression;
var NewBds, OldBds : TExSetBounds;
begin
  _ExSetCoerceToCommon(Left, Right);
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
  DisposeExpr(Left);
  DisposeExpr(Right)
end;

function _ExSetDifference(Left, Right : TExpression) : TExpression;
var 
  LtBds, RtBds, NewBds : TExSetBounds;
begin
  _ExSetCoerceToCommon(Left, Right);
  LtBds := Left^.Immediate.SetBounds;
  RtBds := Right^.Immediate.SetBounds;
  NewBds := nil;
  while (LtBds <> nil) and (RtBds <> nil) do
  begin
    {      llllll  or      llllll }
    { rrrr             rrrr       }
    { Go to next right }
    if RtBds^.Last < LtBds^.First then RtBds := RtBds^.Next
    {     lllll  or  lllll }
    {  rrrrr     or  rrr   }
    { Trim left to end of right }
    else if (RtBds^.First <= LtBds^.First) and (RtBds^.Last < LtBds^.Last) then
           LtBds^.First := RtBds^.Last + 1
    {     llll  or    ll  }
    {   rrrrrr      rrrrr }
    { Go to next left }
    else if (RtBds^.First <= LtBds^.First) and (RtBds^.Last >= LtBds^.Last) then
           LtBds := LtBds^.Next
    { llllll }
    {  rrr   }
    { Add part of left before right, trim left to end of right }
    else if (RtBds^.First > LtBds^.First) and (RtBds^.Last < LtBds^.Last) then
    begin
      NewBds := ExSetAddBounds(NewBds, LtBds^.First, RtBds^.First - 1);
      LtBds^.First := RtBds^.Last + 1
    end
    { llllll  or  lllllll    }
    {   rrrr        rrrrrrrr }
    { Add part of left before right, go to next left }
    else if (RtBds^.First > LtBds^.First) and (RtBds^.Last = RtBds^.Last) then
    begin
      NewBds := ExSetAddBounds(NewBds, LtBds^.First, RtBds^.First - 1);
      LtBds := LtBds^.Next
    end
    { lllll      or  lllll       }
    {      rrrr            rrrrr }
    { Add left, go to next left }
    else if RtBds^.First > LtBds^.Last then
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
  DisposeExpr(Left);
  DisposeExpr(Right)
end;

function _ExSetIntersection(Left, Right : TExpression) : TExpression;
var 
  LtBds, RtBds, NewBds : TExSetBounds;
begin
  _ExSetCoerceToCommon(Left, Right);
  LtBds := Left^.Immediate.SetBounds;
  RtBds := Right^.Immediate.SetBounds;
  NewBds := nil;
  while (LtBds <> nil) and (RtBds <> nil) do
  begin
    {      llllll  or      llllll }
    { rrrr             rrrr       }
    { Go to next right }
    if RtBds^.Last < LtBds^.First then RtBds := RtBds^.Next
    {     lllll  or  lllll }
    {  rrrrr     or  rrr   }
    { Add first left to last right, go to next right }
    else if (RtBds^.First <= LtBds^.First) and (RtBds^.Last < LtBds^.Last) then
    begin
      NewBds := ExSetAddBounds(NewBds, LtBds^.First, RtBds^.Last);
      RtBds := RtBds^.Next
    end
    {     llll  or    ll  }
    {   rrrrrr      rrrrr }
    { Add left, go to next left }
    else if (RtBds^.First <= LtBds^.First) and (RtBds^.Last >= LtBds^.Last) then
    begin
      NewBds := ExSetAddBounds(NewBds, LtBds^.First, LtBds^.Last);
      LtBds := LtBds^.Next
    end
    { llllll }
    {  rrr   }
    { Add right, go to next right }
    else if (RtBds^.First > LtBds^.First) and (RtBds^.Last < LtBds^.Last) then
    begin
      NewBds := ExSetAddBounds(NewBds, RtBds^.First, RtBds^.Last);
      RtBds := RtBds^.Next
    end
    { llllll  or  lllllll    }
    {   rrrr        rrrrrrrr }
    { Add first right to last left, go to next left }
    else if (RtBds^.First > LtBds^.First) and (RtBds^.Last = RtBds^.Last) then
    begin
      NewBds := ExSetAddBounds(NewBds, RtBds^.First, LtBds^.Last);
      LtBds := LtBds^.Next
    end
    { lllll      or  lllll       }
    {      rrrr            rrrrr }
    { Go to next left }
    else if RtBds^.First > LtBds^.Last then
           LtBds := LtBds^.Next
  end;
  Result := ExSetConstant(NewBds, Left^.TypePtr);
  DisposeExpr(Left);
  DisposeExpr(Right)
end;

function _ExSetEquals(Left, Right : TExpression;
                      Negate : boolean) : TExpression;
var 
  LtBds, RtBds : TExSetBounds;
  Equals : boolean;
begin
  _ExSetCoerceToCommon(Left, Right);
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
  if Negate then Equals := not Equals;
  Result := ExBooleanConstant(Equals);
  DisposeExpr(Left);
  DisposeExpr(Right)
end;

function _ExSetSubset(Left, Right : TExpression) : TExpression;
var 
  LtBds, RtBds : TExSetBounds;
  Subset : boolean;
begin
  _ExSetCoerceToCommon(Left, Right);
  LtBds := Left^.Immediate.SetBounds;
  RtBds := Right^.Immediate.SetBounds;
  Subset := true;
  while (LtBds <> nil) and (RtBds <> nil) and Subset do
  begin
    { llllll       or   lllll      }
    {       rrrrr             rrrr }
    { Go to next left }
    if (LtBds^.Last < RtBds^.First) then
      LtBds := LtBds^.Next
    { lllllll }
    {  rrrr   }
    { Go to next right }
    else if (LtBds^.First <= RtBds^.First) and (LtBds^.Last >= RtBds^.Last) then
           RtBds := RtBds^.Next
    else
      Subset := false
  end;
  Subset := Subset and (RtBds = nil);
  Result := ExBooleanConstant(Subset);
  DisposeExpr(Left);
  DisposeExpr(Right)
end;

function _ExSetIn(Needle, Haystack : TExpression) : TExpression;
var 
  ElemType : TPsTypePtr;
  Bounds : TExSetBounds;
  Cond : TExpression;
  TmpVar : TPsVarPtr;
  Wanted : TExpression;
begin
  ElemType := Haystack^.TypePtr^.SetDef.ElementTypePtr;
  if ElemType <> nil then ElemType := GetFundamentalType(ElemType);
  if (ElemType <> nil)
     and not IsSameType(GetFundamentalType(Needle^.TypePtr), ElemType) then
    CompileError('Types of ' + DescribeExpr(Needle, 10) + ' and ' +
    DescribeExpr(Haystack, 10) + ' are incompatible: ' +
    TypeName(Needle^.TypePtr) + ' and ' + TypeName(Haystack^.TypePtr));
  if (Needle^.Cls <> XcVariable) and (ElemType <> nil) then
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
  Bounds := Haystack^.Immediate.SetBounds;
  while Bounds <> nil do
  begin
    if Bounds^.First = Bounds^.Last then
      Cond := ExBinaryOp(CopyExpr(Wanted),
              ExGetAntiOrdinal(Bounds^.First, ElemType),
              TkEquals)
    else
      Cond := ExBinaryOp(
              ExBinaryOp(ExGetAntiOrdinal(Bounds^.First, ElemType),
              CopyExpr(Wanted),
              TkLessOrEquals),
              ExBinaryOp(CopyExpr(Wanted),
              ExGetAntiOrdinal(Bounds^.Last, ElemType),
              TkLessOrEquals),
              TkAnd);
    Result := ExBinaryOp(Result, Cond, TkOr);
    Bounds := Bounds^.Next
  end;
  if TmpVar <> nil then
  begin
    Result := ExSetTmpVar(Wanted, Needle, Result);
  end
  else DisposeExpr(Needle);
  DisposeExpr(Haystack)
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

  if ExIsImmediate(Parent) then ExUnaryOp := _ExUnOpImm(Parent, Op)
  else ExUnaryOp := _ExUnOpCmp(Parent, Op)
end;

function _ExUnOpImm;
begin
  if (Op = TkMinus) and ExIsImmediateOfClass(Parent, XicInteger) then
    Parent^.Immediate.IntegerVal := -Parent^.Immediate.IntegerVal
  else if (Op = TkPlus) and ExIsImmediateOfClass(Parent, XicInteger) then
    { do nothing }
  else if (Op = TkMinus) and ExIsImmediateOfClass(Parent, XicReal) then
         Parent^.Immediate.RealVal := -Parent^.Immediate.RealVal
  else if (Op = TkPlus) and ExIsImmediateOfClass(Parent, XicReal) then
    { do nothing }
  else if (Op = TkNot) and ExIsImmediateOfClass(Parent, XicBoolean) then
         Parent^.Immediate.BooleanVal := not Parent^.Immediate.BooleanVal
  else if (Op = TkNot) and ExIsImmediateOfClass(Parent, XicInteger) then
         Parent^.Immediate.IntegerVal := not Parent^.Immediate.IntegerVal
  else InternalError('Invalid immediate unary operation');
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
function _ExBinOpSetImm(Left, Right : TExpression;
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
function _ExBinOpSetCmp(Left, Right : TExpression;
                        Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpShortcut(var Left, Right : TExpression;
                          Op : TLxTokenId) : boolean;
forward;
function ExBinaryOp(Left, Right : TExpression;
                    Op : TLxTokenId) : TExpression;
var 
  Immediate : boolean;
begin
  Left := ExOutrange(Left);
  Right := ExOutrange(Right);
  Immediate := ExIsImmediate(Left) and ExIsImmediate(Right);

  if _ExBinOpShortcut(Left, Right, Op) then
  begin
    Result := Left
  end
  else if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
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
  else if IsSetType(Right^.TypePtr) then
  begin
    if ExIsImmediate(Right) then Result := _ExBinOpSetImm(Left, Right, Op)
    else Result := _ExBinOpSetCmp(Left, Right, Op)
  end
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
  if ExIsImmediateOfClass(Left, XicInteger) then
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
  if ExIsImmediateOfClass(Left, XicReal) then
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
  if ExIsImmediateOfClass(Left, XicChar) then Lt := Left^.Immediate.CharVal
  else Lt := Left^.Immediate.StringVal;
  if ExIsImmediateOfClass(Right, XicChar) then Rt := Right^.Immediate.CharVal
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
  if ExIsImmediateOfClass(Left, XicString) then
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

function _ExBinOpSetImm;
begin
  if IsSetType(Left^.TypePtr) and IsSetType(Right^.TypePtr) then
    case Op of 
      TkPlus: Result := _ExSetUnion(Left, Right);
      TkMinus: Result := _ExSetDifference(Left, Right);
      TkAsterisk: Result := _ExSetIntersection(Left, Right);
      TkEquals: Result := _ExSetEquals(Left, Right, false);
      TkNotEquals: Result := _ExSetEquals(Left, Right, true);
      TkMoreOrEquals: Result := _ExSetSubset(Left, Right);
      TkLessOrEquals: Result := _ExSetSubset(Right, Left);
      else CompileError('Invalid set operator: ' + LxTokenName(Op))
    end
  else if Op = TkIn then Result := _ExSetIn(Left, Right)
  else CompileError('Invalid set operator: ' + LxTokenName(Op))
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

function _ExBinOpSetCmp;
begin
  CompileError('Invalid set operator: ' + LxTokenName(Op))
end;

{ Returns whether an expression evaluates to 0 integer or real. }
function _ExIsZero(Expr : TExpression) : boolean;
begin
  Result := (ExIsImmediateOfClass(Expr, XicInteger)
            and (Expr^.Immediate.IntegerVal = 0))
            or (ExIsImmediateOfClass(Expr, XicReal)
            and (Expr^.Immediate.
            IntegerVal = 0.0))
end;

{ Returns whether an expression evaluates to 1 integer or real. }
function _ExIsOne(Expr : TExpression) : boolean;
begin
  Result := (ExIsImmediateOfClass(Expr, XicInteger)
            and (Expr^.Immediate.IntegerVal = 1))
            or (ExIsImmediateOfClass(Expr, XicReal)
            and (Expr^.Immediate.
            IntegerVal = 1.0))
end;

{ Returns whether an expression evaluates to true. }
function _ExIsTrue(Expr : TExpression) : boolean;
begin
  Result := ExIsImmediateOfClass(Expr, XicBoolean)
            and Expr^.Immediate.BooleanVal
end;

{ Returns whether an expression evaluates to false. }
function _ExIsFalse(Expr : TExpression) : boolean;
begin
  Result := ExIsImmediateOfClass(Expr, XicBoolean)
            and not Expr^.Immediate.BooleanVal
end;

{ Computes a shortcut operation, if appropriate. }
{ Returns true if the shortcut was taken. }
{ Also, if the shortcut was taken, returns the result in the Left expression }
{ and frees the Right expression. }
function _ExBinOpShortcut(var Left, Right : TExpression;
                          Op : TLxTokenId) : boolean;
var Use : (UseLeft, UseRight, Keep);
begin
  Use := Keep;
  case Op of 
    { X + 0 -> X ; 0 + X -> X }
    TkPlus : if _ExIsZero(Left) then Use := UseRight
             else if _ExIsZero(Right) then Use := UseLeft;
    { X - 0 -> X }
    TkMinus : if _ExIsZero(Right) then Use := UseLeft;
    { X * 1 -> X ; 1 * X -> X}
    TkAsterisk : if _ExIsOne(Left) then Use := UseRight
                 else if _ExIsOne(Right) then Use := UseLeft;
    { X / 1 -> X }
    TkSlash : if _ExIsOne(Right) then Use := UseLeft;
    { X div 1 -> X }
    TkDiv : if _ExIsOne(Right) then Use := UseLeft;
    { false AND X -> false ; X AND true -> X ; true AND X -> X }
    TkAnd : if _ExIsFalse(Left) or _ExIsTrue(Right) then Use := UseLeft
            else if _ExIsTrue(Left) then Use := UseRight;
    { true OR X -> true ; X OR false -> X ; false OR X -> X }
    TkOr : if _ExIsTrue(Left) or _ExIsFalse(Right) then Use := UseLeft
           else if _ExIsFalse(Left) then Use := UseRight;
  end;
  case Use of 
    UseLeft :
              begin
                DisposeExpr(Right);
                Result := true
              end;
    UseRight :
               begin
                 DisposeExpr(Left);
                 Left := Right;
                 Result := true
               end;
    Keep : Result := false
  end
end;

function ExGetOrdinal(Expr : TExpression) : integer;
begin
  if not ExIsImmediate(Expr) then
    CompileError('Expression is not immediate: ' + DescribeExpr(Expr, 5));
  with Expr^.Immediate do
    case Cls of 
      XicBoolean : Result := Ord(BooleanVal);
      XicInteger : Result := IntegerVal;
      XicChar : Result := Ord(CharVal);
      XicEnum : Result := EnumOrdinal;
      else CompileError('Expression does not belong to an ordinal type: ' +
                        DescribeExpr(Expr, 5))
    end
end;

function ExGetAntiOrdinal(Ordinal : integer;
                          TypePtr : TPsTypePtr) : TExpression;
begin
  TypePtr := GetFundamentalType(TypePtr);
  case TypePtr^.Cls of 
    TtcBoolean: if Ordinal = 0 then Result := ExBooleanConstant(false)
                else Result := ExBooleanConstant(true);
    TtcInteger: Result := ExIntegerConstant(Ordinal);
    TtcChar: Result := ExCharConstant(Chr(Ordinal));
    TtcEnum: Result := ExEnumConstant(Ordinal, TypePtr);
    else InternalError('Cannot compute anti-ordinal for ' + TypeName(TypePtr))
  end
end;

function ExSubrange;
var Ordinal : integer;
begin
  if ExIsImmediate(Parent) then
  begin
    Ordinal := ExGetOrdinal(Parent);
    if (Ordinal < TypePtr^.RangeDef.First)
       or (Ordinal > TypePtr^.RangeDef.Last) then
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
  if (TypePtr^.RangeDef.First <= Expr^.TypePtr^.RangeDef.First)
     and (Expr^.TypePtr^.RangeDef.Last <= TypePtr^.RangeDef.Last) then
  begin
    Expr^.TypePtr := TypePtr;
    Result := Expr
  end
  else
    Result := ExSubrange(ExOutrange(Expr), TypePtr)
end;

function _ExCoerceSet(Expr : TExpression; TypePtr : TPsTypePtr) : TExpression;
var 
  Outcome : (Pass, Reject, Replace);
  ExprElemType, DestElemType : TPsTypePtr;
begin
  ExprElemType := Expr^.TypePtr^.SetDef.ElementTypePtr;
  DestElemType := TypePtr^.SetDef.ElementTypePtr;
  if ExprElemType = nil then Outcome := Replace
  else if not IsSameType(GetFundamentalType(ExprElemType),
          GetFundamentalType(DestElemType)) then Outcome := Reject
  else if ExIsImmediate(Expr) then Outcome := Replace
  else if (GetTypeLowBound(ExprElemType) = GetTypeLowBound(DestElemType))
          and (GetTypeHighBound(ExprElemType) = GetTypeHighBound(DestElemType))
         then Outcome := Pass
  else Outcome := Reject;

  case Outcome of 
    Reject : CompileError('Type mismatch: ' + TypeName(Expr^.TypePtr) +
             ' cannot be assigned to ' + TypeName(TypePtr));
    Replace : Expr^.TypePtr := TypePtr;
    Pass : { do nothing };
  end;
  Result := Expr
end;

function ExCoerce;
begin
  if IsRangeType(Expr^.TypePtr)
     and IsSameType(TypePtr, GetFundamentalType(Expr^.TypePtr)) then
    ExCoerce := ExOutrange(Expr)
  else if IsRangeType(TypePtr)
          and IsSameType(GetFundamentalType(TypePtr), Expr^.TypePtr) then
         ExCoerce := ExSubrange(Expr, TypePtr)
  else if IsRangeType(Expr^.TypePtr) and IsRangeType(TypePtr)
          and IsSameType(GetFundamentalType(Expr^.TypePtr),
          GetFundamentalType(TypePtr)) then
         ExCoerce := ExRerange(Expr, TypePtr)
  else if IsCharType(Expr^.TypePtr) and IsStringType(TypePtr) then
         ExCoerce := ExToString(Expr)
  else if IsIntegerType(Expr^.TypePtr) and IsRealType(TypePtr) then
         ExCoerce := ExToReal(Expr)
  else if IsSameType(Expr^.TypePtr, TypePtr) then
         ExCoerce := Expr
  else if IsNilType(Expr^.TypePtr) and IsPointeryType(TypePtr) then
         ExCoerce := Expr
  else if IsSetType(Expr^.TypePtr) and IsSetType(TypePtr) then
  begin
    ExCoerce := _ExCoerceSet(Expr, TypePtr)
  end
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

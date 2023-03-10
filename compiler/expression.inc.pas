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
  if Call.Arg1 <> nil then ExDispose(Call.Arg1);
  if Call.Arg2 <> nil then ExDispose(Call.Arg2);
  if Call.Arg3 <> nil then ExDispose(Call.Arg3);
  if Call.Arg4 <> nil then ExDispose(Call.Arg4);
  if Call.PseudoFn in [TpfRead, TpfReadln] then
  begin
    ReadArg := Call.ReadArgs;
    while ReadArg <> nil do
    begin
      NextReadArg := ReadArg^.Next;
      ExDispose(ReadArg^.Arg);
      dispose(ReadArg);
      ReadArg := NextReadArg
    end
  end
  else if Call.PseudoFn in [TpfWrite, TpfWriteln] then
  begin
    WriteArg := Call.WriteArgs;
    while WriteArg <> nil do
    begin
      NextWriteArg := WriteArg^.Next;
      ExDispose(WriteArg^.Arg);
      if WriteArg^.Width <> nil then ExDispose(WriteArg^.Width);
      if WriteArg^.Prec <> nil then ExDispose(WriteArg^.Prec);
      dispose(WriteArg);
      WriteArg := NextWriteArg
    end
  end
end;

procedure _DisposeImmediate(var Imm : TExImmediate);
var Bounds : TExSetImmBounds;
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

procedure _DisposeBounds(Bounds : TExSetExprBounds);
var Next : TExSetExprBounds;
begin
  while Bounds <> nil do
  begin
    Next := Bounds^.Next;
    ExDispose(Bounds^.First);
    if Bounds^.Last <> nil then ExDispose(Bounds^.Last);
    dispose(Bounds);
    Bounds := Next
  end
end;

procedure ExDispose;
var Pos : integer;
begin
  case Expr^.Cls of 
    XcImmediate : _DisposeImmediate(Expr^.Immediate);
    XcToString : ExDispose(Expr^.ToStrParent);
    XcToReal : ExDispose(Expr^.ToRealParent);
    XcWithTmpVar :
                   begin
                     ExDispose(Expr^.TmpVar);
                     ExDispose(Expr^.TmpVarValue);
                     ExDispose(Expr^.TmpVarChild);
                   end;
    XcSubrange : ExDispose(Expr^.SubrangeParent);
    XcSet :
            begin
              ExDispose(Expr^.SetBase);
              _DisposeBounds(Expr^.SetBounds);
            end;
    XcField : ExDispose(Expr^.RecExpr);
    XcArray :
              begin
                ExDispose(Expr^.ArrayExpr);
                ExDispose(Expr^.ArrayIndex);
              end;
    XcPointer : ExDispose(Expr^.PointerExpr);
    XcStringChar :
                   begin
                     ExDispose(Expr^.StringExpr);
                     ExDispose(Expr^.StringIndex);
                   end;
    XcFnCall :
               begin
                 ExDispose(Expr^.FnExpr);
                 for Pos := 1 to Expr^.CallArgs.Size do
                   ExDispose(Expr^.CallArgs.Values[Pos]);
               end;
    XcPseudoFnCall : _DisposePseudoCallExpr(Expr^.PseudoFnCall);
    XcUnaryOp : ExDispose(Expr^.Unary.Parent);
    XcBinaryOp :
                 begin
                   ExDispose(Expr^.Binary.Left);
                   ExDispose(Expr^.Binary.Right);
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
  if Call.Arg1 <> nil then Copy.Arg1 := ExCopy(Call.Arg1);
  if Call.Arg2 <> nil then Copy.Arg2 := ExCopy(Call.Arg2);
  if Call.Arg3 <> nil then Copy.Arg3 := ExCopy(Call.Arg3);
  if Call.Arg4 <> nil then Copy.Arg4 := ExCopy(Call.Arg4);
  if Call.PseudoFn in [TpfRead, TpfReadln] then
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
      CopyReadArg^.Arg := ExCopy(ReadArg^.Arg);
      ReadArg := NextReadArg
    end
  end
  else if Call.PseudoFn in [TpfWrite, TpfWriteln] then
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
      CopyWriteArg^.Arg := ExCopy(WriteArg^.Arg);
      if WriteArg^.Width <> nil then
        CopyWriteArg^.Width := ExCopy(WriteArg^.Width);
      if WriteArg^.Prec <> nil then
        CopyWriteArg^.Prec := ExCopy(WriteArg^.Prec);
      WriteArg := NextWriteArg
    end
  end
end;

function _CopyImmediate(const Imm : TExImmediate) : TExImmediate;
var Src, Dst : TExSetImmBounds;
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

function _CopyBounds(Bounds : TExSetExprBounds) : TExSetExprBounds;
var Src, Dst : TExSetExprBounds;
begin
  Src := Bounds;
  Dst := nil;
  while Src <> nil do
  begin
    if Dst = nil then
    begin
      new(Dst);
      Result := Dst;
    end
    else
    begin
      new(Dst^.Next);
      Dst := Dst^.Next
    end;
    Dst^.First := ExCopy(Src^.First);
    if Src^.Last <> nil then Dst^.Last := ExCopy(Src^.Last)
    else Dst^.Last := nil;
    Dst^.Next := nil;
    Src := Src^.Next
  end
end;

function ExCopy;
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
    XcToString: Copy^.ToStrParent := ExCopy(Expr^.ToStrParent);
    XcToReal: Copy^.ToRealParent := ExCopy(Expr^.ToRealParent);
    XcWithTmpVar :
                   begin
                     Copy^.TmpVar := ExCopy(Expr^.TmpVar);
                     Copy^.TmpVarValue := ExCopy(Expr^.TmpVarValue);
                     Copy^.TmpVarChild := ExCopy(Expr^.TmpVarChild);
                   end;
    XcSubrange : Copy^.SubrangeParent := ExCopy(Expr^.SubrangeParent);
    XcSet :
            begin
              Copy^.SetBase := ExCopy(Expr^.SetBase);
              Copy^.SetBounds := _CopyBounds(Expr^.SetBounds);
            end;
    XcVariable: Copy^.VarPtr := Expr^.VarPtr;
    XcField:
             begin
               Copy^.RecExpr := ExCopy(Expr^.RecExpr);
               Copy^.RecFieldNum := Expr^.RecFieldNum
             end;
    XcArray:
             begin
               Copy^.ArrayExpr := ExCopy(Expr^.ArrayExpr);
               Copy^.ArrayIndex := ExCopy(Expr^.ArrayIndex)
             end;
    XcPointer: Copy^.PointerExpr := ExCopy(Expr^.PointerExpr);
    XcStringChar:
                  begin
                    Copy^.StringExpr := ExCopy(Expr^.StringExpr);
                    Copy^.StringIndex := ExCopy(Expr^.StringIndex)
                  end;
    XcFnRef: Copy^.FnPtr := Expr^.FnPtr;
    XcFnCall:
              begin
                Copy^.FnExpr := ExCopy(Expr^.FnExpr);
                Copy^.CallArgs.Size := Expr^.CallArgs.Size;
                for Pos := 1 to Expr^.CallArgs.Size do
                  Copy^.CallArgs.Values[Pos] := ExCopy(Expr^.CallArgs
                                                .Values[Pos])
              end;
    XcPseudoFnRef: Copy^.PseudoFn := Expr^.PseudoFn;
    XcPseudoFnCall: _CopyPseudoCallExpr(Expr^.PseudoFnCall,
                                        Copy^.PseudoFnCall);
    XcUnaryOp:
               begin
                 Copy^.Unary.Parent := ExCopy(Expr^.Unary.Parent);
                 Copy^.Unary.Op := Expr^.Unary.Op
               end;
    XcBinaryOp:
                begin
                  Copy^.Binary.Left := ExCopy(Expr^.Binary.Left);
                  Copy^.Binary.Right := ExCopy(Expr^.Binary.Right);
                  Copy^.Binary.Op := Expr^.Binary.Op
                end;
    else InternalError('Cannot copy expression: ' + ExDescribe(Expr))
  end;
  ExCopy := Copy
end;

function _DescribeImmSetInternal(Bounds : TExSetImmBounds;
                                 SetOfTypePtr : TPsTypePtr) : string;
begin
  Result := '';
  while Bounds <> nil do
  begin
    Result := Result +
              ExDescribe(ExGetAntiOrdinal(Bounds^.First, SetOfTypePtr));
    if Bounds^.First <> Bounds^.Last then
      Result := Result + '..' +
                ExDescribe(ExGetAntiOrdinal(Bounds^.Last, SetOfTypePtr));
    Bounds := Bounds^.Next;
    if Bounds <> nil then Result := Result + ', '
  end
end;

function _DescribeImmSet(Bounds : TExSetImmBounds;
                         SetOfTypePtr : TPsTypePtr) : string;
begin
  Result := '[' + _DescribeImmSetInternal(Bounds, SetOfTypePtr) + ']'
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
      XicSet: Result := _DescribeImmSet(SetBounds, SetOfTypePtr);
      else InternalError('Cannot describe immediate value')
    end
end;

function _ExprPrecedence(Expr : TExpression) : integer;
begin
  case Expr^.Cls of 
    XcImmediate: Result := 0;
    XcToString: Result := _ExprPrecedence(Expr^.ToStrParent);
    XcToReal: Result := _ExprPrecedence(Expr^.ToRealParent);
    XcWithTmpVar: Result := _ExprPrecedence(Expr^.TmpVarChild);
    XcSubrange: Result := _ExprPrecedence(Expr^.SubrangeParent);
    XcSet: Result := 0;
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
                  TkXor: Result := 4;
                  TkShl: Result := 3;
                  TkShr: Result := 3;
                  TkIn: Result := 5;
                  TkEquals: Result := 5;
                  TkNotEquals: Result := 5;
                  TkLessthan: Result := 5;
                  TkMorethan: Result := 5;
                  TkLessOrEquals: Result := 5;
                  TkMoreOrEquals: Result := 5;
                end;
  end;
end;

function _DescribeUnaryOpExpr(Expr : TExpression) : string;
var UseParens : boolean;
begin
  case Expr^.Unary.Op of 
    TkMinus: Result := '-';
    TkNot: Result := 'not ';
    else InternalError('Cannot describe unary operation')
  end;
  UseParens := _ExprPrecedence(Expr) < _ExprPrecedence(Expr^.Unary.Parent);
  if UseParens then Result := Result + '(';
  Result := Result + ExDescribe(Expr^.Unary.Parent);
  if UseParens then Result := Result + ')';
end;

function _DescribeBinaryOpExpr(Expr : TExpression) : string;
var UseParens : boolean;
begin
  UseParens := _ExprPrecedence(Expr) < _ExprPrecedence(Expr^.Binary.Left);
  if UseParens then Result := '('
  else Result := '';
  Result := Result + ExDescribe(Expr^.Binary.Left);
  if UseParens then Result := Result + ')';
  case Expr^.Binary.Op of 
    TkPlus: Result := Result + ' + ';
    TkMinus: Result := Result + ' - ';
    TkAsterisk: Result := Result + ' * ';
    TkSlash: Result := Result + ' / ';
    TkDiv: Result := Result + ' div ';
    TkAnd: Result := Result + ' and ';
    TkOr: Result := Result + ' or ';
    TkXor: Result := Result + ' xor ';
    TkShl: Result := Result + ' shl ';
    TkShr: Result := Result + ' shr ';
    TkIn: Result := Result + ' in ';
    TkEquals: Result := Result + ' = ';
    TkNotEquals: Result := Result + ' <> ';
    TkLessthan: Result := Result + ' < ';
    TkMorethan: Result := Result + ' > ';
    TkLessOrEquals: Result := Result + ' <= ';
    TkMoreOrEquals: Result := Result + ' >= ';
    else InternalError('Cannot describe binary operation for operator ' +
                       LxTokenName(Expr^.Binary.Op) + ' and operands ' +
      ExDescribe(Expr^.Binary.Left) +
      ' and ' + ExDescribe(Expr^.Binary.Right))
  end;
  UseParens := _ExprPrecedence(Expr) < _ExprPrecedence(Expr^.Binary.Right);
  if UseParens then Result := Result + '(';
  Result := Result + ExDescribe(Expr^.Binary.Right);
  if UseParens then Result := Result + ')';
end;

function _DescribeWithTmpVar(Expr : TExpression) : string;
begin
  Result := '{with ';
  while Expr^.Cls = XcWithTmpVar do
  begin
    Result := Result + ExDescribe(Expr^.TmpVar) + ':=' +
              ExDescribe(Expr^.TmpVarValue);
    Expr := Expr^.TmpVarChild;
    if Expr^.Cls = XcWithTmpVar then Result := Result + ', '
  end;
  Result := Result + '} ' + ExDescribe(Expr)
end;

function _DescribeSet(Expr : TExpression) : string;
var 
  Bounds : TExSetExprBounds;
begin
  Result := '[' + _DescribeImmSetInternal(Expr^.SetBase^.Immediate.SetBounds,
            Expr^.SetBase^.Immediate.SetOfTypePtr);
  if Expr^.SetBase^.Immediate.SetBounds <> nil then Result := Result + ', ';
  Bounds := Expr^.SetBounds;
  while Bounds <> nil do
  begin
    Result := Result + ExDescribe(Bounds^.First);
    if Bounds^.Last <> nil then
      Result := Result + '..' + ExDescribe(Bounds^.Last);
    Bounds := Bounds^.Next;
    if Bounds <> nil then Result := Result + ', '
  end;
  Result := Result + ']'
end;

function ExDescribe;
var 
  Pos : integer;
begin
  case Expr^.Cls of 
    XcImmediate: Result := _DescribeImmediate(Expr);
    XcToString: Result := ExDescribe(Expr^.ToStrParent);
    XcToReal: Result := ExDescribe(Expr^.ToRealParent);
    XcWithTmpVar: Result := _DescribeWithTmpVar(Expr);
    XcSubrange: Result := ExDescribe(Expr^.ToStrParent);
    XcSet: Result := _DescribeSet(Expr);
    XcVariable: Result := Expr^.VarPtr^.Name;
    XcField: Result := ExDescribe(Expr^.RecExpr) + '.' +
                       Expr^.RecExpr^.TypePtr^.RecPtr^
                       .Fields[Expr^.RecFieldNum].Name;
    XcArray: Result := ExDescribe(Expr^.ArrayExpr) +
                       '[' + ExDescribe(Expr^.ArrayIndex) + ']';
    XcPointer: Result := ExDescribe(Expr^.PointerExpr) + '^';
    XcStringChar: Result := ExDescribe(Expr^.StringExpr) + '[' +
                            ExDescribe(Expr^.StringIndex) + ']';
    XcFnRef: Result := Expr^.FnPtr^.Name;
    XcFnCall:
              begin
                Result := ExDescribe(Expr^.FnExpr) + '(';
                for Pos := 1 to Expr^.CallArgs.Size do
                begin
                  if Pos <> 1 then Result := Result + ', ';
                  Result := Result + ExDescribe(Expr^.CallArgs.Values[Pos])
                end;
                Result := Result + ')'
              end;
    XcPseudoFnRef: Result := Pf_DescribeName(Expr);
    XcPseudoFnCall: Result := Pf_DescribeCall(Expr);
    XcUnaryOp: Result := _DescribeUnaryOpExpr(Expr);
    XcBinaryOp: Result := _DescribeBinaryOpExpr(Expr);
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
  EnsureEnumType(TypePtr);
  if (Ordinal < 0) or (Ordinal > TypePtr^.EnumPtr^.Size - 1) then
    CompileError('Invalid value for ' + TypeName(TypePtr));
  Result := _ExImmediate(XicEnum);
  Result^.Immediate.EnumOrdinal := Ordinal;
  Result^.Immediate.EnumPtr := TypePtr^.EnumPtr;
  Result^.TypePtr := TypePtr
end;

function ExSetConstant(Bounds : TExSetImmBounds;
                       TypePtr : TPsTypePtr) : TExpression;
var ElementType : TPsTypePtr;
begin
  ElementType := TypePtr^.ElementTypePtr;
  if ElementType <> nil then EnsureOrdinalType(ElementType);
  Result := _ExImmediate(XicSet);
  Result^.Immediate.SetBounds := Bounds;
  Result^.Immediate.SetOfTypePtr := ElementType;
  Result^.TypePtr := TypePtr
end;

function ExSetAddBounds(Bounds : TExSetImmBounds;
                        First, Last : integer) : TExSetImmBounds;
var 
  Prev, This, NewBounds : TExSetImmBounds;
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

function ExSet : TExpression;
var SetType : TPsType;
begin
  SetType := EmptyType;
  SetType.Cls := TtcSet;
  SetType.ElementTypePtr := nil;
  Result := _ExImmediate(XicSet);
  Result^.Immediate.SetBounds := nil;
  Result^.Immediate.SetOfTypePtr := nil;
  Result^.TypePtr := AddType(SetType)
end;

function ExSetAddRange(SetExpr : TExpression;
                       First, Last : TExpression) : TExpression;
var 
  ElementTypePtr : TPsTypePtr;
  ImmSet : TExpression;
  ExprSet : TExpression;
  NewBounds : TExSetExprBounds;
begin
  ElementTypePtr := SetExpr^.TypePtr^.ElementTypePtr;
  if ElementTypePtr = nil then
  begin
    ElementTypePtr := GetFundamentalType(First^.TypePtr);
    SetExpr^.Immediate.SetOfTypePtr := ElementTypePtr;
    SetExpr^.TypePtr^.ElementTypePtr := ElementTypePtr
  end;
  if not IsSameType(GetFundamentalType(First^.TypePtr), ElementTypePtr) then
    ErrorForExpr('Cannot add element to set ' +
                         ErrorDescribeExpr(SetExpr), First);
  if (Last <> nil)
     and not IsSameType(GetFundamentalType(Last^.TypePtr), ElementTypePtr)
     and not IsSameType(First^.TypePtr, Last^.TypePtr) then
    ErrorForExpr('Cannot add element to set ' +
                         ErrorDescribeExpr(SetExpr), Last);

  if ExIsImmediate(SetExpr) then
  begin
    ImmSet := SetExpr;
    ExprSet := nil
  end
  else
  begin
    ImmSet := SetExpr^.SetBase;
    ExprSet := SetExpr
  end;
  if ExIsImmediate(First) and (Last = nil) then Last := ExCopy(First);
  if ExIsImmediate(First) and ExIsImmediate(Last) then
  begin
    with ImmSet^.Immediate do
      SetBounds := ExSetAddBounds(SetBounds,
                   ExGetOrdinal(First), ExGetOrdinal(Last));
    ExDispose(First);
    ExDispose(Last)
  end
  else
  begin
    if ExprSet = nil then
    begin
      ExprSet := _NewExpr(XcSet);
      ExprSet^.SetBase := ImmSet;
      ExprSet^.SetBounds := nil;
      ExprSet^.TypePtr := ImmSet^.TypePtr;
    end;
    new(NewBounds);
    NewBounds^.First := First;
    NewBounds^.Last := Last;
    NewBounds^.Next := ExprSet^.SetBounds;
    ExprSet^.SetBounds := NewBounds;
  end;
  if ExprSet <> nil then Result := ExprSet
  else Result := ImmSet
end;

function ExToString(Parent : TExpression) : TExpression;
var Str : string;
begin
  EnsureStringyExpr(Parent);
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
  else  Result := Parent
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

function ExWithTmpVar(TmpVar, Value, Child : TExpression) : TExpression;
begin
  Result := _NewExpr(XcWithTmpVar);
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
    TmpExpr := ExCopy(Expr^.SubrangeParent);
    ExDispose(Expr);
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
  EnsureRecordExpr(Parent);
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
  EnsureArrayExpr(Parent);
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
  EnsurePointerExpr(Parent);
  Result := _NewExpr(XcPointer);
  Result^.PointerExpr := Parent;
  Result^.TypePtr := Parent^.TypePtr^.PointedTypePtr;
  Result^.IsAssignable := true;
  Result^.IsFunctionResult := Parent^.IsFunctionResult
end;

function ExStringChar(Parent, Subscript : TExpression) : TExpression;
begin
  EnsureStringyExpr(Parent);
  EnsureIntegerExpr(Subscript);
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
    ErrorForExpr('Cannot call non-function', FnExpr);
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
          Result := ExWithTmpVar(ExVariable(AddTmpVariable(
                    'tmp', FnExpr^.FnPtr^.Args[Pos].TypePtr)),
                    FnCall^.CallArgs.Values[Pos], Result);
          FnCall^.CallArgs.Values[Pos] := ExCopy(Result^.TmpVar)
        end
        else
          CompileError('Pass-by-reference argument must be assignable: ' +
                       ExDescribe(FnCall^.CallArgs.Values[Pos]))
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
    InternalError('Expected a pseudofunction, got ' + ExDescribe(Expr));
  Fn := Expr^.PseudoFn;
  Expr^.Cls := XcPseudoFnCall;
  Expr^.PseudoFnCall.PseudoFn := Fn;
  Expr^.PseudoFnCall.Arg1 := nil;
  Expr^.PseudoFnCall.Arg2 := nil;
  Expr^.PseudoFnCall.Arg3 := nil;
  Expr^.PseudoFnCall.Arg4 := nil;
  Expr^.PseudoFnCall.ReadArgs := nil;
  Expr^.PseudoFnCall.WriteArgs := nil;
  Result := Expr
end;

procedure _ExSetCoerceToCommon(Left, Right : TExpression);
var LeftType, RightType : TPsTypePtr;
begin
  LeftType := Left^.TypePtr;
  RightType := Right^.TypePtr;
  if (LeftType^.ElementTypePtr = nil) or ExIsImmediate(Left) then
    LeftType^.ElementTypePtr := RightType^.ElementTypePtr
  else if (RightType^.ElementTypePtr = nil) or ExIsImmediate(Right) then
         RightType^.ElementTypePtr := LeftType^.ElementTypePtr
  else if not IsSameType(LeftType, RightType) then
         CompileError('Type mismatch: cannot combine ' +
                      TypeName(Left^.TypePtr) + ' with ' +
         TypeName(Right^.TypePtr))
end;

function _ExSetUnion(Left, Right : TExpression) : TExpression;
var NewBds, OldBds : TExSetImmBounds;
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
  ExDispose(Left);
  ExDispose(Right)
end;

function _ExSetDifference(Left, Right : TExpression) : TExpression;
var 
  LtBds, RtBds, NewBds : TExSetImmBounds;
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
    else if (RtBds^.First < LtBds^.Last) and (RtBds^.Last >= LtBds^.Last) then
    begin
      NewBds := ExSetAddBounds(NewBds, LtBds^.First, RtBds^.First - 1);
      LtBds := LtBds^.Next
    end
    { lllll      or  lllll       }
    {      rrrr            rrrrr }
    { Add left, go to next left }
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
end;

function _ExSetIntersection(Left, Right : TExpression) : TExpression;
var 
  LtBds, RtBds, NewBds : TExSetImmBounds;
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
    else if (RtBds^.First < LtBds^.Last) and (RtBds^.Last >= LtBds^.Last) then
    begin
      NewBds := ExSetAddBounds(NewBds, RtBds^.First, LtBds^.Last);
      LtBds := LtBds^.Next
    end
    { lllll      or  lllll       }
    {      rrrr            rrrrr }
    { Go to next left }
    else LtBds := LtBds^.Next
  end;
  Result := ExSetConstant(NewBds, Left^.TypePtr);
  ExDispose(Left);
  ExDispose(Right)
end;

function _ExSetEquals(Left, Right : TExpression;
                      Negate : boolean) : TExpression;
var 
  LtBds, RtBds : TExSetImmBounds;
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
  ExDispose(Left);
  ExDispose(Right)
end;

function _ExSetSubset(Left, Right : TExpression) : TExpression;
var 
  LtBds, RtBds : TExSetImmBounds;
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
  ExDispose(Left);
  ExDispose(Right)
end;

function _ExSetIn(Needle, Haystack : TExpression) : TExpression;
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
        Cond := ExBinaryOp(ExCopy(Wanted),
                ExGetAntiOrdinal(ImmBounds^.First, ElemType),
                TkEquals)
      else
        Cond := ExBinaryOp(
                ExBinaryOp(ExGetAntiOrdinal(ImmBounds^.First, ElemType),
                ExCopy(Wanted),
                TkLessOrEquals),
                ExBinaryOp(ExCopy(Wanted),
                ExGetAntiOrdinal(ImmBounds^.Last, ElemType),
                TkLessOrEquals),
                TkAnd);
      Result := ExBinaryOp(Result, Cond, TkOr);
      ImmBounds := ImmBounds^.Next
    end
  end;
  if ExprSet <> nil then
  begin
    ExprBounds := ExprSet^.SetBounds;
    while ExprBounds <> nil do
    begin
      if ExprBounds^.Last = nil then
        Cond := ExBinaryOp(ExCopy(Wanted), ExCopy(ExprBounds^.First),
                TkEquals)
      else
        Cond := ExBinaryOp(
                ExBinaryOp(ExCopy(ExprBounds^.First),
                ExCopy(Wanted),
                TkLessOrEquals),
                ExBinaryOp(ExCopy(Wanted),
                ExCopy(ExprBounds^.Last),
                TkLessOrEquals),
                TkAnd);
      Result := ExBinaryOp(Result, Cond, TkOr);
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

function _ExUnOpImm(Parent : TExpression; Op : TLxTokenId) : TExpression;
forward;
function _ExUnOpCmp(Parent : TExpression; Op : TLxTokenId) : TExpression;
forward;
function ExUnaryOp(Parent : TExpression; Op : TLxTokenId) : TExpression;
begin
  if Op in [TkMinus, TkPlus] then
  begin
    if not IsNumericType(Parent^.TypePtr) then ErrorInvalidOperator(Parent, Op)
  end
  else if Op = TkNot then
  begin
    if not IsBooleanType(Parent^.TypePtr)
       and not IsIntegerType(Parent^.TypePtr) then
      ErrorInvalidOperator(Parent, Op)
  end
  else ErrorInvalidOperator(Parent, Op);

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
    if ExIsImmediate(Right)
       and (ExIsImmediate(Left) or not IsSetType(Left^.TypePtr)) then
      Result := _ExBinOpSetImm(Left, Right, Op)
    else if (Op = TkIn) and (Right^.Cls = XcSet) then
           Result := _ExBinOpSetImm(Left, Right, Op)
    else Result := _ExBinOpSetCmp(Left, Right, Op)
  end
  else ErrorInvalidOperator2(Left, Right, Op)
end;

function _ExBinOpBoolImm;
var Lt, Rt : boolean;
begin
  Lt := Left^.Immediate.BooleanVal;
  Rt := Right^.Immediate.BooleanVal;
  case Op of 
    TkAnd : Lt := Lt and Rt;
    TkOr : Lt := Lt or Rt;
    TkXor : Lt := Lt xor Rt;
    TkEquals : Lt := Lt = Rt;
    TkNotEquals : Lt := Lt <> Rt;
    TkLessthan : Lt := Lt < Rt;
    TkMorethan : Lt := Lt > Rt;
    TkLessOrEquals : Lt := Lt <= Rt;
    TkMoreOrEquals : Lt := Lt >= Rt;
    else ErrorInvalidOperator2(Left, Right, Op)
  end;
  Left^.Immediate.BooleanVal := Lt;
  Left^.TypePtr := PrimitiveTypes.PtBoolean;
  Left^.IsAssignable := false;
  ExDispose(Right);
  Result := Left
end;

function _ExBinOpIntImm;
var 
  Lt, Rt : integer;
  Bo : boolean;
begin
  Lt := Left^.Immediate.IntegerVal;
  Rt := Right^.Immediate.IntegerVal;
  case Op of 
    TkPlus : Lt := Lt + Rt;
    TkMinus : Lt := Lt - Rt;
    TkAsterisk : Lt := Lt * Rt;
    TkDiv : Lt := Lt div Rt;
    TkMod : Lt := Lt mod Rt;
    TkAnd : Lt := Lt and Rt;
    TkOr : Lt := Lt or Rt;
    TkXor : Lt := Lt xor Rt;
    TkShl : Lt := Lt shl Rt;
    TkShr : Lt := Lt shr Rt;
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
        else ErrorInvalidOperator2(Left, Right, Op)
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
  ExDispose(Right);
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
        else ErrorInvalidOperator2(Left, Right, Op)
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
  ExDispose(Right);
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
      else ErrorInvalidOperator2(Left, Right, Op)
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
  ExDispose(Right);
  Result := Left
end;

function _ExBinOpEnumImm;
var 
  Lt, Rt : integer;
  Bo : boolean;
begin
  Lt := Left^.Immediate.EnumOrdinal;
  Rt := Right^.Immediate.EnumOrdinal;
  case Op of 
    TkEquals : Bo := Lt = Rt;
    TkNotEquals : Bo := Lt <> Rt;
    TkLessthan : Bo := Lt < Rt;
    TkMorethan : Bo := Lt > Rt;
    TkLessOrEquals : Bo := Lt <= Rt;
    TkMoreOrEquals : Bo := Lt >= Rt;
    else ErrorInvalidOperator2(Left, Right, Op)
  end;
  Left^.Immediate.Cls := XicBoolean;
  Left^.Immediate.BooleanVal := Bo;
  Left^.TypePtr := PrimitiveTypes.PtBoolean;
  Left^.IsAssignable := false;
  ExDispose(Right);
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
      else ErrorInvalidOperator2(Left, Right, Op)
    end
  else if Op = TkIn then Result := _ExSetIn(Left, Right)
  else ErrorInvalidOperator2(Left, Right, Op)
end;

function _ExBinOpBoolCmp;
begin
  if Op in [TkAnd, tkOr, TkXor, TkEquals, TkNotEquals, TkLessthan, TkMorethan,
     TkLessOrEquals, TkMoreOrEquals] then
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
  else ErrorInvalidOperator2(Left, Right, Op)
end;

function _ExBinOpIntCmp;
begin
  Result := _NewExpr(XcBinaryOp);
  Result^.Binary.Left := Left;
  Result^.Binary.Right := Right;
  Result^.Binary.Op := Op;
  Result^.IsAssignable := false;
  Result^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
  if Op in [TkPlus, TkMinus, TkAsterisk, TkDiv, TkMod, TkAnd, TkOr, TkXor,
     TkShl, TkShr] then
    Result^.TypePtr := PrimitiveTypes.PtInteger
  else if Op in [TkEquals, TkNotEquals, TkLessthan, TkMorethan, TkLessOrEquals,
          TkMoreOrEquals] then
         Result^.TypePtr := PrimitiveTypes.PtBoolean
  else ErrorInvalidOperator2(Left, Right, Op)
end;

function _ExBinOpNumCmp;
begin
  Result := _NewExpr(XcBinaryOp);
  Result^.Binary.Left := ExCoerce(Left, PrimitiveTypes.PtReal);
  Result^.Binary.Right := ExCoerce(Right, PrimitiveTypes.PtReal);
  Result^.Binary.Op := Op;
  Result^.IsAssignable := false;
  Result^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
  if Op in [TkPlus, TkMinus, TkAsterisk, TkSlash] then
    Result^.TypePtr := PrimitiveTypes.PtReal
  else if Op in [TkEquals, TkNotEquals, TkLessthan, TkMorethan, TkLessOrEquals,
          TkMoreOrEquals] then
         Result^.TypePtr := PrimitiveTypes.PtBoolean
  else ErrorInvalidOperator2(Left, Right, Op)
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
  else if Op in [TkEquals, TkNotEquals, TkLessthan, TkMorethan, TkLessOrEquals,
          TkMoreOrEquals] then
         Result^.TypePtr := PrimitiveTypes.PtBoolean
  else ErrorInvalidOperator2(Left, Right, Op)
end;

function _ExBinOpEnumCmp;
begin
  Result := _NewExpr(XcBinaryOp);
  Result^.Binary.Left := Left;
  Result^.Binary.Right := Right;
  Result^.Binary.Op := Op;
  Result^.IsAssignable := false;
  Result^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
  if Op in [TkEquals, TkNotEquals, TkLessthan, TkMorethan, TkLessOrEquals,
     TkMoreOrEquals] then
    Result^.TypePtr := PrimitiveTypes.PtBoolean
  else ErrorInvalidOperator2(Left, Right, Op)
end;

function _ExBinOpPtrCmp;
begin
  Result := _NewExpr(XcBinaryOp);
  Result^.Binary.Left := Left;
  Result^.Binary.Right := Right;
  Result^.Binary.Op := Op;
  Result^.IsAssignable := false;
  Result^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
  if Op in [TkEquals, TkNotEquals] then
    Result^.TypePtr := PrimitiveTypes.PtBoolean
  else ErrorInvalidOperator2(Left, Right, Op)
end;

function _ExBinOpSetCmp;
begin
  Result := _NewExpr(XcBinaryOp);
  if Op = TkIn then
  begin
    Left := ExCoerce(Left, Right^.TypePtr^.ElementTypePtr);
    Result^.TypePtr := PrimitiveTypes.PtBoolean
  end
  else if Op in [TkEquals, TkNotEquals, TkLessOrEquals, TkMoreOrEquals] then
  begin
    _ExSetCoerceToCommon(Left, Right);
    Result^.TypePtr := PrimitiveTypes.PtBoolean
  end
  else
  begin
    _ExSetCoerceToCommon(Left, Right);
    Result^.TypePtr := Left^.TypePtr
  end;
  Result^.Binary.Left := Left;
  Result^.Binary.Right := Right;
  Result^.Binary.Op := Op;
  Result^.IsAssignable := false;
  Result^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
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
    { X shl 0 -> X }
    TkShl : if _ExIsZero(Right) then Use := UseLeft;
    { X shr 0 -> X }
    TkShr : if _ExIsZero(Right) then Use := UseLeft;
  end;
  case Use of 
    UseLeft :
              begin
                ExDispose(Right);
                Result := true
              end;
    UseRight :
               begin
                 ExDispose(Left);
                 Left := Right;
                 Result := true
               end;
    Keep : Result := false
  end
end;

function ExGetOrdinal(Expr : TExpression) : integer;
begin
  if not ExIsImmediate(Expr) then
    ErrorForExpr('Expected an immediate value', Expr);
  with Expr^.Immediate do
    case Cls of 
      XicBoolean : Result := Ord(BooleanVal);
      XicInteger : Result := IntegerVal;
      XicChar : Result := Ord(CharVal);
      XicEnum : Result := EnumOrdinal;
      else ErrorForExpr('Expected an ordinal', Expr)
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
      CompileError('Value ' + ExDescribe(Parent) +
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
  ExprElemType := Expr^.TypePtr^.ElementTypePtr;
  DestElemType := TypePtr^.ElementTypePtr;
  if ExprElemType = nil then Outcome := Replace
  else if not IsSameType(GetFundamentalType(ExprElemType),
          GetFundamentalType(DestElemType)) then Outcome := Reject
  else if ExIsImmediate(Expr) then Outcome := Replace
  else if Expr^.Cls = XcSet then Outcome := Replace
  else if (GetTypeLowBound(ExprElemType) = GetTypeLowBound(DestElemType))
          and (GetTypeHighBound(ExprElemType) = GetTypeHighBound(DestElemType))
         then Outcome := Pass
  else Outcome := Reject;

  case Outcome of 
    Reject : ErrorForExpr('Cannot treat set as ' + TypeName(TypePtr),
             Expr);
    Replace :
              begin
                Expr^.TypePtr := TypePtr;
                if Expr^.Cls = XcSet then Expr^.SetBase^.TypePtr := TypePtr
              end;
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
    ErrorForExpr('Cannot treat value as ' + TypeName(TypePtr), Expr)
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

function _NewExpr(Cls : TExpressionClass) : TExpression;
begin
  new(Result);
  Result^.Cls := Cls;
  Result^.IsAssignable := false;
  Result^.IsAddressable := false;
  Result^.IsFunctionResult := false
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
  ReadArg := Call.ReadArgs;
  while ReadArg <> nil do
  begin
    NextReadArg := ReadArg^.Next;
    ExDispose(ReadArg^.Arg);
    dispose(ReadArg);
    ReadArg := NextReadArg
  end;
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
    XcImmediate: _DisposeImmediate(Expr^.Immediate);
    XcToString: ExDispose(Expr^.ToStrParent);
    XcToReal: ExDispose(Expr^.ToRealParent);
    XcToRawPtr: ExDispose(Expr^.ToRawPtrParent);
    XcWithTmpVar:
                  begin
                    ExDispose(Expr^.TmpVar);
                    ExDispose(Expr^.TmpVarValue);
                    ExDispose(Expr^.TmpVarChild);
                  end;
    XcSubrange: ExDispose(Expr^.SubrangeParent);
    XcSet:
           begin
             ExDispose(Expr^.SetBase);
             _DisposeBounds(Expr^.SetBounds);
           end;
    XcField: ExDispose(Expr^.RecExpr);
    XcArray:
             begin
               ExDispose(Expr^.ArrayExpr);
               ExDispose(Expr^.ArrayIndex);
             end;
    XcPointer: ExDispose(Expr^.PointerExpr);
    XcAddress: ExDispose(Expr^.AddressExpr);
    XcStringChar:
                  begin
                    ExDispose(Expr^.StringExpr);
                    ExDispose(Expr^.StringIndex);
                  end;
    XcFnCall:
              begin
                ExDispose(Expr^.FnExpr);
                for Pos := 1 to Expr^.CallArgs.Size do
                  ExDispose(Expr^.CallArgs.Values[Pos]);
              end;
    XcPseudoFnCall: _DisposePseudoCallExpr(Expr^.PseudoFnCall);
    XcUnaryOp: ExDispose(Expr^.Unary.Parent);
    XcBinaryOp:
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
  Copy.PseudoFnPtr := Call.PseudoFnPtr;
  if Call.Arg1 <> nil then Copy.Arg1 := ExCopy(Call.Arg1);
  if Call.Arg2 <> nil then Copy.Arg2 := ExCopy(Call.Arg2);
  if Call.Arg3 <> nil then Copy.Arg3 := ExCopy(Call.Arg3);
  if Call.Arg4 <> nil then Copy.Arg4 := ExCopy(Call.Arg4);
  Copy.TypeArg := Call.TypeArg;
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
  end;
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
  Copy^.IsAddressable := Expr^.IsAddressable;
  Copy^.IsFunctionResult := Expr^.IsFunctionResult;
  case Expr^.Cls of 
    XcImmediate: Copy^.Immediate := _CopyImmediate(Expr^.Immediate);
    XcToString: Copy^.ToStrParent := ExCopy(Expr^.ToStrParent);
    XcToReal: Copy^.ToRealParent := ExCopy(Expr^.ToRealParent);
    XcToRawPtr: Copy^.ToRawPtrParent := ExCopy(Expr^.ToRawPtrParent);
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
    XcAddress: Copy^.AddressExpr := ExCopy(Expr^.AddressExpr);
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
    XcPseudoFnRef: Copy^.PseudoFnPtr := Expr^.PseudoFnPtr;
    XcPseudoFnCall: _CopyPseudoCallExpr(Expr^.PseudoFnCall,
                                        Copy^.PseudoFnCall);
    XcSizeof: Copy^.SizeofTypePtr := Expr^.SizeofTypePtr;
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

const _ExPrecedences : array[TExpressionClass] of integer 
                       = ( 0, -1, -1, -1, -1, 0, 0, 1, 1, 1, 1, 1, 0, 1,
                          0, 1, 1, -1, -1);
const _ExOpPrecedences : array[TExOperator] of integer 
                         = (4, 4, 4, 3, 3, 3, 4,
                            3, 4, 4, 3, 3,
                            5, 5, 5, 5, 5, 5,
                            1, 1, 1);
const _ExOpNames : array[TExOperator] of string 
                   = ('+', '-', '*', '/', 'DIV', 'MOD', '-',
                      'AND', 'OR', 'XOR', 'NOT', 'SHL', 'SHR',
                      'IN',
                      '=', '<>', '<', '>', '<=', '>=',
                      'ORD', 'PRED', 'SUCC');

function _ExprPrecedence(Expr : TExpression) : integer;
begin
  Result := _ExPrecedences[Expr^.Cls];
  if Result < 0 then
  begin
    case Expr^.Cls of 
      XcToString: Result := _ExprPrecedence(Expr^.ToStrParent);
      XcToReal: Result := _ExprPrecedence(Expr^.ToRealParent);
      XcToRawPtr: Result := _ExprPrecedence(Expr^.ToRawPtrParent);
      XcWithTmpVar: Result := _ExprPrecedence(Expr^.TmpVarChild);
      XcSubrange: Result := _ExprPrecedence(Expr^.SubrangeParent);
      XcUnaryOp: Result := _ExOpPrecedences[Expr^.Unary.Op];
      XcBinaryOp: Result := _ExOpPrecedences[Expr^.Binary.Op];
    end
  end;
  if Result < 0 then
    CompileError('Unknown precedence for expression')
end;

function ExDescribeOperator(Op : TExOperator) : string;
begin
  Result := _ExOpNames[Op]
end;

function _DescribeUnaryOpExpr(Expr : TExpression) : string;
var UseParens : boolean;
begin
  if Expr^.Unary.Op in [XoOrd, XoPred, XoSucc] then
  begin
    Result := ExDescribeOperator(Expr^.Unary.Op) + '(' +
              ExDescribe(Expr^.Unary.Parent) + ')'
  end
  else
  begin
    Result := ExDescribeOperator(Expr^.Unary.Op);
    if Expr^.Unary.Op <> XoNeg then Result := Result + ' ';
    UseParens := _ExprPrecedence(Expr) < _ExprPrecedence(Expr^.Unary.Parent);
    if UseParens then Result := Result + '(';
    Result := Result + ExDescribe(Expr^.Unary.Parent);
    if UseParens then Result := Result + ')'
  end
end;

function _DescribeBinaryOpExpr(Expr : TExpression) : string;
var UseParens : boolean;
begin
  UseParens := _ExprPrecedence(Expr) < _ExprPrecedence(Expr^.Binary.Left);
  if UseParens then Result := '('
  else Result := '';
  Result := Result + ExDescribe(Expr^.Binary.Left);
  if UseParens then Result := Result + ')';
  Result := Result + ' ' + ExDescribeOperator(Expr^.Binary.Op) + ' ';
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
    XcToRawPtr: Result := ExDescribe(Expr^.ToRawPtrParent);
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
    XcAddress: Result := '@' + ExDescribe(Expr^.AddressExpr);
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
    XcPseudoFnRef: Result := Expr^.PseudoFnPtr^.Name;
    XcPseudoFnCall: Result := Expr^.PseudoFnPtr^.DescribeFn(Expr);
    XcSizeof: Result := 'SIZEOF(' + TypeName(Expr^.SizeofTypePtr) + ')';
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
begin
  Result := _ExImmediate(XicSet);
  Result^.Immediate.SetBounds := nil;
  Result^.Immediate.SetOfTypePtr := nil;
  Result^.TypePtr := PrimitiveTypes.PtEmptySet
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
    SetExpr^.TypePtr := MakeSetType(First^.TypePtr)
  end;
  if not IsFundamentallySameType(First^.TypePtr, ElementTypePtr) then
    ErrorForExpr('Cannot add element to set ' +
                 ErrorDescribeExpr(SetExpr), First);
  if (Last <> nil)
     and not IsFundamentallySameType(Last^.TypePtr, ElementTypePtr)
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
    Result^.IsFunctionResult := Parent^.IsFunctionResult
  end;
end;

function ExToRawPtr(Parent : TExpression) : TExpression;
begin
  Result := _NewExpr(XcToRawPtr);
  Result^.ToRawPtrParent := Parent;
  Result^.TypePtr := PrimitiveTypes.PtRawPtr;
  Result^.IsFunctionResult := Parent^.IsFunctionResult;
  Result^.IsAssignable := Parent^.IsAssignable;
  Result^.IsAddressable := Parent^.IsAddressable
end;

function ExWithTmpVar(TmpVar, Value, Child : TExpression) : TExpression;
begin
  Result := _NewExpr(XcWithTmpVar);
  Result^.TmpVar := TmpVar;
  Result^.TmpVarValue := Value;
  Result^.TmpVarChild := Child;
  Result^.TypePtr := Child^.TypePtr;
  Result^.IsAssignable := Child^.IsAssignable;
  Result^.IsAddressable := Child^.IsAddressable;
  Result^.IsFunctionResult := Child^.IsFunctionResult;
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
  Result^.IsAssignable := not VarPtr^.IsConstant;
  Result^.IsAddressable := true
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
  Result^.IsAddressable := Parent^.IsAddressable;
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
  Result^.IsAddressable := Parent^.IsAddressable;
  Result^.IsFunctionResult := Parent^.IsFunctionResult
end;

function ExPointerAccess(Parent : TExpression) : TExpression;
begin
  EnsurePointerExpr(Parent);
  Result := _NewExpr(XcPointer);
  Result^.PointerExpr := Parent;
  Result^.TypePtr := Parent^.TypePtr^.PointedTypePtr;
  Result^.IsAssignable := true;
  Result^.IsAddressable := true;
  Result^.IsFunctionResult := Parent^.IsFunctionResult
end;

function ExAddressOf(Parent : TExpression) : TExpression;
begin
  Result := _NewExpr(XcAddress);
  Result^.AddressExpr := Parent;
  if Parent^.Cls = XcFnRef then
    Result^.TypePtr := MakeFunctionType(Parent^.FnPtr^.Args,
                       Parent^.FnPtr^.ReturnTypePtr)
  else
  begin
    EnsureAddressableExpr(Parent);
    EnsureAssignableExpr(Parent);
    if Parent^.Cls <> XcVariable then
      ErrorForExpr('Expected a variable', Parent);
    Result^.TypePtr := MakePointerType(Parent^.TypePtr)
  end
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
  Result^.IsAddressable := Parent^.IsAddressable;
  Result^.IsFunctionResult := Parent^.IsFunctionResult
end;

function ExFnRef(FnPtr : TPsFnPtr) : TExpression;
begin
  Result := _NewExpr(XcFnRef);
  Result^.FnPtr := FnPtr;
  Result^.TypePtr := nil
end;

function _ExFunctionCall(FnExpr : TExpression;
                         const ArgDefs : TPsFnArgs;
                         ReturnTypePtr : TPsTypePtr;
                         const Args : TExFunctionArgs) : TExpression;
var 
  Pos : integer;
  FnCall : TExpression;
begin
  if Args.Size <> ArgDefs.Count then
    CompileError('Wrong number of arguments in call to ' + ExDescribe(FnExpr));
  FnCall := _NewExpr(XcFnCall);
  FnCall^.FnExpr := FnExpr;
  FnCall^.CallArgs.Size := Args.Size;
  FnCall^.TypePtr := ReturnTypePtr;
  FnCall^.IsFunctionResult := true;
  Result := FnCall;
  for Pos := 1 to Args.Size do
  begin
    FnCall^.CallArgs.Values[Pos] := ExCoerce(Args.Values[Pos],
                                    ArgDefs.Defs[Pos].TypePtr);
    if ArgDefs.Defs[Pos].IsReference then
    begin
      if not FnCall^.CallArgs.Values[Pos]^.IsAddressable then
      begin
        if ArgDefs.Defs[Pos].IsConstant then
        begin
          Result := ExWithTmpVar(ExVariable(AddTmpVariable(
                    'tmp', ArgDefs.Defs[Pos].TypePtr)),
                    FnCall^.CallArgs.Values[Pos], Result);
          FnCall^.CallArgs.Values[Pos] := ExCopy(Result^.TmpVar)
        end
        else
          CompileError('Pass-by-reference argument must be assignable: ' +
                       ExDescribe(FnCall^.CallArgs.Values[Pos]))
      end
      else if not ArgDefs.Defs[Pos].IsConstant then
      begin
        EnsureAssignableExpr(FnCall^.CallArgs.Values[Pos]);
        ExMarkInitialized(FnCall^.CallArgs.Values[Pos])
      end
    end
  end
end;

function ExFunctionCall(FnExpr : TExpression;
                        const Args : TExFunctionArgs) : TExpression;
begin
  if FnExpr^.Cls = XcFnRef then
  begin
    FnExpr^.FnPtr^.WasUsed := true;
    Result := _ExFunctionCall(FnExpr, FnExpr^.FnPtr^.Args,
              FnExpr^.FnPtr^.ReturnTypePtr, Args)
  end
  else if IsFunctionType(FnExpr^.TypePtr) then
         Result := _ExFunctionCall(FnExpr, FnExpr^.TypePtr^.FnDefPtr^.Args,
                   FnExpr^.TypePtr^.FnDefPtr^.ReturnTypePtr, Args)
  else
    ErrorForExpr('Cannot call non-function', FnExpr)
end;

function ExPseudoFn(SpecialFn : TPsPseudoFnPtr) : TExpression;
begin
  Result := _NewExpr(XcPseudoFnRef);
  Result^.PseudoFnPtr := SpecialFn
end;

function ExPseudoFnCall(Expr : TExpression) : TExpression;
var Fn : TPsPseudoFnPtr;
begin
  if Expr^.Cls <> XcPseudoFnRef then
    InternalError('Expected a pseudofunction, got ' + ExDescribe(Expr));
  Fn := Expr^.PseudoFnPtr;
  Expr^.Cls := XcPseudoFnCall;
  Expr^.PseudoFnCall.PseudoFnPtr := Fn;
  Expr^.PseudoFnCall.Arg1 := nil;
  Expr^.PseudoFnCall.Arg2 := nil;
  Expr^.PseudoFnCall.Arg3 := nil;
  Expr^.PseudoFnCall.Arg4 := nil;
  Expr^.PseudoFnCall.TypeArg := nil;
  Expr^.PseudoFnCall.ReadArgs := nil;
  Expr^.PseudoFnCall.WriteArgs := nil;
  Result := Expr
end;

function ExSizeof(TypePtr : TPsTypePtr) : TExpression;
begin
  Result := _NewExpr(XcSizeof);
  Result^.SizeofTypePtr := TypePtr;
  Result^.TypePtr := PrimitiveTypes.PtInteger
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

procedure ExSetCoerceToCommon(Left, Right : TExpression);
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

function _ExCoerceSet(Expr : TExpression; TypePtr : TPsTypePtr) : TExpression;
var 
  Outcome : (Pass, Reject, Replace);
  ExprElemType, DestElemType : TPsTypePtr;
begin
  ExprElemType := Expr^.TypePtr^.ElementTypePtr;
  DestElemType := TypePtr^.ElementTypePtr;
  if ExprElemType = nil then Outcome := Replace
  else if not IsFundamentallySameType(ExprElemType, DestElemType) then
         Outcome := Reject
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
  if IsFundamentallySameType(Expr^.TypePtr, TypePtr) then
  begin
    if IsRangeType(Expr^.TypePtr) and IsRangeType(TypePtr) then
      ExCoerce := ExRerange(Expr, TypePtr)
    else if IsRangeType(Expr^.TypePtr) then
           ExCoerce := ExOutrange(Expr)
    else if IsRangeType(TypePtr) then
           ExCoerce := ExSubrange(Expr, TypePtr)
    else
      ExCoerce := Expr
  end
  else if IsCharType(Expr^.TypePtr) and IsStringType(TypePtr) then
         ExCoerce := ExToString(Expr)
  else if IsIntegerType(Expr^.TypePtr) and IsRealType(TypePtr) then
         ExCoerce := ExToReal(Expr)
  else if IsNilType(Expr^.TypePtr) and IsPointeryType(TypePtr) then
         ExCoerce := Expr
  else if IsNilType(Expr^.TypePtr) and IsFunctionyType(TypePtr) then
         ExCoerce := Expr
  else if IsPointeryType(Expr^.TypePtr) and IsRawPtrType(TypePtr) then
         ExCoerce := ExToRawPtr(Expr)
  else if IsSetType(Expr^.TypePtr) and IsSetType(TypePtr) then
         ExCoerce := _ExCoerceSet(Expr, TypePtr)
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

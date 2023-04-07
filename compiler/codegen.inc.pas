type 
  TOutputType = (TotNone, TotType, TotVar, TotEnumVal, TotFunDec, TotFunDef);

var 
  Codegen : record
    Output : text;
    IsMultiStatement : boolean;
    Indent : integer;
    Newline : boolean;
    LastOut : TOutputType;
  end;

procedure OutVariableDeclaration(VarDef : TPsVariable);
forward;
procedure OutTypeReference(TypePtr : TPsTypePtr);
forward;

procedure _OutComma;
begin
  write(Codegen.Output, ', ')
end;

procedure _OutNewline;
begin
  writeln(Codegen.Output);
  Codegen.Newline := true
end;

procedure _OutBlankline(NewOut : TOutputType);
begin
  if (Codegen.Indent = 0) and
     ((Codegen.LastOut <> NewOut) or (NewOut = TotFunDef)) then _OutNewline;
  Codegen.LastOut := NewOut
end;

procedure _OutIndent;
var Ct : integer;
begin
  if Codegen.Newline then
    for Ct := 1 to Codegen.Indent do
      write(Codegen.Output, '  ');
  Codegen.Newline := false
end;

procedure OutBegin;
begin
  Codegen.IsMultiStatement := true;
  write(Codegen.Output, '{');
  _OutNewline;
  Codegen.Indent := Codegen.Indent + 1
end;

procedure OutEnd;
begin
  Codegen.Indent := Codegen.Indent - 1;
  _OutIndent;
  write(Codegen.Output, '}');
  _OutNewline
end;

procedure OutEndSameLine;
begin
  Codegen.Indent := Codegen.Indent - 1;
  _OutIndent;
  write(Codegen.Output, '}')
end;

procedure _OutEscapedChar(Ch : char);
var Code, N1, N2 : integer;
begin
  Code := Ord(Ch);
  N1 := Code div 16;
  N2 := Code mod 16;
  write(Codegen.Output, '\x');
  if N1 < 10 then write(Codegen.Output, Chr(N1 + 48))
  else write(Codegen.Output, Chr(N1 + 87));
  if N2 < 10 then write(Codegen.Output, Chr(N2 + 48))
  else write(Codegen.Output, Chr(N2 + 87))
end;


procedure _OutChar(Chr : char);
begin
  write(Codegen.Output, '''');
  if Chr = '''' then write(Codegen.Output, '\''')
  else if Chr = '\' then write(Codegen.Output, '\\')
  else if Chr in [#32..#126] then write(Codegen.Output, Chr)
  else _OutEscapedChar(Chr);
  write(Codegen.Output, '''')
end;

procedure _OutCstring(const Str : string);
var 
  Pos : integer;
  Ch : char;
begin
  write(Codegen.Output, '"');
  for Pos := 1 to Length(Str) do
  begin
    Ch := Str[Pos];
    if (Ch < ' ') or (Ch > #126) then
    begin
      _OutEscapedChar(Ch);
      if (Pos < Length(Str)) and (Str[Pos + 1] in LxCharsHex) then
        write(Codegen.Output, '""')
    end
    else
    begin
      if Ch = '"' then write(Codegen.Output, '\"')
      else if Ch = '\' then write(Codegen.Output, '\\')
      else write(Codegen.Output, Ch)
    end
  end;
  write(Codegen.Output, '"')
end;

procedure _OutString(const Str : string);
begin
  if Length(Str) = 1 then
  begin
    write(Codegen.Output, 'str_of(');
    _OutChar(Str[1]);
    write(Codegen.Output, ')')
  end
  else
  begin
    write(Codegen.Output, 'str_make(', Length(Str));
    _OutComma;
    _OutCstring(Str);
    write(Codegen.Output, ')')
  end
end;

function _BinOpPrec(Expr : TExpression) : integer;
var IsSetLeft, IsSetRight : boolean;
begin
  IsSetLeft := IsSetType(Expr^.Binary.Left^.TypePtr);
  IsSetRight := IsSetType(Expr^.Binary.Right^.TypePtr);
  case Expr^.Binary.Op of 
    XoAdd: if IsStringyType(Expr^.TypePtr) then Result := 1
           else Result := 4;
    XoSub: Result := 4;
    XoMul: Result := 3;
    XoDivReal: Result := 3;
    XoDivInt: Result := 3;
    XoMod: Result := 3;
    XoAnd: if IsBooleanType(Expr^.TypePtr) then Result := 11
           else Result := 8;
    XoOr: if IsBooleanType(Expr^.TypePtr) then Result := 12
          else Result := 10;
    XoXor: if IsBooleanType(Expr^.TypePtr) then Result := 7
           else Result := 9;
    XoShl: Result := 5;
    XoShr: Result := 5;
    XoIn: if ExIsImmediate(Expr^.Binary.Right) then Result := 12
          else Result := 1;
    XoEq: if IsSetLeft and IsSetRight then Result := 1
          else Result := 7;
    XoNe: if IsSetLeft and IsSetRight then Result := 1
          else Result := 7;
    XoLt : Result := 6;
    XoGt : Result := 6;
    XoLtEq : if IsSetLeft and IsSetRight then Result := 1
             else Result := 6;
    XoGtEq : if IsSetLeft and IsSetRight then Result := 1
             else Result := 6;
    else InternalError('Unknown precedence for operator in ' +
                       ExDescribe(Expr))
  end
end;

function _Precedence(Expr : TExpression) : integer;
begin
  case Expr^.Cls of 
    XcImmediate : Result := 0;
    XcToString : Result := 0;
    XcToReal : Result := 2;
    XcToUntypedPtr : Result := _Precedence(Expr^.ToUntypedPtrParent);
    XcWithTmpVar : Result := 0;
    XcSubrange : Result := 0;
    XcSet : Result := 0;
    XcVariable : if Expr^.VarPtr^.IsReference then Result := 2
                 else Result := 0;
    XcField : Result := 1;
    XcArray : Result := 1;
    XcPointer : Result := 2;
    XcAddress : Result := 2;
    XcStringChar : Result := 1;
    XcFnRef : Result := 0;
    XcFnCall : Result := 1;
    XcPseudoFnRef : Result := 0;
    XcSizeof : Result := 1;
    XcConvertToStr : Result := 1;
    XcConvertToVal : Result := 1;
    XcRead : Result := 1;
    XcWrite : Result := 1;
    XcUnaryOp : Result := 2;
    XcBinaryOp : Result := _BinOpPrec(Expr);
    else InternalError('Unknown precedence for ' + ExDescribe(Expr))
  end
end;

procedure _OutExpressionParensPrec(Expr : TExpression; Prec : integer);
var UseParens : boolean;
begin
  UseParens := _Precedence(Expr) > Prec;
  if UseParens then write(Codegen.Output, '(');
  OutExpression(Expr);
  if UseParens then write(Codegen.Output, ')')
end;

procedure _OutExpressionParens(Expr, Ref : TExpression);
begin
  _OutExpressionParensPrec(Expr, _Precedence(Ref))
end;

procedure _OutExpressionParensExtra(Expr, Ref : TExpression);
begin
  _OutExpressionParensPrec(Expr, _Precedence(Ref) - 1)
end;

procedure _OutSetTypeName(TypePtr : TPsTypePtr);
var NumBytes : integer;
begin
  NumBytes := GetTypeHighBound(TypePtr^.ElementTypePtr) div 8 -
              GetTypeLowBound(TypePtr^.ElementTypePtr) div 8 + 1;
  write(Codegen.Output, 'PSet', 8 * NumBytes)
end;

procedure _OutSetImmediate(Expr : TExpression);
var 
  LowBound, HighBound, LowBoundByte, SetSize : integer;
  Bounds : TExSetImmBounds;
  ElemTypePtr : TPsTypePtr;
  SetElems : array[1..32] of integer;
  Pos, ByteNum, BitNum : integer;
begin
  Bounds := Expr^.Immediate.SetBounds;
  ElemTypePtr := Expr^.TypePtr^.ElementTypePtr;
  LowBound := GetTypeLowBound(ElemTypePtr);
  HighBound := GetTypeHighBound(ElemTypePtr);
  LowBoundByte := GetTypeLowBound(ElemTypePtr) div 8;
  SetSize := HighBound div 8 - LowBound div 8 + 1;
  for Pos := 1 to SetSize do
    SetElems[Pos] := 0;
  while Bounds <> nil do
  begin
    if (Bounds^.First < LowBound) or (Bounds^.Last > HighBound) then
      CompileError('Set ' + ExDescribe(Expr) + ' contains elements ' +
      'that are out of bounds for ' + TypeName(Expr^.TypePtr));
    for Pos := Bounds^.First to Bounds^.Last do
    begin
      ByteNum := 1 + Pos div 8 - LowBoundByte;
      BitNum := Pos mod 8;
      SetElems[ByteNum] := SetElems[ByteNum] or (1 shl BitNum);
    end;
    Bounds := Bounds^.Next
  end;
  write(Codegen.Output, '(PSet', SetSize * 8, ') { ');
  for Pos := 1 to SetSize do
  begin
    if Pos <> 1 then _OutComma;
    write(Codegen.Output, SetElems[Pos]);
  end;
  write(Codegen.Output, ' }');
end;

procedure _OutExSet(Expr : TExpression);
var 
  ElementTypePtr : TPsTypePtr;
  Bounds : TExSetExprBounds;
  First, Last : TExpression;
  LowBoundByte : integer;
begin
  ElementTypePtr := Expr^.TypePtr^.ElementTypePtr;
  LowBoundByte := GetTypeLowBound(ElementTypePtr) div 8;
  write(Codegen.Output, '({ ');
  _OutSetTypeName(Expr^.TypePtr);
  write(Codegen.Output, ' dst = ');
  _OutSetImmediate(Expr^.SetBase);
  write(Codegen.Output, '; ');
  Bounds := Expr^.SetBounds;
  while Bounds <> nil do
  begin
    write(Codegen.Output, 'set_set(');
    First := ExOpOrd(ExCoerce(ExCopy(Bounds^.First), ElementTypePtr));
    if Bounds^.Last = nil then Last := ExCopy(First)
    else Last := ExOpOrd(ExCoerce(ExCopy(Bounds^.Last), ElementTypePtr));
    OutExpression(First);
    _OutComma;
    OutExpression(Last);
    _OutComma;
    write(Codegen.Output, LowBoundByte, ', dst.bits); ');
    ExDispose(First);
    ExDispose(Last);
    Bounds := Bounds^.Next
  end;
  write(Codegen.Output, 'dst; })')
end;

procedure _OutExImmediate(Expr : TExpression);
begin
  with Expr^.Immediate do
    case Cls of 
      XicNil : write(Codegen.Output, 'PNil');
      XicBoolean : if BooleanVal then write(Codegen.Output, '1')
                   else write(Codegen.Output, '0');
      XicInteger : write(Codegen.Output, IntegerVal);
      XicReal : write(Codegen.Output, RealVal);
      XicChar : _OutChar(CharVal);
      XicString : _OutString(StringVal);
      XicEnum : write(Codegen.Output, EnumPtr^.Values[EnumOrdinal]);
      XicSet : _OutSetImmediate(Expr);
    end
end;

procedure _OutBounds(TypePtr : TPsTypePtr);
begin
  write(Codegen.Output, GetTypeLowBound(TypePtr));
  _OutComma;
  write(Codegen.Output, GetTypeHighBound(TypePtr))
end;

procedure _OutArrayIndex(Index : TExpression; TypePtr : TPsTypePtr);
var LowBound : integer;
  Size : TExpression;
begin
  LowBound := GetTypeLowBound(TypePtr^.ArrayDef.IndexTypePtr);
  Size := ExOpSub(ExOpOrd(ExCopy(Index)), ExIntegerConstant(LowBound));
  OutExpression(Size);
  ExDispose(Size)
end;

procedure _OutAddress(Expr : TExpression);
begin
  if Expr^.Cls = XcPointer then OutExpression(Expr^.PointerExpr)
  else if (Expr^.Cls = XcVariable) and (Expr^.VarPtr^.IsReference) then
         write(Codegen.Output, Expr^.VarPtr^.Name)
  else if Expr^.Cls = XcToUntypedPtr then
  begin
    write(Codegen.Output, '(void**)&');
    _OutExpressionParensPrec(Expr, 1)
  end
  else
  begin
    write(Codegen.Output, '&');
    _OutExpressionParensPrec(Expr, 1)
  end
end;

procedure _OutExSubrange(Expr : TExpression);
begin
  if not Options.CheckBounds then OutExpression(Expr^.SubrangeParent)
  else
  begin
    write(Codegen.Output, 'subrange(');
    OutExpression(Expr^.SubrangeParent);
    _OutComma;
    _OutBounds(Expr^.TypePtr);
    write(Codegen.Output, ')')
  end
end;

procedure _OutExVariable(Expr : TExpression);
begin
  if Expr^.VarPtr^.IsReference then
    write(Codegen.Output, '*', Expr^.VarPtr^.Name)
  else
    write(Codegen.Output, Expr^.VarPtr^.Name)
end;

procedure _OutExFieldAccess(Expr : TExpression);
begin
  with Expr^.RecExpr^ do
  begin
    if Cls = XcPointer then
    begin
      _OutExpressionParens(PointerExpr, Expr);
      write(Codegen.Output, '->')
    end
    else if (Cls = XcVariable) and VarPtr^.IsReference then
           write(Codegen.Output, VarPtr^.Name, '->')
    else
    begin
      _OutExpressionParens(Expr^.RecExpr, Expr);
      write(Codegen.Output, '.')
    end;
    write(Codegen.Output, Expr^.RecExpr^.TypePtr^.RecPtr^
          .Fields[Expr^.RecFieldNum].Name)
  end
end;

procedure _OutExStringChar(Expr : TExpression);
begin
  with Expr^.StringExpr^ do
  begin
    if Cls = XcPointer then
    begin
      _OutExpressionParens(PointerExpr, Expr);
      write(Codegen.Output, '->chr[')
    end
    else if (Cls = XcVariable) and VarPtr^.IsReference then
           write(Codegen.Output, VarPtr^.Name, '->chr[')
    else
    begin
      _OutExpressionParens(Expr^.StringExpr, Expr);
      write(Codegen.Output, '.chr[')
    end;
    OutExpression(Expr^.StringIndex);
    write(Codegen.Output, ']')
  end
end;

procedure _OutExFunctionCallArgs(const ArgDefs : TPsFnArgs;
                                 const ArgValues : TExFunctionArgs);
var Pos : integer;
begin
  write(Codegen.Output, '(');
  for Pos := 1 to ArgValues.Size do
  begin
    if Pos <> 1 then _OutComma;
    if ArgDefs.Defs[Pos].IsReference then
    begin
      EnsureAddressableExpr(ArgValues.Values[Pos]);
      _OutAddress(ArgValues.Values[Pos])
    end
    else
      OutExpression(ArgValues.Values[Pos])
  end;
  write(Codegen.Output, ')')
end;

procedure _OutExFunctionCall(Expr : TExpression);
begin
  _OutExpressionParens(Expr^.FnExpr, Expr);
  if Expr^.FnExpr^.Cls = XcFnRef then
    _OutExFunctionCallArgs(Expr^.FnExpr^.FnPtr^.Args, Expr^.CallArgs)
  else
    _OutExFunctionCallArgs(Expr^.FnExpr^.TypePtr^.FnDefPtr^.Args,
                           Expr^.CallArgs)
end;

procedure _OutOrd(Expr : TExpression);
begin
  EnsureOrdinalExpr(Expr^.Unary.Parent);
  if IsCharType(Expr^.Unary.Parent^.TypePtr) then
  begin
    write(Codegen.Output, '(int)');
    _OutExpressionParensPrec(Expr^.Unary.Parent, 2)
  end
  else OutExpression(Expr^.Unary.Parent)
end;

procedure _OutPred(Expr : TExpression);
var TmpExpr : TExpression;
begin
  EnsureOrdinalExpr(Expr^.Unary.Parent);
  if IsBoundedType(Expr^.Unary.Parent^.TypePtr) then
  begin
    if Options.CheckBounds then
    begin
      write(Codegen.Output, 'pred(');
      OutExpression(Expr^.Unary.Parent);
      _OutComma;
      _OutBounds(Expr^.Unary.Parent^.TypePtr);
      write(Codegen.Output, ')')
    end
    else
    begin
      TmpExpr := ExOpSub(ExOpOrd(ExCopy(Expr^.Unary.Parent)),
                 ExIntegerConstant(1));
      OutExpression(TmpExpr);
      ExDispose(TmpExpr)
    end
  end
  else
  begin
    TmpExpr := ExOpSub(ExCopy(Expr^.Unary.Parent),
               ExIntegerConstant(1));
    OutExpression(TmpExpr);
    ExDispose(TmpExpr)
  end
end;

procedure _OutSucc(Expr : TExpression);
var TmpExpr : TExpression;
begin
  EnsureOrdinalExpr(Expr^.Unary.Parent);
  if IsBoundedType(Expr^.Unary.Parent^.TypePtr) then
  begin
    if Options.CheckBounds then
    begin
      write(Codegen.Output, 'succ(');
      OutExpression(Expr^.Unary.Parent);
      _OutComma;
      _OutBounds(Expr^.Unary.Parent^.TypePtr);
      write(Codegen.Output, ')')
    end
    else
    begin
      TmpExpr := ExOpAdd(ExOpOrd(ExCopy(Expr^.Unary.Parent)),
                 ExIntegerConstant(1));
      OutExpression(TmpExpr);
      ExDispose(TmpExpr)
    end
  end
  else
  begin
    TmpExpr := ExOpAdd(ExCopy(Expr^.Unary.Parent), ExIntegerConstant(1));
    OutExpression(TmpExpr);
    ExDispose(TmpExpr)
  end
end;

procedure _OutExUnaryOp(Expr : TExpression);
begin
  if Expr^.Unary.Op = XoOrd then _OutOrd(Expr)
  else if Expr^.Unary.Op = XoPred then _OutPred(Expr)
  else if Expr^.Unary.Op = XoSucc then _OutSucc(Expr)
  else if Expr^.Unary.Op = XoNeg then
  begin
    write(Codegen.Output, '-');
    _OutExpressionParens(Expr^.Unary.Parent, Expr)
  end
  else if (Expr^.Unary.Op = XoNot) and IsBooleanType(Expr^.TypePtr) then
  begin
    write(Codegen.Output, '!');
    _OutExpressionParens(Expr^.Unary.Parent, Expr)
  end
  else if (Expr^.Unary.Op = XoNot) and IsIntegerType(Expr^.TypePtr) then
  begin
    write(Codegen.Output, '~');
    _OutExpressionParens(Expr^.Unary.Parent, Expr)
  end
end;

function _IsArithmeticOp(Op : TExOperator) : boolean;
begin
  _IsArithmeticOp := Op in [XoAdd, XoSub, XoMul, XoDivReal, XoDivInt, XoMod]
end;

function _GetArithmeticOp(Op : TExOperator) : string;
begin
  case Op of 
    XoAdd : _GetArithmeticOp := '+';
    XoSub : _GetArithmeticOp := '-';
    XoMul : _GetArithmeticOp := '*';
    XoDivReal : _GetArithmeticOp := '/';
    XoDivInt : _GetArithmeticOp := '/';
    XoMod : _GetArithmeticOp := '%';
  end
end;

function _IsLogicalOrBitwiseOp(Op : TExOperator) : boolean;
begin
  _IsLogicalOrBitwiseOp := Op in [XoAnd, XoOr, XoXor]
end;

function _IsBitwiseOp(Op : TExOperator) : boolean;
begin
  _IsBitwiseOp := Op in [XoShl, XoShr]
end;

function _GetLogicalOp(Op : TExOperator) : string;
begin
  case Op of 
    XoAnd: Result := '&&';
    XoOr: Result := '||';
    XoXor: Result := '!=';
    else InternalError('Unimplemented logical operator ' +
                       ExDescribeOperator(Op))
  end
end;

function _GetBitwiseOp(Op : TExOperator) : string;
begin
  case Op of 
    XoAnd: Result := '&';
    XoOr: Result := '|';
    XoXor: Result := '^';
    XoShl: Result := '<<';
    XoShr: Result := '>>';
    else InternalError('Unimplemented bitwise operator ' +
                       ExDescribeOperator(Op))
  end
end;

function _IsRelationalOp(Op : TExOperator) : boolean;
begin
  _IsRelationalOp := Op in [XoEq, XoNe, XoLt, XoGt, XoLtEq, XoGtEq]
end;

function _GetRelationalOp(Op : TExOperator) : string;
begin
  case Op of 
    XoEq: _GetRelationalOp := '==';
    XoNe : _GetRelationalOp := '!=';
    XoLt : _GetRelationalOp := '<';
    XoGt : _GetRelationalOp := '>';
    XoLtEq : _GetRelationalOp := '<=';
    XoGtEq : _GetRelationalOp := '>='
  end
end;

procedure _OutExSetOperation(Left, Right: TExpression; Op : TExOperator);
var 
  ElemTypePtr : TPsTypePtr;
  LowBound, HighBound, LowBoundByte, SetSize : integer;
begin
  ElemTypePtr := Right^.TypePtr^.ElementTypePtr;
  if Op = XoLtEq then _OutExSetOperation(Right, Left, XoGtEq)
  else if Op = XoNe then
  begin
    write(Codegen.Output, '!');
    _OutExSetOperation(Left, Right, XoEq)
  end
  else if Op = XoIn then
  begin
    LowBoundByte := GetTypeLowBound(ElemTypePtr) div 8;
    write(Codegen.Output, 'set_in(');
    OutExpression(Left);
    _OutComma;
    write(Codegen.Output, LowBoundByte);
    _OutComma;
    _OutExpressionParensPrec(Right, 1);
    write(Codegen.Output, '.bits)')
  end
  else
  begin
    LowBound := GetTypeLowBound(ElemTypePtr);
    HighBound := GetTypeHighBound(ElemTypePtr);
    SetSize := HighBound div 8 - LowBound div 8 + 1;
    if Op = XoEq then
    begin
      write(Codegen.Output, 'set_equals(');
      _OutExpressionParensPrec(Left, 1);
      write(Codegen.Output, '.bits, ');
      _OutExpressionParensPrec(Right, 1);
      write(Codegen.Output, '.bits, ', SetSize, ')')
    end
    else if Op = XoGtEq then
    begin
      write(Codegen.Output, 'set_issuperset(');
      _OutExpressionParensPrec(Left, 1);
      write(Codegen.Output, '.bits, ');
      _OutExpressionParensPrec(Right, 1);
      write(Codegen.Output, '.bits, ', SetSize, ')')
    end
    else
    begin
      write(Codegen.Output, '({ PSet', SetSize * 8, ' dst; ');
      case Op of 
        XoAdd: write(Codegen.Output, 'set_union(');
        XoSub: write(Codegen.Output, 'set_difference(');
        XoMul: write(Codegen.Output, 'set_intersection(');
        else
          InternalError('Materialized set operation not implemented: ' +
                        ExDescribeOperator(Op))
      end;
      _OutExpressionParensPrec(Left, 1);
      write(Codegen.Output, '.bits, ');
      _OutExpressionParensPrec(Right, 1);
      write(Codegen.Output, '.bits, dst.bits, ', SetSize, '); dst; })')
    end
  end
end;

procedure _OutCmpConcatArg(Expr : TExpression);
begin
  if ExIsImmediate(Expr) and IsStringType(Expr^.TypePtr) then
  begin
    write(Codegen.Output, 'CpLenPtr, ', Length(Expr^.Immediate.StringVal));
    _OutComma;
    _OutCstring(Expr^.Immediate.StringVal)
  end
  else if IsCharType(Expr^.TypePtr) then
  begin
    write(Codegen.Output, 'CpChar, ');
    OutExpression(Expr)
  end
  else if Expr^.IsAddressable then
  begin
    write(Codegen.Output, 'CpStringPtr, ');
    _OutAddress(Expr)
  end
  else
  begin
    write(Codegen.Output, 'CpString, ');
    OutExpression(Expr)
  end
end;

procedure _OutConcatArgs(Expr : TExpression; Last : boolean);
begin
  if not IsStringyType(Expr^.TypePtr) then
    InternalError('Expected a stringy type for ' + ExDescribe(Expr))
  else if (Expr^.Cls <> XcBinaryOp) or (Expr^.Binary.Op <> XoAdd) then
  begin
    if Last then write(Codegen.Output, 'CpEnd | ');
    _OutCmpConcatArg(Expr);
    if not Last then _OutComma
  end
  else
  begin
    _OutConcatArgs(Expr^.Binary.Left, {Last=}false);
    _OutConcatArgs(Expr^.Binary.Right, Last)
  end
end;

procedure _OutExBinaryOp(Expr : TExpression);
begin
  with Expr^.Binary do
  begin
    if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    begin
      _OutExpressionParens(Left, Expr);
      if _IsLogicalOrBitwiseOp(Op) then
        write(Codegen.Output, ' ', _GetLogicalOp(Op), ' ')
      else if _IsRelationalOp(Op) then
             write(Codegen.Output, ' ' , _GetRelationalOp(Op), ' ')
      else ErrorInvalidOperator(Expr, Op);
      _OutExpressionParensExtra(Right, Expr)
    end
    else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
    begin
      _OutExpressionParens(Left, Expr);
      if _IsArithmeticOp(Op) then
        write(Codegen.Output, ' ', _GetArithmeticOp(Op), ' ')
      else if _IsLogicalOrBitwiseOp(Op) or _IsBitwiseOp(Op) then
             write(Codegen.Output, ' ', _GetBitwiseOp(Op), ' ')
      else if _IsRelationalOp(Op) then
             write(Codegen.Output, ' ' , _GetRelationalOp(Op), ' ')
      else ErrorInvalidOperator(Expr, Op);
      _OutExpressionParensExtra(Right, Expr)
    end
    else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
    begin
      if Op = XoAdd then
      begin
        write(Codegen.Output, 'CONCAT(');
        _OutConcatArgs(Expr, {Last=}true);
        write(Codegen.Output, ')')
      end
      else if IsCharType(Left^.TypePtr) and IsCharType(Right^.TypePtr) then
      begin
        _OutExpressionParens(Left, Expr);
        if _IsRelationalOp(Op) then
          write(Codegen.Output, ' ', _GetRelationalOp(Op), ' ')
        else ErrorInvalidOperator(Expr, Op);
        _OutExpressionParensExtra(Right, Expr)
      end
      else
      begin
        write(Codegen.Output, 'cmp_str(');
        case Op of 
          XoEq: write(Codegen.Output, 'CoEq, ');
          XoNe: write(Codegen.Output, 'CoNotEq, ');
          XoLt: write(Codegen.Output, 'CoBefore, ');
          XoGt: write(Codegen.Output, 'CoAfter, ');
          XoLtEq: write(Codegen.Output, 'CoBeforeOrEq, ');
          XoGtEq: write(Codegen.Output, 'CoAfterOrEq, ');
          else ErrorInvalidOperator(Expr, Op)
        end;
        _OutCmpConcatArg(Left);
        _OutComma;
        _OutCmpConcatArg(Right);
        write(Codegen.Output, ')')
      end
    end
    else if IsSetType(Right^.TypePtr) then _OutExSetOperation(Left, Right, Op)
    else
    begin
      _OutExpressionParens(Left, Expr);
      if _IsRelationalOp(Op) then
        write(Codegen.Output, ' ', _GetRelationalOp(Op), ' ')
      else ErrorInvalidOperator(Expr, Op);
      _OutExpressionParensExtra(Right, Expr)
    end
  end
end;

procedure _OutExWithTmpVar(Expr : TExpression);
begin
  write(Codegen.Output, '({ ');
  while Expr^.Cls = XcWithTmpVar do
  begin
    OutVariableDeclaration(Expr^.TmpVar^.VarPtr^);
    write(Codegen.Output, ' = ');
    OutExpression(Expr^.TmpVarValue);
    write(Codegen.Output, '; ');
    Expr := Expr^.TmpVarChild
  end;
  OutExpression(Expr);
  write(Codegen.Output, '; })')
end;

procedure _OutExSizeof(Expr : TExpression);
begin
  write(Codegen.Output, 'sizeof(');
  OutTypeReference(Expr^.SizeofTypePtr);
  write(Codegen.Output, ')')
end;

function ShortTypeName(TypePtr : TPsTypePtr) : char;
begin
  if IsBooleanType(TypePtr) then ShortTypeName := 'b'
  else if IsIntegerType(TypePtr) then ShortTypeName := 'i'
  else if IsRealType(TypePtr) then ShortTypeName := 'r'
  else if IsCharType(TypePtr) then ShortTypeName := 'c'
  else if IsStringType(TypePtr) then ShortTypeName := 's'
  else CompileError('Type ' + TypeName(TypePtr) + ' is not representable for ' +
    'READ, WRITE, STR, or VAL')
end;

procedure _OutExConvertToStr(Expr : TExpression);
var Src, Dst, Width, Prec : TExpression;
begin
  Src := Expr^.ToStrSrc.Arg;
  Dst := Expr^.ToStrDest;
  Width := Expr^.ToStrSrc.Width;
  Prec := Expr^.ToStrSrc.Prec;
  if IsEnumType(Src^.TypePtr) then
  begin
    _OutIndent;
    write(Codegen.Output, 'STR_e(');
    OutExpression(Src);
    write(Codegen.Output, ', enumvalues', Src^.TypePtr^.EnumPtr^.Id);
    _OutComma;
    if Width <> nil then OutExpression(Width)
    else write(Codegen.Output, '0');
    _OutComma;
    _OutAddress(Dst);
    write(Codegen.Output, ')')
  end
  else if IsRealType(Src^.TypePtr) then
  begin
    _OutIndent;
    write(Codegen.Output, 'STR_r(');
    OutExpression(Src);
    _OutComma;
    if Width <> nil then OutExpression(Width)
    else write(Codegen.Output, '0');
    _OutComma;
    if Prec <> nil then OutExpression(Prec)
    else write(Codegen.Output, '-1');
    _OutComma;
    _OutAddress(Dst);
    write(Codegen.Output, ')')
  end
  else
  begin
    _OutIndent;
    write(Codegen.Output, 'STR_', ShortTypeName(Src^.TypePtr), '(');
    OutExpression(Src);
    _OutComma;
    if Width <> nil then OutExpression(Width)
    else write(Codegen.Output, '0');
    _OutComma;
    _OutAddress(Dst);
    write(Codegen.Output, ')')
  end
end;

procedure _OutExConvertToVal(Expr : TExpression);
var Src, Dst, Code, TmpExpr : TExpression;
begin
  Src := Expr^.ToValSrc;
  Dst := Expr^.ToValDest;
  Code := Expr^.ToValCode;
  if IsEnumType(Dst^.TypePtr) then
  begin
    _OutIndent;
    write(Codegen.Output, 'VAL_e(');
    _OutAddress(Src);
    _OutComma;
    _OutAddress(Dst);
    _OutComma;
    TmpExpr := ExIntegerConstant(Dst^.TypePtr^.EnumPtr^.Size);
    OutExpression(TmpExpr);
    ExDispose(TmpExpr);
    write(Codegen.Output, ', enumvalues', Dst^.TypePtr^.EnumPtr^.Id);
    _OutComma;
    _OutAddress(Code);
    write(Codegen.Output, ')')
  end
  else
  begin
    _OutIndent;
    write(Codegen.Output, 'VAL_', ShortTypeName(Dst^.TypePtr), '(');
    _OutAddress(Src);
    _OutComma;
    _OutAddress(Dst);
    _OutComma;
    _OutAddress(Code);
    write(Codegen.Output, ')')
  end
end;

procedure _OutExRead(Expr : TExpression);
var 
  Src : TExpression;
  ReadArg : TExReadArgList;
  Linefeed : boolean;
  TypePtr : TPsTypePtr;
begin
  Src := Expr^.ReadFile;
  Linefeed := Expr^.Readln;
  ReadArg := Expr^.ReadArgs;
  _OutIndent;
  write(Codegen.Output, 'Read(');
  _OutAddress(Src);
  _OutComma;
  OutExpression(ExBooleanConstant(Options.CheckIoResult));
  if ReadArg = nil then
  begin
    write(Codegen.Output, ', RwpEnd');
    if Linefeed then write(Codegen.Output, ' | RwpLn')
  end;
  while ReadArg <> nil do
  begin
    TypePtr := GetFundamentalType(ReadArg^.Dest^.TypePtr);
    case TypePtr^.Cls of 
      TtcInteger: write(Codegen.Output, ', RwpInt');
      TtcReal: write(Codegen.Output, ', RwpReal');
      TtcChar: write(Codegen.Output, ', RwpChar');
      TtcString: write(Codegen.Output, ', RwpString');
      else ErrorForExpr('Expression has invalid type for READ', ReadArg^.Dest)
    end;
    if ReadArg^.Next = nil then
    begin
      if Linefeed then write(Codegen.Output, ' | RwpLn');
      write(Codegen.Output, ' | RwpEnd')
    end;
    _OutComma;
    _OutAddress(ReadArg^.Dest);
    ReadArg := ReadArg^.Next
  end;
  write(Codegen.Output, ')')
end;

procedure _OutExWrite(Expr : TExpression);
var 
  Dst : TExpression;
  WriteArg : TExWriteArgList;
  Linefeed : boolean;
  TypePtr : TPsTypePtr;
begin
  Dst := Expr^.WriteFile;
  Linefeed := Expr^.WriteLn;
  WriteArg := Expr^.WriteArgs;
  _OutIndent;
  write(Codegen.Output, 'Write(');
  _OutAddress(Dst);
  _OutComma;
  OutExpression(ExBooleanConstant(Options.CheckIoResult));
  if WriteArg = nil then
  begin
    write(Codegen.Output, ', RwpEnd');
    if Linefeed then write(Codegen.Output, ' | RwpLn')
  end;
  while WriteArg <> nil do
  begin
    TypePtr := GetFundamentalType(WriteArg^.Value.Arg^.TypePtr);
    case TypePtr^.Cls of 
      TtcBoolean: write(Codegen.Output, ', RwpBool');
      TtcInteger: write(Codegen.Output, ', RwpInt');
      TtcReal: write(Codegen.Output, ', RwpReal');
      TtcChar: write(Codegen.Output, ', RwpChar');
      TtcEnum: write(Codegen.Output, ', RwpEnum');
      TtcString:
                 begin
                   if ExIsImmediate(WriteArg^.Value.Arg) then
                     write(Codegen.Output, ', RwpLenPtr')
                   else if WriteArg^.Value.Arg^.IsAddressable then
                          write(Codegen.Output, ', RwpStringPtr')
                   else
                     write(Codegen.Output, ', RwpString')
                 end;
      else ErrorForExpr('Expression has invalid type for WRITE',
                        WriteArg^.Value.Arg)
    end;
    if WriteArg^.Value.Width <> nil then write(Codegen.Output, ' | RwpWidth');
    if IsRealType(TypePtr) and (WriteArg^.Value.Prec <> nil) then
      write(Codegen.Output, ' | RwpPrec');
    if WriteArg^.Next = nil then
    begin
      if Linefeed then write(Codegen.Output, ' | RwpLn');
      write(Codegen.Output, ' | RwpEnd')
    end;
    if WriteArg^.Value.Width <> nil then
    begin
      _OutComma;
      OutExpression(WriteArg^.Value.Width)
    end;
    if WriteArg^.Value.Prec <> nil then
    begin
      _OutComma;
      OutExpression(WriteArg^.Value.Prec)
    end;
    _OutComma;
    if IsStringType(WriteArg^.Value.Arg^.TypePtr)
       and ExIsImmediate(WriteArg^.Value.Arg) then
    begin
      write(Codegen.Output, Length(WriteArg^.Value.Arg^.Immediate.StringVal));
      _OutComma;
      _OutCstring(WriteArg^.Value.Arg^.Immediate.StringVal)
    end
    else if IsStringType(WriteArg^.Value.Arg^.TypePtr)
            and WriteArg^.Value.Arg^.IsAddressable then
           _OutAddress(WriteArg^.Value.Arg)
    else
      OutExpression(WriteArg^.Value.Arg);
    if IsEnumType(TypePtr) then
      write(Codegen.Output, ', enumvalues', TypePtr^.EnumPtr^.Id);
    WriteArg := WriteArg^.Next
  end;
  write(Codegen.Output, ')')
end;

procedure OutExpression;
begin
  case Expr^.Cls of 
    XcImmediate: _OutExImmediate(Expr);
    XcToString:
                begin
                  write(Codegen.Output, 'str_of(');
                  OutExpression(Expr^.ToStrParent);
                  write(Codegen.Output, ')')
                end;
    XcToReal:
              begin
                write(Codegen.Output, '(double)');
                OutExpression(Expr^.ToRealParent)
              end;
    XcToUntypedPtr: OutExpression(Expr^.ToUntypedPtrParent);
    XcWithTmpVar: _OutExWithTmpVar(Expr);
    XcSubrange: _OutExSubrange(Expr);
    XcSet: _OutExSet(Expr);
    XcVariable : _OutExVariable(Expr);
    XcField: _OutExFieldAccess(Expr);
    XcArray:
             begin
               _OutExpressionParens(Expr^.ArrayExpr, Expr);
               write(Codegen.Output, '[');
               _OutArrayIndex(Expr^.ArrayIndex, Expr^.ArrayExpr^.TypePtr);
               write(Codegen.Output, ']')
             end;
    XcPointer:
               begin
                 write(Codegen.Output, '*');
                 _OutExpressionParens(Expr^.PointerExpr, Expr)
               end;
    XcAddress: _OutAddress(Expr^.AddressExpr);
    XcStringChar: _OutExStringChar(Expr);
    XcFnRef: write(Codegen.Output, Expr^.FnPtr^.ExternalName);
    XcFnCall: _OutExFunctionCall(Expr);
    XcSizeof: _OutExSizeof(Expr);
    XcConvertToStr: _OutExConvertToStr(Expr);
    XcConvertToVal: _OutExConvertToVal(Expr);
    XcRead: _OutExRead(Expr);
    XcWrite: _OutExWrite(Expr);
    XcUnaryOp: _OutExUnaryOp(Expr);
    XcBinaryOp: _OutExBinaryOp(Expr)
  end
end;

procedure OutEnumValues(EnumPtr : TPsEnumPtr);
var 
  PosInEnum : integer;
begin
  _OutBlankline(TotEnumVal);
  _OutIndent;
  write(Codegen.Output, 'const char* enumvalues', EnumPtr^.Id, '[] = { ');
  for PosInEnum := 0 to EnumPtr^.Size - 1 do
  begin
    if PosInEnum <> 0 then _OutComma;
    write(Codegen.Output, '"', EnumPtr^.Values[PosInEnum], '"')
  end;
  write(Codegen.Output, ' };');
  _OutNewline
end;

procedure OutEnumValuesFromCheckpoint;
var 
  Def : TPsDefPtr;
begin
  Def := Checkpoint^.Next;
  while Def <> nil do
  begin
    if (Def^.Cls = TdcType) and (Def^.TypePtr^.Cls = TtcEnum) then
    begin
      if not Def^.TypePtr^.EnumPtr^.ValuesHaveBeenOutput then
        OutEnumValues(Def^.TypePtr^.EnumPtr);
      Def^.TypePtr^.EnumPtr^.ValuesHaveBeenOutput := true
    end;
    Def := Def^.Next
  end
end;

function _GetRangeType(TypePtr : TPsTypePtr) : string;
type 
  Types = (U8, S8, U16, S16, U32, S32);
const 
  Names : array[Types] of string = ('PBits8', 'PBits8S', 'PBits16', 'PBits16S',
                                    'PBits32', 'PBits32S');
  LowLimits : array[Types] of integer = (0, -128, 0, -32768, 0, -2147483648);
  HighLimits : array[Types] of integer = (255, 127, 65535, 32767, 4294967295,
                                          2147483647);
var 
  FitTypes : set of Types;
  Low, High : integer;
  T : Types;
begin
  FitTypes := [];
  Low := GetTypeLowBound(TypePtr);
  High := GetTypeHighBound(TypePtr);
  for T := U8 to S32 do
    if (Low >= LowLimits[T]) and (High <= HighLimits[T]) then
      FitTypes := FitTypes + [T];
  Result := 'PInteger';
  for T := S32 downto U8 do
    if T in FitTypes then Result := Names[T]
end;

procedure OutTypeReference(TypePtr : TPsTypePtr);
var TheType : TPsTypePtr;
begin
  if TypePtr = nil then write(Codegen.Output, 'void')
  else if TypePtr^.Cls = TtcPointer then
  begin
    OutTypeReference(TypePtr^.PointedTypePtr);
    write(Codegen.Output, '*')
  end
  else if TypePtr^.Cls = TtcBoolean then write(Codegen.Output, 'PBoolean')
  else if TypePtr^.Cls = TtcInteger then write(Codegen.Output, 'PInteger')
  else if TypePtr^.Cls = TtcReal then write(Codegen.Output, 'PReal')
  else if TypePtr^.Cls = TtcChar then write(Codegen.Output, 'PChar')
  else if TypePtr^.Cls = TtcString then write(Codegen.Output, 'PString')
  else if TypePtr^.Cls = TtcText then write(Codegen.Output, 'PFile')
  else if TypePtr^.Cls = TtcEnum then
  begin
    if TypePtr^.EnumPtr^.HasBeenDefined and (TypePtr^.Name <> '') then
      write(Codegen.Output, TypePtr^.Name)
    else
      write(Codegen.Output, 'enum enum', TypePtr^.EnumPtr^.Id)
  end
  else if TypePtr^.Cls = TtcRange then
         write(Codegen.Output, _GetRangeType(TypePtr))
  else if TypePtr^.Cls = TtcSet then
         _OutSetTypeName(TypePtr)
  else if TypePtr^.Cls = TtcRecord then
  begin
    if TypePtr^.RecPtr^.HasBeenDefined and (TypePtr^.Name <> '') then
      write(Codegen.Output, TypePtr^.Name)
    else
      write(Codegen.Output, 'struct record', TypePtr^.RecPtr^.Id)
  end
  else if TypePtr^.Cls = TtcArray then
  begin
    TheType := TypePtr;
    while IsArrayType(TheType) do
      TheType := TheType^.ArrayDef.ValueTypePtr;
    OutTypeReference(TheType);
    TheType := TypePtr;
    while IsArrayType(TheType) do
    begin
      write(Codegen.Output, '[', GetBoundedTypeSize(TheType^.ArrayDef.
            IndexTypePtr), ']');
      TheType := TheType^.ArrayDef.ValueTypePtr
    end
  end
  else
    InternalError('Error writing type reference: ' + TypeName(TypePtr))
end;

procedure OutNameAndType(const Name : string; TypePtr : TPsTypePtr);
forward;

procedure OutNameAndRecord(const Name : string; RecPtr : TPsRecPtr);
var 
  Pos : integer;
  NumVariant : integer;
begin
  NumVariant := 0;
  if RecPtr^.HasBeenDefined then
    write(Codegen.Output, 'struct record', RecPtr^.Id)
  else
  begin
    write(Codegen.Output, 'struct ');
    if RecPtr^.IsPacked then
      write(Codegen.Output, '__attribute__((__packed__)) ');
    write(Codegen.Output, 'record', RecPtr^.Id, ' ');
    OutBegin;
    for Pos := 1 to RecPtr^.Size do
    begin
      if (RecPtr^.NumVariants > NumVariant)
         and (RecPtr^.VariantBounds[NumVariant + 1] = Pos) then
      begin
        NumVariant := NumVariant + 1;
        if NumVariant = 1 then
        begin
          _OutIndent;
          write(Codegen.Output, 'union ');
          OutBegin
        end
        else
        begin
          OutEndSameLine;
          write(Codegen.Output, ';');
          _OutNewline
        end;
        _OutIndent;
        write(Codegen.Output, 'struct ');
        OutBegin;
      end;
      _OutIndent;
      OutNameAndType(RecPtr^.Fields[Pos].Name, RecPtr^.Fields[Pos].TypePtr);
      write(Codegen.Output, ';');
      _OutNewline;
    end;
    if NumVariant > 0 then
    begin
      OutEndSameLine;
      write(Codegen.Output, ';');
      _OutNewline;
      OutEndSameLine;
      write(Codegen.Output, ';');
      _OutNewline
    end;
    OutEndSameLine;
    RecPtr^.HasBeenDefined := true
  end;
  write(Codegen.Output, ' ', Name)
end;

procedure OutNameAndEnum(const Name : string; EnumPtr : TPsEnumPtr);
var 
  Pos : integer;
begin
  if EnumPtr^.HasBeenDefined then
    write(Codegen.Output, 'enum enum', EnumPtr^.Id)
  else
  begin
    write(Codegen.Output, 'enum __attribute__((__packed__)) enum', EnumPtr^.Id,
          ' { ');
    for Pos := 0 to EnumPtr^.Size - 1 do
    begin
      if Pos > 0 then
        _OutComma;
      write(Codegen.Output, EnumPtr^.Values[Pos])
    end;
    write(Codegen.Output, ' }');
    EnumPtr^.HasBeenDefined := true
  end;
  write(Codegen.Output, ' ', Name)
end;

procedure OutNameAndArray(const Name : string; TypePtr : TPsTypePtr);
var 
  TheType : TPsTypePtr;
begin
  TheType := TypePtr;
  while IsArrayType(TheType) do
    TheType := TheType^.ArrayDef.ValueTypePtr;
  OutNameAndType(Name, TheType);
  TheType := TypePtr;
  while IsArrayType(TheType) do
  begin
    write(Codegen.Output, '[',
          GetBoundedTypeSize(TheType^.ArrayDef.IndexTypePtr), ']');
    TheType := TheType^.ArrayDef.ValueTypePtr
  end
end;

procedure OutNameAndFunction(const Name : string; TypePtr : TPsTypePtr);
var Pos : integer;
begin
  OutNameAndType('(*' + Name + ')', TypePtr^.FnDefPtr^.ReturnTypePtr);
  write(Codegen.Output, '(');
  for Pos := 1 to TypePtr^.FnDefPtr^.Args.Count do
  begin
    if Pos <> 1 then _OutComma;
    OutVariableDeclaration(TypePtr^.FnDefPtr^.Args.Defs[Pos])
  end;
  write(Codegen.Output, ')')
end;

procedure OutNameAndType(const Name : string; TypePtr : TPsTypePtr);
var Sp : string;
begin
  if Name[1] <> '*' then Sp := ' '
  else Sp := '';
  if TypePtr = nil then write(Codegen.Output, 'void', Sp, Name)
  else if TypePtr^.Cls = TtcPointer then
  begin
    OutTypeReference(TypePtr^.PointedTypePtr);
    write(Codegen.Output, '*', Sp, Name)
  end
  else if (TypePtr^.AliasFor <> nil) and (TypePtr^.Name <> '') then
         write(Codegen.Output, TypePtr^.Name, Sp, Name)
  else if TypePtr^.Cls = TtcBoolean then
         write(Codegen.Output, 'PBoolean', Sp, Name)
  else if TypePtr^.Cls = TtcInteger then
         write(Codegen.Output, 'PInteger', Sp, Name)
  else if TypePtr^.Cls = TtcReal then
         write(Codegen.Output, 'PReal', Sp, Name)
  else if TypePtr^.Cls = TtcChar then
         write(Codegen.Output, 'PChar', Sp, Name)
  else if TypePtr^.Cls = TtcString then
         write(Codegen.Output, 'PString', Sp, Name)
  else if TypePtr^.Cls = TtcText then
         write(Codegen.Output, 'PFile', Sp, Name)
  else if TypePtr^.Cls = TtcEnum then
         OutNameAndEnum(Name, TypePtr^.EnumPtr)
  else if TypePtr^.Cls = TtcRange then
         write(Codegen.Output, _GetRangeType(TypePtr), Sp, Name)
  else if TypePtr^.Cls = TtcSet then
  begin
    _OutSetTypeName(TypePtr);
    write(Codegen.Output, Sp, Name)
  end
  else if TypePtr^.Cls = TtcRecord then
         OutNameAndRecord(Name, TypePtr^.RecPtr)
  else if TypePtr^.Cls = TtcArray then OutNameAndArray(Name, TypePtr)
  else if TypePtr^.Cls = TtcFunction then OutNameAndFunction(Name, TypePtr)
  else
    InternalError('Error writing name and type: ' + Name + ', ' +
                  TypeName(TypePtr))
end;

procedure OutTypeDefinition(TypePtr : TPsTypePtr);
var 
  Name : string;
begin
  _OutBlankline(TotType);
  _OutIndent;
  Name := TypePtr^.Name;
  if TypePtr^.AliasFor = nil then
    InternalError('Type ' + Name + ' is not an alias');
  write(Codegen.Output, 'typedef ');
  OutNameAndType(Name, TypePtr^.AliasFor);
  write(Codegen.Output, ';');
  _OutNewline
end;

procedure OutTypeDefinitionsFromCheckpoint;
var 
  Def : TPsDefPtr;
begin
  Def := Checkpoint^.Next;
  while Def <> nil do
  begin
    if Def^.Cls = TdcType then
    begin
      _ResolvePointerForward(Def^.TypePtr);
      if Def^.TypePtr^.AliasFor <> nil then OutTypeDefinition(Def^.TypePtr)
    end;
    Def := Def^.Next
  end
end;

procedure OutConstantArrayBegin;
begin
  write(Codegen.Output, '{ ')
end;

procedure OutConstantArraySeparator;
begin
  _OutComma
end;

procedure OutConstantArrayEnd;
begin
  write(Codegen.Output, ' }')
end;

procedure OutConstantRecordBegin;
begin
  write(Codegen.Output, '{ ')
end;

procedure OutConstantRecordField(const Name : string);
begin
  write(Codegen.Output, '.', Name, ' = ')
end;

procedure OutConstantRecordSeparator;
begin
  _OutComma
end;

procedure OutConstantRecordEnd;
begin
  write(Codegen.Output, ' }')
end;

procedure OutVariableDeclaration(VarDef : TPsVariable);
var Name : string;
begin
  Name := VarDef.Name;
  if VarDef.IsConstant then
  begin
    if VarDef.IsReference and IsPointerType(VarDef.TypePtr) then
      Name := ' const ' + Name
    else
      write(Codegen.Output, 'const ')
  end;
  if VarDef.IsReference then Name := '* ' + Name;
  OutNameAndType(Name, VarDef.TypePtr)
end;

procedure OutVariableDefinition;
begin
  _OutBlankline(TotVar);
  _OutIndent;
  OutVariableDeclaration(VarPtr^);
  if Location <> nil then
  begin
    write(Codegen.Output, ' = ');
    if not IsSameType(VarPtr^.TypePtr, Location^.TypePtr) then
    begin
      write(Codegen.Output, '(');
      OutTypeReference(VarPtr^.TypePtr);
      write(Codegen.Output, '*)')
    end;
    _OutAddress(Location)
  end;
  write(Codegen.Output, ';');
  _OutNewline
end;

procedure OutConstantDefinitionBegin;
begin
  _OutBlankline(TotVar);
  _OutIndent;
  OutVariableDeclaration(VarPtr^);
  write(Codegen.Output, ' = ')
end;

procedure OutConstantDefinitionEnd;
begin
  write(Codegen.Output, ';');
  _OutNewline
end;

procedure OutFunctionPrototype(Def : TPsFunction);
var 
  Pos : integer;
begin
  _OutIndent;
  OutNameAndType(Def.ExternalName, Def.ReturnTypePtr);
  write(Codegen.Output, '(');
  for Pos := 1 to Def.Args.Count do
  begin
    if Pos <> 1 then _OutComma;
    OutVariableDeclaration(Def.Args.Defs[Pos])
  end;
  write(Codegen.Output, ')')
end;

procedure OutFunctionDeclaration;
begin
  _OutBlankline(TotFunDec);
  OutFunctionPrototype(FnPtr^);
  write(Codegen.Output, ';');
  _OutNewline
end;

procedure OutFunctionDefinition;
begin
  _OutBlankline(TotFunDef);
  OutFunctionPrototype(FnPtr^);
  write(Codegen.Output, ' ');
  OutBegin
end;

procedure OutFunctionEnd;
begin
  if FnPtr^.ReturnTypePtr <> nil then
  begin
    _OutIndent;
    write(Codegen.Output, 'return RESULT;');
    _OutNewline
  end;
  OutEnd
end;

procedure OutProgramHeading;
begin
  write(Codegen.Output, '/* Program: ', Name, ' */');
  _OutNewline;
  write(Codegen.Output, '#include "pascual.h"');
  _OutNewline
end;

procedure OutAssign;
begin
  _OutIndent;
  OutExpression(Lhs);
  write(Codegen.Output, ' = ');
  OutExpression(Rhs);
  write(Codegen.Output, ';');
  _OutNewline
end;

procedure OutDeclareAndAssign;
begin
  _OutIndent;
  OutVariableDeclaration(VarPtr^);
  write(Codegen.Output, ' = ');
  if (VarPtr^.IsReference) then _OutAddress(Rhs)
  else OutExpression(Rhs);
  write(Codegen.Output, ';');
  _OutNewline
end;

procedure OutIf;
begin
  _OutIndent;
  write(Codegen.Output, 'if (');
  OutExpression(Expr);
  write(Codegen.Output, ') ');
  Codegen.IsMultiStatement := false
end;

procedure OutElse;
begin
  _OutIndent;
  write(Codegen.Output, 'else ');
  Codegen.IsMultiStatement := false
end;

procedure OutIfEnd;
begin
  codegen.IsMultiStatement := true
end;

procedure OutSequenceBegin;
begin
  _OutIndent;
  OutBegin
end;

procedure OutSequenceEnd;
begin
  OutEnd
end;

procedure OutCaseBegin;
begin
  _OutIndent;
  write(Codegen.Output, 'switch (');
  OutExpression(CaseIndex);
  write(Codegen.Output, ') ');
  OutBegin
end;

procedure OutCaseStatementBegin;
begin
  _OutIndent;
  write(Codegen.Output, 'case ');
  OutExpression(CaseLabel);
  write(Codegen.Output, ':');
  write(Codegen.Output);
  Codegen.Indent := Codegen.Indent + 1;
  _OutNewline
end;

procedure OutCaseStatementEnd;
begin
  _OutIndent;
  write(CodeGen.Output, 'break;');
  Codegen.Indent := Codegen.Indent - 1;
  _OutNewline
end;

procedure OutCaseElseBegin;
begin
  _OutIndent;
  write(CodeGen.Output, 'default:');
  Codegen.Indent := Codegen.Indent + 1;
  _OutNewline
end;

procedure OutCaseElseEnd;
begin
  _OutIndent;
  write(CodeGen.Output, 'break;');
  Codegen.Indent := Codegen.Indent - 1;
  _OutNewline
end;

procedure OutCaseEnd;
begin
  OutEnd
end;

procedure OutRepeatBegin;
begin
  _OutIndent;
  write(Codegen.Output, 'do ');
  OutBegin
end;

procedure OutRepeatEnd;
var TmpExpr : TExpression;
begin
  OutEndSameLine;
  write(Codegen.Output, ' while (');
  TmpExpr := ExOpNot(ExCopy(Expr));
  OutExpression(TmpExpr);
  ExDispose(TmpExpr);
  write(Codegen.Output, ');');
  _OutNewline
end;

procedure OutWhileBegin;
begin
  _OutIndent;
  write(Codegen.Output, 'while (');
  OutExpression(Expr);
  write(Codegen.Output, ') ');
  Codegen.IsMultiStatement := false
end;

procedure OutWhileEnd;
begin
  codegen.IsMultiStatement := true
end;

procedure OutForBegin;
var 
  LimitType : TPsTypePtr;
  First, Last : TPsVariable;
begin
  LimitType := Iter^.TypePtr;
  if IsEnumType(LimitType) then LimitType := PrimitiveTypes.PtInteger;
  First := MakeVariable('first', LimitType);
  Last := MakeVariable('last', LimitType);
  _OutIndent;
  write(Codegen.Output, 'for (');
  OutVariableDeclaration(First);
  write(Codegen.Output, ' = ');
  OutExpression(FirstExpr);
  _OutComma;
  write(Codegen.Output, Last.Name);
  write(Codegen.Output, ' = ');
  OutExpression(LastExpr);
  write(Codegen.Output, '; ');
  write(Codegen.Output, 'first ');
  if Ascending then
    write(Codegen.Output, '<=')
  else
    write(Codegen.Output, '>=');
  write(Codegen.Output, ' last; /*breaks*/) ');
  OutBegin;
  _OutIndent;
  write(Codegen.Output, 'PBoolean done = 0;');
  _OutNewline;
  _OutIndent;
  write(Codegen.Output, 'for (');
  OutExpression(Iter);
  write(Codegen.Output, ' = first; !done; done = ');
  OutExpression(Iter);
  write(Codegen.Output, ' == last ? 1 : (');
  if Ascending then
    write(Codegen.Output, '++')
  else
    write(Codegen.Output, '--');
  OutExpression(Iter);
  write(Codegen.Output, ', 0)) ')
end;

procedure OutForEnd;
begin
  _OutIndent;
  write(Codegen.Output, 'break;');
  _OutNewline;
  OutEnd
end;

procedure OutExpressionStatement;
begin
  _OutIndent;
  OutExpression(Expr);
  write(Codegen.Output, ';');
  _OutNewline
end;

procedure OutEmptyStatement;
begin
  _OutIndent;
  write(Codegen.Output, ';');
  _OutNewline
end;

procedure OutProgramBegin;
begin
  _OutBlankline(TotFunDef);
  _OutIndent;
  write(Codegen.Output, 'void pascual_main() ');
  OutBegin
end;

procedure OutProgramEnd;
begin
  OutEnd
end;

procedure CodegenReset;
begin
  Codegen.Output := Output;
  Codegen.IsMultiStatement := false;
  Codegen.Indent := 0;
  Codegen.Newline := true;
  Codegen.LastOut := TotNone;
end;

procedure CodegenSetOutput;
begin
  Assign(Codegen.Output, Filename);
  Rewrite(Codegen.Output)
end;

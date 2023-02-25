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

procedure _OutCstring(var Str : string);
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
      if (Pos < Length(Str)) and LxIsHexDigit(Str[Pos + 1]) then
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

procedure _OutString(var Str : string);
begin
  if Length(Str) = 1 then
  begin
    write(Codegen.Output, 'str_of(');
    _OutChar(Str[1]);
    write(Codegen.Output, ')')
  end
  else
  begin
    write(Codegen.Output, 'str_make(', Length(Str), ', ');
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
    TkPlus : if IsStringyType(Expr^.TypePtr) then Result := 1
             else Result := 4;
    TkMinus : Result := 4;
    TkAsterisk : Result := 3;
    TkSlash : Result := 3;
    TkDiv : Result := 3;
    TkMod : Result := 3;
    TkAnd : if IsBooleanType(Expr^.TypePtr) then Result := 11
            else Result := 8;
    TkOr : if IsBooleanType(Expr^.TypePtr) then Result := 12
           else Result := 10;
    TkXor : if IsBooleanType(Expr^.TypePtr) then Result := 7
            else Result := 9;
    TkShl : Result := 5;
    TkShr : Result := 5;
    TkIn : if ExIsImmediate(Expr^.Binary.Right) then Result := 12
           else Result := 1;
    TkEquals : if IsSetLeft and IsSetRight then Result := 1
               else Result := 7;
    TkNotEquals : if IsSetLeft and IsSetRight then Result := 1
                  else Result := 7;
    TkLessthan : Result := 6;
    TkMorethan : Result := 6;
    TkLessOrEquals : if IsSetLeft and IsSetRight then Result := 1
                     else Result := 6;
    TkMoreOrEquals : if IsSetLeft and IsSetRight then Result := 1
                     else Result := 6;
    else InternalError('Unknown precedence for operator in ' +
                       DescribeExpr(Expr, 5))
  end
end;

function _Precedence(Expr : TExpression) : integer;
begin
  case Expr^.Cls of 
    XcImmediate : Result := 0;
    XcToString : Result := 0;
    XcToReal : Result := 2;
    XcWithTmpVar : Result := 0;
    XcSubrange : Result := 0;
    XcSet : Result := 0;
    XcVariable : if Expr^.VarPtr^.IsReference then Result := 2
                 else Result := 0;
    XcField : Result := 1;
    XcArray : Result := 1;
    XcPointer : Result := 2;
    XcStringChar : Result := 1;
    XcFnRef : Result := 0;
    XcFnCall : Result := 1;
    XcPseudoFnRef : Result := 0;
    XcPseudoFnCall : Result := 1;
    XcUnaryOp : Result := 2;
    XcBinaryOp : Result := _BinOpPrec(Expr);
    else InternalError('Unknown precedence for ' + DescribeExpr(Expr, 5))
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
      CompileError('Set ' + DescribeExpr(Expr, 1) + ' contains elements ' +
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
    if Pos <> 1 then write(Codegen.Output, ', ');
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
    First := PfOrd(ExCoerce(CopyExpr(Bounds^.First), ElementTypePtr));
    if Bounds^.Last = nil then Last := CopyExpr(First)
    else Last := PfOrd(ExCoerce(CopyExpr(Bounds^.Last), ElementTypePtr));
    OutExpression(First);
    write(Codegen.Output, ', ');
    OutExpression(Last);
    write(Codegen.Output, ', ', LowBoundByte, ', dst.bits); ');
    DisposeExpr(First);
    DisposeExpr(Last);
    Bounds := Bounds^.Next
  end;
  write(Codegen.Output, 'dst; })')
end;

procedure _OutExImmediate(Expr : TExpression);
begin
  with Expr^.Immediate do
    case Cls of 
      XicNil : write(Codegen.Output, '(void*)0');
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
  write(Codegen.Output, GetTypeLowBound(TypePtr), ', ',
  GetTypeHighBound(TypePtr))
end;

procedure _OutArrayIndex(Index : TExpression; TypePtr : TPsTypePtr);
var LowBound : integer;
  Size : TExpression;
begin
  LowBound := GetTypeLowBound(TypePtr^.ArrayDef.IndexTypePtr);
  Size := ExBinaryOp(PfOrd(CopyExpr(Index)),
          ExIntegerConstant(LowBound),
          TkMinus);
  OutExpression(Size);
  DisposeExpr(Size)
end;

procedure _OutAddress(Expr : TExpression);
begin
  if Expr^.Cls = XcPointer then OutExpression(Expr^.PointerExpr)
  else if (Expr^.Cls = XcVariable) and (Expr^.VarPtr^.IsReference) then
         write(Codegen.Output, Expr^.VarPtr^.Name)
  else
  begin
    write(Codegen.Output, '&');
    _OutExpressionParensPrec(Expr, 2)
  end
end;

procedure _OutExSubrange(Expr : TExpression);
begin
  if not Options.CheckBounds then OutExpression(Expr^.SubrangeParent)
  else
  begin
    write(Codegen.Output, 'subrange(');
    OutExpression(Expr^.SubrangeParent);
    write(Codegen.Output, ', ');
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

procedure _OutExFunctionCall(Expr : TExpression);
var Pos : integer;
begin
  _OutExpressionParens(Expr^.FnExpr, Expr);
  write(Codegen.Output, '(');
  for Pos := 1 to Expr^.CallArgs.Size do
  begin
    if Pos <> 1 then write(Codegen.Output, ', ');
    if Expr^.FnExpr^.FnPtr^.Args[Pos].IsReference then
    begin
      if not Expr^.CallArgs.Values[Pos]^.IsAssignable then
        CompileError('Pass-by-reference argument must be assignable');
      _OutAddress(Expr^.CallArgs.Values[Pos])
    end
    else
      OutExpression(Expr^.CallArgs.Values[Pos])
  end;
  write(Codegen.Output, ')')
end;

procedure _OutDispose(Expr : TExpression);
forward;
procedure _OutNew(Expr : TExpression);
forward;
procedure _OutOrd(Expr : TExpression);
forward;
procedure _OutPred(Expr : TExpression);
forward;
procedure _OutRead(Expr : TExpression);
forward;
procedure _OutStr(Expr : TExpression);
forward;
procedure _OutSucc(Expr : TExpression);
forward;
procedure _OutVal(Expr : TExpression);
forward;
procedure _OutWrite(Expr : TExpression);
forward;

procedure _OutExPseudoFnCall(Expr : TExpression);
begin
  case Expr^.PseudoFnCall.PseudoFn of 
    TpfDispose : _OutDispose(Expr);
    TpfNew : _OutNew(Expr);
    TpfOrd : _OutOrd(Expr);
    TpfPred : _OutPred(Expr);
    TpfRead : _OutRead(Expr);
    TpfReadln : _OutRead(Expr);
    TpfStr : _OutStr(Expr);
    TpfSucc : _OutSucc(Expr);
    TpfVal : _OutVal(Expr);
    TpfWrite : _OutWrite(Expr);
    TpfWriteln : _OutWrite(Expr);
    else InternalError('Unimplemented special function ' +
                       DescribeExpr(Expr, 5))
  end
end;

procedure _OutExUnaryOp(Expr : TExpression);
begin
  if Expr^.Unary.Op = TkMinus then write(Codegen.Output, '-')
  else if (Expr^.Unary.Op = TkNot) and IsBooleanType(Expr^.TypePtr) then
         write(Codegen.Output, '!')
  else if (Expr^.Unary.Op = TkNot) and IsIntegerType(Expr^.TypePtr) then
         write(Codegen.Output, '~');
  _OutExpressionParens(Expr^.Unary.Parent, Expr)
end;

function _IsArithmeticOp(Op : TLxTokenId) : boolean;
begin
  _IsArithmeticOp := Op in [TkPlus, TkMinus, TkAsterisk, TkSlash, TkDiv, TkMod]
end;

function _GetArithmeticOp(Op : TLxTokenId) : string;
begin
  case Op of 
    TkPlus : _GetArithmeticOp := '+';
    TkMinus : _GetArithmeticOp := '-';
    TkAsterisk : _GetArithmeticOp := '*';
    TkSlash : _GetArithmeticOp := '/';
    TkDiv : _GetArithmeticOp := '/';
    TkMod : _GetArithmeticOp := '%';
  end
end;

function _IsLogicalOrBitwiseOp(Op : TLxTokenId) : boolean;
begin
  _IsLogicalOrBitwiseOp := Op in [TkAnd, TkOr, TkXor]
end;

function _IsBitwiseOp(Op : TLxTokenId) : boolean;
begin
  _IsBitwiseOp := Op in [TkShl, TkShr]
end;

function _GetLogicalOp(Op : TLxTokenId) : string;
begin
  if Op = TkAnd then _GetLogicalOp := '&&'
  else if Op = TkOr then _GetLogicalOp := '||'
  else if Op = TkXor then _GetLogicalOp := '!='
  else InternalError('Unimplemented logical operator ' + LxTokenName(Op))
end;

function _GetBitwiseOp(Op : TLxTokenId) : string;
begin
  if Op = TkAnd then _GetBitwiseOp := '&'
  else if Op = TkOr Then _GetBitwiseOp := '|'
  else if Op = TkXor then _GetBitwiseOp := '^'
  else if Op = TkShl then _GetBitwiseOp := '<<'
  else if Op = TkShr then _GetBitwiseOp := '>>'
  else InternalError('Unimplemented bitwise operator ' + LxTokenName(Op))
end;

function _IsRelationalOp(Op : TLxTokenId) : boolean;
begin
  _IsRelationalOp := Op in [TkEquals, TkNotEquals, TkLessthan, TkMorethan,
                     TkLessOrEquals, TkMoreOrEquals]
end;

function _GetRelationalOp(Op : TLxTokenId) : string;
begin
  case Op of 
    TkEquals: _GetRelationalOp := '==';
    TkNotEquals : _GetRelationalOp := '!=';
    TkLessthan : _GetRelationalOp := '<';
    TkMorethan : _GetRelationalOp := '>';
    TkLessOrEquals : _GetRelationalOp := '<=';
    TkMoreOrEquals : _GetRelationalOp := '>='
  end
end;

procedure _OutExSetOperation(Left, Right: TExpression; Op : TLxTokenId);
var 
  ElemTypePtr : TPsTypePtr;
  LowBound, HighBound, LowBoundByte, SetSize : integer;
begin
  ElemTypePtr := Right^.TypePtr^.ElementTypePtr;
  if Op = TkLessOrEquals then _OutExSetOperation(Right, Left, TkMoreOrEquals)
  else if Op = TkNotEquals then
  begin
    write(Codegen.Output, '!');
    _OutExSetOperation(Left, Right, TkEquals)
  end
  else if Op = TkIn then
  begin
    LowBoundByte := GetTypeLowBound(ElemTypePtr) div 8;
    write(Codegen.Output, 'set_in(');
    OutExpression(Left);
    write(Codegen.Output, ', ', LowBoundByte, ', ');
    _OutExpressionParensPrec(Right, 1);
    write(Codegen.Output, '.bits)')
  end
  else
  begin
    LowBound := GetTypeLowBound(ElemTypePtr);
    HighBound := GetTypeHighBound(ElemTypePtr);
    SetSize := HighBound div 8 - LowBound div 8 + 1;
    if Op = TkEquals then
    begin
      write(Codegen.Output, 'set_equals(');
      _OutExpressionParensPrec(Left, 1);
      write(Codegen.Output, '.bits, ');
      _OutExpressionParensPrec(Right, 1);
      write(Codegen.Output, '.bits, ', SetSize, ')')
    end
    else if Op = TkMoreOrEquals then
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
        TkPlus: write(Codegen.Output, 'set_union(');
        TkMinus: write(Codegen.Output, 'set_difference(');
        TkAsterisk: write(Codegen.Output, 'set_intersection(');
        else
          InternalError('Materialized set operation not implemented: ' +
                        LxTokenName(Op))
      end;
      _OutExpressionParensPrec(Left, 1);
      write(Codegen.Output, '.bits, ');
      _OutExpressionParensPrec(Right, 1);
      write(Codegen.Output, '.bits, dst.bits, ', SetSize, '); dst; })')
    end
  end
end;

procedure _OutExBinaryOp(Expr : TExpression);
var Ltype, Rtype : char;
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
      else CompileError('Not a valid operator: ' + LxTokenName(Op));
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
      else CompileError('Not a valid operator: ' + LxTokenName(Op));
      _OutExpressionParensExtra(Right, Expr)
    end
    else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
    begin
      if IsCharType(Left^.TypePtr) then Ltype := 'c'
      else Ltype := 's';
      if IsCharType(Right^.TypePtr) then Rtype := 'c'
      else Rtype := 's';
      if Op = TkPlus then
      begin
        write(Codegen.Output, 'cat_', Ltype, Rtype, '(');
        OutExpression(Left);
        write(Codegen.Output, ', ');
        OutExpression(Right);
        write(Codegen.Output, ')')
      end
      else if IsCharType(Left^.TypePtr) and IsCharType(Right^.TypePtr) then
      begin
        _OutExpressionParens(Left, Expr);
        if _IsRelationalOp(Op) then
          write(Codegen.Output, ' ', _GetRelationalOp(Op), ' ')
        else CompileError('Not a valid operator: ' + LxTokenName(Op));
        _OutExpressionParensExtra(Right, Expr)
      end
      else
      begin
        write(Codegen.Output, 'cmp_', Ltype, Rtype, '(');
        OutExpression(Left);
        write(Codegen.Output, ', ');
        OutExpression(Right);
        if _IsRelationalOp(Op) then
          write(Codegen.Output, ') ', _GetRelationalOp(Op), ' 0')
        else CompileError('Not a valid operator: ' + LxTokenName(Op))
      end
    end
    else if IsSetType(Right^.TypePtr) then _OutExSetOperation(Left, Right, Op)
    else
    begin
      _OutExpressionParens(Left, Expr);
      if _IsRelationalOp(Op) then
        write(Codegen.Output, ' ', _GetRelationalOp(Op), ' ')
      else CompileError('Not a valid operator: ' + LxTokenName(Op));
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
    XcStringChar:
                  begin
                    _OutExpressionParens(Expr^.StringExpr, Expr);
                    write(Codegen.Output, '.chr[');
                    OutExpression(Expr^.StringIndex);
                    write(Codegen.Output, ']')
                  end;
    XcFnRef: write(Codegen.Output, Expr^.FnPtr^.Name);
    XcFnCall: _OutExFunctionCall(Expr);
    XcPseudoFnCall: _OutExPseudoFnCall(Expr);
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
    if PosInEnum <> 0 then write(Codegen.Output, ', ');
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

function OutVariableName(Name : string; IsReference : boolean) : string;
begin
  if IsReference then OutVariableName := '*' + Name
  else OutVariableName := Name
end;

procedure OutTypeReference(TypePtr : TPsTypePtr);
begin
  if TypePtr = nil then write(Codegen.Output, 'void')
  else if TypePtr^.Cls = TtcPointer then
  begin
    OutTypeReference(TypePtr^.PointedTypePtr);
    write(Codegen.Output, '*')
  end
  else if TypePtr^.Cls = TtcBoolean then write(Codegen.Output, 'int')
  else if TypePtr^.Cls = TtcInteger then write(Codegen.Output, 'int')
  else if TypePtr^.Cls = TtcReal then write(Codegen.Output, 'double')
  else if TypePtr^.Cls = TtcChar then write(Codegen.Output, 'unsigned char')
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
         OutTypeReference(GetFundamentalType(TypePtr))
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
    OutTypeReference(TypePtr^.ArrayDef.ValueTypePtr);
    write(Codegen.Output, '*')
  end
  else
    CompileError('Error writing type reference: ' + TypeName(TypePtr))
end;

procedure OutNameAndType(Name : string; TypePtr : TPsTypePtr);
forward;

procedure OutNameAndRecord(Name : string; RecPtr : TPsRecPtr);
var 
  Pos : integer;
  NumVariant : integer;
begin
  NumVariant := 0;
  write(Codegen.Output, 'struct record', RecPtr^.Id);
  if not RecPtr^.HasBeenDefined then
  begin
    write(Codegen.Output, ' ');
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

procedure OutNameAndEnum(Name : string; EnumPtr : TPsEnumPtr);
var 
  Pos : integer;
begin
  write(Codegen.Output, 'enum enum', EnumPtr^.Id);
  if not EnumPtr^.HasBeenDefined then
  begin
    write(Codegen.Output, ' { ');
    for Pos := 0 to EnumPtr^.Size - 1 do
    begin
      if Pos > 0 then
        write(Codegen.Output, ', ');
      write(Codegen.Output, EnumPtr^.Values[Pos])
    end;
    write(Codegen.Output, ' }');
    EnumPtr^.HasBeenDefined := true
  end;
  write(Codegen.Output, ' ', Name)
end;

procedure OutNameAndArray(Name : string; TypePtr : TPsTypePtr);
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
          GetBoundedTypeSize(TypePtr^.ArrayDef.IndexTypePtr), ']');
    TheType := TheType^.ArrayDef.ValueTypePtr
  end
end;

procedure OutNameAndType(Name : string; TypePtr : TPsTypePtr);
begin
  if TypePtr = nil then write(Codegen.Output, 'void ', Name)
  else if TypePtr^.Cls = TtcPointer then
  begin
    OutTypeReference(TypePtr^.PointedTypePtr);
    write(Codegen.Output, ' *', Name)
  end
  else if (TypePtr^.AliasFor <> nil) and (TypePtr^.Name <> '') then
         write(Codegen.Output, TypePtr^.Name, ' ', Name)
  else if TypePtr^.Cls = TtcBoolean then
         write(Codegen.Output, 'int ', Name)
  else if TypePtr^.Cls = TtcInteger then
         write(Codegen.Output, 'int ', Name)
  else if TypePtr^.Cls = TtcReal then
         write(Codegen.Output, 'double ', Name)
  else if TypePtr^.Cls = TtcChar then
         write(Codegen.Output, 'unsigned char ', Name)
  else if TypePtr^.Cls = TtcString then
         write(Codegen.Output, 'PString ', Name)
  else if TypePtr^.Cls = TtcText then
         write(Codegen.Output, 'PFile ', Name)
  else if TypePtr^.Cls = TtcEnum then
         OutNameAndEnum(Name, TypePtr^.EnumPtr)
  else if TypePtr^.Cls = TtcRange then
         OutNameAndType(Name, GetFundamentalType(TypePtr))
  else if TypePtr^.Cls = TtcSet then
  begin
    _OutSetTypeName(TypePtr);
    write(Codegen.Output, ' ', Name)
  end
  else if TypePtr^.Cls = TtcRecord then
         OutNameAndRecord(Name, TypePtr^.RecPtr)
  else if TypePtr^.Cls = TtcArray then OutNameAndArray(Name, TypePtr)
  else
    CompileError('Error writing name and type: ' + Name + ', ' +
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
    CompileError('Type ' + Name + ' is not an alias');
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
      _ResolvePointerUnknown(Def^.TypePtr);
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
  write(Codegen.Output, ', ')
end;

procedure OutConstantArrayEnd;
begin
  write(Codegen.Output, ' }')
end;

procedure OutVariableDeclaration(VarDef : TPsVariable);
begin
  if VarDef.IsConstant then write(Codegen.Output, 'const ');
  OutNameAndType(OutVariableName(VarDef.Name, VarDef.IsReference),
  VarDef.TypePtr)
end;

procedure OutVariableDefinition;
begin
  _OutBlankline(TotVar);
  _OutIndent;
  OutVariableDeclaration(VarPtr^);
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
  OutNameAndType(Def.Name, Def.ReturnTypePtr);
  write(Codegen.Output, '(');
  for Pos := 1 to Def.ArgCount do
  begin
    OutVariableDeclaration(def.Args[Pos]);
    if Pos <> Def.ArgCount then
      write(Codegen.Output, ', ')
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

function ShortTypeName(TypePtr : TPsTypePtr) : char;
begin
  TypePtr := GetFundamentalType(TypePtr);
  if IsBooleanType(TypePtr) then ShortTypeName := 'b'
  else if IsIntegerType(TypePtr) then ShortTypeName := 'i'
  else if IsRealType(TypePtr) then ShortTypeName := 'r'
  else if IsCharType(TypePtr) then ShortTypeName := 'c'
  else if IsStringType(TypePtr) then ShortTypeName := 's'
  else CompileError('Type ' + TypeName(TypePtr) + ' is not representable for ' +
    'READ, WRITE, STR, or VAL')
end;

procedure _OutRead(Expr : TExpression);
var 
  Src : TExpression;
  ReadArg : ^TExReadArgs;
  Linefeed : boolean;
  Braces : boolean;
begin
  Src := Expr^.PseudoFnCall.Arg1;
  Linefeed := Expr^.PseudoFnCall.PseudoFn = TpfReadln;
  ReadArg := Expr^.PseudoFnCall.ReadArgs;
  Braces := (not Codegen.IsMultiStatement) and (ReadArg <> nil)
            and ((ReadArg^.Next <> nil) or Linefeed);
  if Braces then OutBegin;
  while ReadArg <> nil do
  begin
    _OutIndent;
    write(Codegen.Output, 'READ_', ShortTypeName(ReadArg^.Arg^.TypePtr), '(');
    _OutAddress(Src);
    write(Codegen.Output,', ');
    _OutAddress(ReadArg^.Arg);
    write(Codegen.Output, ');');
    _OutNewline;
    ReadArg := ReadArg^.Next
  end;
  if Linefeed then
  begin
    _OutIndent;
    write(Codegen.Output, 'READLN(');
    _OutAddress(Src);
    write(Codegen.Output, ');');
    _OutNewline
  end;
  if Braces then OutEnd
end;

procedure _OutWrite(Expr : TExpression);
var 
  Dst : TExpression;
  WriteArg : ^TExWriteArgs;
  Linefeed : boolean;
  Braces : boolean;
  TypePtr : TPsTypePtr;
begin
  Dst := Expr^.PseudoFnCall.Arg1;
  Linefeed := Expr^.PseudoFnCall.PseudoFn = TpfWriteln;
  WriteArg := Expr^.PseudoFnCall.WriteArgs;
  Braces := (not Codegen.IsMultiStatement) and (WriteArg <> nil)
            and ((WriteArg^.Next <> nil) or Linefeed);
  if Braces then OutBegin;
  while WriteArg <> nil do
  begin
    TypePtr := GetFundamentalType(WriteArg^.Arg^.TypePtr);
    if IsEnumType(TypePtr) then
    begin
      _OutIndent;
      write(Codegen.Output, 'WRITE_e(');
      _OutAddress(Dst);
      write(Codegen.Output, ', ');
      OutExpression(WriteArg^.Arg);
      write(Codegen.Output, ', enumvalues', TypePtr^.EnumPtr^.Id, ');');
      _OutNewline
    end
    else
    begin
      _OutIndent;
      write(Codegen.Output, 'WRITE_', ShortTypeName(TypePtr), '(');
      _OutAddress(Dst);
      write(Codegen.Output, ', ');
      OutExpression(WriteArg^.Arg);
      write(Codegen.output, ');');
      _OutNewline
    end;
    WriteArg := WriteArg^.Next
  end;
  if Linefeed then
  begin
    _OutIndent;
    write(Codegen.Output, 'WRITELN(');
    _OutAddress(Dst);
    write(Codegen.Output, ');');
    _OutNewline
  end;
  if Braces then OutEnd
end;

procedure _OutStr(Expr : TExpression);
var Src, Dst : TExpression;
begin
  Src := Expr^.PseudoFnCall.Arg1;
  Dst := Expr^.PseudoFnCall.Arg2;
  if IsEnumType(Src^.TypePtr) then
  begin
    _OutIndent;
    write(Codegen.Output, 'STR_e(');
    OutExpression(Src);
    write(Codegen.Output, ', enumvalues', Src^.TypePtr^.EnumPtr^.Id, ', ');
    _OutAddress(Dst);
    write(Codegen.Output, ');');
    _OutNewline
  end
  else
  begin
    _OutIndent;
    write(Codegen.Output, 'STR_', ShortTypeName(Src^.TypePtr), '(');
    OutExpression(Src);
    write(Codegen.Output, ', ');
    _OutAddress(Dst);
    write(Codegen.Output, ');');
    _OutNewline
  end
end;

procedure _OutVal(Expr : TExpression);
var Src, Dst, Code, TmpExpr : TExpression;
begin
  Src := Expr^.PseudoFnCall.Arg1;
  Dst := Expr^.PseudoFnCall.Arg2;
  Code := Expr^.PseudoFnCall.Arg3;
  if IsEnumType(Dst^.TypePtr) then
  begin
    _OutIndent;
    write(Codegen.Output, 'VAL_e(');
    _OutAddress(Src);
    write(Codegen.Output, ', ');
    _OutAddress(Dst);
    write(Codegen.Output, ', ');
    TmpExpr := ExIntegerConstant(Dst^.TypePtr^.EnumPtr^.Size);
    OutExpression(TmpExpr);
    DisposeExpr(TmpExpr);
    write(Codegen.Output, ', enumvalues', Dst^.TypePtr^.EnumPtr^.Id, ', ');
    _OutAddress(Code);
    write(Codegen.Output, ');');
    _OutNewline
  end
  else
  begin
    _OutIndent;
    write(Codegen.Output, 'VAL_', ShortTypeName(Dst^.TypePtr), '(');
    _OutAddress(Src);
    write(Codegen.Output, ', ');
    _OutAddress(Dst);
    write(Codegen.Output, ', ');
    _OutAddress(Code);
    write(Codegen.Output, ');');
    _OutNewline
  end
end;

procedure _OutNew(Expr : TExpression);
var Ptr : TExpression;
begin
  Ptr := Expr^.PseudoFnCall.Arg1;
  _OutIndent;
  OutExpression(Ptr);
  write(Codegen.Output, ' = malloc(sizeof(');
  OutTypeReference(Ptr^.TypePtr^.PointedTypePtr);
  write(Codegen.Output, '));');
  _OutNewline
end;

procedure _OutDispose(Expr : TExpression);
var Ptr : TExpression;
begin
  Ptr := Expr^.PseudoFnCall.Arg1;
  _OutIndent;
  write(Codegen.Output, 'free(');
  OutExpression(Ptr);
  write(Codegen.Output, ');');
  _OutNewline
end;

procedure _OutOrd(Expr : TExpression);
begin
  if IsCharType(Expr^.PseudoFnCall.Arg1^.TypePtr) then
  begin
    write(Codegen.Output, '(int)');
    _OutExpressionParensPrec(Expr^.PseudoFnCall.Arg1, 2)
  end
  else if IsOrdinalType(Expr^.PseudoFnCall.Arg1^.TypePtr) then
         OutExpression(Expr^.PseudoFnCall.Arg1)
  else CompileError('Expected an ordinal type, got ' +
                    TypeName(Expr^.PseudoFnCall.Arg1^.TypePtr))
end;

procedure _OutPred(Expr : TExpression);
var TmpExpr : TExpression;
begin
  if IsIntegerType(Expr^.PseudoFnCall.Arg1^.TypePtr) then
  begin
    TmpExpr := ExBinaryOp(CopyExpr(Expr^.PseudoFnCall.Arg1),
               ExIntegerConstant(1), TkMinus);
    OutExpression(TmpExpr);
    DisposeExpr(TmpExpr)
  end
  else if IsOrdinalType(Expr^.PseudoFnCall.Arg1^.TypePtr) then
  begin
    if not Options.CheckBounds then
    begin
      TmpExpr := ExBinaryOp(PfOrd(CopyExpr(Expr^.PseudoFnCall.Arg1)),
                 ExIntegerConstant(1), TkMinus);
      OutExpression(TmpExpr);
      DisposeExpr(TmpExpr)
    end
    else
    begin
      write(Codegen.Output, 'pred(');
      OutExpression(Expr^.PseudoFnCall.Arg1);
      write(Codegen.Output, ', ');
      _OutBounds(Expr^.PseudoFnCall.Arg1^.TypePtr);
      write(Codegen.Output, ')')
    end
  end
  else CompileError('Expected an ordinal type, got ' +
                    TypeName(Expr^.PseudoFnCall.Arg1^.TypePtr))
end;

procedure _OutSucc(Expr : TExpression);
var TmpExpr : TExpression;
begin
  if IsIntegerType(Expr^.PseudoFnCall.Arg1^.TypePtr) then
  begin
    TmpExpr := ExBinaryOp(CopyExpr(Expr^.PseudoFnCall.Arg1),
               ExIntegerConstant(1), TkPlus);
    OutExpression(TmpExpr);
    DisposeExpr(TmpExpr)
  end
  else if IsOrdinalType(Expr^.PseudoFnCall.Arg1^.TypePtr) then
  begin
    if not Options.CheckBounds then
    begin
      TmpExpr := ExBinaryOp(PfOrd(CopyExpr(Expr^.PseudoFnCall.Arg1)),
                 ExIntegerConstant(1), TkPlus);
      OutExpression(TmpExpr);
      DisposeExpr(TmpExpr)
    end
    else
    begin
      write(Codegen.Output, 'succ(');
      OutExpression(Expr^.PseudoFnCall.Arg1);
      write(Codegen.Output, ', ');
      _OutBounds(Expr^.PseudoFnCall.Arg1^.TypePtr);
      write(Codegen.Output, ')')
    end
  end
  else CompileError('Expected an ordinal type, got ' +
                    TypeName(Expr^.PseudoFnCall.Arg1^.TypePtr))
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
  TmpExpr := ExUnaryOp(CopyExpr(Expr), TkNot);
  OutExpression(TmpExpr);
  DisposeExpr(TmpExpr);
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
  write(Codegen.Output, 'do ');
  OutBegin;
  _OutIndent;
  OutVariableDeclaration(First);
  write(Codegen.Output, ' = ');
  OutExpression(FirstExpr);
  write(Codegen.Output, ';');
  _OutNewline;
  _OutIndent;
  OutVariableDeclaration(Last);
  write(Codegen.Output, ' = ');
  OutExpression(LastExpr);
  write(Codegen.Output, ';');
  _OutNewline;
  _OutIndent;
  write(Codegen.Output, 'if (first ');
  if Ascending then
    write(Codegen.Output, '<=')
  else
    write(Codegen.Output, '>=');
  write(Codegen.Output, ' last) ');
  OutBegin;
  _OutIndent;
  OutExpression(Iter);
  write(Codegen.Output, ' = first;');
  _OutNewline;
  _OutIndent;
  write(Codegen.Output, 'while (1) ');
  OutBegin
end;

procedure OutForEnd;
begin
  _OutIndent;
  write(Codegen.Output, 'if (');
  OutExpression(Iter);
  write(Codegen.Output, ' == last) break;');
  _OutNewline;
  _OutIndent;
  if Ascending then
    write(Codegen.Output, '++')
  else
    write(Codegen.Output, '--');
  OutExpression(Iter);
  write(Codegen.Output, ';');
  _OutNewline;
  OutEnd;
  OutEnd;
  OutEndSameLine;
  write(Codegen.Output, ' while(0);');
  _OutNewline
end;

procedure OutProcedureCall;
begin
  _OutIndent;
  OutExpression(Expr);
  write(Codegen.Output, ';');
  _OutNewline
end;

procedure OutPseudoProcCall;
begin
  _OutExPseudoFnCall(Expr)
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

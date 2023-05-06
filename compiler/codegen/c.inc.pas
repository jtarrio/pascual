type 
  TCgC_OutputType = (TotNone, TotType, TotVar, TotEnumVal,
                     TotFunDec, TotFunDef);
  TCgC = ^TCgC_Obj;
  TCgC_Obj = record
    Base : TCodegenBase;
    Output : text;
    IsMultiStatement : boolean;
    Indent : integer;
    Newline : boolean;
    LastOut : TCgC_OutputType;
  end;

procedure _CgC_OutTypeReference(This : TCgC; TypePtr : TSDType);
forward;
procedure _CgC_OutNameAndType(This : TCgC; const Name : string;
                              TypePtr : TSDType);
forward;
procedure _CgC_OutDefinitions(This : TCgC; Scope : TSScope);
forward;
procedure _CgC_OutExpression(This : TCgC; Expr : TSExpression);
forward;
procedure _CgC_OutBody(This : TCgC; Body : TSSSequence);
forward;
procedure _CgC_OutStatement(This : TCgC; Stmt : TSStatement);
forward;

procedure _CgC_OutNewline(This : TCgC);
begin
  writeln(This^.Output);
  This^.Newline := true
end;

procedure _CgC_OutBlankline(This : TCgC; NewOut : TCgC_OutputType);
begin
  if (This^.Indent = 0) and
     ((This^.LastOut <> NewOut) or (NewOut = TotFunDef)) then
    _CgC_OutNewline(This);
  This^.LastOut := NewOut
end;

procedure _CgC_OutIndent(This : TCgC);
var Ct : integer;
begin
  if This^.Newline then
    for Ct := 1 to This^.Indent do
      write(This^.Output, '  ');
  This^.Newline := false
end;

procedure _CgC_OutBegin(This : TCgC);
begin
  This^.IsMultiStatement := true;
  write(This^.Output, '{');
  _CgC_OutNewline(This);
  This^.Indent := This^.Indent + 1
end;

procedure _CgC_OutEnd(This : TCgC);
begin
  This^.Indent := This^.Indent - 1;
  _CgC_OutIndent(This);
  write(This^.Output, '}');
  _CgC_OutNewline(This)
end;

procedure _CgC_OutEndSameLine(This : TCgC);
begin
  This^.Indent := This^.Indent - 1;
  _CgC_OutIndent(This);
  write(This^.Output, '}')
end;

function _CgC_GetRangeType(TypePtr : TSDType) : string;
type 
  Types = (U8, S8, U16, S16, U32, S32);
const 
  TypeInfo : array[Types] of record
    Name : string;
    Low : integer;
    High : integer
  end 
  = (
    {U8=}(Name: 'PBits8'; Low: 0; High: 255),
    {S8=}(Name: 'PBits8S'; Low: -128; High: 127),
    {U16=}(Name: 'PBits16'; Low: 0; High: 65535),
    {S16=}(Name: 'PBits16S'; Low: -32768; High: 32767),
    {U32=}(Name: 'PBits32'; Low: 0; High: 2147483647 {Should be 4294967295}),
    {S32=}(Name: 'PBits32S'; Low: -2147483648; High: 2147483647)
    );
var 
  FitTypes : set of Types;
  Low, High : integer;
  T : Types;
begin
  FitTypes := [];
  Low := GetTypeLowBound(TypePtr);
  High := GetTypeHighBound(TypePtr);
  for T := U8 to S32 do
    if (Low >= TypeInfo[T].Low) and (High <= TypeInfo[T].High) then
      FitTypes := FitTypes + [T];
  Result := 'PInteger';
  for T := S32 downto U8 do
    if T in FitTypes then Result := TypeInfo[T].Name
end;

function _CgC_GetBinOpPrecedence(Expr : TSExpression) : integer;
var IsSetLeft, IsSetRight : boolean;
begin
  IsSetLeft := IsSetType(Expr^.Binary.Left^.TypePtr);
  IsSetRight := IsSetType(Expr^.Binary.Right^.TypePtr);
  case Expr^.Binary.Op of 
    SeoAdd: if IsStringyType(Expr^.TypePtr) then Result := 1
            else Result := 4;
    SeoSub: Result := 4;
    SeoMul: Result := 3;
    SeoDivReal: Result := 3;
    SeoDivInt: Result := 3;
    SeoMod: Result := 3;
    SeoAnd: if IsBooleanType(Expr^.TypePtr) then Result := 11
            else Result := 8;
    SeoOr: if IsBooleanType(Expr^.TypePtr) then Result := 12
           else Result := 10;
    SeoXor: if IsBooleanType(Expr^.TypePtr) then Result := 7
            else Result := 9;
    SeoShl: Result := 5;
    SeoShr: Result := 5;
    SeoIn: if ExIsImmediate(Expr^.Binary.Right) then Result := 12
           else Result := 1;
    SeoEq: if IsSetLeft and IsSetRight then Result := 1
           else Result := 7;
    SeoNe: if IsSetLeft and IsSetRight then Result := 1
           else Result := 7;
    SeoLt : Result := 6;
    SeoGt : Result := 6;
    SeoLtEq : if IsSetLeft and IsSetRight then Result := 1
              else Result := 6;
    SeoGtEq : if IsSetLeft and IsSetRight then Result := 1
              else Result := 6;
    else InternalError('Unknown precedence for operator in ' +
                       ExDescribe(Expr))
  end
end;

function _CgC_GetPrecedence(Expr : TSExpression) : integer;
begin
  case Expr^.Cls of 
    SecImmediate : Result := 0;
    SecArrayValue : Result := 1;
    SecRecordValue : Result := 1;
    SecSetValue : Result := 0;
    SecToString : Result := 0;
    SecToReal : Result := 2;
    SecToUntypedPtr : Result := _CgC_GetPrecedence(Expr^.ToUntypedPtrParent);
    SecToGenericFile : Result := _CgC_GetPrecedence(Expr^.ToGenericFileParent);
    SecWithTmpVar : Result := 0;
    SecSubrange : Result := 0;
    SecVariable : if Expr^.VarPtr^.IsReference then Result := 2
                  else Result := 0;
    SecField : Result := 1;
    SecArray : Result := 1;
    SecPointer : Result := 2;
    SecAddress : Result := 2;
    SecStringChar : Result := 1;
    SecFnRef : Result := 0;
    SecFnCall : Result := 1;
    SecPsfnRef : Result := 0;
    SecSizeof : Result := 1;
    SecConvertToStr : Result := 1;
    SecConvertToVal : Result := 1;
    SecRead : Result := 1;
    SecWrite : Result := 1;
    SecUnaryOp : Result := 2;
    SecBinaryOp : Result := _CgC_GetBinOpPrecedence(Expr);
    else InternalError('Unknown precedence for ' + ExDescribe(Expr))
  end
end;


procedure _CgC_OutSetTypeName(This : TCgC; TypePtr : TSDType);
var NumBytes : integer;
begin
  NumBytes := GetTypeHighBound(TypePtr^.ElementTypePtr) div 8 -
              GetTypeLowBound(TypePtr^.ElementTypePtr) div 8 + 1;
  write(This^.Output, 'PSet', 8 * NumBytes)
end;

procedure _CgC_OutVarArgDeclaration(This : TCgC;
                                    Name : string;
                                    IsReference, IsConstant : boolean;
                                    TypePtr : TSDType);
begin
  if IsConstant then
  begin
    if IsReference and IsPointerType(TypePtr) then
      Name := ' const ' + Name
    else
      write(This^.Output, 'const ')
  end;
  if IsReference then Name := '* ' + Name;
  _CgC_OutNameAndType(This, Name, TypePtr)
end;

procedure _CgC_OutVariableDeclaration(This : TCgC;
                                      VarDef : TSDVariable);
begin
  _CgC_OutVarArgDeclaration(This, VarDef^.Name, VarDef^.IsReference,
                            VarDef^.IsConstant, VarDef^.TypePtr)
end;

procedure _CgC_OutArgumentDeclaration(This : TCgC;
                                      const ArgDef : TSDSubroutineArg);
begin
  _CgC_OutVarArgDeclaration(This, ArgDef.Name, ArgDef.IsReference,
                            ArgDef.IsConstant, ArgDef.TypePtr)
end;

procedure _CgC_OutBounds(This : TCgC; TypePtr : TSDType);
begin
  write(This^.Output, GetTypeLowBound(TypePtr), ', ',
  GetTypeHighBound(TypePtr))
end;

procedure _CgC_OutExpressionParensPrec(This : TCgC;
                                       Expr : TSExpression;
                                       Prec : integer);
var UseParens : boolean;
begin
  UseParens := _CgC_GetPrecedence(Expr) > Prec;
  if UseParens then write(This^.Output, '(');
  _CgC_OutExpression(This, Expr);
  if UseParens then write(This^.Output, ')')
end;

procedure _CgC_OutExpressionParens(This : TCgC; Expr, Ref : TSExpression);
begin
  _CgC_OutExpressionParensPrec(This, Expr, _CgC_GetPrecedence(Ref))
end;

procedure _CgC_OutExpressionParensExtra(This : TCgC; Expr, Ref : TSExpression);
begin
  _CgC_OutExpressionParensPrec(This, Expr, _CgC_GetPrecedence(Ref) - 1)
end;

procedure _CgC_OutExEscapedChar(This : TCgC; Ch : char);
var Code, N1, N2 : integer;
begin
  Code := Ord(Ch);
  N1 := Code div 16;
  N2 := Code mod 16;
  write(This^.Output, '\x');
  if N1 < 10 then write(This^.Output, Chr(N1 + 48))
  else write(This^.Output, Chr(N1 + 87));
  if N2 < 10 then write(This^.Output, Chr(N2 + 48))
  else write(This^.Output, Chr(N2 + 87))
end;

procedure _CgC_OutExChar(This : TCgC; Chr : char);
begin
  write(This^.Output, '''');
  if Chr = '''' then write(This^.Output, '\''')
  else if Chr = '\' then write(This^.Output, '\\')
  else if Chr in [#32..#126] then write(This^.Output, Chr)
  else _CgC_OutExEscapedChar(This, Chr);
  write(This^.Output, '''')
end;

procedure _CgC_OutExCstring(This : TCgC; const Str : string);
var 
  Pos : integer;
  Ch : char;
begin
  write(This^.Output, '"');
  for Pos := 1 to Length(Str) do
  begin
    Ch := Str[Pos];
    if (Ch < ' ') or (Ch > #126) then
    begin
      _CgC_OutExEscapedChar(This, Ch);
      if (Pos < Length(Str)) and (Str[Pos + 1] in LxCharsHex) then
        write(This^.Output, '""')
    end
    else
    begin
      if Ch = '"' then write(This^.Output, '\"')
      else if Ch = '\' then write(This^.Output, '\\')
      else write(This^.Output, Ch)
    end
  end;
  write(This^.Output, '"')
end;

procedure _CgC_OutExString(This : TCgC; const Str : string);
begin
  if Length(Str) = 1 then
  begin
    write(This^.Output, 'str_of(');
    _CgC_OutExChar(This, Str[1]);
    write(This^.Output, ')')
  end
  else
  begin
    write(This^.Output, 'str_make(', Length(Str), ', ');
    _CgC_OutExCstring(This, Str);
    write(This^.Output, ')')
  end
end;

procedure _CgC_OutExSetImmediate(This : TCgC; Expr : TSExpression);
var 
  LowBound, HighBound, LowBoundByte, SetSize : integer;
  Bounds : TSESetImmBounds;
  ElemTypePtr : TSDType;
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
  write(This^.Output, '(PSet', SetSize * 8, ') { ');
  for Pos := 1 to SetSize do
  begin
    if Pos <> 1 then write(This^.Output, ', ');
    write(This^.Output, SetElems[Pos]);
  end;
  write(This^.Output, ' }');
end;

procedure _CgC_OutExImmediate(This : TCgC; Expr : TSExpression);
begin
  with Expr^.Immediate do
    case Cls of 
      SeicNil : write(This^.Output, 'PNil');
      SeicBoolean : if BooleanVal then write(This^.Output, '1')
                    else write(This^.Output, '0');
      SeicInteger : write(This^.Output, IntegerVal);
      SeicReal : write(This^.Output, RealVal);
      SeicChar : _CgC_OutExChar(This, CharVal);
      SeicString : _CgC_OutExString(This, StringVal);
      SeicEnum : write(This^.Output, EnumPtr^.Values[EnumOrdinal]);
      SeicSet : _CgC_OutExSetImmediate(This, Expr);
    end
end;

procedure _CgC_OutExArrayValue(This : TCgC; Expr : TSExpression);
var Elem : TSEArrayElem;
begin
  Elem := Expr^.ArrayElem;
  if Elem = nil then write(This^.Output, '{}')
  else
  begin
    write(This^.Output, '{ ');
    while Elem <> nil do
    begin
      _CgC_OutExpression(This, Elem^.Value);
      Elem := Elem^.Next;
      if Elem <> nil then write(This^.Output, ', ')
    end;
    write(This^.Output, ' }')
  end
end;

procedure _CgC_OutExRecordValue(This : TCgC; Expr : TSExpression);
var Field : TSERecordField;
begin
  write(This^.Output, '{ ');
  Field := Expr^.RecordField;
  if Field = nil then write(This^.Output, '}')
  else
  begin
    while Field <> nil do
    begin
      write(This^.Output, '.', Expr^.TypePtr^.RecPtr^.Fields[Field^.Ordinal].
            Name, ' = ');
      _CgC_OutExpression(This, Field^.Value);
      Field := Field^.Next;
      if Field <> nil then write(This^.Output, ', ')
    end;
    write(This^.Output, ' }')
  end
end;

procedure _CgC_OutExSetValue(This : TCgC; Expr : TSExpression);
var 
  ElementTypePtr : TSDType;
  Bounds : TSESetExprBounds;
  First, Last : TSExpression;
  LowBoundByte : integer;
begin
  ElementTypePtr := Expr^.TypePtr^.ElementTypePtr;
  LowBoundByte := GetTypeLowBound(ElementTypePtr) div 8;
  write(This^.Output, '({ ');
  _CgC_OutSetTypeName(This, Expr^.TypePtr);
  write(This^.Output, ' dst = ');
  _CgC_OutExSetImmediate(This, Expr^.SetBase);
  write(This^.Output, '; ');
  Bounds := Expr^.SetBounds;
  while Bounds <> nil do
  begin
    write(This^.Output, 'set_set(');
    First := ExOpOrd(ExCoerce(ExCopy(Bounds^.First), ElementTypePtr));
    if Bounds^.Last = nil then Last := ExCopy(First)
    else Last := ExOpOrd(ExCoerce(ExCopy(Bounds^.Last), ElementTypePtr));
    _CgC_OutExpression(This, First);
    write(This^.Output, ', ');
    _CgC_OutExpression(This, Last);
    write(This^.Output, ', ', LowBoundByte, ', dst.bits); ');
    ExDispose(First);
    ExDispose(Last);
    Bounds := Bounds^.Next
  end;
  write(This^.Output, 'dst; })')
end;

procedure _CgC_OutExWithTmpVar(This : TCgC; Expr : TSExpression);
begin
  write(This^.Output, '({ ');
  while Expr^.Cls = SecWithTmpVar do
  begin
    _CgC_OutVariableDeclaration(This, Expr^.TmpVar^.VarPtr);
    write(This^.Output, ' = ');
    _CgC_OutExpression(This, Expr^.TmpVarValue);
    write(This^.Output, '; ');
    Expr := Expr^.TmpVarChild
  end;
  _CgC_OutExpression(This, Expr);
  write(This^.Output, '; })')
end;

procedure _CgC_OutExSubrange(This : TCgC; Expr : TSExpression);
begin
  if not Options.CheckBounds then
    _CgC_OutExpression(This, Expr^.SubrangeParent)
  else
  begin
    write(This^.Output, 'subrange(');
    _CgC_OutExpression(This, Expr^.SubrangeParent);
    write(This^.Output, ', ');
    _CgC_OutBounds(This, Expr^.TypePtr);
    write(This^.Output, ')')
  end
end;

procedure _CgC_OutExVariable(This : TCgC; Expr : TSExpression);
begin
  if Expr^.VarPtr^.IsReference then
    write(This^.Output, '*', Expr^.VarPtr^.Name)
  else
    write(This^.Output, Expr^.VarPtr^.Name)
end;

procedure _CgC_OutExFieldAccess(This : TCgC; Expr : TSExpression);
begin
  with Expr^.RecExpr^ do
  begin
    if Cls = SecPointer then
    begin
      _CgC_OutExpressionParens(This, PointerExpr, Expr);
      write(This^.Output, '->')
    end
    else if (Cls = SecVariable) and VarPtr^.IsReference then
           write(This^.Output, VarPtr^.Name, '->')
    else
    begin
      _CgC_OutExpressionParens(This, Expr^.RecExpr, Expr);
      write(This^.Output, '.')
    end;
    write(This^.Output, Expr^.RecExpr^.TypePtr^.RecPtr^
          .Fields[Expr^.RecFieldNum].Name)
  end
end;

procedure _CgC_OutExArrayIndex(This : TCgC;
                               Index : TSExpression; TypePtr : TSDType);
var LowBound : integer;
  Size : TSExpression;
begin
  LowBound := GetTypeLowBound(TypePtr^.ArrayDef.IndexTypePtr);
  Size := ExOpSub(ExOpOrd(ExCopy(Index)), ExIntegerConstant(LowBound));
  _CgC_OutExpression(This, Size);
  ExDispose(Size)
end;

procedure _CgC_OutExAddress(This : TCgC; Expr : TSExpression);
begin
  if Expr^.Cls = SecToGenericFile then
    _CgC_OutExAddress(This, Expr^.ToGenericFileParent)
  else if Expr^.Cls = SecToReal then
         _CgC_OutExAddress(This, Expr^.ToRealParent)
  else if Expr^.Cls = SecToString then
         _CgC_OutExAddress(This, Expr^.ToStrParent)
  else if Expr^.Cls = SecPointer then
         _CgC_OutExpression(This, Expr^.PointerExpr)
  else if Expr^.Cls = SecToUntypedPtr then
  begin
    write(This^.Output, '(void**)&');
    _CgC_OutExpressionParensPrec(This, Expr, 1)
  end
  else if (Expr^.Cls = SecVariable) and (Expr^.VarPtr^.IsReference) then
         write(This^.Output, Expr^.VarPtr^.Name)
  else
  begin
    write(This^.Output, '&');
    _CgC_OutExpressionParensPrec(This, Expr, 1)
  end
end;

procedure _CgC_OutExStringChar(This : TCgC; Expr : TSExpression);
begin
  with Expr^.StringExpr^ do
  begin
    if Cls = SecPointer then
    begin
      _CgC_OutExpressionParens(This, PointerExpr, Expr);
      write(This^.Output, '->chr[')
    end
    else if (Cls = SecVariable) and VarPtr^.IsReference then
           write(This^.Output, VarPtr^.Name, '->chr[')
    else
    begin
      _CgC_OutExpressionParens(This, Expr^.StringExpr, Expr);
      write(This^.Output, '.chr[')
    end;
    _CgC_OutExpression(This, Expr^.StringIndex);
    write(This^.Output, ']')
  end
end;

procedure _CgC_OutExFunctionCallArgs(This : TCgC;
                                     const ArgDefs : TSDSubroutineArgs;
                                     const ArgValues : TSEFunctionArgs);
var Pos : integer;
begin
  write(This^.Output, '(');
  for Pos := 1 to ArgValues.Size do
  begin
    if Pos <> 1 then write(This^.Output, ', ');
    if ArgDefs.Defs[Pos].IsReference then
    begin
      EnsureAddressableExpr(ArgValues.Values[Pos]);
      _CgC_OutExAddress(This, ArgValues.Values[Pos])
    end
    else
      _CgC_OutExpression(This, ArgValues.Values[Pos])
  end;
  write(This^.Output, ')')
end;

procedure _CgC_OutExFunctionCall(This : TCgC;Expr : TSExpression);
begin
  _CgC_OutExpressionParens(This, Expr^.FnExpr, Expr);
  if Expr^.FnExpr^.Cls = SecFnRef then
    _CgC_OutExFunctionCallArgs(This, Expr^.FnExpr^.FnPtr^.Args, Expr^.CallArgs)
  else
    _CgC_OutExFunctionCallArgs(This, Expr^.FnExpr^.TypePtr^.FnDefPtr^.Args,
                               Expr^.CallArgs)
end;

procedure _CgC_OutExSizeof(This : TCgC; Expr : TSExpression);
begin
  write(This^.Output, 'sizeof(');
  _CgC_OutTypeReference(This, Expr^.SizeofTypePtr);
  write(This^.Output, ')')
end;

function _CgC_ShortTypeName(TypePtr : TSDType) : char;
begin
  if IsBooleanType(TypePtr) then _CgC_ShortTypeName := 'b'
  else if IsIntegerType(TypePtr) then _CgC_ShortTypeName := 'i'
  else if IsRealType(TypePtr) then _CgC_ShortTypeName := 'r'
  else if IsCharType(TypePtr) then _CgC_ShortTypeName := 'c'
  else if IsStringType(TypePtr) then _CgC_ShortTypeName := 's'
  else CompileError('Type ' + TypeName(TypePtr) + ' is not representable for ' +
    'READ, WRITE, STR, or VAL')
end;

procedure _CgC_OutExConvertToStr(This : TCgC; Expr : TSExpression);
var Src, Dst, Width, Prec : TSExpression;
begin
  Src := Expr^.ToStrSrc.Arg;
  Dst := Expr^.ToStrDest;
  Width := Expr^.ToStrSrc.Width;
  Prec := Expr^.ToStrSrc.Prec;
  if IsEnumType(Src^.TypePtr) then
  begin
    _CgC_OutIndent(This);
    write(This^.Output, 'STR_e(');
    _CgC_OutExpression(This, Src);
    write(This^.Output, ', enumvalues', Src^.TypePtr^.EnumPtr^.Id, ', ');
    if Width <> nil then _CgC_OutExpression(This, Width)
    else write(This^.Output, '0');
    write(This^.Output, ', ');
    _CgC_OutExAddress(This, Dst);
    write(This^.Output, ')')
  end
  else if IsRealType(Src^.TypePtr) then
  begin
    _CgC_OutIndent(This);
    write(This^.Output, 'STR_r(');
    _CgC_OutExpression(This, Src);
    write(This^.Output, ', ');
    if Width <> nil then _CgC_OutExpression(This, Width)
    else write(This^.Output, '0');
    write(This^.Output, ', ');
    if Prec <> nil then _CgC_OutExpression(This, Prec)
    else write(This^.Output, '-1');
    write(This^.Output, ', ');
    _CgC_OutExAddress(This, Dst);
    write(This^.Output, ')')
  end
  else
  begin
    _CgC_OutIndent(This);
    write(This^.Output, 'STR_', _CgC_ShortTypeName(Src^.TypePtr), '(');
    _CgC_OutExpression(This, Src);
    write(This^.Output, ', ');
    if Width <> nil then _CgC_OutExpression(This, Width)
    else write(This^.Output, '0');
    write(This^.Output, ', ');
    _CgC_OutExAddress(This, Dst);
    write(This^.Output, ')')
  end
end;

procedure _CgC_OutExConvertToVal(This : TCgC; Expr : TSExpression);
var Src, Dst, Code, TmpExpr : TSExpression;
begin
  Src := Expr^.ToValSrc;
  Dst := Expr^.ToValDest;
  Code := Expr^.ToValCode;
  if IsEnumType(Dst^.TypePtr) then
  begin
    _CgC_OutIndent(This);
    write(This^.Output, 'VAL_e(');
    _CgC_OutExAddress(This, Src);
    write(This^.Output, ', ');
    _CgC_OutExAddress(This, Dst);
    write(This^.Output, ', ');
    TmpExpr := ExIntegerConstant(Dst^.TypePtr^.EnumPtr^.Size);
    _CgC_OutExpression(This, TmpExpr);
    ExDispose(TmpExpr);
    write(This^.Output, ', enumvalues', Dst^.TypePtr^.EnumPtr^.Id, ', ');
    _CgC_OutExAddress(This, Code);
    write(This^.Output, ')')
  end
  else
  begin
    _CgC_OutIndent(This);
    write(This^.Output, 'VAL_', _CgC_ShortTypeName(Dst^.TypePtr), '(');
    _CgC_OutExAddress(This, Src);
    write(This^.Output, ', ');
    _CgC_OutExAddress(This, Dst);
    write(This^.Output, ', ');
    _CgC_OutExAddress(This, Code);
    write(This^.Output, ')')
  end
end;

procedure _CgC_OutExRead(This : TCgC; Expr : TSExpression);
var 
  Src : TSExpression;
  ReadArg : TSEReadArgList;
  Linefeed : boolean;
  TypePtr : TSDType;
begin
  Src := Expr^.ReadFile;
  Linefeed := Expr^.Readln;
  ReadArg := Expr^.ReadArgs;
  _CgC_OutIndent(This);
  write(This^.Output, 'Read(');
  _CgC_OutExAddress(This, Src);
  write(This^.Output, ', ');
  _CgC_OutExpression(This, ExBooleanConstant(Options.CheckIoResult));
  if ReadArg = nil then
  begin
    write(This^.Output, ', RwpEnd');
    if Linefeed then write(This^.Output, ' | RwpLn')
  end;
  while ReadArg <> nil do
  begin
    if IsTextType(Src^.TypePtr) then
    begin
      TypePtr := GetFundamentalType(ReadArg^.Dest^.TypePtr);
      case TypePtr^.Cls of 
        SdtcInteger: write(This^.Output, ', RwpInt');
        SdtcReal: write(This^.Output, ', RwpReal');
        SdtcChar: write(This^.Output, ', RwpChar');
        SdtcString: write(This^.Output, ', RwpString');
        else ErrorForExpr('Expression has invalid type for READ', ReadArg^.Dest)
      end;
      if ReadArg^.Next = nil then
      begin
        if Linefeed then write(This^.Output, ' | RwpLn');
        write(This^.Output, ' | RwpEnd')
      end;
      write(This^.Output, ', ');
      _CgC_OutExAddress(This, ReadArg^.Dest);
    end
    else
    begin
      write(This^.Output, ', RwpDataPtr');
      if ReadArg^.Next = nil then write(This^.Output, ' | RwpEnd');
      write(This^.Output, ', ');
      _CgC_OutExAddress(This, ReadArg^.Dest)
    end;
    ReadArg := ReadArg^.Next
  end;
  write(This^.Output, ')')
end;

procedure _CgC_OutExWrite(This : TCgC; Expr : TSExpression);
var 
  Dst : TSExpression;
  WriteArg : TSEWriteArgList;
  Linefeed : boolean;
  TypePtr : TSDType;
begin
  Dst := Expr^.WriteFile;
  Linefeed := Expr^.WriteLn;
  WriteArg := Expr^.WriteArgs;
  _CgC_OutIndent(This);
  write(This^.Output, 'Write(');
  _CgC_OutExAddress(This, Dst);
  write(This^.Output, ', ');
  _CgC_OutExpression(This, ExBooleanConstant(Options.CheckIoResult));
  if WriteArg = nil then
  begin
    write(This^.Output, ', RwpEnd');
    if Linefeed then write(This^.Output, ' | RwpLn')
  end;
  while WriteArg <> nil do
  begin
    if IsTextType(Dst^.TypePtr) then
    begin
      TypePtr := GetFundamentalType(WriteArg^.Value.Arg^.TypePtr);
      case TypePtr^.Cls of 
        SdtcBoolean: write(This^.Output, ', RwpBool');
        SdtcInteger: write(This^.Output, ', RwpInt');
        SdtcReal: write(This^.Output, ', RwpReal');
        SdtcChar: write(This^.Output, ', RwpChar');
        SdtcEnum: write(This^.Output, ', RwpEnum');
        SdtcString:
                    begin
                      if ExIsImmediate(WriteArg^.Value.Arg) then
                        write(This^.Output, ', RwpLenPtr')
                      else if WriteArg^.Value.Arg^.IsAddressable then
                             write(This^.Output, ', RwpStringPtr')
                      else
                        write(This^.Output, ', RwpString')
                    end;
        else ErrorForExpr('Expression has invalid type for WRITE',
                          WriteArg^.Value.Arg)
      end;
      if WriteArg^.Value.Width <> nil then write(This^.Output, ' | RwpWidth');
      if IsRealType(TypePtr) and (WriteArg^.Value.Prec <> nil) then
        write(This^.Output, ' | RwpPrec');
      if WriteArg^.Next = nil then
      begin
        if Linefeed then write(This^.Output, ' | RwpLn');
        write(This^.Output, ' | RwpEnd')
      end;
      if WriteArg^.Value.Width <> nil then
      begin
        write(This^.Output, ', ');
        _CgC_OutExpression(This, WriteArg^.Value.Width)
      end;
      if WriteArg^.Value.Prec <> nil then
      begin
        write(This^.Output, ', ');
        _CgC_OutExpression(This, WriteArg^.Value.Prec)
      end;
      write(This^.Output, ', ');
      if IsStringType(WriteArg^.Value.Arg^.TypePtr)
         and ExIsImmediate(WriteArg^.Value.Arg) then
      begin
        write(This^.Output, Length(WriteArg^.Value.Arg^.Immediate.StringVal));
        write(This^.Output, ', ');
        _CgC_OutExCstring(This, WriteArg^.Value.Arg^.Immediate.StringVal)
      end
      else if IsStringType(WriteArg^.Value.Arg^.TypePtr)
              and WriteArg^.Value.Arg^.IsAddressable then
             _CgC_OutExAddress(This, WriteArg^.Value.Arg)
      else
        _CgC_OutExpression(This, WriteArg^.Value.Arg);
      if IsEnumType(TypePtr) then
        write(This^.Output, ', enumvalues', TypePtr^.EnumPtr^.Id);
    end
    else
    begin
      write(This^.Output, ', RwpDataPtr');
      if WriteArg^.Next = nil then write(This^.Output, ' | RwpEnd');
      write(This^.Output, ', ');
      _CgC_OutExAddress(This, WriteArg^.Value.Arg)
    end;
    WriteArg := WriteArg^.Next
  end;
  write(This^.Output, ')')
end;

procedure _CgC_OutOrd(This : TCgC; Expr : TSExpression);
begin
  EnsureOrdinalExpr(Expr^.Unary.Parent);
  if IsCharType(Expr^.Unary.Parent^.TypePtr) then
  begin
    write(This^.Output, '(int)');
    _CgC_OutExpressionParensPrec(This, Expr^.Unary.Parent, 2)
  end
  else _CgC_OutExpression(This, Expr^.Unary.Parent)
end;

procedure _CgC_OutPred(This : TCgC; Expr : TSExpression);
var TmpExpr : TSExpression;
begin
  EnsureOrdinalExpr(Expr^.Unary.Parent);
  if IsBoundedType(Expr^.Unary.Parent^.TypePtr) then
  begin
    if Options.CheckBounds then
    begin
      write(This^.Output, 'pred(');
      _CgC_OutExpression(This, Expr^.Unary.Parent);
      write(This^.Output, ', ');
      _CgC_OutBounds(This, Expr^.Unary.Parent^.TypePtr);
      write(This^.Output, ')')
    end
    else
    begin
      TmpExpr := ExOpSub(ExOpOrd(ExCopy(Expr^.Unary.Parent)),
                 ExIntegerConstant(1));
      _CgC_OutExpression(This, TmpExpr);
      ExDispose(TmpExpr)
    end
  end
  else
  begin
    TmpExpr := ExOpSub(ExCopy(Expr^.Unary.Parent),
               ExIntegerConstant(1));
    _CgC_OutExpression(This, TmpExpr);
    ExDispose(TmpExpr)
  end
end;

procedure _CgC_OutSucc(This : TCgC; Expr : TSExpression);
var TmpExpr : TSExpression;
begin
  EnsureOrdinalExpr(Expr^.Unary.Parent);
  if IsBoundedType(Expr^.Unary.Parent^.TypePtr) then
  begin
    if Options.CheckBounds then
    begin
      write(This^.Output, 'succ(');
      _CgC_OutExpression(This, Expr^.Unary.Parent);
      write(This^.Output, ', ');
      _CgC_OutBounds(This, Expr^.Unary.Parent^.TypePtr);
      write(This^.Output, ')')
    end
    else
    begin
      TmpExpr := ExOpAdd(ExOpOrd(ExCopy(Expr^.Unary.Parent)),
                 ExIntegerConstant(1));
      _CgC_OutExpression(This, TmpExpr);
      ExDispose(TmpExpr)
    end
  end
  else
  begin
    TmpExpr := ExOpAdd(ExCopy(Expr^.Unary.Parent), ExIntegerConstant(1));
    _CgC_OutExpression(This, TmpExpr);
    ExDispose(TmpExpr)
  end
end;

procedure _CgC_OutExUnaryOp(This : TCgC; Expr : TSExpression);
begin
  if Expr^.Unary.Op = SeoOrd then _CgC_OutOrd(This, Expr)
  else if Expr^.Unary.Op = SeoPred then _CgC_OutPred(This, Expr)
  else if Expr^.Unary.Op = SeoSucc then _CgC_OutSucc(This, Expr)
  else if Expr^.Unary.Op = SeoNeg then
  begin
    write(This^.Output, '-');
    _CgC_OutExpressionParens(This, Expr^.Unary.Parent, Expr)
  end
  else if (Expr^.Unary.Op = SeoNot) and IsBooleanType(Expr^.TypePtr) then
  begin
    write(This^.Output, '!');
    _CgC_OutExpressionParens(This, Expr^.Unary.Parent, Expr)
  end
  else if (Expr^.Unary.Op = SeoNot) and IsIntegerType(Expr^.TypePtr) then
  begin
    write(This^.Output, '~');
    _CgC_OutExpressionParens(This, Expr^.Unary.Parent, Expr)
  end
end;

procedure _CgC_OutExCmpConcatArg(This : TCgC; Expr : TSExpression);
begin
  if ExIsImmediate(Expr) and IsStringType(Expr^.TypePtr) then
  begin
    write(This^.Output, 'CpLenPtr, ', Length(Expr^.Immediate.StringVal), ', ');
    _CgC_OutExCstring(This, Expr^.Immediate.StringVal)
  end
  else if IsCharType(Expr^.TypePtr) then
  begin
    write(This^.Output, 'CpChar, ');
    _CgC_OutExpression(This, Expr)
  end
  else if Expr^.IsAddressable then
  begin
    write(This^.Output, 'CpStringPtr, ');
    _CgC_OutExAddress(This, Expr)
  end
  else
  begin
    write(This^.Output, 'CpString, ');
    _CgC_OutExpression(This, Expr)
  end
end;

procedure _CgC_OutExConcatArgs(This : TCgC;
                               Expr : TSExpression; Last : boolean);
begin
  if not IsStringyType(Expr^.TypePtr) then
    InternalError('Expected a stringy type for ' + ExDescribe(Expr))
  else if (Expr^.Cls <> SecBinaryOp) or (Expr^.Binary.Op <> SeoAdd) then
  begin
    if Last then write(This^.Output, 'CpEnd | ');
    _CgC_OutExCmpConcatArg(This, Expr);
    if not Last then write(This^.Output, ', ')
  end
  else
  begin
    _CgC_OutExConcatArgs(This, Expr^.Binary.Left, {Last=}false);
    _CgC_OutExConcatArgs(This, Expr^.Binary.Right, Last)
  end
end;

procedure _CgC_OutExSetOperation(This : TCgC;
                                 Left, Right: TSExpression; Op : TSEOperator);
var 
  ElemTypePtr : TSDType;
  LowBound, HighBound, LowBoundByte, SetSize : integer;
begin
  ElemTypePtr := Right^.TypePtr^.ElementTypePtr;
  if Op = SeoLtEq then _CgC_OutExSetOperation(This, Right, Left, SeoGtEq)
  else if Op = SeoNe then
  begin
    write(This^.Output, '!');
    _CgC_OutExSetOperation(This, Left, Right, SeoEq)
  end
  else if Op = SeoIn then
  begin
    LowBoundByte := GetTypeLowBound(ElemTypePtr) div 8;
    write(This^.Output, 'set_in(');
    _CgC_OutExpression(This, Left);
    write(This^.Output, ', ', LowBoundByte, ', ');
    _CgC_OutExpressionParensPrec(This, Right, 1);
    write(This^.Output, '.bits)')
  end
  else
  begin
    LowBound := GetTypeLowBound(ElemTypePtr);
    HighBound := GetTypeHighBound(ElemTypePtr);
    SetSize := HighBound div 8 - LowBound div 8 + 1;
    if Op = SeoEq then
    begin
      write(This^.Output, 'set_equals(');
      _CgC_OutExpressionParensPrec(This, Left, 1);
      write(This^.Output, '.bits, ');
      _CgC_OutExpressionParensPrec(This, Right, 1);
      write(This^.Output, '.bits, ', SetSize, ')')
    end
    else if Op = SeoGtEq then
    begin
      write(This^.Output, 'set_issuperset(');
      _CgC_OutExpressionParensPrec(This, Left, 1);
      write(This^.Output, '.bits, ');
      _CgC_OutExpressionParensPrec(This, Right, 1);
      write(This^.Output, '.bits, ', SetSize, ')')
    end
    else
    begin
      write(This^.Output, '({ PSet', SetSize * 8, ' dst; ');
      case Op of 
        SeoAdd: write(This^.Output, 'set_union(');
        SeoSub: write(This^.Output, 'set_difference(');
        SeoMul: write(This^.Output, 'set_intersection(');
        else
          InternalError('Materialized set operation not implemented: ' +
                        ExDescribeOperator(Op))
      end;
      _CgC_OutExpressionParensPrec(This, Left, 1);
      write(This^.Output, '.bits, ');
      _CgC_OutExpressionParensPrec(This, Right, 1);
      write(This^.Output, '.bits, dst.bits, ', SetSize, '); dst; })')
    end
  end
end;

function _CgC_IsArithmeticOp(Op : TSEOperator) : boolean;
begin
  _CgC_IsArithmeticOp := Op in [SeoAdd, SeoSub, SeoMul, SeoDivReal, SeoDivInt,
                         SeoMod]
end;

function _CgC_GetArithmeticOp(Op : TSEOperator) : string;
begin
  case Op of 
    SeoAdd : _CgC_GetArithmeticOp := '+';
    SeoSub : _CgC_GetArithmeticOp := '-';
    SeoMul : _CgC_GetArithmeticOp := '*';
    SeoDivReal : _CgC_GetArithmeticOp := '/';
    SeoDivInt : _CgC_GetArithmeticOp := '/';
    SeoMod : _CgC_GetArithmeticOp := '%';
  end
end;

function _CgC_IsLogicalOrBitwiseOp(Op : TSEOperator) : boolean;
begin
  _CgC_IsLogicalOrBitwiseOp := Op in [SeoAnd, SeoOr, SeoXor]
end;

function _CgC_IsBitwiseOp(Op : TSEOperator) : boolean;
begin
  _CgC_IsBitwiseOp := Op in [SeoShl, SeoShr]
end;

function _CgC_GetLogicalOp(Op : TSEOperator) : string;
begin
  case Op of 
    SeoAnd: Result := '&&';
    SeoOr: Result := '||';
    SeoXor: Result := '!=';
    else InternalError('Unimplemented logical operator ' +
                       ExDescribeOperator(Op))
  end
end;

function _CgC_GetBitwiseOp(Op : TSEOperator) : string;
begin
  case Op of 
    SeoAnd: Result := '&';
    SeoOr: Result := '|';
    SeoXor: Result := '^';
    SeoShl: Result := '<<';
    SeoShr: Result := '>>';
    else InternalError('Unimplemented bitwise operator ' +
                       ExDescribeOperator(Op))
  end
end;

function _CgC_IsRelationalOp(Op : TSEOperator) : boolean;
begin
  _CgC_IsRelationalOp := Op in [SeoEq, SeoNe, SeoLt, SeoGt, SeoLtEq, SeoGtEq]
end;

function _CgC_GetRelationalOp(Op : TSEOperator) : string;
begin
  case Op of 
    SeoEq: _CgC_GetRelationalOp := '==';
    SeoNe : _CgC_GetRelationalOp := '!=';
    SeoLt : _CgC_GetRelationalOp := '<';
    SeoGt : _CgC_GetRelationalOp := '>';
    SeoLtEq : _CgC_GetRelationalOp := '<=';
    SeoGtEq : _CgC_GetRelationalOp := '>='
  end
end;

procedure _CgC_OutExBinaryOp(This : TCgC; Expr : TSExpression);
begin
  with Expr^.Binary do
  begin
    if IsBooleanType(Left^.TypePtr) and IsBooleanType(Right^.TypePtr) then
    begin
      _CgC_OutExpressionParens(This, Left, Expr);
      if _CgC_IsLogicalOrBitwiseOp(Op) then
        write(This^.Output, ' ', _CgC_GetLogicalOp(Op), ' ')
      else if _CgC_IsRelationalOp(Op) then
             write(This^.Output, ' ' , _CgC_GetRelationalOp(Op), ' ')
      else ErrorInvalidOperator(Expr, Op);
      _CgC_OutExpressionParensExtra(This, Right, Expr)
    end
    else if IsNumericType(Left^.TypePtr) and IsNumericType(Right^.TypePtr) then
    begin
      _CgC_OutExpressionParens(This, Left, Expr);
      if _CgC_IsArithmeticOp(Op) then
        write(This^.Output, ' ', _CgC_GetArithmeticOp(Op), ' ')
      else if _CgC_IsLogicalOrBitwiseOp(Op) or _CgC_IsBitwiseOp(Op) then
             write(This^.Output, ' ', _CgC_GetBitwiseOp(Op), ' ')
      else if _CgC_IsRelationalOp(Op) then
             write(This^.Output, ' ' , _CgC_GetRelationalOp(Op), ' ')
      else ErrorInvalidOperator(Expr, Op);
      _CgC_OutExpressionParensExtra(This, Right, Expr)
    end
    else if IsStringyType(Left^.TypePtr) and IsStringyType(Right^.TypePtr) then
    begin
      if Op = SeoAdd then
      begin
        write(This^.Output, 'CONCAT(');
        _CgC_OutExConcatArgs(This, Expr, {Last=}true);
        write(This^.Output, ')')
      end
      else if IsCharType(Left^.TypePtr) and IsCharType(Right^.TypePtr) then
      begin
        _CgC_OutExpressionParens(This, Left, Expr);
        if _CgC_IsRelationalOp(Op) then
          write(This^.Output, ' ', _CgC_GetRelationalOp(Op), ' ')
        else ErrorInvalidOperator(Expr, Op);
        _CgC_OutExpressionParensExtra(This, Right, Expr)
      end
      else
      begin
        write(This^.Output, 'cmp_str(');
        case Op of 
          SeoEq: write(This^.Output, 'CoEq, ');
          SeoNe: write(This^.Output, 'CoNotEq, ');
          SeoLt: write(This^.Output, 'CoBefore, ');
          SeoGt: write(This^.Output, 'CoAfter, ');
          SeoLtEq: write(This^.Output, 'CoBeforeOrEq, ');
          SeoGtEq: write(This^.Output, 'CoAfterOrEq, ');
          else ErrorInvalidOperator(Expr, Op)
        end;
        _CgC_OutExCmpConcatArg(This, Left);
        write(This^.Output, ', ');
        _CgC_OutExCmpConcatArg(This, Right);
        write(This^.Output, ')')
      end
    end
    else if IsSetType(Right^.TypePtr) then
           _CgC_OutExSetOperation(This, Left, Right, Op)
    else
    begin
      _CgC_OutExpressionParens(This, Left, Expr);
      if _CgC_IsRelationalOp(Op) then
        write(This^.Output, ' ', _CgC_GetRelationalOp(Op), ' ')
      else ErrorInvalidOperator(Expr, Op);
      _CgC_OutExpressionParensExtra(This, Right, Expr)
    end
  end
end;

procedure _CgC_OutExpression(This : TCgC; Expr : TSExpression);
begin
  case Expr^.Cls of 
    SecImmediate: _CgC_OutExImmediate(This, Expr);
    SecArrayValue: _CgC_OutExArrayValue(This, Expr);
    SecRecordValue: _CgC_OutExRecordValue(This, Expr);
    SecSetValue: _CgC_OutExSetValue(This, Expr);
    SecToString:
                 begin
                   write(This^.Output, 'str_of(');
                   _CgC_OutExpression(This, Expr^.ToStrParent);
                   write(This^.Output, ')')
                 end;
    SecToReal:
               begin
                 write(This^.Output, '(double)');
                 _CgC_OutExpression(This, Expr^.ToRealParent)
               end;
    SecToUntypedPtr: _CgC_OutExpression(This, Expr^.ToUntypedPtrParent);
    SecToGenericFile: _CgC_OutExpression(This, Expr^.ToGenericFileParent);
    SecWithTmpVar: _CgC_OutExWithTmpVar(This, Expr);
    SecSubrange: _CgC_OutExSubrange(This, Expr);
    SecVariable : _CgC_OutExVariable(This, Expr);
    SecField: _CgC_OutExFieldAccess(This, Expr);
    SecArray:
              begin
                _CgC_OutExpressionParens(This, Expr^.ArrayExpr, Expr);
                write(This^.Output, '[');
                _CgC_OutExArrayIndex(This, Expr^.ArrayIndex,
                                     Expr^.ArrayExpr^.TypePtr);
                write(This^.Output, ']')
              end;
    SecPointer:
                begin
                  write(This^.Output, '*');
                  _CgC_OutExpressionParens(This, Expr^.PointerExpr, Expr)
                end;
    SecAddress: _CgC_OutExAddress(This, Expr^.AddressExpr);
    SecStringChar: _CgC_OutExStringChar(This, Expr);
    SecFnRef: write(This^.Output, Expr^.FnPtr^.ExternalName);
    SecFnCall: _CgC_OutExFunctionCall(This, Expr);
    SecSizeof: _CgC_OutExSizeof(This, Expr);
    SecConvertToStr: _CgC_OutExConvertToStr(This, Expr);
    SecConvertToVal: _CgC_OutExConvertToVal(This, Expr);
    SecRead: _CgC_OutExRead(This, Expr);
    SecWrite: _CgC_OutExWrite(This, Expr);
    SecUnaryOp: _CgC_OutExUnaryOp(This, Expr);
    SecBinaryOp: _CgC_OutExBinaryOp(This, Expr)
  end
end;

procedure _CgC_OutNameAndEnum(This : TCgC;
                              const Name : string; EnumPtr : TSDTEnum);
var 
  Pos : integer;
begin
  if EnumPtr^.HasBeenDefined then
    write(This^.Output, 'enum enum', EnumPtr^.Id)
  else
  begin
    write(This^.Output, 'enum __attribute__((__packed__)) enum', EnumPtr^.Id,
          ' { ');
    for Pos := 0 to EnumPtr^.Size - 1 do
    begin
      if Pos > 0 then write(This^.Output, ', ');
      write(This^.Output, EnumPtr^.Values[Pos])
    end;
    write(This^.Output, ' }');
    EnumPtr^.HasBeenDefined := true
  end;
  write(This^.Output, ' ', Name)
end;

procedure _CgC_OutNameAndRecord(This : TCgC;
                                const Name : string;
                                RecPtr : TSDTRecord);
var 
  Pos : integer;
  NumVariant : integer;
begin
  NumVariant := 0;
  if RecPtr^.HasBeenDefined then
    write(This^.Output, 'struct record', RecPtr^.Id)
  else
  begin
    write(This^.Output, 'struct ');
    if RecPtr^.IsPacked then
      write(This^.Output, '__attribute__((__packed__)) ');
    write(This^.Output, 'record', RecPtr^.Id, ' ');
    _CgC_OutBegin(This);
    for Pos := 1 to RecPtr^.Size do
    begin
      if (RecPtr^.NumVariants > NumVariant)
         and (RecPtr^.VariantBounds[NumVariant + 1] = Pos) then
      begin
        NumVariant := NumVariant + 1;
        if NumVariant = 1 then
        begin
          _CgC_OutIndent(This);
          write(This^.Output, 'union ');
          _CgC_OutBegin(This)
        end
        else
        begin
          _CgC_OutEndSameLine(This);
          write(This^.Output, ';');
          _CgC_OutNewline(This)
        end;
        _CgC_OutIndent(This);
        write(This^.Output, 'struct ');
        _CgC_OutBegin(This);
      end;
      _CgC_OutIndent(This);
      _CgC_OutNameAndType(This, RecPtr^.Fields[Pos].Name,
                          RecPtr^.Fields[Pos].TypePtr);
      write(This^.Output, ';');
      _CgC_OutNewline(This);
    end;
    if NumVariant > 0 then
    begin
      _CgC_OutEndSameLine(This);
      write(This^.Output, ';');
      _CgC_OutNewline(This);
      _CgC_OutEndSameLine(This);
      write(This^.Output, ';');
      _CgC_OutNewline(This)
    end;
    _CgC_OutEndSameLine(This);
    RecPtr^.HasBeenDefined := true
  end;
  if Name[1] <> '*' then write(This^.Output, ' ');
  write(This^.Output, Name)
end;

procedure _CgC_OutNameAndArray(This : TCgC;
                               const Name : string;
                               TypePtr : TSDType);
var TheType : TSDType;
begin
  TheType := TypePtr;
  while IsArrayType(TheType) do
    TheType := TheType^.ArrayDef.ValueTypePtr;
  _CgC_OutNameAndType(This, Name, TheType);
  TheType := TypePtr;
  while IsArrayType(TheType) do
  begin
    write(This^.Output, '[',
          GetBoundedTypeSize(TheType^.ArrayDef.IndexTypePtr), ']');
    TheType := TheType^.ArrayDef.ValueTypePtr
  end
end;

procedure _CgC_OutNameAndFunction(This : TCgC;
                                  const Name : string;
                                  TypePtr : TSDType);
var Pos : integer;
begin
  _CgC_OutNameAndType(This, '(*' + Name + ')',
                      TypePtr^.FnDefPtr^.ReturnTypePtr);
  write(This^.Output, '(');
  for Pos := 1 to TypePtr^.FnDefPtr^.Args.Count do
  begin
    if Pos <> 1 then write(This^.Output, ', ');
    _CgC_OutArgumentDeclaration(This, TypePtr^.FnDefPtr^.Args.Defs[Pos])
  end;
  write(This^.Output, ')')
end;

procedure _CgC_OutTypeReference(This : TCgC; TypePtr : TSDType);
var TheType : TSDType;
begin
  if TypePtr = nil then write(This^.Output, 'void')
  else if TypePtr^.Cls = SdtcPointer then
  begin
    _CgC_OutTypeReference(This, TypePtr^.PointedTypePtr);
    write(This^.Output, '*')
  end
  else if TypePtr^.Cls = SdtcBoolean then write(This^.Output, 'PBoolean')
  else if TypePtr^.Cls = SdtcInteger then write(This^.Output, 'PInteger')
  else if TypePtr^.Cls = SdtcReal then write(This^.Output, 'PReal')
  else if TypePtr^.Cls = SdtcChar then write(This^.Output, 'PChar')
  else if TypePtr^.Cls = SdtcString then write(This^.Output, 'PString')
  else if TypePtr^.Cls = SdtcFile then write(This^.Output, 'PFile')
  else if TypePtr^.Cls = SdtcEnum then
  begin
    if TypePtr^.EnumPtr^.HasBeenDefined and (TypePtr^.Name <> '') then
      write(This^.Output, TypePtr^.Name)
    else
      write(This^.Output, 'enum enum', TypePtr^.EnumPtr^.Id)
  end
  else if TypePtr^.Cls = SdtcRange then
         write(This^.Output, _CgC_GetRangeType(TypePtr))
  else if TypePtr^.Cls = SdtcSet then
         _CgC_OutSetTypeName(This, TypePtr)
  else if TypePtr^.Cls = SdtcRecord then
  begin
    if TypePtr^.RecPtr^.HasBeenDefined and (TypePtr^.Name <> '') then
      write(This^.Output, TypePtr^.Name)
    else
      write(This^.Output, 'struct record', TypePtr^.RecPtr^.Id)
  end
  else if TypePtr^.Cls = SdtcArray then
  begin
    TheType := TypePtr;
    while IsArrayType(TheType) do
      TheType := TheType^.ArrayDef.ValueTypePtr;
    _CgC_OutTypeReference(This, TheType);
    TheType := TypePtr;
    while IsArrayType(TheType) do
    begin
      write(This^.Output, '[', GetBoundedTypeSize(TheType^.ArrayDef.
            IndexTypePtr), ']');
      TheType := TheType^.ArrayDef.ValueTypePtr
    end
  end
  else
    InternalError('Error writing type reference: ' + TypeName(TypePtr))
end;

procedure _CgC_OutNameAndType(This : TCgC; const Name : string;
                              TypePtr : TSDType);
var Sp : string;
begin
  if Name[1] <> '*' then Sp := ' '
  else Sp := '';
  if TypePtr = nil then write(This^.Output, 'void', Sp, Name)
  else if TypePtr^.Cls = SdtcPointer then
  begin
    _CgC_OutTypeReference(This, TypePtr^.PointedTypePtr);
    write(This^.Output, '*', Sp, Name)
  end
  else if (TypePtr^.AliasFor <> nil) and (TypePtr^.Name <> '') then
         write(This^.Output, TypePtr^.Name, Sp, Name)
  else if TypePtr^.Cls = SdtcBoolean then
         write(This^.Output, 'PBoolean', Sp, Name)
  else if TypePtr^.Cls = SdtcInteger then
         write(This^.Output, 'PInteger', Sp, Name)
  else if TypePtr^.Cls = SdtcReal then
         write(This^.Output, 'PReal', Sp, Name)
  else if TypePtr^.Cls = SdtcChar then
         write(This^.Output, 'PChar', Sp, Name)
  else if TypePtr^.Cls = SdtcString then
         write(This^.Output, 'PString', Sp, Name)
  else if TypePtr^.Cls = SdtcFile then
         write(This^.Output, 'PFile', Sp, Name)
  else if TypePtr^.Cls = SdtcEnum then
         _CgC_OutNameAndEnum(This, Name, TypePtr^.EnumPtr)
  else if TypePtr^.Cls = SdtcRange then
         write(This^.Output, _CgC_GetRangeType(TypePtr), Sp, Name)
  else if TypePtr^.Cls = SdtcSet then
  begin
    _CgC_OutSetTypeName(This, TypePtr);
    write(This^.Output, Sp, Name)
  end
  else if TypePtr^.Cls = SdtcRecord then
         _CgC_OutNameAndRecord(This, Name, TypePtr^.RecPtr)
  else if TypePtr^.Cls = SdtcArray then
         _CgC_OutNameAndArray(This, Name, TypePtr)
  else if TypePtr^.Cls = SdtcFunction then
         _CgC_OutNameAndFunction(This, Name, TypePtr)
  else
    InternalError('Error writing name and type: ' + Name + ', ' +
                  TypeName(TypePtr))
end;

procedure _CgC_OutTypeDefinition(This : TCgC; TypePtr : TSDType);
var Name : string;
begin
  _CgC_OutBlankline(This, TotType);
  _CgC_OutIndent(This);
  Name := TypePtr^.Name;
  if TypePtr^.AliasFor = nil then
    InternalError('Type ' + Name + ' is not an alias');
  write(This^.Output, 'typedef ');
  _CgC_OutNameAndType(This, Name, TypePtr^.AliasFor);
  write(This^.Output, ';');
  _CgC_OutNewline(This)
end;

procedure _CgC_OutEnumValues(This : TCgC; EnumPtr : TSDTEnum);
var PosInEnum : integer;
begin
  _CgC_OutBlankline(This, TotEnumVal);
  _CgC_OutIndent(This);
  write(This^.Output, 'const char* enumvalues', EnumPtr^.Id, '[] = { ');
  for PosInEnum := 0 to EnumPtr^.Size - 1 do
  begin
    if PosInEnum <> 0 then write(This^.Output, ', ');
    write(This^.Output, '"', EnumPtr^.Values[PosInEnum], '"')
  end;
  write(This^.Output, ' };');
  _CgC_OutNewline(This)
end;

procedure _CgC_OutVariableDefinition(This : TCgC; VarPtr : TSDVariable);
begin
  _CgC_OutBlankline(This, TotVar);
  _CgC_OutIndent(This);
  _CgC_OutVariableDeclaration(This, VarPtr);
  if VarPtr^.IsConstant then
  begin
    write(This^.Output, ' = ');
    _CgC_OutExpression(This, VarPtr^.ConstantValue)
  end
  else if VarPtr^.Location <> nil then
  begin
    write(This^.Output, ' = ');
    if not IsSameType(VarPtr^.TypePtr, VarPtr^.Location^.TypePtr) then
    begin
      write(This^.Output, '(');
      _CgC_OutTypeReference(This, VarPtr^.TypePtr);
      write(This^.Output, '*)')
    end;
    _CgC_OutExAddress(This, VarPtr^.Location)
  end
  else if IsFileType(VarPtr^.TypePtr) then
         write(This^.Output, ' = (PFile){.handle = PNil}');
  write(This^.Output, ';');
  _CgC_OutNewline(This)
end;

procedure _CgC_OutSubroutinePrototype(This : TCgC; Def : TSDSubroutine);
var Pos : integer;
begin
  _CgC_OutIndent(This);
  _CgC_OutNameAndType(This, Def^.ExternalName, Def^.ReturnTypePtr);
  write(This^.Output, '(');
  for Pos := 1 to Def^.Args.Count do
  begin
    if Pos <> 1 then write(This^.Output, ', ');
    _CgC_OutArgumentDeclaration(This, Def^.Args.Defs[Pos])
  end;
  write(This^.Output, ')');
end;

procedure _CgC_OutSubroutineDeclaration(This : TCgC; Def : TSDSubroutine);
begin
  _CgC_OutBlankline(This, TotFunDec);
  _CgC_OutSubroutinePrototype(This, Def);
  write(This^.Output, ';');
  _CgC_OutNewline(This)
end;

procedure _CgC_OutSubroutineDefinition(This : TCgC; Def : TSDSubroutine);
begin
  _CgC_OutBlankline(This, TotFunDef);
  _CgC_OutSubroutinePrototype(This, Def);
  write(This^.Output, ' ');
  _CgC_OutBegin(This);
  _CgC_OutDefinitions(This, @Def^.Scope);
  _CgC_OutBody(This, Def^.Body);
  if Def^.ReturnTypePtr <> nil then
  begin
    _CgC_OutIndent(This);
    write(This^.Output, 'return RESULT;');
    _CgC_OutNewline(This)
  end;
  _CgC_OutEnd(This);
end;

procedure _CgC_OutDefinitions(This : TCgC; Scope : TSScope);
var FirstDef, Def : TSDefinition;
begin
  Stack_GetOldest(Scope^.LatestDef, FirstDef);

  { Output defined types. }
  Def := FirstDef;
  while Def <> nil do
  begin
    if (Def^.Cls = SdcType) and (Def^.TypeDef.AliasFor <> nil) then
      _CgC_OutTypeDefinition(This, @Def^.TypeDef);
    Def := Def^.Newer
  end;

  { Output variables and constants. }
  Def := FirstDef;
  while Def <> nil do
  begin
    if (Def^.Cls = SdcVariable) and not Def^.VarDef.IsArgument then
      _CgC_OutVariableDefinition(This, @Def^.VarDef);
    Def := Def^.Newer
  end;

  { Output enum values. }
  Def := FirstDef;
  while Def <> nil do
  begin
    if (Def^.Cls = SdcType) and (Def^.TypeDef.Cls = SdtcEnum)
       and not Def^.TypeDef.EnumPtr^.ValuesHaveBeenOutput then
    begin
      _CgC_OutEnumValues(This, Def^.TypeDef.EnumPtr);
      Def^.TypeDef.EnumPtr^.ValuesHaveBeenOutput := true
    end;
    Def := Def^.Newer
  end;

  { Output forward-declared function prototypes. }
  Def := FirstDef;
  while Def <> nil do
  begin
    if (Def^.Cls = SdcSubroutine) and Def^.SrDef.HadDeclaration then
      _CgC_OutSubroutineDeclaration(This, @Def^.SrDef);
    Def := Def^.Newer
  end;

  { Output non-forward-declared function definitions. }
  Def := FirstDef;
  while Def <> nil do
  begin
    if (Def^.Cls = SdcSubroutine) and not Def^.SrDef.HadDeclaration then
      _CgC_OutSubroutineDefinition(This, @Def^.SrDef);
    Def := Def^.Newer
  end;

  { Output forward-declared function definitions. }
  Def := FirstDef;
  while Def <> nil do
  begin
    if (Def^.Cls = SdcSubroutine) and Def^.SrDef.HadDeclaration then
      _CgC_OutSubroutineDefinition(This, @Def^.SrDef);
    Def := Def^.Newer
  end;
end;

procedure _CgC_OutStEmpty(This : TCgC);
begin
  _CgC_OutIndent(This);
  write(This^.Output, ';');
  _CgC_OutNewline(This)
end;

procedure _CgC_OutStSequence(This : TCgC; Stmt : TSStatement);
var WasMultistatement : boolean;
begin
  WasMultistatement := This^.IsMultiStatement;
  if not WasMultistatement then
  begin
    _CgC_OutIndent(This);
    _CgC_OutBegin(This);
  end;
  _CgC_OutBody(This, Stmt^.Sequence);
  if not WasMultistatement then
    _CgC_OutEnd(This);
  This^.IsMultiStatement := WasMultistatement
end;

procedure _CgC_OutStAssign(This : TCgC; Stmt : TSStatement);
begin
  _CgC_OutIndent(This);
  _CgC_OutExpression(This, Stmt^.Lhs);
  write(This^.Output, ' = ');
  _CgC_OutExpression(This, Stmt^.Rhs);
  write(This^.Output, ';');
  _CgC_OutNewline(This)
end;

procedure _CgC_OutStProcCall(This : TCgC; Stmt : TSStatement);
begin
  _CgC_OutIndent(This);
  _CgC_OutExpression(This, Stmt^.ProcCall);
  write(This^.Output, ';');
  _CgC_OutNewline(This)
end;

procedure _CgC_OutStIf(This : TCgC; Stmt : TSStatement);
var WasMultistatement : boolean;
begin
  WasMultistatement := This^.IsMultiStatement;
  _CgC_OutIndent(This);
  write(This^.Output, 'if (');
  _CgC_OutExpression(This, Stmt^.IfCond);
  write(This^.Output, ') ');
  This^.IsMultiStatement := false;
  _CgC_OutStatement(This, Stmt^.IfThen);
  if Stmt^.IfElse^.Cls <> SscEmpty then
  begin
    _CgC_OutIndent(This);
    write(This^.Output, 'else ');
    This^.IsMultiStatement := false;
    _CgC_OutStatement(This, Stmt^.IfElse);
  end;
  This^.IsMultiStatement := WasMultistatement
end;

procedure _CgC_OutStRepeat(This : TCgC; Stmt : TSStatement);
var TmpExpr : TSExpression;
begin
  _CgC_OutIndent(This);
  write(This^.Output, 'do ');
  _CgC_OutBegin(This);
  _CgC_OutBody(This, Stmt^.RepeatSequence);
  _CgC_OutEndSameLine(This);
  write(This^.Output, ' while (');
  TmpExpr := ExOpNot(ExCopy(Stmt^.UntilCond));
  _CgC_OutExpression(This, TmpExpr);
  ExDispose(TmpExpr);
  write(This^.Output, ');');
  _CgC_OutNewline(This)
end;

procedure _CgC_OutStWhile(This : TCgC; Stmt : TSStatement);
var WasMultistatement : boolean;
begin
  WasMultistatement := This^.IsMultiStatement;
  _CgC_OutIndent(This);
  write(This^.Output, 'while (');
  _CgC_OutExpression(This, Stmt^.WhileCond);
  write(This^.Output, ') ');
  This^.IsMultiStatement := false;
  _CgC_OutStatement(This, Stmt^.WhileStatement);
  This^.IsMultiStatement := WasMultistatement
end;

procedure _CgC_OutStFor(This : TCgC; Stmt : TSStatement);
var 
  WasMultistatement : boolean;
  LimitType : TSDType;
  First, Last : TSDVariableDef;
begin
  WasMultistatement := This^.IsMultiStatement;
  LimitType := Stmt^.Iterator^.TypePtr;
  if IsEnumType(LimitType) then LimitType := PrimitiveTypes.PtInteger;
  First := MakeVariable('first', LimitType);
  Last := MakeVariable('last', LimitType);
  _CgC_OutIndent(This);
  write(This^.Output, 'for (');
  _CgC_OutVariableDeclaration(This, @First);
  write(This^.Output, ' = ');
  _CgC_OutExpression(This, Stmt^.First);
  write(This^.Output, ', ', Last.Name, ' = ');
  _CgC_OutExpression(This, Stmt^.Last);
  write(This^.Output, '; first ');
  if Stmt^.Ascending then
    write(This^.Output, '<=')
  else
    write(This^.Output, '>=');
  write(This^.Output, ' last; /*breaks*/) ');
  _CgC_OutBegin(This);
  _CgC_OutIndent(This);
  write(This^.Output, 'PBoolean done = 0;');
  _CgC_OutNewline(This);
  _CgC_OutIndent(This);
  write(This^.Output, 'for (');
  _CgC_OutExpression(This, Stmt^.Iterator);
  write(This^.Output, ' = first; !done; done = ');
  _CgC_OutExpression(This, Stmt^.Iterator);
  write(This^.Output, ' == last ? 1 : (');
  if Stmt^.Ascending then
    write(This^.Output, '++')
  else
    write(This^.Output, '--');
  _CgC_OutExpression(This, Stmt^.Iterator);
  write(This^.Output, ', 0)) ');
  This^.IsMultiStatement := false;
  _CgC_OutStatement(This, Stmt^.ForStatement);
  This^.IsMultiStatement := WasMultistatement;
  _CgC_OutIndent(This);
  write(This^.Output, 'break;');
  _CgC_OutNewline(This);
  _CgC_OutEnd(This)
end;

procedure _CgC_OutStWith(This : TCgC; Stmt : TSStatement);
var WasMultistatement : boolean;
begin
  WasMultistatement := This^.IsMultiStatement;
  if not WasMultistatement then
  begin
    _CgC_OutIndent(This);
    _CgC_OutBegin(This);
  end;
  _CgC_OutIndent(This);
  write(This^.Output, Stmt^.WithVar^.VarPtr^.Name, ' = ');
  if (Stmt^.WithVar^.VarPtr^.IsReference) then
    _CgC_OutExAddress(This, Stmt^.WithValue)
  else
    _CgC_OutExpression(This, Stmt^.WithValue);
  write(This^.Output, ';');
  _CgC_OutNewline(This);
  _CgC_OutStatement(This, Stmt^.WithStatement);
  if not WasMultistatement then
    _CgC_OutEnd(This);
  This^.IsMultiStatement := WasMultistatement
end;

procedure _CgC_OutStCase(This : TCgC; Stmt : TSStatement);
var 
  CaseEntry : TSSCase;
  WasMultistatement : boolean;
  HasDefault : boolean;
begin
  HasDefault := false;
  WasMultistatement := This^.IsMultiStatement;
  _CgC_OutIndent(This);
  write(This^.Output, 'switch (');
  _CgC_OutExpression(This, Stmt^.CaseSelector);
  write(This^.Output, ') ');
  _CgC_OutBegin(This);

  CaseEntry := Stmt^.CaseEntry;
  while CaseEntry <> nil do
  begin
    _CgC_OutIndent(This);
    if CaseEntry^.CaseLabel <> nil then
    begin
      write(This^.Output, 'case ');
      _CgC_OutExpression(This, CaseEntry^.CaseLabel);
      write(This^.Output, ':');
    end
    else
    begin
      write(This^.Output, 'default:');
      HasDefault := true
    end;
    This^.Indent := This^.Indent + 1;
    _CgC_OutNewline(This);
    This^.IsMultiStatement := true;
    _CgC_OutStatement(This, CaseEntry^.Statement);
    _CgC_OutIndent(This);
    write(This^.Output, 'break;');
    This^.Indent := This^.Indent - 1;
    _CgC_OutNewline(This);
    This^.IsMultiStatement := WasMultistatement;
    CaseEntry := CaseEntry^.Next;
  end;
  if not HasDefault then
  begin
    _CgC_OutIndent(This);
    write(This^.Output, 'default:');
    This^.Indent := This^.Indent + 1;
    _CgC_OutNewline(This);
    _CgC_OutIndent(This);
    write(This^.Output, 'break;');
    This^.Indent := This^.Indent - 1;
    _CgC_OutNewline(This);
  end;
  _CgC_OutEnd(This)
end;

procedure _CgC_OutStatement(This : TCgC; Stmt : TSStatement);
begin
  case Stmt^.Cls of 
    SscEmpty: _CgC_OutStEmpty(This);
    SscSequence: _CgC_OutStSequence(This, Stmt);
    SscAssign: _CgC_OutStAssign(This, Stmt);
    SscProcCall: _CgC_OutStProcCall(This, Stmt);
    SscIf: _CgC_OutStIf(This, Stmt);
    SscRepeat: _CgC_OutStRepeat(This, Stmt);
    SscWhile: _CgC_OutStWhile(This, Stmt);
    SscFor: _CgC_OutStFor(This, Stmt);
    SscWith: _CgC_OutStWith(This, Stmt);
    SscCase: _CgC_OutStCase(This, Stmt);
  end;
end;

procedure _CgC_OutBody(This : TCgC; Body : TSSSequence);
var Elem : TSSSequence;
begin
  Elem := Body;
  while Elem <> nil do
  begin
    _CgC_OutStatement(This, Elem^.Statement);
    Elem := Elem^.Next
  end
end;

procedure _CgC_OutProgram(This : TCgC; Prog : TSProgram);
begin
  write(This^.Output, '/* Program: ', Prog^.Name, ' */');
  _CgC_OutNewline(This);
  write(This^.Output, '#include "pascual.h"');
  _CgC_OutNewline(This);

  _CgC_OutDefinitions(This, @Prog^.Scope);

  _CgC_OutBlankline(This, TotFunDef);
  _CgC_OutIndent(This);
  write(This^.Output, 'void pascual_main() ');
  _CgC_OutBegin(This);
  _CgC_OutBody(This, Prog^.Body);
  _CgC_OutEnd(This)
end;





procedure _CgC_SetOutputFile(This : TCgC; const Name : string);
var F : text;
begin
  Assign(F, Name);
  Rewrite(F);
  This^.Output := F
end;

procedure _CgC_SetOutputFile_Ext(This : TCodegen; const Name : string);
var Cg : TCgC absolute This;
begin
  _CgC_SetOutputFile(Cg, Name)
end;

procedure _CgC_Generate_Ext(This :TCodegen; AST : TSProgram);
var Cg : TCgC absolute This;
begin
  _CgC_OutProgram(Cg, AST);
  close(Cg^.Output)
end;

procedure Cg_C_Init(var Codegen : TCodegen);
var This : TCgC absolute Codegen;
begin
  new(This);
  This^.Base.SetOutputFile := @_CgC_SetOutputFile_Ext;
  This^.Base.Generate := @_CgC_Generate_Ext;
  { TODO change This^.Output := Output; }
  This^.Output := StdErr;
  This^.IsMultiStatement := false;
  This^.Indent := 0;
  This^.Newline := true;
  This^.LastOut := TotNone;
end;

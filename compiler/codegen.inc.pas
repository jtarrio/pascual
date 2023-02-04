type 
  TOutputType = (TotNone, TotType, TotVar, TotEnumVal, TotFunDec, TotFunDef);

var 
  Codegen : record
    Output : text;
    IsMultiStatement : boolean;
    Indent : integer;
    Newline : boolean;
    LastOut : TOutputType
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

procedure _OutChar(Chr : char);
begin
  if Chr = '''' then write(Codegen.Output, '''\''''')
  else if Chr = '\' then write(Codegen.Output, '''\\''')
  else if Chr >= ' ' then write(Codegen.Output, '''', Chr, '''')
  else CompileError('Internal error: escaped chars are not supported yet')
end;

procedure _OutString(var Str : string);
var 
  Pos : integer;
  Chr : char;
begin
  if Length(Str) = 1 then
  begin
    write(Codegen.Output, 'str_of(');
    _OutChar(Str[1]);
    write(Codegen.Output, ')')
  end
  else
  begin
    write(Codegen.Output, 'str_make(', Length(Str), ', "');
    for Pos := 1 to Length(Str) do
    begin
      Chr := Str[Pos];
      if Chr = '"' then write(Codegen.Output, '\"')
      else if Chr = '\' then write(Codegen.Output, '\\')
      else if Chr >= ' ' then write(Codegen.Output, Chr)
      else CompileError('Internal error: escaped chars are not supported yet')
    end;
    write(Codegen.Output, '")')
  end
end;

function _BinOpPrec(Expr : TExpression) : integer;
begin
  case Expr^.BinaryEx.Op of 
    TkPlus : if IsStringyType(Expr^.TypeIndex) then _BinOpPrec := 1
             else _BinOpPrec := 4;
    TkMinus : _BinOpPrec := 4;
    TkAsterisk : _BinOpPrec := 3;
    TkSlash : _BinOpPrec := 3;
    TkDiv : _BinOpPrec := 3;
    TkMod : _BinOpPrec := 3;
    TkAnd : if IsBooleanType(Expr^.TypeIndex) then _BinOpPrec := 11
            else _BinOpPrec := 8;
    TkOr : if IsBooleanType(Expr^.TypeIndex) then _BinOpPrec := 12
           else _BinOpPrec := 10;
    TkEquals : _BinOpPrec := 7;
    TkNotEquals : _BinOpPrec := 7;
    TkLessthan : _BinOpPrec := 6;
    TkMorethan : _BinOpPrec := 6;
    TkLessOrEquals : _BinOpPrec := 6;
    TkMoreOrEquals : _BinOpPrec := 6;
    else CompileError('Internal error: unknown precedence for operator ' +
                      LxTokenName(Expr^.BinaryEx.Op))
  end
end;

function _Precedence(Expr : TExpression) : integer;
begin
  case Expr^.Cls of 
    XcImmediate : _Precedence := 0;
    XcToString : _Precedence := 0;
    XcVariableAccess : if Expr^.VariableEx.VariableIndex^.IsReference then
                         _Precedence := 2
                       else
                         _Precedence := 0;
    XcFieldAccess : _Precedence := 1;
    XcArrayAccess : _Precedence := 1;
    XcPointerAccess : _Precedence := 2;
    XcStringChar : _Precedence := 1;
    XcFunctionRef : _Precedence := 0;
    XcFunctionCall : _Precedence := 1;
    XcPseudoFunRef : _Precedence := 0;
    XcPseudoFunCall : _Precedence := 1;
    XcUnaryOp : _Precedence := 2;
    XcBinaryOp : _Precedence := _BinOpPrec(Expr);
    else CompileError('Internal error: unknown precedence')
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

procedure _OutExImmediate(Expr : TExpression);
begin
  with Expr^.ImmediateEx do
    case Cls of 
      XicNil : write(Codegen.Output, '(void*)0');
      XicBoolean : if BooleanValue then write(Codegen.Output, '1')
                   else write(Codegen.Output, '0');
      XicInteger : write(Codegen.Output, IntegerValue);
      XicChar : _OutChar(CharValue);
      XicString : _OutString(StringValue);
      XicEnum : write(Codegen.Output,
                      Expr^.TypeIndex^.EnumIndex^.Values[EnumOrdinal])
    end
end;

procedure _OutBounds(TypeIndex : TPsTypeIndex);
begin
  if IsBooleanType(TypeIndex) then write(Codegen.Output, '0, 1')
  else if IsIntegerType(TypeIndex) then
         write(Codegen.Output, 'INT_MIN, INT_MAX')
  else if IsCharType(TypeIndex) then write(Codegen.Output, '0, 255')
  else if IsEnumType(TypeIndex) then
         write(Codegen.Output, '0, ', TypeIndex^.EnumIndex^.Size - 1)
  else if IsRangeType(TypeIndex) then
  begin
    OutExpression(TypeIndex^.RangeIndex^.First);
    write(Codegen.Output, ', ');
    OutExpression(TypeIndex^.RangeIndex^.Last)
  end
  else
    CompileError('Internal error: unknown bounds for type ' +
                 TypeName(TypeIndex))
end;

procedure _OutExSubrange(Expr : TExpression);
begin
  write(Codegen.Output, 'subrange(');
  OutExpression(Expr^.SubrangeEx.Parent);
  write(Codegen.Output, ', ');
  _OutBounds(Expr^.TypeIndex);
  write(Codegen.Output, ')')
end;

procedure _OutExVariable(Expr : TExpression);
begin
  if Expr^.VariableEx.VariableIndex^.IsReference then
    write(Codegen.Output,
          '*', Expr^.VariableEx.VariableIndex^.Name)
  else
    write(Codegen.Output,
          Expr^.VariableEx.VariableIndex^.Name)
end;

procedure _OutExFieldAccess(Expr : TExpression);
begin
  if Expr^.FieldEx.Parent^.Cls = XcPointerAccess then
  begin
    _OutExpressionParens(Expr^.FieldEx.Parent^.PointerEx.Parent, Expr);
    write(Codegen.Output, '->')
  end
  else
  begin
    _OutExpressionParens(Expr^.FieldEx.Parent, Expr);
    write(Codegen.Output, '.')
  end;
  write(Codegen.Output, Expr^.FieldEx.Parent^.TypeIndex^.RecordIndex^
        .Fields[Expr^.FieldEx.FieldNumber].Name)
end;

procedure _OutExFunctionCall(Expr : TExpression);
var Pos : integer;
begin
  _OutExpressionParens(Expr^.CallEx.FunctionRef, Expr);
  write(Codegen.Output, '(');
  for Pos := 1 to Expr^.CallEx.Args.Size do
  begin
    if Pos <> 1 then write(Codegen.Output, ', ');
    if Expr^.CallEx.FunctionRef^.FunctionEx.FunctionIndex^.Args[Pos].IsReference
      then
    begin
      if not Expr^.CallEx.Args.Values[Pos]^.IsAssignable then
        CompileError('Pass-by-reference argument must be assignable');
      write(Codegen.Output, '&');
      _OutExpressionParensPrec(Expr^.CallEx.Args.Values[Pos], 2)
    end
    else
      OutExpression(Expr^.CallEx.Args.Values[Pos])
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
procedure _OutWrite(Expr : TExpression);
forward;

procedure _OutExPseudoFunCall(Expr : TExpression);
begin
  case Expr^.PseudoFunCallEx.PseudoFun of 
    TpfDispose : _OutDispose(Expr);
    TpfNew : _OutNew(Expr);
    TpfOrd : _OutOrd(Expr);
    TpfPred : _OutPred(Expr);
    TpfRead : _OutRead(Expr);
    TpfReadln : _OutRead(Expr);
    TpfStr : _OutStr(Expr);
    TpfSucc : _OutSucc(Expr);
    TpfWrite : _OutWrite(Expr);
    TpfWriteln : _OutWrite(Expr);
    else CompileError('Internal error: unimplemented special function')
  end
end;

procedure _OutExUnaryOp(Expr : TExpression);
begin
  if Expr^.UnaryEx.Op = TkMinus then write(Codegen.Output, '-')
  else if (Expr^.UnaryEx.Op = TkNot) and IsBooleanType(Expr^.TypeIndex) then
         write(Codegen.Output, '!')
  else if (Expr^.UnaryEx.Op = TkNot) and IsIntegerType(Expr^.TypeIndex) then
         write(Codegen.Output, '~');
  _OutExpressionParens(Expr^.UnaryEx.Parent, Expr)
end;

function _IsArithmeticOp(Op : TLxTokenId) : boolean;
begin
  _IsArithmeticOp := (Op = TkPlus) or (Op = TkMinus) or (Op = TkAsterisk)
                     or (Op = TkSlash) or (Op = TkDiv) or (Op = TkMod)
end;

function _GetArithmeticOp(Op : TLxTokenId) : string;
begin
  case Op of 
    TkPlus : _GetArithmeticOp := '+';
    TkMinus : _GetArithmeticOp := '-';
    TkAsterisk : _GetArithmeticOp := '*';
    TkSlash : _GetArithmeticOp := '/';
    TkDiv : _GetArithmeticOp := '/';
    TkMod : _GetArithmeticOp := '%'
  end
end;

function _IsLogicalOrBitwiseOp(Op : TLxTokenId) : boolean;
begin
  _IsLogicalOrBitwiseOp := (Op = TkAnd) or (Op = TkOr)
end;

function _GetLogicalOp(Op : TLxTokenId) : string;
begin
  if Op = TkAnd then _GetLogicalOp := '&&'
  else _GetLogicalOp := '||'
end;

function _GetBitwiseOp(Op : TLxTokenId) : string;
begin
  if Op = TkAnd then _GetBitwiseOp := '&'
  else _GetBitwiseOp := '|'
end;

function _IsRelationalOp(Op : TLxTokenId) : boolean;
begin
  _IsRelationalOp := (Op = TkEquals) or (Op = TkNotEquals) or (Op = TkLessthan)
                     or (Op = TkMorethan) or (Op = TkLessOrEquals)
                     or (Op = TkMoreOrEquals)
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

procedure _OutExBinaryOp(Expr : TExpression);
var Ltype, Rtype : char;
begin
  with Expr^.BinaryEx do
  begin
    if IsStringyType(Left^.TypeIndex) then
    begin
      if IsCharType(Left^.TypeIndex) then Ltype := 'c'
      else Ltype := 's';
      if IsCharType(Right^.TypeIndex) then Rtype := 'c'
      else Rtype := 's';
      if Op = TkPlus then
      begin
        write(Codegen.Output, 'cat_', Ltype, Rtype, '(');
        OutExpression(Left);
        write(Codegen.Output, ', ');
        OutExpression(Right);
        write(Codegen.Output, ')')
      end
      else if IsCharType(Left^.TypeIndex) and IsCharType(Right^.TypeIndex)
             then
      begin
        _OutExpressionParens(Left, Expr);
        write(Codegen.Output, ' ', _GetRelationalOp(Op), ' ');
        _OutExpressionParensExtra(Right, Expr)
      end
      else
      begin
        write(Codegen.Output, 'cmp_', Ltype, Rtype, '(');
        OutExpression(Left);
        write(Codegen.Output, ', ');
        OutExpression(Right);
        write(Codegen.Output, ') ', _GetRelationalOp(Op), ' 0')
      end
    end
    else if IsBooleanType(Left^.TypeIndex) then
    begin
      _OutExpressionParens(Left, Expr);
      if _IsLogicalOrBitwiseOp(Op) then
        write(Codegen.Output, ' ', _GetLogicalOp(Op), ' ')
      else
        write(Codegen.Output, ' ' , _GetRelationalOp(Op), ' ');
      _OutExpressionParensExtra(Right, Expr)
    end
    else if IsIntegerType(Left^.TypeIndex) then
    begin
      _OutExpressionParens(Left, Expr);
      if _IsArithmeticOp(Op) then
        write(Codegen.Output, ' ', _GetArithmeticOp(Op), ' ')
      else if _IsLogicalOrBitwiseOp(Op) then
             write(Codegen.Output, ' ', _GetBitwiseOp(Op), ' ')
      else
        write(Codegen.Output, ' ' , _GetRelationalOp(Op), ' ');
      _OutExpressionParensExtra(Right, Expr)
    end
    else
    begin
      _OutExpressionParens(Left, Expr);
      write(Codegen.Output, ' ', _GetRelationalOp(Op), ' ');
      _OutExpressionParensExtra(Right, Expr)
    end
  end
end;

procedure OutExpression;
var TmpExpr : TExpression;
begin
  case Expr^.Cls of 
    XcImmediate: _OutExImmediate(Expr);
    XcToString:
                begin
                  write(Codegen.Output, 'str_of(');
                  OutExpression(Expr^.ToStringEx.Parent);
                  write(Codegen.Output, ')')
                end;
    XcSubrange: _OutExSubrange(Expr);
    XcVariableAccess : _OutExVariable(Expr);
    XcFieldAccess: _OutExFieldAccess(Expr);
    XcArrayAccess:
                   begin
                     _OutExpressionParens(Expr^.ArrayEx.Parent, Expr);
                     write(Codegen.Output, '[');
                     TmpExpr := ExBinaryOp(
                                CopyExpr(Expr^.ArrayEx.Subscript),
                                CopyExpr(Expr^.ArrayEx.Parent^.TypeIndex^
                                .ArrayIndex^.LowBound),
                                TkMinus);
                     OutExpression(TmpExpr);
                     DisposeExpr(TmpExpr);
                     write(Codegen.Output, ']')
                   end;
    XcPointerAccess:
                     begin
                       write(Codegen.Output, '*');
                       _OutExpressionParens(Expr^.PointerEx.Parent, Expr)
                     end;
    XcStringChar:
                  begin
                    _OutExpressionParens(Expr^.StringCharEx.Parent, Expr);
                    write(Codegen.Output, '.chr[');
                    OutExpression(Expr^.StringCharEx.Subscript);
                    write(Codegen.Output, ']')
                  end;
    XcFunctionRef: write(Codegen.Output, Expr^.FunctionEx.FunctionIndex^.Name);
    XcFunctionCall: _OutExFunctionCall(Expr);
    XcPseudoFunCall: _OutExPseudoFunCall(Expr);
    XcUnaryOp: _OutExUnaryOp(Expr);
    XcBinaryOp: _OutExBinaryOp(Expr)
  end
end;

procedure OutEnumValues(EnumIndex : TPsEnumIndex);
var 
  PosInEnum : integer;
begin
  _OutBlankline(TotEnumVal);
  _OutIndent;
  write(Codegen.Output, 'const char* enumvalues',
        EnumIndex^.Id,
        '[] = { ');
  for PosInEnum := 0 to EnumIndex^.Size - 1 do
  begin
    if PosInEnum <> 0 then write(Codegen.Output, ', ');
    write(Codegen.Output, '"', EnumIndex^.Values[PosInEnum], '"')
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
    if Def^.Cls = TdcEnum then OutEnumValues(Def^.EnumIndex);
    Def := Def^.Next
  end
end;

function OutVariableName(Name : string; IsReference : boolean) : string;
begin
  if IsReference then OutVariableName := '*' + Name
  else OutVariableName := Name
end;

procedure OutTypeReference(TypeIndex : TPsTypeIndex);
begin
  if TypeIndex = nil then write(Codegen.Output, 'void')
  else if TypeIndex^.Cls = TtcPointer then
  begin
    OutTypeReference(TypeIndex^.PointedTypeIndex);
    write(Codegen.Output, '*')
  end
  else if TypeIndex^.Cls = TtcBoolean then write(Codegen.Output, 'int')
  else if TypeIndex^.Cls = TtcInteger then write(Codegen.Output, 'int')
  else if TypeIndex^.Cls = TtcChar then write(Codegen.Output, 'char')
  else if TypeIndex^.Cls = TtcString then write(Codegen.Output, 'PString')
  else if TypeIndex^.Cls = TtcText then write(Codegen.Output, 'PFile')
  else if TypeIndex^.Cls = TtcEnum then
  begin
    if TypeIndex^.EnumIndex^.HasBeenDefined and (TypeIndex^.Name <> '') then
      write(Codegen.Output, TypeIndex^.Name)
    else
      write(Codegen.Output, 'enum enum', TypeIndex^.EnumIndex^.Id)
  end
  else if TypeIndex^.Cls = TtcRange then
         OutTypeReference(TypeIndex^.RangeIndex^.First^.TypeIndex)
  else if TypeIndex^.Cls = TtcRecord then
  begin
    if TypeIndex^.RecordIndex^.HasBeenDefined and (TypeIndex^.Name <> '') then
      write(Codegen.Output, TypeIndex^.Name)
    else
      write(Codegen.Output, 'struct record', TypeIndex^.RecordIndex^.Id)
  end
  else if TypeIndex^.Cls = TtcArray then
  begin
    OutTypeReference(TypeIndex^.ArrayIndex^.TypeIndex);
    write(Codegen.Output, '*')
  end
  else
    CompileError('Error writing type reference: ' + TypeName(TypeIndex))
end;

procedure OutNameAndType(Name : string; TypeIndex : TPsTypeIndex);
forward;

procedure OutNameAndRecord(Name : string; RecordIndex : TPsRecordIndex);
var 
  Pos : integer;
  NumVariant : integer;
begin
  NumVariant := 0;
  write(Codegen.Output, 'struct record', RecordIndex^.Id);
  if not RecordIndex^.HasBeenDefined then
  begin
    write(Codegen.Output, ' ');
    OutBegin;
    for Pos := 1 to RecordIndex^.Size do
    begin
      if (RecordIndex^.NumVariants > NumVariant)
         and (RecordIndex^.VariantBounds[NumVariant + 1] = Pos) then
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
      OutNameAndType(RecordIndex^.Fields[Pos].Name,
                     RecordIndex^.Fields[Pos].TypeIndex);
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
    RecordIndex^.HasBeenDefined := true
  end;
  write(Codegen.Output, ' ', Name)
end;

procedure OutNameAndEnum(Name : string; EnumIndex : TPsEnumIndex);
var 
  Pos : integer;
begin
  write(Codegen.Output, 'enum enum', EnumIndex^.Id);
  if not EnumIndex^.HasBeenDefined then
  begin
    write(Codegen.Output, ' { ');
    for Pos := 0 to EnumIndex^.Size - 1 do
    begin
      if Pos > 0 then
        write(Codegen.Output, ', ');
      write(Codegen.Output, EnumIndex^.Values[Pos])
    end;
    write(Codegen.Output, ' }');
    EnumIndex^.HasBeenDefined := true
  end;
  write(Codegen.Output, ' ', Name)
end;

procedure OutNameAndType(Name : string; TypeIndex : TPsTypeIndex);
var 
  Arr : TPsArrayDef;
  SizeExpr : TExpression;
begin
  if TypeIndex = nil then write(Codegen.Output, 'void ', Name)
  else if TypeIndex^.Cls = TtcPointer then
  begin
    OutTypeReference(TypeIndex^.PointedTypeIndex);
    write(Codegen.Output, ' *', Name)
  end
  else if (TypeIndex^.AliasFor <> nil) and (TypeIndex^.Name <> '') then
         write(Codegen.Output, TypeIndex^.Name, ' ', Name)
  else if TypeIndex^.Cls = TtcBoolean then
         write(Codegen.Output, 'int ', Name)
  else if TypeIndex^.Cls = TtcInteger then
         write(Codegen.Output, 'int ', Name)
  else if TypeIndex^.Cls = TtcChar then
         write(Codegen.Output, 'char ', Name)
  else if TypeIndex^.Cls = TtcString then
         write(Codegen.Output, 'PString ', Name)
  else if TypeIndex^.Cls = TtcText then
         write(Codegen.Output, 'PFile ', Name)
  else if TypeIndex^.Cls = TtcEnum then
         OutNameAndEnum(Name, TypeIndex^.EnumIndex)
  else if TypeIndex^.Cls = TtcRange then
         OutNameAndType(Name, TypeIndex^.RangeIndex^.First^.TypeIndex)
  else if TypeIndex^.Cls = TtcRecord then
         OutNameAndRecord(Name, TypeIndex^.RecordIndex)
  else if TypeIndex^.Cls = TtcArray then
  begin
    Arr := TypeIndex^.ArrayIndex^;
    OutNameAndType(Name, Arr.TypeIndex);
    write(Codegen.Output, '[');
    SizeExpr := ExBinaryOp(
                ExBinaryOp(ExIntegerConstant(1), CopyExpr(Arr.Highbound),
                TkPlus),
                CopyExpr(Arr.LowBound),
                TkMinus);
    OutExpression(SizeExpr);
    DisposeExpr(SizeExpr);
    write(Codegen.Output, ']')
  end
  else
    CompileError('Error writing name and type: ' + Name + ', ' +
                 TypeName(TypeIndex))
end;

procedure OutTypeDefinition(TypeIndex : TPsTypeIndex);
var 
  Name : string;
begin
  _OutBlankline(TotType);
  _OutIndent;
  Name := TypeIndex^.Name;
  if TypeIndex^.AliasFor = nil then
    CompileError('Type ' + Name + ' is not an alias');
  write(Codegen.Output, 'typedef ');
  OutNameAndType(Name, TypeIndex^.AliasFor);
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
      _ResolvePointerUnknown(Def^.TypeIndex);
      if Def^.TypeIndex^.AliasFor <> nil then
        OutTypeDefinition(Def^.TypeIndex)
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
  OutNameAndType(OutVariableName(VarDef.Name, VarDef.IsReference),
  VarDef.TypeIndex)
end;

procedure OutVariableDefinition;
begin
  _OutBlankline(TotVar);
  _OutIndent;
  if VarIndex^.IsConstant then
    write(Codegen.Output, 'const ');
  OutVariableDeclaration(VarIndex^);
  write(Codegen.Output, ';');
  _OutNewline
end;

procedure OutConstantDefinitionBegin;
begin
  _OutBlankline(TotVar);
  _OutIndent;
  write(Codegen.Output, 'const ');
  OutVariableDeclaration(VarIndex^);
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
  OutNameAndType(Def.Name, Def.ReturnTypeIndex);
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
  OutFunctionPrototype(FnIndex^);
  write(Codegen.Output, ';');
  _OutNewline
end;

procedure OutFunctionDefinition;
begin
  _OutBlankline(TotFunDef);
  OutFunctionPrototype(FnIndex^);
  write(Codegen.Output, ' ');
  OutBegin
end;

procedure OutFunctionEnd;
begin
  if FnIndex^.ReturnTypeIndex <> nil then
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

function ShortTypeName(TypeIndex : TPsTypeIndex) : char;
begin
  if IsRangeType(TypeIndex) then
    TypeIndex := TypeIndex^.RangeIndex^.BaseTypeIndex;
  if IsBooleanType(TypeIndex) then ShortTypeName := 'b'
  else if IsIntegerType(TypeIndex) then ShortTypeName := 'i'
  else if IsCharType(TypeIndex) then ShortTypeName := 'c'
  else if IsStringType(TypeIndex) then ShortTypeName := 's'
  else if IsEnumType(TypeIndex) then ShortTypeName := 'e'
  else CompileError('No short type name exists for ' + TypeName(TypeIndex))
end;

procedure _OutRead(Expr : TExpression);
var 
  Src : TExpression;
  ReadArg : ^TExReadArgs;
  Linefeed : boolean;
  Braces : boolean;
begin
  Src := Expr^.PseudoFunCallEx.Arg1;
  Linefeed := Expr^.PseudoFunCallEx.PseudoFun = TpfReadln;
  ReadArg := Expr^.PseudoFunCallEx.ReadArgs;
  Braces := (not Codegen.IsMultiStatement) and (ReadArg <> nil)
            and ((ReadArg^.Next <> nil) or Linefeed);
  if Braces then OutBegin;
  while ReadArg <> nil do
  begin
    _OutIndent;
    write(Codegen.Output, 'read_',
          ShortTypeName(ReadArg^.Arg^.TypeIndex), '(&');
    _OutExpressionParensPrec(Src, 2);
    write(Codegen.Output,', &');
    _OutExpressionParensPrec(ReadArg^.Arg, 2);
    write(Codegen.Output, ');');
    _OutNewline;
    ReadArg := ReadArg^.Next
  end;
  if Linefeed then
  begin
    _OutIndent;
    write(Codegen.Output, 'readln(&');
    _OutExpressionParensPrec(Src, 2);
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
begin
  Dst := Expr^.PseudoFunCallEx.Arg1;
  Linefeed := Expr^.PseudoFunCallEx.PseudoFun = TpfWriteln;
  WriteArg := Expr^.PseudoFunCallEx.WriteArgs;
  Braces := (not Codegen.IsMultiStatement) and (WriteArg <> nil)
            and ((WriteArg^.Next <> nil) or Linefeed);
  if Braces then OutBegin;
  while WriteArg <> nil do
  begin
    if IsEnumType(WriteArg^.Arg^.TypeIndex) then
    begin
      _OutIndent;
      write(Codegen.Output, 'write_e(&');
      _OutExpressionParensPrec(Dst, 2);
      write(Codegen.Output, ', ');
      OutExpression(WriteArg^.Arg);
      write(Codegen.Output, ', enumvalues',
            WriteArg^.Arg^.TypeIndex^.EnumIndex^.Id, ');');
      _OutNewline
    end
    else
    begin
      _OutIndent;
      write(Codegen.Output, 'write_',
            ShortTypeName(WriteArg^.Arg^.TypeIndex), '(&');
      _OutExpressionParensPrec(Dst, 2);
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
    write(Codegen.Output, 'writeln(&');
    _OutExpressionParensPrec(Dst, 2);
    write(Codegen.Output, ');');
    _OutNewline
  end;
  if Braces then OutEnd
end;

procedure _OutStr(Expr : TExpression);
var Src, Dst : TExpression;
begin
  Src := Expr^.PseudoFunCallEx.Arg1;
  Dst := Expr^.PseudoFunCallEx.Arg2;
  if IsEnumType(Src^.TypeIndex) then
  begin
    _OutIndent;
    OutExpression(Dst);
    write(Codegen.Output, ' = to_str_e(');
    OutExpression(Src);
    write(Codegen.Output, ', enumvalues',
          Src^.TypeIndex^.EnumIndex^.Id, ');');
    _OutNewline
  end
  else
  begin
    _OutIndent;
    OutExpression(Dst);
    write(Codegen.Output, ' = to_str_', ShortTypeName(Src^.TypeIndex), '(');
    OutExpression(Src);
    write(Codegen.Output, ');');
    _OutNewline
  end
end;

procedure _OutNew(Expr : TExpression);
var Ptr : TExpression;
begin
  Ptr := Expr^.PseudoFunCallEx.Arg1;
  _OutIndent;
  OutExpression(Ptr);
  write(Codegen.Output, ' = malloc(sizeof(');
  OutTypeReference(Ptr^.TypeIndex^.PointedTypeIndex);
  write(Codegen.Output, '));');
  _OutNewline
end;

procedure _OutDispose(Expr : TExpression);
var Ptr : TExpression;
begin
  Ptr := Expr^.PseudoFunCallEx.Arg1;
  _OutIndent;
  write(Codegen.Output, 'free(');
  OutExpression(Ptr);
  write(Codegen.Output, ');');
  _OutNewline
end;

procedure _OutOrd(Expr : TExpression);
begin
  if IsOrdinalType(Expr^.PseudoFunCallEx.Arg1^.TypeIndex) then
  begin
    write(Codegen.Output, '(int)');
    _OutExpressionParensPrec(Expr^.PseudoFunCallEx.Arg1, 2)
  end
  else CompileError('Expected an ordinal type, got ' +
                    TypeName(Expr^.PseudoFunCallEx.Arg1^.TypeIndex))
end;

procedure _OutPred(Expr : TExpression);
begin
  if IsOrdinalType(Expr^.PseudoFunCallEx.Arg1^.TypeIndex) then
  begin
    write(Codegen.Output, 'pred(');
    OutExpression(Expr^.PseudoFunCallEx.Arg1);
    write(Codegen.Output, ', ');
    _OutBounds(Expr^.PseudoFunCallEx.Arg1^.TypeIndex);
    write(Codegen.Output, ')')
  end
  else CompileError('Expected an ordinal type, got ' +
                    TypeName(Expr^.PseudoFunCallEx.Arg1^.TypeIndex))
end;

procedure _OutSucc(Expr : TExpression);
begin
  if IsOrdinalType(Expr^.PseudoFunCallEx.Arg1^.TypeIndex) then
  begin
    write(Codegen.Output, 'succ(');
    OutExpression(Expr^.PseudoFunCallEx.Arg1);
    write(Codegen.Output, ', ');
    _OutBounds(Expr^.PseudoFunCallEx.Arg1^.TypeIndex);
    write(Codegen.Output, ')')
  end
  else CompileError('Expected an ordinal type, got ' +
                    TypeName(Expr^.PseudoFunCallEx.Arg1^.TypeIndex))
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

procedure OutAssignReturnValue;
begin
  _OutIndent;
  write(Codegen.Output, 'RESULT = ');
  OutExpression(Rhs);
  write(Codegen.Output, ';');
  _OutNewline
end;

procedure OutAssignToReference;
begin
  _OutIndent;
  OutVariableDeclaration(VarIndex^);
  write(Codegen.Output, ' = &');
  _OutExpressionParensPrec(Rhs, 2);
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
  LimitType : TPsTypeIndex;
  First, Last : TPsVariable;
begin
  LimitType := Iter^.TypeIndex;
  if IsEnumType(LimitType) then LimitType := PrimitiveTypes.PtInteger;
  First := MakeVariable('first', LimitType, false);
  Last := MakeVariable('last', LimitType, false);
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
  _OutExPseudoFunCall(Expr)
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
  Codegen.LastOut := TotNone
end;

procedure CodegenSetOutput;
begin
  Assign(Codegen.Output, Filename);
  Rewrite(Codegen.Output)
end;

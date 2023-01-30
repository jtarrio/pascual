var 
  Codegen : record
    Output : text;
  end;

procedure _OutChar(Chr : char);
begin
  if (Chr = '''') then write(Codegen.Output, '''\''''')
  else if (Chr >= ' ') then write(Codegen.Output, '''', Chr, '''')
  else CompileError('Internal error: escaped chars are not supported yet')
end;

procedure _OutString(var Str : string);
var 
  Pos : integer;
  Chr : char;
begin
  write(Codegen.Output, '"');
  for Pos := 1 to length(Str) do
  begin
    Chr := Str[Pos];
    if Chr = '"' then write(Codegen.Output, '\"')
    else if Chr = '\' then write(Codegen.Output, '\\')
    else if Chr >= ' ' then write(Codegen.Output, Chr)
    else CompileError('Internal error: escaped chars are not supported yet')
  end;
  write(Codegen.Output, '"')
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
    XcVariableAccess : _Precedence := 0;
    XcFieldAccess : _Precedence := 1;
    XcArrayAccess : _Precedence := 1;
    XcPointerAccess : _Precedence := 2;
    XcStringChar : _Precedence := 1;
    XcFunctionRef : _Precedence := 0;
    XcFunctionCall : _Precedence := 1;
    XcUnaryOp : _Precedence := 2;
    XcBinaryOp : _Precedence := _BinOpPrec(Expr);
    else CompileError('Internal error: unknown precedence')
  end
end;

procedure _OutExpressionParens(Expr, Ref : TExpression);
var UseParens : boolean;
begin
  UseParens := _Precedence(Expr) > _Precedence(Ref);
  if UseParens then write(Codegen.Output, '(');
  OutExpression(Expr);
  if UseParens then   write(Codegen.Output, ')')
end;

procedure _OutExImmediate(Expr : TExpression);
begin
  with Expr^.ImmediateEx do
    case Cls of 
      XicBoolean : if BooleanValue then write(Codegen.Output, '1')
                   else write(Codegen.Output, '0');
      XicInteger : writeln(Codegen.Output, IntegerValue);
      XicChar : _OutChar(CharValue);
      XicString : _OutString(StringValue);
      XicEnum : write(Codegen.Output,
                      Expr^.TypeIndex^.EnumIndex^.Values[EnumOrdinal])
    end
end;

procedure _OutExFunctionCall(Expr : TExpression);
var Pos : integer;
begin
  _OutExpressionParens(Expr^.CallEx.FunctionRef, Expr);
  write(Codegen.Output, '(');
  for Pos := 1 to Expr^.CallEx.Args.Size do
  begin
    if Pos <> 1 then write(Codegen.Output, ', ');
    OutExpression(Expr^.CallEx.Args.Values[Pos])
  end;
  write(Codegen.Output, ')')
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
    TkMoreOrEquals : _GetRelationalOp := '>=';
  end;
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
        _OutExpressionParens(Right, Expr)
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
      _OutExpressionParens(Right, Expr)
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
      _OutExpressionParens(Right, Expr)
    end
  end
end;

procedure OutExpression;
begin
  case Expr^.Cls of 
    XcNothing: CompileError('Internal error: ' +
                            'trying to output empty expression');
    XcImmediate: _OutExImmediate(Expr);
    XcToString:
                begin
                  write(Codegen.Output, 'str_of(');
                  OutExpression(Expr^.ToStringEx.Parent);
                  write(Codegen.Output, ')')
                end;
    XcVariableAccess : write(Codegen.Output,
                             Expr^.VariableEx.VariableIndex^.Name);
    XcFieldAccess:
                   begin
                     _OutExpressionParens(Expr^.FieldEx.Parent, Expr);
                     write(Codegen.Output, '.',
                           Expr^.TypeIndex^.RecordIndex^
                           .Fields[Expr^.FieldEx.FieldNumber].Name)
                   end;
    XcArrayAccess:
                   begin
                     _OutExpressionParens(Expr^.ArrayEx.Parent, Expr);
                     write(Codegen.Output, '[');
                     OutExpression(ExBinaryOp(Expr^.ArrayEx.Subscript,
                                   Expr^.TypeIndex^.ArrayIndex^.LowBound,
                                   TkMinus));
                     write(Codegen.Output, ']')
                   end;
    XcPointerAccess:
                     begin
                       write(Codegen.Output, '*');
                       _OutExpressionParens(Expr^.PointerEx.Parent, Expr);
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
    XcUnaryOp: _OutExUnaryOp(Expr);
    XcBinaryOp: _OutExBinaryOp(Expr);
  end
end;

procedure OutBegin;
begin
  writeln(Codegen.Output, '{')
end;

procedure OutEnd;
begin
  writeln(Codegen.Output, '}')
end;

procedure OutEnumValues;
var 
  PosInEnum : integer;
begin
  write(Codegen.Output, 'const char* enumvalues',
        EnumIndex^.Id,
        '[] = { ');
  for PosInEnum := 1 to EnumIndex^.Size do
  begin
    if PosInEnum <> 1 then write(Codegen.Output, ', ');
    write(Codegen.Output, '"', EnumIndex^.Values[PosInEnum],
          '"')
  end;
  writeln(Codegen.Output, ' };')
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

function OutVariableName;
begin
  if IsReference then OutVariableName := '*' + Name
  else OutVariableName := Name
end;

function OutReturnVariableName;
begin
  OutReturnVariableName := 'return_' + Name
end;

procedure OutTypeReference;
begin
  if TypeIndex = nil then write(Codegen.Output, 'void')
  else if TypeIndex^.Cls = TtcPointer then
  begin
    OutTypeReference(TypeIndex^.PointedTypeIndex);
    write(Codegen.Output, '*')
  end
  else if TypeIndex^.Cls = TtcBoolean then write(Codegen.Output, 'PBoolean')
  else if TypeIndex^.Cls = TtcInteger then write(Codegen.Output, 'int')
  else if TypeIndex^.Cls = TtcChar then write(Codegen.Output, 'char')
  else if TypeIndex^.Cls = TtcString then write(Codegen.Output, 'PString')
  else if TypeIndex^.Cls = TtcText then write(Codegen.Output, 'PFile')
  else if TypeIndex^.Cls = TtcEnum then
         write(Codegen.Output, 'enum enum', TypeIndex^.EnumIndex^.Id)
  else if TypeIndex^.Cls = TtcRecord then
         write(Codegen.Output, 'struct record', TypeIndex^.RecordIndex^.Id)
  else if TypeIndex^.Cls = TtcArray then
  begin
    OutTypeReference(TypeIndex^.ArrayIndex^.TypeIndex);
    write(Codegen.Output, '*')
  end
  else
    CompileError('Error writing type reference: ' + TypeName(TypeIndex))
end;

procedure OutNameAndRecord(Name : string; RecordIndex : TPsRecordIndex);
var 
  Pos : integer;
  NumVariant : integer;
begin
  NumVariant := 0;
  write(Codegen.Output, 'struct record', RecordIndex^.Id);
  if not RecordIndex^.HasBeenDefined then
  begin
    write(Codegen.Output, ' { ');
    for Pos := 1 to RecordIndex^.Size do
    begin
      if (RecordIndex^.NumVariants > NumVariant)
         and (RecordIndex^.VariantBounds[NumVariant + 1] = Pos) then
      begin
        NumVariant := NumVariant + 1;
        if NumVariant = 1 then
          write(Codegen.Output, 'union { ')
        else
          write(Codegen.Output, '}; ');
        write(Codegen.Output, 'struct { ')
      end;
      OutNameAndType(RecordIndex^.Fields[Pos].Name,
                     RecordIndex^.Fields[Pos].TypeIndex);
      write(Codegen.Output, '; ')
    end;
    if NumVariant > 0 then
      write(Codegen.Output, '}; }; ');
    write(Codegen.Output, '}');
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
    for Pos := 1 to EnumIndex^.Size do
    begin
      if Pos > 1 then
        write(Codegen.Output, ', ');
      write(Codegen.Output, EnumIndex^.Values[Pos])
    end;
    write(Codegen.Output, '}');
    EnumIndex^.HasBeenDefined := true
  end;
  write(Codegen.Output, ' ', Name)
end;

procedure OutNameAndType;
var 
  Arr : TPsArrayDef;
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
         write(Codegen.Output, 'PBoolean ', Name)
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
  else if TypeIndex^.Cls = TtcRecord then
         OutNameAndRecord(Name, TypeIndex^.RecordIndex)
  else if TypeIndex^.Cls = TtcArray then
  begin
    Arr := TypeIndex^.ArrayIndex^;
    OutNameAndType(Name, Arr.TypeIndex);
    write(Codegen.Output, '[1 + ');
    OutExpression(Arr.HighBound);
    write(Codegen.Output, ' - ');
    OutExpression(Arr.LowBound);
    write(Codegen.Output, ']')
  end
  else
    CompileError('Error writing name and type: ' + Name + ', ' +
                 TypeName(TypeIndex))
end;

procedure OutTypeDefinition;
var 
  Name : string;
begin
  Name := TypeIndex^.Name;
  if TypeIndex^.AliasFor = nil then
    CompileError('Type ' + Name + ' is not an alias');
  write(Codegen.Output, 'typedef ');
  OutNameAndType(Name, TypeIndex^.AliasFor);
  writeln(Codegen.Output, ';');
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

procedure OutVariableDeclaration;
begin
  OutNameAndType(OutVariableName(VarDef.Name, VarDef.IsReference),
  VarDef.TypeIndex)
end;

procedure OutVariableDefinition;
begin
  if VarIndex^.IsConstant then
    write(Codegen.Output, 'const ');
  OutVariableDeclaration(VarIndex^);
  writeln(Codegen.Output, ';');
end;

procedure OutConstantDefinitionBegin;
begin
  write(Codegen.Output, 'const ');
  OutVariableDeclaration(VarIndex^);
  write(Codegen.Output, ' = ');
end;

procedure OutConstantDefinitionEnd;
begin
  writeln(Codegen.Output, ';')
end;

procedure OutFunctionPrototype;
var 
  Pos : integer;
begin
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
  OutFunctionPrototype(FnIndex^);
  writeln(Codegen.Output, ';')
end;

procedure OutFunctionDefinition;
begin
  OutFunctionPrototype(FnIndex^);
  writeln(Codegen.Output, ' {');
  if FnIndex^.ReturnTypeIndex <> nil then
  begin
    OutNameAndType(OutReturnVariableName(FnIndex^.Name),
    FnIndex^.ReturnTypeIndex);
    writeln(Codegen.Output, ';')
  end
end;

procedure OutFunctionEnd;
begin
  if FnIndex^.ReturnTypeIndex <> nil then
    writeln(Codegen.Output, 'return ',
            OutReturnVariableName(FnIndex^.Name), ';');
  writeln(Codegen.Output, '}')
end;

procedure OutProgramHeading;
begin
  writeln(Codegen.Output, '/* Program: ', Name, ' */');
  writeln(Codegen.Output, '#include "pascual.h"')
end;

function ShortTypeName(TypeIndex : TPsTypeIndex) : char;
begin
  if IsBooleanType(TypeIndex) then ShortTypeName := 'b'
  else if IsIntegerType(TypeIndex) then ShortTypeName := 'i'
  else if IsCharType(TypeIndex) then ShortTypeName := 'c'
  else if IsStringType(TypeIndex) then ShortTypeName := 's'
  else if IsEnumType(TypeIndex) then ShortTypeName := 'e'
  else CompileError('No short type name exists for ' + TypeName(TypeIndex))
end;

procedure OutRead;
begin
  write(Codegen.Output, 'read_', ShortTypeName(OutVar^.TypeIndex), '(&');
  OutExpression(Src);
  write(Codegen.Output,', &');
  OutExpression(OutVar);
  writeln(Codegen.Output, ');')
end;

procedure OutReadln;
begin
  write(Codegen.Output, 'readln(&');
  OutExpression(Src);
  writeln(Codegen.Output, ');')
end;

procedure OutWrite;
begin
  Expr := ExEnsureEvaluation(Expr);
  if IsEnumType(Expr^.TypeIndex) then
  begin
    write(Codegen.Output, 'write_e(&');
    OutExpression(Dst);
    write(Codegen.Output, ', ');
    OutExpression(Expr);
    writeln(Codegen.Output, ', enumvalues',
            Expr^.TypeIndex^.EnumIndex^.Id, ');')
  end
  else
  begin
    write(Codegen.Output, 'write_', ShortTypeName(Expr^.TypeIndex), '(&');
    OutExpression(Dst);
    write(Codegen.Output, ', ');
    OutExpression(Expr);
    writeln(Codegen.output, ');')
  end
end;

procedure OutWriteln;
begin
  write(Codegen.Output, 'writeln(&');
  OutExpression(Dst);
  writeln(Codegen.Output, ');')
end;

procedure OutStr;
begin
  Expr := ExEnsureEvaluation(Expr);
  if IsEnumType(Expr^.TypeIndex) then
  begin
    OutExpression(Dst);
    write(Codegen.Output, ' = to_str_e(');
    OutExpression(Expr);
    writeln(Codegen.Output, ', enumvalues',
            Expr^.TypeIndex^.EnumIndex^.Id, ');')
  end
  else
  begin
    OutExpression(Dst);
    write(Codegen.Output, ' = to_str_', ShortTypeName(Expr^.TypeIndex), '(');
    OutExpression(Expr);
    writeln(Codegen.Output, ');')
  end;
end;

procedure OutNew;
begin
  OutExpression(Dst);
  write(Codegen.Output, ' = malloc(sizeof(');
  OutTypeReference(Dst^.TypeIndex^.PointedTypeIndex);
  writeln(Codegen.Output, '));')
end;

procedure OutDispose;
begin
  write(Codegen.Output, 'free(');
  OutExpression(Dst);
  writeln(Codegen.Output, ');')
end;

procedure OutAssign;
begin
  OutExpression(Lhs);
  write(Codegen.Output, ' = ');
  OutExpression(Rhs);
  writeln(Codegen.Output, ';')
end;

procedure OutAssignReturnValue;
begin
  write(Codegen.Output, 'return_', Lhs^.FunctionEx.FunctionIndex^.Name, ' = ')
  ;
  OutExpression(Rhs);
  writeln(Codegen.Output, ';')
end;

procedure OutAssignToReference;
begin
  OutVariableDeclaration(VarIndex^);
  write(Codegen.Output, ' = &(', );
  OutExpression(Rhs);
  writeln(Codegen.Output, ');')
end;

procedure OutIf;
begin
  write(Codegen.Output, 'if (');
  OutExpression(Expr);
  write(Codegen.Output, ') ')
end;

procedure OutElse;
begin
  write(Codegen.Output, ' else ')
end;

procedure OutCaseBegin;
begin
  write(Codegen.Output, 'switch (');
  OutExpression(CaseIndex);
  writeln(Codegen.Output, ') {')
end;

procedure OutCaseStatementBegin;
begin
  write(Codegen.Output, 'case ');
  OutExpression(CaseLabel);
  write(Codegen.Output, ': ')
end;

procedure OutCaseStatementEnd;
begin
  writeln(CodeGen.Output, 'break;')
end;

procedure OutCaseElseBegin;
begin
  write(CodeGen.Output, 'default: ')
end;

procedure OutCaseElseEnd;
begin
  writeln(CodeGen.Output, 'break;')
end;

procedure OutCaseEnd;
begin
  writeln(CodeGen.Output, '}')
end;

procedure OutRepeatBegin;
begin
  writeln(Codegen.Output, 'do {')
end;

procedure OutRepeatEnd;
begin
  write(Codegen.Output, '} while (!(');
  OutExpression(Expr);
  writeln(Codegen.Output, '));')
end;

procedure OutWhileBegin;
begin
  write(Codegen.Output, 'while (');
  OutExpression(Expr);
  write(Codegen.Output, ') ')
end;

procedure OutWhileEnd;
begin
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
  writeln(Codegen.Output, '{');
  OutVariableDeclaration(First);
  write(Codegen.Output, ' = ');
  OutExpression(FirstExpr);
  writeln(Codegen.Output, ';');
  OutVariableDeclaration(Last);
  write(Codegen.Output, ' = ');
  OutExpression(LastExpr);
  writeln(Codegen.Output, ';');
  write(Codegen.Output, 'if (first ');
  if Ascending then
    write(Codegen.Output, '<=')
  else
    write(Codegen.Output, '>=');
  writeln(Codegen.Output, ' last) {');
  OutExpression(Iter);
  writeln(Codegen.Output, ' = first;');
  writeln(Codegen.Output, 'while (1) {');
end;

procedure OutForEnd;
begin
  write(Codegen.Output, 'if (');
  OutExpression(Iter);
  writeln(Codegen.Output, ' == last) break;');
  if Ascending then
    write(Codegen.Output, '++')
  else
    write(Codegen.Output, '--');
  OutExpression(Iter);
  writeln(Codegen.Output, ';');
  writeln(Codegen.Output, '}');
  writeln(Codegen.Output, '}');
  writeln(Codegen.Output, '}');
end;

procedure OutProcedureCall;
begin
  OutExpression(Expr);
  writeln(Codegen.Output, ';')
end;

procedure OutEmptyStatement;
begin
  writeln(Codegen.Output, ';')
end;

procedure OutProgramBegin;
begin
  writeln(Codegen.Output, 'void pascual_main() {');
end;

procedure OutProgramEnd;
begin
  writeln(Codegen.Output, '}')
end;

procedure CodegenReset;
begin
  Codegen.Output := Output
end;

procedure CodegenSetOutput;
begin
  Assign(Codegen.Output, Filename);
  Rewrite(Codegen.Output)
end;

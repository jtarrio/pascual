function ExprEvaluate(Expr : TPsExpression) : TPsExpression;
begin
  if Expr.Cls = TecFunction then
  begin
    if Defs.Functions[Expr.FunctionIndex].ArgCount <> 0 then
      CompileError('Function requires arguments');
    Expr.Value := Expr.Value + '()';
    Expr.TypeIndex := Defs.Functions[Expr.FunctionIndex].ReturnTypeIndex;
    if Expr.TypeIndex = 0 then Expr.Cls := TecStatement
    else Expr.Cls := TecValue
  end;
  ExprEvaluate := Expr
end;

function ExprCoerce(Expr : TPsExpression; TypeIndex : TPsTypeIndex)
: TPsExpression;
begin
  Expr := ExprEvaluate(Expr);
  if Expr.Cls <> TecValue then
    CompileError('Cannot assign function to ' + TypeName(TypeIndex));
  if IsCharType(Expr.TypeIndex) and IsStringType(TypeIndex) then
  begin
    Expr.TypeIndex := PrimitiveTypes.PtString;
    Expr.Value := 'str_of(' + Expr.Value + ')';
  end
  else if IsNilType(Expr.TypeIndex) and IsPointerType(TypeIndex) then
         Expr.TypeIndex := TypeIndex
  else if not IsSameType(Expr.TypeIndex, TypeIndex) then
         CompileError('Cannot treat ' + TypeName(Expr.TypeIndex) + ' as ' +
         TypeName(TypeIndex));
  ExprCoerce := Expr
end;

function ExprIsVariable(Expr : TPsExpression) : boolean;
begin
  ExprIsVariable := (Expr.Cls = TecValue) and (Expr.TypeIndex <> 0)
                    and not Expr.IsConstant
end;

function ExprNilConstant : TPsExpression;
var 
  Expr : TPsExpression;
begin
  Expr.TypeIndex := PrimitiveTypes.PtNil;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  Expr.Value := '((void*)0)';
  ExprNilConstant := Expr
end;

function ExprBooleanConstant(Value : boolean) : TPsExpression;
var 
  Expr : TPsExpression;
begin
  Expr.TypeIndex := PrimitiveTypes.PtBoolean;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  if Value then
    Expr.Value := '1'
  else
    Expr.Value := '0';
  ExprBooleanConstant := Expr
end;

function ExprStringConstant(Value : string) : TPsExpression;
var 
  Expr : TPsExpression;
  Size : string;
  Pos : integer;
  InStr, LastQuote : boolean;
  Len : integer;
begin
  InStr := false;
  LastQuote := false;
  Expr.Value := '';
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  Len := 0;
  for Pos := 1 to Length(Value) do
  begin
    if Value[Pos] = '''' then
    begin
      InStr := not InStr;
      if InStr and LastQuote then
      begin
        Expr.Value := Expr.Value + '\''';
        Len := Len + 1
      end
      else
        LastQuote := not InStr
    end
    else
    begin
      LastQuote := false;
      if Value[Pos] = '"' then Expr.Value := Expr.Value + '\"'
      else if Value[Pos] = '\' then Expr.Value := Expr.Value + '\\'
      else Expr.Value := Expr.Value + Value[Pos];
      Len := Len + 1
    end
  end;
  if Len = 1 then
  begin
    Expr.Value := '''' + Expr.Value + '''';
    Expr.TypeIndex := PrimitiveTypes.PtChar;
  end
  else
  begin
    Str(Len, Size);
    Expr.Value := 'str_make(' + Size + ', "' + Expr.Value + '")';
    Expr.TypeIndex := PrimitiveTypes.PtString;
  end;
  ExprStringConstant := Expr
end;

function ExprNumberConstant(Value : string) : TPsExpression;
var 
  Expr : TPsExpression;
begin
  Expr.TypeIndex := PrimitiveTypes.PtInteger;
  Expr.Value := Value;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  ExprNumberConstant := Expr
end;

function ExprVariableAccess(VarIndex : TPsVariableIndex) : TPsExpression;
var 
  Expr : TPsExpression;
begin
  with Defs.Variables[VarIndex] do
  begin
    if IsReference then Expr.Value := '*' + Name
    else Expr.Value := Name;
    Expr.Cls := TecValue;
    Expr.TypeIndex := TypeIndex;
    Expr.IsConstant := false
  end;
  ExprVariableAccess := Expr
end;

function ExprFunctionReference(FnIndex : TPsFunctionIndex) : TPsExpression;
var 
  Expr : TPsExpression;
begin
  Expr.Value := Defs.Functions[FnIndex].Name;
  Expr.Cls := TecFunction;
  Expr.FunctionIndex := FnIndex;
  Expr.IsConstant := false;
  ExprFunctionReference := Expr
end;

function ExprEnumValue(Ordinal : integer; TypeIndex : TPsTypeIndex)
: TPsExpression;
var 
  Expr : TPsExpression;
begin
  with Defs.Enums[Defs.Types[TypeIndex].EnumIndex] do
  begin
    Expr.Value := Values[Ordinal];
    Expr.Cls := TecValue;
    Expr.TypeIndex := TypeIndex;
    Expr.IsConstant := true
  end;
  ExprEnumValue := Expr
end;

function ExprFieldAccess(Base : TPsExpression; Name : string) : TPsExpression;
var 
  Expr : TPsExpression;
begin
  if (Base.Cls <> TecValue) or not IsRecordType(Base.TypeIndex) then
    CompileError('Not a record');
  Expr.Value := Base.Value;
  if Expr.Value[1] = '*' then Expr.Value := '(' + Expr.Value + ')';
  Expr.Value := Expr.Value + '.' + Name;
  Expr.Cls := TecValue;
  Expr.TypeIndex := FindFieldType(Base.TypeIndex, Name, {Required=}true);
  Expr.IsConstant := Base.IsConstant;
  ExprFieldAccess := Expr;
end;

function ExprArrayAccess(Base : TPsExpression; Idx : TPsExpression)
: TPsExpression;
var 
  Expr : TPsExpression;
begin
  if (Base.Cls = TecValue) and IsStringType(Base.TypeIndex) then
  begin
    if (Idx.Cls <> TecValue) or not IsIntegerType(Idx.TypeIndex) then
      CompileError('String subscript is not an integer');
    Expr.Value := Base.Value + '.chr[' + Idx.Value + ']';
    Expr.TypeIndex := PrimitiveTypes.PtChar;
    Expr.IsConstant := Base.IsConstant;
  end
  else if (Base.Cls = TecValue) and IsArrayType(Base.TypeIndex) then
  begin
    if (Idx.Cls <> TecValue) or not IsIntegerType(Idx.TypeIndex) then
      CompileError('Array subscript is not an integer');
    with Defs.Arrays[Defs.Types[Base.TypeIndex].ArrayIndex] do
    begin
      Expr.Value := Base.Value + '[(' + Idx.Value + ') - ' + LowBound + ']';
      Expr.Cls := TecValue;
      Expr.TypeIndex := TypeIndex;
      Expr.IsConstant := Base.IsConstant
    end;
  end
  else CompileError('Not a string or array');
  ExprArrayAccess := Expr
end;

function ExprPointerDeref(Ptr : TPsExpression) : TPsExpression;
var 
  Expr : TPsExpression;
begin
  if (Ptr.Cls <> TecValue) or not IsPointerType(Ptr.TypeIndex) then
    CompileError('Not a pointer');
  Expr.Value := '*(' + Ptr.Value + ')';
  Expr.Cls := TecValue;
  Expr.TypeIndex := Defs.Types[Ptr.TypeIndex].PointedTypeIndex;
  Expr.IsConstant := Ptr.IsConstant;
  ExprPointerDeref := Expr
end;

function _ExprBinaryOpInteger(Left : TPsExpression; Op : TLxTokenId;
                             Right : TPsExpression) : TPsExpression;
var 
  Oper, Cmp : string;
  Expr : TPsExpression;
begin
  Oper := '';
  Cmp := '';
  case Op of 
    TkPlus : Oper := '+';
    TkMinus : Oper := '-';
    TkAsterisk : Oper := '*';
    TkDiv : Oper := '/';
    TkMod : Oper := '%';
    TkAnd : Oper := '&';
    TkOr : Oper := '|';
    TkEquals : Cmp := '==';
    TkNotEquals : Cmp := '!=';
    TkLessthan : Cmp := '<';
    TkMorethan : Cmp := '>';
    TkLessOrEquals : Cmp := '<=';
    TkMoreOrEquals : Cmp := '>=';
    else
      CompileError('Expected integer binary operator, found ' + LxTokenName(Op))
  end;
  if Cmp = '' then
    Expr.TypeIndex := PrimitiveTypes.PtInteger
  else
    Expr.TypeIndex := PrimitiveTypes.PtBoolean;
  Expr.Value := Left.Value + ' ' + Oper + Cmp + ' ' + Right.Value;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  _ExprBinaryOpInteger := Expr
end;

function _ExprBinaryOpBoolean(Left : TPsExpression; Op : TLxTokenId;
                             Right : TPsExpression) : TPsExpression;
var 
  Oper : string;
  Expr : TPsExpression;
begin
  case Op of 
    TkAnd : Oper := '&&';
    TkOr : Oper := '||';
    TkEquals : Oper := '==';
    TkNotEquals : Oper := '!=';
    TkLessthan : Oper := '<';
    TkMorethan : Oper := '>';
    TkLessOrEquals : Oper := '<=';
    TkMoreOrEquals : Oper := '>=';
    else
      CompileError('Expected boolean binary operator, found ' + LxTokenName(Op))
  end;
  Expr.TypeIndex := PrimitiveTypes.PtBoolean;
  Expr.Value := Left.Value + ' ' + Oper + ' ' + Right.Value;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  _ExprBinaryOpBoolean := Expr
end;


function _ExprBinaryOpStringy(Left : TPsExpression; Op : TLxTokenId;
                             Right : TPsExpression) : TPsExpression;
var 
  FName, Cmp : string;
  Expr : TPsExpression;
  LType, RType : char;
begin
  FName := 'cmp';
  Cmp := '';
  case Op of 
    TkPlus : FName := 'cat';
    TkEquals : Cmp := '==';
    TkNotEquals : Cmp := '!=';
    TkLessthan : Cmp := '<';
    TkMorethan : Cmp := '>';
    TkLessOrEquals : Cmp := '<=';
    TkMoreOrEquals : Cmp := '>=';
    else
      CompileError('Expected string binary operator, found ' + LxTokenName(Op))
  end;

  if IsCharType(Left.TypeIndex) then LType := 'c'
  else LType := 's';
  if IsCharType(Right.TypeIndex) then RType := 'c'
  else RType := 's';
  FName := FName + '_' + LType + RType;

  Expr.TypeIndex := PrimitiveTypes.PtString;
  Expr.Value := FName + '(' + Left.Value + ', ' + Right.Value + ')';
  if Cmp <> '' then
  begin
    Expr.TypeIndex := PrimitiveTypes.PtBoolean;
    Expr.Value := Expr.Value + ' ' + Cmp + ' 0'
  end;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  _ExprBinaryOpStringy := Expr
end;

function _ExprBinaryOpEnum(Left : TPsExpression; Op : TLxTokenId;
                          Right : TPsExpression) : TPsExpression;
var 
  Cmp : string;
  Expr : TPsExpression;
begin
  Cmp := '';
  case Op of 
    TkEquals : Cmp := '==';
    TkNotEquals : Cmp := '!=';
    TkLessthan : Cmp := '<';
    TkMorethan : Cmp := '>';
    TkLessOrEquals : Cmp := '<=';
    TkMoreOrEquals : Cmp := '>=';
    else
      CompileError('Expected ordinal binary operator, found ' + LxTokenName(Op))
  end;
  Expr.TypeIndex := PrimitiveTypes.PtBoolean;
  Expr.Value := Left.Value + ' ' + Cmp + ' ' + Right.Value;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  _ExprBinaryOpEnum := Expr
end;

function _ExprBinaryOpPointer(Left : TPsExpression; Op : TLxTokenId;
                             Right : TPsExpression) : TPsExpression;
var 
  Cmp : string;
  Expr : TPsExpression;
begin
  Cmp := '';
  case Op of 
    TkEquals : Cmp := '==';
    TkNotEquals : Cmp := '!=';
    else
      CompileError('Expected pointer binary operator, found ' + LxTokenName(Op))
  end;
  Expr.TypeIndex := PrimitiveTypes.PtBoolean;
  Expr.Value := Left.Value + ' ' + Cmp + ' ' + Right.Value;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  _ExprBinaryOpPointer := Expr
end;

function ExprBinaryOp(Left : TPsExpression; Op : TLxTokenId;
                      Right : TPsExpression) : TPsExpression;
begin
  Left := ExprEvaluate(Left);
  Right := ExprEvaluate(Right);
  if IsBooleanType(Left.TypeIndex) and IsBooleanType(Right.TypeIndex) then
    ExprBinaryOp := _ExprBinaryOpBoolean(Left, Op, Right)
  else if IsIntegerType(Left.TypeIndex)
          and IsIntegerType(Right.TypeIndex) then
         ExprBinaryOp := _ExprBinaryOpInteger(Left, Op, Right)
  else if IsStringyType(Left.TypeIndex)
          and IsStringyType(Right.TypeIndex) then
         ExprBinaryOp := _ExprBinaryOpStringy(Left, Op, Right)
  else if IsEnumType(Left.TypeIndex)
          and IsSameType(Left.TypeIndex, Right.TypeIndex) then
         ExprBinaryOp := _ExprBinaryOpEnum(Left, Op, Right)
  else if ArePointersCompatible(Left.TypeIndex, Right.TypeIndex) then
         ExprBinaryOp := _ExprBinaryOpPointer(Left, Op, Right)
  else
    CompileError('Type mismatch for operator ' + LxTokenName(Op) + ': ' +
    TypeName(Left.TypeIndex) + ' and ' + TypeName(Right.TypeIndex))
end;

function ExprUnaryOp(Op : TLxTokenId; Expr : TPsExpression) : TPsExpression;
begin
  Expr := ExprEvaluate(Expr);
  if Op = TkNot then
  begin
    if not IsBooleanType(Expr.TypeIndex) then
      CompileError('Expected boolean expression, got ' +
                   TypeName(Expr.TypeIndex));
    Expr.Value := '!' + Expr.Value;
  end
  else if Op = TkMinus then
  begin
    if not IsIntegerType(Expr.TypeIndex) then
      CompileError('Expected numeric expression, got ' +
                   TypeName(Expr.TypeIndex));
    Expr.Value := '-' + Expr.Value;
  end
  else
    CompileError('Expected unary operator, found ' + LxTokenName(Op));
  Expr.IsConstant := true;
  ExprUnaryOp := Expr
end;

function ExprParentheses(Expr : TPsExpression) : TPsExpression;
begin
  Expr.Value := '(' + Expr.Value + ')';
  ExprParentheses := Expr
end;

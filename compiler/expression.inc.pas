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

function ExprMakeNilConstant : TPsExpression;
var 
  Expr : TPsExpression;
begin
  Expr.TypeIndex := PrimitiveTypes.PtNil;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  Expr.Value := '((void*)0)';
  ExprMakeNilConstant := Expr
end;

function ExprMakeBooleanConstant(Value : boolean) : TPsExpression;
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
  ExprMakeBooleanConstant := Expr
end;

function ExprMakeStringConstant(Value : string) : TPsExpression;
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
  ExprMakeStringConstant := Expr
end;

function ExprMakeNumberConstant(Value : string) : TPsExpression;
var 
  Expr : TPsExpression;
begin
  Expr.TypeIndex := PrimitiveTypes.PtInteger;
  Expr.Value := Value;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  ExprMakeNumberConstant := Expr
end;

function ExprBinaryOpInteger(Left : TPsExpression; Op : TLxTokenId;
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
  ExprBinaryOpInteger := Expr
end;

function ExprBinaryOpBoolean(Left : TPsExpression; Op : TLxTokenId;
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
  ExprBinaryOpBoolean := Expr
end;


function ExprBinaryOpStringy(Left : TPsExpression; Op : TLxTokenId;
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
  ExprBinaryOpStringy := Expr
end;

function ExprBinaryOpEnum(Left : TPsExpression; Op : TLxTokenId;
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
  ExprBinaryOpEnum := Expr
end;

function ExprBinaryOpPointer(Left : TPsExpression; Op : TLxTokenId;
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
  ExprBinaryOpPointer := Expr
end;

function ExprBinaryOp(Left : TPsExpression; Op : TLxTokenId; Right :
                      TPsExpression) : TPsExpression;
begin
  Left := ExprEvaluate(Left);
  Right := ExprEvaluate(Right);
  if IsBooleanType(Left.TypeIndex) and IsBooleanType(Right.TypeIndex) then
    ExprBinaryOp := ExprBinaryOpBoolean(Left, Op, Right)
  else if IsIntegerType(Left.TypeIndex)
          and IsIntegerType(Right.TypeIndex) then
         ExprBinaryOp := ExprBinaryOpInteger(Left, Op, Right)
  else if IsStringyType(Left.TypeIndex)
          and IsStringyType(Right.TypeIndex) then
         ExprBinaryOp := ExprBinaryOpStringy(Left, Op, Right)
  else if IsEnumType(Left.TypeIndex)
          and IsSameType(Left.TypeIndex, Right.TypeIndex) then
         ExprBinaryOp := ExprBinaryOpEnum(Left, Op, Right)
  else if ArePointersCompatible(Left.TypeIndex, Right.TypeIndex) then
         ExprBinaryOp := ExprBinaryOpPointer(Left, Op, Right)
  else
    CompileError('Type mismatch for operator ' + LxTokenName(Op) + ': ' +
    TypeName(Left.TypeIndex) + ' and ' + TypeName(Right.TypeIndex))
end;

function ExprUnaryOp(Op : TLxTokenId; Expr : TPsExpression)
: TPsExpression;
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

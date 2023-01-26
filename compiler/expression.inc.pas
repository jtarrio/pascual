function Evaluate(Expr : TPsExpression) : TPsExpression;
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
  Evaluate := Expr
end;

function CoerceType(Expr : TPsExpression; TypeIndex : TPsTypeIndex)
: TPsExpression;
begin
  Expr := Evaluate(Expr);
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
         CompileError('Cannot assign ' + TypeName(Expr.TypeIndex) + ' to ' +
         TypeName(TypeIndex));
  CoerceType := Expr
end;

function IsVariableExpression(Expr : TPsExpression) : boolean;
begin
  IsVariableExpression := (Expr.Cls = TecValue) and (Expr.TypeIndex <> 0)
                          and not Expr.IsConstant
end;

function GenNilConstant : TPsExpression;
var 
  Expr : TPsExpression;
begin
  Expr.TypeIndex := PrimitiveTypes.PtNil;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  Expr.Value := '((void*)0)';
  GenNilConstant := Expr
end;

function GenBooleanConstant(Value : boolean) : TPsExpression;
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
  GenBooleanConstant := Expr
end;

function GenStringConstant(Value : string) : TPsExpression;
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
  GenStringConstant := Expr
end;

function GenNumberConstant(Value : string) : TPsExpression;
var 
  Expr : TPsExpression;
begin
  Expr.TypeIndex := PrimitiveTypes.PtInteger;
  Expr.Value := Value;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  GenNumberConstant := Expr
end;

function ShortTypeName(TypeIndex : TPsTypeIndex) : char;
begin
  if IsBooleanType(TypeIndex) then ShortTypeName := 'b'
  else if IsIntegerType(TypeIndex) then ShortTypeName := 'i'
  else if IsCharType(TypeIndex) then ShortTypeName := 'c'
  else if IsStringType(TypeIndex) then ShortTypeName := 's'
  else if IsEnumType(TypeIndex) then ShortTypeName := 'e'
  else
    CompileError('No short type name exists for ' + TypeName(TypeIndex))
end;

function IntegerBinaryExpression(Left : TPsExpression; Op : TLxTokenId;
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
  IntegerBinaryExpression := Expr
end;

function BooleanBinaryExpression(Left : TPsExpression; Op : TLxTokenId;
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
  BooleanBinaryExpression := Expr
end;


function StringyBinaryExpression(Left : TPsExpression; Op : TLxTokenId;
                                 Right : TPsExpression) : TPsExpression;
var 
  FName, Cmp : string;
  Expr : TPsExpression;
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

  FName := FName + '_' + ShortTypeName(Left.TypeIndex) +
           ShortTypeName(Right.TypeIndex);

  Expr.TypeIndex := PrimitiveTypes.PtString;
  Expr.Value := FName + '(' + Left.Value + ', ' + Right.Value + ')';
  if Cmp <> '' then
  begin
    Expr.TypeIndex := PrimitiveTypes.PtBoolean;
    Expr.Value := Expr.Value + ' ' + Cmp + ' 0'
  end;
  Expr.Cls := TecValue;
  Expr.IsConstant := true;
  StringyBinaryExpression := Expr
end;

function EnumBinaryExpression(Left : TPsExpression; Op : TLxTokenId;
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
  EnumBinaryExpression := Expr
end;

function PointerBinaryExpression(Left : TPsExpression; Op : TLxTokenId;
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
  PointerBinaryExpression := Expr
end;

function BinaryExpression(Left : TPsExpression; Op : TLxTokenId; Right :
                          TPsExpression) : TPsExpression;
begin
  Left := Evaluate(Left);
  Right := Evaluate(Right);
  if IsBooleanType(Left.TypeIndex) and IsBooleanType(Right.TypeIndex) then
    BinaryExpression := BooleanBinaryExpression(Left, Op, Right)
  else if IsIntegerType(Left.TypeIndex)
          and IsIntegerType(Right.TypeIndex) then
         BinaryExpression := IntegerBinaryExpression(Left, Op, Right)
  else if IsStringyType(Left.TypeIndex)
          and IsStringyType(Right.TypeIndex) then
         BinaryExpression := StringyBinaryExpression(Left, Op, Right)
  else if IsEnumType(Left.TypeIndex)
          and IsSameType(Left.TypeIndex, Right.TypeIndex) then
         BinaryExpression := EnumBinaryExpression(Left, Op, Right)
  else if ArePointersCompatible(Left.TypeIndex, Right.TypeIndex) then
         BinaryExpression := PointerBinaryExpression(Left, Op, Right)
  else
    CompileError('Type mismatch for operator ' + LxTokenName(Op) + ': ' +
    TypeName(Left.TypeIndex) + ' and ' + TypeName(Right.TypeIndex))
end;

function UnaryExpression(Op : TLxTokenId; Expr : TPsExpression)
: TPsExpression;
begin
  Expr := Evaluate(Expr);
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
  UnaryExpression := Expr
end;

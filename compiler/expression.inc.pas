function Evaluate(Expr : TPsExpression) : TPsExpression;
begin
  if Expr.Cls = TecFunction then
  begin
    if Defs.Functions[Expr.FunctionIndex].ArgCount <> 0 then
    begin
      writeln(StdErr, 'Function requires arguments', LxWhereStr);
      halt(1)
    end;
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
  begin
    writeln(StdErr, 'Cannot assign function to ', TypeName(TypeIndex),
    LxWhereStr);
    halt(1)
  end;
  if IsCharType(Expr.TypeIndex) and IsStringType(TypeIndex) then
  begin
    Expr.TypeIndex := PrimitiveTypes.PtString;
    Expr.Value := 'str_of(' + Expr.Value + ')';
  end
  else if IsNilType(Expr.TypeIndex) and IsPointerType(TypeIndex) then
         Expr.TypeIndex := TypeIndex
  else if not IsSameType(Expr.TypeIndex, TypeIndex) then
  begin
    writeln(StdErr, 'Cannot assign ', TypeName(Expr.TypeIndex), ' to ',
    TypeName(TypeIndex), LxWhereStr);
    halt(1)
  end;
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
  InStr : boolean;
  LastQuote : boolean;
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
  begin
    writeln(StdErr, 'No short type name exists for ', TypeName(TypeIndex),
    LxWhereStr);
    halt(1)
  end
end;

function IntegerBinaryExpression(Left : TPsExpression; Op : TLxTokenId;
                                 Right : TPsExpression) : TPsExpression;
var 
  Oper : string;
  Cmp : string;
  Expr : TPsExpression;
begin
  Oper := '';
  Cmp := '';
  if Op = TkPlus then Oper := '+'
  else if Op = TkMinus then Oper := '-'
  else if Op = TkAsterisk then Oper := '*'
  else if Op = TkDiv then Oper := '/'
  else if Op = TkMod then Oper := '%'
  else if Op = TkAnd then Oper := '&'
  else if Op = TkOr then Oper := '|'
  else if Op = TkEquals then Cmp := '=='
  else if Op = TkNotEquals then Cmp := '!='
  else if Op = TkLessthan then Cmp := '<'
  else if Op = TkMorethan then Cmp := '>'
  else if Op = TkLessOrEquals then Cmp := '<='
  else if Op = TkMoreOrEquals then Cmp := '>='
  else
  begin
    writeln(StdErr, 'Expected integer binary operator, found ', Op, LxWhereStr);
    halt(1)
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
  if Op = TkAnd then Oper := '&&'
  else if Op = TkOr then Oper := '||'
  else if Op = TkEquals then Oper := '=='
  else if Op = TkNotEquals then Oper := '!='
  else if Op = TkLessthan then Oper := '<'
  else if Op = TkMorethan then Oper := '>'
  else if Op = TkLessOrEquals then Oper := '<='
  else if Op = TkMoreOrEquals then Oper := '>='
  else
  begin
    writeln(StdErr, 'Expected boolean binary operator, found ', Op, LxWhereStr);
    halt(1)
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
  FName : string;
  Cmp : string;
  Expr : TPsExpression;
begin
  FName := 'cmp';
  Cmp := '';
  if Op = TkPlus then FName := 'cat'
  else if Op = TkEquals then Cmp := '=='
  else if Op = TkNotEquals then Cmp := '!='
  else if Op = TkLessthan then Cmp := '<'
  else if Op = TkMorethan then Cmp := '>'
  else if Op = TkLessOrEquals then Cmp := '<='
  else if Op = TkMoreOrEquals then Cmp := '>='
  else
  begin
    writeln(StdErr, 'Expected string binary operator, found ', Op, LxWhereStr);
    halt(1)
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
  if Op = TkEquals then Cmp := '=='
  else if Op = TkNotEquals then Cmp := '!='
  else if Op = TkLessthan then Cmp := '<'
  else if Op = TkMorethan then Cmp := '>'
  else if Op = TkLessOrEquals then Cmp := '<='
  else if Op = TkMoreOrEquals then Cmp := '>='
  else
  begin
    writeln(StdErr, 'Expected ordinal binary operator, found ', Op, LxWhereStr);
    halt(1)
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
  if Op = TkEquals then Cmp := '=='
  else if Op = TkNotEquals then Cmp := '!='
  else
  begin
    writeln(StdErr, 'Expected pointer binary operator, found ', Op, LxWhereStr);
    halt(1)
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
  begin
    writeln(StdErr, 'Type mismatch for operator ', Op,
            ': ', TypeName(Left.TypeIndex), ' and ', TypeName(Right.TypeIndex),
    LxWhereStr);
    halt(1)
  end
end;

function UnaryExpression(Op : TLxTokenId; Expr : TPsExpression)
: TPsExpression;
begin
  Expr := Evaluate(Expr);
  if Op = TkNot then
  begin
    if not IsBooleanType(Expr.TypeIndex) then
    begin
      writeln(StdErr, 'Expected boolean expression, got ',
              TypeName(Expr.TypeIndex), LxWhereStr);
      halt(1)
    end;
    Expr.Value := '!' + Expr.Value;
  end
  else if Op = TkMinus then
  begin
    if not IsIntegerType(Expr.TypeIndex) then
    begin
      writeln(StdErr, 'Expected numeric expression, got ',
              TypeName(Expr.TypeIndex), LxWhereStr);
      halt(1)
    end;
    Expr.Value := '-' + Expr.Value;
  end
  else
  begin
    writeln(StdErr, 'Expected unary operator, found ', Op, LxWhereStr);
    halt(1)
  end;
  Expr.IsConstant := true;
  UnaryExpression := Expr
end;

function ErrorDescribeExpr(Expr : TSExpression) : string;
begin
  Result := ExDescribe(Expr) + ' (' + TypeName(Expr^.TypePtr) + ')'
end;

procedure ErrorInvalidOperator(Expr : TSExpression; Op : TSEOperator);
begin
  CompileError('Operator ''' + ExDescribeOperator(Op) +
  ''' is not valid in expression ' + ErrorDescribeExpr(Expr))
end;

procedure ErrorInvalidOperator2(Left, Right : TSExpression; Op : TSEOperator);
begin
  CompileError('Invalid operator ''' + ExDescribeOperator(Op) + ''' for ' +
  ErrorDescribeExpr(Left) + ' and ' + ErrorDescribeExpr(Right))
end;

procedure ErrorForType(const Msg : string; Got : TSDType);
begin
  CompileError(Msg + '; got ' + TypeName(Got))
end;

procedure ErrorForExpr(const Msg : string; Got : TSExpression);
begin
  CompileError(Msg + '; got ' + ErrorDescribeExpr(Got))
end;

procedure EnsureRecordType(Typ : TSDType);
begin
  if not IsRecordType(Typ) then ErrorForType('Expected a record', Typ)
end;

procedure EnsureEnumType(Typ : TSDType);
begin
  if not IsEnumType(Typ) then ErrorForType('Expected an enumeration', Typ)
end;

procedure EnsureOrdinalType(Typ : TSDType);
begin
  if not IsOrdinalType(Typ) then ErrorForType('Expected an ordinal', Typ)
end;

procedure EnsureTypedExpr(Expr : TSExpression);
begin
  if IsUntyped(Expr^.TypePtr) then
    ErrorForExpr('Expected a typed expression', Expr)
end;

procedure EnsureRecordExpr(Expr : TSExpression);
begin
  if not IsRecordType(Expr^.TypePtr) then
    ErrorForExpr('Expected a record', Expr)
end;

procedure EnsureArrayExpr(Expr : TSExpression);
begin
  if not IsArrayType(Expr^.TypePtr) then
    ErrorForExpr('Expected an array', Expr)
end;

procedure EnsurePointerExpr(Expr : TSExpression);
begin
  if not IsPointerType(Expr^.TypePtr) then
    ErrorForExpr('Expected a pointer', Expr)
end;

procedure EnsureStringyExpr(Expr : TSExpression);
begin
  if not IsStringyType(Expr^.TypePtr) then
    ErrorForExpr('Expected a string', Expr)
end;

procedure EnsureStringExpr(Expr : TSExpression);
begin
  if not IsStringType(Expr^.TypePtr) then
    ErrorForExpr('Expected a string', Expr)
end;

procedure EnsureNumericExpr(Expr : TSExpression);
begin
  if not IsNumericType(Expr^.TypePtr) then
    ErrorForExpr('Expected a numeric expression', Expr)
end;

procedure EnsureIntegerExpr(Expr : TSExpression);
begin
  if not IsIntegerType(Expr^.TypePtr) then
    ErrorForExpr('Expected an integer', Expr)
end;

procedure EnsureOrdinalExpr(Expr : TSExpression);
begin
  if not IsOrdinalType(Expr^.TypePtr) then
    ErrorForExpr('Expected an ordinal', Expr)
end;

procedure EnsureOrdinalOrPointerExpr(Expr : TSExpression);
begin
  if (not IsOrdinalType(Expr^.TypePtr)) and (not IsPointerType(Expr^.TypePtr)) then
    ErrorForExpr('Expected an ordinal or a pointer', Expr)
end;

procedure EnsureAssignableExpr(Expr : TSExpression);
begin
  if not Expr^.IsAssignable then
    ErrorForExpr('Expected a variable', Expr)
end;

procedure EnsureAddressableExpr(Expr : TSExpression);
begin
  if not Expr^.IsAddressable then
    ErrorForExpr('Expected an addressable value', Expr)
end;

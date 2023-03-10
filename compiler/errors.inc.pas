function ErrorDescribeExpr(Expr : TExpression) : string;
begin
  Result := ExDescribe(Expr) + ' (' + TypeName(Expr^.TypePtr) + ')'
end;

procedure ErrorInvalidOperator(Expr : TExpression; Op : TLxTokenId);
begin
  CompileError('Operator ' + LxTokenName(Op) +
  ' is not valid in expression ' + ErrorDescribeExpr(Expr))
end;

procedure ErrorInvalidOperator2(Left, Right : TExpression; Op : TLxTokenId);
begin
  CompileError('Invalid operator ' + LxTokenName(Op) + ' for ' +
  ErrorDescribeExpr(Left) + ' and ' + ErrorDescribeExpr(Right))
end;

procedure ErrorForType(Msg : string; Got : TPsTypePtr);
begin
  CompileError(Msg + '; got ' + TypeName(Got))
end;

procedure ErrorForExpr(Msg : string; Got : TExpression);
begin
  CompileError(Msg + '; got ' + ErrorDescribeExpr(Got))
end;

procedure EnsureRecordType(Typ : TPsTypePtr);
begin
  if not IsRecordType(Typ) then ErrorForType('Expected a record', Typ)
end;

procedure EnsureEnumType(Typ : TPsTypePtr);
begin
  if not IsEnumType(Typ) then ErrorForType('Expected an enumeration', Typ)
end;

procedure EnsureOrdinalType(Typ : TPsTypePtr);
begin
  if not IsOrdinalType(Typ) then ErrorForType('Expected an ordinal', Typ)
end;

procedure EnsureRecordExpr(Expr : TExpression);
begin
  if not IsRecordType(Expr^.TypePtr) then
    ErrorForExpr('Expected a record', Expr)
end;

procedure EnsureArrayExpr(Expr : TExpression);
begin
  if not IsArrayType(Expr^.TypePtr) then
    ErrorForExpr('Expected an array', Expr)
end;

procedure EnsurePointerExpr(Expr : TExpression);
begin
  if not IsPointerType(Expr^.TypePtr) then
    ErrorForExpr('Expected a pointer', Expr)
end;

procedure EnsureStringyExpr(Expr : TExpression);
begin
  if not IsStringyType(Expr^.TypePtr) then
    ErrorForExpr('Expected a string', Expr)
end;

procedure EnsureStringExpr(Expr : TExpression);
begin
  if not IsStringType(Expr^.TypePtr) then
    ErrorForExpr('Expected a string', Expr)
end;

procedure EnsureIntegerExpr(Expr : TExpression);
begin
  if not IsIntegerType(Expr^.TypePtr) then
    ErrorForExpr('Expected an integer', Expr)
end;

procedure EnsureOrdinalExpr(Expr : TExpression);
begin
  if not IsOrdinalType(Expr^.TypePtr) then
    ErrorForExpr('Expected an ordinal', Expr)
end;

procedure EnsureAssignableExpr(Expr : TExpression);
begin
  if not Expr^.IsAssignable then
    ErrorForExpr('Expected a variable', Expr)
end;

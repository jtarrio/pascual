procedure ErrorInvalidOperator(Expr : TExpression; Op : TLxTokenId);
begin
  CompileError('Operator ' + LxTokenName(Op) +
  ' is not valid in expression ' + ExDescribe(Expr))
end;

procedure ErrorInvalidOperator2(Left, Right : TExpression; Op : TLxTokenId);
begin
  CompileError('Invalid operator ' + LxTokenName(Op) +
  ' for ' + ExDescribe(Left) + ' (' + TypeName(Left^.TypePtr) + ') and ' +
  ExDescribe(Right) + ' (' + TypeName(Right^.TypePtr) + ')')
end;

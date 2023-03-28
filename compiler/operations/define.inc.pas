function _ExOp_MakeUnary(Expr : TExpression; Op : TExOperator;
                       ResultType : TPsTypePtr) : TExpression;
begin
  Result := _NewExpr(XcUnaryOp);
  Result^.Unary.Parent := Expr;
  Result^.Unary.Op := Op;
  Result^.TypePtr := ResultType;
  Result^.IsFunctionResult := Expr^.IsFunctionResult
end;

function _ExOp_MakeBinary(Left, Right : TExpression; Op : TExOperator;
                          ResultType : TPsTypePtr) : TExpression;
begin
  Result := _NewExpr(XcBinaryOp);
  Result^.Binary.Left := Left;
  Result^.Binary.Right := Right;
  Result^.Binary.Op := Op;
  Result^.TypePtr := ResultType;
  Result^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult
end;

{$I arithmetic.inc.pas}
{$I bitwise.inc.pas}
{$I logical.inc.pas}
{$I ordinals.inc.pas}
{$I relational.inc.pas}

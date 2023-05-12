function _ExOp_MakeUnary(Expr : TSExpression; Op : TSEOperator;
                         ResultType : TSDType) : TSExpression;
begin
  ExClearTmpVar(Expr);
  Result := _NewExpr(SecUnaryOp);
  Result^.Unary.Parent := Expr;
  Result^.Unary.Op := Op;
  Result^.TypePtr := ResultType;
  Result^.IsFunctionResult := Expr^.IsFunctionResult
end;

function _ExOp_MakeBinary(Left, Right : TSExpression; Op : TSEOperator;
                          ResultType : TSDType) : TSExpression;
begin
  ExClearTmpVar(Left);
  ExClearTmpVar(Right);
  Result := _NewExpr(SecBinaryOp);
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

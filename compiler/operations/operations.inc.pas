function ExOpNeg(Expr : TExpression) : TExpression;
forward;
function ExOpAdd(Left, Right : TExpression) : TExpression;
forward;
function ExOpSub(Left, Right : TExpression) : TExpression;
forward;
function ExOpMul(Left, Right : TExpression) : TExpression;
forward;
function ExOpDivReal(Left, Right : TExpression) : TExpression;
forward;
function ExOpDivInt(Left, Right : TExpression) : TExpression;
forward;
function ExOpMod(Left, Right : TExpression) : TExpression;
forward;

function ExOpNot(Expr : TExpression) : TExpression;
forward;
function ExOpAnd(Left, Right : TExpression) : TExpression;
forward;
function ExOpOr(Left, Right : TExpression) : TExpression;
forward;
function ExOpXor(Left, Right : TExpression) : TExpression;
forward;

function ExOpShl(Left, Right : TExpression) : TExpression;
forward;
function ExOpShr(Left, Right : TExpression) : TExpression;
forward;

function ExOpIn(Left, Right : TExpression) : TExpression;
forward;
function ExOpEq(Left, Right : TExpression) : TExpression;
forward;
function ExOpNe(Left, Right : TExpression) : TExpression;
forward;
function ExOpLt(Left, Right : TExpression) : TExpression;
forward;
function ExOpGt(Left, Right : TExpression) : TExpression;
forward;
function ExOpLtEq(Left, Right : TExpression) : TExpression;
forward;
function ExOpGtEq(Left, Right : TExpression) : TExpression;
forward;

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
{$I relational.inc.pas}

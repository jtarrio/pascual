procedure DisposeExpr(var Expr : TExpression);
forward;

function CopyExpr(Expr : TExpression) : TExpression;
forward;

function DescribeExpr(Expr : TExpression; Levels : integer) : string;
forward;

procedure ExMarkInitialized(Lhs : TExpression);
forward;

function ExUnaryOp(Parent : TExpression; Op : TLxTokenId) : TExpression;
forward;

function ExBinaryOp(Left, Right : TExpression;
                    Op : TLxTokenId) : TExpression;
forward;

function ExGetOrdinal(Expr : TExpression) : integer;
forward;

function ExGetAntiOrdinal(Ordinal : integer;
                          TypePtr : TPsTypePtr) : TExpression;
forward;

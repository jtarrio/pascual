procedure ExDispose(var Expr : TExpression);
forward;

function ExCopy(Expr : TExpression) : TExpression;
forward;

function ExDescribeOperator(Op : TExOperator) : string;
forward;

function ExDescribe(Expr : TExpression) : string;
forward;

procedure ExMarkInitialized(Lhs : TExpression);
forward;

function ExGetOrdinal(Expr : TExpression) : integer;
forward;

function ExGetAntiOrdinal(Ordinal : integer;
                          TypePtr : TPsTypePtr) : TExpression;
forward;

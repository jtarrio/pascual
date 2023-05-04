procedure ExDispose(var Expr : TSExpression);
forward;

function ExCopy(Expr : TSExpression) : TSExpression;
forward;

function ExDescribeOperator(Op : TSEOperator) : string;
forward;

function ExDescribe(Expr : TSExpression) : string;
forward;

procedure ExMarkInitialized(Lhs : TSExpression);
forward;

function ExGetOrdinal(Expr : TSExpression) : integer;
forward;

function ExGetAntiOrdinal(Ordinal : integer;
                          TypePtr : TSDType) : TSExpression;
forward;

function Pf_Parse(Fn : TExpression) : TExpression;
forward;
function Pf_DescribeName(Fn : TExpression) : string;
forward;
function Pf_DescribeCall(Expr : TExpression; Levels : integer) : string;
forward;

function PfOrd(Arg : TExpression) : TExpression;
forward;
function PfPred(Arg : TExpression) : TExpression;
forward;
function PfSucc(Arg : TExpression) : TExpression;
forward;

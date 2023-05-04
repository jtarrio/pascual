function _ModMath_Abs_Parse(FnExpr : TSExpression) : TSExpression;
begin
  Result := Pf_Overload_Parse(FnExpr, 'ABS')
end;

function _ModMath_Sqr_Parse(FnExpr : TSExpression) : TSExpression;
begin
  Result := Pf_Overload_Parse(FnExpr, 'SQR')
end;


procedure RegisterGlobals_Math;
begin
  { Mandatory constants }
  AddConstant(MakeConstant('PI', ExRealConstant(3.141592653589793238)));

  { Math subroutines }
  AddPsfn('ABS', @_ModMath_Abs_Parse);
  AddPsfn('SQR', @_ModMath_Sqr_Parse);
  AddFunction(MakeFunction1('ABS_i', PrimitiveTypes.PtInteger,
              MakeArg('NUM', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction1('ABS_r', PrimitiveTypes.PtReal,
              MakeArg('NUM', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('ARCTAN', PrimitiveTypes.PtReal,
              MakeArg('TAN', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('COS', PrimitiveTypes.PtReal,
              MakeArg('ANGLE', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('EXP', PrimitiveTypes.PtReal,
              MakeArg('POW', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('FRAC', PrimitiveTypes.PtReal,
              MakeArg('X', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('INT', PrimitiveTypes.PtReal,
              MakeArg('X', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('LN', PrimitiveTypes.PtReal,
              MakeArg('X', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('ODD', PrimitiveTypes.PtBoolean,
              MakeArg('X', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction1('ROUND', PrimitiveTypes.PtInteger,
              MakeArg('X', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('SIN', PrimitiveTypes.PtReal,
              MakeArg('ANGLE', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('SQR_i', PrimitiveTypes.PtInteger,
              MakeArg('NUM', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction1('SQR_r', PrimitiveTypes.PtReal,
              MakeArg('NUM', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('SQRT', PrimitiveTypes.PtReal,
              MakeArg('X', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('TRUNC', PrimitiveTypes.PtInteger,
              MakeArg('X', PrimitiveTypes.PtReal)));
end;

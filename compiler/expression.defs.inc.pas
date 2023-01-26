type
  TPsExpressionClass = (TecValue, TecFunction, TecStatement);
  TPsExpression = record
    Value : string;
    IsConstant : boolean;
    case Cls : TPsExpressionClass of
      TecValue : (TypeIndex : TPsTypeIndex);
      TecFunction : (FunctionIndex : TPsFunctionIndex)
  end;

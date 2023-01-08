type
  TPsExpressionClass = (TecValue, TecFunction, TecStatement);
  TPsExpression = record
    Value : string;
    Cls : TPsExpressionClass;
    IsConstant : boolean;
    TypeIndex : TPsTypeIndex;
    FunctionIndex : TPsFunctionIndex
  end;

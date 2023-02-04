const 
  MaxEnumValues = 128;
  MaxRecordFields = 32;
  MaxFunctionArguments = 4;

type 
  TLxTokenId = (TkUnknown, TkEof, TkComment, TkIdentifier, TkNumber, TkString,
                TkPlus, TkMinus, TkAsterisk, TkSlash, TkEquals, TkLessthan,
                TkMorethan, TkLbracket, TkRbracket, TkDot, TkComma, TkColon,
                TkSemicolon, TkCaret, TkLparen, TkRparen, TkNotEquals,
                TkLessOrEquals, TkMoreOrEquals, TkAssign, TkRange, TkAnd,
                TkArray, TkBegin, TkCase, TkConst, TkDiv, TkDo, TkDownto,
                TkElse, TkEnd, TkFile, TkFor, TkForward, TkFunction, TkGoto,
                TkIf, TkIn, TkLabel, TkMod, TkNil, TkNot, TkOf, TkOr, TkPacked,
                TkProcedure, TkProgram, TkRecord, TkRepeat, TkSet, TkThen, TkTo,
                TkType, TkUntil, TkVar, TkWhile, TkWith);
  TLxPos = record
    Row, Col : integer
  end;
  TLxToken = record
    Id : TLxTokenId;
    Value : string;
    Pos : TLxPos
  end;

  TPsPseudoFun = (TpfDispose, TpfNew, TpfOrd, TpfPred, TpfRead, TpfReadln,
                  TpfStr, TpfSucc, TpfWrite, TpfWriteln);

  TPsTypeIndex = ^TPsType;
  TPsVariableIndex = ^TPsVariable;
  TPsFunctionIndex = ^TPsFunction;

  TExpression = ^TExpressionObj;
  TExImmediateClass = (XicNil, XicBoolean, XicInteger, XicChar, XicString,
                       XicEnum);
  TExImmediate = record
    case Cls : TExImmediateClass of 
      XicBoolean : (BooleanValue : boolean);
      XicInteger : (IntegerValue : integer);
      XicChar : (CharValue : char);
      XicString : (StringValue : string);
      XicEnum : (EnumOrdinal : integer)
  end;
  TExToString = record
    Parent : TExpression
  end;
  TExSubrange = record
    Parent : TExpression
  end;
  TExVariableAccess = record
    VariableIndex : TPsVariableIndex
  end;
  TExFieldAccess = record
    Parent : TExpression;
    FieldNumber : integer
  end;
  TExArrayAccess = record
    Parent : TExpression;
    Subscript : TExpression
  end;
  TExPointerAccess = record
    Parent : TExpression
  end;
  TExStringChar = record
    Parent : TExpression;
    Subscript : TExpression
  end;
  TExFunctionRef = record
    FunctionIndex : TPsFunctionIndex
  end;
  TExFunctionArgs = record
    Size : integer;
    Values : array[1..MaxFunctionArguments] of TExpression
  end;
  TExFunctionCall = record
    FunctionRef : TExpression;
    Args : TExFunctionArgs
  end;
  TExPseudoFunRef = record
    PseudoFun : TPsPseudoFun
  end;
  TExReadArgs = record
    Arg : TExpression;
    Next : ^TExReadArgs
  end;
  TExWriteArgs = record
    Arg : TExpression;
    Next : ^TExWriteArgs
  end;
  TExPseudoFunCall = record
    Arg1 : TExpression;
    Arg2 : TExpression;
    case PseudoFun : TPsPseudoFun of 
      TpfRead, TpfReadln: (ReadArgs : ^TExReadArgs);
      TpfWrite, TpfWriteln: (WriteArgs : ^TExWriteArgs);
  end;
  TExUnaryOp = record
    Parent : TExpression;
    Op : TLxTokenId
  end;
  TExBinaryOp = record
    Left : TExpression;
    Right : TExpression;
    Op : TLxTokenId
  end;

  TExpressionClass = (XcImmediate, XcToString, XcSubrange,
                      XcVariableAccess, XcFieldAccess,
                      XcArrayAccess, XcPointerAccess, XcStringChar,
                      XcFunctionRef, XcFunctionCall, XcPseudoFunRef,
                      XcPseudoFunCall, XcUnaryOp, XcBinaryOp);
  TExpressionObj = record
    TypeIndex : TPsTypeIndex;
    IsConstant : boolean;
    IsAssignable : boolean;
    IsFunctionResult : boolean;
    case Cls : TExpressionClass of 
      XcImmediate : (ImmediateEx : TExImmediate);
      XcToString : (ToStringEx : TExToString);
      XcSubrange : (SubrangeEx : TExSubrange);
      XcVariableAccess : (VariableEx : TExVariableAccess);
      XcFieldAccess : (FieldEx : TExFieldAccess);
      XcArrayAccess : (ArrayEx : TExArrayAccess);
      XcPointerAccess : (PointerEx : TExPointerAccess);
      XcStringChar : (StringCharEx : TExStringChar);
      XcFunctionRef : (FunctionEx : TExFunctionRef);
      XcFunctionCall : (CallEx : TExFunctionCall);
      XcPseudoFunRef : (PseudoFunEx : TExPseudoFunRef);
      XcPseudoFunCall : (PseudoFunCallEx : TExPseudoFunCall);
      XcUnaryOp : (UnaryEx : TExUnaryOp);
      XcBinaryOp : (BinaryEx : TExBinaryOp);
  end;

  TPsIdentifier = record
    Name : string;
  end;

  TPsEnumIndex = ^TPsEnumDef;
  TPsRangeIndex = ^TPsRangeDef;
  TPsRecordIndex = ^TPsRecordDef;
  TPsArrayIndex = ^TPsArrayDef;
  TPsConstantIndex = ^TPsConstant;
  TPsWithVarIndex = ^TPsWithVar;
  TPsNameIndex = ^TPsName;

  TPsTypeClass = (TtcBoolean, TtcInteger, TtcChar, TtcString, TtcText,
                  TtcEnum, TtcRange, TtcRecord, TtcArray, TtcPointer, TtcNil,
                  TtcPointerUnknown);
  TPsType = record
    Name : string;
    AliasFor : TPsTypeIndex;
    case Cls : TPsTypeClass of 
      TtcEnum : (EnumIndex : TPsEnumIndex);
      TtcRange : (RangeIndex : TPsRangeIndex);
      TtcRecord : (RecordIndex : TPsRecordIndex);
      TtcArray : (ArrayIndex : TPsArrayIndex);
      TtcPointer : (PointedTypeIndex : TPsTypeIndex);
      TtcPointerUnknown : (TargetName : ^string);
  end;
  TPsEnumDef = record
    Size : integer;
    Values : array[0..MaxEnumValues - 1] of string;
    Id : integer;
    HasBeenDefined : boolean
  end;
  TPsRangeDef = record
    First, Last : TExpression;
    BaseTypeIndex : TPsTypeIndex
  end;
  TPsRecordField = record
    Name : string;
    TypeIndex : TPsTypeIndex
  end;
  TPsRecordDef = record
    Size : integer;
    Fields : array[1..MaxRecordFields] of TPsRecordField;
    NumVariants : integer;
    VariantBounds : array[1..MaxRecordFields] of integer;
    Id : integer;
    HasBeenDefined : boolean
  end;
  TPsArrayDef = record
    LowBound, HighBound : TExpression;
    TypeIndex : TPsTypeIndex
  end;
  TPsConstant = record
    Name : string;
    Value : TExpression;
  end;
  TPsVariable = record
    Name : string;
    TypeIndex : TPsTypeIndex;
    IsReference : boolean;
    IsConstant : boolean
  end;
  TPsFunction = record
    Name : string;
    ArgCount : integer;
    Args : array[1..MaxFunctionArguments] of TPsVariable;
    ReturnTypeIndex : TPsTypeIndex;
    IsDeclaration : boolean;
  end;
  TPsWithVar = record
    VariableIndex : TPsVariableIndex
  end;
  TPsNameClass = (TncType, TncVariable, TncConstant, TncEnumValue, TncFunction,
                  TncPseudoFun);
  TPsName = record
    Name : string;
    case Cls : TPsNameClass of 
      TncType : (TypeIndex : TPsTypeIndex);
      TncVariable : (VariableIndex : TPsVariableIndex);
      TncConstant : (ConstantIndex : TPsConstantIndex);
      TncEnumValue : (EnumTypeIndex : TPsTypeIndex;
                      Ordinal : integer);
      TncFunction : (FunctionIndex : TPsFunctionIndex);
      TncPseudoFun : (PseudoFun : TPsPseudoFun)
  end;

  TPsDefPtr = ^TPsDefEntry;
  TPsDefClass = (TdcName, TdcType, TdcEnum, TdcRange, TdcRecord, TdcArray,
                 TdcConstant, TdcVariable, TdcFunction, TdcWithVar,
                 TdcScopeBoundary);
  TPsDefEntry = record
    Prev : TPsDefPtr;
    Next : TPsDefPtr;
    case Cls : TPsDefClass of 
      TdcName : (NameIndex : TPsNameIndex);
      TdcType : (TypeIndex : TPsTypeIndex);
      TdcEnum : (EnumIndex : TPsEnumIndex);
      TdcRange : (RangeIndex : TPsRangeIndex);
      TdcRecord : (RecordIndex : TPsRecordIndex);
      TdcArray : (ArrayIndex : TPsArrayIndex);
      TdcConstant : (ConstantIndex : TPsConstantIndex);
      TdcVariable : (VariableIndex : TPsVariableIndex);
      TdcFunction : (FunctionIndex : TPsFunctionIndex);
      TdcWithVar : (WithVarIndex : TPsWithVarIndex);
      TdcScopeBoundary : (TemporaryScope : boolean;
                          CurrentFunction : TPsFunctionIndex)
  end;

  TPsDefs = record
    Latest : TPsDefPtr;
    CurrentFunction : TPsFunctionIndex;
    Counter : integer;
  end;

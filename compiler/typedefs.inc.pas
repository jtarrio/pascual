const 
  MaxEnumVals = 128;
  MaxRecordFields = 32;
  MaxFnArgs = 4;

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

  TPsPseudoFn = (TpfDispose, TpfNew, TpfOrd, TpfPred, TpfRead, TpfReadln,
                 TpfStr, TpfSucc, TpfWrite, TpfWriteln);

  TPsTypePtr = ^TPsType;
  TPsEnumPtr = ^TPsEnumDef;
  TPsVarPtr = ^TPsVariable;
  TPsFnPtr = ^TPsFunction;

  TExpression = ^TExpressionObj;
  TExImmediateClass = (XicNil, XicBoolean, XicInteger, XicChar, XicString,
                       XicEnum);
  TExImmediate = record
    case Cls : TExImmediateClass of 
      XicBoolean : (BooleanVal : boolean);
      XicInteger : (IntegerVal : integer);
      XicChar : (CharVal : char);
      XicString : (StringVal : string);
      XicEnum : (EnumOrdinal : integer;
                 EnumPtr : TPsEnumPtr)
  end;
  TExFunctionArgs = record
    Size : integer;
    Values : array[1..MaxFnArgs] of TExpression;
  end;
  TExPseudoFnRef = record
    PseudoFn : TPsPseudoFn
  end;
  TExReadArgs = record
    Arg : TExpression;
    Next : ^TExReadArgs
  end;
  TExWriteArgs = record
    Arg : TExpression;
    Next : ^TExWriteArgs
  end;
  TExPseudoFnCall = record
    Arg1 : TExpression;
    Arg2 : TExpression;
    case PseudoFn : TPsPseudoFn of 
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
                      XcVariable, XcField,
                      XcArray, XcPointer, XcStringChar,
                      XcFnRef, XcFnCall, XcPseudoFnRef,
                      XcPseudoFnCall, XcUnaryOp, XcBinaryOp);
  TExpressionObj = record
    TypePtr : TPsTypePtr;
    IsConstant : boolean;
    IsAssignable : boolean;
    IsFunctionResult : boolean;
    case Cls : TExpressionClass of 
      XcImmediate : (Immediate : TExImmediate);
      XcToString : (ToStrParent : TExpression);
      XcSubrange : (SubrangeParent : TExpression);
      XcVariable : (VarPtr : TPsVarPtr);
      XcField : (RecExpr : TExpression;
                 RecFieldNum : integer);
      XcArray : (ArrayExpr : TExpression;
                 ArrayIndex : TExpression);
      XcPointer : (PointerExpr : TExpression);
      XcStringChar : (StringExpr : TExpression;
                      StringIndex : TExpression);
      XcFnRef : (FnPtr : TPsFnPtr);
      XcFnCall : (FnExpr : TExpression;
                  CallArgs : TExFunctionArgs);
      XcPseudoFnRef : (PseudoFn : TPsPseudoFn);
      XcPseudoFnCall : (PseudoFnCall : TExPseudoFnCall);
      XcUnaryOp : (Unary : TExUnaryOp);
      XcBinaryOp : (Binary : TExBinaryOp);
  end;

  TPsIdentifier = record
    Name : string;
  end;

  TPsRangePtr = ^TPsRangeDef;
  TPsRecPtr = ^TPsRecordDef;
  TPsArrayPtr = ^TPsArrayDef;
  TPsConstPtr = ^TPsConstant;
  TPsWithVarPtr = ^TPsWithVar;
  TPsNamePtr = ^TPsName;

  TPsTypeClass = (TtcBoolean, TtcInteger, TtcChar, TtcString, TtcText,
                  TtcEnum, TtcRange, TtcRecord, TtcArray, TtcPointer, TtcNil,
                  TtcPointerUnknown);
  TPsType = record
    Name : string;
    AliasFor : TPsTypePtr;
    case Cls : TPsTypeClass of 
      TtcEnum : (EnumPtr : TPsEnumPtr);
      TtcRange : (RangePtr : TPsRangePtr);
      TtcRecord : (RecPtr : TPsRecPtr);
      TtcArray : (ArrayPtr : TPsArrayPtr);
      TtcPointer : (PointedTypePtr : TPsTypePtr);
      TtcPointerUnknown : (TargetName : ^string);
  end;
  TPsEnumDef = record
    Size : integer;
    Values : array[0..MaxEnumVals - 1] of string;
    Id : integer;
    HasBeenDefined : boolean
  end;
  TPsRangeDef = record
    First, Last : TExpression;
    BaseTypePtr : TPsTypePtr
  end;
  TPsRecordField = record
    Name : string;
    TypePtr : TPsTypePtr
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
    TypePtr : TPsTypePtr
  end;
  TPsConstant = record
    Name : string;
    Value : TExpression;
  end;
  TPsVariable = record
    Name : string;
    TypePtr : TPsTypePtr;
    IsReference : boolean;
    IsConstant : boolean
  end;
  TPsFunction = record
    Name : string;
    ArgCount : integer;
    Args : array[1..MaxFnArgs] of TPsVariable;
    ReturnTypePtr : TPsTypePtr;
    IsDeclaration : boolean;
  end;
  TPsWithVar = record
    VarPtr : TPsVarPtr
  end;
  TPsNameClass = (TncType, TncVariable, TncConstant, TncEnumVal, TncFunction,
                  TncPseudoFn);
  TPsName = record
    Name : string;
    case Cls : TPsNameClass of 
      TncType : (TypePtr : TPsTypePtr);
      TncVariable : (VarPtr : TPsVarPtr);
      TncConstant : (ConstPtr : TPsConstPtr);
      TncEnumVal : (EnumTypePtr : TPsTypePtr;
                    Ordinal : integer);
      TncFunction : (FnPtr : TPsFnPtr);
      TncPseudoFn : (PseudoFn : TPsPseudoFn)
  end;

  TPsDefPtr = ^TPsDefEntry;
  TPsDefClass = (TdcName, TdcType, TdcEnum, TdcRange, TdcRecord, TdcArray,
                 TdcConstant, TdcVariable, TdcFunction, TdcWithVar,
                 TdcScopeBoundary);
  TPsDefEntry = record
    Prev : TPsDefPtr;
    Next : TPsDefPtr;
    case Cls : TPsDefClass of 
      TdcName : (NamePtr : TPsNamePtr);
      TdcType : (TypePtr : TPsTypePtr);
      TdcEnum : (EnumPtr : TPsEnumPtr);
      TdcRange : (RangePtr : TPsRangePtr);
      TdcRecord : (RecPtr : TPsRecPtr);
      TdcArray : (ArrayPtr : TPsArrayPtr);
      TdcConstant : (ConstPtr : TPsConstPtr);
      TdcVariable : (VarPtr : TPsVarPtr);
      TdcFunction : (FnPtr : TPsFnPtr);
      TdcWithVar : (WithVarPtr : TPsWithVarPtr);
      TdcScopeBoundary : (TemporaryScope : boolean;
                          CurrentFn : TPsFnPtr)
  end;

  TPsDefs = record
    Latest : TPsDefPtr;
    CurrentFn : TPsFnPtr;
    Counter : integer;
  end;

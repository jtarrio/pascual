const 
  MaxEnumVals = 128;
  MaxRecordFields = 64;
  MaxFnArgs = 16;

type 
  TPsTypePtr = ^TPsType;
  TPsEnumPtr = ^TPsEnumDef;
  TPsVarPtr = ^TPsVariable;
  TPsFnPtr = ^TPsFunction;
  TPsPseudoFnPtr = ^TPsPseudoFn;
  TExSetImmBounds = ^TExSetImmBoundsObj;
  TExSetExprBounds = ^TExSetExprboundsObj;

  TExpression = ^TExpressionObj;
  TExImmediateClass = (XicNil, XicBoolean, XicInteger, XicReal, XicChar,
                       XicString, XicEnum, XicSet);
  TExImmediate = record
    case Cls : TExImmediateClass of 
      XicBoolean : (BooleanVal : boolean);
      XicInteger : (IntegerVal : integer);
      XicReal : (RealVal : real);
      XicChar : (CharVal : char);
      XicString : (StringVal : string);
      XicEnum : (EnumOrdinal : integer;
                 EnumPtr : TPsEnumPtr);
      XicSet : (SetBounds : TExSetImmBounds;
                SetOfTypePtr : TPsTypePtr)
  end;
  TExSetImmBoundsObj = record
    First, Last : integer;
    Next : TExSetImmBounds;
  end;
  TExSetExprboundsObj = record
    First, Last : TExpression;
    Next : TExSetExprBounds;
  end;
  TExFunctionArgs = record
    Size : integer;
    Values : array[1..MaxFnArgs] of TExpression;
  end;
  TExWriteArg = record
    Arg : TExpression;
    Width : TExpression;
    Prec : TExpression
  end;
  TExReadArgs = record
    Arg : TExpression;
    Next : ^TExReadArgs
  end;
  TExWriteArgs = record
    Arg : TExpression;
    Width : TExpression;
    Prec : TExpression;
    Next : ^TExWriteArgs
  end;
  TExPseudoFnCall = record
    PseudoFnPtr : TPsPseudoFnPtr;
    Arg1 : TExpression;
    ReadArgs : ^TExReadArgs;
    WriteArgs : ^TExWriteArgs
  end;
  TExOperator = (XoAdd, XoSub, XoMul, XoDivReal, XoDivInt, XoMod, XoNeg,
                 XoAnd, XoOr, XoXor, XoNot, XoShl, XoShr,
                 XoIn,
                 XoEq, XoNe, XoLt, XoGt, XoLtEq, XoGtEq,
                 XoOrd, XoPred, XoSucc);
  TExUnaryOp = record
    Parent : TExpression;
    Op : TExOperator
  end;
  TExBinaryOp = record
    Left : TExpression;
    Right : TExpression;
    Op : TExOperator
  end;

  TExpressionClass = (XcImmediate, XcToString, XcToReal, XcToRawPtr,
                      XcWithTmpVar, XcSubrange, XcSet, XcVariable, XcField,
                      XcArray, XcPointer, XcAddress, XcStringChar,
                      XcFnRef, XcFnCall, XcPseudoFnRef, XcPseudoFnCall,
                      XcSizeof, XcConvertToStr, XcConvertToVal,
                      XcUnaryOp, XcBinaryOp);
  TExpressionObj = record
    TypePtr : TPsTypePtr;
    IsAssignable : boolean;
    IsAddressable : boolean;
    IsFunctionResult : boolean;
    case Cls : TExpressionClass of 
      XcImmediate : (Immediate : TExImmediate);
      XcToString : (ToStrParent : TExpression);
      XcToReal : (ToRealParent : TExpression);
      XcToRawPtr : (ToRawPtrParent : TExpression);
      XcWithTmpVar : (TmpVar : TExpression;
                      TmpVarValue : TExpression;
                      TmpVarChild : TExpression);
      XcSubrange : (SubrangeParent : TExpression);
      XcSet : (SetBase : TExpression;
               SetBounds : TExSetExprBounds);
      XcVariable : (VarPtr : TPsVarPtr);
      XcField : (RecExpr : TExpression;
                 RecFieldNum : integer);
      XcArray : (ArrayExpr : TExpression;
                 ArrayIndex : TExpression);
      XcPointer : (PointerExpr : TExpression);
      XcAddress : (AddressExpr : TExpression);
      XcStringChar : (StringExpr : TExpression;
                      StringIndex : TExpression);
      XcFnRef : (FnPtr : TPsFnPtr);
      XcFnCall : (FnExpr : TExpression;
                  CallArgs : TExFunctionArgs);
      XcPseudoFnRef : (PseudoFnPtr : TPsPseudoFnPtr);
      XcPseudoFnCall : (PseudoFnCall : TExPseudoFnCall);
      XcSizeof : (SizeofTypePtr : TPsTypePtr);
      XcConvertToStr : (ToStrSrc : TExWriteArg;
                        ToStrDest : TExpression);
      XcConvertToVal : (ToValSrc : TExpression;
                        ToValDest : TExpression;
                        ToValCode : TExpression);
      XcUnaryOp : (Unary : TExUnaryOp);
      XcBinaryOp : (Binary : TExBinaryOp);
  end;

  TPsIdentifier = record
    Name : string;
  end;

  TPsRecPtr = ^TPsRecordDef;
  TPsFnDefPtr = ^TPsFnDef;
  TPsConstPtr = ^TPsConstant;
  TPsWithVarPtr = ^TPsWithVar;
  TPsNamePtr = ^TPsName;

  TPsTypeClass = (TtcBoolean, TtcInteger, TtcReal, TtcChar, TtcString, TtcText,
                  TtcEnum, TtcRange, TtcSet, TtcRecord, TtcArray,
                  TtcPointer, TtcNil, TtcPointerUnknown, TtcRawPtr,
                  TtcFunction);
  TPsType = record
    Name : string;
    AliasFor : TPsTypePtr;
    WasUsed : boolean;
    case Cls : TPsTypeClass of 
      TtcEnum : (EnumPtr : TPsEnumPtr);
      TtcRange : (RangeDef : record
                  First, Last : integer;
                  BaseTypePtr : TPsTypePtr
                  end);
      TtcSet : (ElementTypePtr : TPsTypePtr);
      TtcRecord : (RecPtr : TPsRecPtr);
      TtcArray : (ArrayDef : record
                  IndexTypePtr : TPsTypePtr;
                  ValueTypePtr : TPsTypePtr;
                  end);
      TtcPointer : (PointedTypePtr : TPsTypePtr);
      TtcPointerUnknown : (TargetName : ^string);
      TtcFunction : (FnDefPtr : TPsFnDefPtr);
  end;
  TPsEnumDef = record
    Size : integer;
    Values : array[0..MaxEnumVals - 1] of string;
    Id : integer;
    HasBeenDefined : boolean;
    ValuesHaveBeenOutput : boolean;
    RefCount : integer
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
    IsPacked : boolean;
    Id : integer;
    HasBeenDefined : boolean;
    RefCount : integer
  end;
  TPsConstant = record
    Name : string;
    Value : TExpression;
  end;
  TPsVariable = record
    Name : string;
    TypePtr : TPsTypePtr;
    IsReference : boolean;
    IsConstant : boolean;
    WasInitialized : boolean;
    WasUsed : boolean
  end;
  TPsFnArgs = record
    Count : integer;
    Defs : array[1..MaxFnArgs] of TPsVariable;
  end;
  TPsFnDef = record
    Args : TPsFnArgs;
    ReturnTypePtr : TPsTypePtr;
    RefCount : integer;
  end;
  TPsFunction = record
    Name : string;
    ExternalName : string;
    Args : TPsFnArgs;
    ReturnTypePtr : TPsTypePtr;
    IsDeclaration : boolean;
    WasUsed : boolean
  end;
  TPsPseudoFnParser = function (FnExpr : TExpression) : TExpression;
  TPsPseudoFnDescriptor = function (FnExpr : TExpression) : string;
  TPsPseudoFn = record
    Name : string;
    ParseFn : TPsPseudoFnParser;
    DescribeFn : TPsPseudoFnDescriptor;
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
      TncPseudoFn : (PseudoFnPtr : TPsPseudoFnPtr)
  end;

  TPsCounterType = (TctEnum, TctRecord, TctTmpVar);
  TPsCounters = record
    EnumCtr : integer;
    RecordCtr : integer;
    TmpVarCtr : integer;
  end;

  TPsDefPtr = ^TPsDefEntry;
  TPsDefClass = (TdcName, TdcType, TdcConstant, TdcVariable,
                 TdcFunction, TdcPseudoFn, TdcWithVar, TdcScopeBoundary);
  TPsDefEntry = record
    Prev : TPsDefPtr;
    Next : TPsDefPtr;
    case Cls : TPsDefClass of 
      TdcName : (NamePtr : TPsNamePtr);
      TdcType : (TypePtr : TPsTypePtr);
      TdcConstant : (ConstPtr : TPsConstPtr);
      TdcVariable : (VarPtr : TPsVarPtr);
      TdcFunction : (FnPtr : TPsFnPtr);
      TdcPseudoFn : (PseudoFnPtr : TPsPseudoFnPtr);
      TdcWithVar : (WithVarPtr : TPsWithVarPtr);
      TdcScopeBoundary : (TemporaryScope : boolean;
                          Counters : TPsCounters;
                          CurrentFn : TPsFnPtr)
  end;

  TPsDefs = record
    Latest : TPsDefPtr;
    CurrentFn : TPsFnPtr;
    Counters : TPsCounters;
  end;

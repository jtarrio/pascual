const 
  MaxEnumVals = 128;
  MaxRecordFields = 32;
  MaxFnArgs = 8;

type 
  TLxTokenId = (TkUnknown, TkEof, TkComment, TkIdentifier, TkInteger, TkReal,
                TkString, TkPlus, TkMinus, TkAsterisk, TkSlash, TkEquals,
                TkLessthan, TkMorethan, TkLbracket, TkRbracket, TkDot, TkComma,
                TkColon, TkSemicolon, TkCaret, TkLparen, TkRparen, TkNotEquals,
                TkLessOrEquals, TkMoreOrEquals, TkAssign, TkRange, TkAt, TkAnd,
                TkArray, TkBegin, TkCase, TkConst, TkDiv, TkDo, TkDownto,
                TkElse, TkEnd, TkFile, TkFor, TkForward, TkFunction, TkGoto,
                TkIf, TkIn, TkLabel, TkMod, TkNil, TkNot, TkOf, TkOr, TkPacked,
                TkProcedure, TkProgram, TkRecord, TkRepeat, TkSet, TkShl, TkShr,
                TkThen, TkTo, TkType, TkUntil, TkVar, TkWhile, TkWith, TkXor);
  TLxPos = record
    Row, Col : integer
  end;
  TLxToken = record
    Id : TLxTokenId;
    Value : string;
    Pos : TLxPos
  end;

  TPsPseudoFn = (TpfAbs, TpfConcat, TpfDispose, TpfNew, TpfOrd, TpfPred,
                 TpfRandom, TpfRead, TpfReadln, TpfSizeof, TpfSqr, TpfStr,
                 TpfSucc, TpfVal, TpfWrite, TpfWriteln);

  TPsTypePtr = ^TPsType;
  TPsEnumPtr = ^TPsEnumDef;
  TPsVarPtr = ^TPsVariable;
  TPsFnPtr = ^TPsFunction;
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
    Arg1 : TExpression;
    Arg2 : TExpression;
    Arg3 : TExpression;
    Arg4 : TExpression;
    TypeArg : TPsTypePtr;
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

  TExpressionClass = (XcImmediate, XcToString, XcToReal, XcWithTmpVar,
                      XcSubrange, XcSet, XcVariable, XcField, XcArray,
                      XcPointer, XcAddress, XcStringChar, XcFnRef, XcFnCall,
                      XcPseudoFnRef, XcPseudoFnCall, XcUnaryOp, XcBinaryOp);
  TExpressionObj = record
    TypePtr : TPsTypePtr;
    IsAssignable : boolean;
    IsAddressable : boolean;
    IsFunctionResult : boolean;
    case Cls : TExpressionClass of 
      XcImmediate : (Immediate : TExImmediate);
      XcToString : (ToStrParent : TExpression);
      XcToReal : (ToRealParent : TExpression);
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
      XcPseudoFnRef : (PseudoFn : TPsPseudoFn);
      XcPseudoFnCall : (PseudoFnCall : TExPseudoFnCall);
      XcUnaryOp : (Unary : TExUnaryOp);
      XcBinaryOp : (Binary : TExBinaryOp);
  end;

  TPsIdentifier = record
    Name : string;
  end;

  TPsRecPtr = ^TPsRecordDef;
  TPsConstPtr = ^TPsConstant;
  TPsWithVarPtr = ^TPsWithVar;
  TPsNamePtr = ^TPsName;

  TPsTypeClass = (TtcBoolean, TtcInteger, TtcReal, TtcChar, TtcString, TtcText,
                  TtcEnum, TtcRange, TtcSet, TtcRecord, TtcArray,
                  TtcPointer, TtcNil, TtcPointerUnknown);
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
  TPsFunction = record
    Name : string;
    ExternalName : string;
    ArgCount : integer;
    Args : array[1..MaxFnArgs] of TPsVariable;
    ReturnTypePtr : TPsTypePtr;
    IsDeclaration : boolean;
    WasUsed : boolean
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

  TPsCounterType = (TctEnum, TctRecord, TctTmpVar);
  TPsCounters = record
    EnumCtr : integer;
    RecordCtr : integer;
    TmpVarCtr : integer;
  end;

  TPsDefPtr = ^TPsDefEntry;
  TPsDefClass = (TdcName, TdcType, TdcConstant, TdcVariable,
                 TdcFunction, TdcWithVar, TdcScopeBoundary);
  TPsDefEntry = record
    Prev : TPsDefPtr;
    Next : TPsDefPtr;
    case Cls : TPsDefClass of 
      TdcName : (NamePtr : TPsNamePtr);
      TdcType : (TypePtr : TPsTypePtr);
      TdcConstant : (ConstPtr : TPsConstPtr);
      TdcVariable : (VarPtr : TPsVarPtr);
      TdcFunction : (FnPtr : TPsFnPtr);
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

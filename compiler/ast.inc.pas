const 
  { Maximum number of values per enumerated type. }
  MaxEnumVals = 128;
  { Maximum number of fields per record type. }
  MaxRecordFields = 64;
  { Maximum number of arguments per subroutine. }
  MaxSubroutineArgs = 16;

type 
  { An expression that yields a value. }
  TSExpression = ^TSExpressionObj;
  { A type. }
  TSDType = ^TSDTypeDef;
  { A defined type, constant, variable, etc. }
  TSDefinition = ^TSDefEntry;
  { A subroutine. }
  TSDSubroutine = ^TSDSubroutineDef;
  { A Pascal statement. }
  TSStatement = ^TSStatementObj;
  { A sequence of statements. }
  TSSSequence = ^TSSSequenceEntry;

  { Counters for records, enums, and temporary variables. }
  TSCounterType = (SctEnum, SctRecord, SctTmpVar);
  TSCounters = record
    { Enum counter. }
    EnumCtr : integer;
    { Record counter. }
    RecordCtr : integer;
    { Temporary variable counter. }
    TmpVarCtr : integer;
  end;

  { The set of defined objects for a function or global. }
  TSScope = ^TSScopeObj;
  TSScopeObj = record
    { Parent scope. }
    Parent : TSScope;
    { Stack for the definitions. }
    LatestDef : TSDefinition;
    { Function this scope belongs to. }
    CurrentFn : TSDSubroutine;
    { Current state of the counters. }
    Counters : TSCounters;
  end;

  { An untyped constant. }
  TSDConstant = ^TSDConstantDef;
  TSDConstantDef = record
    { Name of the constant. }
    Name : string;
    { Value of the constant. }
    Value : TSExpression;
  end;

  { A file type. }
  { File class: unqualified, text, binary. }
  TSDTFileClass = (SdtfcNone, SdtfcText, SdtfcBinary);
  TSDTFile = record
    { File class. }
    Cls : TSDTFileClass;
    { Type stored in the binary type. }
    TypePtr : TSDType
  end;
  { An enum type. }
  TSDTEnum = ^TSDTEnumDef;
  TSDTEnumDef = record
    { Size of the enum. }
    Size : integer;
    { Values of the enum. }
    Values : array[0..MaxEnumVals - 1] of string;
    { A unique ID for the enum. }
    Id : integer;
    { Has this enum been defined in the output? }
    HasBeenDefined : boolean;
    { Do this enum's values need to be output as an array? }
    NeedValueArray : boolean
  end;
  { A range type. }
  TSDTRange = record
    { First and last ordinals. }
    First, Last : integer;
    { Base type for this subrange. }
    BaseTypePtr : TSDType
  end;
  { A record type. }
  TSDTRecord = ^TSDTRecordDef;
  { A record field. }
  TSDTRecordField = record
    { Field's name. }
    Name : string;
    { Field's type.}
    TypePtr : TSDType
  end;
  TSDTRecordDef = record
    { Number of fields in the record. }
    Size : integer;
    { List of fields in the record. }
    Fields : array[1..MaxRecordFields] of TSDTRecordField;
    { How many variants this record has. }
    NumVariants : integer;
    { Index in Fields of the start of each variant. }
    VariantBounds : array[1..MaxRecordFields] of integer;
    { Is this record packed? }
    IsPacked : boolean;
    { Unique ID for this record type. }
    Id : integer;
    { Has this record been defined in the output? }
    HasBeenDefined : boolean
  end;
  { A subroutine type (for a variable holding a subroutine pointer.) }
  TSDTSubroutine = ^TSDTSubroutineDef;
  { A single subroutine argument. }
  TSDSubroutineArg = record
    { The argument's name. }
    Name : string;
    { The argument's type. }
    TypePtr : TSDType;
    { Does this argument contain a reference? }
    IsReference : boolean;
    { Is this argument actually a constant? }
    IsConstant : boolean;
  end;
  { Subroutine arguments. }
  TSDSubroutineArgs = record
    { Number of arguments. }
    Count : integer;
    { Argument definitions. }
    Defs : array[1..MaxSubroutineArgs] of TSDSubroutineArg;
  end;
  TSDTSubroutineDef = record
    { Subroutine arguments. }
    Args : TSDSubroutineArgs;
    { Subroutine's return type. Nil for a procedure. }
    ReturnTypePtr : TSDType
  end;
  { An array type. }
  TSDTArray = record
    { Index type. }
    IndexTypePtr : TSDType;
    { Value type. }
    ValueTypePtr : TSDType;
  end;
  { Class of the type: boolean, integer, etc. }
  TSDTypeClass = (SdtcBoolean, SdtcInteger, SdtcReal, SdtcChar, SdtcString,
                  SdtcFile, SdtcEnum, SdtcRange, SdtcSet, SdtcRecord, SdtcArray,
                  SdtcPointer, SdtcNil, SdtcPointerForward, SdtcFunction);
  TSDTypeDef = record
    { The type's name. }
    Name : string;
    { If this type is an alias for another type, a pointer to that type. }
    AliasFor : TSDType;
    { Has this type been used? }
    WasUsed : boolean;
    case Cls : TSDTypeClass of 
      { File type. }
      SdtcFile : (FileDef : TSDTFile);
      { Enumerated type. }
      SdtcEnum : (EnumPtr : TSDTEnum);
      { Subrange type. }
      SdtcRange : (RangeDef : TSDTRange);
      { Set type. }
      SdtcSet : (ElementTypePtr : TSDType);
      { Record type. }
      SdtcRecord : (RecPtr : TSDTRecord);
      { Array type. }
      SdtcArray : (ArrayDef : TSDTArray);
      { Pointer type. }
      SdtcPointer : (PointedTypePtr : TSDType);
      { Forward-declared pointer type. }
      SdtcPointerForward : (TargetName : ^string);
      { Function type. }
      SdtcFunction : (FnDefPtr : TSDTSubroutine);
  end;

  { A variable or typed constant. }
  TSDVariable = ^TSDVariableDef;
  TSDVariableDef = record
    { The variable's name. }
    Name : string;
    { The variable's type. }
    TypePtr : TSDType;
    { Does this variable contain a reference? }
    IsReference : boolean;
    { If this is an absolute variable, its location. }
    Location : TSExpression;
    { Is this variable actually a constant? }
    IsConstant : boolean;
    { If this is a constant, its value. }
    ConstantValue : TSExpression;
    { Is this variable actually a subroutine argument? }
    IsArgument : boolean;
    { Has this variable been initialized? }
    WasInitialized : boolean;
    { Has this variable been used? }
    WasUsed : boolean;
    { If this variable is an alias for an expression, the expression. }
    IsAliasFor : TSExpression
  end;

  { A subroutine. }
  TSDSubroutineDef = record
    { The subroutine's Pascal name. }
    Name : string;
    { The subroutine's external name. }
    ExternalName : string;
    { The arguments. }
    Args : TSDSubroutineArgs;
    { The return type. Nil if this subroutine is a procedure. }
    ReturnTypePtr : TSDType;
    { The definitions in this function's scope. }
    Scope : TSScopeObj;
    { The statements in this function. }
    Body : TSSSequence;
    { Is this a forward declaration? }
    IsDeclaration : boolean;
    { Did this function have a forward declaration? }
    HadDeclaration : boolean;
    { Has this subroutine been used? }
    WasUsed : boolean
  end;

  { A pseudo-function. It's a custom parser that produces an expression. }
  TSDPsfn = ^TSDPsfnDef;
  { A function that parses the input to produce an expression. }
  TSDPsfnParser = function (FnExpr : TSExpression) : TSExpression;
  TSDPsfnDef = record
    { The pseudo-function's name. }
    Name : string;
    { The parser for this pseudo-function. }
    ParseFn : TSDPsfnParser
  end;

  { Temporary variable. }
  TSDTmpVar = ^TSDTmpVarDef;
  TSDTmpVarDef = record
    { Temporary variable definition. }
    VarDef : TSDVariableDef
  end;

  { "With" variable. }
  TSDWithVar = ^TSDWithVarDef;
  TSDWithVarDef = record
    { Pointer to the temporary variable. }
    TmpVarPtr : TSDTmpVar;
    { Is this "with" variable active? }
    IsActive : boolean
  end;

  { Definition class. }
  TSDefClass = (SdcType, SdcConstant, SdcVariable,
                SdcSubroutine, SdcPsfn, SdcTmpVar, SdcWithVar);
  { A defined type, constant, variable, etc. }
  TSDefEntry = record
    { Older entry in the stack. }
    Older : TSDefinition;
    { Newer entry in the stack. }
    Newer : TSDefinition;
    { Pointer to the definition. }
    case Cls : TSDefClass of 
      { Type. }
      SdcType : (TypeDef : TSDTypeDef);
      { Untyped constant. }
      SdcConstant : (ConstDef : TSDConstantDef);
      { Variable. }
      SdcVariable : (VarDef : TSDVariableDef);
      { Subroutine. }
      SdcSubroutine : (SrDef : TSDSubroutineDef);
      { Pseudo-function. }
      SdcPsfn : (PsfnDef : TSDPsfnDef);
      { Temporary variable. }
      SdcTmpVar : (TmpVarDef : TSDTmpVarDef);
      { "With" variable. }
      SdcWithVar : (WithVarDef : TSDWithVarDef);
  end;

  { Element of an array value. }
  TSEArrayElem = ^TSEArrayElemObj;
  TSEArrayElemObj = record
    { Next element. }
    Next : TSEArrayElem;
    { Value. }
    Value : TSExpression;
  end;
  { Element of an array value. }
  TSERecordField = ^TSERecordFieldObj;
  TSERecordFieldObj = record
    { Next element. }
    Next : TSERecordField;
    { Field ordinal. }
    Ordinal : integer;
    { Value. }
    Value : TSExpression;
  end;
  { Immediate bounds for a set constructor. }
  TSESetImmBounds = ^TSESetImmBoundsObj;
  TSESetImmBoundsObj = record
    { Next pair of bounds. }
    Next : TSESetImmBounds;
    { First and last ordinals. }
    First, Last : integer
  end;
  { Expression bounds for a set constructor. }
  TSESetExprBounds = ^TSESetExprBoundsObj;
  TSESetExprBoundsObj = record
    { Next pair of bounds. }
    Next : TSESetExprBounds;
    { First and last values. }
    First, Last : TSExpression
  end;

  { Immediate expression. }
  TSEImmediateClass = (SeicNil, SeicBoolean, SeicInteger, SeicReal, SeicChar,
                       SeicString, SeicEnum, SeicSet);
  TSEImmediate = record
    case Cls : TSEImmediateClass of 
      SeicBoolean : (BooleanVal : boolean);
      SeicInteger : (IntegerVal : integer);
      SeicReal : (RealVal : real);
      SeicChar : (CharVal : char);
      SeicString : (StringVal : string);
      SeicEnum : (EnumOrdinal : integer;
                  EnumPtr : TSDTEnum);
      SeicSet : (SetBounds : TSESetImmBounds;
                 SetOfTypePtr : TSDType)
  end;
  { Function arguments. }
  TSEFunctionArgs = record
    { Number of arguments. }
    Size : integer;
    { Values of the arguments. }
    Values : array[1..MaxSubroutineArgs] of TSExpression;
  end;
  { List of arguments for the READ procedure. }
  TSEReadArgList = ^TSEReadArgValue;
  TSEReadArgValue = record
    { Next argument. }
    Next : TSEReadArgList;
    { Target of the operation. }
    Dest : TSExpression
  end;
  { List of arguments for the WRITE procedure. }
  TSEWriteArgList = ^TSEWriteArgValue;
  TSEWriteArg = record
    { Source of the operation. }
    Arg : TSExpression;
    { Width. 0 = unspecified. }
    Width : TSExpression;
    { Precision. -1 = unspecified. }
    Prec : TSExpression
  end;
  TSEWriteArgValue = record
    { Next argument. }
    Next : TSEWriteArgList;
    { Value. }
    Value : TSEWriteArg
  end;
  { Operators. }
  TSEOperator = (SeoAdd, SeoSub, SeoMul, SeoDivReal, SeoDivInt, SeoMod, SeoNeg,
                 SeoAnd, SeoOr, SeoXor, SeoShl, SeoShr, SeoNot,
                 SeoIn,
                 SeoEq, SeoNe, SeoLt, SeoGt, SeoLtEq, SeoGtEq,
                 SeoOrd, SeoPred, SeoSucc);
  { Unary operation. }
  TSEUnaryOp = record
    { Operand. }
    Parent : TSExpression;
    { Operator. }
    Op : TSEOperator
  end;
  { Binary operation. }
  TSEBinaryOp = record
    { Left operand. }
    Left : TSExpression;
    { Right operand. }
    Right : TSExpression;
    { Operator. }
    Op : TSEOperator
  end;

  { Expressions. }
  TSExpressionClass = (SecImmediate, SecArrayValue, SecRecordValue, SecSetValue,
                       SecToString, SecToReal, SecToUntypedPtr,
                       SecToGenericFile, SecWithTmpVar, SecSubrange,
                       SecVariable, SecField, SecArray, SecPointer, SecAddress,
                       SecStringChar, SecFnRef, SecFnCall, SecPsfnRef,
                       SecSizeof, SecConvertToStr, SecConvertToVal, SecRead,
                       SecWrite, SecUnaryOp, SecBinaryOp);
  TSExpressionObj = record
    { Type of this expression. }
    TypePtr : TSDType;
    { Can you assign a value to the result of this expression? }
    IsAssignable : boolean;
    { Can you get the address of the result of this expression? }
    IsAddressable : boolean;
    { Is the result of this expression the result of a function? }
    IsFunctionResult : boolean;
    { Can the result of this expression work as a standalone statement? }
    IsStatement : boolean;
    { Check bounds of the result? }
    CheckBounds : boolean;
    case Cls : TSExpressionClass of 
      { Immediate expression. }
      SecImmediate : (Immediate : TSEImmediate);
      { Array value. }
      SecArrayValue : (ArrayElem : TSEArrayElem);
      { Record value. }
      SecRecordValue : (RecordField : TSERecordField);
      { Set value. }
      SecSetValue : (SetBase : TSExpression;
                     SetBounds : TSESetExprBounds);
      { Character to string cast. }
      SecToString : (ToStrParent : TSExpression);
      { Integer to real cast. }
      SecToReal : (ToRealParent : TSExpression);
      { Typed pointer to untyped pointer cast. }
      SecToUntypedPtr : (ToUntypedPtrParent : TSExpression);
      { Typed file to generic file cast. }
      SecToGenericFile : (ToGenericFileParent : TSExpression);
      { An expression that uses a temporary variable. }
      SecWithTmpVar : (TmpVarPtr : TSDTmpVar;
                       TmpVarValue : TSExpression;
                       TmpVarChild : TSExpression);
      { Base to subrange cast. }
      SecSubrange : (SubrangeParent : TSExpression);
      { Variable access. }
      SecVariable : (VarPtr : TSDVariable);
      { Field access. }
      SecField : (RecExpr : TSExpression;
                  RecFieldNum : integer);
      { Array access. }
      SecArray : (ArrayExpr : TSExpression;
                  ArrayIndex : TSExpression);
      { Pointer access. }
      SecPointer : (PointerExpr : TSExpression);
      { Address-of. }
      SecAddress : (AddressExpr : TSExpression);
      { String subscript. }
      SecStringChar : (StringExpr : TSExpression;
                       StringIndex : TSExpression);
      { Reference to subroutine. }
      SecFnRef : (FnPtr : TSDSubroutine);
      { Subroutine call. }
      SecFnCall : (FnExpr : TSExpression;
                   CallArgs : TSEFunctionArgs);
      { Reference to pseudo-function. }
      SecPsfnRef : (PsfnPtr : TSDPsfn);
      { SIZEOF() }
      SecSizeof : (SizeofTypePtr : TSDType);
      { STR() }
      SecConvertToStr : (ToStrSrc : TSEWriteArg;
                         ToStrDest : TSExpression);
      { VAL() }
      SecConvertToVal : (ToValSrc : TSExpression;
                         ToValDest : TSExpression;
                         ToValCode : TSExpression);
      { READ()/READLN() }
      SecRead : (ReadFile : TSExpression;
                 ReadArgs : TSEReadArgList;
                 ReadLn : boolean;
                 CheckIoResultAfterRead : boolean);
      { WRITE()/WRITELN() }
      SecWrite : (WriteFile : TSExpression;
                  WriteArgs : TSEWriteArgList;
                  WriteLn : boolean;
                  CheckIoResultAfterWrite : boolean);
      { Unary operation. }
      SecUnaryOp : (Unary : TSEUnaryOp);
      { Binary operation. }
      SecBinaryOp : (Binary : TSEBinaryOp);
  end;

  { A sequence of statements. }
  TSSSequenceEntry = record
    { Next entry in the sequence. }
    Next : TSSSequence;
    { Statement in this entry. }
    Statement : TSStatement;
  end;

  { An entry in a case statement. }
  TSSCase = ^TSSCaseEntry;
  TSSCaseEntry = record
    { Next entry in the case statement. }
    Next : TSSCase;
    { Label for the entry. Nil for 'else'. }
    CaseLabel : TSExpression;
    { Statement in this entry. }
    Statement : TSStatement;
  end;

  { Class of statement. }
  TSStatementClass = (SscEmpty, SscSequence, SscAssign, SscProcCall,
                      SscIf, SscRepeat, SscWhile, SscFor, SscWith, SscCase);
  TSStatementObj = record
    case Cls : TSStatementClass of 
      { Sequence. }
      SscSequence : (Sequence : TSSSequence);
      { Assign. }
      SscAssign : (Lhs, Rhs : TSExpression);
      { Procedure call. }
      SscProcCall : (ProcCall : TSExpression);
      { If statement. }
      SscIf : (IfCond : TSExpression;
               IfThen : TSStatement;
               IfElse : TSStatement);
      { Repeat statement. }
      SscRepeat : (UntilCond : TSExpression;
                   RepeatSequence : TSSSequence);
      { While statement. }
      SscWhile : (WhileCond : TSExpression;
                  WhileStatement : TSStatement);
      { For statement. }
      SscFor : (Iterator, First, Last : TSExpression;
                Ascending : boolean;
                ForStatement : TSStatement);
      { With statement. }
      SscWith : (WithVar : TSDVariable;
                 WithValue : TSExpression;
                 WithStatement : TSStatement);
      { Case statement. }
      SscCase : (CaseSelector : TSExpression;
                 CaseEntry : TSSCase);

  end;

  { Program syntax node. }
  TSProgram = ^TSProgramObj;
  TSProgramObj = record
    { The program's name. }
    Name : string;
    { Program's global scope. }
    Scope : TSScopeObj;
    { Program body. }
    Body : TSSSequence;
  end;

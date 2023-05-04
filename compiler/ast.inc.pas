type 
  { An untyped constant. }
  TSDConstant = ^TSDConstantObj;
  TSDConstantObj = record
    { Name of the constant. }
    Name : string;
    { Value of the constant. }
    Value : TExpression;
  end;

  { A name for a definition. }
  TSDName = ^TSDNameObj;
  TSDNameClass = (SdncType, SdncVariable, SdncConstant, SdncEnumVal,
                  SdncSubroutine, SdncPseudoFn);
  TSDNameObj = record
    { The name. }
    Name : string;
    { A pointer to the definition with this name. }
    case Cls : TSDNameClass of 
      SdncType : (TypePtr : TPsTypePtr);
      SdncVariable : (VarPtr : TPsVarPtr);
      SdncConstant : (ConstPtr : TSDConstant);
      SdncEnumVal : (EnumTypePtr : TPsTypePtr;
                     Ordinal : integer);
      SdncSubroutine : (SrPtr : TPsSubrPtr);
      SdncPseudoFn : (PseudoFnPtr : TPsPseudoFnPtr)
  end;

  { A defined type, constant, variable, etc. }
  TSDefinition = ^TSDefEntry;
  TSDefClass = (SdcName, SdcType, SdcConstant, SdcVariable,
                SdcSubroutine, SdcPseudoFn, SdcWithVar);
  TSDefEntry = record
    { Older entry in the stack. }
    Older : TSDefinition;
    { Newer entry in the stack. }
    Newer : TSDefinition;
    { Pointer to the definition. }
    case Cls : TSDefClass of 
      SdcName : (NamePtr : TSDName);
      SdcType : (TypePtr : TPsTypePtr);
      SdcConstant : (ConstPtr : TSDConstant);
      SdcVariable : (VarPtr : TPsVarPtr);
      SdcSubroutine : (SrPtr : TPsSubrPtr);
      SdcPseudoFn : (PseudoFnPtr : TPsPseudoFnPtr);
      SdcWithVar : (WithVarPtr : TPsWithVarPtr);
  end;

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
    CurrentFn : TPsSubrPtr;
    { Current state of the counters. }
    Counters : TSCounters;
  end;

  { Program syntax node. }
  TSProgram = ^TSProgramObj;
  TSProgramObj = record
    { Name of the program. }
    Name : string;
    { Program's global scope. }
    Scope : TSScopeObj;
  end;

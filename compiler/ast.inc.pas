type 
  TSDName = ^TSDNameObj;
  TSDNameClass = (SdncType, SdncVariable, SdncConstant, SdncEnumVal,
                  SdncSubroutine, SdncPseudoFn);
  TSDNameObj = record
    Name : string;
    case Cls : TSDNameClass of 
      SdncType : (TypePtr : TPsTypePtr);
      SdncVariable : (VarPtr : TPsVarPtr);
      SdncConstant : (ConstPtr : TPsConstPtr);
      SdncEnumVal : (EnumTypePtr : TPsTypePtr;
                     Ordinal : integer);
      SdncSubroutine : (SrPtr : TPsSubrPtr);
      SdncPseudoFn : (PseudoFnPtr : TPsPseudoFnPtr)
  end;

  TSDefinition = ^TSDefEntry;
  TSDefClass = (SdcName, SdcType, SdcConstant, SdcVariable,
                SdcSubroutine, SdcPseudoFn, SdcWithVar);
  TSDefEntry = record
    Older : TSDefinition;
    Newer : TSDefinition;
    case Cls : TSDefClass of 
      SdcName : (NamePtr : TSDName);
      SdcType : (TypePtr : TPsTypePtr);
      SdcConstant : (ConstPtr : TPsConstPtr);
      SdcVariable : (VarPtr : TPsVarPtr);
      SdcSubroutine : (SrPtr : TPsSubrPtr);
      SdcPseudoFn : (PseudoFnPtr : TPsPseudoFnPtr);
      SdcWithVar : (WithVarPtr : TPsWithVarPtr);
  end;

  TSCounterType = (SctEnum, SctRecord, SctTmpVar);
  TSCounters = record
    EnumCtr : integer;
    RecordCtr : integer;
    TmpVarCtr : integer;
  end;

  TSScope = ^TSScopeObj;
  TSScopeObj = record
    Parent : TSScope;
    LatestDef : TSDefinition;
    CurrentFn : TPsSubrPtr;
    Counters : TSCounters;
  end;

  TSProgram = ^TSProgramObj;
  TSProgramObj = record
    Name : string;
    Scope : TSScopeObj;
  end;

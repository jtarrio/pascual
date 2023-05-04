type 
  TSDefinition = ^TSDefEntry;
  TSDefClass = (TdcName, TdcType, TdcConstant, TdcVariable,
                TdcSubroutine, TdcPseudoFn, TdcWithVar);
  TSDefEntry = record
    Older : TSDefinition;
    Newer : TSDefinition;
    case Cls : TSDefClass of 
      TdcName : (NamePtr : TPsNamePtr);
      TdcType : (TypePtr : TPsTypePtr);
      TdcConstant : (ConstPtr : TPsConstPtr);
      TdcVariable : (VarPtr : TPsVarPtr);
      TdcSubroutine : (SrPtr : TPsSubrPtr);
      TdcPseudoFn : (PseudoFnPtr : TPsPseudoFnPtr);
      TdcWithVar : (WithVarPtr : TPsWithVarPtr);
  end;

  TSCounterType = (TctEnum, TctRecord, TctTmpVar);
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

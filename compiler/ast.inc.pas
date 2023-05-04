type 
  TSDefinition = ^TSDefEntry;
  TSDefClass = (TdcName, TdcType, TdcConstant, TdcVariable,
                TdcFunction, TdcPseudoFn, TdcWithVar);
  TSDefEntry = record
    Older : TSDefinition;
    Newer : TSDefinition;
    case Cls : TSDefClass of 
      TdcName : (NamePtr : TPsNamePtr);
      TdcType : (TypePtr : TPsTypePtr);
      TdcConstant : (ConstPtr : TPsConstPtr);
      TdcVariable : (VarPtr : TPsVarPtr);
      TdcFunction : (FnPtr : TPsFnPtr);
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
    CurrentFn : TPsFnPtr;
    Counters : TSCounters;
  end;

  TSProgram = ^TSProgramObj;
  TSProgramObj = record
    Name : string;
    Scope : TSScopeObj;
  end;

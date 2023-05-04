type
  TSScope = ^TSScopeObj;
  TSScopeObj = record
    Parent : TSScope;
    LatestDef : TPsDefPtr;
    CurrentFn : TPsFnPtr;
    Counters : TPsCounters;
  end;

  TSProgram = ^TSProgramObj;
  TSProgramObj = record
    Name : string;
    Scope : TSScopeObj;
  end;

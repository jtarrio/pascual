procedure CreateGlobalDefinitions;
var 
  Def : TSDefinition;
begin
  new(GlobalDefinitions);
  PushGlobalDefs(GlobalDefinitions);

  { Primitive types }
  PrimitiveTypes.PtNil := MakeBaseType('NIL', SdtcNil);
  PrimitiveTypes.PtBoolean := MakeBaseType('BOOLEAN', SdtcBoolean);
  PrimitiveTypes.PtInteger := MakeBaseType('INTEGER', SdtcInteger);
  PrimitiveTypes.PtReal := MakeBaseType('REAL', SdtcReal);
  PrimitiveTypes.PtChar := MakeBaseType('CHAR', SdtcChar);
  PrimitiveTypes.PtString := MakeBaseType('STRING', SdtcString);
  PrimitiveTypes.PtText := MakeTextType;
  PrimitiveTypes.PtFile := MakeGenericFileType;
  PrimitiveTypes.PtEmptySet := MakeSetType(nil);
  PrimitiveTypes.PtUntypedPtr := MakePointerType(nil);

  { Mandatory constants }
  AddConstant(MakeConstant('FALSE', ExBooleanConstant(false)));
  AddConstant(MakeConstant('TRUE', ExBooleanConstant(true)));
  AddConstant(MakeConstant('MAXINT', ExIntegerConstant(32767)));

  { Ordinal subroutines }
  AddPsfn('ORD', @PfOrd_Parse);
  AddPsfn('PRED', @PfPred_Parse);
  AddPsfn('SUCC', @PfSucc_Parse);

  { Memory subroutines }
  AddPsfn('DISPOSE', @PfDispose_Parse);
  AddPsfn('NEW', @PfNew_Parse);
  AddPsfn('SIZEOF', @PfSizeof_Parse);
  AddFunction(MakeProcedure1('Dispose',
              MakeVarArg('PTR', PrimitiveTypes.PtUntypedPtr)));
  AddFunction(MakeProcedure2('New',
              MakeVarArg('PTR', PrimitiveTypes.PtUntypedPtr),
              MakeArg('SIZE', PrimitiveTypes.PtInteger)));

  RegisterGlobals_Io;
  RegisterGlobals_Math;
  RegisterGlobals_Strings;

  { Misc subroutines }
  AddPsfn('RANDOM', @PfRandom_Parse);
  AddFunction(MakeFunction0('ENVCOUNT', PrimitiveTypes.PtInteger));
  AddFunction(MakeFunction1('ENVSTR', PrimitiveTypes.PtString,
              MakeArg('I', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction1('GETENV', PrimitiveTypes.PtString,
              MakeConstArg('VARNAME', PrimitiveTypes.PtString)));
  AddFunction(MakeProcedure1('HALT',
              MakeArg('CODE', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction0('PARAMCOUNT', PrimitiveTypes.PtInteger));
  AddFunction(MakeFunction1('PARAMSTR', PrimitiveTypes.PtString,
              MakeArg('I', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction0('RANDOM_n', PrimitiveTypes.PtReal));
  AddFunction(MakeFunction1('RANDOM_i', PrimitiveTypes.PtInteger,
              MakeArg('NUM', PrimitiveTypes.PtInteger)));
  AddFunction(MakeProcedure0('RANDOMIZE'));

  { Mark everything as initialized and used }
  Def := CurrentScope^.LatestDef;
  while Def <> nil do
  begin
    case Def^.Cls of 
      SdcVariable:
                   begin
                     Def^.VarDef.WasInitialized := true;
                     Def^.VarDef.WasUsed := true
                   end;
      SdcSubroutine: Def^.SrDef.WasUsed := true;
      SdcType: Def^.TypeDef.WasUsed := true;
    end;
    Def := Def^.Older
  end
end;

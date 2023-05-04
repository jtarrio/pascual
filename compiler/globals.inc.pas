procedure CreateGlobalDefinitions;
var 
  Def : TPsDefPtr;
begin
  new(GlobalDefinitions);
  PushGlobalDefs(GlobalDefinitions);

  { Primitive types }
  PrimitiveTypes.PtNil := MakeBaseType('NIL', TtcNil);
  PrimitiveTypes.PtBoolean := MakeBaseType('BOOLEAN', TtcBoolean);
  PrimitiveTypes.PtInteger := MakeBaseType('INTEGER', TtcInteger);
  PrimitiveTypes.PtReal := MakeBaseType('REAL', TtcReal);
  PrimitiveTypes.PtChar := MakeBaseType('CHAR', TtcChar);
  PrimitiveTypes.PtString := MakeBaseType('STRING', TtcString);
  PrimitiveTypes.PtText := AddTypeName('TEXT', MakeTextType)^.TypePtr;
  PrimitiveTypes.PtFile := MakeGenericFileType;
  PrimitiveTypes.PtEmptySet := MakeSetType(nil);
  PrimitiveTypes.PtUntypedPtr := AddTypeName('POINTER', 
                                 MakePointerType(nil))^.TypePtr;

  { Mandatory constants }
  AddConstant(MakeConstant('FALSE', ExBooleanConstant(false)));
  AddConstant(MakeConstant('TRUE', ExBooleanConstant(true)));
  AddConstant(MakeConstant('MAXINT', ExIntegerConstant(32767)));

  { Ordinal subroutines }
  AddPseudoFn('ORD', @PfOrd_Parse);
  AddPseudoFn('PRED', @PfPred_Parse);
  AddPseudoFn('SUCC', @PfSucc_Parse);

  { Memory subroutines }
  AddPseudoFn('DISPOSE', @PfDispose_Parse);
  AddPseudoFn('NEW', @PfNew_Parse);
  AddPseudoFn('SIZEOF', @PfSizeof_Parse);
  AddFunction(MakeProcedure1('Dispose',
              MakeVarArg('PTR', PrimitiveTypes.PtUntypedPtr)));
  AddFunction(MakeProcedure2('New',
              MakeVarArg('PTR', PrimitiveTypes.PtUntypedPtr),
              MakeArg('SIZE', PrimitiveTypes.PtInteger)));

  RegisterGlobals_Io;
  RegisterGlobals_Math;
  RegisterGlobals_Strings;

  { Misc subroutines }
  AddPseudoFn('RANDOM', @PfRandom_Parse);
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
      TdcVariable:
                   begin
                     Def^.VarPtr^.WasInitialized := true;
                     Def^.VarPtr^.WasUsed := true
                   end;
      TdcFunction: Def^.FnPtr^.WasUsed := true;
      TdcType: Def^.TypePtr^.WasUsed := true;
    end;
    Def := Def^.Older
  end
end;

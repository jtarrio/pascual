procedure StartGlobalScope;
var 
  Def : TPsDefPtr;
begin
  InitDefs;

  { Primitive types }
  PrimitiveTypes.PtNil := MakeBaseType('NIL', TtcNil);
  PrimitiveTypes.PtBoolean := MakeBaseType('BOOLEAN', TtcBoolean);
  PrimitiveTypes.PtInteger := MakeBaseType('INTEGER', TtcInteger);
  PrimitiveTypes.PtReal := MakeBaseType('REAL', TtcReal);
  PrimitiveTypes.PtChar := MakeBaseType('CHAR', TtcChar);
  PrimitiveTypes.PtString := MakeBaseType('STRING', TtcString);
  PrimitiveTypes.PtText := MakeBaseType('TEXT', TtcText);
  PrimitiveTypes.PtEmptySet := MakeSetType(nil);

  { Mandatory constants }
  AddConstant(MakeConstant('FALSE', ExBooleanConstant(false)));
  AddConstant(MakeConstant('TRUE', ExBooleanConstant(true)));
  AddConstant(MakeConstant('MAXINT', ExIntegerConstant(32767)));

  { Ordinal subroutines }
  AddPseudoFn('ORD', @PfOrd_Parse, @Pf_Unary_Describe);
  AddPseudoFn('PRED', @PfPred_Parse, @Pf_Unary_Describe);
  AddPseudoFn('SUCC', @PfSucc_Parse, @Pf_Unary_Describe);

  { Memory subroutines }
  AddPseudoFn('DISPOSE', @PfDispose_Parse, @Pf_Unary_Describe);
  AddPseudoFn('NEW', @PfNew_Parse, @Pf_Unary_Describe);
  AddPseudoFn('SIZEOF', @PfSizeof_Parse, @PfSizeof_Describe);

  RegisterGlobals_Io;
  RegisterGlobals_Math;
  RegisterGlobals_Strings;

  { Misc subroutines }
  AddPseudoFn('RANDOM', @PfRandom_Parse, @Pf_Indef_Describe);
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
  Def := Defs.Latest;
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
    Def := Def^.Prev
  end;

  { Let the user redefine all the global names }
  StartTemporaryScope;
end;

procedure StartGlobalScope;
var 
  Def : TPsDefPtr;
begin
  InitDefs;

  { Primitive types }
  PrimitiveTypes.PtNil := AddType(MakeType('NIL', TtcNil));
  PrimitiveTypes.PtBoolean := AddType(MakeType('BOOLEAN', TtcBoolean));
  PrimitiveTypes.PtInteger := AddType(MakeType('INTEGER', TtcInteger));
  PrimitiveTypes.PtReal := AddType(MakeType('REAL', TtcReal));
  PrimitiveTypes.PtChar := AddType(MakeType('CHAR', TtcChar));
  PrimitiveTypes.PtString := AddType(MakeType('STRING', TtcString));
  PrimitiveTypes.PtText := AddType(MakeType('TEXT', TtcText));

  { Mandatory constants }
  AddConstant(MakeConstant('FALSE', ExBooleanConstant(false)));
  AddConstant(MakeConstant('TRUE', ExBooleanConstant(true)));
  AddConstant(MakeConstant('MAXINT', ExIntegerConstant(32767)));

  { Default files }
  AddVariable(MakeVariable('INPUT', PrimitiveTypes.PtText));
  AddVariable(MakeVariable('OUTPUT', PrimitiveTypes.PtText));
  AddVariable(MakeVariable('STDERR', PrimitiveTypes.PtText));

  { Ordinal subroutines }
  AddPseudoFn('ORD', TpfOrd);
  AddPseudoFn('PRED', TpfPred);
  AddPseudoFn('SUCC', TpfSucc);

  { Memory subroutines }
  AddPseudoFn('DISPOSE', TpfDispose);
  AddPseudoFn('NEW', TpfNew);

  { I/O subroutines }
  AddPseudoFn('READ', TpfRead);
  AddPseudoFn('READLN', TpfReadln);
  AddPseudoFn('WRITE', TpfWrite);
  AddPseudoFn('WRITELN', TpfWriteln);
  AddFunction(MakeProcedure2('ASSIGN',
              MakeArg('F', PrimitiveTypes.PtText, true),
              MakeArg('NAME', PrimitiveTypes.PtString, false)));
  AddFunction(MakeProcedure1('CLOSE',
              MakeArg('F', PrimitiveTypes.PtText, true)));
  AddFunction(MakeFunction1('EOF', PrimitiveTypes.PtBoolean,
              MakeArg('F', PrimitiveTypes.PtText, true)));
  AddFunction(MakeProcedure1('RESET',
              MakeArg('F', PrimitiveTypes.PtText, true)));
  AddFunction(MakeProcedure1('REWRITE',
              MakeArg('F', PrimitiveTypes.PtText, true)));

  { Character and String subroutines }
  AddPseudoFn('CONCAT', TpfConcat);
  AddPseudoFn('STR', TpfStr);
  AddPseudoFn('VAL', TpfVal);
  AddFunction(MakeFunction1('CHR', PrimitiveTypes.PtChar,
              MakeArg('POS', PrimitiveTypes.PtInteger, false)));
  AddFunction(MakeFunction3('COPY', PrimitiveTypes.PtString,
              MakeArg('STR', PrimitiveTypes.PtString, false),
              MakeArg('POS', PrimitiveTypes.PtInteger, false),
              MakeArg('NUM', PrimitiveTypes.PtInteger, false)));
  AddFunction(MakeProcedure3('DELETE',
              MakeArg('STR', PrimitiveTypes.PtString, true),
              MakeArg('POS', PrimitiveTypes.PtInteger, false),
              MakeArg('NUM', PrimitiveTypes.PtInteger, false)));
  AddFunction(MakeProcedure3('INSERT',
              MakeArg('INS', PrimitiveTypes.PtString, false),
              MakeArg('TARGET', PrimitiveTypes.PtString, true),
              MakeArg('POS', PrimitiveTypes.PtInteger, false)));
  AddFunction(MakeFunction1('LENGTH', PrimitiveTypes.PtInteger,
              MakeArg('STR', PrimitiveTypes.PtString, false)));
  AddFunction(MakeFunction2('POS', PrimitiveTypes.PtInteger,
              MakeArg('NEEDLE', PrimitiveTypes.PtString, false),
              MakeArg('HAYSTACK', PrimitiveTypes.PtString, false)));
  AddFunction(MakeFunction1('UPCASE', PrimitiveTypes.PtChar,
              MakeArg('CHR', PrimitiveTypes.PtChar, false)));

  { Misc subroutines }
  AddFunction(MakeProcedure1('HALT',
              MakeArg('CODE', PrimitiveTypes.PtInteger, false)));
  AddFunction(MakeFunction0('PARAMCOUNT', PrimitiveTypes.PtInteger));
  AddFunction(MakeFunction1('PARAMSTR', PrimitiveTypes.PtString,
              MakeArg('I', PrimitiveTypes.PtInteger, false)));

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

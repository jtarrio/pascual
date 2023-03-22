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
  AddConstant(MakeConstant('PI', ExRealConstant(3.141592653589793238)));

  { Default files }
  AddVariable(MakeVariable('INPUT', PrimitiveTypes.PtText));
  AddVariable(MakeVariable('OUTPUT', PrimitiveTypes.PtText));
  AddVariable(MakeVariable('STDERR', PrimitiveTypes.PtText));

  { Ordinal subroutines }
  PseudoFuns.Ord := AddPseudoFn('ORD', @PfOrd_Parse, @Pf_Unary_Describe);
  PseudoFuns.Pred := AddPseudoFn('PRED', @PfPred_Parse, @Pf_Unary_Describe);
  PseudoFuns.Succ := AddPseudoFn('SUCC', @PfSucc_Parse, @Pf_Unary_Describe);

  { Math subroutines }
  AddPseudoFn('ABS', @PfAbs_Parse, @Pf_Unary_Describe);
  AddPseudoFn('SQR', @PfSqr_Parse, @Pf_Unary_Describe);
  AddFunction(MakeFunction1('ABS_i', PrimitiveTypes.PtInteger,
              MakeArg('NUM', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction1('ABS_r', PrimitiveTypes.PtReal,
              MakeArg('NUM', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('ARCTAN', PrimitiveTypes.PtReal,
              MakeArg('TAN', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('COS', PrimitiveTypes.PtReal,
              MakeArg('ANGLE', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('EXP', PrimitiveTypes.PtReal,
              MakeArg('POW', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('FRAC', PrimitiveTypes.PtReal,
              MakeArg('X', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('INT', PrimitiveTypes.PtReal,
              MakeArg('X', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('LN', PrimitiveTypes.PtReal,
              MakeArg('X', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('ODD', PrimitiveTypes.PtBoolean,
              MakeArg('X', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction1('ROUND', PrimitiveTypes.PtInteger,
              MakeArg('X', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('SIN', PrimitiveTypes.PtReal,
              MakeArg('ANGLE', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('SQR_i', PrimitiveTypes.PtInteger,
              MakeArg('NUM', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction1('SQR_r', PrimitiveTypes.PtReal,
              MakeArg('NUM', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('SQRT', PrimitiveTypes.PtReal,
              MakeArg('X', PrimitiveTypes.PtReal)));
  AddFunction(MakeFunction1('TRUNC', PrimitiveTypes.PtInteger,
              MakeArg('X', PrimitiveTypes.PtReal)));

  { Memory subroutines }
  PseudoFuns.Dispose := AddPseudoFn('DISPOSE', @PfDispose_Parse, @Pf_Unary_Describe);
  PseudoFuns.New := AddPseudoFn('NEW', @PfNew_Parse, @Pf_Unary_Describe);

  { I/O subroutines }
  PseudoFuns.Read := AddPseudoFn('READ', @PfRead_Parse, @Pf_Indef_Describe);
  PseudoFuns.Readln := AddPseudoFn('READLN', @PfRead_Parse, @Pf_Indef_Describe);
  PseudoFuns.Write := AddPseudoFn('WRITE', @PfWrite_Parse, @Pf_Indef_Describe);
  PseudoFuns.Writeln := AddPseudoFn('WRITELN', @PfWrite_Parse, @Pf_Indef_Describe);
  AddFunction(MakeProcedure2('ASSIGN',
              MakeVarArg('F', PrimitiveTypes.PtText),
              MakeConstArg('NAME', PrimitiveTypes.PtString)));
  AddFunction(MakeProcedure1('CLOSE',
              MakeVarArg('F', PrimitiveTypes.PtText)));
  AddFunction(MakeFunction1('EOF', PrimitiveTypes.PtBoolean,
              MakeVarArg('F', PrimitiveTypes.PtText)));
  AddFunction(MakeFunction1('EOLN', PrimitiveTypes.PtBoolean,
              MakeVarArg('F', PrimitiveTypes.PtText)));
  AddFunction(MakeFunction1('FILEPOS', PrimitiveTypes.PtInteger,
              MakeConstArg('F', PrimitiveTypes.PtText)));
  AddFunction(MakeFunction1('FILESIZE', PrimitiveTypes.PtInteger,
              MakeConstArg('F', PrimitiveTypes.PtText)));
  AddFunction(MakeProcedure1('FLUSH', 
              MakeConstArg('F', PrimitiveTypes.PtText)));
  AddFunction(MakeFunction0('IORESULT', PrimitiveTypes.PtInteger));
  AddFunction(MakeProcedure1('RESET',
              MakeVarArg('F', PrimitiveTypes.PtText)));
  AddFunction(MakeProcedure1('REWRITE',
              MakeVarArg('F', PrimitiveTypes.PtText)));
  AddFunction(MakeProcedure2('SEEK',
              MakeVarArg('F', PrimitiveTypes.PtText),
              MakeArg('POS', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction1('SEEKEOF', PrimitiveTypes.PtBoolean,
              MakeVarArg('F', PrimitiveTypes.PtText)));
  AddFunction(MakeFunction1('SEEKEOLN', PrimitiveTypes.PtBoolean,
              MakeVarArg('F', PrimitiveTypes.PtText)));

  { File and directory subroutines }
  AddFunction(MakeProcedure1('CHDIR',
              MakeConstArg('DIR', PrimitiveTypes.PtString)));
  AddFunction(MakeProcedure1('ERASE',
              MakeVarArg('F', PrimitiveTypes.PtText)));
  AddFunction(MakeProcedure2('GETDIR',
              MakeArg('DRIVE', PrimitiveTypes.PtInteger),
              MakeVarArg('DIR', PrimitiveTypes.PtString)));
  AddFunction(MakeProcedure1('MKDIR',
              MakeConstArg('DIR', PrimitiveTypes.PtString)));
  AddFunction(MakeProcedure2('RENAME',
              MakeVarArg('F', PrimitiveTypes.PtText),
              MakeConstArg('NAME', PrimitiveTypes.PtString)));
  AddFunction(MakeProcedure1('RMDIR',
              MakeConstArg('DIR', PrimitiveTypes.PtString)));

  { Character and String subroutines }
  AddPseudoFn('CONCAT', @PfConcat_Parse, @Pf_Indef_Describe);
  PseudoFuns.Str := AddPseudoFn('STR', @PfStr_Parse, @PfStr_Describe);
  PseudoFuns.Val := AddPseudoFn('VAL', @PfVal_Parse, @Pf_Ternary_Describe);
  AddFunction(MakeFunction1('CHR', PrimitiveTypes.PtChar,
              MakeArg('POS', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction3('COPY', PrimitiveTypes.PtString,
              MakeConstArg('STR', PrimitiveTypes.PtString),
              MakeArg('POS', PrimitiveTypes.PtInteger),
              MakeArg('NUM', PrimitiveTypes.PtInteger)));
  AddFunction(MakeProcedure3('DELETE',
              MakeVarArg('STR', PrimitiveTypes.PtString),
              MakeArg('POS', PrimitiveTypes.PtInteger),
              MakeArg('NUM', PrimitiveTypes.PtInteger)));
  AddFunction(MakeProcedure3('INSERT',
              MakeConstArg('INS', PrimitiveTypes.PtString),
              MakeVarArg('TARGET', PrimitiveTypes.PtString),
              MakeArg('POS', PrimitiveTypes.PtInteger)));
  AddFunction(MakeFunction1('LENGTH', PrimitiveTypes.PtInteger,
              MakeConstArg('STR', PrimitiveTypes.PtString)));
  AddFunction(MakeFunction2('POS', PrimitiveTypes.PtInteger,
              MakeConstArg('NEEDLE', PrimitiveTypes.PtString),
              MakeConstArg('HAYSTACK', PrimitiveTypes.PtString)));
  AddFunction(MakeFunction1('UPCASE', PrimitiveTypes.PtChar,
              MakeArg('CHR', PrimitiveTypes.PtChar)));

  { Misc subroutines }
  AddPseudoFn('RANDOM', @PfRandom_Parse, @Pf_Indef_Describe);
  PseudoFuns.Sizeof := AddPseudoFn('SIZEOF', @PfSizeof_Parse, @PfSizeof_Describe);
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

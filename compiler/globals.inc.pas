procedure StartGlobalScope;
var 
  Fun : TPsFunction;
  Def : TPsDefPtr;
begin
  InitDefs;

  PrimitiveTypes.PtNil := AddType(MakeType('NIL', TtcNil));
  PrimitiveTypes.PtBoolean := AddType(MakeType('BOOLEAN', TtcBoolean));
  PrimitiveTypes.PtInteger := AddType(MakeType('INTEGER', TtcInteger));
  PrimitiveTypes.PtChar := AddType(MakeType('CHAR', TtcChar));
  PrimitiveTypes.PtString := AddType(MakeType('STRING', TtcString));
  PrimitiveTypes.PtText := AddType(MakeType('TEXT', TtcText));

  AddConstant(MakeConstant('FALSE', ExBooleanConstant(false)));
  AddConstant(MakeConstant('TRUE', ExBooleanConstant(true)));
  AddConstant(MakeConstant('MAXINT', ExIntegerConstant(32767)));

  AddVariable(MakeVariable('INPUT', PrimitiveTypes.PtText, false));
  AddVariable(MakeVariable('OUTPUT', PrimitiveTypes.PtText, false));
  AddVariable(MakeVariable('STDERR', PrimitiveTypes.PtText, false));

  AddPseudoFn('DISPOSE', TpfDispose);
  AddPseudoFn('NEW', TpfNew);
  AddPseudoFn('ORD', TpfOrd);
  AddPseudoFn('PRED', TpfPred);
  AddPseudoFn('READ', TpfRead);
  AddPseudoFn('READLN', TpfReadln);
  AddPseudoFn('STR', TpfStr);
  AddPseudoFn('SUCC', TpfSucc);
  AddPseudoFn('WRITE', TpfWrite);
  AddPseudoFn('WRITELN', TpfWriteln);

  Fun.Name := 'ASSIGN';
  Fun.ArgCount := 2;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.Args[2] := MakeVariable('NAME', PrimitiveTypes.PtString, false);
  Fun.ReturnTypePtr := nil;
  AddFunction(Fun);
  Fun.Name := 'CHR';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('POS', PrimitiveTypes.PtInteger, false);
  Fun.ReturnTypePtr := PrimitiveTypes.PtChar;
  AddFunction(Fun);
  Fun.Name := 'CLOSE';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.ReturnTypePtr := nil;
  AddFunction(Fun);
  Fun.Name := 'COPY';
  Fun.ArgCount := 3;
  Fun.Args[1] := MakeVariable('STR', PrimitiveTypes.PtString, false);
  Fun.Args[2] := MakeVariable('POS', PrimitiveTypes.PtInteger, false);
  Fun.Args[3] := MakeVariable('NUM', PrimitiveTypes.PtInteger, false);
  Fun.ReturnTypePtr := PrimitiveTypes.PtString;
  AddFunction(Fun);
  Fun.Name := 'DELETE';
  Fun.ArgCount := 3;
  Fun.Args[1] := MakeVariable('STR', PrimitiveTypes.PtString, true);
  Fun.Args[2] := MakeVariable('POS', PrimitiveTypes.PtInteger, false);
  Fun.Args[3] := MakeVariable('NUM', PrimitiveTypes.PtInteger, false);
  Fun.ReturnTypePtr := nil;
  AddFunction(Fun);
  Fun.Name := 'EOF';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.ReturnTypePtr := PrimitiveTypes.PtBoolean;
  AddFunction(Fun);
  Fun.Name := 'HALT';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('CODE', PrimitiveTypes.PtInteger, false);
  Fun.ReturnTypePtr := nil;
  AddFunction(Fun);
  Fun.Name := 'LENGTH';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('STR', PrimitiveTypes.PtString, false);
  Fun.ReturnTypePtr := PrimitiveTypes.PtInteger;
  AddFunction(Fun);
  Fun.Name := 'PARAMCOUNT';
  Fun.ArgCount := 0;
  Fun.ReturnTypePtr := PrimitiveTypes.PtInteger;
  AddFunction(Fun);
  Fun.Name := 'PARAMSTR';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('I', PrimitiveTypes.PtInteger, false);
  Fun.ReturnTypePtr := PrimitiveTypes.PtString;
  AddFunction(Fun);
  Fun.Name := 'RESET';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.ReturnTypePtr := nil;
  AddFunction(Fun);
  Fun.Name := 'REWRITE';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.ReturnTypePtr := nil;
  AddFunction(Fun);
  Fun.Name := 'UPCASE';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('CHR', PrimitiveTypes.PtChar, false);
  Fun.ReturnTypePtr := PrimitiveTypes.PtChar;
  AddFunction(Fun);

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
  end
end;

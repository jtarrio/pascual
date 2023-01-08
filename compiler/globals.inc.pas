procedure StartGlobalScope;
var 
  Fun : TPsFunction;
begin
  ClearDefs;
  GlobalScope := GetCurrentScope;

  PrimitiveTypes.PtBoolean := AddType(MakeType('BOOLEAN', TtcBoolean),
                              GlobalScope);
  PrimitiveTypes.PtInteger := AddType(MakeType('INTEGER', TtcInteger),
                              GlobalScope);
  PrimitiveTypes.PtChar := AddType(MakeType('CHAR', TtcChar), GlobalScope);
  PrimitiveTypes.PtString := AddType(MakeType('STRING', TtcString),
                             GlobalScope);
  PrimitiveTypes.PtText := AddType(MakeType('TEXT', TtcText), GlobalScope);

  AddConstant(MakeConstant('FALSE', TkFalse, 'FALSE'), GlobalScope);
  AddConstant(MakeConstant('TRUE', TkTrue, 'TRUE'), GlobalScope);

  AddVariable(MakeVariable('INPUT', PrimitiveTypes.PtText, false), GlobalScope);
  AddVariable(MakeVariable('OUTPUT', PrimitiveTypes.PtText, false),
  GlobalScope);
  AddVariable(MakeVariable('STDERR', PrimitiveTypes.PtText, false),
  GlobalScope);

  Fun.Name := 'ASSIGN';
  Fun.ArgCount := 2;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.Args[2] := MakeVariable('NAME', PrimitiveTypes.PtString, false);
  Fun.ReturnTypeIndex := 0;
  AddFunction(Fun);
  Fun.Name := 'CLOSE';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.ReturnTypeIndex := 0;
  AddFunction(Fun);
  Fun.Name := 'COPY';
  Fun.ArgCount := 3;
  Fun.Args[1] := MakeVariable('STR', PrimitiveTypes.PtString, false);
  Fun.Args[2] := MakeVariable('POS', PrimitiveTypes.PtInteger, false);
  Fun.Args[3] := MakeVariable('NUM', PrimitiveTypes.PtInteger, false);
  Fun.ReturnTypeIndex := PrimitiveTypes.PtString;
  AddFunction(Fun);
  Fun.Name := 'DELETE';
  Fun.ArgCount := 3;
  Fun.Args[1] := MakeVariable('STR', PrimitiveTypes.PtString, true);
  Fun.Args[2] := MakeVariable('POS', PrimitiveTypes.PtInteger, false);
  Fun.Args[3] := MakeVariable('NUM', PrimitiveTypes.PtInteger, false);
  Fun.ReturnTypeIndex := 0;
  AddFunction(Fun);
  Fun.Name := 'EOF';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.ReturnTypeIndex := PrimitiveTypes.PtBoolean;
  AddFunction(Fun);
  Fun.Name := 'HALT';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('CODE', PrimitiveTypes.PtInteger, false);
  Fun.ReturnTypeIndex := 0;
  AddFunction(Fun);
  Fun.Name := 'LENGTH';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('STR', PrimitiveTypes.PtString, false);
  Fun.ReturnTypeIndex := PrimitiveTypes.PtInteger;
  AddFunction(Fun);
  Fun.Name := 'PARAMCOUNT';
  Fun.ArgCount := 0;
  Fun.ReturnTypeIndex := PrimitiveTypes.PtInteger;
  AddFunction(Fun);
  Fun.Name := 'PARAMSTR';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('I', PrimitiveTypes.PtInteger, false);
  Fun.ReturnTypeIndex := PrimitiveTypes.PtString;
  AddFunction(Fun);
  Fun.Name := 'RESET';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.ReturnTypeIndex := 0;
  AddFunction(Fun);
  Fun.Name := 'REWRITE';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('F', PrimitiveTypes.PtText, true);
  Fun.ReturnTypeIndex := 0;
  AddFunction(Fun);
  Fun.Name := 'UPCASE';
  Fun.ArgCount := 1;
  Fun.Args[1] := MakeVariable('CHR', PrimitiveTypes.PtChar, false);
  Fun.ReturnTypeIndex := PrimitiveTypes.PtChar;
  AddFunction(Fun);
end;

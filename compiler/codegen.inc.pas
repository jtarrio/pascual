var 
  Codegen : record
    Output : text;
  end;

procedure OutBegin;
begin
  writeln(Codegen.Output, '{')
end;

procedure OutEnd;
begin
  writeln(Codegen.Output, '}')
end;

procedure OutEnumValues;
var 
  PosInEnum : integer;
begin
  write(Codegen.Output, 'const char* enumvalues', EnumIndex^.Id, '[] = { ');
  for PosInEnum := 1 to EnumIndex^.Size do
  begin
    if PosInEnum <> 1 then write(Codegen.Output, ', ');
    write(Codegen.Output, '"', EnumIndex^.Values[PosInEnum], '"')
  end;
  writeln(Codegen.Output, ' };')
end;

procedure OutEnumValuesFromCheckpoint;
var 
  Def : TPsDefPtr;
begin
  Def := Checkpoint^.Next;
  while Def <> nil do
  begin
    if Def^.Cls = TdcEnum then OutEnumValues(Def^.EnumIndex);
    Def := Def^.Next
  end
end;

function OutVariableName;
begin
  if IsReference then OutVariableName := '*' + Name
  else OutVariableName := Name
end;

function OutReturnVariableName;
begin
  OutReturnVariableName := 'return_' + Name
end;

procedure OutTypeReference;
begin
  if TypeIndex = nil then write(Codegen.Output, 'void')
  else if TypeIndex^.Cls = TtcPointer then
  begin
    OutTypeReference(TypeIndex^.PointedTypeIndex);
    write(Codegen.Output, '*')
  end
  else if TypeIndex^.Cls = TtcBoolean then write(Codegen.Output, 'PBoolean')
  else if TypeIndex^.Cls = TtcInteger then write(Codegen.Output, 'int')
  else if TypeIndex^.Cls = TtcChar then write(Codegen.Output, 'char')
  else if TypeIndex^.Cls = TtcString then write(Codegen.Output, 'PString')
  else if TypeIndex^.Cls = TtcText then write(Codegen.Output, 'PFile')
  else if TypeIndex^.Cls = TtcEnum then
         write(Codegen.Output, 'enum enum', TypeIndex^.EnumIndex^.Id)
  else if TypeIndex^.Cls = TtcRecord then
         write(Codegen.Output, 'struct record', TypeIndex^.RecordIndex^.Id)
  else if TypeIndex^.Cls = TtcArray then
  begin
    OutTypeReference(TypeIndex^.ArrayIndex^.TypeIndex);
    write(Codegen.Output, '*')
  end
  else
    CompileError('Error writing type reference: ' + TypeName(TypeIndex))
end;

procedure OutNameAndRecord(Name : string; RecordIndex : TPsRecordIndex);
var 
  Pos : integer;
  NumVariant : integer;
begin
  NumVariant := 0;
  write(Codegen.Output, 'struct record', RecordIndex^.Id);
  if not RecordIndex^.HasBeenDefined then
  begin
    write(Codegen.Output, ' { ');
    for Pos := 1 to RecordIndex^.Size do
    begin
      if (RecordIndex^.NumVariants > NumVariant)
         and (RecordIndex^.VariantBounds[NumVariant + 1] = Pos) then
      begin
        NumVariant := NumVariant + 1;
        if NumVariant = 1 then
          write(Codegen.Output, 'union { ')
        else
          write(Codegen.Output, '}; ');
        write(Codegen.Output, 'struct { ')
      end;
      OutNameAndType(RecordIndex^.Fields[Pos].Name,
                     RecordIndex^.Fields[Pos].TypeIndex);
      write(Codegen.Output, '; ')
    end;
    if NumVariant > 0 then
      write(Codegen.Output, '}; }; ');
    write(Codegen.Output, '}');
    RecordIndex^.HasBeenDefined := true
  end;
  write(Codegen.Output, ' ', Name)
end;

procedure OutNameAndEnum(Name : string; EnumIndex : TPsEnumIndex);
var 
  Pos : integer;
begin
  write(Codegen.Output, 'enum enum', EnumIndex^.Id);
  if not EnumIndex^.HasBeenDefined then
  begin
    write(Codegen.Output, ' { ');
    for Pos := 1 to EnumIndex^.Size do
    begin
      if Pos > 1 then
        write(Codegen.Output, ', ');
      write(Codegen.Output, EnumIndex^.Values[Pos])
    end;
    write(Codegen.Output, '}');
    EnumIndex^.HasBeenDefined := true
  end;
  write(Codegen.Output, ' ', Name)
end;

procedure OutNameAndType;
var 
  Arr : TPsArrayDef;
begin
  if TypeIndex = nil then write(Codegen.Output, 'void ', Name)
  else if TypeIndex^.Cls = TtcPointer then
  begin
    OutTypeReference(TypeIndex^.PointedTypeIndex);
    write(Codegen.Output, ' *', Name)
  end
  else if (TypeIndex^.AliasFor <> nil) and (TypeIndex^.Name <> '') then
         write(Codegen.Output, TypeIndex^.Name, ' ', Name)
  else if TypeIndex^.Cls = TtcBoolean then
         write(Codegen.Output, 'PBoolean ', Name)
  else if TypeIndex^.Cls = TtcInteger then
         write(Codegen.Output, 'int ', Name)
  else if TypeIndex^.Cls = TtcChar then
         write(Codegen.Output, 'char ', Name)
  else if TypeIndex^.Cls = TtcString then
         write(Codegen.Output, 'PString ', Name)
  else if TypeIndex^.Cls = TtcText then
         write(Codegen.Output, 'PFile ', Name)
  else if TypeIndex^.Cls = TtcEnum then
         OutNameAndEnum(Name, TypeIndex^.EnumIndex)
  else if TypeIndex^.Cls = TtcRecord then
         OutNameAndRecord(Name, TypeIndex^.RecordIndex)
  else if TypeIndex^.Cls = TtcArray then
  begin
    Arr := TypeIndex^.ArrayIndex^;
    OutNameAndType(Name, Arr.TypeIndex);
    write(Codegen.Output, '[1 + ', Arr.HighBound, ' - ', Arr.LowBound, ']')
  end
  else
    CompileError('Error writing name and type: ' + Name + ', ' +
                 TypeName(TypeIndex))
end;

procedure OutTypeDefinition;
var 
  Name : string;
begin
  Name := TypeIndex^.Name;
  if TypeIndex^.AliasFor = nil then
    CompileError('Type ' + Name + ' is not an alias');
  write(Codegen.Output, 'typedef ');
  OutNameAndType(Name, TypeIndex^.AliasFor);
  writeln(Codegen.Output, ';');
end;

procedure OutTypeDefinitionsFromCheckpoint;
var 
  Def : TPsDefPtr;
begin
  Def := Checkpoint^.Next;
  while Def <> nil do
  begin
    if Def^.Cls = TdcType then
    begin
      _ResolvePointerUnknown(Def^.TypeIndex);
      if Def^.TypeIndex^.AliasFor <> nil then
        OutTypeDefinition(Def^.TypeIndex)
    end;
    Def := Def^.Next
  end
end;

procedure OutConstantValue;
begin
  write(Codegen.Output, Expr.Value)
end;

procedure OutConstantArrayBegin;
begin
  write(Codegen.Output, '{ ')
end;

procedure OutConstantArraySeparator;
begin
  write(Codegen.Output, ', ')
end;

procedure OutConstantArrayEnd;
begin
  write(Codegen.Output, ' }')
end;

procedure OutVariableDeclaration;
begin
  OutNameAndType(OutVariableName(VarDef.Name, VarDef.IsReference),
  VarDef.TypeIndex)
end;

procedure OutVariableDefinition;
begin
  if VarIndex^.IsConstant then
    write(Codegen.Output, 'const ');
  OutVariableDeclaration(VarIndex^);
  writeln(Codegen.Output, ';');
end;

procedure OutConstantDefinitionBegin;
begin
  write(Codegen.Output, 'const ');
  OutVariableDeclaration(VarIndex^);
  write(Codegen.Output, ' = ');
end;

procedure OutConstantDefinitionEnd;
begin
  writeln(Codegen.Output, ';')
end;

procedure OutFunctionPrototype;
var 
  Pos : integer;
begin
  OutNameAndType(Def.Name, Def.ReturnTypeIndex);
  write(Codegen.Output, '(');
  for Pos := 1 to Def.ArgCount do
  begin
    OutVariableDeclaration(def.Args[Pos]);
    if Pos <> Def.ArgCount then
      write(Codegen.Output, ', ')
  end;
  write(Codegen.Output, ')')
end;

procedure OutFunctionDeclaration;
begin
  OutFunctionPrototype(FnIndex^);
  writeln(Codegen.Output, ';')
end;

procedure OutFunctionDefinition;
begin
  OutFunctionPrototype(FnIndex^);
  writeln(Codegen.Output, ' {');
  if FnIndex^.ReturnTypeIndex <> nil then
  begin
    OutNameAndType(OutReturnVariableName(FnIndex^.Name),
    FnIndex^.ReturnTypeIndex);
    writeln(Codegen.Output, ';')
  end
end;

procedure OutFunctionEnd;
begin
  if FnIndex^.ReturnTypeIndex <> nil then
    writeln(Codegen.Output, 'return ',
            OutReturnVariableName(FnIndex^.Name), ';');
  writeln(Codegen.Output, '}')
end;

procedure OutProgramHeading;
begin
  writeln(Codegen.Output, '/* Program: ', Name, ' */');
  writeln(Codegen.Output, '#include "pascual.h"')
end;

function GenFunctionCallStart;
begin
  GenFunctionCallStart := Fn + '('
end;

function GenFunctionCallEnd;
begin
  GenFunctionCallEnd := Fn + ')'
end;

function GenFunctionCallArgument;
begin
  if ArgNum <> 1 then Fn := Fn + ', ';
  if IsReference then Fn := Fn + '&' + Expr.Value
  else Fn := Fn + Expr.Value;
  GenFunctionCallArgument := Fn
end;

function ShortTypeName(TypeIndex : TPsTypeIndex) : char;
begin
  if IsBooleanType(TypeIndex) then ShortTypeName := 'b'
  else if IsIntegerType(TypeIndex) then ShortTypeName := 'i'
  else if IsCharType(TypeIndex) then ShortTypeName := 'c'
  else if IsStringType(TypeIndex) then ShortTypeName := 's'
  else if IsEnumType(TypeIndex) then ShortTypeName := 'e'
  else CompileError('No short type name exists for ' + TypeName(TypeIndex))
end;

procedure OutRead;
begin
  writeln(Codegen.Output, 'read_', ShortTypeName(OutVar.TypeIndex), '(&',
  Src.Value, ', &', OutVar.Value, ');')
end;

procedure OutReadln;
begin
  writeln(Codegen.Output, 'readln(&', Src.Value, ');')
end;

procedure OutWrite;
begin
  Expr := ExprEvaluate(Expr);
  if Expr.TypeIndex^.Cls = TtcEnum then
    writeln(Codegen.Output, 'write_e(&', Dst.Value, ', ', Expr.Value,
            ', enumvalues', Expr.TypeIndex^.EnumIndex^.Id, ');')
  else
    writeln(Codegen.Output, 'write_', ShortTypeName(Expr.TypeIndex),
    '(&', Dst.Value, ', ', Expr.Value, ');')
end;

procedure OutWriteln;
begin
  writeln(Codegen.Output, 'writeln(&', Dst.Value, ');')
end;

procedure OutStr;
begin
  Expr := ExprEvaluate(Expr);
  if Expr.TypeIndex^.Cls = TtcEnum then
    writeln(Codegen.Output, Dst, ' = to_str_e(', Expr.Value, ', enumvalues',
            Expr.TypeIndex^.EnumIndex^.Id, ');')
  else
    writeln(Codegen.Output, Dst, ' = to_str_', ShortTypeName(Expr.TypeIndex),
    '(', Expr.Value, ');')
end;

procedure OutNew;
begin
  write(Codegen.Output, Dst.Value, ' = malloc(sizeof(');
  OutTypeReference(Dst.TypeIndex^.PointedTypeIndex);
  writeln(Codegen.Output, '));')
end;

procedure OutDispose;
begin
  writeln(Codegen.Output, 'free(', Dst.Value, ');')
end;

procedure OutExpression;
begin
  write(Codegen.Output, Expr.Value)
end;

procedure OutAssign;
begin
  writeln(Codegen.Output, Lhs.Value, ' = ', Rhs.Value, ';')
end;

procedure OutAssignReturnValue;
begin
  writeln(Codegen.Output, 'return_', Lhs.FunctionIndex^.Name,
          ' = ', Rhs.Value, ';')
end;

procedure OutAssignToReference;
begin
  OutVariableDeclaration(VarIndex^);
  writeln(Codegen.Output, ' = &(', Rhs.Value, ');')
end;

procedure OutIf;
begin
  write(Codegen.Output, 'if (', Expr.Value, ') ')
end;

procedure OutElse;
begin
  write(Codegen.Output, ' else ')
end;

procedure OutCaseBegin;
begin
  writeln(Codegen.Output, 'switch (', CaseIndex.Value, ') {')
end;

procedure OutCaseStatementBegin;
begin
  write(CodeGen.Output, 'case ', CaseLabel.Value, ': ')
end;

procedure OutCaseStatementEnd;
begin
  writeln(CodeGen.Output, 'break;')
end;

procedure OutCaseElseBegin;
begin
  write(CodeGen.Output, 'default: ')
end;

procedure OutCaseElseEnd;
begin
  writeln(CodeGen.Output, 'break;')
end;

procedure OutCaseEnd;
begin
  writeln(CodeGen.Output, '}')
end;

procedure OutRepeatBegin;
begin
  writeln(Codegen.Output, 'do {')
end;

procedure OutRepeatEnd;
begin
  writeln(Codegen.Output, '} while (!(', Expr.Value, '));')
end;

procedure OutWhileBegin;
begin
  write(Codegen.Output, 'while (', Expr.Value, ') ')
end;

procedure OutWhileEnd;
begin
end;

procedure OutForBegin;
var 
  LimitType : TPsTypeIndex;
  First, Last : TPsVariable;
begin
  LimitType := Iter.TypeIndex;
  if IsEnumType(LimitType) then LimitType := PrimitiveTypes.PtInteger;
  First := MakeVariable('first', LimitType, false);
  Last := MakeVariable('last', LimitType, false);
  writeln(Codegen.Output, '{');
  OutVariableDeclaration(First);
  writeln(Codegen.Output, ' = ', FirstExpr.Value, ';');
  OutVariableDeclaration(Last);
  writeln(Codegen.Output, ' = ', LastExpr.Value, ';');
  write(Codegen.Output, 'if (first ');
  if Ascending then
    write(Codegen.Output, '<=')
  else
    write(Codegen.Output, '>=');
  writeln(Codegen.Output, ' last) {');
  writeln(Codegen.Output, Iter.Value, ' = first;');
  writeln(Codegen.Output, 'while (1) {');
end;

procedure OutForEnd;
begin
  writeln(Codegen.Output, 'if (', Iter.Value, ' == last) break;');
  if Ascending then
    writeln(Codegen.Output, '++', Iter.Value, ';')
  else
    write(Codegen.Output, '--', Iter.Value, ';');
  writeln(Codegen.Output, '}');
  writeln(Codegen.Output, '}');
  writeln(Codegen.Output, '}');
end;

procedure OutProcedureCall;
begin
  writeln(Codegen.Output, Expr.Value, ';')
end;

procedure OutEmptyStatement;
begin
  writeln(Codegen.Output, ';')
end;

procedure OutProgramBegin;
begin
  writeln(Codegen.Output, 'void pascual_main() {');
end;

procedure OutProgramEnd;
begin
  writeln(Codegen.Output, '}')
end;

procedure CodegenReset;
begin
  Codegen.Output := Output
end;

procedure CodegenSetOutput;
begin
  Assign(Codegen.Output, Filename);
  Rewrite(Codegen.Output)
end;

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
  write(Codegen.Output, 'const char* EnumValues', Pos, '[] = { ');
  for PosInEnum := 1 to Defs.Enums[Pos].Size do
  begin
    if PosInEnum <> 1 then write(Codegen.Output, ', ');
    write(Codegen.Output, '"', Defs.Enums[Pos].Values[PosInEnum], '"')
  end;
  writeln(Codegen.Output, ' };')
end;

procedure OutEnumValuesInScope;
var 
  Pos : TPsEnumIndex;
begin
  for Pos := Scope.NumEnums + 1 to Defs.Scope.NumEnums do
    OutEnumValues(Pos)
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
var 
  Typ : TPsType;
begin
  if TypeIndex <> 0 then Typ := Defs.Types[TypeIndex];
  if TypeIndex = 0 then write(Codegen.Output, 'void')
  else if Typ.Cls = TtcPointer then
  begin
    OutTypeReference(Typ.PointedTypeIndex);
    write(Codegen.Output, '*')
  end
  else if Typ.Cls = TtcBoolean then write(Codegen.Output, 'PBoolean')
  else if Typ.Cls = TtcInteger then write(Codegen.Output, 'int')
  else if Typ.Cls = TtcChar then write(Codegen.Output, 'char')
  else if Typ.Cls = TtcString then write(Codegen.Output, 'PString')
  else if Typ.Cls = TtcText then write(Codegen.Output, 'PFile')
  else if Typ.Cls = TtcEnum then write(Codegen.Output, 'enum enum', Typ.
                                       EnumIndex)
  else if Typ.Cls = TtcRecord then
         write(Codegen.Output, 'struct record', Typ.RecordIndex)
  else if Typ.Cls = TtcArray then
  begin
    OutTypeReference(Defs.Arrays[Typ.ArrayIndex].TypeIndex);
    write(Codegen.Output, '*')
  end
  else
    CompileError('Error writing type reference: ' + TypeName(TypeIndex))
end;

procedure OutNameAndRecord(Name : string; RecordIndex : TPsRecordIndex);
var 
  Rec : TPsRecordDef;
  Pos : integer;
  NumVariant : integer;
begin
  Rec := Defs.Records[RecordIndex];
  NumVariant := 0;
  write(Codegen.Output, 'struct record', RecordIndex, ' { ');
  for Pos := 1 to Rec.Size do
  begin
    if (Rec.NumVariants > NumVariant)
       and (Rec.VariantBounds[NumVariant + 1] = Pos) then
    begin
      NumVariant := NumVariant + 1;
      if NumVariant = 1 then
        write(Codegen.Output, 'union { ')
      else
        write(Codegen.Output, '}; ');
      write(Codegen.Output, 'struct { ')
    end;
    OutNameAndType(Rec.Fields[Pos].Name, Rec.Fields[Pos].TypeIndex);
    write(Codegen.Output, '; ')
  end;
  if NumVariant > 0 then
    write(Codegen.Output, '}; }; ');
  write(Codegen.Output, '} ', Name)
end;

procedure OutNameAndType;
var 
  Typ : TPsType;
  Enum : TPsEnumDef;
  Arr : TPsArrayDef;
  Pos : integer;
begin
  if TypeIndex <> 0 then
    Typ := Defs.Types[TypeIndex];
  if TypeIndex = 0 then write(Codegen.Output, 'void ', Name)
  else if Typ.Cls = TtcPointer then
  begin
    OutTypeReference(Typ.PointedTypeIndex);
    write(Codegen.Output, ' *', Name)
  end
  else if (Typ.AliasFor <> 0) and (Typ.Name <> '') then
         write(Codegen.Output, Typ.Name, ' ', Name)
  else if Typ.Cls = TtcBoolean then write(Codegen.Output, 'PBoolean ', Name)
  else if Typ.Cls = TtcInteger then write(Codegen.Output, 'int ', Name)
  else if Typ.Cls = TtcChar then write(Codegen.Output, 'char ', Name)
  else if Typ.Cls = TtcString then write(Codegen.Output, 'PString ', Name)
  else if Typ.Cls = TtcText then write(Codegen.Output, 'PFile ', Name)
  else if Typ.Cls = TtcEnum then
  begin
    Enum := Defs.Enums[Typ.EnumIndex];
    write(Codegen.Output, 'enum enum', Typ.EnumIndex, ' { ');
    for Pos := 1 to Enum.Size do
    begin
      if Pos > 1 then
        write(Codegen.Output, ', ');
      write(Codegen.Output, Enum.Values[Pos])
    end;
    write(Codegen.Output, '} ', Name)
  end
  else if Typ.Cls = TtcRecord then OutNameAndRecord(Name, Typ.RecordIndex)
  else if Typ.Cls = TtcArray then
  begin
    Arr := Defs.Arrays[Typ.ArrayIndex];
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
  Name := Defs.Types[TypeIndex].Name;
  if Defs.Types[TypeIndex].AliasFor = 0 then
    CompileError('Type ' + Name + ' is not an alias');
  write(Codegen.Output, 'typedef ');
  OutNameAndType(Name, Defs.Types[TypeIndex].AliasFor);
  writeln(Codegen.Output, ';');
end;

procedure OutConstantValue;
begin
  write(Codegen.Output, Value)
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
  if Defs.Variables[VarIndex].IsConstant then
    write(Codegen.Output, 'const ');
  OutVariableDeclaration(Defs.Variables[VarIndex]);
  writeln(Codegen.Output, ';');
end;

procedure OutConstantDefinitionBegin;
begin
  write(Codegen.Output, 'const ');
  OutVariableDeclaration(Defs.Variables[VarIndex]);
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
  OutFunctionPrototype(Defs.Functions[FnIndex]);
  writeln(Codegen.Output, ';')
end;

procedure OutFunctionDefinition;
var 
  Fun : TPsFunction;
begin
  Fun := Defs.Functions[FnIndex];
  OutFunctionPrototype(Fun);
  writeln(Codegen.Output, ' {');
  if Fun.ReturnTypeIndex <> 0 then
  begin
    OutNameAndType(OutReturnVariableName(Fun.Name), Fun.ReturnTypeIndex);
    writeln(Codegen.Output, ';')
  end
end;

procedure OutFunctionEnd;
begin
  if Defs.Functions[FnIndex].ReturnTypeIndex <> 0 then
    writeln(Codegen.Output, 'return ',
            OutReturnVariableName(Defs.Functions[FnIndex].Name), ';');
  writeln(Codegen.Output, '}')
end;

procedure OutProgramHeading;
begin
  writeln(Codegen.Output, '/* Program: ', Name, ' */');
  writeln(Codegen.Output, '#include "pascual.h"')
end;

procedure SetStringIndex;
begin
  Str.Value := Str.Value + '.chr[' + Idx.Value + ']';
end;

procedure SetArrayIndex;
begin
  Arr.Value := Arr.Value + '[(' + Idx.Value + ') - '
               + Defs.Arrays[Defs.Types[Arr.TypeIndex].ArrayIndex].LowBound
               + ']'
end;

procedure SetFieldAccess;
begin
  if Rec.Value[1] = '*' then Rec.Value := '(' + Rec.Value + ')';
  Rec.Value := Rec.Value + '.' + Fld
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

function GenParens;
begin
  Expr.Value := '(' + Expr.Value + ')';
  GenParens := Expr
end;

procedure OutRead;
begin
  writeln(Codegen.Output, 'read_', ShortTypeName(OutVar.TypeIndex), '(&',
  Src, ', &', OutVar.Value, ');')
end;

procedure OutReadln;
begin
  writeln(Codegen.Output, 'readln(&', Src, ');')
end;

procedure OutWrite;
begin
  Expr := Evaluate(Expr);
  if Defs.Types[Expr.TypeIndex].Cls = TtcEnum then
    writeln(Codegen.Output, 'write_e(&', Dst, ', ', Expr.Value, ', EnumValues',
            Defs.Types[Expr.TypeIndex].EnumIndex, ');')
  else
    writeln(Codegen.Output, 'write_', ShortTypeName(Expr.TypeIndex),
    '(&', Dst, ', ', Expr.Value, ');')
end;

procedure OutWriteln;
begin
  writeln(Codegen.Output, 'writeln(&', Dst, ');')
end;

procedure OutStr;
begin
  Expr := Evaluate(Expr);
  if Defs.Types[Expr.TypeIndex].Cls = TtcEnum then
    writeln(Codegen.Output, Dst, ' = to_str_e(', Expr.Value, ', EnumValues',
            Defs.Types[Expr.TypeIndex].EnumIndex, ');')
  else
    writeln(Codegen.Output, Dst, ' = to_str_', ShortTypeName(Expr.TypeIndex),
    '(', Expr.Value, ');')
end;

procedure OutNew;
begin
  write(Codegen.Output, Dst.Value, ' = malloc(sizeof(');
  OutTypeReference(Defs.Types[Dst.TypeIndex].PointedTypeIndex);
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
  writeln(Codegen.Output, 'return_', Defs.Functions[Lhs.FunctionIndex].Name,
          ' = ', Rhs.Value, ';')
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

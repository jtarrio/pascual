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

procedure OutEnumValues(Pos : TPsEnumIndex);
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

procedure OutEnumValuesInScope(Scope : TPsScope);
var 
  Pos : TPsEnumIndex;
begin
  for Pos := Scope.NumEnums + 1 to Defs.Scope.NumEnums do
    OutEnumValues(Pos)
end;

function OutVariableName(Name : string; IsReference : boolean) : string;
begin
  if IsReference then OutVariableName := '*' + Name
  else OutVariableName := Name
end;

function OutReturnVariableName(Name : string) : string;
begin
  OutReturnVariableName := 'return_' + Name
end;

procedure OutNameAndType(Name : string; TypeIndex : TPsTypeIndex);
var 
  Typ : TPsType;
  Enum : TPsEnumDef;
  Rec : TPsRecordDef;
  Arr : TPsArrayDef;
  Pos : integer;
begin
  if TypeIndex <> 0 then
    Typ := Defs.Types[TypeIndex];
  if TypeIndex = 0 then write(Codegen.Output, 'void ', Name)
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
    write(Codegen.Output, 'enum { ');
    for Pos := 1 to Enum.Size do
    begin
      if Pos > 1 then
        write(Codegen.Output, ', ');
      write(Codegen.Output, Enum.Values[Pos])
    end;
    write(Codegen.Output, '} ', Name)
  end
  else if Typ.Cls = TtcRecord then
  begin
    Rec := Defs.Records[Typ.RecordIndex];
    write(Codegen.Output, 'struct { ');
    for Pos := 1 to Rec.Size do
    begin
      OutNameAndType(Rec.Fields[Pos].Name, Rec.Fields[Pos].TypeIndex);
      write(Codegen.Output, '; ')
    end;
    write(Codegen.Output, '} ', Name)
  end
  else if Typ.Cls = TtcArray then
  begin
    Arr := Defs.Arrays[Typ.ArrayIndex];
    OutNameAndType(Name, Arr.TypeIndex);
    write(Codegen.Output, '[1 + ', Arr.HighBound, ' - ', Arr.LowBound, ']')
  end
  else if Typ.Cls = TtcPointer then
         OutNameAndType('*' + Name, Typ.PointedTypeIndex)
  else
  begin
    writeln(StdErr, 'Error writing name and type: ', Name, ', ',
            TypeName(TypeIndex), LxWhereStr);
    halt(1)
  end
end;

procedure OutTypeDefinition(TypeIndex : TPsTypeIndex);
var 
  Name : string;
begin
  Name := Defs.Types[TypeIndex].Name;
  if Defs.Types[TypeIndex].AliasFor = 0 then
  begin
    writeln(StdErr, 'Type ', Name, ' is not an alias', LxWhereStr);
    halt(1)
  end;
  write(Codegen.Output, 'typedef ');
  OutNameAndType(Name, Defs.Types[TypeIndex].AliasFor);
  writeln(Codegen.Output, ';');
end;

procedure OutConstantValue(Value : string);
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

procedure OutVariableDeclaration(VarDef : TPsVariable);
begin
  OutNameAndType(OutVariableName(VarDef.Name, VarDef.IsReference),
  VarDef.TypeIndex)
end;

procedure OutVariableDefinition(VarIndex : TPsVariableIndex);
begin
  if Defs.Variables[VarIndex].IsConstant then
    write(Codegen.Output, 'const ');
  OutVariableDeclaration(Defs.Variables[VarIndex]);
  writeln(Codegen.Output, ';');
end;

procedure OutConstantDefinitionBegin(VarIndex : TPsVariableIndex);
begin
  write(Codegen.Output, 'const ');
  OutVariableDeclaration(Defs.Variables[VarIndex]);
  write(Codegen.Output, ' = ');
end;

procedure OutConstantDefinitionEnd;
begin
  writeln(Codegen.Output, ';')
end;

procedure OutFunctionPrototype(Def : TPsFunction);
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

procedure OutFunctionDeclaration(FnIndex : TPsFunctionIndex);
begin
  OutFunctionPrototype(Defs.Functions[FnIndex]);
  writeln(Codegen.Output, ';')
end;

procedure OutFunctionDefinition(FnIndex : TPsFunctionIndex);
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

procedure OutFunctionEnd(FnIndex : TPsFunctionIndex);
begin
  if Defs.Functions[FnIndex].ReturnTypeIndex <> 0 then
    writeln(Codegen.Output, 'return ',
            OutReturnVariableName(Defs.Functions[FnIndex].Name), ';');
  writeln(Codegen.Output, '}')
end;

procedure OutProgramHeading(Name : string);
begin
  writeln(Codegen.Output, '/* Program: ', Name, ' */');
  writeln(Codegen.Output, '#include "pascual.h"')
end;

procedure SetStringIndex(var Str : TPsExpression; Idx : TPsExpression);
begin
  Str.Value := Str.Value + '.chr[' + Idx.Value + ']';
end;

procedure SetArrayIndex(var Arr : TPsExpression; Idx : TPsExpression);
begin
  Arr.Value := Arr.Value + '[(' + Idx.Value + ') - '
               + Defs.Arrays[Defs.Types[Arr.TypeIndex].ArrayIndex].LowBound
               + ']'
end;

procedure SetFieldAccess(var Rec : TPsExpression; Fld : string);
begin
  if Rec.Value[1] = '*' then Rec.Value := '(' + Rec.Value + ')';
  Rec.Value := Rec.Value + '.' + Fld
end;

function GenFunctionCallStart(Fn : string) : string;
begin
  GenFunctionCallStart := Fn + '('
end;

function GenFunctionCallEnd(Fn : string) : string;
begin
  GenFunctionCallEnd := Fn + ')'
end;

function GenFunctionCallArgument(Fn : string; Expr : TPsExpression;
                                 IsReference : boolean;
                                 ArgNum : integer) : string;
begin
  if ArgNum <> 1 then Fn := Fn + ', ';
  if IsReference then Fn := Fn + '&' + Expr.Value
  else Fn := Fn + Expr.Value;
  GenFunctionCallArgument := Fn
end;

function GenParens(Expr : TPsExpression) : TPsExpression;
begin
  Expr.Value := '(' + Expr.Value + ')';
  GenParens := Expr
end;

procedure OutRead(Src : string; OutVar : TPsExpression);
begin
  writeln(Codegen.Output, 'read_', ShortTypeName(OutVar.TypeIndex), '(&',
  Src, ', &', OutVar.Value, ');')
end;

procedure OutReadln(Src : string);
begin
  writeln(Codegen.Output, 'readln(&', Src, ');')
end;

procedure OutWrite(Dst : string; Expr : TPsExpression);
begin
  Expr := Evaluate(Expr);
  if Defs.Types[Expr.TypeIndex].Cls = TtcEnum then
    writeln(Codegen.Output, 'write_e(&', Dst, ', ', Expr.Value, ', EnumValues',
            Defs.Types[Expr.TypeIndex].EnumIndex, ');')
  else
    writeln(Codegen.Output, 'write_', ShortTypeName(Expr.TypeIndex),
    '(&', Dst, ', ', Expr.Value, ');')
end;

procedure OutWriteln(Dst : string);
begin
  writeln(Codegen.Output, 'writeln(&', Dst, ');')
end;

procedure OutStr(Dst : string; Expr : TPsExpression);
begin
  Expr := Evaluate(Expr);
  if Defs.Types[Expr.TypeIndex].Cls = TtcEnum then
    writeln(Codegen.Output, Dst, ' = to_str_e(', Expr.Value, ', EnumValues',
            Defs.Types[Expr.TypeIndex].EnumIndex, ');')
  else
    writeln(Codegen.Output, Dst, ' = to_str_', ShortTypeName(Expr.TypeIndex),
    '(', Expr.Value, ');')
end;

procedure OutNew(Dst : TPsExpression);
begin
  write(Codegen.Output, Dst.Value, ' = malloc(sizeof(');
  OutNameAndType('', Defs.Types[Dst.TypeIndex].PointedTypeIndex);
  writeln(Codegen.Output, '));')
end;

procedure OutDispose(Dst : TPsExpression);
begin
  writeln(Codegen.Output, 'free(', Dst.Value, ');')
end;

procedure OutExpression(Expr : TPsExpression);
begin
  write(Codegen.Output, Expr.Value)
end;

procedure OutAssign(Lhs : TPsExpression; Rhs : TPsExpression);
begin
  writeln(Codegen.Output, Lhs.Value, ' = ', Rhs.Value, ';')
end;

procedure OutAssignReturnValue(Lhs : TPsExpression; Rhs : TPsExpression);
begin
  writeln(Codegen.Output, 'return_', Defs.Functions[Lhs.FunctionIndex].Name,
          ' = ', Rhs.Value, ';')
end;

procedure OutIf(Expr : TPsExpression);
begin
  write(Codegen.Output, 'if (', Expr.Value, ') ')
end;

procedure OutElse;
begin
  write(Codegen.Output, ' else ')
end;

procedure OutRepeatBegin;
begin
  writeln(Codegen.Output, 'do {')
end;

procedure OutRepeatEnd(Expr : TPsExpression);
begin
  if not IsBooleanType(Expr.TypeIndex) then
  begin
    writeln(StdErr, 'Expected boolean expression, got ',
            TypeName(Expr.TypeIndex), LxWhereStr);
    halt(1)
  end;
  writeln(Codegen.Output, '} while (!(', Expr.Value, '));')
end;

procedure OutWhileBegin(Expr : TPsExpression);
begin
  if not IsBooleanType(Expr.TypeIndex) then
  begin
    writeln(StdErr, 'Expected boolean expression, got ',
            TypeName(Expr.TypeIndex), LxWhereStr);
    halt(1)
  end;
  write(Codegen.Output, 'while (', Expr.Value, ') ')
end;

procedure OutWhileEnd;
begin
end;

procedure OutForBegin(Iter : TPsExpression; FirstExpr : TPsExpression;
                      LastExpr : TPsExpression; Ascending : boolean);
var 
  LimitType : TPsTypeIndex;
  First : TPsVariable;
  Last : TPsVariable;
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

procedure OutForEnd(Iter : TPsExpression; Ascending : boolean);
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

procedure OutProcedureCall(Expr : TPsExpression);
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

procedure CodegenSetOutput(Filename : string);
begin
  Assign(Codegen.Output, Filename);
  Rewrite(Codegen.Output)
end;

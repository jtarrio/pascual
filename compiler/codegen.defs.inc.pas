procedure CodegenReset;
forward;
procedure CodegenSetOutput(Filename : string);
forward;
procedure OutBegin;
forward;
procedure OutEnd;
forward;
procedure OutEnumValues(Pos : TPsEnumIndex);
forward;
procedure OutEnumValuesInScope(Scope : TPsScope);
forward;
function OutVariableName(Name : string; IsReference : boolean) : string;
forward;
function OutReturnVariableName(Name : string) : string;
forward;
procedure OutNameAndType(Name : string; TypeIndex : TPsTypeIndex);
forward;
procedure OutTypeDefinition(TypeIndex : TPsTypeIndex);
forward;
procedure OutConstantValue(Value : string);
forward;
procedure OutConstantArrayBegin;
forward;
procedure OutConstantArraySeparator;
forward;
procedure OutConstantArrayEnd;
forward;
procedure OutVariableDeclaration(VarDef : TPsVariable);
forward;
procedure OutVariableDefinition(VarIndex : TPsVariableIndex);
forward;
procedure OutConstantDefinitionBegin(VarIndex : TPsVariableIndex);
forward;
procedure OutConstantDefinitionEnd;
forward;
procedure OutFunctionPrototype(Def : TPsFunction);
forward;
procedure OutFunctionDeclaration(FnIndex : TPsFunctionIndex);
forward;
procedure OutFunctionDefinition(FnIndex : TPsFunctionIndex);
forward;
procedure OutFunctionEnd(FnIndex : TPsFunctionIndex);
forward;
procedure OutProgramHeading(Name : string);
forward;
procedure SetStringIndex(var Str : TPsExpression; Idx : TPsExpression);
forward;
procedure SetArrayIndex(var Arr : TPsExpression; Idx : TPsExpression);
forward;
procedure SetFieldAccess(var Rec : TPsExpression; Fld : string);
forward;
function GenFunctionCallStart(Fn : string) : string;
forward;
function GenFunctionCallEnd(Fn : string) : string;
forward;
function GenFunctionCallArgument(Fn : string; Expr : TPsExpression;
                                 IsReference : boolean;
                                 ArgNum : integer) : string;
forward;
function GenParens(Expr : TPsExpression) : TPsExpression;
forward;
procedure OutRead(Src : string; OutVar : TPsExpression);
forward;
procedure OutReadln(Src : string);
forward;
procedure OutWrite(Dst : string; Expr : TPsExpression);
forward;
procedure OutWriteln(Dst : string);
forward;
procedure OutStr(Dst : string; Expr : TPsExpression);
forward;
procedure OutNew(Dst : TPsExpression);
forward;
procedure OutDispose(Dst : TPsExpression);
forward;
procedure OutExpression(Expr : TPsExpression);
forward;
procedure OutAssign(Lhs, Rhs : TPsExpression);
forward;
procedure OutAssignReturnValue(Lhs, Rhs : TPsExpression);
forward;
procedure OutIf(Expr : TPsExpression);
forward;
procedure OutElse;
forward;
procedure OutRepeatBegin;
forward;
procedure OutRepeatEnd(Expr : TPsExpression);
forward;
procedure OutWhileBegin(Expr : TPsExpression);
forward;
procedure OutWhileEnd;
forward;
procedure OutForBegin(Iter, FirstExpr, LastExpr : TPsExpression;
                      Ascending : boolean);
forward;
procedure OutForEnd(Iter : TPsExpression; Ascending : boolean);
forward;
procedure OutProcedureCall(Expr : TPsExpression);
forward;
procedure OutEmptyStatement;
forward;
procedure OutProgramBegin;
forward;
procedure OutProgramEnd;
forward;

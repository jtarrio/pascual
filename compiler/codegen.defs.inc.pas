procedure CodegenReset;
forward;
procedure CodegenSetOutput(Filename : string);
forward;
procedure OutBegin;
forward;
procedure OutEnd;
forward;
procedure OutEnumValues(EnumIndex : TPsEnumIndex);
forward;
procedure OutEnumValuesFromCheckpoint(Checkpoint : TPsDefPtr);
forward;
function OutVariableName(Name : string; IsReference : boolean) : string;
forward;
function OutReturnVariableName(Name : string) : string;
forward;
procedure OutTypeReference(TypeIndex : TPsTypeIndex);
forward;
procedure OutNameAndType(Name : string; TypeIndex : TPsTypeIndex);
forward;
procedure OutTypeDefinition(TypeIndex : TPsTypeIndex);
forward;
procedure OutTypeDefinitionsFromCheckpoint(Checkpoint : TPsDefPtr);
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
procedure OutExpression(Expr : TExpression);
forward;
procedure OutAssign(Lhs, Rhs : TExpression);
forward;
procedure OutAssignReturnValue(Lhs, Rhs : TExpression);
forward;
procedure OutAssignToReference(VarIndex : TPsVariableIndex;
                               Rhs : TExpression);
forward;
procedure OutIf(Expr : TExpression);
forward;
procedure OutElse;
forward;
procedure OutIfEnd;
forward;
procedure OutSequenceBegin;
forward;
procedure OutSequenceEnd;
forward;
procedure OutCaseBegin(CaseIndex : TExpression);
forward;
procedure OutCaseStatementBegin(CaseLabel : TExpression);
forward;
procedure OutCaseStatementEnd;
forward;
procedure OutCaseElseBegin;
forward;
procedure OutCaseElseEnd;
forward;
procedure OutCaseEnd;
forward;
procedure OutRepeatBegin;
forward;
procedure OutRepeatEnd(Expr : TExpression);
forward;
procedure OutWhileBegin(Expr : TExpression);
forward;
procedure OutWhileEnd;
forward;
procedure OutForBegin(Iter, FirstExpr, LastExpr : TExpression;
                      Ascending : boolean);
forward;
procedure OutForEnd(Iter : TExpression; Ascending : boolean);
forward;
procedure OutProcedureCall(Expr : TExpression);
forward;
procedure OutSpecialProcedureCall(Expr : TExpression);
forward;
procedure OutEmptyStatement;
forward;
procedure OutProgramBegin;
forward;
procedure OutProgramEnd;
forward;

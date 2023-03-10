procedure CodegenReset;
forward;
procedure CodegenSetOutput(Filename : string);
forward;
procedure OutEnumValuesFromCheckpoint(Checkpoint : TPsDefPtr);
forward;
procedure OutTypeDefinitionsFromCheckpoint(Checkpoint : TPsDefPtr);
forward;
procedure OutConstantArrayBegin;
forward;
procedure OutConstantArraySeparator;
forward;
procedure OutConstantArrayEnd;
forward;
procedure OutVariableDefinition(VarPtr : TPsVarPtr);
forward;
procedure OutConstantDefinitionBegin(VarPtr : TPsVarPtr);
forward;
procedure OutConstantDefinitionEnd;
forward;
procedure OutFunctionDeclaration(FnPtr : TPsFnPtr);
forward;
procedure OutFunctionDefinition(FnPtr : TPsFnPtr);
forward;
procedure OutFunctionEnd(FnPtr : TPsFnPtr);
forward;
procedure OutExpression(Expr : TExpression);
forward;
procedure OutAssign(Lhs, Rhs : TExpression);
forward;
procedure OutDeclareAndAssign(VarPtr : TPsVarPtr; Rhs : TExpression);
forward;
procedure OutSequenceBegin;
forward;
procedure OutSequenceEnd;
forward;
procedure OutIf(Expr : TExpression);
forward;
procedure OutElse;
forward;
procedure OutIfEnd;
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
procedure OutPseudoProcCall(Expr : TExpression);
forward;
procedure OutEmptyStatement;
forward;
procedure OutProgramHeading(Name : string);
forward;
procedure OutProgramBegin;
forward;
procedure OutProgramEnd;
forward;

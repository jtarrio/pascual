procedure CodegenReset;
forward;
procedure CodegenSetOutput(Filename : string);
forward;
procedure OutEnumValuesFromCheckpoint(Checkpoint : TSDefinition);
forward;
procedure OutTypeDefinitionsFromCheckpoint(Checkpoint : TSDefinition);
forward;
procedure OutConstantArrayBegin;
forward;
procedure OutConstantArraySeparator;
forward;
procedure OutConstantArrayEnd;
forward;
procedure OutConstantRecordBegin;
forward;
procedure OutConstantRecordField(const Name : string);
forward;
procedure OutConstantRecordSeparator;
forward;
procedure OutConstantRecordEnd;
forward;
procedure OutVariableDefinition(VarPtr : TPsVarPtr; Location : TExpression);
forward;
procedure OutConstantDefinitionBegin(VarPtr : TPsVarPtr);
forward;
procedure OutConstantDefinitionEnd;
forward;
procedure OutFunctionDeclaration(SrPtr : TPsSubrPtr);
forward;
procedure OutFunctionDefinition(SrPtr : TPsSubrPtr);
forward;
procedure OutFunctionEnd(SrPtr : TPsSubrPtr);
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
procedure OutForEnd;
forward;
procedure OutExpressionStatement(Expr : TExpression);
forward;
procedure OutEmptyStatement;
forward;
procedure OutProgramHeading(Name : string);
forward;
procedure OutProgramBegin;
forward;
procedure OutProgramEnd;
forward;

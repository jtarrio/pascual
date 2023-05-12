function StEmpty : TSStatement;
begin
  new(Result);
  Result^.Cls := SscEmpty
end;

function StSequence : TSStatement;
begin
  new(Result);
  Result^.Cls := SscSequence;
  Result^.Sequence := nil
end;

function StSequenceEntry(Stmt : TSStatement) : TSSSequence;
begin
  new(Result);
  Result^.Next := nil;
  Result^.Statement := Stmt
end;

function StAssign(Lhs, Rhs : TSExpression) : TSStatement;
begin
  new(Result);
  Result^.Cls := SscAssign;
  Result^.Lhs := Lhs;
  Result^.Rhs := Rhs
end;

function StProcCall(ProcCall : TSExpression) : TSStatement;
begin
  new(Result);
  Result^.Cls := SscProcCall;
  Result^.ProcCall := ProcCall
end;

function StIf(Cond : TSExpression; IfThen, IfElse : TSStatement) : TSStatement;
begin
  if IfThen = nil then IfThen := StEmpty;
  if IfElse = nil then IfElse := StEmpty;
  new(Result);
  Result^.Cls := SscIf;
  Result^.IfCond := Cond;
  Result^.IfThen := IfThen;
  Result^.IfElse := IfElse
end;

function StRepeat(Cond : TSExpression; Sequence : TSSSequence) : TSStatement;
begin
  new(Result);
  Result^.Cls := SscRepeat;
  Result^.UntilCond := Cond;
  Result^.RepeatSequence := Sequence
end;

function StWhile(Cond : TSExpression; Stmt : TSStatement) : TSStatement;
begin
  new(Result);
  Result^.Cls := SscWhile;
  Result^.WhileCond := Cond;
  Result^.WhileStatement := Stmt
end;

function StFor(Iterator, First, Last : TSExpression; Ascending : boolean;
               Stmt : TSStatement) : TSStatement;
begin
  new(Result);
  Result^.Cls := SscFor;
  Result^.Iterator := Iterator;
  Result^.First := First;
  Result^.Last := Last;
  Result^.Ascending := Ascending;
  Result^.ForStatement := Stmt
end;

function StWith(WithVar: TSDVariable;
                Value : TSExpression;
                Stmt : TSStatement) : TSStatement;
begin
  new(Result);
  Result^.Cls := SscWith;
  Result^.WithVar := WithVar;
  Result^.WithValue := Value;
  Result^.WithStatement := Stmt
end;

function StCase(Selector : TSExpression; Cases : TSSCase) : TSStatement;
begin
  new(Result);
  Result^.Cls := SscCase;
  Result^.CaseSelector := Selector;
  Result^.CaseEntry := Cases
end;

function StCaseEntry(CaseLabel : TSExpression; Stmt : TSStatement) : TSSCase;
begin
  new(Result);
  Result^.CaseLabel := CaseLabel;
  Result^.Statement := Stmt
end;

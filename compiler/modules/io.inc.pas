

function _ModIoRead_Parse(FnExpr : TExpression) : TExpression;
var 
  First : boolean;
  OutVar : TExpression;
  ReadArg : ^TExReadArgs;
begin
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := ExVariable(FindNameOfClass('INPUT',
                               TncVariable, {Required=}true)^.VarPtr);
  ReadArg := nil;
  if Lexer.Token.Id = TkLparen then
  begin
    First := true;
    WantTokenAndRead(TkLparen);
    while Lexer.Token.Id <> TkRparen do
    begin
      OutVar := PsExpression;
      if First and IsTextType(OutVar^.TypePtr) then
      begin
        EnsureAddressableExpr(OutVar);
        ExDispose(Result^.PseudoFnCall.Arg1);
        Result^.PseudoFnCall.Arg1 := OutVar
      end
      else
      begin
        EnsureAssignableExpr(OutVar);
        if ReadArg = nil then
        begin
          new(Result^.PseudoFnCall.ReadArgs);
          ReadArg := Result^.PseudoFnCall.ReadArgs
        end
        else
        begin
          new(ReadArg^.Next);
          ReadArg := ReadArg^.Next;
        end;
        ReadArg^.Next := nil;
        ReadArg^.Arg := OutVar;
        ExMarkInitialized(OutVar)
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      First := false
    end;
    WantTokenAndRead(TkRparen)
  end
end;

function _ModIoWrite_EvaluateZeroArg(Expr : TExpression) : TExpression;
var Args : TExFunctionArgs;
begin
  Args.Size := 0;
  if IsFunctionType(Expr^.TypePtr)
     and (Expr^.TypePtr^.FnDefPtr^.ReturnTypePtr <> nil)
     and (Expr^.TypePtr^.FnDefPtr^.Args.Count = 0) then
    Result := ExFunctionCall(Expr, Args)
  else Result := Expr
end;

function _ModIoWrite_Parse(FnExpr : TExpression) : TExpression;
var 
  First : boolean;
  OutExpr : TExpression;
  WriteArg : ^TExWriteArgs;
begin
  Result := ExPseudoFnCall(FnExpr);
  Result^.PseudoFnCall.Arg1 := ExVariable(FindNameOfClass('OUTPUT',
                               TncVariable, {Required=}true)^.VarPtr);
  WriteArg := nil;
  if Lexer.Token.Id = TkLparen then
  begin
    First := true;
    WantTokenAndRead(TkLparen);
    while Lexer.Token.Id <> TkRparen do
    begin
      OutExpr := _ModIoWrite_EvaluateZeroArg(PsExpression);
      if First and IsTextType(OutExpr^.TypePtr) then
      begin
        EnsureAddressableExpr(OutExpr);
        ExDispose(Result^.PseudoFnCall.Arg1);
        Result^.PseudoFnCall.Arg1 := OutExpr
      end
      else
      begin
        if WriteArg = nil then
        begin
          new(Result^.PseudoFnCall.WriteArgs);
          WriteArg := Result^.PseudoFnCall.WriteArgs
        end
        else
        begin
          new(WriteArg^.Next);
          WriteArg := WriteArg^.Next;
        end;
        WriteArg^.Arg := OutExpr;
        WriteArg^.Width := nil;
        WriteArg^.Prec := nil;
        WriteArg^.Next := nil;
        if Lexer.Token.Id = TkColon then
        begin
          WantTokenAndRead(TkColon);
          WriteArg^.Width := PsExpression;
          EnsureIntegerExpr(WriteArg^.Width);
          if IsRealType(WriteArg^.Arg^.TypePtr)
             and (Lexer.Token.Id = TkColon) then
          begin
            WantTokenAndRead(TkColon);
            WriteArg^.Prec := PsExpression;
            EnsureIntegerExpr(WriteArg^.Prec);
          end
        end
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      First := false
    end;
    WantTokenAndRead(TkRparen)
  end
end;

procedure _UpFirst(var Str : string);
var Pos : integer;
begin
  for Pos := 1 to Length(Str) do
    if Pos = 1 then Str[Pos] := Upcase(Str[Pos])
    else Str[Pos] := Lowercase(Str[Pos]);
end;

function _ModIo_FileFun_Parse(FnExpr : TExpression) : TExpression;
var 
  Args : TExFunctionArgs;
  FnName : string;
  FnPtr : TPsFnPtr;
begin
  FnName := FnExpr^.PseudoFnPtr^.Name;
  _UpFirst(FnName);
  ExDispose(FnExpr);
  FnPtr := FindNameOfClass(FnName, TncFunction, {Required=}true)^.FnPtr;
  Args := PsFunctionArgs;
  Args.Size := Args.Size + 1;
  Args.Values[Args.Size] := ExBooleanConstant(Options.CheckIoResult);
  Result := ExFunctionCall(ExFnRef(FnPtr), Args)
end;

function _ModIo_FileResetFun_Parse(FnExpr : TExpression) : TExpression;
var 
  Args : TExFunctionArgs;
  FnName : string;
  FnPtr : TPsFnPtr;
begin
  FnName := FnExpr^.PseudoFnPtr^.Name;
  _UpFirst(FnName);
  ExDispose(FnExpr);
  FnPtr := FindNameOfClass(FnName, TncFunction, {Required=}true)^.FnPtr;
  Args := PsFunctionArgs;
  Args.Size := Args.Size + 2;
  Args.Values[Args.Size - 1] := ExIntegerConstant(0);
  Args.Values[Args.Size] := ExBooleanConstant(Options.CheckIoResult);
  Result := ExFunctionCall(ExFnRef(FnPtr), Args)
end;

procedure _AddIoProc1(Name : string; Arg1 : TPsVariable);
begin
  AddPseudoFn(Name, @_ModIo_FileFun_Parse, @Pf_Indef_Describe);
  _UpFirst(Name);
  AddFunction(MakeProcedure2(Name, Arg1,
              MakeArg('DIE_ON_ERROR', PrimitiveTypes.PtBoolean)))
end;

procedure _AddIoProc2(Name : string; Arg1, Arg2 : TPsVariable);
begin
  AddPseudoFn(Name, @_ModIo_FileFun_Parse, @Pf_Indef_Describe);
  _UpFirst(Name);
  AddFunction(MakeProcedure3(Name, Arg1, Arg2,
              MakeArg('DIE_ON_ERROR', PrimitiveTypes.PtBoolean)))
end;

procedure _AddIoFun1(Name : string; RetType : TPsTypePtr; Arg1 : TPsVariable);
begin
  AddPseudoFn(Name, @_ModIo_FileFun_Parse, @Pf_Indef_Describe);
  _UpFirst(Name);
  AddFunction(MakeFunction2(Name, RetType, Arg1,
              MakeArg('DIE_ON_ERROR', PrimitiveTypes.PtBoolean)))
end;

procedure _AddFileProc(Name : string);
begin
  _AddIoProc1(Name, MakeVarArg('F', PrimitiveTypes.PtText))
end;

procedure _AddFileResetProc(Name : string);
begin
  AddPseudoFn(Name, @_ModIo_FileResetFun_Parse, @Pf_Indef_Describe);
  _UpFirst(Name);
  AddFunction(MakeProcedure3(Name,
              MakeVarArg('F', PrimitiveTypes.PtText),
              MakeArg('BLOCK_SIZE', PrimitiveTypes.PtInteger),
              MakeArg('DIE_ON_ERROR', PrimitiveTypes.PtBoolean)))
end;

procedure _AddFileProc1(Name : string; Arg1 : TPsVariable);
begin
  _AddIoProc2(Name, MakeVarArg('F', PrimitiveTypes.PtText), Arg1)
end;

procedure _AddDirProc(Name : string);
begin
  _AddIoProc1(Name, MakeConstArg('DIR', PrimitiveTypes.PtString))
end;

procedure _AddFileFun(Name : string; ReturnTypePtr : TPsTypePtr);
begin
  _AddIoFun1(Name, ReturnTypePtr, MakeVarArg('F', PrimitiveTypes.PtText))
end;

procedure _AddConstFileFun(Name : string; ReturnTypePtr : TPsTypePtr);
begin
  _AddIoFun1(Name, ReturnTypePtr, MakeConstArg('F', PrimitiveTypes.PtText))
end;


procedure RegisterGlobals_Io;
begin
  { I/O subroutines }
  PseudoFuns.Read := AddPseudoFn('READ', @_ModIoRead_Parse, @Pf_Indef_Describe);
  PseudoFuns.Readln := AddPseudoFn('READLN', @_ModIoRead_Parse,
                       @Pf_Indef_Describe);
  PseudoFuns.Write := AddPseudoFn('WRITE', @_ModIoWrite_Parse,
                      @Pf_Indef_Describe);
  PseudoFuns.Writeln := AddPseudoFn('WRITELN', @_ModIoWrite_Parse,
                        @Pf_Indef_Describe);
  _AddFileProc1('ASSIGN', MakeConstArg('NAME', PrimitiveTypes.PtString));
  _AddFileProc('CLOSE');
  _AddFileFun('EOF', PrimitiveTypes.PtBoolean);
  _AddFileFun('EOLN', PrimitiveTypes.PtBoolean);
  _AddConstFileFun('FILEPOS', PrimitiveTypes.PtInteger);
  _AddConstFileFun('FILESIZE', PrimitiveTypes.PtInteger);
  _AddFileProc('FLUSH');
  AddFunction(MakeFunction0('IORESULT', PrimitiveTypes.PtInteger));
  _AddFileResetProc('RESET');
  _AddFileResetProc('REWRITE');
  _AddFileProc1('SEEK', MakeArg('POS', PrimitiveTypes.PtInteger));
  _AddFileFun('SEEKEOF', PrimitiveTypes.PtBoolean);
  _AddFileFun('SEEKEOLN', PrimitiveTypes.PtBoolean);

  { File and directory subroutines }
  _AddDirProc('CHDIR');
  _AddFileProc('ERASE');
  _AddIoProc2('GETDIR',
              MakeArg('DRIVE', PrimitiveTypes.PtInteger),
              MakeVarArg('DIR', PrimitiveTypes.PtString));
  _AddDirProc('MKDIR');
  _AddFileProc1('RENAME',
                MakeConstArg('NAME', PrimitiveTypes.PtString));
  _AddDirProc('RMDIR');
end;

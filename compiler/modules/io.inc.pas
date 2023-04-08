

function _ModIoRead_Parse(FnExpr : TExpression) : TExpression;
var 
  First : boolean;
  ReadVar : TExpression;
  InFile : TExpression;
  NewLine : boolean;
  ArgList, ReadArg : TExReadArgList;
begin
  NewLine := FnExpr^.PseudoFnPtr^.Name = 'READLN';
  ExDispose(FnExpr);
  InFile := ExCoerce(ExVariable(FindNameOfClass('INPUT',
            TncVariable, {Required=}true)^.VarPtr), PrimitiveTypes.PtFile);
  ArgList := nil;
  if Lexer.Token.Id = TkLparen then
  begin
    First := true;
    WantTokenAndRead(TkLparen);
    while Lexer.Token.Id <> TkRparen do
    begin
      ReadVar := PsExpression;
      if First and IsFileType(ReadVar^.TypePtr) then
      begin
        EnsureAddressableExpr(ReadVar);
        ExDispose(InFile);
        InFile := ExCoerce(ReadVar, PrimitiveTypes.PtFile)
      end
      else
      begin
        EnsureAssignableExpr(ReadVar);
        if ArgList = nil then
        begin
          new(ArgList);
          ReadArg := ArgList
        end
        else
        begin
          new(ReadArg^.Next);
          ReadArg := ReadArg^.Next;
        end;
        ReadArg^.Next := nil;
        ReadArg^.Dest := ReadVar;
        ExMarkInitialized(ReadVar)
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      First := false
    end;
    WantTokenAndRead(TkRparen)
  end;
  Result := ExRead(InFile, ArgList, NewLine)
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
  WriteValue : TExWriteArg;
  OutFile : TExpression;
  NewLine : boolean;
  ArgList, WriteArg : TExWriteArgList;
begin
  NewLine := FnExpr^.PseudoFnPtr^.Name = 'WRITELN';
  ExDispose(FnExpr);
  OutFile := ExCoerce(ExVariable(FindNameOfClass('OUTPUT',
             TncVariable, {Required=}true)^.VarPtr), PrimitiveTypes.PtFile);
  ArgList := nil;
  if Lexer.Token.Id = TkLparen then
  begin
    First := true;
    WantTokenAndRead(TkLparen);
    while Lexer.Token.Id <> TkRparen do
    begin
      WriteValue := Pf_WriteArg_Parse;
      WriteValue.Arg := _ModIoWrite_EvaluateZeroArg(WriteValue.Arg);
      if First and IsTextType(WriteValue.Arg^.TypePtr) then
      begin
        EnsureAddressableExpr(WriteValue.Arg);
        ExDispose(OutFile);
        OutFile := ExCoerce(WriteValue.Arg, PrimitiveTypes.PtFile)
      end
      else
      begin
        if ArgList = nil then
        begin
          new(ArgList);
          WriteArg := ArgList
        end
        else
        begin
          new(WriteArg^.Next);
          WriteArg := WriteArg^.Next;
        end;
        WriteArg^.Value := WriteValue;
        WriteArg^.Next := nil
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      First := false
    end;
    WantTokenAndRead(TkRparen)
  end;
  Result := ExWrite(OutFile, ArgList, NewLine);
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
  AddPseudoFn(Name, @_ModIo_FileFun_Parse);
  _UpFirst(Name);
  AddFunction(MakeProcedure2(Name, Arg1,
              MakeArg('DIE_ON_ERROR', PrimitiveTypes.PtBoolean)))
end;

procedure _AddIoProc2(Name : string; Arg1, Arg2 : TPsVariable);
begin
  AddPseudoFn(Name, @_ModIo_FileFun_Parse);
  _UpFirst(Name);
  AddFunction(MakeProcedure3(Name, Arg1, Arg2,
              MakeArg('DIE_ON_ERROR', PrimitiveTypes.PtBoolean)))
end;

procedure _AddIoFun1(Name : string; RetType : TPsTypePtr; Arg1 : TPsVariable);
begin
  AddPseudoFn(Name, @_ModIo_FileFun_Parse);
  _UpFirst(Name);
  AddFunction(MakeFunction2(Name, RetType, Arg1,
              MakeArg('DIE_ON_ERROR', PrimitiveTypes.PtBoolean)))
end;

procedure _AddFileProc(Name : string);
begin
  _AddIoProc1(Name, MakeVarArg('F', PrimitiveTypes.PtFile))
end;

procedure _AddFileResetProc(Name : string);
begin
  AddPseudoFn(Name, @_ModIo_FileResetFun_Parse);
  _UpFirst(Name);
  AddFunction(MakeProcedure3(Name,
              MakeVarArg('F', PrimitiveTypes.PtFile),
  MakeArg('BLOCK_SIZE', PrimitiveTypes.PtInteger),
  MakeArg('DIE_ON_ERROR', PrimitiveTypes.PtBoolean)))
end;

procedure _AddFileProc1(Name : string; Arg1 : TPsVariable);
begin
  _AddIoProc2(Name, MakeVarArg('F', PrimitiveTypes.PtFile), Arg1)
end;

procedure _AddDirProc(Name : string);
begin
  _AddIoProc1(Name, MakeConstArg('DIR', PrimitiveTypes.PtString))
end;

procedure _AddFileFun(Name : string; ReturnTypePtr : TPsTypePtr);
begin
  _AddIoFun1(Name, ReturnTypePtr, MakeVarArg('F', PrimitiveTypes.PtFile))
end;

procedure _AddConstFileFun(Name : string; ReturnTypePtr : TPsTypePtr);
begin
  _AddIoFun1(Name, ReturnTypePtr, MakeConstArg('F', PrimitiveTypes.PtFile))
end;


procedure RegisterGlobals_Io;
begin
  { Default files }
  AddVariable(MakeVariable('INPUT', PrimitiveTypes.PtText));
  AddVariable(MakeVariable('OUTPUT', PrimitiveTypes.PtText));
  AddVariable(MakeVariable('STDERR', PrimitiveTypes.PtText));

  { I/O subroutines }
  AddPseudoFn('READ', @_ModIoRead_Parse);
  AddPseudoFn('READLN', @_ModIoRead_Parse);
  AddPseudoFn('WRITE', @_ModIoWrite_Parse);
  AddPseudoFn('WRITELN', @_ModIoWrite_Parse);
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

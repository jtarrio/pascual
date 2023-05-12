function _ModIo_TypeIsValidForFileRead(InFile, Expr : TSExpression) : boolean;
const ReadableTypes = [SdtcInteger, SdtcReal, SdtcChar, SdtcString];
begin
  if Expr^.TypePtr = nil then Result := false
  else if IsTextType(InFile^.TypePtr) then
         Result := GetFundamentalType(Expr^.TypePtr)^.Cls in ReadableTypes
  else if IsFileType(InFile^.TypePtr) then
         Result := IsSameType(InFile^.TypePtr^.FileDef.TypePtr, Expr^.TypePtr)
  else Result := false
end;

function _ModIo_TypeIsValidForFileWrite(OutFile, Expr : TSExpression) : boolean;
const WritableTypes = [SdtcBoolean, SdtcInteger, SdtcReal, SdtcChar, SdtcString,
                      SdtcEnum];
begin
  if Expr^.TypePtr = nil then Result := false
  else if IsTextType(OutFile^.TypePtr) then
         Result := GetFundamentalType(Expr^.TypePtr)^.Cls in WritableTypes
  else if IsFileType(OutFile^.TypePtr) then
         Result := IsSameType(OutFile^.TypePtr^.FileDef.TypePtr, Expr^.TypePtr)
  else Result := false
end;

function _ModIo_NeedsToMakeAddressable(OutFile, Expr : TSExpression) : boolean;
begin
  Result := not IsTextType(OutFile^.TypePtr) and not Expr^.IsAddressable
end;

function _ModIoRead_Parse(FnExpr : TSExpression) : TSExpression;
var 
  First : boolean;
  ReadVar : TSExpression;
  InFile : TSExpression;
  NewLine : boolean;
  ArgList, ReadArg : TSEReadArgList;
  ArgAddPoint : TListAddPoint;
begin
  NewLine := FnExpr^.PsfnPtr^.Name = 'READLN';
  ExDispose(FnExpr);
  InFile := ExVariable(FindNameOfClass('INPUT',
            SdncVariable, {Required=}true)^.VarPtr);
  ArgList := nil;
  ArgAddPoint := List_GetAddPoint(ArgList);
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
        InFile := ReadVar;
        if NewLine and not IsTextType(InFile^.TypePtr) then
          ErrorForExpr('Invalid file type for READLN', InFile)
      end
      else
      begin
        EnsureAssignableExpr(ReadVar);
        if not _ModIo_TypeIsValidForFileRead(InFile, ReadVar) then
          ErrorForExpr('Variable has invalid type for READ on ' +
                       TypeName(InFile^.TypePtr), ReadVar);
        new(ReadArg);
        ReadArg^.Dest := ReadVar;
        ExMarkInitialized(ReadVar);
        List_Add(ArgAddPoint, ReadArg)
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      First := false
    end;
    WantTokenAndRead(TkRparen)
  end;
  Result := ExRead(InFile, ArgList, NewLine, Options.CheckIoResult)
end;

function _ModIoWrite_EvaluateZeroArg(Expr : TSExpression) : TSExpression;
var Args : TSEFunctionArgs;
begin
  Args.Size := 0;
  if IsFunctionType(Expr^.TypePtr)
     and (Expr^.TypePtr^.FnDefPtr^.ReturnTypePtr <> nil)
     and (Expr^.TypePtr^.FnDefPtr^.Args.Count = 0) then
    Result := ExFunctionCall(Expr, Args)
  else Result := Expr
end;

function _ModIoWrite_Parse(FnExpr : TSExpression) : TSExpression;
var 
  First : boolean;
  WriteValue : TSEWriteArg;
  OutFile : TSExpression;
  NewLine : boolean;
  ArgList, WriteArg : TSEWriteArgList;
  ArgAddPoint : TListAddPoint;
begin
  NewLine := FnExpr^.PsfnPtr^.Name = 'WRITELN';
  ExDispose(FnExpr);
  OutFile := ExVariable(FindNameOfClass('OUTPUT',
             SdncVariable, {Required=}true)^.VarPtr);
  Result := nil;
  ArgList := nil;
  ArgAddPoint := List_GetAddPoint(ArgList);
  if Lexer.Token.Id = TkLparen then
  begin
    First := true;
    WantTokenAndRead(TkLparen);
    while Lexer.Token.Id <> TkRparen do
    begin
      WriteValue := Pf_WriteArg_Parse;
      WriteValue.Arg := _ModIoWrite_EvaluateZeroArg(WriteValue.Arg);
      if First and IsFileType(WriteValue.Arg^.TypePtr) then
      begin
        EnsureAddressableExpr(WriteValue.Arg);
        ExDispose(OutFile);
        OutFile := WriteValue.Arg;
        if NewLine and not IsTextType(OutFile^.TypePtr) then
          ErrorForExpr('Invalid file type for WRITELN', OutFile)
      end
      else
      begin
        if not _ModIo_TypeIsValidForFileWrite(OutFile, WriteValue.Arg) then
          ErrorForExpr('Expression has invalid type for WRITE on ' +
                       TypeName(OutFile^.TypePtr), WriteValue.Arg);
        if _ModIo_NeedsToMakeAddressable(OutFile, WriteValue.Arg) then
        begin
          if Result = nil then
          begin
            Result := ExWrite(OutFile, ArgList, NewLine, Options.CheckIoResult);
            ArgAddPoint := List_GetAddPoint(Result^.WriteArgs)
          end;
          Result := ExWithTmpVar(ExVariable(AddAliasVariable(
                    'tmp', WriteValue.Arg^.TypePtr, WriteValue.Arg)),
                    WriteValue.Arg, Result);
          WriteValue.Arg := ExCopy(Result^.TmpVar)
        end;
        if IsEnumType(WriteValue.Arg^.TypePtr) then
          GetFundamentalType(WriteValue.Arg^.TypePtr)^.EnumPtr^
          .NeedValueArray := true;
        new(WriteArg);
        WriteArg^.Value := WriteValue;
        List_Add(ArgAddPoint, WriteArg)
      end;
      WantToken2(TkComma, TkRparen);
      SkipToken(TkComma);
      First := false
    end;
    WantTokenAndRead(TkRparen)
  end;
  if Result = nil then
    Result := ExWrite(OutFile, ArgList, NewLine, Options.CheckIoResult)
end;

procedure _UpFirst(var Str : string);
var Pos : integer;
begin
  for Pos := 1 to Length(Str) do
    if Pos = 1 then Str[Pos] := Upcase(Str[Pos])
    else Str[Pos] := Lowercase(Str[Pos]);
end;

function _ModIo_FileFun_Parse(FnExpr : TSExpression) : TSExpression;
var 
  Args : TSEFunctionArgs;
  FnName : string;
  SrPtr : TSDSubroutine;
begin
  FnName := FnExpr^.PsfnPtr^.Name;
  _UpFirst(FnName);
  ExDispose(FnExpr);
  SrPtr := FindNameOfClass(FnName, SdncSubroutine, {Required=}true)^.SrPtr;
  Args := PsFunctionArgs;
  Args.Size := Args.Size + 1;
  Args.Values[Args.Size] := ExBooleanConstant(Options.CheckIoResult);
  Result := ExFunctionCall(ExFnRef(SrPtr), Args)
end;

function _ModIo_FileResetFun_Parse(FnExpr : TSExpression) : TSExpression;
var 
  Args : TSEFunctionArgs;
  FnName : string;
  SrPtr : TSDSubroutine;
  FileTypePtr : TSDType;
begin
  FnName := FnExpr^.PsfnPtr^.Name;
  _UpFirst(FnName);
  ExDispose(FnExpr);
  SrPtr := FindNameOfClass(FnName, SdncSubroutine, {Required=}true)^.SrPtr;
  Args := PsFunctionArgs;
  Args.Size := Args.Size + 2;
  if Args.Values[1]^.Cls = SecToGenericFile then
    FileTypePtr := Args.Values[1]^.ToGenericFileParent^.TypePtr
  else FileTypePtr := Args.Values[1]^.TypePtr;
  if IsTextType(FileTypePtr) or IsGenericFileType(FileTypePtr) then
    Args.Values[Args.Size - 1] := ExIntegerConstant(0)
  else
    Args.Values[Args.Size - 1] := ExSizeof(FileTypePtr^.FileDef.TypePtr);
  Args.Values[Args.Size] := ExBooleanConstant(Options.CheckIoResult);
  Result := ExFunctionCall(ExFnRef(SrPtr), Args)
end;

procedure _AddIoProc1(Name : string; Arg1 : TSDSubroutineArg);
begin
  AddPsfn(Name, @_ModIo_FileFun_Parse);
  _UpFirst(Name);
  AddFunction(MakeProcedure2(Name, Arg1,
              MakeArg('DIE_ON_ERROR', PrimitiveTypes.PtBoolean)))
end;

procedure _AddIoProc2(Name : string; Arg1, Arg2 : TSDSubroutineArg);
begin
  AddPsfn(Name, @_ModIo_FileFun_Parse);
  _UpFirst(Name);
  AddFunction(MakeProcedure3(Name, Arg1, Arg2,
              MakeArg('DIE_ON_ERROR', PrimitiveTypes.PtBoolean)))
end;

procedure _AddIoFun1(Name : string; RetType : TSDType; Arg1 : TSDSubroutineArg);
begin
  AddPsfn(Name, @_ModIo_FileFun_Parse);
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
  AddPsfn(Name, @_ModIo_FileResetFun_Parse);
  _UpFirst(Name);
  AddFunction(MakeProcedure3(Name,
              MakeVarArg('F', PrimitiveTypes.PtFile),
  MakeArg('BLOCK_SIZE', PrimitiveTypes.PtInteger),
  MakeArg('DIE_ON_ERROR', PrimitiveTypes.PtBoolean)))
end;

procedure _AddFileProc1(Name : string; Arg1 : TSDSubroutineArg);
begin
  _AddIoProc2(Name, MakeVarArg('F', PrimitiveTypes.PtFile), Arg1)
end;

procedure _AddDirProc(Name : string);
begin
  _AddIoProc1(Name, MakeConstArg('DIR', PrimitiveTypes.PtString))
end;

procedure _AddFileFun(Name : string; ReturnTypePtr : TSDType);
begin
  _AddIoFun1(Name, ReturnTypePtr, MakeVarArg('F', PrimitiveTypes.PtFile))
end;

procedure _AddConstFileFun(Name : string; ReturnTypePtr : TSDType);
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
  AddPsfn('READ', @_ModIoRead_Parse);
  AddPsfn('READLN', @_ModIoRead_Parse);
  AddPsfn('WRITE', @_ModIoWrite_Parse);
  AddPsfn('WRITELN', @_ModIoWrite_Parse);
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

function _NewExpr(Cls : TSExpressionClass) : TSExpression;
begin
  new(Result);
  Result^.Cls := Cls;
  Result^.TypePtr := nil;
  Result^.IsAssignable := false;
  Result^.IsAddressable := false;
  Result^.IsFunctionResult := false;
  Result^.IsStatement := false;
  Result^.CheckBounds := Options.CheckBounds
end;

procedure _DisposeImmediate(var Imm : TSEImmediate);
var Bounds : TSESetImmBounds;
begin
  if Imm.Cls = SeicSet then
  begin
    while Imm.SetBounds <> nil do
    begin
      Bounds := Imm.SetBounds^.Next;
      Dispose(Imm.SetBounds);
      Imm.SetBounds := Bounds
    end
  end
end;

procedure _DisposeArrayElems(Elem : TSEArrayElem);
var NextElem : TSEArrayElem;
begin
  while Elem <> nil do
  begin
    NextElem := Elem^.Next;
    ExDispose(Elem^.Value);
    dispose(Elem);
    Elem := NextElem
  end
end;

procedure _DisposeRecordFields(Field : TSERecordField);
var NextField : TSERecordField;
begin
  while Field <> nil do
  begin
    NextField := Field^.Next;
    ExDispose(Field^.Value);
    dispose(Field);
    Field := NextField
  end
end;

procedure _DisposeBounds(Bounds : TSESetExprBounds);
var Next : TSESetExprBounds;
begin
  while Bounds <> nil do
  begin
    Next := Bounds^.Next;
    ExDispose(Bounds^.First);
    if Bounds^.Last <> nil then ExDispose(Bounds^.Last);
    dispose(Bounds);
    Bounds := Next
  end
end;

procedure _DisposeWriteArg(var WriteArg : TSEWriteArg);
begin
  ExDispose(WriteArg.Arg);
  if WriteArg.Width <> nil then ExDispose(WriteArg.Width);
  if WriteArg.Prec <> nil then ExDispose(WriteArg.Prec)
end;

procedure _DisposeReadExpr(var Expr : TSExpression);
var ReadArg : TSEReadArgList;
begin
  ExDispose(Expr^.ReadFile);
  while List_Shift(Expr^.ReadArgs, ReadArg) do
  begin
    ExDispose(ReadArg^.Dest);
    dispose(ReadArg)
  end
end;

procedure _DisposeWriteExpr(var Expr : TSExpression);
var WriteArg : TSEWriteArgList;
begin
  ExDispose(Expr^.WriteFile);
  while List_Shift(Expr^.WriteArgs, WriteArg) do
  begin
    _DisposeWriteArg(WriteArg^.Value);
    dispose(WriteArg)
  end
end;

procedure ExDispose;
var Pos : integer;
begin
  case Expr^.Cls of 
    SecImmediate: _DisposeImmediate(Expr^.Immediate);
    SecArrayValue: _DisposeArrayElems(Expr^.ArrayElem);
    SecRecordValue: _DisposeRecordFields(Expr^.RecordField);
    SecSetValue:
                 begin
                   ExDispose(Expr^.SetBase);
                   _DisposeBounds(Expr^.SetBounds);
                 end;
    SecToString: ExDispose(Expr^.ToStrParent);
    SecToReal: ExDispose(Expr^.ToRealParent);
    SecToUntypedPtr: ExDispose(Expr^.ToUntypedPtrParent);
    SecToGenericFile: ExDispose(Expr^.ToGenericFileParent);
    SecWithTmpVar:
                   begin
                     ExDispose(Expr^.TmpVar);
                     ExDispose(Expr^.TmpVarValue);
                     ExDispose(Expr^.TmpVarChild);
                   end;
    SecSubrange: ExDispose(Expr^.SubrangeParent);
    SecField: ExDispose(Expr^.RecExpr);
    SecArray:
              begin
                ExDispose(Expr^.ArrayExpr);
                ExDispose(Expr^.ArrayIndex);
              end;
    SecPointer: ExDispose(Expr^.PointerExpr);
    SecAddress: ExDispose(Expr^.AddressExpr);
    SecStringChar:
                   begin
                     ExDispose(Expr^.StringExpr);
                     ExDispose(Expr^.StringIndex);
                   end;
    SecFnCall:
               begin
                 ExDispose(Expr^.FnExpr);
                 for Pos := 1 to Expr^.CallArgs.Size do
                   ExDispose(Expr^.CallArgs.Values[Pos]);
               end;
    SecConvertToStr:
                     begin
                       _DisposeWriteArg(Expr^.ToStrSrc);
                       ExDispose(Expr^.ToStrDest)
                     end;
    SecConvertToVal:
                     begin
                       ExDispose(Expr^.ToValSrc);
                       ExDispose(Expr^.ToValDest);
                       ExDispose(Expr^.ToValCode)
                     end;
    SecRead: _DisposeReadExpr(Expr);
    SecWrite: _DisposeWriteExpr(Expr);
    SecUnaryOp: ExDispose(Expr^.Unary.Parent);
    SecBinaryOp:
                 begin
                   ExDispose(Expr^.Binary.Left);
                   ExDispose(Expr^.Binary.Right);
                 end;
  end;
  dispose(Expr);
end;

function _CopyImmediate(const Imm : TSEImmediate) : TSEImmediate;
var 
  Src, Dst : TSESetImmBounds;
  AddPoint : TListAddPoint;
begin
  Result := Imm;
  if Imm.Cls = SeicSet then
  begin
    Src := Imm.SetBounds;
    Result.SetBounds := nil;
    AddPoint := List_GetAddPoint(Result.SetBounds);
    while Src <> nil do
    begin
      new(Dst);
      Dst^ := Src^;
      List_Add(AddPoint, Dst);
      Src := Src^.Next
    end
  end
end;

function _CopyArrayElems(Elem : TSEArrayElem) : TSEArrayElem;
var 
  Src, Dst : TSEArrayElem;
  AddPoint : TListAddPoint;
begin
  Src := Elem;
  Result := nil;
  AddPoint := List_GetAddPoint(Result);
  while Src <> nil do
  begin
    new(Dst);
    Dst^.Value := ExCopy(Src^.Value);
    List_Add(AddPoint, Dst);
    Src := Src^.Next
  end
end;

function _CopyRecordFields(Field : TSERecordField) : TSERecordField;
var 
  Src, Dst : TSERecordField;
  AddPoint : TListAddPoint;
begin
  Src := Field;
  Result := nil;
  AddPoint := List_GetAddPoint(Result);
  while Src <> nil do
  begin
    new(Dst);
    Dst^.Ordinal := Src^.Ordinal;
    Dst^.Value := ExCopy(Src^.Value);
    List_Add(AddPoint, Dst);
    Src := Src^.Next
  end
end;

function _CopyBounds(Bounds : TSESetExprBounds) : TSESetExprBounds;
var 
  Src, Dst : TSESetExprBounds;
  AddPoint : TListAddPoint;
begin
  Src := Bounds;
  Result := nil;
  AddPoint := List_GetAddPoint(Result);
  while Src <> nil do
  begin
    new(Dst);
    Dst^.First := ExCopy(Src^.First);
    if Src^.Last <> nil then Dst^.Last := ExCopy(Src^.Last)
    else Dst^.Last := nil;
    List_Add(AddPoint, Dst);
    Src := Src^.Next
  end
end;

function _CopyWriteArg(const WriteArg : TSEWriteArg) : TSEWriteArg;
begin
  Result.Arg := ExCopy(WriteArg.Arg);
  if WriteArg.Width = nil then Result.Width := nil
  else Result.Width := ExCopy(WriteArg.Width);
  if WriteArg.Prec = nil then Result.Prec := nil
  else Result.Prec := ExCopy(WriteArg.Prec)
end;

procedure _CopyReadExpr(var Expr, Copy : TSExpression);
var 
  Src, Dst : TSEReadArgList;
  AddPoint : TListAddPoint;
begin
  Copy^.ReadFile := ExCopy(Expr^.ReadFile);
  Copy^.ReadLn := Expr^.ReadLn;
  Copy^.ReadArgs := nil;
  AddPoint := List_GetAddPoint(Copy^.ReadArgs);
  Src := Expr^.ReadArgs;
  while Src <> nil do
  begin
    new(Dst);
    Dst^.Dest := ExCopy(Src^.Dest);
    List_Add(AddPoint, Dst);
    Src := Src^.Next
  end
end;

procedure _CopyWriteExpr(var Expr, Copy : TSExpression);
var 
  Src, Dst : TSEWriteArgList;
  AddPoint : TListAddPoint;
begin
  Copy^.WriteFile := ExCopy(Expr^.WriteFile);
  Copy^.WriteLn := Expr^.WriteLn;
  Copy^.WriteArgs := nil;
  AddPoint := List_GetAddPoint(Copy^.WriteArgs);
  Src := Expr^.WriteArgs;
  while Src <> nil do
  begin
    new(Dst);
    Dst^.Value := _CopyWriteArg(Src^.Value);
    List_Add(AddPoint, Dst);
    Src := Src^.Next
  end
end;

function ExCopy;
var 
  Copy : TSExpression;
  Pos : integer;
begin
  Copy := _NewExpr(Expr^.Cls);
  Copy^.TypePtr := Expr^.TypePtr;
  Copy^.IsAssignable := Expr^.IsAssignable;
  Copy^.IsAddressable := Expr^.IsAddressable;
  Copy^.IsFunctionResult := Expr^.IsFunctionResult;
  Copy^.CheckBounds := Expr^.CheckBounds;
  case Expr^.Cls of 
    SecImmediate: Copy^.Immediate := _CopyImmediate(Expr^.Immediate);
    SecArrayValue: Copy^.ArrayElem := _CopyArrayElems(Expr^.ArrayElem);
    SecRecordValue: Copy^.RecordField := _CopyRecordFields(Expr^.RecordField);
    SecSetValue :
                  begin
                    Copy^.SetBase := ExCopy(Expr^.SetBase);
                    Copy^.SetBounds := _CopyBounds(Expr^.SetBounds);
                  end;
    SecToString: Copy^.ToStrParent := ExCopy(Expr^.ToStrParent);
    SecToReal: Copy^.ToRealParent := ExCopy(Expr^.ToRealParent);
    SecToUntypedPtr: Copy^.ToUntypedPtrParent := ExCopy(Expr^.
                                                 ToUntypedPtrParent);
    SecToGenericFile: Copy^.ToGenericFileParent := ExCopy(Expr^.
                                                   ToGenericFileParent);
    SecWithTmpVar :
                    begin
                      Copy^.TmpVar := ExCopy(Expr^.TmpVar);
                      Copy^.TmpVarValue := ExCopy(Expr^.TmpVarValue);
                      Copy^.TmpVarChild := ExCopy(Expr^.TmpVarChild);
                    end;
    SecSubrange : Copy^.SubrangeParent := ExCopy(Expr^.SubrangeParent);
    SecVariable: Copy^.VarPtr := Expr^.VarPtr;
    SecField:
              begin
                Copy^.RecExpr := ExCopy(Expr^.RecExpr);
                Copy^.RecFieldNum := Expr^.RecFieldNum
              end;
    SecArray:
              begin
                Copy^.ArrayExpr := ExCopy(Expr^.ArrayExpr);
                Copy^.ArrayIndex := ExCopy(Expr^.ArrayIndex)
              end;
    SecPointer: Copy^.PointerExpr := ExCopy(Expr^.PointerExpr);
    SecAddress: Copy^.AddressExpr := ExCopy(Expr^.AddressExpr);
    SecStringChar:
                   begin
                     Copy^.StringExpr := ExCopy(Expr^.StringExpr);
                     Copy^.StringIndex := ExCopy(Expr^.StringIndex)
                   end;
    SecFnRef: Copy^.FnPtr := Expr^.FnPtr;
    SecFnCall:
               begin
                 Copy^.FnExpr := ExCopy(Expr^.FnExpr);
                 Copy^.CallArgs.Size := Expr^.CallArgs.Size;
                 for Pos := 1 to Expr^.CallArgs.Size do
                   Copy^.CallArgs.Values[Pos] := ExCopy(Expr^.CallArgs
                                                 .Values[Pos])
               end;
    SecPsfnRef: Copy^.PsfnPtr := Expr^.PsfnPtr;
    SecSizeof: Copy^.SizeofTypePtr := Expr^.SizeofTypePtr;
    SecConvertToStr:
                     begin
                       Copy^.ToStrSrc := _CopyWriteArg(Expr^.ToStrSrc);
                       Copy^.ToStrDest := ExCopy(Expr^.ToStrDest)
                     end;
    SecConvertToVal:
                     begin
                       Copy^.ToValSrc := ExCopy(Expr^.ToValSrc);
                       Copy^.ToValDest := ExCopy(Expr^.ToValDest);
                       Copy^.ToValCode := ExCopy(Expr^.ToValCode)
                     end;
    SecRead: _CopyReadExpr(Expr, Copy);
    SecWrite: _CopyWriteExpr(Expr, Copy);
    SecUnaryOp:
                begin
                  Copy^.Unary.Parent := ExCopy(Expr^.Unary.Parent);
                  Copy^.Unary.Op := Expr^.Unary.Op
                end;
    SecBinaryOp:
                 begin
                   Copy^.Binary.Left := ExCopy(Expr^.Binary.Left);
                   Copy^.Binary.Right := ExCopy(Expr^.Binary.Right);
                   Copy^.Binary.Op := Expr^.Binary.Op
                 end;
    else InternalError('Cannot copy expression: ' + ExDescribe(Expr))
  end;
  ExCopy := Copy
end;

function _DescribeImmSetInternal(Bounds : TSESetImmBounds;
                                 SetOfTypePtr : TSDType) : string;
begin
  Result := '';
  while Bounds <> nil do
  begin
    Result := Result +
              ExDescribe(ExGetAntiOrdinal(Bounds^.First, SetOfTypePtr));
    if Bounds^.First <> Bounds^.Last then
      Result := Result + '..' +
                ExDescribe(ExGetAntiOrdinal(Bounds^.Last, SetOfTypePtr));
    Bounds := Bounds^.Next;
    if Bounds <> nil then Result := Result + ', '
  end
end;

function _DescribeImmSet(Bounds : TSESetImmBounds;
                         SetOfTypePtr : TSDType) : string;
begin
  Result := '[' + _DescribeImmSetInternal(Bounds, SetOfTypePtr) + ']'
end;

function _DescribeImmediate(Expr : TSExpression) : string;
begin
  with Expr^.Immediate do
    case Cls of 
      SeicNil: Result := 'nil';
      SeicBoolean: Str(BooleanVal, Result);
      SeicInteger: Str(IntegerVal, Result);
      SeicReal: Str(RealVal, Result);
      SeicChar: Result := UnparseChar(CharVal);
      SeicString: Result := UnparseString(StringVal);
      SeicEnum: Result := EnumPtr^.Values[EnumOrdinal];
      SeicSet: Result := _DescribeImmSet(SetBounds, SetOfTypePtr);
      else InternalError('Cannot describe immediate value')
    end
end;

const _ExPrecedences : array[TSExpressionClass] of integer 
                       = (
                          {SecImmediate=}0, {SecArrayValue=}0,
                          {SecRecordValue=}0, {SecSetValue=}0,
                          {SecToString=}-1, {SecToReal=}-1,
                          {SecToUntypedPtr=}-1, {SecToGenericFile=}-1,
                          {SecWithTmpVar=}-1, {SecSubrange=}0,
                          {SecVariable=}0, {SecField=}1, {SecArray=}1,
                          {SecPointer=}1, {SecAddress=}1, {SecStringChar=}1,
                          {SecFnRef=}0, {SecFnCall=}0, {SecPsfnRef=}0,
                          {SecSizeof=}1, {SecConvertToStr=}1,
                          {SecConvertToVal=}1, {SecRead=}1, {SecWrite=}1,
                          {SecUnaryOp=}-1, {SecBinaryOp=}-1);
const _ExOperators : array[TSEOperator] of record
  Precedence: integer;
  Name: string
end 
= (
    {SeoAdd=}(Precedence: 4; Name: '+'),
    {SeoSub=}(Precedence: 4; Name: '-'),
    {SeoMul=}(Precedence: 4; Name: '*'),
    {SeoDivReal=}(Precedence: 3; Name: '/'),
    {SeoDivInt=}(Precedence: 3; Name: 'DIV'),
    {SeoMod=}(Precedence: 3; Name: 'MOD'),
    {SeoNeg=}(Precedence: 4; Name: '-'),
    {SeoAnd=}(Precedence: 3; Name: 'AND'),
    {SeoOr=}(Precedence: 4; Name: 'OR'),
    {SeoXor=}(Precedence: 4; Name: 'XOR'),
    {SeoShl=}(Precedence: 3; Name: 'SHL'),
    {SeoShr=}(Precedence: 3; Name: 'SHR'),
    {SeoNot=}(Precedence: 1; Name: 'NOT'),
    {SeoIn=}(Precedence: 5; Name: 'IN'),
    {SeoEq=}(Precedence: 5; Name: '='),
    {SeoNe=}(Precedence: 5; Name: '<>'),
    {SeoLt=}(Precedence: 5; Name: '<'),
    {SeoGt=}(Precedence: 5; Name: '>'),
    {SeoLtEq=}(Precedence: 5; Name: '<='),
    {SeoGtEq=}(Precedence: 5; Name: '>='),
    {SeoOrd=}(Precedence: 1; Name: 'ORD'),
    {SeoPred=}(Precedence: 1; Name: 'PREC'),
    {SeoSucc=}(Precedence: 1; Name: 'SUCC')
  );

function _ExprPrecedence(Expr : TSExpression) : integer;
begin
  Result := _ExPrecedences[Expr^.Cls];
  if Result < 0 then
  begin
    case Expr^.Cls of 
      SecToString: Result := _ExprPrecedence(Expr^.ToStrParent);
      SecToReal: Result := _ExprPrecedence(Expr^.ToRealParent);
      SecToUntypedPtr: Result := _ExprPrecedence(Expr^.ToUntypedPtrParent);
      SecToGenericFile: Result := _ExprPrecedence(Expr^.ToGenericFileParent);
      SecWithTmpVar: Result := _ExprPrecedence(Expr^.TmpVarChild);
      SecSubrange: Result := _ExprPrecedence(Expr^.SubrangeParent);
      SecUnaryOp: Result := _ExOperators[Expr^.Unary.Op].Precedence;
      SecBinaryOp: Result := _ExOperators[Expr^.Binary.Op].Precedence;
    end
  end;
  if Result < 0 then
    CompileError('Unknown precedence for expression')
end;

function ExDescribeOperator(Op : TSEOperator) : string;
begin
  Result := _ExOperators[Op].Name
end;

function _DescribeUnaryOpExpr(Expr : TSExpression) : string;
var UseParens : boolean;
begin
  if Expr^.Unary.Op in [SeoOrd, SeoPred, SeoSucc] then
  begin
    Result := ExDescribeOperator(Expr^.Unary.Op) + '(' +
              ExDescribe(Expr^.Unary.Parent) + ')'
  end
  else
  begin
    Result := ExDescribeOperator(Expr^.Unary.Op);
    if Expr^.Unary.Op <> SeoNeg then Result := Result + ' ';
    UseParens := _ExprPrecedence(Expr) < _ExprPrecedence(Expr^.Unary.Parent);
    if UseParens then Result := Result + '(';
    Result := Result + ExDescribe(Expr^.Unary.Parent);
    if UseParens then Result := Result + ')'
  end
end;

function _DescribeBinaryOpExpr(Expr : TSExpression) : string;
var UseParens : boolean;
begin
  UseParens := _ExprPrecedence(Expr) < _ExprPrecedence(Expr^.Binary.Left);
  if UseParens then Result := '('
  else Result := '';
  Result := Result + ExDescribe(Expr^.Binary.Left);
  if UseParens then Result := Result + ')';
  Result := Result + ' ' + ExDescribeOperator(Expr^.Binary.Op) + ' ';
  UseParens := _ExprPrecedence(Expr) < _ExprPrecedence(Expr^.Binary.Right);
  if UseParens then Result := Result + '(';
  Result := Result + ExDescribe(Expr^.Binary.Right);
  if UseParens then Result := Result + ')';
end;

function _DescribeArrayValue(Expr : TSExpression) : string;
var Elem : TSEArrayElem;
begin
  Result := '(';
  Elem := Expr^.ArrayElem;
  while Elem <> nil do
  begin
    Result := Result + ExDescribe(Elem^.Value);
    Elem := Elem^.Next;
    if Elem <> nil then Result := Result + ', '
  end;
  Result := Result + ')'
end;

function _DescribeRecordValue(Expr : TSExpression) : string;
var Field : TSERecordField;
begin
  Result := '(';
  Field := Expr^.RecordField;
  while Field <> nil do
  begin
    Result := Result + Expr^.TypePtr^.RecPtr^.Fields[Field^.Ordinal].Name +
              ':' + ExDescribe(Field^.Value);
    Field := Field^.Next;
    if Field <> nil then Result := Result + '; '
  end;
  Result := Result + ')'
end;

function _DescribeSetValue(Expr : TSExpression) : string;
var 
  Bounds : TSESetExprBounds;
begin
  Result := '[' + _DescribeImmSetInternal(Expr^.SetBase^.Immediate.SetBounds,
            Expr^.SetBase^.Immediate.SetOfTypePtr);
  if Expr^.SetBase^.Immediate.SetBounds <> nil then Result := Result + ', ';
  Bounds := Expr^.SetBounds;
  while Bounds <> nil do
  begin
    Result := Result + ExDescribe(Bounds^.First);
    if Bounds^.Last <> nil then
      Result := Result + '..' + ExDescribe(Bounds^.Last);
    Bounds := Bounds^.Next;
    if Bounds <> nil then Result := Result + ', '
  end;
  Result := Result + ']'
end;

function ExDescribe;
var 
  Pos : integer;
begin
  case Expr^.Cls of 
    SecImmediate: Result := _DescribeImmediate(Expr);
    SecArrayValue: Result := _DescribeArrayValue(Expr);
    SecRecordValue: Result := _DescribeRecordValue(Expr);
    SecSetValue: Result := _DescribeSetValue(Expr);
    SecToString: Result := ExDescribe(Expr^.ToStrParent);
    SecToReal: Result := ExDescribe(Expr^.ToRealParent);
    SecToUntypedPtr: Result := ExDescribe(Expr^.ToUntypedPtrParent);
    SecToGenericFile: Result := ExDescribe(Expr^.ToGenericFileParent);
    SecWithTmpVar: Result := ExDescribe(Expr^.TmpVarChild);
    SecSubrange: Result := ExDescribe(Expr^.ToStrParent);
    SecVariable: if Expr^.VarPtr^.IsAliasFor = nil then
                   Result := Expr^.VarPtr^.Name
                 else
                   Result := ExDescribe(Expr^.VarPtr^.IsAliasFor);
    SecField: Result := ExDescribe(Expr^.RecExpr) + '.' +
                        Expr^.RecExpr^.TypePtr^.RecPtr^
                        .Fields[Expr^.RecFieldNum].Name;
    SecArray: Result := ExDescribe(Expr^.ArrayExpr) +
                        '[' + ExDescribe(Expr^.ArrayIndex) + ']';
    SecPointer: Result := ExDescribe(Expr^.PointerExpr) + '^';
    SecAddress: Result := '@' + ExDescribe(Expr^.AddressExpr);
    SecStringChar: Result := ExDescribe(Expr^.StringExpr) + '[' +
                             ExDescribe(Expr^.StringIndex) + ']';
    SecFnRef: Result := Expr^.FnPtr^.Name;
    SecFnCall:
               begin
                 Result := ExDescribe(Expr^.FnExpr) + '(';
                 for Pos := 1 to Expr^.CallArgs.Size do
                 begin
                   if Pos <> 1 then Result := Result + ', ';
                   Result := Result + ExDescribe(Expr^.CallArgs.Values[Pos])
                 end;
                 Result := Result + ')'
               end;
    SecPsfnRef: Result := Expr^.PsfnPtr^.Name;
    SecSizeof: Result := 'SIZEOF(' + TypeName(Expr^.SizeofTypePtr) + ')';
    SecConvertToStr: with Expr^.ToStrSrc do
                       if Width = nil then
                         Result := 'STR(' + ExDescribe(Arg) + ', ' +
                                   ExDescribe(Expr^.ToStrDest) + ')'
                       else if Prec = nil then
                              Result := 'STR(' + ExDescribe(Arg) + ':' +
                                        ExDescribe(Width) + ', ' +
                                        ExDescribe(Expr^.ToStrDest) + ')'
                       else Result := 'STR(' + ExDescribe(Arg) + ':' +
                                      ExDescribe(Width) + ':' +
                                      ExDescribe(Prec) + ', ' +
                                      ExDescribe(Expr^.ToStrDest) + ')';
    SecConvertToVal: Result := 'VAL(' + ExDescribe(Expr^.ToValSrc) + ', ' +
                               ExDescribe(Expr^.ToValDest) + ', ' +
                               ExDescribe( Expr^.ToValCode) + ')';
    SecRead: if Expr^.ReadLn then Result := 'READLN(...)'
             else Result := 'READ(...)';
    SecWrite: if Expr^.WriteLn then Result := 'WRITELN(...)'
              else Result := 'WRITE(...)';
    SecUnaryOp: Result := _DescribeUnaryOpExpr(Expr);
    SecBinaryOp: Result := _DescribeBinaryOpExpr(Expr);
    else InternalError('Cannot describe expression')
  end
end;

function ExCoerce(Expr : TSExpression; TypePtr : TSDType) : TSExpression;
forward;

function _ExImmediate(Cls : TSEImmediateClass) : TSExpression;
var Expr : TSExpression;
begin
  Expr := _NewExpr(SecImmediate);
  Expr^.Immediate.Cls := Cls;
  _ExImmediate := Expr
end;

function ExNil : TSExpression;
begin
  Result := _ExImmediate(SeicNil);
  Result^.TypePtr := PrimitiveTypes.PtNil
end;

function ExBooleanConstant(Value : boolean) : TSExpression;
begin
  Result := _ExImmediate(SeicBoolean);
  Result^.Immediate.BooleanVal := Value;
  Result^.TypePtr := PrimitiveTypes.PtBoolean
end;

function ExIntegerConstant(Value : integer) : TSExpression;
begin
  Result := _ExImmediate(SeicInteger);
  Result^.Immediate.IntegerVal := Value;
  Result^.TypePtr := PrimitiveTypes.PtInteger
end;

function ExRealConstant(Value : real) : TSExpression;
begin
  Result := _ExImmediate(SeicReal);
  Result^.Immediate.RealVal := Value;
  Result^.TypePtr := PrimitiveTypes.PtReal
end;

function ExCharConstant(Value : char) : TSExpression;
begin
  Result := _ExImmediate(SeicChar);
  Result^.Immediate.CharVal := Value;
  Result^.TypePtr := PrimitiveTypes.PtChar
end;

function ExStringConstant(Value : string) : TSExpression;
begin
  Result := _ExImmediate(SeicString);
  Result^.Immediate.StringVal := Value;
  Result^.TypePtr := PrimitiveTypes.PtString
end;

function ExEnumConstant(Ordinal : integer; TypePtr : TSDType) : TSExpression;
begin
  EnsureEnumType(TypePtr);
  if (Ordinal < 0) or (Ordinal > TypePtr^.EnumPtr^.Size - 1) then
    CompileError('Invalid value for ' + TypeName(TypePtr));
  Result := _ExImmediate(SeicEnum);
  Result^.Immediate.EnumOrdinal := Ordinal;
  Result^.Immediate.EnumPtr := TypePtr^.EnumPtr;
  Result^.TypePtr := TypePtr
end;

function ExSetConstant(Bounds : TSESetImmBounds;
                       TypePtr : TSDType) : TSExpression;
var ElementType : TSDType;
begin
  ElementType := TypePtr^.ElementTypePtr;
  if ElementType <> nil then EnsureOrdinalType(ElementType);
  Result := _ExImmediate(SeicSet);
  Result^.Immediate.SetBounds := Bounds;
  Result^.Immediate.SetOfTypePtr := ElementType;
  Result^.TypePtr := TypePtr
end;

function ExSetAddBounds(Bounds : TSESetImmBounds;
                        First, Last : integer) : TSESetImmBounds;
var 
  Prev, This, NewBounds : TSESetImmBounds;
  Done : boolean;
begin
  if First > Last then
    CompileError('Set bounds must appear in ascending order');
  Result := Bounds;
  Done := false;
  Prev := nil;
  This := Bounds;
  repeat
    {         nil  or         pppppp }
    { nnnnnnn          nnnnnn        }
    if (This = nil) or (Last + 1 < This^.First) then
    begin
      new(NewBounds);
      NewBounds^.First := First;
      NewBounds^.Last := Last;
      NewBounds^.Next := This;
      if Prev = nil then Result := NewBounds
      else Prev^.Next := NewBounds;
      Done := true
    end
    {      ppppp  or      ppppp  or     ppppp }
    {  nnnn           nnnnnn         nnnnnnnn }
    else if (First < This^.First) and (Last <= This^.Last) then
    begin
      This^.First := First;
      Done := true
    end
    { pppppppp  or  pppppppp  or  pppppppp  or  pppppppp }
    { nnnn              nnnn        nnnn        nnnnnnnn }
    else if (First >= This^.First) and (Last <= This^.Last) then
    begin
      Done := true
    end
    {    ppppppp   or   pppppp   }
    {  nnnnnnnnnn       nnnnnnnn }
    else if (First <= This^.First) and (Last > This^.Last) then
    begin
      NewBounds := This^.Next;
      Dispose(This);
      This := NewBounds;
      if Prev = nil then Result := NewBounds
      else Prev^.Next := NewBounds;
      Done := false
    end
    { pppppp     or  ppppp       }
    {   nnnnnnn           nnnnnn }
    else if (First > This^.First) and (First <= This^.Last + 1)
            and (Last > This^.Last) then
    begin
      First := This^.First;
      NewBounds := This^.Next;
      Dispose(This);
      This := NewBounds;
      if Prev = nil then Result := NewBounds
      else Prev^.Next := NewBounds;
      Done := false
    end
    { ppppppp       }
    {         nnnnn }
    else
    begin
      Prev := This;
      This := This^.Next;
      Done := false
    end
  until Done;
end;

function ExIsImmediate(Expr : TSExpression) : boolean;
begin
  Result := Expr^.Cls = SecImmediate
end;

function ExIsImmediateOfClass(Expr : TSExpression;
                              Cls : TSEImmediateClass) : boolean;
begin
  Result := (Expr^.Cls = SecImmediate) and (Expr^.Immediate.Cls = Cls)
end;

function ExArrayValue(TypePtr : TSDType; Elems : TSEArrayElem) : TSExpression;
begin
  Result := _NewExpr(SecArrayValue);
  Result^.ArrayElem := Elems;
  Result^.TypePtr := TypePtr
end;

function ExRecordValue(TypePtr : TSDType;
                       Fields : TSERecordField) : TSExpression;
begin
  Result := _NewExpr(SecRecordValue);
  Result^.RecordField := Fields;
  Result^.TypePtr := TypePtr
end;

function ExSet : TSExpression;
begin
  Result := _ExImmediate(SeicSet);
  Result^.Immediate.SetBounds := nil;
  Result^.Immediate.SetOfTypePtr := nil;
  Result^.TypePtr := PrimitiveTypes.PtEmptySet
end;

function ExSetAddRange(SetExpr : TSExpression;
                       First, Last : TSExpression) : TSExpression;
var 
  ElementTypePtr : TSDType;
  ImmSet : TSExpression;
  ExprSet : TSExpression;
  NewBounds : TSESetExprBounds;
begin
  ElementTypePtr := SetExpr^.TypePtr^.ElementTypePtr;
  if ElementTypePtr = nil then
  begin
    ElementTypePtr := GetFundamentalType(First^.TypePtr);
    SetExpr^.Immediate.SetOfTypePtr := ElementTypePtr;
    SetExpr^.TypePtr := MakeSetType(First^.TypePtr)
  end;
  if not IsFundamentallySameType(First^.TypePtr, ElementTypePtr) then
    ErrorForExpr('Cannot add element to set ' +
                 ErrorDescribeExpr(SetExpr), First);
  if (Last <> nil)
     and not IsFundamentallySameType(Last^.TypePtr, ElementTypePtr)
     and not IsSameType(First^.TypePtr, Last^.TypePtr) then
    ErrorForExpr('Cannot add element to set ' +
                 ErrorDescribeExpr(SetExpr), Last);

  if ExIsImmediate(SetExpr) then
  begin
    ImmSet := SetExpr;
    ExprSet := nil
  end
  else
  begin
    ImmSet := SetExpr^.SetBase;
    ExprSet := SetExpr
  end;
  if ExIsImmediate(First) and (Last = nil) then Last := ExCopy(First);
  if ExIsImmediate(First) and ExIsImmediate(Last) then
  begin
    with ImmSet^.Immediate do
      SetBounds := ExSetAddBounds(SetBounds,
                   ExGetOrdinal(First), ExGetOrdinal(Last));
    ExDispose(First);
    ExDispose(Last)
  end
  else
  begin
    if ExprSet = nil then
    begin
      ExprSet := _NewExpr(SecSetValue);
      ExprSet^.SetBase := ImmSet;
      ExprSet^.SetBounds := nil;
      ExprSet^.TypePtr := ImmSet^.TypePtr;
    end;
    new(NewBounds);
    NewBounds^.First := First;
    NewBounds^.Last := Last;
    NewBounds^.Next := ExprSet^.SetBounds;
    ExprSet^.SetBounds := NewBounds;
  end;
  if ExprSet <> nil then Result := ExprSet
  else Result := ImmSet
end;

function ExToString(Parent : TSExpression) : TSExpression;
var Str : string;
begin
  EnsureStringyExpr(Parent);
  if IsCharType(Parent^.TypePtr) then
  begin
    if ExIsImmediate(Parent) then
    begin
      Str := Parent^.Immediate.CharVal;
      Parent^.Immediate.Cls := SeicString;
      Parent^.Immediate.StringVal := Str;
      Result := Parent
    end
    else
    begin
      Result := _NewExpr(SecToString);
      Result^.ToStrParent := Parent;
      Result^.TypePtr := PrimitiveTypes.PtString;
      Result^.IsFunctionResult := Parent^.IsFunctionResult
    end
  end
  else  Result := Parent
end;

function ExToReal(Parent : TSExpression) : TSExpression;
var Value : real;
begin
  if ExIsImmediate(Parent) then
  begin
    Value := Parent^.Immediate.IntegerVal;
    Parent^.Immediate.Cls := SeicReal;
    Parent^.Immediate.RealVal := Value;
    Parent^.TypePtr := PrimitiveTypes.PtReal;
    Result := Parent
  end
  else
  begin
    Result := _NewExpr(SecToReal);
    Result^.ToRealParent := Parent;
    Result^.TypePtr := PrimitiveTypes.PtReal;
    Result^.IsFunctionResult := Parent^.IsFunctionResult
  end;
end;

function ExToUntypedPtr(Parent : TSExpression) : TSExpression;
begin
  Result := _NewExpr(SecToUntypedPtr);
  Result^.ToUntypedPtrParent := Parent;
  Result^.TypePtr := PrimitiveTypes.PtUntypedPtr;
  Result^.IsFunctionResult := Parent^.IsFunctionResult;
  Result^.IsAssignable := Parent^.IsAssignable;
  Result^.IsAddressable := Parent^.IsAddressable
end;

function ExToGenericFile(Parent : TSExpression) : TSExpression;
begin
  Result := _NewExpr(SecToGenericFile);
  Result^.ToGenericFileParent := Parent;
  Result^.TypePtr := PrimitiveTypes.PtFile;
  Result^.IsFunctionResult := Parent^.IsFunctionResult;
  Result^.IsAssignable := Parent^.IsAssignable;
  Result^.IsAddressable := Parent^.IsAddressable
end;

function ExWithTmpVar(TmpVar, Value, Child : TSExpression) : TSExpression;
begin
  Result := _NewExpr(SecWithTmpVar);
  Result^.TmpVar := TmpVar;
  Result^.TmpVarValue := Value;
  Result^.TmpVarChild := Child;
  Result^.TypePtr := Child^.TypePtr;
  Result^.IsAssignable := Child^.IsAssignable;
  Result^.IsAddressable := Child^.IsAddressable;
  Result^.IsFunctionResult := Child^.IsFunctionResult;
  TmpVar^.VarPtr^.WasInitialized := true;
  TmpVar^.VarPtr^.WasUsed := true
end;

function ExSubrange(Parent : TSExpression; TypePtr : TSDType)
: TSExpression;
forward;

function ExOutrange(Expr : TSExpression) : TSExpression;
var TmpExpr : TSExpression;
begin
  while Expr^.Cls = SecSubrange do
  begin
    TmpExpr := ExCopy(Expr^.SubrangeParent);
    ExDispose(Expr);
    Expr := TmpExpr;
  end;
  Expr^.TypePtr := GetFundamentalType(Expr^.TypePtr);
  Result := Expr
end;

function ExVariable(VarPtr : TSDVariable) : TSExpression;
begin
  Result := _NewExpr(SecVariable);
  Result^.VarPtr := VarPtr;
  Result^.TypePtr := VarPtr^.TypePtr;
  Result^.IsAssignable := not VarPtr^.IsConstant;
  Result^.IsAddressable := true
end;

function ExFieldAccess(Parent : TSExpression; FieldNum : integer)
: TSExpression;
begin
  EnsureRecordExpr(Parent);
  if (FieldNum < 1)
     or (FieldNum > Parent^.TypePtr^.RecPtr^.Size) then
    CompileError('Invalid field for type ' + TypeName(Parent^.TypePtr));
  Result := _NewExpr(SecField);
  Result^.RecExpr := Parent;
  Result^.RecFieldNum := FieldNum;
  Result^.TypePtr := Parent^.TypePtr^.RecPtr^
                     .Fields[FieldNum].TypePtr;
  Result^.IsAssignable := Parent^.IsAssignable;
  Result^.IsAddressable := Parent^.IsAddressable;
  Result^.IsFunctionResult := Parent^.IsFunctionResult
end;

function ExArrayAccess(Parent, Subscript : TSExpression) : TSExpression;
begin
  EnsureArrayExpr(Parent);
  Result := _NewExpr(SecArray);
  Result^.ArrayExpr := Parent;
  Result^.ArrayIndex := ExCoerce(Subscript,
                        Parent^.TypePtr^.ArrayDef.IndexTypePtr);
  Result^.TypePtr := Parent^.TypePtr^.ArrayDef.ValueTypePtr;
  Result^.IsAssignable := Parent^.IsAssignable;
  Result^.IsAddressable := Parent^.IsAddressable;
  Result^.IsFunctionResult := Parent^.IsFunctionResult
end;

function ExPointerAccess(Parent : TSExpression) : TSExpression;
begin
  EnsurePointerExpr(Parent);
  Result := _NewExpr(SecPointer);
  Result^.PointerExpr := Parent;
  Result^.TypePtr := Parent^.TypePtr^.PointedTypePtr;
  Result^.IsAssignable := true;
  Result^.IsAddressable := true;
  Result^.IsFunctionResult := Parent^.IsFunctionResult
end;

function ExAddressOf(Parent : TSExpression) : TSExpression;
begin
  Result := _NewExpr(SecAddress);
  Result^.AddressExpr := Parent;
  if Parent^.Cls = SecFnRef then
    Result^.TypePtr := MakeFunctionType(Parent^.FnPtr^.Args,
                       Parent^.FnPtr^.ReturnTypePtr)
  else
  begin
    EnsureAddressableExpr(Parent);
    EnsureAssignableExpr(Parent);
    Result^.TypePtr := MakePointerType(Parent^.TypePtr)
  end
end;

function ExStringChar(Parent, Subscript : TSExpression) : TSExpression;
begin
  EnsureStringyExpr(Parent);
  EnsureIntegerExpr(Subscript);
  Result := _NewExpr(SecStringChar);
  Result^.ArrayExpr := ExToString(Parent);
  Result^.ArrayIndex := Subscript;
  Result^.TypePtr := PrimitiveTypes.PtChar;
  Result^.IsAssignable := Parent^.IsAssignable;
  Result^.IsAddressable := Parent^.IsAddressable;
  Result^.IsFunctionResult := Parent^.IsFunctionResult
end;

function ExFnRef(FnPtr : TSDSubroutine) : TSExpression;
begin
  Result := _NewExpr(SecFnRef);
  Result^.FnPtr := FnPtr;
  Result^.TypePtr := nil
end;

function _ExFunctionCall(FnExpr : TSExpression;
                         const ArgDefs : TSDSubroutineArgs;
                         ReturnTypePtr : TSDType;
                         const Args : TSEFunctionArgs) : TSExpression;
var 
  Pos : integer;
  FnCall : TSExpression;
begin
  if Args.Size <> ArgDefs.Count then
    CompileError('Wrong number of arguments in call to ' + ExDescribe(FnExpr));
  FnCall := _NewExpr(SecFnCall);
  FnCall^.FnExpr := FnExpr;
  FnCall^.CallArgs.Size := Args.Size;
  FnCall^.TypePtr := ReturnTypePtr;
  FnCall^.IsFunctionResult := true;
  FnCall^.IsStatement := true;
  Result := FnCall;
  for Pos := 1 to Args.Size do
  begin
    FnCall^.CallArgs.Values[Pos] := ExCoerce(Args.Values[Pos],
                                    ArgDefs.Defs[Pos].TypePtr);
    if ArgDefs.Defs[Pos].IsReference then
    begin
      if not FnCall^.CallArgs.Values[Pos]^.IsAddressable then
      begin
        if ArgDefs.Defs[Pos].IsConstant then
        begin
          Result := ExWithTmpVar(ExVariable(AddAliasVariable(
                    ArgDefs.Defs[Pos].TypePtr,
                    FnCall^.CallArgs.Values[Pos])),
                    FnCall^.CallArgs.Values[Pos], Result);
          FnCall^.CallArgs.Values[Pos] := ExCopy(Result^.TmpVar)
        end
        else
          CompileError('Pass-by-reference argument must be assignable: ' +
                       ExDescribe(FnCall^.CallArgs.Values[Pos]))
      end
      else if not ArgDefs.Defs[Pos].IsConstant then
      begin
        EnsureAssignableExpr(FnCall^.CallArgs.Values[Pos]);
        ExMarkInitialized(FnCall^.CallArgs.Values[Pos])
      end
    end
  end
end;

function ExFunctionCall(FnExpr : TSExpression;
                        const Args : TSEFunctionArgs) : TSExpression;
begin
  if FnExpr^.Cls = SecFnRef then
    Result := _ExFunctionCall(FnExpr, FnExpr^.FnPtr^.Args,
              FnExpr^.FnPtr^.ReturnTypePtr, Args)
  else if IsFunctionType(FnExpr^.TypePtr) then
         Result := _ExFunctionCall(FnExpr, FnExpr^.TypePtr^.FnDefPtr^.Args,
                   FnExpr^.TypePtr^.FnDefPtr^.ReturnTypePtr, Args)
  else
    ErrorForExpr('Cannot call non-function', FnExpr)
end;

function ExPsfn(SpecialFn : TSDPsfn) : TSExpression;
begin
  Result := _NewExpr(SecPsfnRef);
  Result^.TypePtr := nil;
  Result^.PsfnPtr := SpecialFn
end;

function ExSizeof(TypePtr : TSDType) : TSExpression;
begin
  Result := _NewExpr(SecSizeof);
  Result^.SizeofTypePtr := TypePtr;
  Result^.TypePtr := PrimitiveTypes.PtInteger
end;

function ExConvertToStr(Src, Width, Prec, Dest : TSExpression) : TSExpression;
begin
  Result := _NewExpr(SecConvertToStr);
  Result^.TypePtr := nil;
  Result^.ToStrSrc.Arg := Src;
  Result^.ToStrSrc.Width := Width;
  Result^.ToStrSrc.Prec := Prec;
  Result^.ToStrDest := Dest;
  Result^.IsStatement := true
end;

function ExConvertToVal(Src, Dest, Code : TSExpression) : TSExpression;
begin
  Result := _NewExpr(SecConvertToVal);
  Result^.TypePtr := nil;
  Result^.ToValSrc := Src;
  Result^.ToValDest := Dest;
  Result^.ToValCode := Code;
  Result^.IsStatement := true
end;

function ExRead(ReadFile : TSExpression; Args : TSEReadArgList;
                NewLine : boolean; CheckIoResult : boolean) : TSExpression;
begin
  Result := _NewExpr(SecRead);
  Result^.TypePtr := nil;
  Result^.ReadFile := ReadFile;
  Result^.ReadArgs := Args;
  Result^.ReadLn := NewLine;
  Result^.CheckIoResultAfterRead := CheckIoResult;
  Result^.IsStatement := true
end;

function ExWrite(WriteFile : TSExpression; Args : TSEWriteArgList;
                 NewLine : boolean; CheckIoResult : boolean) : TSExpression;
begin
  Result := _NewExpr(SecWrite);
  Result^.TypePtr := nil;
  Result^.WriteFile := WriteFile;
  Result^.WriteArgs := Args;
  Result^.WriteLn := NewLine;
  Result^.CheckIoResultAfterWrite := CheckIoResult;
  Result^.IsStatement := true
end;

function ExGetOrdinal(Expr : TSExpression) : integer;
begin
  if not ExIsImmediate(Expr) then
    ErrorForExpr('Expected an immediate value', Expr);
  with Expr^.Immediate do
    case Cls of 
      SeicBoolean : Result := Ord(BooleanVal);
      SeicInteger : Result := IntegerVal;
      SeicChar : Result := Ord(CharVal);
      SeicEnum : Result := EnumOrdinal;
      else ErrorForExpr('Expected an ordinal', Expr)
    end
end;

function ExGetAntiOrdinal(Ordinal : integer;
                          TypePtr : TSDType) : TSExpression;
begin
  TypePtr := GetFundamentalType(TypePtr);
  case TypePtr^.Cls of 
    SdtcBoolean: if Ordinal = 0 then Result := ExBooleanConstant(false)
                 else Result := ExBooleanConstant(true);
    SdtcInteger: Result := ExIntegerConstant(Ordinal);
    SdtcChar: Result := ExCharConstant(Chr(Ordinal));
    SdtcEnum: Result := ExEnumConstant(Ordinal, TypePtr);
    else InternalError('Cannot compute anti-ordinal for ' + TypeName(TypePtr))
  end
end;

function ExSubrange;
var Ordinal : integer;
begin
  if ExIsImmediate(Parent) then
  begin
    Ordinal := ExGetOrdinal(Parent);
    if (Ordinal < TypePtr^.RangeDef.First)
       or (Ordinal > TypePtr^.RangeDef.Last) then
      CompileError('Value ' + ExDescribe(Parent) +
      ' out of bounds for ' + TypeName(TypePtr));
    Parent^.TypePtr := TypePtr;
    Result := Parent
  end
  else
  begin
    Result := _NewExpr(SecSubrange);
    Result^.SubrangeParent := Parent;
    Result^.TypePtr := TypePtr
  end
end;

function ExRerange(Expr : TSExpression; TypePtr : TSDType) : TSExpression;
begin
  if (TypePtr^.RangeDef.First <= Expr^.TypePtr^.RangeDef.First)
     and (Expr^.TypePtr^.RangeDef.Last <= TypePtr^.RangeDef.Last) then
  begin
    Expr^.TypePtr := TypePtr;
    Result := Expr
  end
  else
    Result := ExSubrange(ExOutrange(Expr), TypePtr)
end;

procedure ExSetCoerceToCommon(Left, Right : TSExpression);
var LeftType, RightType : TSDType;
begin
  LeftType := Left^.TypePtr;
  RightType := Right^.TypePtr;
  if (LeftType^.ElementTypePtr = nil) or ExIsImmediate(Left) then
    LeftType^.ElementTypePtr := RightType^.ElementTypePtr
  else if (RightType^.ElementTypePtr = nil) or ExIsImmediate(Right) then
         RightType^.ElementTypePtr := LeftType^.ElementTypePtr
  else if not IsSameType(LeftType, RightType) then
         CompileError('Type mismatch: cannot combine ' +
                      TypeName(Left^.TypePtr) + ' with ' +
         TypeName(Right^.TypePtr))
end;

function _ExCoerceSet(Expr : TSExpression; TypePtr : TSDType) : TSExpression;
var 
  Outcome : (Pass, Reject, Replace);
  ExprElemType, DestElemType : TSDType;
begin
  ExprElemType := Expr^.TypePtr^.ElementTypePtr;
  DestElemType := TypePtr^.ElementTypePtr;
  if ExprElemType = nil then Outcome := Replace
  else if not IsFundamentallySameType(ExprElemType, DestElemType) then
         Outcome := Reject
  else if ExIsImmediate(Expr) then Outcome := Replace
  else if Expr^.Cls = SecSetValue then Outcome := Replace
  else if (GetTypeLowBound(ExprElemType) = GetTypeLowBound(DestElemType))
          and (GetTypeHighBound(ExprElemType) = GetTypeHighBound(DestElemType))
         then Outcome := Pass
  else Outcome := Reject;

  case Outcome of 
    Reject : ErrorForExpr('Cannot treat set as ' + TypeName(TypePtr),
             Expr);
    Replace :
              begin
                Expr^.TypePtr := TypePtr;
                if Expr^.Cls = SecSetValue then
                  Expr^.SetBase^.TypePtr := TypePtr
              end;
    Pass : { do nothing };
  end;
  Result := Expr
end;

function ExCoerce;
begin
  if IsFundamentallySameType(Expr^.TypePtr, TypePtr) then
  begin
    if IsRangeType(Expr^.TypePtr) and IsRangeType(TypePtr) then
      ExCoerce := ExRerange(Expr, TypePtr)
    else if IsRangeType(Expr^.TypePtr) then
           ExCoerce := ExOutrange(Expr)
    else if IsRangeType(TypePtr) then
           ExCoerce := ExSubrange(Expr, TypePtr)
    else
      ExCoerce := Expr
  end
  else if IsCharType(Expr^.TypePtr) and IsStringType(TypePtr) then
         ExCoerce := ExToString(Expr)
  else if IsIntegerType(Expr^.TypePtr) and IsRealType(TypePtr) then
         ExCoerce := ExToReal(Expr)
  else if IsNilType(Expr^.TypePtr) and IsPointeryType(TypePtr) then
         ExCoerce := Expr
  else if IsNilType(Expr^.TypePtr) and IsFunctionyType(TypePtr) then
         ExCoerce := Expr
  else if IsPointeryType(Expr^.TypePtr) and IsUntypedPtrType(TypePtr) then
         ExCoerce := ExToUntypedPtr(Expr)
  else if IsSetType(Expr^.TypePtr) and IsSetType(TypePtr) then
         ExCoerce := _ExCoerceSet(Expr, TypePtr)
  else if IsFileType(Expr^.TypePtr) and IsGenericFileType(TypePtr) then
         ExCoerce := ExToGenericFile(Expr)
  else if IsUntyped(TypePtr) then
         ExCoerce := Expr
  else
    ErrorForExpr('Cannot treat value as ' + TypeName(TypePtr), Expr)
end;

procedure ExMarkInitialized(Lhs : TSExpression);
var IsTerminal : boolean;
begin
  IsTerminal := false;
  while not IsTerminal do
    case Lhs^.Cls of 
      SecField: Lhs := Lhs^.RecExpr;
      SecArray: Lhs := Lhs^.ArrayExpr;
      SecStringChar: Lhs := Lhs^.StringExpr;
      SecToString: Lhs := Lhs^.ToStrParent;
      SecToReal: Lhs := Lhs^.ToRealParent;
      SecToGenericFile: Lhs := Lhs^.ToGenericFileParent;
      else IsTerminal := true
    end;
  if Lhs^.Cls = SecVariable then
  begin
    Lhs^.VarPtr^.WasInitialized := true;
    if Lhs^.VarPtr^.IsReference then Lhs^.VarPtr^.WasUsed := true
  end
end;

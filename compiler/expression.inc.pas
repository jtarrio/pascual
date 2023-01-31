function _NewExpr(Cls : TExpressionClass) : TExpression;
var Expr : TExpression;
begin
  new(Expr);
  Expr^.Cls := Cls;
  Expr^.IsConstant := false;
  Expr^.IsAssignable := false;
  Expr^.IsFunctionResult := false;
  _NewExpr := Expr
end;

procedure DisposeExpr(var Expr : TExpression);
var Pos : integer;
begin
  case Expr^.Cls of 
    XcToString : DisposeExpr(Expr^.ToStringEx.Parent);
    XcFieldAccess : DisposeExpr(Expr^.FieldEx.Parent);
    XcArrayAccess :
                    begin
                      DisposeExpr(Expr^.ArrayEx.Parent);
                      DisposeExpr(Expr^.ArrayEx.Subscript);
                    end;
    XcPointerAccess : DisposeExpr(Expr^.PointerEx.Parent);
    XcStringChar :
                   begin
                     DisposeExpr(Expr^.StringCharEx.Parent);
                     DisposeExpr(Expr^.StringCharEx.Subscript);
                   end;
    XcFunctionCall :
                     begin
                       DisposeExpr(Expr^.CallEx.FunctionRef);
                       for Pos := 1 to Expr^.CallEx.Args.Size do
                         DisposeExpr(Expr^.CallEx.Args.Values[Pos]);
                     end;
    XcUnaryOp : DisposeExpr(Expr^.UnaryEx.Parent);
    XcBinaryOp :
                 begin
                   DisposeExpr(Expr^.BinaryEx.Left);
                   DisposeExpr(Expr^.BinaryEx.Right);
                 end
  end;
  dispose(Expr);
end;

function CopyExpr(Expr : TExpression) : TExpression;
var 
  Copy : TExpression;
  Pos : integer;
begin
  Copy := _NewExpr(Expr^.Cls);
  Copy^.TypeIndex := Expr^.TypeIndex;
  Copy^.IsConstant := Expr^.IsConstant;
  Copy^.IsAssignable := Expr^.IsAssignable;
  Copy^.IsFunctionResult := Expr^.IsFunctionResult;
  case Expr^.Cls of 
    XcImmediate: Copy^.ImmediateEx := Expr^.ImmediateEx;
    XcToString: Copy^.ToStringEx.Parent := CopyExpr(Expr^.ToStringEx.Parent);
    XcVariableAccess: Copy^.VariableEx := Expr^.VariableEx;
    XcFieldAccess:
                   begin
                     Copy^.FieldEx.Parent := CopyExpr(Expr^.FieldEx.Parent);
                     Copy^.FieldEx.FieldNumber := Expr^.FieldEx.FieldNumber
                   end;
    XcArrayAccess:
                   begin
                     Copy^.ArrayEx.Parent := CopyExpr(Expr^.ArrayEx.Parent);
                     Copy^.ArrayEx.Subscript := CopyExpr(Expr^.ArrayEx
                                                .Subscript)
                   end;
    XcPointerAccess: Copy^.PointerEx.Parent := CopyExpr(Expr^.PointerEx.Parent);
    XcStringChar:
                  begin
                    Copy^.StringCharEx.Parent := CopyExpr(Expr^.StringCharEx
                                                 .Parent);
                    Copy^.StringCharEx.Subscript := CopyExpr(Expr^.StringCharEx
                                                    .Subscript)
                  end;
    XcFunctionRef:
                   Copy^.FunctionEx.FunctionIndex := Expr^.FunctionEx
                                                     .FunctionIndex;
    XcFunctionCall:
                    begin
                      Copy^.CallEx.FunctionRef := CopyExpr(Expr^.CallEx
                                                  .FunctionRef);
                      Copy^.CallEx.Args.Size := Expr^.CallEx.Args.Size;
                      for Pos := 1 to Expr^.CallEx.Args.Size do
                        Copy^.CallEx.Args.Values[Pos] := CopyExpr(Expr^.CallEx
                                                         .Args.Values[Pos])
                    end;
    XcUnaryOp:
               begin
                 Copy^.UnaryEx.Parent := CopyExpr(Expr^.UnaryEx.Parent);
                 Copy^.UnaryEx.Op := Expr^.UnaryEx.Op
               end;
    XcBinaryOp:
                begin
                  Copy^.BinaryEx.Left := CopyExpr(Expr^.BinaryEx.Left);
                  Copy^.BinaryEx.Right := CopyExpr(Expr^.BinaryEx.Right);
                  Copy^.BinaryEx.Op := Expr^.BinaryEx.Op
                end;
  end;
  CopyExpr := Copy
end;

function ExCoerce(Expr : TExpression; TypeIndex : TPsTypeIndex) : TExpression;
forward;

function ExEnsureEvaluation(Expr : TExpression) : TExpression;
forward;

function ExNothing : TExpression;
var Expr : TExpression;
begin
  Expr := _NewExpr(XcNothing);
  Expr^.TypeIndex := nil;
  ExNothing := Expr
end;

function _ExImmediate(Cls : TExImmediateClass) : TExpression;
var Expr : TExpression;
begin
  Expr := _NewExpr(XcImmediate);
  Expr^.IsConstant := true;
  Expr^.ImmediateEx.Cls := Cls;
  _ExImmediate := Expr
end;

function ExNil : TExpression;
var Expr : TExpression;
begin
  Expr := _ExImmediate(XicNil);
  Expr^.TypeIndex := PrimitiveTypes.PtNil;
  ExNil := Expr
end;

function ExBooleanConstant(Value : boolean) : TExpression;
var Expr : TExpression;
begin
  Expr := _ExImmediate(XicBoolean);
  Expr^.ImmediateEx.BooleanValue := Value;
  Expr^.TypeIndex := PrimitiveTypes.PtBoolean;
  ExBooleanConstant := Expr
end;

function ExIntegerConstant(Value : integer) : TExpression;
var Expr : TExpression;
begin
  Expr := _ExImmediate(XicInteger);
  Expr^.ImmediateEx.IntegerValue := Value;
  Expr^.TypeIndex := PrimitiveTypes.PtInteger;
  ExIntegerConstant := Expr
end;

function ExCharConstant(Value : char) : TExpression;
var Expr : TExpression;
begin
  Expr := _ExImmediate(XicChar);
  Expr^.ImmediateEx.CharValue := Value;
  Expr^.TypeIndex := PrimitiveTypes.PtChar;
  ExCharConstant := Expr
end;

function ExStringConstant(Value : string) : TExpression;
var Expr : TExpression;
begin
  Expr := _ExImmediate(XicString);
  Expr^.ImmediateEx.StringValue := Value;
  Expr^.TypeIndex := PrimitiveTypes.PtString;
  ExStringConstant := Expr
end;

function ExEnumConstant(Ordinal : integer; TypeIndex : TPsTypeIndex)
: TExpression;
var Expr : TExpression;
begin
  if not IsEnumType(TypeIndex) then
    CompileError('Not an enumeration type: ' + TypeName(TypeIndex));
  if (Ordinal < 1) or (Ordinal > TypeIndex^.EnumIndex^.Size) then
    CompileError('Invalid value for ' + TypeName(TypeIndex));
  Expr := _ExImmediate(XicEnum);
  Expr^.ImmediateEx.EnumOrdinal := Ordinal;
  Expr^.TypeIndex := TypeIndex;
  ExEnumConstant := Expr
end;

function ExToString(Parent : TExpression) : TExpression;
var 
  Expr : TExpression;
  Str : string;
begin
  Parent := ExEnsureEvaluation(Parent);
  if IsCharType(Parent^.TypeIndex) then
  begin
    if Parent^.Cls = XcImmediate then
    begin
      Str := Parent^.ImmediateEx.CharValue;
      Parent^.ImmediateEx.Cls := XicString;
      Parent^.ImmediateEx.StringValue := Str;
      ExToString := Parent
    end
    else
    begin
      Expr := _NewExpr(XcToString);
      Expr^.ToStringEx.Parent := Parent;
      Expr^.TypeIndex := PrimitiveTypes.PtString;
      Expr^.IsAssignable := false;
      Expr^.IsFunctionResult := Parent^.IsFunctionResult;
      ExToString := Expr
    end
  end
  else if IsStringType(Parent^.TypeIndex) then ExToString := Parent
  else CompileError('Cannot convert a value of this type to string: ' +
                    TypeName(Parent^.TypeIndex))
end;

function ExVariable(VariableIndex : TPsVariableIndex) : TExpression;
var Expr : TExpression;
begin
  Expr := _NewExpr(XcVariableAccess);
  Expr^.VariableEx.VariableIndex := VariableIndex;
  Expr^.TypeIndex := VariableIndex^.TypeIndex;
  Expr^.IsConstant := VariableIndex^.IsConstant;
  Expr^.IsAssignable := true;
  ExVariable := Expr
end;

function ExFieldAccess(Parent : TExpression; FieldNumber : integer)
: TExpression;
var Expr : TExpression;
begin
  Parent := ExEnsureEvaluation(Parent);
  if not IsRecordType(Parent^.TypeIndex) then
    CompileError('Cannot access field of non-record type ' +
                 TypeName(Parent^.TypeIndex));
  if (FieldNumber < 1)
     or (FieldNumber > Parent^.TypeIndex^.RecordIndex^.Size) then
    CompileError('Invalid field for ' + TypeName(Parent^.TypeIndex));
  Expr := _NewExpr(XcFieldAccess);
  Expr^.FieldEx.Parent := Parent;
  Expr^.FieldEx.FieldNumber := FieldNumber;
  Expr^.TypeIndex := Parent^.TypeIndex^.RecordIndex^
                     .Fields[FieldNumber].TypeIndex;
  Expr^.IsConstant := Parent^.IsConstant;
  Expr^.IsAssignable := Parent^.IsAssignable;
  Expr^.IsFunctionResult := Parent^.IsFunctionResult;
  ExFieldAccess := Expr
end;

function ExArrayAccess(Parent : TExpression; Subscript : TExpression)
: TExpression;
var Expr : TExpression;
begin
  Parent := ExEnsureEvaluation(Parent);
  if not IsArrayType(Parent^.TypeIndex) then
    CompileError('Cannot access subscript of non-array type ' +
                 TypeName(Parent^.TypeIndex));
  if not IsIntegerType(Subscript^.TypeIndex) then
    CompileError('Invalid type for subscript of ' +
                 TypeName(Parent^.TypeIndex) + ': ' +
    TypeName(Subscript^.TypeIndex));
  Expr := _NewExpr(XcArrayAccess);
  Expr^.ArrayEx.Parent := Parent;
  Expr^.ArrayEx.Subscript := Subscript;
  Expr^.TypeIndex := Parent^.TypeIndex^.ArrayIndex^.TypeIndex;
  Expr^.IsConstant := Parent^.IsConstant;
  Expr^.IsAssignable := Parent^.IsAssignable;
  Expr^.IsFunctionResult := Parent^.IsFunctionResult;
  ExArrayAccess := Expr
end;

function ExPointerAccess(Parent : TExpression) : TExpression;
var Expr : TExpression;
begin
  Parent := ExEnsureEvaluation(Parent);
  if not IsPointerType(Parent^.TypeIndex) then
    CompileError('Cannot dereference non-pointer type ' +
                 TypeName(Parent^.TypeIndex));
  Expr := _NewExpr(XcPointerAccess);
  Expr^.PointerEx.Parent := Parent;
  Expr^.TypeIndex := Parent^.TypeIndex^.PointedTypeIndex;
  Expr^.IsConstant := false;
  Expr^.IsAssignable := true;
  Expr^.IsFunctionResult := Parent^.IsFunctionResult;
  ExPointerAccess := Expr
end;

function ExStringChar(Parent : TExpression; Subscript : TExpression)
: TExpression;
var Expr : TExpression;
begin
  Parent := ExEnsureEvaluation(Parent);
  if not IsStringyType(Parent^.TypeIndex) then
    CompileError('Cannot access subscript of non-string type ' +
                 TypeName(Parent^.TypeIndex));
  if not IsIntegerType(Subscript^.TypeIndex) then
    CompileError('Invalid type for subscript of string: ' +
                 TypeName(Subscript^.TypeIndex));
  Expr := _NewExpr(XcStringChar);
  Expr^.ArrayEx.Parent := ExToString(Parent);
  Expr^.ArrayEx.Subscript := Subscript;
  Expr^.TypeIndex := PrimitiveTypes.PtChar;
  Expr^.IsConstant := Parent^.IsConstant;
  Expr^.IsAssignable := Parent^.IsAssignable;
  Expr^.IsFunctionResult := Parent^.IsFunctionResult;
  ExStringChar := Expr
end;

function ExFunctionRef(FunctionIndex : TPsFunctionIndex) : TExpression;
var Expr : TExpression;
begin
  Expr := _NewExpr(XcFunctionRef);
  Expr^.FunctionEx.FunctionIndex := FunctionIndex;
  Expr^.TypeIndex := nil;
  Expr^.IsConstant := true;
  ExFunctionRef := Expr
end;

function ExFunctionCall(FunctionRef : TExpression; var Args : TExFunctionArgs)
: TExpression;
var 
  Expr : TExpression;
  Pos : integer;
begin
  if FunctionRef^.Cls <> XcFunctionRef then
    CompileError('Cannot call non-function');
  with FunctionRef^.FunctionEx do
  begin
    if Args.Size <> FunctionIndex^.ArgCount then
      CompileError('Wrong number of arguments in function call');
    Expr := _NewExpr(XcFunctionCall);
    Expr^.CallEx.FunctionRef := FunctionRef;
    Expr^.CallEx.Args.Size := Args.Size;
    for Pos := 1 to Args.Size do
    begin
      Expr^.CallEx.Args.Values[Pos] := ExCoerce(
                                       ExEnsureEvaluation(Args.Values[Pos]),
                                       FunctionIndex^.Args[Pos].TypeIndex);
      if FunctionIndex^.Args[Pos].IsReference
         and (Expr^.CallEx.Args.Values[Pos]^.IsConstant
         or not Expr^.CallEx.Args.Values[Pos]^.IsAssignable) then
        CompileError('Pass-by-reference argument must be assignable')
    end;
    Expr^.TypeIndex := FunctionIndex^.ReturnTypeIndex;
  end;
  Expr^.IsConstant := false;
  Expr^.IsAssignable := false;
  Expr^.IsFunctionResult := true;
  ExFunctionCall := Expr
end;

function _ExUnOpImm(Parent : TExpression; Op : TLxTokenId) : TExpression;
forward;
function _ExUnOpCmp(Parent : TExpression; Op : TLxTokenId) : TExpression;
forward;
function ExUnaryOp(Parent : TExpression; Op : TLxTokenId) : TExpression;
begin
  Parent := ExEnsureEvaluation(Parent);
  if (Op = TkMinus) or (Op = TkPlus) then
  begin
    if not IsIntegerType(Parent^.TypeIndex) then
      CompileError('Invalid type for ' + LxTokenName(Op) + ': ' +
      TypeName(Parent^.TypeIndex))
  end
  else if Op = TkNot then
  begin
    if not IsBooleanType(Parent^.TypeIndex)
       and not IsIntegerType(Parent^.TypeIndex) then
      CompileError('Invalid type for ' + LxTokenName(Op) + ': ' +
      TypeName(Parent^.TypeIndex))
  end
  else CompileError('Invalid unary operator: ' + LxTokenName(Op));

  if Parent^.Cls = XcImmediate then ExUnaryOp := _ExUnOpImm(Parent, Op)
  else ExUnaryOp := _ExUnOpCmp(Parent, Op)
end;

function _ExUnOpImm;
begin
  if (Op = TkMinus) and (Parent^.ImmediateEx.Cls = XicInteger) then
    Parent^.ImmediateEx.IntegerValue := -Parent^.ImmediateEx.IntegerValue
  else if (Op = TkPlus) and (Parent^.ImmediateEx.Cls = XicInteger) then
    { do nothing }
  else if (Op = TkNot) and (Parent^.ImmediateEx.Cls = XicBoolean) then
         Parent^.ImmediateEx.BooleanValue := not Parent^.ImmediateEx.
                                             BooleanValue
  else if (Op = TkNot) and (Parent^.ImmediateEx.Cls = XicInteger) then
         Parent^.ImmediateEx.IntegerValue := not Parent^.ImmediateEx.
                                             IntegerValue
  else CompileError('Internal error: invalid immediate unary operation');
  _ExUnOpImm := Parent
end;

function _ExUnOpCmp;
var Expr : TExpression;
begin
  Expr := _NewExpr(XcUnaryOp);
  Expr^.UnaryEx.Parent := Parent;
  Expr^.UnaryEx.Op := Op;
  Expr^.TypeIndex := Parent^.TypeIndex;
  Expr^.IsConstant := true;
  Expr^.IsAssignable := false;
  Expr^.IsFunctionResult := Parent^.IsFunctionResult;
  _ExUnOpCmp := Expr
end;

function _ExBinOpBoolImm(Left : TExpression; Right : TExpression;
                         Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpIntImm(Left : TExpression; Right : TExpression;
                        Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpStrImm(Left : TExpression; Right : TExpression;
                        Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpEnumImm(Left : TExpression; Right : TExpression;
                         Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpBoolCmp(Left : TExpression; Right : TExpression;
                         Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpIntCmp(Left : TExpression; Right : TExpression;
                        Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpStrCmp(Left : TExpression; Right : TExpression;
                        Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpEnumCmp(Left : TExpression; Right : TExpression;
                         Op : TLxTokenId) : TExpression;
forward;
function _ExBinOpPtrCmp(Left : TExpression; Right : TExpression;
                        Op : TLxTokenId) : TExpression;
forward;
function ExBinaryOp(Left : TExpression; Right : TExpression; Op : TLxTokenId)
: TExpression;
var 
  Immediate : boolean;
begin
  Left := ExEnsureEvaluation(Left);
  Right := ExEnsureEvaluation(Right);
  Immediate := (Left^.Cls = XcImmediate) and (Right^.Cls = XcImmediate);
  if IsBooleanType(Left^.TypeIndex) and IsBooleanType(Right^.TypeIndex) then
  begin
    if Immediate then ExBinaryOp := _ExBinOpBoolImm(Left, Right, Op)
    else ExBinaryOp := _ExBinOpBoolCmp(Left, Right, Op)
  end
  else if IsIntegerType(Left^.TypeIndex)
          and IsIntegerType(Right^.TypeIndex) then
  begin
    if Immediate then ExBinaryOp := _ExBinOpIntImm(Left, Right, Op)
    else ExBinaryOp := _ExBinOpIntCmp(Left, Right, Op)
  end
  else if IsStringyType(Left^.TypeIndex)
          and IsStringyType(Right^.TypeIndex) then
  begin
    if Immediate then ExBinaryOp := _ExBinOpStrImm(Left, Right, Op)
    else ExBinaryOp := _ExBinOpStrCmp(Left, Right, Op)
  end
  else if IsEnumType(Left^.TypeIndex)
          and IsSameType(Left^.TypeIndex, Right^.TypeIndex) then
  begin
    if Immediate then ExBinaryOp := _ExBinOpEnumImm(Left, Right, Op)
    else ExBinaryOp := _ExBinOpEnumCmp(Left, Right, Op)
  end
  else if ArePointersCompatible(Left^.TypeIndex, Right^.TypeIndex) then
         ExBinaryOp := _ExBinOpPtrCmp(Left, Right, Op)
  else
    CompileError('Type mismatch for operator ' + LxTokenName(Op) + ': ' +
    TypeName(Left^.TypeIndex) + ' and ' + TypeName(Right^.TypeIndex))
end;

function _ExBinOpBoolImm;
var Lt, Rt : boolean;
begin
  Lt := Left^.ImmediateEx.BooleanValue;
  Rt := Right^.ImmediateEx.BooleanValue;
  DisposeExpr(Right);
  case Op of 
    TkAnd : Lt := Lt and Rt;
    TkOr : Lt := Lt or Rt;
    TkEquals : Lt := Lt = Rt;
    TkNotEquals : Lt := Lt <> Rt;
    TkLessthan : Lt := Lt < Rt;
    TkMorethan : Lt := Lt > Rt;
    TkLessOrEquals : Lt := Lt <= Rt;
    TkMoreOrEquals : Lt := Lt >= Rt;
    else CompileError('Invalid boolean operator: ' + LxTokenName(Op))
  end;
  Left^.ImmediateEx.BooleanValue := Lt;
  Left^.TypeIndex := PrimitiveTypes.PtBoolean;
  _ExBinOpBoolImm := Left
end;

function _ExBinOpIntImm;
var 
  Lt, Rt : integer;
  Bo : boolean;
begin
  Lt := Left^.ImmediateEx.IntegerValue;
  Rt := Right^.ImmediateEx.IntegerValue;
  DisposeExpr(Right);
  case Op of 
    TkPlus : Lt := Lt + Rt;
    TkMinus : Lt := Lt - Rt;
    TkAsterisk : Lt := Lt * Rt;
    TkDiv : Lt := Lt div Rt;
    TkMod : Lt := Lt mod Rt;
    TkAnd : Lt := Lt and Rt;
    TkOr : Lt := Lt or Rt;
    else
    begin
      Left^.ImmediateEx.Cls := XicBoolean;
      case Op of 
        TkEquals : Bo := Lt = Rt;
        TkNotEquals : Bo := Lt <> Rt;
        TkLessthan : Bo := Lt < Rt;
        TkMorethan : Bo := Lt > Rt;
        TkLessOrEquals : Bo := Lt <= Rt;
        TkMoreOrEquals : Bo := Lt >= Rt;
        else CompileError('Invalid integer operator: ' + LxTokenName(Op))
      end
    end
  end;
  if Left^.ImmediateEx.Cls = XicInteger then
  begin
    Left^.ImmediateEx.IntegerValue := Lt;
    Left^.TypeIndex := PrimitiveTypes.PtInteger
  end
  else
  begin
    Left^.ImmediateEx.BooleanValue := Bo;
    Left^.TypeIndex := PrimitiveTypes.PtBoolean
  end;
  _ExBinOpIntImm := Left
end;

function _ExBinOpStrImm;
var 
  Lt, Rt : string;
  Bo : boolean;
begin
  if Left^.ImmediateEx.Cls = XicChar then Lt := Left^.ImmediateEx.CharValue
  else Lt := Left^.ImmediateEx.StringValue;
  if Right^.ImmediateEx.Cls = XicChar then Rt := Right^.ImmediateEx.CharValue
  else Rt := Right^.ImmediateEx.StringValue;
  DisposeExpr(Right);
  if Op = TkPlus then
  begin
    Left^.ImmediateEx.Cls := XicString;
    Lt := Lt + Rt;
  end
  else
  begin
    Left^.ImmediateEx.Cls := XicBoolean;
    case Op of 
      TkEquals : Bo := Lt = Rt;
      TkNotEquals : Bo := Lt <> Rt;
      TkLessthan : Bo := Lt < Rt;
      TkMorethan : Bo := Lt > Rt;
      TkLessOrEquals : Bo := Lt <= Rt;
      TkMoreOrEquals : Bo := Lt >= Rt;
      else CompileError('Invalid string operator: ' + LxTokenName(Op))
    end;
  end;
  if Left^.ImmediateEx.Cls = XicString then
  begin
    Left^.ImmediateEx.StringValue := Lt;
    Left^.TypeIndex := PrimitiveTypes.PtString
  end
  else
  begin
    Left^.ImmediateEx.BooleanValue := Bo;
    Left^.TypeIndex := PrimitiveTypes.PtBoolean
  end;
  _ExBinOpStrImm := Left
end;

function _ExBinOpEnumImm;
var 
  Lt, Rt : integer;
  Bo : boolean;
begin
  Lt := Left^.ImmediateEx.EnumOrdinal;
  Rt := Right^.ImmediateEx.EnumOrdinal;
  DisposeExpr(Right);
  case Op of 
    TkEquals : Bo := Lt = Rt;
    TkNotEquals : Bo := Lt <> Rt;
    TkLessthan : Bo := Lt < Rt;
    TkMorethan : Bo := Lt > Rt;
    TkLessOrEquals : Bo := Lt <= Rt;
    TkMoreOrEquals : Bo := Lt >= Rt;
    else CompileError('Invalid integer operator: ' + LxTokenName(Op))
  end;
  Left^.ImmediateEx.Cls := XicBoolean;
  Left^.ImmediateEx.BooleanValue := Bo;
  Left^.TypeIndex := PrimitiveTypes.PtBoolean;
  _ExBinOpEnumImm := Left
end;

function _ExBinOpBoolCmp;
var Expr : TExpression;
begin
  if (Op = TkAnd) or (Op = TkOr) or (Op = TkEquals) or (Op = TkNotEquals)
     or (Op = TkLessthan) or (Op = TkMorethan) or (Op = TkLessOrEquals)
     or (Op = TkMoreOrEquals) then
  begin
    Expr := _NewExpr(XcBinaryOp);
    Expr^.BinaryEx.Left := Left;
    Expr^.BinaryEx.Right := Right;
    Expr^.BinaryEx.Op := Op;
    Expr^.TypeIndex := PrimitiveTypes.PtBoolean;
    Expr^.IsConstant := true;
    Expr^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
    _ExBinOpBoolCmp := Expr
  end
  else CompileError('Invalid boolean operator: ' + LxTokenName(Op))
end;

function _ExBinOpIntCmp;
var Expr : TExpression;
begin
  Expr := _NewExpr(XcBinaryOp);
  Expr^.BinaryEx.Left := Left;
  Expr^.BinaryEx.Right := Right;
  Expr^.BinaryEx.Op := Op;
  Expr^.IsConstant := true;
  Expr^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
  if (Op = TkPlus) or (Op = TkMinus) or (Op = TkAsterisk) or (Op = TkDiv)
     or (Op = TkMod) or (Op = TkAnd) or (Op = TkOr) then
    Expr^.TypeIndex := PrimitiveTypes.PtInteger
  else if (Op = TkEquals) or (Op = TkNotEquals)
          or (Op = TkLessthan) or (Op = TkMorethan) or (Op = TkLessOrEquals)
          or (Op = TkMoreOrEquals) then
         Expr^.TypeIndex := PrimitiveTypes.PtBoolean
  else CompileError('Invalid integer operator: ' + LxTokenName(Op));
  _ExBinOpIntCmp := Expr
end;

function _ExBinOpStrCmp;
var Expr : TExpression;
begin
  Expr := _NewExpr(XcBinaryOp);
  Expr^.BinaryEx.Left := Left;
  Expr^.BinaryEx.Right := Right;
  Expr^.BinaryEx.Op := Op;
  Expr^.IsConstant := true;
  Expr^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
  if Op = TkPlus then
    Expr^.TypeIndex := PrimitiveTypes.PtString
  else if (Op = TkEquals) or (Op = TkNotEquals)
          or (Op = TkLessthan) or (Op = TkMorethan) or (Op = TkLessOrEquals)
          or (Op = TkMoreOrEquals) then
         Expr^.TypeIndex := PrimitiveTypes.PtBoolean
  else CompileError('Invalid string operator: ' + LxTokenName(Op));
  _ExBinOpStrCmp := Expr
end;

function _ExBinOpEnumCmp;
var Expr : TExpression;
begin
  Expr := _NewExpr(XcBinaryOp);
  Expr^.BinaryEx.Left := Left;
  Expr^.BinaryEx.Right := Right;
  Expr^.BinaryEx.Op := Op;
  Expr^.IsConstant := true;
  Expr^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
  if (Op = TkEquals) or (Op = TkNotEquals)
     or (Op = TkLessthan) or (Op = TkMorethan) or (Op = TkLessOrEquals)
     or (Op = TkMoreOrEquals) then
    Expr^.TypeIndex := PrimitiveTypes.PtBoolean
  else CompileError('Invalid enum operator: ' + LxTokenName(Op));
  _ExBinOpEnumCmp := Expr
end;

function _ExBinOpPtrCmp;
var Expr : TExpression;
begin
  Expr := _NewExpr(XcBinaryOp);
  Expr^.BinaryEx.Left := Left;
  Expr^.BinaryEx.Right := Right;
  Expr^.BinaryEx.Op := Op;
  Expr^.IsConstant := true;
  Expr^.IsFunctionResult := Left^.IsFunctionResult or Right^.IsFunctionResult;
  if (Op = TkEquals) or (Op = TkNotEquals) then
    Expr^.TypeIndex := PrimitiveTypes.PtBoolean
  else CompileError('Invalid string operator: ' + LxTokenName(Op));
  _ExBinOpPtrCmp := Expr
end;

function ExCoerce;
begin
  if IsCharType(Expr^.TypeIndex) and IsStringType(TypeIndex) then
    ExCoerce := ExToString(Expr)
  else if IsSameType(Expr^.TypeIndex, TypeIndex) then
         ExCoerce := Expr
  else if IsNilType(Expr^.TypeIndex) and IsPointeryType(TypeIndex) then
         ExCoerce := Expr
  else
    CompileError('Type mismatch: expected ' + TypeName(TypeIndex) +
    ', got ' + TypeName(Expr^.TypeIndex))
end;

function ExEnsureEvaluation;
var Args : TExFunctionArgs;
begin
  if Expr^.Cls = XcFunctionRef then
  begin
    if Expr^.FunctionEx.FunctionIndex^.ArgCount <> 0 then
      CompileError('Function requires arguments');
    Args.Size := 0;
    ExEnsureEvaluation := ExFunctionCall(Expr, Args)
  end
  else
    ExEnsureEvaluation := Expr
end;

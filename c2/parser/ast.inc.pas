{ Requires ../containers.inc.pas }
{ Requires ../lexer.inc.pas }

type
    TAstType = (AstIdentifier, AstProgram, AstProgramHeader);
    { Parse tree. }
    TAst = ^TAstNode;
    TAstList = ^TAstListNode;
    TCommentList = ^TCommentListNode;
    TAstNode = record
        { The type of this node. }
        AstType: TAstType;
        { The source. }
        Lexer: TLexer;
        { The location in the input. }
        Location: TBlock;
        { Children of this node. }
        Children: TAstList;
        { Comments before this node. }
        Comments: TCommentList;
    end;
    TAstListNode = record
        _ : TList;
        Value : TAst
    end;
    TCommentListNode = record
        _ : TList;
        Value : TToken
    end;

function TAst_New(const Token: TToken; AstType: TAstType): TAst;
begin
    New(Result);
    Result^.AstType := AstType;
    Result^.Lexer := Token.Lexer;
    Result^.Location := Token.Location;
    TList_New(Result^.Children);
    TList_New(Result^.Comments)
end;

procedure TAst_Dispose(var self: TAst);
var Child : TAstList;
begin
    while self^.Children <> nil do
    begin
        TList_Pop_Front(self^.Children, Child);
        TAst_Dispose(Child^.Value)
    end;
    Dispose(self);
    self := nil
end;

procedure TAst_SetEndLocation(self: TAst; const EndLoc : TBlock);
begin
    self^.Location.Size := EndLoc.Offset + EndLoc.Size - self^.Location.Offset
end;

procedure TAst_AddChild(var self: TAst; Child : TAst);
var Node : TAstList;
begin
    New(Node);
    Node^.Value := Child;
    TList_Push_Back(self^.Children, Node)
end;

procedure TAst_AddComment(var self: TAst; const Comment : TToken);
var Node : TCommentList;
begin
    New(Node);
    Node^.Value := Comment;
    TList_Push_Back(self^.Comments, Node)
end;
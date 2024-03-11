{ A list with O(1) addition. }
{ To implement, make your nodes have as their first field a pointer to the next element.
  Alternatively, you can make TListNode the first field. See ByteBufferBuilder for an example. }
type
    TList = ^TListNode;
    TListNode = record
        Next: ^TListNode;
    end;

{ Initializes a new TList. }
procedure TList_New(var self {:TList});
var List : TList absolute self;
begin
    List := nil
end;

{ Pushes an element onto the end of a TList. }
procedure TList_Push(var self {:TList}; var Elem {:^TListNode});
var
    List : TList absolute self;
    Node : TList absolute Elem;
begin
    if List = nil then Node^.Next := Node
    else
    begin
        Node^.Next := List^.Next;
        List^.Next := Node
    end;
    List := Node
end;

{ Removes the first element of the given TList.
  out_Removed: the argument where the removed element will be stored.
  This will be nil if the list is empty. }
procedure TList_Shift(var self {:TList}; var out_Removed {:^TListNode});
var
    List : TList absolute self;
    Node : TList absolute out_Removed;
begin
    if List = nil then Node := nil
    else
    begin
        Node := List^.Next;
        if List = Node then List := nil
        else List^.Next := Node^.Next
    end
end;

{ Retrieves the first element of the given TList.
  out_Node: the argument where the first element will be stored.
  This will be nil if the list is empty. }
procedure TList_First(var self {:TList}; var out_Node {:^TListNode});
var
    List : TList absolute self;
    Node : TList absolute out_Node;
begin
    if List = nil then Node := nil
    else Node := List^.Next
end;

{ Retrieves the next element of the given TList.
  out_Node: the argument that holds the current element, where the next element will be stored.
  This will be nil if the end of the list has been reached. }
procedure TList_Next(var self {:TList}; var out_Node {:^TListNode});
var
    List : TList absolute self;
    Node : TList absolute out_Node;
begin
    if (List = nil) or (Node = List) then Node := nil
    else Node := Node^.Next
end;


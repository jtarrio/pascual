{ Linked stack. }
{ Designated by a pointer to the last item in the stack, }
{ or to nil if the stack is empty. }
type 
  TStack = ^TStackPtrs;
  TStackPtrs = record
    { Link to the previous item in the stack. }
    { Points to nil if this is the first item in the stack. }
    Older : TStack;
  end;
  { A function that checks a predicate on an Item using a given Context. }
  TStackPredicate = function (var Item {: TStack};
                              var Context {: Any}) : boolean;

{ Adds the Item to the stack designated by Head. }
procedure Stack_Push(var Head, Item {:TStack});
var 
  TheHead : TStack absolute Head;
  NewItem : TStack absolute Item;
begin
  NewItem^.Older := TheHead;
  TheHead := NewItem
end;

{ Finds the first (oldest) element in the stack designated by Head and }
{ sets it in OldestItem. }
procedure Stack_GetOldest(var Head, OldestItem {:TStack});
var 
  TheHead : TStack absolute Head;
  TheItem : TStack absolute OldestItem;
begin
  TheItem := TheHead;
  if TheHead <> nil then
    while TheItem^.Older <> nil do
      TheItem := TheItem^.Older
end;

{ Finds the latest element in the stack designated by Head for which the }
{ Predicate returns true. The function returns false if no such element was }
{ found; otherwise, it returns true and the found item is set in FoundItem. }
function Stack_Find(var Head, FoundItem {:TStack};
                    Predicate : TStackPredicate;
                    var Context {:Any}) : boolean;
var 
  TheHead : TStack absolute Head;
  Item : TStack;
  TheFoundItem : TStack absolute FoundItem;
  Found : boolean;
begin
  Found := false;
  Item := TheHead;
  while not Found and (Item <> nil) do
  begin
    Found := Predicate(Item, Context);
    if Found then TheFoundItem := Item
    else Item := Item^.Older
  end;
  Result := Found
end;

{ Double linked stack. }
{ Designated by pointer to the last item in the stack, }
{ or to nil if the stack is empty. }
{ The Stack_GetOldest and Stack_Find functions also work for a TDStack. }
type 
  TDStack = ^TDStackPtrs;
  TDStackPtrs = record
    { Link to the previous item in the stack. }
    { Points to nil if this is the first item in the stack. }
    Older : TDStack;
    { Link to the next item in the stack. }
    { Points to nil item if this is the last item in the stack. }
    Newer : TDStack
  end;

{ Adds the Item to the stack designated by Head. }
procedure DStack_Push(var Head, Item {:TDStack});
var 
  TheHead : TDStack absolute Head;
  NewItem : TDStack absolute Item;
begin
  NewItem^.Older := TheHead;
  NewItem^.Newer := Nil;
  if TheHead <> nil then TheHead^.Newer := NewItem;
  TheHead := NewItem
end;

{ Linked list. }
{ Designated by a pointer to the first item in the list, or to nil }
{ if the list is empty. }
type 
  TList = ^TListPtrs;
  TListPtrs = record
    { Link to the next item in the list. }
    { Points to nil if this is the last item in the list. }
    Next : TList
  end;
  { A pointer to the end of a list. Used for adding more items in O(1). }
  TListAddPoint = ^TList;

{ Returns the add point for an existing list. }
function List_GetAddPoint(var List {: TList}) : TListAddPoint;
var 
  TheList : TList absolute List;
  Tail : TList;
begin
  if TheList = nil then Result := @TheList
  else
  begin
    Tail := TheList;
    while Tail^.Next <> nil do
      Tail := Tail^.Next;
    Result := @Tail^.Next
  end
end;

{ Adds an Item in the given AddPoint, which is updated. }
procedure List_Add(var AddPoint {: TListAddPoint}; var Item {: TList});
var 
  TheAddPoint : TListAddPoint absolute AddPoint;
  TheItem : TList absolute Item;
begin
  TheItem^.Next := nil;
  TheAddPoint^ := TheItem;
  TheAddPoint := @TheItem^.Next
end;

{ Removes the first item in the list. }
{ If the list was empty, the function returns false; otherwise it returns }
{ true and the removed item is set in DeletedItem. }
function List_Shift(var List, DeletedItem {: TList}) : boolean;
var 
  TheList : TList absolute List;
  TheDeletedItem : TList absolute DeletedItem;
begin
  Result := TheList <> nil;
  if Result then
  begin
    TheDeletedItem := TheList;
    TheList := TheList^.Next
  end
end;

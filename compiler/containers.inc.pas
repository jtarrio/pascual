{ Stack. }
{ They are designated by a "head" object which points to the last item in the }
{ stack, or to nil if the stack is empty. }
type 
  TStack = ^TStackPtrs;
  TStackPtrs = record
    { Link to the previous item in the stack. }
    { Points to nil if this is the first item in the stack. }
    Older : TStack;
    { Link to the next item in the stack. }
    { Points to nil item if this is the last item in the stack. }
    Newer : TStack
  end;
  { A function that checks a predicate on an Item using a given Context. }
  { The function can set Stop to true if it wants to stop the search. }
  TStackPredicate = function (var Item {: TStack}; var Context {: Any};
                              var Stop : boolean) : boolean;

{ Adds the Item to the stack designated by Head. }
procedure Stack_Push(var Head, Item {:TStack});
var 
  TheHead : TStack absolute Head;
  NewItem : TStack absolute Item;
begin
  NewItem^.Older := TheHead;
  NewItem^.Newer := Nil;
  if TheHead <> nil then TheHead^.Newer := NewItem;
  TheHead := NewItem
end;

{ Removes the last item from the stack designated by Head. }
{ If the stack was empty, the function returns false; otherwise, it returns }
{ true and the removed item is set in DeletedItem. }
function Stack_Pop(var Head, DeletedItem {:TStack}) : boolean;
var 
  TheHead : TStack absolute Head;
  TheDeletedItem : TStack absolute DeletedItem;
begin
  TheDeletedItem := TheHead;
  Result := TheHead <> nil;
  if Result then TheHead := TheHead^.Older
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
  Stop : boolean;
begin
  Found := false;
  Stop := false;
  Item := TheHead;
  while not Found and not Stop and (Item <> nil) do
  begin
    Found := Predicate(Item, Context, Stop);
    if Found then TheFoundItem := Item
    else if not Stop then Item := Item^.Older
  end;
  Result := Found
end;

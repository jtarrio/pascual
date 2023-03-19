{ Some examples of pointers. }
program pointers;

{ Defining some pointer types for existing types. }
type
  StringP = ^string;
  MyRecord = record
    a : integer;
    b : boolean
  end;
  MyRecordP = ^MyRecord;

{ But you can also declare a pointer for a type that hasn't been defined yet, }
{ as long as you define it in the same 'type' block. }
{ You pretty much need to do this to implement linked lists and the like. }
type
  NodeP = ^Node;
  Node = record
    Value: integer;
    Prev, Next: NodeP;
  end;

var
  MyStrPtr : StringP;
  MyRecPtr : MyRecordP;
  RootNode : NodeP;
  { Of course, you can just declare and use pointer types }
  MyIntPtr : ^integer;
  MyInt : integer;

{ Some linked list subroutines. }
procedure AddNode(Value : integer);
var PrevRoot : NodeP;
begin
  PrevRoot := RootNode;
  New(RootNode);
  RootNode^.Value := Value;
  RootNode^.Next := PrevRoot;
  { The null pointer is named 'nil' and is compatible with all pointer types }
  if PrevRoot = nil then
    RootNode^.Prev := nil
  else
  begin
    RootNode^.Prev := PrevRoot^.Prev;
    PrevRoot^.Prev := RootNode
  end
end;

function FindNode(Count : integer) : NodeP;
var Node : NodeP;
begin
  Node := RootNode;
  while (Count > 1) and (Node <> nil) do
  begin
    if Node <> nil then Node := Node^.Next;
    Count := Count - 1
  end;
  FindNode := Node
end;

procedure DisposeList;
var Next : NodeP;
begin
  while RootNode <> nil do
  begin
    Next := RootNode^.Next;
    Dispose(RootNode);
    RootNode := Next
  end
end;    

begin
  { Using a pointer to a string }
  New(MyStrPtr);
  MyStrPtr^ := 'foo';
  writeln(MyStrPtr^);
  Dispose(MyStrPtr);

  { Using a pointer to a record }
  New(MyRecPtr);
  MyRecPtr^.a := 123;
  MyRecPtr^.b := true;
  writeln(MyRecPtr^.a, ' ', MyRecPtr^.b);
  Dispose(MyRecPtr);

  { Using a pointer to an integer }
  New(MyIntPtr);
  MyIntPtr^ := 42;
  writeln(MyIntPtr^);
  Dispose(MyIntPtr);

  { Using the address-of operator }
  MyInt := 123456;
  MyIntPtr := @MyInt;
  writeln(MyIntPtr^);

  { Using a doubly linked list }
  RootNode := nil;
  AddNode(123);
  AddNode(456);
  AddNode(789);
  writeln(FindNode(2)^.Value);
  DisposeList
end.
